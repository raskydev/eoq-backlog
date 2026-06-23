#include <stdio.h>
#include <math.h>


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

