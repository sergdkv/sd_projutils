/*-------------------------------------------------------------------------
   tgdb.hpp
-------------------------------------------------------------------------*/
#ifndef _TGDB_HPP_
#define _TGDB_HPP_

#include <c_lib.hpp>


#pragma pack(1)
struct TGDBPoint {
    long lon;
    long lat;
};
#pragma pack()


struct TGDBSegList {
    long rank;
    long length;
    TGDBPoint * points;
    TGDBSegList * next;

    TGDBSegList( long r, long l, TGDBPoint * p, TGDBSegList * n ) : rank(r), length(l), points(p), next(n) {}
};


/*-------------------------------------------------------------------------
    class TGDB
-------------------------------------------------------------------------*/
class _Export TGDB {
public:
/*
    ���������, ���뢠�騩 ����� � �������᪨� ��ꥪ�� �� ⥪�⮢���
    䠩�� �ଠ� CIA_DB.
    ��ࠬ����:
    ranks   �����⥫� �� ���ᨢ ࠭��� ᥣ���⮢, ����� ������ ����
            �� 䠩�� ����. ��᫥���� ����⮬ ���ᨢ� ������ ���� �᫮
            -1, �㦠饥 �ਧ����� ��� ����. �᫨ ��� ��ࠬ��� ࠢ�� 0,
            �� 䠩�� ���� ����㦥�� ᥣ����� ��� ࠭���, �����訥 �
            ������� ��ࠬ��ࠬ� lon,lat,lon_size,lat_size �������᪨�
            ࠩ��.
    lon,lat,lon_size,lat_size    ������ �������᪨� ࠩ��. �������
            ����७�� - 㣫��� ᥪ㭤�. ������ ������� �� �ਭ��� ���
            ������� � �� ���� ��� ����. �᫨ ��� �� ���� �� ��ࠬ��஢
            lon_size ��� lat_size ࠢ�� 0, ���祭�� �� �࠭�栬 ࠩ���
            �� ���뢠��� ���� �ந��������� �� �㤥�.
    �᪫�祭��:
    TAccessExc ae1( 1, "TGDB::TGDB: �訡�� ����㯠 � 䠩�� ����" );
*/
    TGDB( const char * file_name, long * ranks = 0, long lon = 0, long lat = 0, long lon_size = 0, long lat_size = 0 ) throw ( TAccessExc );

// ���������, �ᯮ����騩 ����� �� ᠬ�� dll.
    TGDB( long * ranks = 0, long lon = 0, long lat = 0, long lon_size = 0, long lat_size = 0 );

    ~TGDB() { removeAllSegments(); }

// ��᫮ ᥣ���⮢, ᮤ�ঠ���� � ��ꥪ�.
    long numberOfSegments() { return fSegNum; }

// ���ᨬ��쭠� ����� ᥣ���� � ����.
    long maxSegmentLength() { return fMaxSegmentLength; }

// �㭪�� ������ ��뢠����, �᫨ �������� ����室������ ����୮�� ����㯠
// � ᥣ���⠬ ����. �ࠧ� ��᫥ ᮧ����� ��ꥪ� ��� �㭪�� ��뢠�� �� �㦭�.
    void again() { fCurSeg = 0; }

// �㭪�� ������ ��뢠���� ��। ��砫�� ����㯠 � ����� ᫥���饣� ᥣ���� ����.
// ������ ��뢠����, ����� �뫨 ���⠭� �� �窨 ⥪�饣� ᥣ����, � ⠪��
// �ࠧ� ��᫥ ᮧ����� ��ꥪ� ��� �맮�� �㭪樨 again() ��� ��砫� ࠡ��� �
// ���� ᥣ���⮬.
// �����頥��� ���祭��:
// 0    �ᯥ譮.
// 1    �뫨 ���⠭� �� ᥣ����� ����.
    int nextSegment();

// ������� ����� � �窠� ᥣ����.
    TGDBPoint * segmentPoints();

// �����頥� ������⢮ �祪 � ⥪�饬 ᥣ���� ����.
    long segmentLength();
// �����頥� ࠭� ⥪�饣� ᥣ���� ����.
    long segmentRank();


protected:

    static long pointNumFromText( const char * hdr_rec_text );
    static long rankFromText( const char * hdr_rec_text );
    static void geoPointFromText( const char * point_rec_text, TGDBPoint * point );
    static int pointBelongsRegion( const TGDBPoint & point, long lon, long lat, long lon_size, long lat_size );

    // ������� ����� ᯨ᪠ ᥣ���⮢.
    void addSegment( TGDBPoint * points, long point_num, long rank );
    // ��������� 㭨�⮦��� ᯨ᮪ ᥣ���⮢.
    void removeAllSegments();


private:
    int fAllocation;        // 1 - ����� �� 䠩��, 2 - ����� �� dll

    long fSegNum;               // �᫮ ᥣ���⮢ � ᯨ᪥
    long fMaxSegmentLength;     // ᠬ� ������ ᥣ����
    TGDBSegList * fSegList;     // ᯨ᮪ ᥣ���⮢
    TGDBSegList * fLastSeg;     // 㪠��⥫� �� ��᫥���� ᥣ���� ᯨ᪠

    TGDBSegList * fCurSeg;      // ⥪�騩 ᥣ����
};

#endif