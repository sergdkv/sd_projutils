
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <lab34NProjLib.hpp>
#include <iostream>
#include <stdlib.h>



void use_message(){
    puts( 
"binopProj\n"
"программа для выполнения бинарных операций над файлами-проекциями нового формата lab34\n"
"Формат вызова:\n"
"   binopProj <имя исходного файла> [опции обработки] -o <имя конечного файла>\n"
"Список опций:\n"
"-o       [имя файла]\n"
"--output [имя файла]\t--- запись результата в файл\n"
"--save   [имя файла]\t--- запись результата в файл\n"
"--set-ka [число]\t--- определить новый множитель представления чисел\n"
"--temp2rad [длина волны]\t--- преобразовать температуры в радиации\n"
"--rad2temp [длина волны]\t--- преобразовать радиацию в температуру\n"
"-M      <число>\n"
"--mult [число]\t--- умножить значения проекции на число\n"
"-R      <число>\n"
"--relation [число]\t--- поделить значения проекции на число\n"
"-d       [имя файла]\n"
"--diffp [имя файла]\t--- вычесть значение проекции\n"
"-s       [имя файла]\n"
"--sump [имя файла]\t--- прибавить значения взяв их из проекции\n"
"--maskover  [имя файла]\t--- наложить на проекцию новую маску облачности\n"
"--maskunder [имя файла]\t--- дополнить маску облачности значениями из проекции\n"
"-S       [число]\n"
"--sum  [число]\t--- прибавить значение\n"
"-L     x1,x2,y1,y2\n"
"--linear-trans-value x1,x2,y1,y2\t--- линейное преобразование значений проекции\n"
"--min       [имя файла]\t--- минимальное значение из двух проекций\n"
"--tempover  [имя файла]\t--- наложить на проекцию температуры из другой проекции\n"
"--tempunder [имя файла]\t--- дополнить отсутствующие температуры одной проекции температурами из другой\n"
"--filtrif  [опции фильтрации]\t--- фильтрация проекции\n"
"                             \t    [опции фильтрации] = \n"
"                             \t       [имя файла],\n"
"                             \t       [less|large|lessoreq|largeoreq|less_soft|\n"
"                             \t         large_soft|lessoreq_soft|largeoreq_soft], \n"
"                             \t       [limit (float double)],[mask value (5)]\n"
"                    (пример:          --filtrif n1235_ir14.pro,large,-2,mask5)\n"
"--changemask  <from>,<to>\n"
"     (пример:          --changemask -2,-16 \n"
"--abs  \n"
"     (пример:          --abs \n"
"--freepersent  sizeX,sizeY[,step]\n"
"     (пример:          --freepersent 5,5 \n"
"--median-rect [число]x[число][,шаг]\t--- медианное осреднение\n"
"--razbros-rect [число]x[число]\t--- разброс\n"
"--sigma-rect [число]x[число]\t--- разброс\n"
"--average-rect [число]x[число]\t--- осреднение\n"
"--print-median-row \t--- осреднение по строке\n"
"--print-median-column \t--- осреднение по столбцу\n"
"--shift [число X],[число Y] \t--- сдвигуть проекцию вправо на X и вверх на Y\n"
"--MNK-h [число]\t--- квадратиное (МНК) осреднение по горизонтальному отрезку \n"
"--MNK-v [число]\t--- квадратиное (МНК) осреднение по горизонтальному отрезку \n"
"--row-from-file [имя файла]\t--- инициализация значений в строках из \n"
"                           \tтекстового файла формата <номер строки> <значение>\n"

"--col-from-file [имя файла]\t--- инициализация значений в колонках из \n"
"                           \tтекстового файла формата <номер колонки> <значение>\n"


"--set_year [год]\t--- модификация года приема данных\n"

"--set_day [год]\t--- модификация дня приема (номер дня в году X-01-01 -- первый день)\n"

"--set_daytime [год]\t--- модификация времени дня (время дня -- секунды с начала дня * 1000)\n"

"--set_channel [год]\t--- модификация номера канала\n"

"--set_sat_id [год]\t--- модификация номера спутника\n"

"--set_sat_name [sat name]\t--- изменение имени спутника\n" );
};
std::string get_strhead( const std::string& s, const std::string& delim ){
       int i = s.find( delim );
       return s.substr( 0, i );
}

std::string get_strtail( const std::string& s, const std::string& delim ){
       int i = s.find( delim );
       if ( i == std::string::npos ){
           return std::string("");
       }
       return s.substr( i+1, std::string::npos );
}
int parseLongDouble( const char *buffer, double *ret_double, long*ret_long ){
    const char *begin;
    char *end;
    double d;
    long l;

    begin = buffer;
    while( *begin && isspace(*begin) ) begin++;
    if( ret_double ) 
       d = strtod( begin, &end );
    else 
       l = strtol( begin, &end, 10 );
    if( begin == end ){
        return 1;
    }
    else {
        while( *end && isspace(*end) ) end++;
        if( *end ) return 2;
    }
    if( ret_double ) 
        *ret_double = d;
    else if ( ret_long ) 
        *ret_long = l;
    return 0;
}
int parseLong( const char *buffer, long *ret_long ){
    return parseLongDouble( buffer, NULL, ret_long );
}
int parseDouble( const char *buffer, double *ret_double ){
    return parseLongDouble( buffer, ret_double, NULL );
}
int double_compare( const void *p_a, const void *p_b ){
    double a = *(const double*)p_a;
    double b = *(const double*)p_b;
    if(a < b) return -1;
    if(a > b) return 1;
    return 0;
}

int main( int argc, char ** argv ){
    if ( argc < 2 ){
        use_message();
        return 0;
    };

    char * input_file_name = argv[1];
    TLab34NProj inp_proj;
    TLab34NProj::read( input_file_name, inp_proj );

    const char *short_options = "" "h"
                                   "o:"

                                   "M:"
                                   "R:"
                                   "d:"
                                   "s:"
                                   "S:"
                                   "L:";
    static struct option long_options[] = {
        {"help",          0, 0, 'h' },
        {"save",          1, 0, 'o' },
        {"output",        1, 0, 'o' },
        {"set-ka",          1, 0, 301 },
        {"temp2rad",          1, 0, 190 },
        {"rad2temp",          1, 0, 191 },
        {"mult",          1, 0, 'M' },
        {"relation",          1, 0, 'R' },
        {"diffp",          1, 0, 'd' },
        {"sump",          1, 0, 's' },
        {"cutover",          1, 0, 101 },
        {"cutunder",          1, 0, 101 },
        {"maskover",          1, 0, 101 },
        {"maskunder",         1, 0, 102 },
        {"sum",          1, 0, 'S' },
        {"linear-trans-value",          1, 0, 'L' },
        {"min",               1, 0, 112 },
        {"tempover",           1, 0, 121 },
        {"tempunder",          1, 0, 122 },
        {"filtrif",          1, 0, 'F' },
        {"changemask",          1, 0, 'C' },
        {"abs",          0, 0, 'A' },
        {"freepersent",          1, 0, 'P' },
        {"median-rect",          1, 0, 201 },
        {"razbros-rect",          1, 0, 209 },
        {"sigma-rect",          1, 0, 215 },
        {"average-rect",          1, 0, 202 },
        {"print-median-row",          0, 0, 203 },
        {"print-median-column",          0, 0, 204 },
        {"shift",          1, 0, 205 },
        {"MNK-h",          1, 0, 206 },
        {"MNK-v",          1, 0, 207 },
        {"row-from-file",          1, 0, 208 },
        {"col-from-file",          1, 0, 210 },
        {"set_year",          1, 0, 211 },
        {"set_day",          1, 0, 212 },
        {"set_daytime",          1, 0, 213 },
        {"set_channel",          1, 0, 214 },
        {"set_sat_id",          1, 0, 225 },
        {"set_sat_name",          1, 0, 216 },
    };

    int optIdx;
    bool result_saved = false;
    while (1){
       int opt_char;
       opt_char = getopt_long( 
            argc-1, &(argv[1]),  // Мы пропустили имя исходного файла
            short_options, long_options, &optIdx);
       if( opt_char == -1 ) {
           break;  
       }
       result_saved = false;
       switch( opt_char ){
           case 'h':
               use_message();
               return(0);
               break;;
           case 'o':
               {
               result_saved = true;
               std::string  out_filename( optarg );
               inp_proj.write( out_filename );
               }
               break;;
           case 301:
               {
                  double M = 999.0;
                  if( sscanf( optarg, "%lf", &M ) != 1 ){
                     fprintf( stderr, "Ошибка разбора числа %s (опция set_ka)\n", optarg );
                  };
                  inp_proj.blk0.ka = M;
               }
               break;;
           case 190:
               {
                  double M = 999.0;
                  if( sscanf( optarg, "%lf", &M ) != 1 ){
                     fprintf( stderr, "Ошибка разбора числа %s (опция temp2rad)\n", optarg );
                  };
                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] < 0 ) continue;
                         double T = inp_proj.geo[i][j] + 273.14;
                         const double c1=1.1910427e-5;
                         const double c2=1.4387752;
                         double wavenumber = 10000./M;
                         double A1 = c1 * wavenumber*wavenumber*wavenumber;
                         double A2 = exp(c2*wavenumber/T) - 1;
                         inp_proj.geo[i][j] = A1/A2;
                     }
                  }
               }
               break;;
           case 191:
               {
                  double M = 999.0;
                  if( sscanf( optarg, "%lf", &M ) != 1 ){
                     fprintf( stderr, "Ошибка разбора числа %s (опция rad2temp)\n", optarg );
                  };
                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] < 0 ) continue;
                         double Radiance = inp_proj.geo[i][j];
                         const double c1=1.1910427e-5;
                         const double c2=1.4387752;
                         double wavenumber = 10000./M;
                         double A1 = c2 * wavenumber;
                         double A2 = log(1 + c1*wavenumber*wavenumber*wavenumber/Radiance ) ;
                         inp_proj.geo[i][j] = A1/A2 - 273.14;
                     }
                  }
               }
               break;;
           case 'M':
               {
                  double M = 999.0;
                  if( sscanf( optarg, "%lf", &M ) != 1 ){
                     fprintf( stderr, "Ошибка разбора числа %s (опция mult)\n", optarg );
                  };
                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] < 0 ) continue;
                         inp_proj.geo[i][j] *= M;
                     }
                  }
               }
               break;;
           case 'R':
               {
                  double M = 999.0;
                  if( sscanf( optarg, "%lf", &M ) != 1 ){
                     fprintf( stderr, "Ошибка разбора числа %s (опция relation)\n", optarg );
                  };
                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] < 0 ) continue;
                         inp_proj.geo[i][j] /= M;
                     }
                  }
               }
               break;;
           case 'd':
               {
                  TLab34NProj inp2_proj;
                  TLab34NProj::read( optarg, inp2_proj );
                  for( unsigned int i = 0; i < inp2_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp2_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] < 0 ) continue;
                         if( inp2_proj.raw[i][j] < 0 ) {
                            inp_proj.raw[i][j] = inp2_proj.raw[i][j];
                            continue;
                         }
                         inp_proj.geo[i][j] -= inp2_proj.geo[i][j];
                     }
                  }
               }
               break;;
           case 's':
               {
                  TLab34NProj inp2_proj;
                  inp2_proj.read( optarg, inp2_proj );
                  for( unsigned int i = 0; i < inp2_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp2_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] < 0 ) continue;
                         if( inp2_proj.raw[i][j] < 0 ) {
                            inp_proj.raw[i][j] = inp2_proj.raw[i][j];
                            continue;
                         }
                         inp_proj.geo[i][j] += inp2_proj.geo[i][j];
                     }
                  }
               }
               break;;
           case 101:
               {
                  TLab34NProj inp2_proj;
                  inp2_proj.read( optarg, inp2_proj );
                  for( unsigned int i = 0; i < inp2_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp2_proj.cols(); j++ ){
                         if( inp2_proj.raw[i][j] < 0 ) {
                            inp_proj.raw[i][j] = inp2_proj.raw[i][j];
                         }
                     }
                  }
               }
               break;;
           case 102:
               {
                  TLab34NProj inp2_proj;
                  inp2_proj.read( optarg, inp2_proj );
                  for( unsigned int i = 0; i < inp2_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp2_proj.cols(); j++ ){
                         if( inp2_proj.raw[i][j] < 0 && inp_proj.raw[i][j] >= 0 ) {
                            inp_proj.raw[i][j] = inp2_proj.raw[i][j];
                         }
                     }
                  }
               }
               break;;
           case 'S':
               {
                  double S = 999.0;
                  if( sscanf( optarg, "%lf", &S ) != 1 ){
                     fprintf( stderr, "Ошибка разбора числа %s (опция sum)\n", optarg );
                  };

                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] < 0 ) continue;
                         inp_proj.geo[i][j] += S;
                     }
                  }
               }
               break;;
           case 'L':
               {
                  double x1 = 999.0;
                  double x2 = 999.0;
                  double y1 = 999.0;
                  double y2 = 999.0;
                  if( sscanf( optarg, "%lf,%lf,%lf,%lf", &x1, &x2, &y1, &y2 ) != 4 ){
                     fprintf( stderr, "Ошибка разбора чисел %s (опция linear-trans-value)\n", optarg );
                  };

                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] < 0 ) continue;
                         double x = inp_proj.geo[i][j];
                         double new_x = y1+(x-x1)*(y2-y1)/(x2-x1);
                         inp_proj.geo[i][j] = new_x;
                     }
                  }
               }
               break;;
           case 112:
               {
                  TLab34NProj inp2_proj;
                  inp2_proj.read( optarg, inp2_proj );
                  for( unsigned int i = 0; i < inp2_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp2_proj.cols(); j++ ){
                         if( inp2_proj.raw[i][j] < 0 ) {
                            inp_proj.raw[i][j] = inp2_proj.raw[i][j];
                            continue;
                         }
                         if( inp_proj.raw[i][j] < 0 ) {
                            continue;
                         }
                         if( inp2_proj.geo[i][j] < inp_proj.geo[i][j] ){
                            inp_proj.geo[i][j] = inp2_proj.geo[i][j];
                         }
                     }
                  }
               }
               break;;
           case 121:
               {
                  TLab34NProj inp2_proj;
                  inp2_proj.read( optarg, inp2_proj );
                  for( unsigned int i = 0; i < inp2_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp2_proj.cols(); j++ ){
                         if( inp2_proj.raw[i][j] >= 0 ) {
                            inp_proj.geo[i][j] = inp2_proj.geo[i][j];
                            inp_proj.raw[i][j] = 0;
                         }
                     }
                  }
               }
               break;;
           case 122:
               {
                  TLab34NProj inp2_proj;
                  inp2_proj.read( optarg, inp2_proj );
                  for( unsigned int i = 0; i < inp2_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp2_proj.cols(); j++ ){
                         if( inp2_proj.raw[i][j] >= 0 && inp_proj.raw[i][j] < 0 ) {
                            inp_proj.geo[i][j] = inp2_proj.geo[i][j];
                            inp_proj.raw[i][j] = 0;
                         }
                     }
                  }
               }
               break;;
           case 'F':
               {
                  std::string s(optarg);
                  std::string filename;
                  std::string operation;
                  double limit;
                  long mask;

                  std::string filtrif_options[100];
                  int filtrif_options_num = 0;
                  for( int k = 0; k < 100; k++ ){
                     filtrif_options[k] = get_strhead( s, "," );  
                     filtrif_options_num = k + 1;
                     s = get_strtail( s, "," );
                     if( s.size() == 0 ){
                        break;
                     }
                  }
                  int filtr_nums = filtrif_options_num/3;

                  std::string mask_str  = filtrif_options[filtrif_options_num-1];

                  TLab34NProj inp_proj_filtr;
                  TLab34NProj::copyFromTo( inp_proj, inp_proj_filtr );
                  for( unsigned int i = 0; i < inp_proj_filtr.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj_filtr.cols(); j++ ){
                        inp_proj_filtr.raw[i][j] = -1;
                     }
                  }
                  for( int k = 0; k < filtr_nums; k++ ){
                      std::string fname_str  = filtrif_options[k*3];
                      std::string operand_str  = filtrif_options[k*3+1];
                      std::string operand_limit  = filtrif_options[k*3+2];

                      
                      // Выделяем имя файла
                      filename  = fname_str;  

                      // Выделяем вид операции
                      operation = operand_str;

                      // Выделяем величину порога фильтрации
                      std::string limit_str = operand_limit; 
                      if( parseDouble( limit_str.c_str(), &limit ) != 0 ){
                          std::string s10 = 
                             std::string("Ошибка преобразования величины предела ") + limit_str + 
                                    " в число (в строке " + optarg + ")";
                          puts( s10.c_str() );
                          exit(1);
                      };

                      // Определяем значение, которым будет произовдится маскирование
                      std::string mask_str2 = mask_str;
                      if( mask_str2.substr( 0, 4 ) != "mask" ){
                          std::string s10 = 
                             std::string("Ошибка формата маски") + mask_str2 + 
                                    " должен начитаться с mask (в строке " + optarg + ")";
                          puts( s10.c_str() );
                          exit(1);
                      }
                      mask_str2 = mask_str2.substr(4, std::string::npos );
                            
                      if( parseLong( mask_str2.c_str(), &mask ) != 0 ){
                          std::string s10 = 
                             std::string("Ошибка преобразования маски ") + mask_str2 + 
                                    " в число (в строке " + optarg + ")";
                          puts( s10.c_str() );
                          exit(1);
                      };


                      //std::cout << "filename=<"   << filename  << "> " << 
                      //      "operations=<" << operation << "> " <<
                      //      "limit=<"      << limit     << "> " <<
                      //      "mask=<"       << mask      << "> " << "\n";
                      int opt_code = 0;
                      if( operand_str.find( "less"     )  != std::string::npos ) opt_code = 4;
                      if( operand_str.find( "lessoreq" )  != std::string::npos ) opt_code = 8;
                      if( operand_str.find( "large"    )  != std::string::npos ) opt_code = 12;
                      if( operand_str.find( "largeoreq" ) != std::string::npos ) opt_code = 16;
                      if( operand_str.find( "soft"     )  != std::string::npos ) opt_code += 1;
                      if ( opt_code  < 4 ){
                          std::cout <<  "Операция " << operation << " в строке " << operand_str
                                  << " указана неверно" << "\n";
                          return 1;
                      }

                      TLab34NProj inp2_proj;
                      inp2_proj.read( filename, inp2_proj );
                      for( unsigned int i = 0; i < inp2_proj.lines(); i++ ){
                         for( unsigned int j = 0; j < inp2_proj.cols(); j++ ){
                            if( inp_proj.raw[i][j] < 0 ) {
                                continue;
                            }
                            if( inp2_proj.raw[i][j] < 0 ) { 
                                if( (opt_code % 2) == 0 ) {
                                    inp_proj_filtr.raw[i][j] = -mask;
                                }
                                continue;
                            }
                            else {
                                if( inp_proj_filtr.raw[i][j] >= 0 ) {
                                   continue;
                                }
                                double b = inp2_proj.geo[i][j];
                                switch( ( opt_code / 4 ) * 4 ){
                                    case 4: if( b < limit  ) 
                                              inp_proj_filtr.raw[i][j] = -mask; 
                                          else inp_proj_filtr.raw[i][j] = 0;  
                                          break;; 
                                    case 8: if( b <= limit )
                                             inp_proj_filtr.raw[i][j] = -mask;
                                          else inp_proj_filtr.raw[i][j] = 0;  
                                          break;; 
                                    case 12: if( b > limit )  
                                              inp_proj_filtr.raw[i][j] = -mask; 
                                           else inp_proj_filtr.raw[i][j] = 0;  
                                           break;; 
                                    case 16: if( b >= limit ) 
                                              inp_proj_filtr.raw[i][j] = -mask;
                                           else inp_proj_filtr.raw[i][j] = 0;  
                                           break;; 
                                } // case
                            }  // if
                         } // j 
                      } // i
                  }// k

                  for( unsigned int i = 0; i < inp_proj_filtr.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj_filtr.cols(); j++ ){
                        if( inp_proj.raw[i][j] < 0 ) continue;
                        inp_proj.raw[i][j] = inp_proj_filtr.raw[i][j];
                     }
                  }
               }
               break;;
           case 'C':
               {
                  int mask1 = -1;
                  int mask2 = -1;
                  if( sscanf( optarg, "%d,%d", &mask1, &mask2 ) != 2 ){
                     fprintf( stderr, "Ошибка разбора ЧИСЛОxЧИСЛО %s (опция changemask)\n", optarg );
                  };

                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] == mask1 ) {
                            inp_proj.raw[i][j] = mask2;
                         }
                     }
                  }
               }
               break;;
           case 'A':
               {

                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         if( inp_proj.raw[i][j] < 0 ) continue;
                         if( inp_proj.geo[i][j] < 0 ) {
                           inp_proj.geo[i][j] = -1*inp_proj.geo[i][j];
                         }
                     }
                  }
               }
               break;;
           case 'P':
               {
                  TLab34NProj out_proj;
                  TLab34NProj::copyFromTo( inp_proj, out_proj );

                  int sizeX = -1;
                  int sizeY = -1;
                  int step = 1;
                  if( sscanf( optarg, "%d,%d,%d", &sizeX, &sizeY, &step ) < 2 ){
                     fprintf( stderr, "Ошибка разбора ЧИСЛОxЧИСЛО %s (опция freepersent)\n", optarg );
                  };

                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         int i1 = i  - sizeY/2;
                         int i2 = i1 + sizeY;
                         int j1 = j  - sizeX/2;
                         int j2 = j1 + sizeX;
                         int n_free = 0;
                         int n = 0;
                         for(    int i3 = i1; i3 < i2; i3 += step ){
                            for( int j3 = j1; j3 < j2; j3 += step    ){
                                //printf( "i=%d j=%d %d %d n_free=%d\n", i, j, i3, j3, n_free );
                                if ( i3 < 0 || i3 >= inp_proj.lines() ) continue;
                                if ( j3 < 0 || j3 >= inp_proj.cols() ) continue;
                                n += 1;
                                if( inp_proj.raw[i3][j3] >= 0 ) {
                                   n_free += 1;
                                }
                            }
                         }
                         //printf( "i=%d j=%d %d %d n_free=%d\n", i, j, 0, 0, n_free );
                         out_proj.raw[i][j] = 1;
                         out_proj.geo[i][j] = (100.*n_free)/n;
                         //inp_proj.geo[i][j] = n_free;
                     }
                  }
                  TLab34NProj::copyFromTo( out_proj, inp_proj );
               }
               break;;
           case 201:
               {
                  TLab34NProj out_proj;
                  TLab34NProj::copyFromTo( inp_proj, out_proj );
                  int rect_x = 0;
                  int rect_y = 0;
                  int step = 1;
                  int rc = sscanf( optarg, "%dx%d,%d", &rect_x, &rect_y, &step );
                  if( rc == 2 ){
                         step = 1;
                  }
                  else if ( rc == 3 ){
                         // step = step
                  }
                  else {
                     fprintf( stderr, "Ошибка разбора ЧИСЛОxЧИСЛО %s (опция median-rect)\n", optarg );
                  }
                  
                  //printf( "median rect_x=%d rect_y=%d\n", rect_x, rect_y );
                  static double med_buf[5000*5000];
                  static double med_buf2[5000*5000];
                  int med_num = 0;
                  if( rect_x != 1 ){
                     for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                        for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                            med_num = 0;
                            int i1 = i - rect_y/2;
                            int i2 = i1 + rect_y;
                            int j1 = j  - rect_x/2;
                            int j2 = j1 + rect_x;
                            if( i1 < 0 ) i1 = 0;
                            if( i2 > inp_proj.lines() ) i2 = inp_proj.lines();
                            if( j1 < 0 ) j1 = 0;
                            if( j2 > inp_proj.cols() )  j2 = inp_proj.cols();
                            for( unsigned int i3 = i1; i3 < i2; i3++ ){
                                for( unsigned int j3 = j1; j3 < j2; j3++ ){
                                    if( inp_proj.raw[i3][j3] < 0 ) continue;
                                    med_buf[med_num] = inp_proj.geo[i3][j3];
                                    med_num += 1;
                                }
                            }
                            qsort( med_buf, med_num, sizeof(med_buf[0]), double_compare );
                            if( med_num > 0 ){
                                if( med_num % 2 == 0 ){
                                    out_proj.geo[i][j] = (med_buf[med_num/2] + med_buf[med_num/2-1])/2.;
                                    out_proj.raw[i][j] = 1;
                                }
                                if( med_num % 2 == 1 ){
                                    out_proj.geo[i][j] = med_buf[med_num/2];
                                    out_proj.raw[i][j] = 1;
                                }
                            }
                        }
                     }
                  }
                  else { //rect_x == 1
                     for( unsigned int j = 0; j < inp_proj.cols(); j += step ){
                        for( unsigned int i = 0; i < inp_proj.lines(); i += step ){
                            if( i == 0 ){
                                med_num = 0;
                                int i1 = i - rect_y/2;
                                int i2 = i1 + rect_y;
                                int j1 = j  - rect_x/2;
                                int j2 = j1 + rect_x;
                                if( i1 < 0 ) i1 = 0;
                                if( i2 > inp_proj.lines() ) i2 = inp_proj.lines();
                                if( j1 < 0 ) j1 = 0;
                                if( j2 > inp_proj.cols() )  j2 = inp_proj.cols();
                                for( unsigned int i3 = i1; i3 < i2; i3++ ){
                                    for( unsigned int j3 = j1; j3 < j2; j3++ ){
                                        if( inp_proj.raw[i3][j3] < 0 ) continue;
                                        med_buf[med_num] = inp_proj.geo[i3][j3];
                                        med_num += 1;
                                    }
                                }
                                qsort( med_buf, med_num, sizeof(med_buf[0]), double_compare );
                            }
                            else {
                                int i1 = i - rect_y/2;
                                int i2 = i1 + rect_y;
                                double value_to_delete = -99999.;
                                double value_to_add = -99999.;
                                if( i1 - 1 >= 0 && i1 - 1 < inp_proj.lines() 
                                    && inp_proj.raw[i1-1][j] >= 0 ){
                                   value_to_delete = inp_proj.geo[i1-1][j];
                                }
                                if( i2 - 1 >= 0 && i2 - 1 < inp_proj.lines() 
                                    && inp_proj.raw[i2-1][j] >= 0 ){
                                   value_to_add = inp_proj.geo[i2-1][j];
                                }
                                int k2 = 0;
                                for( int k = 0; k < med_num; k++ ){
                                   if( value_to_delete != -99999. 
                                       && med_buf[k] == value_to_delete ){
                                       value_to_delete = -99999.;
                                       continue;
                                   }
                                   if( value_to_add != -99999. 
                                       && med_buf[k] > value_to_add ){
                                       med_buf2[k2] = value_to_add;
                                       k2++;
                                       value_to_add = -99999.;
                                   }
                                   med_buf2[k2] = med_buf[k];
                                   k2++;
                                }
                                if( value_to_add != -99999. ){
                                   med_buf2[k2] = value_to_add;
                                   k2++;
                                }
                                if(0){ // проверка
                                   med_num = 0;
                                   int i1 = i - rect_y/2;
                                   int i2 = i1 + rect_y;
                                   int j1 = j  - rect_x/2;
                                   int j2 = j1 + rect_x;
                                   if( i1 < 0 ) i1 = 0;
                                   if( i2 > inp_proj.lines() ) i2 = inp_proj.lines();
                                   if( j1 < 0 ) j1 = 0;
                                   if( j2 > inp_proj.cols() )  j2 = inp_proj.cols();
                                   for( unsigned int i3 = i1; i3 < i2; i3++ ){
                                       for( unsigned int j3 = j1; j3 < j2; j3++ ){
                                           if( inp_proj.raw[i3][j3] < 0 ) continue;
                                           med_buf[med_num] = inp_proj.geo[i3][j3];
                                           med_num += 1;
                                       }
                                   }
                                   qsort( med_buf, med_num, sizeof(med_buf[0]), double_compare );
                                   if( k2 != med_num ){
                                      printf( "Error, %d %d ", i, j );
                                      exit(1);
                                   }
                                   for( int k = 0; k < med_num; k++ ){
                                      if( med_buf[k] != med_buf2[k] ){
                                         printf( "Error, %d %d ", i, j );
                                         exit(1);
                                      }
                                   }
                                }
                            }
                            if( med_num > 0 ){
                                if( med_num % 2 == 0 ){
                                    out_proj.geo[i][j] = (med_buf[med_num/2] + med_buf[med_num/2-1])/2.;
                                    out_proj.raw[i][j] = 1;
                                }
                                if( med_num % 2 == 1 ){
                                    out_proj.geo[i][j] = med_buf[med_num/2];
                                    out_proj.raw[i][j] = 1;
                                }
                            }
                        }
                     }
                  }
                  TLab34NProj::copyFromTo( out_proj, inp_proj );
               }
               break;;
           case 209:
               {
                  TLab34NProj out_proj;
                  TLab34NProj::copyFromTo( inp_proj, out_proj );
                  int rect_x = 0;
                  int rect_y = 0;
                  if( sscanf( optarg, "%dx%d", &rect_x, &rect_y ) != 2 ){
                     fprintf( stderr, "Ошибка разбора ЧИСЛОxЧИСЛО %s (опция razbros-rect)\n", optarg );
                  };
                  printf( "razbros rect_x=%d rect_y=%d\n", rect_x, rect_y );
                  static double med_buf[5000*5000];
                  int med_num = 0;
                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         int med_num = 0;
                         int i1 = i - rect_y/2;
                         int i2 = i1 + rect_y;
                         int j1 = j  - rect_x/2;
                         int j2 = j1 + rect_x;
                         if( i1 < 0 ) i1 = 0;
                         if( i2 > inp_proj.lines() ) i2 = inp_proj.lines();
                         if( j1 < 0 ) j1 = 0;
                         if( j2 > inp_proj.cols() )  j2 = inp_proj.cols();
                         double min_f = 999999;
                         double max_f = 999999;
                         for( unsigned int i3 = i1; i3 < i2; i3++ ){
                             for( unsigned int j3 = j1; j3 < j2; j3++ ){
                                if( inp_proj.raw[i3][j3] < 0 ) continue;
                                if ( min_f == 999999  || inp_proj.geo[i3][j3] < min_f ){
                                    min_f = inp_proj.geo[i3][j3];
                                }
                                if ( max_f == 999999  || inp_proj.geo[i3][j3] > max_f ){
                                    max_f = inp_proj.geo[i3][j3];
                                }
                             }
                         }
                         if( max_f != 999999 ){
                            out_proj.geo[i][j] = max_f - min_f;
                            out_proj.raw[i][j] = 1;
                         }
                     }
                  }
                  TLab34NProj::copyFromTo( out_proj, inp_proj );
               }
               break;;
           case 215:
               {
                  TLab34NProj out_proj;
                  TLab34NProj::copyFromTo( inp_proj, out_proj );
                  int rect_x = 0;
                  int rect_y = 0;
                  if( sscanf( optarg, "%dx%d", &rect_x, &rect_y ) != 2 ){
                     fprintf( stderr, "Ошибка разбора ЧИСЛОxЧИСЛО %s (опция sigma-rect)\n", optarg );
                  };
                  printf( "sigma rect_x=%d rect_y=%d\n", rect_x, rect_y );
                  // static double med_buf[5000*5000];
                  // int med_num = 0;
                  
                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         int med_num = 0;
                         int i1 = i - rect_y/2;
                         int i2 = i1 + rect_y;
                         int j1 = j  - rect_x/2;
                         int j2 = j1 + rect_x;
                         if( i1 < 0 ) i1 = 0;
                         if( i2 > inp_proj.lines() ) i2 = inp_proj.lines();
                         if( j1 < 0 ) j1 = 0;
                         if( j2 > inp_proj.cols() )  j2 = inp_proj.cols();
                         double sum_f = 0;
                         double num_f = 0;
                         double sum2_f = 0;
                         for( unsigned int i3 = i1; i3 < i2; i3++ ){
                             for( unsigned int j3 = j1; j3 < j2; j3++ ){
                                if( inp_proj.raw[i3][j3] < 0 ) continue;
                                sum_f += inp_proj.geo[i3][j3];
                                num_f += 1;
                             }
                         }
                         for( unsigned int i3 = i1; i3 < i2; i3++ ){
                             for( unsigned int j3 = j1; j3 < j2; j3++ ){
                                if( inp_proj.raw[i3][j3] < 0 ) continue;
                                sum2_f += (inp_proj.geo[i3][j3] - sum_f/num_f)*
                                          (inp_proj.geo[i3][j3] - sum_f/num_f);
                                //num_f += 1;
                             }
                         }
                         if( num_f > 0 ){
                             out_proj.geo[i][j] = sqrt(sum2_f / num_f);
                             out_proj.raw[i][j] = 0;
                         }
                     }
                  }
                  TLab34NProj::copyFromTo( out_proj, inp_proj );
               }
               break;;
           case 202:
               {
                  TLab34NProj out_proj;
                  TLab34NProj::copyFromTo( inp_proj, out_proj );
                  int rect_x = 0;
                  int rect_y = 0;
                  if( sscanf( optarg, "%dx%d", &rect_x, &rect_y ) != 2 ){
                     fprintf( stderr, "Ошибка разбора ЧИСЛОxЧИСЛО %s (опция median-rect)\n", optarg );
                  };
                  //printf( "median rect_x=%d rect_y=%d\n", rect_x, rect_y );
                  static double med_buf[5000*5000];
                  int med_num = 0;
                  for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                     for( unsigned int j = 0; j < inp_proj.cols(); j++ ){
                         int med_num = 0;
                         int i1 = i - rect_y/2;
                         int i2 = i1 + rect_y;
                         int j1 = j  - rect_x/2;
                         int j2 = j1 + rect_x;
                         if( i1 < 0 ) i1 = 0;
                         if( i2 > inp_proj.lines() ) i2 = inp_proj.lines();
                         if( j1 < 0 ) j1 = 0;
                         if( j2 > inp_proj.cols() )  j2 = inp_proj.cols();
                         double s = 0.0;
                         int n = 0;
                         for( unsigned int i3 = i1; i3 < i2; i3++ ){
                             for( unsigned int j3 = j1; j3 < j2; j3++ ){
                                if( inp_proj.raw[i3][j3] < 0 ) continue;
                                s += inp_proj.geo[i3][j3];
                                n += 1;
                             }
                         }
                         if( n > 0 ){
                             out_proj.geo[i][j] = s/n;
                             out_proj.raw[i][j] = 1;
                         }
                     }
                  }
                  TLab34NProj::copyFromTo( out_proj, inp_proj );
               }
               break;;
           case 203:
               {
                   static double med_buf[5000*5000];
                   int med_num = 0;
                   TLab34NProj out_proj;
                   TLab34NProj::copyFromTo( inp_proj, out_proj );
                   for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                      int med_num = 0;
                      for( unsigned int j3 = 0; j3 < inp_proj.cols(); j3++ ){
                         if( inp_proj.raw[i][j3] < 0 ) continue;
                         med_buf[med_num] = inp_proj.geo[i][j3];
                         med_num += 1;                     
                      }
                      qsort( med_buf, med_num, sizeof(med_buf[0]), double_compare );
                      //printf( "%d %d\n", i, med_num );
                      if( med_num > 0 ){
                         double a = 0;
                         if( med_num % 2 == 0 ){
                            a=(med_buf[med_num/2] + med_buf[med_num/2-1])/2.;
                         }
                         if( med_num % 2 == 1 ){
                            a=med_buf[med_num/2];
                         }
                         double Q25= med_buf[med_num/4];
                         double Q75= med_buf[3*med_num/4];
                         double Q40= med_buf[med_num*4/10];
                         double Q60= med_buf[med_num*6/10];

                         printf( "line= % 5d Q25= %lf Q40= %lf MED= %lf Q60= %lf Q75= %lf N=%d\n", \
                                           i,     Q25,     Q40,     a,       Q60,     Q75, med_num );
                         for( unsigned int j3 = 0; j3 < inp_proj.cols(); j3++ ){                                                            out_proj.geo[i][j3] = a;
                            out_proj.raw[i][j3] = 1;                              
                         }
                      }
                   }
                   TLab34NProj::copyFromTo( out_proj, inp_proj ); 
               }
               break;;
           case 204:
               {
                   static double med_buf[5000*5000];
                   int med_num = 0;
                   TLab34NProj out_proj;
                   TLab34NProj::copyFromTo( inp_proj, out_proj );
                   for( unsigned int j3 = 0; j3 < inp_proj.cols(); j3++ ){
                      int med_num = 0;
                      for( unsigned int i = 0; i < inp_proj.lines(); i++ ){
                         if( inp_proj.raw[i][j3] < 0 ) continue;
                         med_buf[med_num] = inp_proj.geo[i][j3];
                         med_num += 1;                     
                      }
                      qsort( med_buf, med_num, sizeof(med_buf[0]), double_compare );
                      //printf( "%d %d\n", i, med_num );
                      if( med_num > 0 ){
                         double a = 0;
                         if( med_num % 2 == 0 ){
                            a=(med_buf[med_num/2] + med_buf[med_num/2-1])/2.;
                         }
                         if( med_num % 2 == 1 ){
                            a=med_buf[med_num/2];
                         }
                         double Q25= med_buf[med_num/4];
                         double Q75= med_buf[3*med_num/4];
                         double Q40= med_buf[med_num*4/10];
                         double Q60= med_buf[med_num*6/10];

                         printf( "col= % 5d Q25= %lf Q40= %lf MED= %lf Q60= %lf Q75= %lf N=%d\n", \
                                         j3,     Q25,     Q40,     a,       Q60,     Q75, med_num );

                         for( unsigned int i = 0; i < inp_proj.lines(); i++ ){                                                              out_proj.geo[i][j3] = a;
                            out_proj.raw[i][j3] = 1;                              
                         }
                      }
                   }
                   TLab34NProj::copyFromTo( out_proj, inp_proj ); 
               }
               break;;
           case 205:
               {
                  TLab34NProj out_proj;
                  TLab34NProj::copyFromTo( inp_proj, out_proj );
                  for( int i = 0; i < inp_proj.lines(); i++ ){
                     for( int j = 0; j < inp_proj.cols(); j++ ){
                         out_proj.raw[i][j] = -1;
                     }
                  }

                  int X = 0;
                  int Y = 0;
                  if( sscanf( optarg, "%d,%d", &X, &Y ) != 2 ){
                     fprintf( stderr, "Ошибка разбора числа %s (опция shift 10,5)\n", optarg );
                  };

                  for( int i = 0; i < inp_proj.lines(); i++ ){
                     for( int j = 0; j < inp_proj.cols(); j++ ){
                         int i1 = i + Y;
                         int j1 = j + X;
                         if( i1 < 0 || i1 >= inp_proj.lines() ) continue;
                         if( j1 < 0 || j1 >= inp_proj.cols() ) continue;
                         out_proj.geo[i1][j1] = inp_proj.geo[i][j];
                         out_proj.raw[i1][j1] = inp_proj.raw[i][j];
                     }
                  }
                  TLab34NProj::copyFromTo( out_proj, inp_proj );
               }
               break;;
           case 206:
           case 207:
               {
                  TLab34NProj out_proj;
                  TLab34NProj::copyFromTo( inp_proj, out_proj );
                  int rect_size = 0;
                  if( sscanf( optarg, "%d", &rect_size ) != 1 ){
                     fprintf( stderr, "Ошибка разбора ЧИСЛО %s (опция --MNK-v, --MNK-h)\n", optarg );
                  };
                  //printf( " %s rect_size=%d\n", (opt_char == 206 ? "MNK-h" : "MNK-v" ), rect_size );

                  for( int i = 0; i < inp_proj.lines(); i++ ){
                     for( int j = 0; j < inp_proj.cols(); j++ ){
                         int med_num = 0;
                         int rect_x = 1;
                         int rect_y = 1;
                         if( opt_char == 206 ) rect_x = rect_size;
                         if( opt_char == 207 ) rect_y = rect_size;
                         int i1 = i - rect_y/2;
                         int i2 = i1 + rect_y;
                         int j1 = j  - rect_x/2;
                         int j2 = j1 + rect_x;
                         if( i1 < 0 ) i1 = 0;
                         if( i2 > inp_proj.lines() ) i2 = inp_proj.lines();
                         if( j1 < 0 ) j1 = 0;
                         if( j2 > inp_proj.cols() )  j2 = inp_proj.cols();
                         double b1 = 0;
                         double b2 = 0;
                         double b3 = 0;
                         double a11 = 0;
                         double a12 = 0;
                         double a13 = 0;
                         double a21 = 0;
                         double a22 = 0;
                         double a23 = 0;
                         double a31 = 0;
                         double a32 = 0;
                         double a33 = 0;
                         for( int i3 = i1; i3 < i2; i3++ ){
                             for( int j3 = j1; j3 < j2; j3++ ){
                                if( inp_proj.raw[i3][j3] < 0 ) continue;
                                int x = 0;
                                double y = inp_proj.geo[i3][j3];
                                if( opt_char == 206 ) x = j3-j;
                                if( opt_char == 207 ) x = i3-i;
                                b1 += y;
                                b2 += y*x;
                                b3 += y*x*x;
                                a11 += 1;
                                a12 += x;
                                a13 += x*x;
                                a21 += x;
                                a22 += x*x;
                                a23 += x*x*x;
                                a31 += x*x;
                                a32 += x*x*x;
                                a33 += x*x*x*x;
                             }
                         }
                         double D  = a11*(a23*a32-a22*a33)+a12*(a21*a33-a23*a31)+a13*(a22*a31-a21*a32);
                         double D1 = a12*(a33*b2-a23*b3)+a13*(a22*b3-a32*b2)+(a23*a32-a22*a33)*b1;
                         double D2 = -1*(a11*(a33*b2-a23*b3)+a13*(a21*b3-a31*b2)+(a23*a31-a21*a33)*b1);
                         double D3 = a11*(a32*b2-a22*b3)+a12*(a21*b3-a31*b2)+(a22*a31-a21*a32)*b1;
                         double a = D1/D;
                         out_proj.geo[i][j] = a;
                         // printf( "%d %d %lf\n", i, j, a );
                     }
                  }
                  TLab34NProj::copyFromTo( out_proj, inp_proj );
               }
               break;;
           case 208:
               {
                  char *txt_file_name = optarg;
                  FILE *txt_file = fopen( txt_file_name, "r" );
                  if( txt_file == NULL ){
                      perror( txt_file_name );
                      exit(1);
                  }
                  static char buf[2000];
                  while( fgets( buf, 1998, txt_file ) != NULL ){
                      int row = 0;
                      double value = 0.0;
                      char *p = buf;
                      while( *p != '\000' && isspace(*p) ){
                          p++;
                      }
                      if( *p == '\000' ){
                          continue;
                      }
                      if( sscanf( p, "%d %lf", &row, &value ) != 2 ){
                         fprintf( stderr, "Ошибка строки <%s> файла <%s> (ожидается 3 201.1 )\n", buf, txt_file_name );
                         exit(1);
                      }
                      if( row < 0 || row >= inp_proj.lines() ){
                         fprintf( stderr, \
                             "Неверное значение номера строки в файле файла <%s> (указано %d)\n", txt_file_name, row );
                         continue;
                      }
                      for( int j = 0; j < inp_proj.cols(); j++ ){
                         inp_proj.raw[row][j] = 1;
                         inp_proj.geo[row][j] = value;
                      }
                  }
               }
               break;;
           case 210:
               {
                  char *txt_file_name = optarg;
                  FILE *txt_file = fopen( txt_file_name, "r" );
                  if( txt_file == NULL ){
                      perror( txt_file_name );
                      exit(1);
                  }
                  static char buf[2000];
                  while( fgets( buf, 1998, txt_file ) != NULL ){
                      int col = 0;
                      double value = 0.0;
                      char *p = buf;
                      while( *p != '\000' && isspace(*p) ){
                          p++;
                      }
                      if( *p == '\000' ){
                          continue;
                      }
                      if( sscanf( p, "%d %lf", &col, &value ) != 2 ){
                         fprintf( stderr, "Ошибка строки <%s> файла <%s> (ожидается 3 201.1 )\n", buf, txt_file_name );
                         exit(1);
                      }
                      if( col < 0 || col >= inp_proj.cols() ){
                         fprintf( stderr, \
                             "Неверное значение номера строки в файле файла <%s> (указано %d)\n", txt_file_name, col );
                         continue;
                      }
                      for( int j = 0; j < inp_proj.lines(); j++ ){
                         inp_proj.raw[j][col] = 1;
                         inp_proj.geo[j][col] = value;
                      }
                  }
               }
               break;;
           case 211:
               {
                   int new_year = 999.0;
                   if( sscanf( optarg, "%d", &new_year ) != 1 ){
                       fprintf( stderr, "Ошибка разбора числа %s (опция set_year)\n", optarg );
                   };
                   inp_proj.blk0.b0.year = new_year;
               }
               break;;
           case 212:
               {
                   int new_year = 999.0;
                   if( sscanf( optarg, "%d", &new_year ) != 1 ){
                       fprintf( stderr, "Ошибка разбора числа %s (опция set_day)\n", optarg );
                   };
                   inp_proj.blk0.b0.day = new_year;
               }
               break;;
           case 213:
               {
                   int new_year = 999.0;
                   if( sscanf( optarg, "%d", &new_year ) != 1 ){
                       fprintf( stderr, "Ошибка разбора числа %s (опция set_daytime)\n", optarg );
                   };
                   inp_proj.blk0.b0.dayTime = new_year;
               }
               break;;
           case 214:
               {
                   int new_year = 999.0;
                   if( sscanf( optarg, "%d", &new_year ) != 1 ){
                       fprintf( stderr, "Ошибка разбора числа %s (опция set_daytime)\n", optarg );
                   };
                   inp_proj.blk0.channel = new_year;
               }
               break;;
           case 225:
               {
                   int new_year = 999.0;
                   if( sscanf( optarg, "%d", &new_year ) != 1 ){
                       fprintf( stderr, "Ошибка разбора числа %s (опция set_sat_id)\n", optarg );
                   };
                   inp_proj.blk0.b0.satId = new_year;
               }
               break;;
           case 216:
               {
                   memset( inp_proj.blk0.b0.satName, 0, 13 );
                   strncpy( inp_proj.blk0.b0.satName, optarg, 12 );
               }
               break;;
           default:
               use_message();
               return(-1);
       }
    }
    if( ! result_saved ){
        puts( "Результат должен быть сохранен. Используете опцию -o .\n"  );
        use_message();
        return(-1);
    }
    return 0;
}
