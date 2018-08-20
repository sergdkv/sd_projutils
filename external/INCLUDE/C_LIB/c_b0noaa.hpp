/*-------------------------------------------------------------------------
    c_b0noaa.hpp
-------------------------------------------------------------------------*/
#ifndef _C_B0NOAA_HPP_
#define _C_B0NOAA_HPP_


#pragma pack(1)

/*-------------------------------------------------------------------------
    TBlk0_HRPT
    Формат 0-блока файлов упакованных данных HRPT.
-------------------------------------------------------------------------*/
struct TBlk0_HRPT {
    TBlk0_Common b0;

/*
Значения полей frameNum и lostFrameNum в сумме дают общее количество строк
в файле.
*/
	uint16  frameNum;       /* 64   Количество принятых строк. */
	uint16  lostFrameNum;   /* 66   Количество строк со сбоем синхронизации. */
/*
Следующие три поля описывают качество данных в файле.
Среди принятых могут быть строки, у которых было сбито поле времени. После
приема оно восстанавливается. Количество таких строк в файле записывается в
поле correctedTimeFrameNum.
Поле gapNum содержит количество групп строк со сбоем синхронизации. Суммарное
число строк в этих группах равно lostFrameNum.
*/
    uint16  validTimeFrameNum;      /* 68   Количество строк, изначально принятых с правильным значением поля времени. */
    uint16  correctedTimeFrameNum;  /* 70   Количество строк, изначально принятых с неправильным значением поля времени, которое затем было исправлено. */
    uint16  gapNum;         /* 72   Количество пропусков данных (gaps). Общее количество кадров в этих дырах равно lostFrameNum. */

    uint16  packType;       /* 74   Тип упаковки. 0 - 1*10->2b, 1 - 3*10->4b, 2 - 8*10->10b */
    uint16  totalPixNum;    /* 76   Полная длина строки в пикселах. Для NOAA всегда 2048. */
    uint32   frameMask;      /* 78   Маска кадра HRPT. */
    uint16  pixGap;         /* 82   Количество пропущенных пикселов от начала строки. */
    uint16  pixNum;         /* 84   Количество принятых пикселов строки. */
    uint16  ascendFlag;     /* 86   Тип витка: 0 - нисходящий, 1 - восходящий. */
    uchar   reserved1[40];  /* 88 */

/* Телеграмма NORAD. */
    uint32   revNum;         /* 128  Номер опорного витка. */
    uint16  setNum;         /* 132  Номер набора. */
    uint16  ephemType;      /* 134  Тип эфемерид. */
    uint16  year;           /* 136  Год. */
    double  yearTime;       /* 138  Полное время в году, выраженное в днях (1-based). */
    double  n0;             /* 146  Среднее движение (рад/мин). */
    double  bstar;          /* 154  Сопротивление атмосферы. */
    double  i0;             /* 162  Наклонение орбиты (рад). */
    double  raan;           /* 170  Прямое восхождение (рад). */
    double  e0;             /* 178  Эксцентриситет. */
    double  w0;             /* 186  Аргумент перигея (рад). */
    double  m0;             /* 194  Средняя аномалия (рад). */

    uchar   reserved2[54];  /* 202 */

/*
Параметры коррекции географической привязки. Непосредственно после приема
все эти поля равны 0. Вычисляются при обработке программой коррекции привязки.
*/
    uint16  corVersion;     /* 256  Номер версии коррекции, 0 - ее отсутствие. */
    int16   corTBUSTime;    /* 258  Поправка времени, вычисленная по телеграмме TBUS (мсек). */
    int16   corTime;        /* 260  Поправка времени, вычисленная иным способом (мсек). */
    double  corRoll;        /* 262  Угол крена (рад). */
    double  corPitch;       /* 270  Угол тангажа (рад). */
    double  corYaw;         /* 278  Угол рысканья (рад). */

    uchar   spare[512-286];
};


/*-------------------------------------------------------------------------
    TBlk0_AVHRR
    Формат переменной части 0-блока файлов распакованных данных одного
    канала AVHRR (одноканальных файлов данных).

    Пикселы изображения, хранящегося в файле этого формата, имеют тип
    знаковое_двухбайтовое_целое и делятся на значимые и незначимые.
    Значимые пикселы сформированы сканером спутника или получены из них в
    результате обработки и имеют значения >=0. Незначимые пикселы - это
    пикселы сбойных строк или масок; имеют значения <0.
-------------------------------------------------------------------------*/
struct TBlk0_AVHRR {
    TBlk0_Common b0;

/*
Поле processLevel является набором битовых флагов. Служит для фиксации стадий
обработки данных. После создания файла утилитой распаковки это поле равно 0
(необработанные данные). При последующей обработке обновляется соответствующими
утилитами или процедурами.
*/
    uint32   processLevel;        /* 64 */
/*
Номер канала. Для данных NOAA AVHRR допустимы значения 1,2,3,4,5. После
процедуры многоканальной атмосферной коррекции, где используются данные 4 и 5
каналов, это поле равно 4.
*/
    uint16  channel;        /* 68 */
/*
Общее количество строк. Равно сумме значений полей frameNum и lostFrameNum
исходного файла данных HRPT (см. описание структуры TBlk0_HRPT).
*/
    uint16  totalFrameNum;  /* 70 */
    uint16  totalPixNum;    /* 72 */
    uint16  pixGap;         /* 74 */
    uint16  pixNum;         /* 76 */
    uint16  ascendFlag;     /* 78 */
/*
    Поле maxPixelValue содержит максимальную величину среди ЗНАЧИМЫХ пикселов изображения.
*/
    uint16  maxPixelValue;  /* 80 */
/*
Поля ka и kb содержат значения коэффициентов пересчета значения пиксела в
физическую величину. Формула пересчета: t = ka*p + kb, где p - значение пиксела.
После создания файла утилитой распаковки значения этих полей равны 0. При
дальнейшей обработке эти поля содержат такие значения, при которых значения
пикселов изображения >= 0 (ЗАМЕЧАНИЕ: при этом в изображении ДОЛЖНЫ БЫТЬ
пикселы, равные 0). Значения пикселов >= 32768 (0x8000) используются для целей
маскирования.
*/
    double  ka;             /* 82 */
    double  kb;             /* 90 */

	uchar   reserved1[30];  /* 98 */

/* Телеграмма NORAD. */
	uint32   revNum;         /* 128 */
	uint16  setNum;         /* 132 */
	uint16  ephemType;      /* 134 */
	uint16  year;           /* 136 */
	double  yearTime;       /* 138 */
	double  n0;             /* 146 */
	double  bstar;          /* 154 */
	double  i0;             /* 162 */
	double  raan;           /* 170 */
	double  e0;             /* 178 */
	double  w0;             /* 186 */
	double  m0;             /* 194 */

	uchar   reserved2[54];  /* 202 */

/* Параметры коррекции географической привязки. */
	uint16  corVersion;     /* 256 */
	int16   corTBUSTime;    /* 258 */
	int16   corTime;        /* 260 */
	double  corRoll;        /* 262 */
	double  corPitch;       /* 270 */
	double  corYaw;         /* 278 */

    uchar   spare[512-286];
};




/*-------------------------------------------------------------------------
	TBlk0_TLM
	Структура, описывающая формат переменной части 0-блока для файлов данных
	телеметрии NOAA.
-------------------------------------------------------------------------*/
struct TBlk0_TLM {
    TBlk0_Common b0;

    uint16  totalFrameNum;  /* 64 */
    uint16	channel;		/* 66 	Номер канала (1,2,3,4,5). */

    uchar   spare[512-68];
};


/*-------------------------------------------------------------------------
    TOldBlk0
    Формат паспорта файлов данных, использовавшийся до 1999 года.
-------------------------------------------------------------------------*/
struct TOldBlk0 {
        uchar day;       /* 0    Число (1..31). */
        uchar month;     /* 1    Месяц (1..12). */
        uchar year;      /* 2    Год ( начиная от 1900 ). */
        uchar satNumber; /* 3    бит 0: 0 - канал A, канал B (только для APT); биты 1-6: номер ИСЗ; бит 7: 0-APT, 1-HRPT */
        uchar hour;      /* 4 */
        uchar minute;    /* 5 */
        uchar sec;       /* 6 */
        uchar tic;       /* 7 */
        uint16 revNum;      /* 8    Номер витка. */
        uint16 totalScans;  /* 10   Число сканов в снимке. */
/* В полях unused3 и unused4 располагается телеграмма APT. */
        uchar unused3[40];
        uchar satType;       /* 52    Тип спутника ( 0 - NOAA, 1 - GMS ). */
        uchar unused35[1];
        uchar launchYear;    /* 54    Год запуска спутника ( начиная от 1900 ). */
        uchar unused4[129];
/*-------------------------------------------------------------------------*/
        float minT;         /* 184   Минимальная температура после калибровки. */
        float stepT;        /* 188   Шаг по температуре. */
        uchar chNumber;      /* 192   Номер канала. */
        uchar calCode;       /* 193   Код калибровки. */
        uchar numberOfClasses;   /* 194 Число классов после классификации, <= 20. */
        uchar classes[60];       /* Классы: [0] код класса */
                                /*         [1] min высота ( в сотнях метров ) */
                                /*         [2] max высота ( в сотнях метров ) */
        uchar    unused10[1];
/*-------------------------------------------------------------------------*/
        uchar typeOfFile; /* 256 Тип файла:
                            0 - APT
                            1 - HRPT
                            2 - unpacked HRPT ( 2 byte/pixel )
                            3 - unpacked HRPT ( 1 byte/pixel )
                            4 - GMS LRFAX
                            5 - GMS HRFAX
                            6 - Mercator projection
                            7 - Equdist. projection */
        uchar typeOfPack; /* 257 Тип упаковки HRPT: 0 - 1*10->2b, 1 - 3*10->4b, 2 - 8*10->10b */
        uint16 numberOfFrames;  /* 258 Количество малых кадров HRPT. */
        uint16 frameLength;     /* 260 Длина кадра HRPT. */
        uchar unused11[2];       /* 262 */
        uint16 frameSegMask;    /* 264 Маска сегментов кадра HRPT. */
        uint16 pixStrPassed;    /* 266 Число пропущенных пикселов от начала строки AVHRR. */
        uint16 pixStrLength;    /* 268 Число принятых пикселов строки AVHRR. */
/*-------------------------------------------------------------------------*/
        uint16 projPixStrLength;    /* 270 Число пикселов в строке проекции. */
        uint16 projNumberOfScans;   /* 272 Число строк проекции. */
        uint16 projPixelBytes;      /* 274 Число байт на пиксел проекции. */
        float projPixelSize;        /* 276 Размер одного пиксела проекции ( град. ) */
        float minProjLat;           /* 280 */
        float maxProjLat;           /* 284 */
        float minProjLon;           /* 288 */
        float maxProjLon;           /* 292 */
        uint16 hProjShift;          /* 296 Сдвиг проекции по горизонтали. */
        uint16 vProjShift;          /* 298 Сдвиг проекции по вертикали. */
        uint16 processState;        /* 300 */
        float mKHR;                 /* 302 Среднее по фрагменту отношение высоты ИСЗ над Землей к радиусу орбиты. */
        uint16 hPolyDegree;
        uint16 vPolyDegree;
        float polyCoeffs[20];
/* Телеграмма NORAD. ----------------------------------------------------*/
        uint16 nSatnum;     /* 390 номер спутника */
        uint32 nRevnum;      /* 392 номер витка */
        uint16 nElnum;      /* 396 номер набора элементов */
        uint16 nEtype;      /* 398 тип эфемерид */
        uint16 nYear;       /* 400 год */
        uint16 nDay;        /* 402 день в году */
        double nDayfrac;    /* 404 время дня */
        double nN0;         /* 412 среднее движение */
        double nBSTAR;      /* 420 */
        double nI0;         /* 428 наклонение */
        double nRAAN;       /* 436 долгота восходящего узла */
        double nE0;         /* 444 эксцентриситет */
        double nW0;         /* 452 аргумент перигея */
        double nA0;         /* 460 большая полуось */
        double nM0;         /* 468 средняя аномалия */
        uchar unused15[36];  /* 476 */
};


#pragma pack()

#endif
