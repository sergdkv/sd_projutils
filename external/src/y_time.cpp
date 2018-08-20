/*-------------------------------------------------------------------------
    y_time.cpp
-------------------------------------------------------------------------*/

#include "y_time.hpp"

static int months[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
static int leapMonths[13] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };


int isLeapYear( int year )
{
    return ( (year%4==0 && year%100!=0) || year%400==0 ) ? 1 : 0;
}


int dateToDay( int year, int month, int date )
{
    int n = date + months[month];
    if( month>1 && isLeapYear(year) ) n++;
    return n;
}


void dayToDate( int year, int year_day, int *month, int *date )
{
    int *m = isLeapYear(year) ? leapMonths : months;
    int i = 1;
    while( m[i] <= year_day ) i++;
    *month = i-1;
    *date = year_day - m[i-1];
}


double dayTimeToFraction( int h, int m, int s, int t )
{
    return (double(t)/360000. + double(s)/3600. + double(m)/60. + double(h)) / 24.;
}


void dayFractionToTime( double dayFraction, int *hour, int *minute, int *second, int *tic )
{
    dayFraction *= 24.;
    *hour = int( dayFraction );
    dayFraction -= double( *hour );
    dayFraction *= 60.;
    *minute = int( dayFraction );
    dayFraction -= double( *minute );
    dayFraction *= 60.;
    *second = int( dayFraction );
    dayFraction -= double( *second );
    dayFraction *= 100.;
    *tic = int( dayFraction );
}


double timeBetween( int year1, double yearTime1, int year2, double yearTime2 )
{
    int y1,y2;
    double yt1,yt2;
    if( year2 >= year1 ){
        y2 = year2;
        yt2 = yearTime2;
        y1 = year1;
        yt1 = yearTime1;
    }
    else{
        y1 = year2;
        yt1 = yearTime2;
        y2 = year1;
        yt2 = yearTime1;
    }
    double t = yt2 - yt1;
    if( y2 > y1 ){
        t += 365.;
        if( isLeapYear( y1 ) ) t += 1.;
    }
    return (year2 >= year1 ? t : -t);
}
