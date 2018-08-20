/*-----------------------------------------------------------------------------
    y_math.hpp
-----------------------------------------------------------------------------*/
#ifndef _Y_MATH_HPP_
#define _Y_MATH_HPP_


// ����஢���� �����.
void v_cpy( double * dst, const double * src );

// �������� ����஢.
void v_add( const double *a, const double *b, double *r );

// ���⠭�� ����஢.
void v_sub( const double *a, const double *b, double *r );

// ��������� ����� �� �᫮.
void v_mul( const double * a, double n, double * r );

// ���᫥��� ����� �����.
double v_len( const double *a );

/*
    ���᫥��� ᪠��୮�� �ந��������.
*/
double v_smul( const double * a, const double * b );


/*
    ���᫥��� 㣫� ����� ����ࠬ�.
*/
double v_angle( const double * a, const double * b );


/*
    ��ନ஢���� �����. ���ᨢ b ����� ᮢ������ � ���ᨢ�� a.
*/
void v_norm( const double * a, double * b );

/*
    ���᫥��� ����୮�� �ந��������. ���ᨢ c �� ������ ᮢ������ �� � �����
    �� ���ᨢ�� a ��� b.
*/
void v_vmul( const double * a, const double * b, double * c );


/*
    ���᫥��� ����� ��ଠ�� ���᪮�� ����஢ a � b. ���ᨢ c �� ������
    ᮢ������ �� � ����� �� ���ᨢ�� a ��� b.
*/
void v_ort( const double * a, const double * b, double * c );




double mgi240( const double * a, const double * b, const double * c );

#endif
