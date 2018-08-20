//-------------------------------------------------------------------------
// NRC.HPP
//-------------------------------------------------------------------------
#ifndef _NRC_
#define _NRC_

#include <math.h>

double _Export f1dim(double x, int n, double *pcom, double *xicom,
   double (*func)(double[]) );

double _Export brent(double ax, double bx, double cx, double (*f)(double),
   double tol, double *xmin);

double _Export dfridr( double (*func)(double), double x, double h, double *err );

void _Export linmin(double p[], double xi[], int n, double *fret,
   double (*func)(double []));

void _Export mnbrak(double *ax, double *bx, double *cx, double *fa, double *fb,
    double *fc, double (*func)(double) );

void _Export powell( double p[], double *xi, int n, double ftol, int *iter,
    double *fret, double (*func)(double []));


inline double nrc_sqr(double a){ return a*a; }
inline double nrc_max(double a,double b){ return (a>b)?a:b; }
inline double nrc_sign(double a,double b){ return (b>0.)?fabs(a):-fabs(a); }

/*
    rtbis
    Поиск корня методом бисекции.
    Returns:
    0   OK
    1   Функция принимает на концах указанного интервала значения
        одинакового знака.
    2   Слишком большое число итераций.
*/
int _Export rtbis( double (*func)(double), double x1, double x2, double xacc,
    double *root );


/*
    Функция производит LU-разложение матрицы a.
    Возвращаемое значение:
    0   OK
    1   Матрица вырождена.
*/
int _Export ludcmp( double *a, int n, int *indx, double *d );
void _Export lubksb( double *a, int n, int *indx, double *b, double *x );


#endif
