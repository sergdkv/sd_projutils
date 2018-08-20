/*-------------------------------------------------------------------------
   orbmodel.h
-------------------------------------------------------------------------*/
#ifndef _ORBMODEL_H_
#define _ORBMODEL_H_

#include <orbconst.h>
#include <c_except.h>
#include <c_sattbl.h>

//#include <c_lib.hpp>

#include <c_types.hpp>
#include <c_b0.hpp>
#include <c_b0noaa.hpp>
#include <c_b0proj.hpp>


/*-------------------------------------------------------------------------
    TCorrectionParams
    ����� ��� ������������� ������ ���������� ��������� ��������������
    ��������.
-------------------------------------------------------------------------*/
class TCorrectionParams {
public:

    TCorrectionParams( const TBlk0 & blk0 );

    TCorrectionParams( ulong ver = 0,
        double t = 0, double tbus_t = 0,
        double roll_angle = 0, double pitch_angle = 0, double yaw_angle = 0 );

    friend int operator==( const TCorrectionParams &c1, const TCorrectionParams &c2 );
    friend int operator!=( const TCorrectionParams &c1, const TCorrectionParams &c2 );

// 0 - ���������� ���������
// 1 - ��������� ��������� � �������������� ����������� ������ SGP8
// 2 - ��������� ��������� � �������������� ����������� ������ SGP4
    ulong fVersion;

// ��������� ������� ������ ������
    double fTime;       // � ��������.
    double fTBUSTime;   // � ��������.
// ��������� ���������� ��������� ��������, � ��������
    double roll;
    double pitch;
    double yaw;
};


//--------------------------------------------------------------------------
//  TIniSatParams
//  ���������� ����������� ���������.
//--------------------------------------------------------------------------
class TIniSatParams {
public:
    TIniSatParams( const TBlk0 & );
    TIniSatParams( const TOldBlk0 & );
//--------------------------------------------------------------------------
//  ������������� ������� ���������� TLE, ��� ����������� � �����. �����
//  ������ ��������� ������ ����� ����������, ��� ����� �������� ����� ���.
//--------------------------------------------------------------------------
    TIniSatParams( const char * );
//--------------------------------------------------------------------------
//  ������������� �� ����� ��������� TLE, ����������� �� E-Mail.
//  TIniSatParams( const char * fileName, const char *satName )
//  ���������:
//  satName     ������ ��� ��������. ��������, "NOAA 12".
//              ������������ ������ - 32 �������, ������� '\0'.
//  ����������:
//  TAccessExc( 1, "������ ������� � ����� TLE" )
//  TRequestExc( 1, "���� TLE �� �������� ���������� ��� ������� ��������" )
//-------------------------------------------------------------------------
    TIniSatParams( const char *, const char * ) throw ( TAccessExc, TRequestExc );

    unsigned fRevNum;
    unsigned fSetNum;
    int    fEpochYear;
    double fEpochYearTime;  // ������ ����� � ����, ���������� � ���� (0-based).
    double n0;       // Mean motion.
    double bstar;    // BSTAR drag term.
    double i0;       // Inclination.
    double raan;     // Right ascension of ascending node.
    double e0;       // Eccentricity.
    double w0;       // Argument of perigee.
    double m0;       // Mean anomaly.
};


/*-------------------------------------------------------------------------
    TNOAAImageParams
-------------------------------------------------------------------------*/
class TNOAAImageParams {
public:
    TNOAAImageParams( const TBlk0 & );

    TSatInfoTable::NoradSatId   satId;      // ������������� �������� NORAD
    ulong   fYear;          // ������ ���.
    double  fYearTime;      // ������ ����� � ����, ���������� � ���� (0-based).
    ulong   fScans;         // ����� ������ � ������.
    int     fAscendFlag;    // false - ����������, true - ����������
};


/*-------------------------------------------------------------------------
    class TOrbitalModel
-------------------------------------------------------------------------*/
class TOrbitalModel {
public:
//--------------------------------------------------------------------------
//  ������������ ������������� ����������, ��� �� ��������� �� �������, ���
//  ��������� ������ �� �������� ������� ������� ( baseYear � baseYearTime ).
//  ���������, ����������� ��� ����������� ������� �������, ��������
//  ���������������. ��� �� ���������� ���������� ��������������� ��������
//  model.
//  ���������:
//  baseYear        ���, ������� ���: 1997, � �� 97.
//  baseYearTime    ����� � ����, ���������� � ���� (0-based).
//  �������� !!! ��������� baseYear � baseYearTime ������ �������� �����,
//  ��� ����� ����� ������� � ��� �������� �������, ��� ������� � ����������
//  ����� ���������� ������� model.
//--------------------------------------------------------------------------
    TOrbitalModel( const TIniSatParams&, int baseYear, double baseYearTime,
         const TCorrectionParams& cop = TCorrectionParams() );

// ����������� ����������� ��������� �� ������ �������, �������� ������������ ��������.
// ���������:
// rel_time      ������������� ����� � ���� ( ����� ���� ������������� ).
    void model( double rel_time );

    int     fBaseYear;
    double  fBaseYearTime;
    double  fRelTime; // ������ �������, �� ������� ������ ���������� ����������� ������. �������� ������������ fBaseYear+fBaseYearTime � ����.

//-------------------------------------------------------------------------
// ���������, ����������� ��� ����������� ������� �������.
//-------------------------------------------------------------------------
    double r[3];    // ������-������ � ����������� �.�.
    double v[3];    // �������� � ����������� �.�.
    double m;       // ������� ��������.
    double E;       // ��������������� ��������.
    double f;       // �������� ��������.
    double ov;      // ���� �� ����.
    double om;      // ���������� ����.
    double e;       // Eccentricity.
    double n;       // ������� ��������.
    double w;       // �������� �������.
    double R;       // ���������� �� ������ ����� �� ���
    double i;       // ���������� ������
    double a;       // ������� ������� ������

//-------------------------------------------------------------------------
//  ���������, ���������������� �������������.
//-------------------------------------------------------------------------
    // SGP4, SGP8
    TIniSatParams isp;
    double ov0;     // ���� �� ����
    double n0;      // Original mean motion.
    double a0;      // Semimajor axis.

    // SGP8
    double dn;      // ����������� �� ������� �������� ��������.
    double de;      // ����������� �� ������� ���������������.
    double omf, oms;
    double wf, ws;
    double mf, ms;

    // SGP4
    double c1, c2, c3, c4, c5;
    double d2, d3, d4;
    double ksi, beta, etta, teta;
    double dmdt;
    double dwdt;
    double domdt;

private:

    enum OrbitalModel { SGP4, SGP8 };

/*
    ���������������� ������������� � ����������� �� �������� ���� fVersion ��������� cop.
    �� ��������� (fVersion == 0) ������������ ������ sgp8.
*/
    OrbitalModel fModel;

    void ini_sgp4();
    void ini_sgp8();

    void sgp4( double rel_time );
    void sgp8( double rel_time );

    static double ovenAngle( int year, double year_time );
};



//-------------------------------------------------------------------------
//  TInverseReferencer
//-------------------------------------------------------------------------
struct TStoreElement;
class TInverseReferencer {
public:
    TInverseReferencer( const TIniSatParams&, const TNOAAImageParams&,
        const TCorrectionParams& corParams = TCorrectionParams() );
    ~TInverseReferencer();

//--------------------------------------------------------------------------
//  ll2xy
//  ������� ������ ������ ����������� ��������� (x,y) ������� ������
//  AVHRR, ���������������� �������������� ����� � ������������ (lon,lat),
//  �.�. �������� ������ �������������� ��������.
//  ���������:
//  lon, lat    ������ �������������� ���������� �����.
//              -PI <= lon <= PI
//              -PI/2 <= lat <= PI/2
//  x, y        ��������� �� ���������� ��� ���������.
//  ������������ ��������:
//  0               ����� � ���������� ������������ �� ����������� ������.
//  1               �������������� (lon,lat)->(x,y) ��������� �������.
//--------------------------------------------------------------------------
    int ll2xy( double lon, double lat, double *x, double *y );
    int ll2xy( double lon, double lat, int *x, int *y );

private:
    void ini_ll2xy( const TIniSatParams&, const TNOAAImageParams&, const TCorrectionParams& );

    ulong fTotalScans;
    double fOven0;
    double ER, VR;
    double gc_lat, sin_lat, cos_lat, prev_lat;
    TStoreElement *se;

    double maxScanAngle;    // ������������ ���� ������������, ���������� � �������� (����� �������� �������)
};


//-------------------------------------------------------------------------
//  TStraightReferencer
//-------------------------------------------------------------------------
class TStraightReferencer{
public:
    TStraightReferencer( const TIniSatParams &, const TNOAAImageParams &,
        const TCorrectionParams& corParams = TCorrectionParams() );
    ~TStraightReferencer();

//-------------------------------------------------------------------------
//  ������� ������ ������ ����������� ��������� (lon,lat) ��������������
//  �����, ��������������� ������� ������ AVHRR � ������������ (x,y), �.�.
//  ������ ������ �������������� ��������.
//  ���������:
//  x               ������� ����� ������.
//  y               ������ ����� ������.
//  lat, lon        ����� ��� ��������� ����� ���������� ��������������
//                  ����������, ��������������� ����� ������.
//                  -PI < lon <= PI
//                  -PI/2 <= lat <= PI/2
//  ������������� ������ ������� ������ ������ ������ �������������� ��������
//  ��� ������ ������� � ���������� ������������.
//-------------------------------------------------------------------------
    void xy2ll( double x, double y, double *lon, double *lat );
    void xy2ll( int x, int y, double *lon, double *lat );

private:
    void ini_xy2ll_seq( double );
    void xy2ll_seq( double, double *, double * );

    ulong fTotalScans;
    double fLastScan;
    double sin_ov, cos_ov;
    double myp[3][3];
    double mr[3][3];
    double ma[3][2]; // ������� �������������� ���������, �������� � �.�. �������, � ���������� � ����������� ��������������� �.�. ������ ������� �� ������������.
    double C;
    double b[3];
    TOrbitalModel *om;

    double maxScanAngle;    // ������������ ���� ������������, ���������� � �������� (����� �������� �������)
};


#endif
