/*-------------------------------------------------------------------------
    ref_str.cpp
-------------------------------------------------------------------------*/

#include <math.h>

#include "orbconst.h"
#include "orbmodel.h"
#include "c_const.hpp"
#include "y_math.hpp"

TStraightReferencer::TStraightReferencer( const TIniSatParams& isp, const TNOAAImageParams& nip,
               const TCorrectionParams& cop ):
    fTotalScans( nip.fScans )
{
    // ������������� ���� maxScanAngle
    if( nip.satId == TSatInfoTable::sat_id_noaa_16 ){
        maxScanAngle = NOAA16_MAX_SCAN_ANGLE * DR;
    }
    else{
        maxScanAngle = NOAA_MAX_SCAN_ANGLE * DR;
    }

    om = new TOrbitalModel( isp, nip.fYear, nip.fYearTime, cop );
    double roll = cop.fVersion == 0 ? 0 : cop.roll;
    double pitch = cop.fVersion == 0 ? 0 : cop.pitch;
    double yaw = cop.fVersion == 0 ? 0 : cop.yaw;
    mr[0][0] = mr[1][1] = cos( roll );
    mr[1][0] = sin( roll );
    mr[0][1] = -mr[1][0];
    mr[0][2] = mr[2][0] = mr[1][2] = mr[2][1] = 0.;
    mr[2][2] = 1.;
    double mp[3][3] = {
        { cos( pitch ), 0., -sin( pitch ) },
        { 0., 1., 0. },
        { sin( pitch ), 0., cos( pitch ) } };
    double my[3][3] = {
        { 1., 0., 0. },
        { 0., cos( yaw ), -sin( yaw ) },
        { 0., sin( yaw ), cos( yaw ) } };

    for( int i=0; i<3; i++ ){
        for( int j=0; j<3; j++ ){
            double t=0.;
            for( int k=0; k<3; k++ ) t+=my[i][k]*mp[k][j];
            myp[i][j] = t;
        }
    }
    fLastScan = -1;
}


TStraightReferencer::~TStraightReferencer()
{
    delete om;
}


void TStraightReferencer::ini_xy2ll_seq( double scan )
{
    double msi[3][3];
    double v[3];
    double r[3];
    int m,n,k;

    om->model( scan/(360.*1440.) );
    v_cpy( r, om->r );
    r[0] /= Ea2; r[1] /= Ea2; r[2] /= Eb2;
    v_add( r, r, b );
    C = v_smul( r, om->r ) - 1.0;
    sin_ov = sin( om->ov );
    cos_ov = cos( om->ov );

// ���������� ������� msi.
    v_mul( om->r, -1./v_len(om->r), msi[0] );
    v_mul( om->v, 1./v_len(om->v), v );
    v_vmul( v, msi[0], msi[1] );
    v_norm( msi[1], msi[1] );
    v_vmul( msi[0], msi[1], msi[2] );
// ���������� ���� a.
    double cosa = v_smul( msi[2], v );
    double sina = sqrt( 1. - cosa*cosa );
    if( sina > 1.e-10 ){
        double d[3];
        v_vmul( msi[2], v, d );
        if( v_smul( d, msi[1] ) < 0. ) sina = -sina;
    }
    else{
        sina = 0.; cosa = 1.;
    }
    double mal[3][3] = {
        { cosa, 0., sina },
        { 0., 1., 0. },
        { -sina, 0., cosa } };
// ���������� ������� ma, maT = msi * myp * malT * mr * ma.
    double mm1[3][3];
    for(m=0;m<3;m++){
        for(n=0;n<3;n++){
            double t=0.;
            for(k=0;k<3;k++) t+=mr[m][k]*mal[k][n];
            mm1[m][n] = t;
        }
    }
    double mm2[3][3];
    for(m=0;m<3;m++){
        for(n=0;n<3;n++){
            double t=0.;
            for(k=0;k<3;k++) t += mal[k][m]*mm1[k][n];
            mm2[m][n] = t;
        }
    }
    for(m=0;m<3;m++){
        for(n=0;n<3;n++){
            double t = 0.;
            for(k=0;k<3;k++) t += myp[m][k]*mm2[k][n];
            mm1[m][n]=t;
        }
    }
    for(m=0;m<3;m++){
        for(n=0;n<2;n++){
            double t=0.;
            for(k=0;k<3;k++) t+=msi[k][m]*mm1[k][n];
            ma[m][n] = t;
        }
    }
}


void TStraightReferencer::xy2ll_seq( double x, double *lat, double *lon )
{
    double R, A, B;
    double d[3];
    double e[3], ecf[2];
    double theta = (1024. - x)*(maxScanAngle/1023.5);

    double dpqs[2] = { cos(theta), -sin(theta) };
    for(int i=0;i<3;i++) d[i] = ma[i][0]*dpqs[0] + ma[i][1]*dpqs[1];

// ������� ����������� ��������� ��� R.
    A = (d[0]*d[0] + d[1]*d[1])/Ea2 + d[2]*d[2]/Eb2;
    B = v_smul( b, d );
    R = -(B + sqrt( B*B - 4.0*A*C ))/(A+A);

// ������-������ ����� ����������� � ����������� �.�.
    v_mul( d, R, d );
    v_add( om->r, d, e );
// ������� � �������, ��������� � ��������� �����.
    ecf[0] = e[0]*cos_ov + e[1]*sin_ov;
    ecf[1] = e[1]*cos_ov - e[0]*sin_ov;
// ������� ������������� ������ � �������.
    *lat = atan2( e[2], (1.0-Ee2)*sqrt(ecf[0]*ecf[0]+ecf[1]*ecf[1]) );
    *lon = atan2( ecf[1], ecf[0] );
}


void TStraightReferencer::xy2ll( double x, double y, double *lon, double *lat )
{
    if( fLastScan != y ){ fLastScan = y; ini_xy2ll_seq( fLastScan ); }
    xy2ll_seq( x, lat, lon );
}


void TStraightReferencer::xy2ll( int x, int y, double *lon, double *lat )
{
    xy2ll( double(x)+.5, double(y)+.5, lon, lat );
}

