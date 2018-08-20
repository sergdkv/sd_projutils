/*-------------------------------------------------------------------------
    maskff.hpp
    ����ন� ��।������ �ଠ� 䠩�� ��᪨.
-------------------------------------------------------------------------*/

#pragma pack(1)

struct TMaskFileHdr {
    char signature[5];  // ASCIIZ-��ப� "MASK"
    char dataFormat;    // �ଠ� ������ ��᪨:
                        // 1 - ���ᨢ char
    char projType;      // 1 - ��ઠ��᪠�, 2 - ࠢ���஬����筠�
    char unused;        // ��� ��ࠢ�������; ������ ���� ࠢ�� 0
/*  lon, lat, lonSize, latSize ��।����� �������᪨�  ࠩ��, ��� ���ண� ����஥�� ��᪠;
    ������� ����७�� - �ࠤ���; ������ �������: �� ������ - �� �ਭ���, �� ��� - �� ����. */
    double lon;
    double lat;
    double lonSize;
    double latSize;
    long pixNum;        // �᫮ ���ᥫ�� � ��ப�
    long scanNum;       // �᫮ ��ப
};

#pragma pack()
