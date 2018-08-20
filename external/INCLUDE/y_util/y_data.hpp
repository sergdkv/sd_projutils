/*-----------------------------------------------------------------------------
    y_data.hpp
-----------------------------------------------------------------------------*/
#ifndef _Y_DATA_HPP_
#define _Y_DATA_HPP_


/*
    ����஥��� lookup-⠡����, ������� �⮡ࠦ��饩 ���祭�� ���ࢠ�� [src_min, src_max] � ���ࢠ� [dst_min, dst_max] ���祭��
    ⨯� unsigned char.

    lookup  ���ᨢ ��� ⠡���� ࠧ��஬ (src_max - src_min + 1) ����⮢.
*/
void _Export linear_lookup_8( uchar * lookup, ulong src_min, ulong src_max, uchar dst_min, uchar dst_max );


/*
    ����஥��� ���⮣ࠬ�� ���ᨢ� ������ ⨯� unsigned char.

    hist    ���ᨢ ��� ���⮣ࠬ�� ࠧ��஬ 256 ����⮢.
*/
void _Export hist_8( ulong * hist, uchar * buf, ulong bufsize );


/*
    ����஥��� ���⮣ࠬ�� ���ᨢ� ������ ⨯� short.

    min_value, max_value    ������ �������� ���⮣ࠬ���㥬�� ���祭��. ���祭�� ��� �⮣� ��������� � ���⮣ࠬ�� �� ���뢠����.
    hist                    ���ᨢ ��� ���⮣ࠬ�� ࠧ��஬ (max_value - min_value + 1) ����⮢.
*/
void _Export hist_16( ulong * hist, short * buf, ulong bufsize, short min_value, short max_value );


/*
    p   [0, 1]
    �����頥��� ���祭��: ����� ����� ���ᨢ� hist, ����⨫� ���ண� ࠢ�� p.
*/
ulong _Export quant( ulong * hist, ulong hist_size, double p );


#endif
