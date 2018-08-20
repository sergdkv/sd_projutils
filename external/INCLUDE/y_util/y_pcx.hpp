/*--------------------------------------------------------------------------
    y_pcx.hpp
--------------------------------------------------------------------------*/
#ifndef _Y_PCX_HPP_
#define _Y_PCX_HPP_


#pragma pack(1)
struct TPCXHdr {
    uchar manuf;
    uchar hard;
    uchar encod;
    uchar bitpx;
    ushort x1;
    ushort y1;
    ushort x2;
    ushort y2;
    ushort hres;
    ushort vres;
    uchar pal[48];
    uchar vmode;
    uchar nplanes;
    ushort bplin;
    ushort palinfo;
    ushort shres;
    ushort svres;
    char unused[54];
};
#pragma pack()


// Структура заголовка grayscale PCX.
#pragma pack(1)
struct TGrayscalePCXHdr {
    unsigned char manuf;                 // +0
    unsigned char hard;                  // +1
    unsigned char encod;                 // +2
    unsigned char bitpx;                 // +3
    unsigned short x1;                  // +4
    unsigned short y1;                  // +6
    unsigned short x2;                  // +8
    unsigned short y2;                  // +10
    unsigned short hres;                // +12
    unsigned short vres;                // +14

    unsigned char blk0_day;              // +16
    unsigned char blk0_month;            // +17
    unsigned char blk0_year;             // +18
    unsigned char blk0_satNumber;        // +19
    unsigned char blk0_hour;             // +20
    unsigned char blk0_minute;           // +21
    unsigned char blk0_second;           // +22
    unsigned char blk0_tic;              // +23
    unsigned short blk0_revNum;         // +24
    unsigned short blk0_numberOfScans;  // +26

    float fTime;                // +28
    float fRoll;                // +32
    float fPitch;               // +36
    float fYaw;                 // +40

    unsigned short fNoradYear;          // +44  полностью
    unsigned short fNoradDay;           // +46
    double fNoradDayFraction;   // +48
    double fNoradN0;            // +56

    unsigned char fCorrectionVersion;    // +64

    unsigned char nplanes;               // +65
    unsigned short bplin;               // +66
    unsigned short palinfo;             // +68
    unsigned short fInfoBarHeight;      // +70  Высота информационной панели.
    unsigned short svres;               // +72

    unsigned short fOptimalLowBound;    // +74
    unsigned short fOptimalHighBound;   // +76
    char fIdent[2];             // +78

    double fNoradBSTAR;         // +80
    double fNoradI0;            // +88
    double fNoradRAAN;          // +96
    double fNoradE0;            // +104
    double fNoradW0;            // +112
    double fNoradM0;            // +120
};
#pragma pack()


#pragma pack(1)
struct PCXRGB {
    uchar r;
    uchar g;
    uchar b;
};
#pragma pack()


/*-----------------------------------------------------------------------------
    Параметры:
    dst Указатель на буфер, в который нужно будет поместить PCX-упакованную
        область.
        Должен иметь размер 2*src_len, если src_len - чётное, и 2*src_len+1,
        если src_len - нечётное, так как в худшем случае именно столько байт
        будет туда записано.
    src Указатель на буфер со строкой-источником.
    src_len Объем данных в src, который нужно упаковать. Может быть нечётным,
            в этом случае функция поместит в конец dst нулевой байт.
    Возвращаемое значение: объем данных, помещенных в dst.
-----------------------------------------------------------------------------*/
ulong _Export pcx_pack_str( char *dst, char *src, ulong src_len );


/*
    Функция для инициализации структуры заголовка PCX-файла.
*/
void _Export pcx_ini_hdr_256( TPCXHdr * hdr, ulong width, ulong height );


/*
    Функция для создания 8-бит PCX-файла по массиву данных.

    Параметры:
    file_name       Имя создаваемого PCX-файла.
    hdr             Должным образом проинициалированная структура заголовка
                    PCX-файла. Функция просто записывает его в файл, ничего
                    не изменяя.
    width           Размер строки пикселов в массиве data_buf. Может быть
                    нечётным, в этом случае размер массива data_buf ДОЛЖЕН
                    быть как минимум на 1 байт больше, чем width*height.
    orientation     1   массив данных следует записывать в файл сверху вниз
                    2   снизу вверх

    Возвращаемое значение:
    0       успешно
    не 0    ошибка
*/
int _Export pcx_create_file_256( const char * file_name, const TPCXHdr * hdr, char * data_buf,
        ulong width, ulong height, const PCXRGB pal[256], int orientation );


#endif