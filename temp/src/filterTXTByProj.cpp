
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <math.h>
#include <getopt.h>
#include <lab34NProjLib.hpp>



void use_message(){
    puts( 
"filterTXTByProj -- программа, осуществляющая фильтрацию строк текстового файла на основе маски облачности\n"
"\n"
"filterTXTByProj 1 2 mt21344_ir1.pro input.txt output.txt  # фильтрация строк файла input.txt\n"
"# при этом широта находится в 1 колонке файла input.txt, долгота во второй колонке\n"
"# результат записывается в файл output.txt\n"
"\n"
"filterTXTByProj 1 2 mt21344_ir1.pro - output.txt\n"
"# аналог, исходные данные извлекаются из потока ввода\n"
"\n"
"filterTXTByProj 1 2 mt21344_ir1.pro - output.txt\n"
"# аналог, результат записывается в поток вывода\n"
"\n"
"filterTXTByProj 1 2 mt21344_ir1.pro input.txt output.txt -7\n"
"# аналог, фильтруются только строки, маска облачности которых содержит -7 (суша)"
 );
};

int main( int argc, char ** argv ){
    if ( argc < 6 ){
        use_message();
        return 0;
    };

    // разбор опций программы
    int lat_pos = 0;
    sscanf( argv[1], "%d", &lat_pos );
    lat_pos -= 1;

    int lon_pos = 0;
    sscanf( argv[2], "%d", &lon_pos );
    lon_pos -= 1;

    char * proj_file_name = argv[3];
    std::string input_file_name(argv[4]);
    std::string output_file_name(argv[5]);

    int mask_val = 0;
    if( argc == 7 ){
       sscanf( argv[7], "%d", &mask_val );
       if( mask_val > 0 ) mask_val = -mask_val;
    }
    
    // получение файла-проекции
    

    TLab34NProj inp_proj;
    TLab34NProj::read( proj_file_name, inp_proj );

    // открытие исходных файлов
    
    FILE * input_file = NULL;
    if( input_file_name == "-" ){
        input_file = stdin;
    }
    else {
        input_file = fopen( input_file_name.c_str(), "r" );
        if( input_file == NULL ){
            fprintf( stderr, "can't open file %s\n", input_file_name.c_str() );
            exit(1);
        }
    }

    FILE * output_file = NULL;
    std::string output_file_tmp = "";
    if( output_file_name == "-" ){
        output_file = stdin;
    }
    else {
        output_file_tmp = output_file_name+".tmp";
        output_file = fopen( output_file_tmp.c_str(), "w" );
        if( output_file == NULL ){
            fprintf( stderr, "can't open file %s\n", output_file_tmp.c_str() );
            exit(1);
        }
    }

    // обработка исходного файла, запись результата
    
    char input_s[1024];
    char input_s2[1024];
    while( fgets(input_s,1023,input_file) != NULL ){
       strncpy( input_s2, input_s, 1023 );
       double lat = -9999;
       double lon = -9999;
       int pos = 0;
       char*p = input_s;
       while( strlen(p) > 0 && isspace(p[strlen(p)-1]) ) {
          p[strlen(p)-1] = 0;
       }
       p = strtok( p, ";# \t" );
       if( p == NULL || strlen(p) == 0 ) continue;  // empty string
       //printf( "TEST %d %s\n", (int)strlen(p), p );
       while( p != NULL ){
         if( lat_pos == pos ) sscanf( p, "%lf", &lat );
         if( lon_pos == pos ) sscanf( p, "%lf", &lon );
         p = strtok( NULL, ";# \t" );
         pos += 1;
       }
       if( lat == -9999 || lon == -9999 ){
          fprintf( stderr, "Error in parsing string %s\n", input_s2 );
       }
       int col = inp_proj.getColumn_for_Lon(lon);
       int row = inp_proj.getLine_for_Lat(lat);
       if( col < 0 ) continue;
       if( row < 0 ) continue;
       if( col >= inp_proj.lines() ) continue;
       if( row >= inp_proj.cols() ) continue;
       int val = inp_proj.raw[row][col];
       fputs( input_s2, stderr );
       fprintf( stderr, "lat=%lf lon=%lf row=%d col=%d val=%d\n", lat, lon, row, col, val );
       if( mask_val == 0 && val < 0 || val == mask_val ) {
         fprintf( stderr, "SKIP\n" );
         continue;
       }
       else {
         fprintf( stderr, "OK\n" );
       }
       
       fputs( input_s2, output_file );
    }
    //printf( "OK\n" );

    // закрытие файлов
    
    if( input_file_name == "-" ){
        ;
    }
    else {
        fclose( input_file );
    }

    if( output_file_name == "-" ){
        ;
    }
    else {
        fclose( output_file );
    }

    // сохранение результата в файл
    

    if( output_file_name != "-" ){
        FILE *from_file = fopen( output_file_tmp.c_str(), "r" );
        FILE *to_file = fopen( output_file_name.c_str(), "w" );
        char buf[1024];
        while( fgets( buf,1023,from_file) != NULL ){
           fputs( buf, to_file );
        }
        fclose( from_file );
        fclose( to_file );
        unlink( output_file_tmp.c_str() );
    }

    return 0;
}
