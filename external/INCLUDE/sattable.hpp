/*-------------------------------------------------------------------------
	sattable.hpp
-------------------------------------------------------------------------*/
#ifndef _SATTABLE_HPP_
#define _SATTABLE_HPP_


#include <c_lib.hpp>


/*-------------------------------------------------------------------------
    class TSatInfoTable

    ����� ⠡���� ��饩 ���ଠ樨 � ��⭨��.
-------------------------------------------------------------------------*/
class _Export TSatInfoTable {
public:
    TSatInfoTable() : fIndex(-1) {}
    ~TSatInfoTable() {}

    // ID ��⭨���, �� ����� ⠡��� ᮤ�ন� ���ଠ��
	enum NoradSatId {
		sat_id_noaa_9 = 15427,
		sat_id_noaa_10 = 16969,
		sat_id_noaa_11 = 19531,
		sat_id_noaa_12 = 21263,
		sat_id_noaa_14 = 23455,
		sat_id_noaa_15 = 25338,
        sat_id_noaa_16 = 26536,
        sat_id_noaa_17 = 27453
	};

    //---------------------------------------------------------------------
    // �㭪樨 ����㯠 � ����⠬ ⠡���� �१ �� ������.
    //---------------------------------------------------------------------
    // �����頥��� ���祭��: ���� �� ���祭��, ��।����� � NoradSatId.
    ulong satId( ulong index ) const throw( TRequestExc );
    // �����頥��� ���祭��:
    // ��� ��⭨��� NOAA - ��ப� ���� "NOAA-12"
    const char * satName( ulong index ) const throw( TRequestExc );
    const char * fileNamePrefix( ulong index ) const throw( TRequestExc );

    //---------------------------------------------------------------------
    // �㭪樨 ����㯠 � ⠡��� �� ����� ����७���� ����� �����.
    //---------------------------------------------------------------------
    int setToFirst();
    int setToNext();
    int isValid() const;
    // ��⠭����� ����� �� ��⭨� � 㪠����� id.
    // �����頥��� ���祭��:
    // true     ����� ��⠭����� �ᯥ譮. ����� � ⮫쪮 ⮣��, ����� id ࠢ�� ������ �� 童��� NoradSatId (�. ���).
    // false    ����� �� ��⠭�����.
    int setToSatelliteWithId( ulong id );
    int setToSatelliteWithName( const char * );
    ulong satId() const throw( TRequestExc );
    const char * satName() const throw( TRequestExc );
    const char * fileNamePrefix() const throw( TRequestExc );

    // �᫮ ��⭨��� � ⠡���
    static ulong numberOfSatellites();
    // ᮤ�ন� �� ⠡��� ���ଠ�� � ��⭨�� � ����� �����䨪��஬
    static int containsInfoForSatellite( ulong sat_id );

private:
    ulong fIndex;
};


#endif
