/*-------------------------------------------------------------------------
    hrpt.hpp
-------------------------------------------------------------------------*/
#ifndef _HRPT_HPP_
#define _HRPT_HPP_

#include <stdio.h>

#include <c_lib.hpp>


class _Export THRPTUnpacker {
public:
// Создание по файлу данных.
// memory_flag  = 0 При распаковке читать файл данных построчно.
//              = 1 Прочитать файл данных заранее, распаковку производить из буфера.
// Исключения:
// TAccessExc( 1, "THRPTUnpacker::THRPTUnpacker: ошибка доступа к файлу" );
    THRPTUnpacker( const char * file_name, int memory_flag ) throw ( TAccessExc );

// Создание по данным, уже загруженным в буфер.
    THRPTUnpacker( char * file_data );

    ~THRPTUnpacker();


	TBlk0* blk0();

    void channelsAvailability( ulong availabilityArray[5] );

// Содержит ли файл данные указанного канала AVHRR.
// channel  Номер канала (0-based).
    int channelAvailable( ulong channel );

    ulong totalFrames();

// Возвращает номер кадра (0-based), который будет получен следующим вызовом функции unpackNextFrame.
    ulong currentFrameNumber();

// frame_num    0..total_frames-1
//
// Исключения:
// TAccessExc( 1, "THRPTUnpacker::setCurrentFrameNumber: ошибка доступа к файлу" )
// TRequestExc( 2, "THRPTUnpacker::setCurrentFrameNumber: кадр с указанным номером в файле отсутствует" )
    void setCurrentFrameNumber( ulong frame_num ) throw( TAccessExc, TRequestExc );

//-------------------------------------------------------------------------
// mask
//
// Функция возвращает маску малых кадров HRPT.
//-------------------------------------------------------------------------	
	ulong mask();

//-------------------------------------------------------------------------
// frameWordLength
//
// Функция возвращает размер распакованного малого кадра HRPT, выраженный в
// 2-х байтовых словах.
//-------------------------------------------------------------------------
	ulong frameWordLength();
    ulong packedFrameWordLength();

//-------------------------------------------------------------------------
// unpackNextFrame
//
// Функция распаковывает очередной малый кадр HRPT и выделяет из него
// данные каналов AVHRR и телеметрии.
// В случае, если обработаны все малые кадры HRPT файла, функция ничего не
// делает.
// Если при приеме кадра произошел сбой синхронизации, во все указанные в
// массиве avhrrBuffers буфера записывается -1, а в остальные - 0.
//
// Прототип:
// void unpackNextFrame( USHORT* avhrrBuffers[5], USHORT* tlmBuffers[3], USHORT* tipBuffer ) throw ( TException )
// Параметры:
// avhrrDataBuffers     Массив указателей на буфера размером totalPix*2 байт под
//                      распакованные данные AVHRR. Любой из его элементов
//                      может быть равен NULL.
// tlmBuffers			Массив указателей на буфера размером 60 байт под
//                      данные телеметрии 3,4,5 каналов. Любой из его
//                      элементов может быть равен NULL.
// tipBuffer			Указатель на массив размером 1010 байт (505 слов, или 5 кадров по 101 слову) под данные TIP.
//						Модет быть равен NULL.
//
// Исключения:
// TAccessExc( 1, "THRPTUnpacker::unpackNextFrame: ошибка доступа к файлу" )
// TRequestExc( 2, "THRPTUnpacker::unpackNextFrame: кадр со сбоем синхронизации" )
//-------------------------------------------------------------------------
    void unpackNextFrame( ushort * avhrrBuffers[5], ushort * tlmBuffers[3], ushort * tipBuffer = 0 ) throw ( TAccessExc, TRequestExc );

//-------------------------------------------------------------------------
// unpackNextFrame
//
// Функция распаковывает весь малый кадр AVHRR, не выделяя из него данные
// отдельных полей.
// Прототип:
// void unpackNextFrame( USHORT* buffer ) throw( TException )
// Параметры:
// buffer	Указатель на буфер под распаковываемые данные. Размер буфера
//			можно узнать при помощи функции frameWordLength().
//
// Исключения:
// TAccessExc( 1, "THRPTUnpacker::unpackNextFrame: ошибка доступа к файлу" )
//-------------------------------------------------------------------------
	void unpackNextFrame( ushort * buffer ) throw( TAccessExc );

private:
    void init( char * buf );

/*-------------------------------------------------------------------------
   Прототип:
   unsigned buildMask ( uchar * mask, unsigned code, unsigned nPassPix, unsigned nPix )
   Параметры:
        mask   Байтовый массив для маски. Если равен 0, построения маски
               не происходит.
        code    битовый код построения маски;
        биты:   0 - IDN -идентификатор борта
            1 - TCD -код времени
            2 - TLM -телеметрия
            3 - BSC -сканирование черного тела
            4 - SPC -сканирование космоса (SPACE)
            5 - HSY -синхросерия AVHRR
            6 - TIP -информ. пакет TIROS (без синхросерий)
            7 - SPR -резерв
            8 - AVHRR
            9 - ASY -синхросерия конца кадра
               10 - FSY -синхросерия начала кадра
               11 - канал 5 AVHRR
               12 - канал 4 AVHRR
               13 - канал 3 AVHRR
               14 - канал 2 AVHRR
               15 - канал 1 AVHRR
        nPassPix    число пропускаемых пикселов от начала строки AVHRR
        nPix        кол-во принимаемых пикселов строки AVHRR
    Возвращает: кол-во записываемых слов из кадра HRPT

    Примечание: значение 1 конкретного бита кода маски означает запись
    полностью соответствующего сегмента из кадра HRPT, кроме TIP-а.
-------------------------------------------------------------------------*/
    unsigned buildMask( uchar *, unsigned, unsigned, unsigned );

    FILE *  fFile;

    int     fFileFormat;    // Формат файла. 1 - файл с 0-блоком нового формата, 2 - файл с 0-блоком старого формата.

    ulong   fTotalFrames;  // Число малых кадров в файле данных.

    ulong   fNChannels;    // Число каналов AVHRR, присутствующих в файле.
    int     fChannels[5];  // Флаги наличия каналов.
    ulong   fUnpackedFrameLength;  // Длина в словах распакованного кадра.
    ulong   fPackedFrameLength;    // Длина в словах нераспакованного кадра.
    ulong   fAVHRROffset;	// Смещение данных AVHRR в кадре, выраженное в словах.
	ulong   fTLMOffset;	// Смещение данных телеметрии в кадре, выраженное в словах.
	ulong   fTIPOffset;	// Смещение данных TIP в кадре, выраженное в словах.
    ulong   fMask;
	ulong   fTotalPix;
    ulong   fPixPassed;
    ulong   fPix;
    ulong   fPackType;

    char *  fBuf;
    TBlk0 * fBlk0;
    ushort * fData;
    ushort * fUnpackedFrame;

    ushort * fCurPackedFrame;
    unsigned fCurFrameNum; // Номер кадра, который будет получен следующим вызовом функции unpackNextFrame.

    int     fExtBufFlag;   // 0 - буфер под данные заведены самим объектом; 1 - буфер с данными файла получен извне
};

#endif
