/*-------------------------------------------------------------------------
    y_prjmap.hpp
-------------------------------------------------------------------------*/
#ifndef _Y_PRJMAP_HPP_
#define _Y_PRJMAP_HPP_


class _Export TProjMapper {
public:
    enum ProjType { mrt = 1, eqd };

    // все параметры - в радианах
    TProjMapper( ProjType t, double lon, double lat, double lon_size, double lat_size, double lon_res, double lat_res );

    ulong sizeX() const { return size_x; }
    ulong sizeY() const { return size_y; }

    // узнать долготу по номеру столбца
    // радианы
    // Внимание !!! Если проекция проходит через 180-меридиан, т.е. левый край находится в восточном полушарии, а правый - в
    // западном, то для столбцов, попадающих в западное полушарие, будут возвращаться значения >PI.
    double lon( ulong column ) const;
    // узнать широту по номеру скана
    // радианы
    double lat( ulong scan ) const;

    // узнать номер скана по широте
    // lat  широта в радианах
    ulong scan( double lat ) const;
    // возвращает неокруглённое значение
    double dScan( double lat ) const;

    // узнамть номер столбца по долготе
    // lon  долгота в радианах
    // Внимание !!! Если проекция проходит через 180-меридиан, т.е. левый край находится в восточном полушарии, а правый - в
    // западном, то для получения номеров столбцов, попадающих в западное полушарие, должны указываться значения lon>PI.
    ulong column( double lon ) const;
    // возвращает неокруглённое значение
    double dColumn( double lon ) const;

private:
    ProjType pt;
    double lat_a, lat_b;
    double lon_a, lon_b;
    ulong size_x, size_y;
};


#endif
