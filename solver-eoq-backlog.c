#include <stdio.h>
#include <math.h>


//metodo da bisseção -- A.4
double metodoBisseccao(double a, double b,
                         double (*f)(double),
                         double epsilon, int maxIter) {

    if (f(a) * f(b) > 0) {
        printf("Funcao nao convergiu\n");
        return NAN;
    }

    double x, deltaX = (b - a) / 2.0;
    int iter = 0;

    do {
        x = (a + b) / 2.0;
        printf("%d\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n",
               iter, a, b, f(a), f(b), f(x), deltaX);

        if (fabs(f(x)) < epsilon && deltaX < epsilon) {
            return x;
        }

        if (iter >= maxIter) {
            return NAN;
        }

        if (f(a) * f(x) > 0) {
            a = x;
        } else {
            b = x;
        }

        iter++;
        deltaX /= 2.0;

    } while (1);
}
// A.1 - entrada dos dados- Henrique

 // equação 14 --A.2


double f(double q, double d, double s, double h, double pi){

     double termo_constante = (h / 2.0) * (1.0 - h / (h + pi));

    return (-d * s) / ( q * q) + termo_constante;

}

// isolamento do intervalo entre a e b -- A.3

void funcao_isolamento(double d, double s, double h, double pi, double *a, double *b){
    *a = 1;
    double tamanho_intervalo = 1.0;
    *b = *a + tamanho_intervalo;

    while ( f(*a, d, s, h, pi)* f(*b, d, s, h, pi) >= 0 ){

        *a = *b;
        *b = *a + tamanho_intervalo;

     }
} 

