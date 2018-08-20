/*-------------------------------------------------------------------------
   orbmodel.hpp
-------------------------------------------------------------------------*/
#ifndef _ORBMODEL_HPP_
#define _ORBMODEL_HPP_


#include <c_lib.hpp>
#include <sattable.hpp>


/*-------------------------------------------------------------------------
    TCorrectionParams
    ����� ��� �।�⠢����� ����� ��ࠬ��஢ ���४樨 �������᪮�
    �ਢ離�.
-------------------------------------------------------------------------*/
class _Export TCorrectionParams {
public:

    TCorrectionParams( const TBlk0 & blk0 );

    TCorrectionParams( ulong ver = 0,
        double t = 0, double tbus_t = 0,
        double roll_angle = 0, double pitch_angle = 0, double yaw_angle = 0 );

    friend int operator==( const TCorrectionParams &c1, const TCorrectionParams &c2 );
    friend int operator!=( const TCorrectionParams &c1, const TCorrectionParams &c2 );

// 0 - ������⢨� ���४樨
// 1 - ���४�� ���᫥�� � �ᯮ�짮������ �ࡨ⠫쭮� ������ SGP8
// 2 - ���४�� ���᫥�� � �ᯮ�짮������ �ࡨ⠫쭮� ������ SGP4
    ulong fVersion;

// ���४�� �६��� ��砫� ᥠ��
    double fTime;       // � ᥪ㭤��.
    double fTBUSTime;   // � ᥪ㭤��.
// ���४�� �ਥ��樨 ������� ��⭨��, � ࠤ�����
    double roll;
    double pitch;
    double yaw;
};


//--------------------------------------------------------------------------
//  TIniSatParams
//  �ண����� �ࡨ⠫�� ��ࠬ����.
//--------------------------------------------------------------------------
class _Export TIniSatParams {
public:
    TIniSatParams( const TBlk0 & );
    TIniSatParams( const TOldBlk0 & );
//--------------------------------------------------------------------------
//  ���樠������ ⥪�⮬ ⥫��ࠬ�� TLE, 㦥 ���⠭�� � ����. ����
//  ������ ᮤ�ঠ�� ⮫쪮 ⥪�� ⥫��ࠬ��, ��� ����� ��⭨�� ��। ���.
//--------------------------------------------------------------------------
    TIniSatParams( const char * );
//--------------------------------------------------------------------------
//  ���樠������ �� 䠩�� ⥫��ࠬ� TLE, ����砥���� �� E-Mail.
//  TIniSatParams( const char * fileName, const char *satName )
//  ��ࠬ����:
//  satName     ������ ��� ��⭨��. ���ਬ��, "NOAA 12".
//              ���ᨬ���� ࠧ��� - 32 ᨬ����, ������ '\0'.
//  �᪫�祭��:
//  TAccessExc( 1, "�訡�� ����㯠 � 䠩�� TLE" )
//  TRequestExc( 1, "���� TLE �� ᮤ�ন� ⥫��ࠬ�� ��� ������� ��⭨��" )
//-------------------------------------------------------------------------
    TIniSatParams( const char *, const char * ) throw ( TAccessExc, TRequestExc );

    unsigned fRevNum;
    unsigned fSetNum;
    int    fEpochYear;
    double fEpochYearTime;  // ������ �६� � ����, ��ࠦ����� � ���� (0-based).
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
class _Export TNOAAImageParams {
public:
    TNOAAImageParams( const TBlk0 & );

    TSatInfoTable::NoradSatId   satId;      // �����䨪��� ��⭨�� NORAD
    ulong   fYear;          // ����� ���.
    double  fYearTime;      // ������ �६� � ����, ��ࠦ����� � ���� (0-based).
    ulong   fScans;         // ��᫮ ᪠��� � ᭨���.
    int     fAscendFlag;    // false - ���室�騩, true - ���室�騩
};


/*-------------------------------------------------------------------------
    class TOrbitalModel
-------------------------------------------------------------------------*/
class _Export TOrbitalModel {
public:
//--------------------------------------------------------------------------
//  �ந�������� ���樠������ ��६�����, ��� �� �������� �� �६���, ���
//  �������� ⮫쪮 �� �������� ������ �६��� ( baseYear � baseYearTime ).
//  ��ࠬ����, ����塞� ��� �����⭮�� ������ �६���, �������
//  ����।�����묨. ��� �� ���᫥��� ����室��� ��ᯮ�짮������ �㭪樥�
//  model.
//  ��ࠬ����:
//  baseYear        ���, ������ ���: 1997, � �� 97.
//  baseYearTime    �६� � ����, ��ࠦ����� � ���� (0-based).
//  �������� !!! ��ࠬ���� baseYear � baseYearTime ������ �������� �६�,
//  ��� ����� ����� ����饥 � ⥬ �����⠬ �६���, ��� ������ � ���쭥�襬
//  �㤥� ��뢠���� �㭪�� model.
//--------------------------------------------------------------------------
    TOrbitalModel( const TIniSatParams&, int baseYear, double baseYearTime,
         const TCorrectionParams& cop = TCorrectionParams() );

// �����뢠�� �ࡨ⠫�� ��ࠬ���� �� ������ �६���, ������� �⭮�⥫쭮 ��������.
// ��ࠬ����:
// rel_time      �⭮�⥫쭮� �६� � ���� ( ����� ���� ����⥫�� ).
    void model( double rel_time );

    int     fBaseYear;
    double  fBaseYearTime;
    double  fRelTime; // ������ �६���, �� ����� ᥩ�� ����⠭� �ࡨ⠫쭠� ������. ��ࠦ��� �⭮�⥫쭮 fBaseYear+fBaseYearTime � ����.

//-------------------------------------------------------------------------
// ��ࠬ����, ����塞� ��� �����⭮�� ������ �६���.
//-------------------------------------------------------------------------
    double r[3];    // ������-����� � ����������� �.�.
    double v[3];    // ������� � ����������� �.�.
    double m;       // �।��� ��������.
    double E;       // ���業���᪠� ��������.
    double f;       // ��⨭��� ��������.
    double ov;      // ���� �� ����.
    double om;      // ���室�騩 㧥�.
    double e;       // Eccentricity.
    double n;       // �।��� ��������.
    double w;       // ��㬥�� ��ਣ��.
    double R;       // ����ﭨ� �� 業�� ����� �� ���
    double i;       // ���������� �ࡨ��
    double a;       // ������ ������ �ࡨ��

//-------------------------------------------------------------------------
//  ��ࠬ����, ���樠�����㥬� ��������஬.
//-------------------------------------------------------------------------
    // SGP4, SGP8
    TIniSatParams isp;
    double ov0;     // 㣮� �� ����
    double n0;      // Original mean motion.
    double a0;      // Semimajor axis.

    // SGP8
    double dn;      // �ந������� �� �६��� �।���� ��������.
    double de;      // �ந������� �� �६��� ��業�����.
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
    ���樠��������� ��������஬ � ����ᨬ��� �� ���祭�� ���� fVersion ��ࠬ��� cop.
    �� 㬮�砭�� (fVersion == 0) �ᯮ������ ������ sgp8.
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
class _Export TInverseReferencer {
public:
    TInverseReferencer( const TIniSatParams&, const TNOAAImageParams&,
        const TCorrectionParams& corParams = TCorrectionParams() );
    ~TInverseReferencer();

//--------------------------------------------------------------------------
//  ll2xy
//  �㭪�� �蠥� ������ ��।������ ���न��� (x,y) ���ᥫ� ᭨���
//  AVHRR, ᮮ⢥�����饣� �������᪮� �窥 � ���न��⠬� (lon,lat),
//  �.�. ������ ������ �������᪮� �ਢ離�.
//  ��ࠬ����:
//  lon, lat    ������ �������᪨� ���न���� �窨.
//              -PI <= lon <= PI
//              -PI/2 <= lat <= PI/2
//  x, y        �����⥫� �� ��६���� ��� १����.
//  �����頥��� ���祭��:
//  0               ��窠 � 㪠����묨 ���न��⠬� �� �ਭ������� ᭨���.
//  1               �८�ࠧ������ (lon,lat)->(x,y) �஢����� �ᯥ譮.
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

    double maxScanAngle;    // ���ᨬ���� 㣮� ᪠��஢����, ��ࠦ���� � ࠤ����� (業�� �ࠩ���� ���ᥫ�)
};


//-------------------------------------------------------------------------
//  TStraightReferencer
//-------------------------------------------------------------------------
class _Export TStraightReferencer{
public:
    TStraightReferencer( const TIniSatParams &, const TNOAAImageParams &,
        const TCorrectionParams& corParams = TCorrectionParams() );
    ~TStraightReferencer();

//-------------------------------------------------------------------------
//  �㭪�� �蠥� ������ ��।������ ���न��� (lon,lat) �������᪮�
//  �窨, ᮮ⢥�����饩 ���ᥫ� ᭨��� AVHRR � ���न��⠬� (x,y), �.�.
//  ����� ������ �������᪮� �ਢ離�.
//  ��ࠬ����:
//  x               �⮫��� �窨 ᭨���.
//  y               ��ப� �窨 ᭨���.
//  lat, lon        ��१ �� ��ࠬ���� �㤥� �����饭� �������᪨�
//                  ���न����, ᮮ⢥�����騥 �窥 ᭨���.
//                  -PI < lon <= PI
//                  -PI/2 <= lat <= PI/2
//  �����᫥���� ����� �㭪樨 �蠥� ����� ������ �������᪮� �ਢ離�
//  ��� 業�� ���ᥫ� � 㪠����묨 ���न��⠬�.
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
    double ma[3][2]; // ����� �८�ࠧ������ ���न���, �������� � �.�. ᪠���, � ���न���� � ����������� ���業���᪮� �.�. ��⨩ �⮫��� �� �ᯮ������.
    double C, b[3];
    TOrbitalModel *om;

    double maxScanAngle;    // ���ᨬ���� 㣮� ᪠��஢����, ��ࠦ���� � ࠤ����� (業�� �ࠩ���� ���ᥫ�)
};


#endif
