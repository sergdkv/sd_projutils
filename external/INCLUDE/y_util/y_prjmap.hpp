/*-------------------------------------------------------------------------
    y_prjmap.hpp
-------------------------------------------------------------------------*/
#ifndef _Y_PRJMAP_HPP_
#define _Y_PRJMAP_HPP_


class _Export TProjMapper {
public:
    enum ProjType { mrt = 1, eqd };

    // �� ��ࠬ���� - � ࠤ�����
    TProjMapper( ProjType t, double lon, double lat, double lon_size, double lat_size, double lon_res, double lat_res );

    ulong sizeX() const { return size_x; }
    ulong sizeY() const { return size_y; }

    // 㧭��� ������� �� ������ �⮫��
    // ࠤ����
    // �������� !!! �᫨ �஥��� ��室�� �१ 180-��ਤ���, �.�. ���� �ࠩ ��室���� � ����筮� �����ਨ, � �ࠢ� - �
    // ��������, � ��� �⮫�殢, ��������� � �������� �����ਥ, ���� ���������� ���祭�� >PI.
    double lon( ulong column ) const;
    // 㧭��� ���� �� ������ ᪠��
    // ࠤ����
    double lat( ulong scan ) const;

    // 㧭��� ����� ᪠�� �� ���
    // lat  ��� � ࠤ�����
    ulong scan( double lat ) const;
    // �����頥� �����㣫񭭮� ���祭��
    double dScan( double lat ) const;

    // 㧭���� ����� �⮫�� �� ������
    // lon  ������ � ࠤ�����
    // �������� !!! �᫨ �஥��� ��室�� �१ 180-��ਤ���, �.�. ���� �ࠩ ��室���� � ����筮� �����ਨ, � �ࠢ� - �
    // ��������, � ��� ����祭�� ����஢ �⮫�殢, ��������� � �������� �����ਥ, ������ 㪠�뢠���� ���祭�� lon>PI.
    ulong column( double lon ) const;
    // �����頥� �����㣫񭭮� ���祭��
    double dColumn( double lon ) const;

private:
    ProjType pt;
    double lat_a, lat_b;
    double lon_a, lon_b;
    ulong size_x, size_y;
};


#endif
