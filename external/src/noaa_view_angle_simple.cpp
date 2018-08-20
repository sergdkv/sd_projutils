#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <orbmodel_const.h>
#include <orbmodel.h>
#include <y_time.hpp>

#include <view_angle_simple.hpp>


using std::cout;
using std::endl;

const double H_NOAA=820.;
const double H_MTSAT=38162.84 - 6371.032 ; // 35800;
const double R_EARTH=6371.032;
//const double PI=3.14159265359;


int main( int argc, char** argv ){

   const char * fileName = argv[1];
   FILE * f = fopen( fileName, "rb" );

   TBlk0_AVHRR blk0;

   if ( fread( (void*)&blk0, 1, 512, f ) != 512 ){	
      fprintf( stderr, "Can not read file %s \n", fileName );	
      fclose( f );
      return 1;
   }
   fclose( f );

   TIniSatParams isp(     (TBlk0&)blk0 );
   TCorrectionParams cop( (TBlk0&)blk0 );
   int fYear = blk0.b0.year;
   double fYearTime = double( blk0.b0.day - 1 ) + double( blk0.b0.dayTime ) / (24.*60.*60.*1000.);

   TOrbitalModel orbModel = TOrbitalModel( isp, fYear, fYearTime, cop );

   for( double rel_time = -600; rel_time < 60; rel_time += 5 ){
       orbModel.model( rel_time/(24.*60.*60.) );

       double sin_ov = sin(orbModel.ov);
       double cos_ov = cos(orbModel.ov);
       double e_2 = orbModel.r[2];
       double e_1 = orbModel.r[1];
       double e_0 = orbModel.r[0];

       // Переход к системе, связанной с вращением Земли.
       double ecf_0 = e_0*cos_ov + e_1*sin_ov;
       double ecf_1 = e_1*cos_ov - e_0*sin_ov;
       // Находим геодезические широту и долготу.
       double lat = atan2( e_2, (1.0-Ee2)*sqrt(ecf_0*ecf_0+ecf_1*ecf_1) );
       double lon = atan2( ecf_1, ecf_0 );

       printf( "%f rtime=%10.3f  lat=%8.3f lon=% 8.3f R=%8.3f    %8.3f %8.3f %8.3f \n", 
           Ee2, rel_time, 
           lat*RD, lon*RD, orbModel.R, orbModel.r[0], orbModel.r[1], orbModel.r[2] );

   }


   double zenith, azimuth, h;
   calc_sat_zenith_azimuth_h( 
     R_EARTH + 900,  23.3, 144.3,
     H_MTSAT,  0, 130, 
     &zenith, &azimuth, &h  );
   printf( "%f %f %f\n", zenith, azimuth, h );
}

