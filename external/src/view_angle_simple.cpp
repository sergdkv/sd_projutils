#include <math.h>
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

const double H_NOAA=820.;
const double H_MTSAT=38162.84 - 6371.032 ; // 35800;
const double R_EARTH=6371.032;
const double PI = 3.14159265359;

// alpha = 55.3*math.pi/180.



#include <stdio.h>

void calc_sat_zenith_azimuth_h( 
   double R1, double lat1, double lon1,
   double R2, double lat2, double lon2,
   double *zenith, double *azimuth, double *h 
 ){

    double PI = atan(1)*4.0;
    double DR = PI / 180.0;
    double RD = 180.0 / PI;

    lat1 *= DR;
    lat2 *= DR;
    lon1 *= DR;
    lon2 *= DR;


    // Ед. вектор из ц.з. на точку наблюдения (вектор направленный вверх из точки наблюдения)
    double z_top = sin(lat1);
    double x_top = cos(lat1)*cos(lon1);
    double y_top = cos(lat1)*sin(lon1);
    printf( "%f %f %f\n", z_top*R1, x_top*R1, y_top*R1 );

    // Вектор из ц.з. на спутник
    double z_sat = sin(lat2)    * R2;
    double x_sat = cos(lat2)*cos(lon2) * R2;
    double y_sat = cos(lat2)*sin(lon2) * R2;

    // Вектор из точки наблюдения на спутник (вектор наблюдения)
    double x_s = x_sat - x_top * R1;
    double y_s = y_sat - y_top * R1;
    double z_s = z_sat - z_top * R1;
    double R_s = sqrt( x_s*x_s + y_s*y_s + z_s*z_s );

    // Вектор, направленный на север из точки наблюдения
    double z_nord = cos(lat1);
    double x_nord = -sin(lat1)*cos(lon1);
    double y_nord = -sin(lat1)*sin(lon1);
    double l_nord = sqrt(x_nord*x_nord + y_nord*y_nord + z_nord*z_nord);
    x_nord /= l_nord;
    y_nord /= l_nord;
    z_nord /= l_nord;

    // Вектор, направленный на восток из точки наблюдения
    double z_east = 0;
    double x_east = -cos(lat1)*sin(lon1);
    double y_east =  cos(lat1)*cos(lon1);
    double l_east = sqrt(x_east*x_east + y_east*y_east + z_east*z_east);
    x_east /= l_east;
    y_east /= l_east;
    z_east /= l_east;

    double z = (x_s*x_top  + y_s*y_top  + z_s*z_top);
    double x = (x_s*x_nord + y_s*y_nord + z_s*z_nord);
    double y = (x_s*x_east + y_s*y_east + z_s*z_east);

    //printf( "%f\n", x_nord * x_top  + y_nord *y_top  + z_nord *z_top );
    //printf( "%f\n", x_east * x_top  + y_east *y_top  + z_east *z_top );
    //printf( "%f\n", x_nord * x_east  + y_nord * y_east  + z_nord * z_east  );

    
    //printf( "nord %f\n", x_nord * x_nord  + y_nord * y_nord  + z_nord * z_nord );
    //printf( "%f\n", x_top * x_top    + y_top * y_top  + z_top*z_top );
    //printf( "%f\n", x_east * x_east  + y_east*y_east + z_east*z_east );

    //printf( "%f %f %f\n", z, x, y );

    *zenith =  acos( z / R_s ) * RD;
    *azimuth = atan2( y, x ) * RD;
    *h = 90 - (*zenith);
    return;
}

//int main(){
//   double zenith, azimuth, h;
//   calc_sat_zenith_azimuth_h( 
//     R_EARTH,  25, 115,
//     H_MTSAT,  0, 130, 
//     &zenith, &azimuth, &h  );
//   printf( "%f %f %f\n", zenith, azimuth, h );
//}

double calc_earth_angle( double r, double R, double alpha ){
    if ( alpha > asin(R/(R+r)) ){
        return PI/2;
	}
    double beta = 0;
    for( int i = 0; i < 10; i++ ){
        double error = sin(2.*beta) - (r/R + 1. - cos(2*beta))*tan(alpha);
        double diff_beta = 2*cos(2*beta) - 2*sin(2*beta)*tan(alpha);
        double step =  error/diff_beta;
        beta -= step;
        if ( fabs(step) < 0.0001 / 180 * PI ){ 
		    break;
		}
	}
    return beta*2;
}

double calc_sat_angle( double r, double R, double beta ){
    double alpha = atan( sin(beta)/(r/R +1 - cos(beta)));
    return alpha;
}

double dist_grad( double lat1, double lon1, double lat2, double lon2 ){
    const double DR=PI / 180;
    const double RD=180 / PI;
    double z1 =    sin(lat1*DR);
    double l1 =    cos(lat1*DR);
    double x1 = l1*cos(lon1*DR);
    double y1 = l1*sin(lon1*DR);

    double z2 =    sin(lat2*DR);
    double l2 =    cos(lat2*DR);
    double x2 = l2*cos(lon2*DR);
    double y2 = l2*sin(lon2*DR);
    double r = x1*x2 + y1*y2 + z1*z2;
    if ( r > 1 ) {
	   return 0;
	}
    return acos(r)*RD;
}

double calc_MTSAT_zen_angle( double lat, double lon ){
   double beta =  dist_grad( 0, 140, lat, lon )*PI/180;
   double alpha = calc_sat_angle( H_MTSAT, R_EARTH, beta );
   double MTSAT_zen_angle = alpha + beta;
   return MTSAT_zen_angle;   
}

void test(){
    // MTSAT example
    for( double LON = 140; LON < 140 +90 +1; LON += 0.1 ){
	   double MTSAT_zen_angle = calc_MTSAT_zen_angle( 0, LON );
       cout << std::fixed << std::setprecision(4) << std::setw(9) 
	       << LON << " " << 
	       std::fixed << std::setprecision(4) << std::setw(9) << 
		   1/cos(MTSAT_zen_angle) << " " << 
		   std::fixed << std::setprecision(4) << std::setw(9) <<
		   MTSAT_zen_angle*180/PI << endl;
	}

    //# NOAA example
    //#for a in range( 0, 55+1, 5 ):
    //#  alpha =  math.fabs(a) * math.pi / 180.
    //#  beta = calc_earth_angle( r, R, alpha )
    //#  print a, (alpha+beta)*180/math.pi, (beta)*180/math.pi
}

// int main(){
   // test();
   // return 0;
// }

