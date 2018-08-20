 /*-------------------------------------------------------------------------
    gms.hpp
-------------------------------------------------------------------------*/
#ifndef _GMS_HPP_
#define _GMS_HPP_


#include <c_lib.hpp>

#pragma pack(1)

/*-------------------------------------------------------------------------
    TGMSDocSector
    Структура DOC-сектора кадра GMS-5.
-------------------------------------------------------------------------*/
struct TGMSDocSector {
    char id[2];
    char cond[126];
    char mapping1[64];
    char scid[4];
    char mapping2[100];
    char orbit[128];
    char manam[410];
    char calibr[256];
    char spare[1203];
};


/*-------------------------------------------------------------------------
    Структуры, соответствующие различным частям DOC-сектора.
-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
    TGMSCond
-------------------------------------------------------------------------*/
struct TGMSCond {
    uchar scan_mode;
    uchar scan_status;
    uchar frame_flag;
    uchar picture_flag;
    ulong pfs_line_number;
    ulong pfr_line_number;
    ulong scan_count;
    ulong west_horizon_point;
    ulong east_horizon_point;
    uchar sync_lock_qd;
    ulong bit_error_count;

    long year;
    long month;
    long day;
    long hour;
    long minute;
    long second;
    long tic;

    ulong calibr_id;
    ulong manam_rev_number;
    uchar data_source;
};


/*-------------------------------------------------------------------------
    TGMSMapping1
-------------------------------------------------------------------------*/
struct TGMSMapping1 {
    double earth_radius;
    double sat_elevation;
    double stepping_angle;
    double sampling_angle;
    double ssp_lat;
    double ssp_lon;
    long ssp_ir1_line;
    long ssp_ir1_pixel;
    double ratio;
    double x1;
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
};


/*-------------------------------------------------------------------------
    TGMSOrbitAttitude
-------------------------------------------------------------------------*/
struct TGMSOrbitAttitude {
    double start_time;          // observation start time (MJD)
    double vis_stepping_angle;
    double ir_stepping_angle;
    double vis_sampling_angle;
    double ir_sampling_angle;
    double vis_center_line;
    double ir1_center_line;
    double vis_center_pixel;
    double ir1_center_pixel;
    long vis_sens_number;
    long ir_sens_number;
    long vis_total_lines;
    long ir_total_lines;
    long vis_pixel_number;
    long ir_pixel_number;
    double mis_angles[3];       // VISSR misalignment angles
    double mis_matrix[3][3];    // VISSR misalignment matrix
    double ir2_center_line;
    double ir3_center_line;
    double ir2_center_pixel;
    double ir3_center_pixel;
    // constants
    double pi;
    double dr;                  // pi/180
    double rd;                  // 180/pi
    double erad;                // equatorial radius of the Earth
    double eobl;                // oblateness of the Earth
    double eecc;                // eccentricity of the Earth
    double vissr_sun_angle;     // angle between the VISSR and the view
                                // direction of the Sun sensor
    // orbital parameters
    double o_time;              // epoch time of orbital parameters
    double o_a;                 // semi-major axis (km)
    double o_e;                 // eccentricity
    double o_i;                 // inclination
    double o_raan;              // longitude of ascending node
    double o_w;                 // argument of perigee
    double o_m;                 // mean anomaly
    double o_ssp_lon;           // sub-satellite east longitude
    double o_ssp_lat;           // sub-satellite north latitude
    // attitude parameters
    double a_time;              // epoch time of attitude parameters
    double a_a;                 // angle between Z-axis and satellite spin
                                // axis projected on yz-plane
    double a_da;                // change-rate of a (rad/sec)
    double a_d;              // angle between satellite spin axis and yz-plane
    double a_dd;                // change-rate of d (rad/sec)
    double a_mean_spin_rate;    // daily mean of satellite spin rate (rpm)
    //
    double first_att_time; // time of the first attitude prediction data (MJD)
    double last_att_time;  // time of the last attitude prediction data (MJD)
    double att_int_time;   // interval time of attitude prediction data (MJD)
    long att_number;       // number of attitude prediction data
    //
    double first_orb_time; // time of the first orbit prediction data (MJD)
    double last_orb_time;  // time of the last orbit prediction data (MJD)
    double orb_int_time;   // interval time of orbit prediction data (MJD)
    long orb_number;       // number of orbit prediction data
};


/*-------------------------------------------------------------------------
    TGMSAttitude
-------------------------------------------------------------------------*/
struct TGMSAttitude {
    double p_time;  // prediction time (UTC represented in MJD)
    // anno domini
    long ad_year;
    long ad_month;
    long ad_day;
    long ad_hour;
    long ad_minute;
    long ad_second;
    //
    double a_angle;         // angle between z-axis and satellite spin axis
                            // projected on yz-plane
    double d_angle;         // angle between satellite spin axis and yz-plane
    double sun_earth_angle; // dihedrial angle between the Sun and the Earth
                            // measured clockwise seeing from North
    double spin_rate;       // spin speed of satellite (rpm)
    double spin_raan;       // right ascension of satellite spin axis in the
                            // satellite orbit plane coordinate system
    double dcl;             // declination
};


/*-------------------------------------------------------------------------
    TGMSOrbit
-------------------------------------------------------------------------*/
struct TGMSOrbit {
    double p_time;      // prediction time (UTC represented in MJD)
    // anno domini
    long ad_year;
    long ad_month;
    long ad_day;
    long ad_hour;
    long ad_minute;
    long ad_second;
    //
    double r1950[3];
    double v1950[3];
    double r[3];
    double v[3];
    double gst;         // greenwich sidereal time
    double rasun1950;   // right ascension from the satellite to the Sun
                        // in mean of 1950.0 coordinates
    double dcl1950;     // declination from the satellite to the Sun
                        // in mean of 1950.0 coordinates
    double rasun;       // right ascension from the satellite to the Sun
                        // in the Earth-fixed coordinates
    double dcl;         // declination from the satellite to the Sun
                        // in the Earth-fixed coordinates
    double npm[3][3];   // nutation and precession matrix
    double ssp_lat;
    double ssp_lon;
    double height;      // height of the satellite above the Earth surface
};


#pragma pack()


/*-----------------------------------------------------------------------------
    LinePixel
-----------------------------------------------------------------------------*/
struct LinePixel {
    long line;
    long pixel;
};


/*-----------------------------------------------------------------------------
    Функции инициализации структур, соответствующих различным частям сектора
    DOC.
-----------------------------------------------------------------------------*/

/*
    buf    Указатель на буефер размером 126 байт с данными блока "Spacecraft
           and CDAS status block".
*/
void _Export gms_ini_cond( TGMSCond * c, uchar * buf );
void _Export gms_ini_mapping1( TGMSMapping1 * map, uchar * buf );
void _Export gms_ini_mapping2( LinePixel * map, uchar * buf );

/*
    buf    Указатель на буфер размером 128 байт с данными блока "Orbit and
           attitude".
*/
void _Export gms_ini_orbit_attitude( TGMSOrbitAttitude * oa, TGMSAttitude attitude[10], TGMSOrbit orbit[8], const uchar * buf );

/*
    buf   Буфер размером 6400 байт с данными Calibration data block (Table A.7)
*/
void _Export gms_ini_clb( double vis[4*64], double ir[3*256], uchar * buf );


/*-----------------------------------------------------------------------------
    Функции печати полей DOC-структур.
-----------------------------------------------------------------------------*/
void _Export gms_print_cond( FILE * f, const TGMSCond * c );
void _Export gms_print_orbit( FILE * f, const TGMSOrbit * o );
void _Export gms_print_attitude( FILE * f, const TGMSAttitude * a );
void _Export gms_print_oa( FILE * f, const TGMSOrbitAttitude * oa );
void _Export gms_print_mapping1( FILE * f, const TGMSMapping1 * map );
void _Export gms_print_mapping2( FILE * f, const LinePixel * map );

/*-----------------------------------------------------------------------------
    Функции преобразования данных в обычный формат.
-----------------------------------------------------------------------------*/
uchar _Export extract_bcd( const uchar * s );
long _Export extract_long2( const uchar * s );
long _Export extract_long4( const uchar * s );
double _Export extract_double4( const uchar * s, int m );
double _Export extract_double6( const uchar * s, int m );


/*-----------------------------------------------------------------------------
    Печать полей 0-блока.
-----------------------------------------------------------------------------*/
void _Export tblk0_print_vissrdcs( FILE *, TBlk0_VISSRDCS * );


/*-----------------------------------------------------------------------------
    class TGMSDataUnpacker
-----------------------------------------------------------------------------*/
class _Export TGMSDataUnpacker {
public:
    TGMSDataUnpacker( const char * file_name ) throw ( TAccessExc );

    ~TGMSDataUnpacker();

    long totalFrames() const { return fTotalFrames; }

/*
    Возвращает номер кадра (0-based), который будет получен следующим вызовом функции unpackNextFrame.
*/
    long currentFrameNumber() const { return fCurFrameNum; }

/*
    Параметры:
    frame_num    0..totalFrames-1

    Возвращаемое значение:
    >=0     ok, номер строки на которую спозиционировались
    -1      ошибка
*/
    void setCurrentFrameNumber( long frame_num ) throw ( TAccessExc, TParamExc );

/*
    vis     9164*4
    ir      2291
    doc     2293

    Эти указатели могут быть равны 0. Для ir это верно как для его элементов, для и для него самого.
*/
    void unpackNextFrame( uchar * vis, uchar * ir[3], uchar * doc ) throw ( TAccessExc, TRequestExc );

private:
    FILE * fFile;

    long fTotalFrames;  // Число малых кадров в файле данных.
    uchar * fBuf;   // буфер для упакованного скана VISSR
    long fCurFrameNum; // Номер кадра, который будет получен следующим вызовом функции unpackNextFrame.

};



#endif
