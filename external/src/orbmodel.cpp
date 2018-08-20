/*-------------------------------------------------------------------------
    orbmodel.cpp
-------------------------------------------------------------------------*/
// #include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c_const.hpp"

#include "orbmodel.h"


static double econv( const char *ps )
{
    char t[20];
    char *pt;

// Пропускаем пробелы. После этого ps указывает на первый значимый символ.
    while( *ps == ' ' ) ps++;
// Формирование мантиссы.
    pt = t;
    *pt++ = '.';
    while( *ps != '+' && *ps != '-' ) *pt++ = *ps++;
// Формирование порядка.
    *pt++ = 'e';
    *pt++ = *ps++;
    while( *ps != ' ' ) *pt++ = *ps++;
    *pt = '\0';

    return atof( t );
}


TNOAAImageParams::TNOAAImageParams( const TBlk0 &b )
{
    // заполнение поля satId
        if( strcmp( b.b0.satName, "NOAA" ) == 0 && (b.b0.satId & 0xffff) == 0 ){  // работа с файлами формата 1999 года
                satId =
                        b.b0.satId == 0x090000 ? TSatInfoTable::sat_id_noaa_9 :
                        b.b0.satId == 0x0a0000 ? TSatInfoTable::sat_id_noaa_10 :
                        b.b0.satId == 0x0b0000 ? TSatInfoTable::sat_id_noaa_11 :
                        b.b0.satId == 0x0c0000 ? TSatInfoTable::sat_id_noaa_12 :
                        b.b0.satId == 0x0e0000 ? TSatInfoTable::sat_id_noaa_14 : TSatInfoTable::sat_id_noaa_15;
    }
    else{
        satId = (TSatInfoTable::NoradSatId)b.b0.satId;
    }

    fYear = b.b0.year;
    fYearTime = double( b.b0.day - 1 ) + double( b.b0.dayTime ) / 86400000.;

    if( b.b0.dataType1 == 1 ){     // Исходные данные HRPT.
        TBlk0_HRPT *p = (TBlk0_HRPT *)(&b);
        fScans = p->frameNum + p->lostFrameNum;
        fAscendFlag = p->ascendFlag;
    }
    else if( b.b0.dataType1 == 2 ){     // Данные одного канала AVHRR.
        TBlk0_AVHRR *p = (TBlk0_AVHRR *)(&b);
        fScans = p->totalFrameNum;
        fAscendFlag = p->ascendFlag;
    }
    else if( b.b0.dataType1 == 3 ){     // Проекция.
        fScans = 5000; // Так как в проекции не содержится число сканов снимка, по которому она была сформирована, инициализирую максимально возможным числом.
        TIniSatParams isp( b );
        TOrbitalModel om( isp, fYear, fYearTime );
        om.model( 0. );
        fAscendFlag = (om.v[2] > 0);
    }
}


TIniSatParams::TIniSatParams( const TOldBlk0& b ) :
    fRevNum( b.nRevnum ),
    fSetNum( b.nElnum ),
    fEpochYear( int(b.nYear) + 1900 ),
    fEpochYearTime( double(b.nDay) + b.nDayfrac - 1 ),
    n0( b.nN0 ),
    bstar( b.nBSTAR ),
    i0( b.nI0 ),
    raan( b.nRAAN ),
    e0( b.nE0 ),
    w0( b.nW0 ),
    m0( b.nM0 )
{
}


TIniSatParams::TIniSatParams( const TBlk0 & b )
{
// Структура орбитальных параметров NORAD находится в 0-блоке по постоянному смещению, поэтому для файлов любого типа можно использовать одинаковый код.
    TBlk0_HRPT * p = (TBlk0_HRPT *)(&b);
    fRevNum = p->revNum;
    fSetNum = p->setNum;
    fEpochYear = p->year;
    fEpochYearTime = p->yearTime - 1.;
    n0 = p->n0;
    bstar = p->bstar;
    i0 = p->i0;
    raan = p->raan;
    e0 = p->e0;
    w0 = p->w0;
    m0 = p->m0;
}


/*
void TIniSatParams::upToTime( double relTime )
{
    TOrbitalModel om( *this, fEpochYear, fEpochYearTime );
    om.calculate( relTime );
    raan = om.raan;
    e0 = om.e;
    w0 = om.w;
    m0 = om.m;
    n0 = calc_NORAD_n0( om );
// Пересчет fEpochYear и fEpochYearTime.
    fEpochYearTime += relTime;
    if( relTime > 0 ){
    if( isLeapYear( fEpochYear ) ){
        if( fEpochYearTime >= 366 ){ fEpochYearTime -= 366; fEpochYear++; }
    }
    else{
        if( fEpochYearTime >= 365 ){ fEpochYearTime -= 365; fEpochYear++; }
    }
    }
    else{
        if( isLeapYear( fEpochYear - 1 ) ){
            if( fEpochYearTime < 0 ){ fEpochYearTime += 366; fEpochYear--; }
        }
        else{
            if( fEpochYearTime < 0 ){ fEpochYearTime += 365; fEpochYear--; }
        }
    }
}


//  Вычисление n0 из телеграммы TLE по "истинному n0", вычисляемому орбитальной моделью.
double TIniSatParams::calc_NORAD_n0( const TOrbitalModel& om )
{
    double dx,f,fmid,xmid,root;
    double x1 = om.n - 1.e-3;
    double x2 = om.n + 1.e+3;

    f = equationFunc( x1, om.n, om.e, i0 );
    fmid = equationFunc( x2, om.n, om.e, i0 );

    if( f < 0.0 ){
        root = x1; dx = x2 - x1;
    }
    else{
        root = x2; dx = x1 - x2;
    }

    do{
        fmid = equationFunc( xmid = root + (dx *= .5), om.n, om.e, i0 );
        if( fmid <= 0.0 ) root = xmid;
    } while( fabs(dx) > 1.e-10 && fmid != 0.0 );

    return root;
}



double TIniSatParams::equationFunc( double x, double n, double e, double i )
{
   double be2 = 1.0 - e * e;
   double be1 = sqrt( be2 );
   double A = TK2*(3.*pow(cos(i), 2.) - 1.)/(be1*be2);
   double A1 = pow( Ke/x, 2./3. );
   double D1 = A/(A1*A1);
   double D2 = D1*D1;
   double D3 = D2*D1;
   double PD = - 134.*D3/81. - D2 - D1/3. + 1.;
   return ( n * A * pow( A1 * PD, -2.0 ) + (n - x) );
}
*/

/*TIniSatParams::TIniSatParams( const char * file_name, const char * sat_name ) throw( TAccessExc, TRequestExc )
{
    TAccessExc ae1( 1, "TIniSatParams::TIniSatParams( const char *, const char * )" );
    TRequestExc re1( 1, "TIniSatParams::TIniSatParams( const char *, const char * )" );

    char *p, s[16];

    FILE * f = fopen( file_name, "rb" );
    if( f == NULL ){
        throw ae1;
    }
    unsigned l = _filelength( fileno( f ) );
    char * buf = new char [l+1];
    if( fread( buf, 1, l, f ) != l ){
        fclose( f );
        throw ae1;
    }
    fclose( f );

    buf[l] = '\0';
    if( (p = strstr( buf, sat_name )) == NULL ){
        delete buf;
        throw re1;
    }
    while( *p++ != (char)0xa );
//  Сейчас p указывает на начало первой строки телеграммы. 
//  Год. 
    memcpy( s, p+18, 2 );
    s[2] = '\0';
    fEpochYear = 1900 + atoi( s );
//  День и его дробная часть. 
    memcpy( s, p+20, 12 );
    s[12] = '\0';
    fEpochYearTime = atof( s ) - 1;
//  BSTAR drag term. 
    bstar = econv( p + 53 );
//     Номер набора. 
    memcpy( s, p+64, 4 );
    s[4] = '\0';
    fSetNum = atoi( s );

    while( *p++ != char(0xa) );
//  Сейчас p указывает на начало второй строки телеграммы. 
//  Наклонение. 
    memcpy( s, p+8, 8 );
    s[8] = '\0';
    i0 = atof( s ) * DR;
//  Восходящий узел. 
    memcpy( s, p+17, 8 );
    s[8] = '\0';
    raan = atof( s ) * DR;
// Эксцентриситет. 
    memcpy( s+1, p+26, 7 );
    *s = '.';
    s[8] = '\0';
    e0 = atof( s );
// Перигей. 
    memcpy( s, p+34, 8 );
    s[8] = '\0';
    w0 = atof( s ) * DR;
// Средняя аномалия. 
    memcpy( s, p+43, 8 );
    s[8] = '\0';
    m0 = atof( s ) * DR;
// Среднее движение. 
    memcpy( s, p+52, 11 );
    s[11] = '\0';
    n0 = atof( s ) * ( .25 * DR ); //4.36332313e-3;
// Номер орбиты.  
    memcpy( s, p+63, 5 );
    s[5] = '\0';
    fRevNum = atoi( s );

    delete [] buf;
}*/


TIniSatParams::TIniSatParams( const char * p )
{
    char s[13];
/* Разбор первой строки телеграммы. */
/* Год. */
    memcpy( s, p+18, 2 );
    s[2] = '\0';
    fEpochYear = 1900 + atoi( s );
/* День и его дробная часть. */
    memcpy( s, p+20, 12 );
    s[12] = '\0';
    fEpochYearTime = atof( s ) - 1;
/* BSTAR drag term. */
    bstar = econv( p + 53 );
/* Номер набора. */
    memcpy( s, p+64, 4 );
    s[4] = '\0';
    fSetNum = atoi( s );

    while( *p++ != char(0xa) );
/* Разбор второй строки телеграммы. */
/* Наклонение. */
    memcpy( s, p+8, 8 );
    s[8] = '\0';
    i0 = atof( s ) * DR;
/* Восходящий узел. */
    memcpy( s, p+17, 8 );
    s[8] = '\0';
    raan = atof( s ) * DR;
/* Эксцентриситет. */
    memcpy( s+1, p+26, 7 );
    *s = '.';
    s[8] = '\0';
    e0 = atof( s );
/* Перигей. */
    memcpy( s, p+34, 8 );
    s[8] = '\0';
    w0 = atof( s ) * DR;
/* Средняя аномалия. */
    memcpy( s, p+43, 8 );
    s[8] = '\0';
    m0 = atof( s ) * DR;
/* Среднее движение. */
    memcpy( s, p+52, 11 );
    s[11] = '\0';
    n0 = atof( s ) * ( .25 * DR ); //4.36332313e-3;
/* Номер орбиты. */
    memcpy( s, p+63, 5 );
    s[5] = '\0';
    fRevNum = atoi( s );
}

int operator==( const TCorrectionParams &c1, const TCorrectionParams &c2 )
{
    return (c1.fVersion == c2.fVersion && c1.fTime == c2.fTime && c1.fTBUSTime == c2.fTBUSTime &&
        c1.roll == c2.roll && c1.pitch == c2.pitch && c1.yaw == c2.yaw );
}


int operator!=( const TCorrectionParams &c1, const TCorrectionParams &c2 )
{
    return !(c1==c2);
}


TCorrectionParams::TCorrectionParams( const TBlk0 & b )
{
// Структура параметров коррекции географической привязки находится в 0-блоке по постоянному смещению, поэтому для файлов любого типа можно использовать одинаковый код.
    TBlk0_HRPT *p = (TBlk0_HRPT *)(&b);

    if( (fVersion = p->corVersion) == 0 ){
        fVersion = 2;
        fTime = fTBUSTime = roll = pitch = yaw = 0.;
    }
    else{
        fTime = double(p->corTime) / 1000.;
        fTBUSTime = double(p->corTBUSTime) / 1000.;
        roll = p->corRoll;
        pitch = p->corPitch;
        yaw = p->corYaw;
    }
}


TCorrectionParams::TCorrectionParams( unsigned long version, double t, double tbus_t, double ROLL, double PITCH, double YAW ) :
    fVersion( version ), fTime( t ), fTBUSTime( tbus_t ), roll( ROLL ), pitch( PITCH ), yaw( YAW )
{
}
