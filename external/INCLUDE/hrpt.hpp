/*-------------------------------------------------------------------------
    hrpt.hpp
-------------------------------------------------------------------------*/
#ifndef _HRPT_HPP_
#define _HRPT_HPP_

#include <stdio.h>

#include <c_lib.hpp>


class _Export THRPTUnpacker {
public:
// �������� �� 䠩�� ������.
// memory_flag  = 0 �� �ᯠ����� ���� 䠩� ������ �����筮.
//              = 1 ������ 䠩� ������ ��࠭��, �ᯠ����� �ந������� �� ����.
// �᪫�祭��:
// TAccessExc( 1, "THRPTUnpacker::THRPTUnpacker: �訡�� ����㯠 � 䠩��" );
    THRPTUnpacker( const char * file_name, int memory_flag ) throw ( TAccessExc );

// �������� �� �����, 㦥 ����㦥��� � ����.
    THRPTUnpacker( char * file_data );

    ~THRPTUnpacker();


	TBlk0* blk0();

    void channelsAvailability( ulong availabilityArray[5] );

// ����ন� �� 䠩� ����� 㪠������� ������ AVHRR.
// channel  ����� ������ (0-based).
    int channelAvailable( ulong channel );

    ulong totalFrames();

// �����頥� ����� ���� (0-based), ����� �㤥� ����祭 ᫥���騬 �맮��� �㭪樨 unpackNextFrame.
    ulong currentFrameNumber();

// frame_num    0..total_frames-1
//
// �᪫�祭��:
// TAccessExc( 1, "THRPTUnpacker::setCurrentFrameNumber: �訡�� ����㯠 � 䠩��" )
// TRequestExc( 2, "THRPTUnpacker::setCurrentFrameNumber: ���� � 㪠����� ����஬ � 䠩�� ���������" )
    void setCurrentFrameNumber( ulong frame_num ) throw( TAccessExc, TRequestExc );

//-------------------------------------------------------------------------
// mask
//
// �㭪�� �����頥� ���� ����� ���஢ HRPT.
//-------------------------------------------------------------------------	
	ulong mask();

//-------------------------------------------------------------------------
// frameWordLength
//
// �㭪�� �����頥� ࠧ��� �ᯠ��������� ������ ���� HRPT, ��ࠦ���� �
// 2-� ���⮢�� ᫮���.
//-------------------------------------------------------------------------
	ulong frameWordLength();
    ulong packedFrameWordLength();

//-------------------------------------------------------------------------
// unpackNextFrame
//
// �㭪�� �ᯠ���뢠�� ��।��� ���� ���� HRPT � �뤥��� �� ����
// ����� ������� AVHRR � ⥫����ਨ.
// � ��砥, �᫨ ��ࠡ�⠭� �� ���� ����� HRPT 䠩��, �㭪�� ��祣� ��
// ������.
// �᫨ �� �ਥ�� ���� �ந��襫 ᡮ� ᨭ�஭���樨, �� �� 㪠����� �
// ���ᨢ� avhrrBuffers ���� �����뢠���� -1, � � ��⠫�� - 0.
//
// ���⨯:
// void unpackNextFrame( USHORT* avhrrBuffers[5], USHORT* tlmBuffers[3], USHORT* tipBuffer ) throw ( TException )
// ��ࠬ����:
// avhrrDataBuffers     ���ᨢ 㪠��⥫�� �� ���� ࠧ��஬ totalPix*2 ���� ���
//                      �ᯠ������� ����� AVHRR. �� �� ��� ����⮢
//                      ����� ���� ࠢ�� NULL.
// tlmBuffers			���ᨢ 㪠��⥫�� �� ���� ࠧ��஬ 60 ���� ���
//                      ����� ⥫����ਨ 3,4,5 �������. �� �� ���
//                      ����⮢ ����� ���� ࠢ�� NULL.
// tipBuffer			�����⥫� �� ���ᨢ ࠧ��஬ 1010 ���� (505 ᫮�, ��� 5 ���஢ �� 101 ᫮��) ��� ����� TIP.
//						����� ���� ࠢ�� NULL.
//
// �᪫�祭��:
// TAccessExc( 1, "THRPTUnpacker::unpackNextFrame: �訡�� ����㯠 � 䠩��" )
// TRequestExc( 2, "THRPTUnpacker::unpackNextFrame: ���� � ᡮ�� ᨭ�஭���樨" )
//-------------------------------------------------------------------------
    void unpackNextFrame( ushort * avhrrBuffers[5], ushort * tlmBuffers[3], ushort * tipBuffer = 0 ) throw ( TAccessExc, TRequestExc );

//-------------------------------------------------------------------------
// unpackNextFrame
//
// �㭪�� �ᯠ���뢠�� ���� ���� ���� AVHRR, �� �뤥��� �� ���� �����
// �⤥���� �����.
// ���⨯:
// void unpackNextFrame( USHORT* buffer ) throw( TException )
// ��ࠬ����:
// buffer	�����⥫� �� ���� ��� �ᯠ���뢠��� �����. ������ ����
//			����� 㧭��� �� ����� �㭪樨 frameWordLength().
//
// �᪫�祭��:
// TAccessExc( 1, "THRPTUnpacker::unpackNextFrame: �訡�� ����㯠 � 䠩��" )
//-------------------------------------------------------------------------
	void unpackNextFrame( ushort * buffer ) throw( TAccessExc );

private:
    void init( char * buf );

/*-------------------------------------------------------------------------
   ���⨯:
   unsigned buildMask ( uchar * mask, unsigned code, unsigned nPassPix, unsigned nPix )
   ��ࠬ����:
        mask   ���⮢� ���ᨢ ��� ��᪨. �᫨ ࠢ�� 0, ����஥��� ��᪨
               �� �ந�室��.
        code    ��⮢� ��� ����஥��� ��᪨;
        ����:   0 - IDN -�����䨪��� ����
            1 - TCD -��� �६���
            2 - TLM -⥫������
            3 - BSC -᪠��஢���� �୮�� ⥫�
            4 - SPC -᪠��஢���� ��ᬮ� (SPACE)
            5 - HSY -ᨭ����� AVHRR
            6 - TIP -����. ����� TIROS (��� ᨭ���਩)
            7 - SPR -१��
            8 - AVHRR
            9 - ASY -ᨭ����� ���� ����
               10 - FSY -ᨭ����� ��砫� ����
               11 - ����� 5 AVHRR
               12 - ����� 4 AVHRR
               13 - ����� 3 AVHRR
               14 - ����� 2 AVHRR
               15 - ����� 1 AVHRR
        nPassPix    �᫮ �ய�᪠���� ���ᥫ�� �� ��砫� ��ப� AVHRR
        nPix        ���-�� �ਭ������� ���ᥫ�� ��ப� AVHRR
    �����頥�: ���-�� �����뢠���� ᫮� �� ���� HRPT

    �ਬ�砭��: ���祭�� 1 �����⭮�� ��� ���� ��᪨ ����砥� ������
    ��������� ᮮ⢥�����饣� ᥣ���� �� ���� HRPT, �஬� TIP-�.
-------------------------------------------------------------------------*/
    unsigned buildMask( uchar *, unsigned, unsigned, unsigned );

    FILE *  fFile;

    int     fFileFormat;    // ��ଠ� 䠩��. 1 - 䠩� � 0-������ ������ �ଠ�, 2 - 䠩� � 0-������ ��ண� �ଠ�.

    ulong   fTotalFrames;  // ��᫮ ����� ���஢ � 䠩�� ������.

    ulong   fNChannels;    // ��᫮ ������� AVHRR, ����������� � 䠩��.
    int     fChannels[5];  // ����� ������ �������.
    ulong   fUnpackedFrameLength;  // ����� � ᫮��� �ᯠ��������� ����.
    ulong   fPackedFrameLength;    // ����� � ᫮��� ���ᯠ��������� ����.
    ulong   fAVHRROffset;	// ���饭�� ������ AVHRR � ����, ��ࠦ����� � ᫮���.
	ulong   fTLMOffset;	// ���饭�� ������ ⥫����ਨ � ����, ��ࠦ����� � ᫮���.
	ulong   fTIPOffset;	// ���饭�� ������ TIP � ����, ��ࠦ����� � ᫮���.
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
    unsigned fCurFrameNum; // ����� ����, ����� �㤥� ����祭 ᫥���騬 �맮��� �㭪樨 unpackNextFrame.

    int     fExtBufFlag;   // 0 - ���� ��� ����� �������� ᠬ�� ��ꥪ⮬; 1 - ���� � ����묨 䠩�� ����祭 �����
};

#endif
