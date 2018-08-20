#include <math.h>
#include <iostream>
#include <iomanip>
#include <view_angle.hpp>

using std::cout;
using std::endl;

const double H_NOAA=820.;
const double H_MTSAT=35800;
const double R_EARTH=6371.032;
const double PI = 3.14159265359;

// alpha = 55.3*math.pi/180.

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

