/*-----------------------------------------------------------------------------
    y_data.hpp
-----------------------------------------------------------------------------*/
#ifndef _Y_DATA_HPP_
#define _Y_DATA_HPP_


/*
    Построение lookup-таблицы, линейно отображающей значения интервала [src_min, src_max] в интервал [dst_min, dst_max] значений
    типа unsigned char.

    lookup  Массив под таблицу размером (src_max - src_min + 1) элементов.
*/
void _Export linear_lookup_8( uchar * lookup, ulong src_min, ulong src_max, uchar dst_min, uchar dst_max );


/*
    Построение гистограммы массива данных типа unsigned char.

    hist    Массив под гистограмму размером 256 элементов.
*/
void _Export hist_8( ulong * hist, uchar * buf, ulong bufsize );


/*
    Построение гистограммы массива данных типа short.

    min_value, max_value    Задают диапазон гистограммируемых значений. Значения вне этого диапазона в гистограмме не учитываются.
    hist                    Массив под гистограмму размером (max_value - min_value + 1) элементов.
*/
void _Export hist_16( ulong * hist, short * buf, ulong bufsize, short min_value, short max_value );


/*
    p   [0, 1]
    Возвращаемое значение: номер элемента массива hist, квантиль которого равна p.
*/
ulong _Export quant( ulong * hist, ulong hist_size, double p );


#endif
