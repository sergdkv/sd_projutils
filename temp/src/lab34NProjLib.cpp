#include <lab34NProjLib.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


TLab34NProj::TLab34NProj(){
    memset( &blk0, 0, sizeof(blk0) );
    raw.clear();
    geo.clear();
}

// Чтение файла - проекции нового формата
// вход : file_name - имя файла
// выход : содержимое проекции (сырые данные и геофизические величины и нулевой блок)
TLab34NProj& TLab34NProj::read( std::string file_name,  TLab34NProj& proj )
{       
    FILE* file_header = fopen(file_name.c_str(), "rb");
    if( file_header == 0 ){ 
        printf( "header is NULL for file %s\n", file_name.c_str() );
        perror( file_name.c_str() );
        exit(1);
    }
    else { 
        //printf( "header is not NULL for file %s\n", file_name.c_str() );
    } 
    if( fread( &proj.blk0, sizeof( proj.blk0 ), 1, file_header ) != 1 ){
        printf( "header is NULL for file %s\n", file_name.c_str() );
        perror( file_name.c_str() );
        exit(1);
    }
    unsigned int lines = proj.blk0.scanNum;
    unsigned int cols =  proj.blk0.pixNum; 

    proj.geo.resize(lines); 
    proj.raw.resize(lines);
    for( unsigned int i = 0; i < lines; i++ ){ 
        proj.geo[i].resize(cols); 
        proj.raw[i].resize(cols);
    }

    static short line_buf[1000 * 1000];
    for( unsigned int i = 0; i < lines; i++ ){ 
        if( fread( line_buf, sizeof(short), cols, file_header ) != cols ){
            printf( "header is NULL for file %s\n", file_name.c_str() );
            perror( file_name.c_str() );
            exit(1);
        };
        for( unsigned int j = 0; j < cols; j++ ){ 
            short a = line_buf[j];
            proj.raw[i][j] = a;
            if( a < 0 ){
                proj.geo[i][j] = -999;
            }
            else {
                proj.geo[i][j] = a * proj.blk0.ka + proj.blk0.kb;
            }
        }
    }
    fclose( file_header ); 
    return proj;
}

// Чтение файла - проекции нового формата
// вход : file_name - имя файла
// выход : содержимое проекции (сырые данные и геофизические величины и нулевой блок)
TLab34NProj& TLab34NProj::copyFromTo( TLab34NProj& from_proj, TLab34NProj& to_proj )
{
    to_proj.blk0 = from_proj.blk0;
    unsigned int lines = to_proj.blk0.scanNum;
    unsigned int cols =  to_proj.blk0.pixNum; 

    to_proj.geo.resize(lines); 
    to_proj.raw.resize(lines);
    for( unsigned int i = 0; i < lines; i++ ){ 
        to_proj.geo[i].resize(cols); 
        to_proj.raw[i].resize(cols);
    }

    for( unsigned int i = 0; i < lines; i++ ){ 
        for( unsigned int j = 0; j < cols; j++ ){ 
            to_proj.raw[i][j] = from_proj.raw[i][j];
            to_proj.geo[i][j] = from_proj.geo[i][j];
        }
    }
    return to_proj;
}

TLab34NProj& TLab34NProj::write( std::string file_name )
{
    // Подготовка к записи
    // Корректируем коэффициенты представления данных и максимальное значение пикселя
    double min_value = 1000.*1000.*1000.*1000.;
    for( int line = 0; line < lines(); line++ ){
        for( int col = 0; col < cols(); col++ ){
            if( raw[line][col] >= 0 ){
                if( geo[line][col] < min_value ) {
                    min_value = geo[line][col];
                }
            }
        }
    }
    blk0.kb = floor( min_value );

    // выполняем преобразование данных к коротким целым
    int max_pixel_value = 0;
    for( int line = 0; line < lines(); line++ ){
        for( int col = 0; col < cols(); col++ ){
            if( raw[line][col] >= 0 ){
                double a = geo[line][col];
                double c = floor( (a - blk0.kb) / blk0.ka + 0.5 );
                raw[line][col] = c;
                if ( c > max_pixel_value ) { 
                    max_pixel_value = c;
                }
            }
        }
    }
    short M = max_pixel_value;
    if( (int)M != max_pixel_value ){
        printf( "Слишком большое максимальное значение в файле %s\n", file_name.c_str() );
        exit(1);
    }
    blk0.maxPixelValue = M;

    FILE* file_header = fopen(file_name.c_str(), "wb");
    if( file_header == 0 ){ 
        printf( "header is NULL for file %s\n", file_name.c_str() );
        perror( file_name.c_str() );
        exit(1);
    }
    else { 
        //printf( "header is not NULL for file %s\n", file_name.c_str() );
    } 

   if( fwrite( &blk0, sizeof( blk0 ), 1, file_header ) != 1 ){
        printf( "header is NULL for file %s\n", file_name.c_str() );
        perror( file_name.c_str() );
        exit(1);
    }

    static short line_buf[1000 * 1000];
    for( int i = 0; i < lines(); i++ ){ 
        for( int j = 0; j < cols(); j++ ){ 
            line_buf[j] = raw[i][j];
        }

       if( (int)fwrite( line_buf, sizeof(short), cols(), file_header ) != cols() ){
            printf( "header is NULL for file %s\n", file_name.c_str() );
            perror( file_name.c_str() );
            exit(1);
        };
    }
    if( fclose( file_header ) != 0 ){
        perror( file_name.c_str() );
        exit(1);
    };
    return *this;
}
TLab34NProj::TLab34NProj( const TLab34NProj& old ){
    blk0 = old.blk0;
    raw.resize(old.lines()); 
    geo.resize(old.lines()); 
    for( int i = 0; i < old.lines(); i++ ){ 
        raw[i].resize(old.cols() ); 
        geo[i].resize(old.cols());
    }
 
    for( int i = 0; i < old.lines(); i++ ){ 
       for( int j = 0; j < old.cols(); j++ ){ 
          raw[i][j] = old.raw[i][j];
          geo[i][j] = old.geo[i][j];
       }
    }
}
TLab34NProj& TLab34NProj::clear( int cloud_value )
{
    for(  int i = 0; i < lines(); i++ ){ 
        for(  int j = 0; j < cols(); j++ ){ 
            raw[i][j] = cloud_value; 
            geo[i][j] = -999;
        }
    }
    return *this;
}
double TLab34NProj::centerLon_for_Column( double column ) const
{
    double res = blk0.lonSize / (blk0.pixNum-1);
    return blk0.lon + column * res;
}
int TLab34NProj::getColumn_for_Lon( double lon ) const
{
    double res = blk0.lonSize / (blk0.pixNum-1);
    double column = (lon - blk0.lon) / res;
    double rounded_col = floor(column + 0.5);
    return int( rounded_col );
}
// 0 <= line < scanNum
double TLab34NProj::centerLat_for_Line( double line ) const
{
    double min_lat = blk0.lat;
    if( blk0.projType == 1 ) min_lat = mercatorLat( min_lat );

    double max_lat = blk0.lat + blk0.latSize;
    if( blk0.projType == 1 ) max_lat = mercatorLat( max_lat );

    double res = ( max_lat - min_lat ) / (blk0.pixNum-1);
    double lat = min_lat + line * res;
    if( blk0.projType == 1 ) lat = unmercatorLat( lat );
    return lat;
}
double TLab34NProj::mercatorLat( double lat ) const {
   lat = lat * atan2(1,1) / 90;
   double mLat = log( tan( 0.5*lat + atan2(1,1) ) );
   mLat = 90 * mLat / atan2(1,1);
   return mLat;
};

double TLab34NProj::unmercatorLat( double mlat ) const {
   mlat = mlat * atan2(1,1) / 90;
   double lat = 2.0 *( atan(exp(mlat)) - atan2(1,1) ) ;
   lat = 90 * lat / atan2(1,1);
   return lat;
};

int TLab34NProj::getLine_for_Lat( double lat ) const
{
    if( blk0.projType == 1 ) lat = mercatorLat( lat );

    double min_lat = blk0.lat;
    if( blk0.projType == 1 ) min_lat = mercatorLat( min_lat );

    double max_lat = blk0.lat + blk0.latSize;
    if( blk0.projType == 1 ) max_lat = mercatorLat( max_lat );

    double res = ( max_lat - min_lat ) / (blk0.pixNum-1);

    double line = (lat - min_lat) / res;
    double rounded_line = floor(line + 0.5);
    return int( rounded_line );
}

