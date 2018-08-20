/*-----------------------------------------------------------------------------
    y_math.hpp
-----------------------------------------------------------------------------*/
#ifndef _Y_MATH_HPP_
#define _Y_MATH_HPP_


// Копирование вектора.
void v_cpy( double * dst, const double * src );

// Сложение векторов.
void v_add( const double *a, const double *b, double *r );

// Вычитание векторов.
void v_sub( const double *a, const double *b, double *r );

// Умножение вектора на число.
void v_mul( const double * a, double n, double * r );

// Вычисление длины вектора.
double v_len( const double *a );

/*
    Вычисление скалярного произведения.
*/
double v_smul( const double * a, const double * b );


/*
    Вычисление угла между векторами.
*/
double v_angle( const double * a, const double * b );


/*
    Нормирование вектора. Массив b может совпадать с массивом a.
*/
void v_norm( const double * a, double * b );

/*
    Вычисление векторного произведения. Массив c НЕ должен совпадать ни с одним
    из массивов a или b.
*/
void v_vmul( const double * a, const double * b, double * c );


/*
    Вычисление вектора нормали плоскости векторов a и b. Массив c НЕ должен
    совпадать ни с одним из массивов a или b.
*/
void v_ort( const double * a, const double * b, double * c );




double mgi240( const double * a, const double * b, const double * c );

#endif
