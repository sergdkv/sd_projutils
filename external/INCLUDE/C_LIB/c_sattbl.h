/*-----------------------------------------------------------------------------
    c_sattbl.h
-----------------------------------------------------------------------------*/
#ifndef _C_SATTBL_H_
#define _C_SATTBL_H_


#include "c_except.h"

/*-------------------------------------------------------------------------
    class TSatInfoTable

    Класс таблицы общей информации о спутнике.
-------------------------------------------------------------------------*/
class TSatInfoTable {
public:
    TSatInfoTable() : fIndex((ulong)-1) {}
    ~TSatInfoTable() {}

    // ID спутников, по которым таблица содержит информацию
	enum NoradSatId {
		sat_id_noaa_9 = 15427,
		sat_id_noaa_10 = 16969,
		sat_id_noaa_11 = 19531,
		sat_id_noaa_12 = 21263,
		sat_id_noaa_14 = 23455,
		sat_id_noaa_15 = 25338,
        sat_id_noaa_16 = 26536,
        sat_id_noaa_17 = 27453,
        sat_id_gms5 = 23522,
        sat_id_orbview2 = 24883,
        sat_id_fy2a = 24834,
        sat_id_fy2b = 26382,
        sat_id_fy1b = 20788,
        sat_id_fy1c = 25730,
        sat_id_fy1d = 27431
    };

    //---------------------------------------------------------------------
    // Функции доступа к элементам таблицы через их индекс.
    //---------------------------------------------------------------------
    // Возвращаемое значение: одно из значений, определённых в NoradSatId.
    ulong satId( ulong index ) const throw( TRequestExc );
    // Возвращаемое значение:
    // для спутников NOAA - строка вида "NOAA-12"
    const char * satName( ulong index ) const throw( TRequestExc );
    const char * fileNamePrefix( ulong index ) const throw( TRequestExc );

    //---------------------------------------------------------------------
    // Функции доступа к таблице при помощи внутреннего курсора класса.
    //---------------------------------------------------------------------
    int setToFirst();
    int setToNext();
    int isValid() const;
    // Установить курсор на спутник с указанным id.
    // Возвращаемое значение:
    // true     Курсор установлен успешно. Тогда и только тогда, когда id равно одному из членов NoradSatId (см. выше).
    // false    Курсор не установлен.
    int setToSatelliteWithId( ulong id );
    int setToSatelliteWithName( const char * );
    ulong satId() const throw( TRequestExc );
    const char * satName() const throw( TRequestExc );
    const char * fileNamePrefix() const throw( TRequestExc );

    // число спутников в таблице
    static ulong numberOfSatellites();
    // содержит ли таблица информацию о спутнике с данным идентификатором
    static int containsInfoForSatellite( ulong sat_id );

private:
    ulong fIndex;
};


#endif
