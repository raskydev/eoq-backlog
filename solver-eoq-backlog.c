#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//henrique: a parte da bissecção tava conflitando um pouco com a minha porque ela foi feita pra receber um argumento só 
//e eu fiz pra receber cinco então tive que fazer umas mudancinhas vou deixar comentadas e avisadas aí tudo que eu fiz

double D_global, S_global, H_global, PI_global; //pra solucionar esse conflito ai eu botei essas variaveis globais pra
                                                //bissecçao acessar os dados de entrada

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

//henrique: funçao tradutora pra bissecção enxergar a f
//como a funçao recebe um ponteiro para uma função so de um argumento,
//precisa disso pra passar o 'q' e completar com as variáveis globais (D, S, H, PI).
double wrapper_f(double q) {
    return f(q, D_global, S_global, H_global, PI_global);
}
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
        //printf("%d\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n",
               //iter, a, b, f(a), f(b), f(x), deltaX);

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

//henrique:

int main(int argc, char** argv) {
    
    //trava de segurança
    //ela serve pra garantir que os quatro parâmetros foram inseridos (D, H, S, PI) e o primeiro espaço
    //fica reservado para o nome do programa/comando utilizado

    if (argc != 5) {
        printf("Parametros invalidos, tente novamente.\n");
        printf("Siga o formato ./solver-eoq-backlog.bin <D> <H> <S> <PI>\n"); //no caso, o primeiro espaço se refere a essa parte do
                                                                              //./solver-eoq-backlog.bin que para o programa é literalmente
                                                                              //interpretado como um dos cinco parâmetros
        return 1;
    }

    //tradução das strings
    //como os dados numéricos são recebidos como texto e o computador não pode fazer contas com textos
    //é preciso converter esses textos em números, que é o que a função atof() faz

    double D = atof(argv[1]); //exemplo prático dessa linha: a função procura o texto (que é um número)
                              //dentro na posição de memória do argv[1] e transforma em um double
    double H = atof(argv[2]);
    double S = atof(argv[3]);
    double PI = atof(argv[4]);

    //parte rayane: variáveis que vão receber a resposta
    double a = 0.0, b = 0.0;
    
    //chamando a função da rayane (A.3)
    funcao_isolamento(D, S, H, PI, &a, &b); //aqui é importante o & no a e no b pra sinalizar que os valores iniciais 0.0
                                            //tem que ser alterados (passagem por referência), são dados de saída
                                            //nos outros não é necessário porque são apenas dados de entrada que a função vai ler

    //atribuindo os valores das variaveis globais no que precisa
    D_global = D; H_global = H; S_global = S; PI_global = PI;

    //chamando a funçao da bissecção e a funçao da traduçao (wrapper) ali da linha 41
    double Q_opt = metodoBisseccao(a, b, wrapper_f, 0.0001, 1000); //E = 0.0001 e maxiter = 1000

    //bissecção descobre o Q_opt e com ele dá pra calcular o B, Imax e CT
    double B_opt = Q_opt * (H / (H + PI));
    double Imax = Q_opt - B_opt;
    double CT = (D/Q_opt)*S + ((Q_opt-B_opt)*(Q_opt-B_opt)/(2*Q_opt))*H + (B_opt*B_opt/(2*Q_opt))*PI;

    //relatorio 
    printf("=========================================================\n");
    printf("    RELATORIO DE DIMENSIONAMENTO DE ESTOQUES (RAIZES)    \n");
    printf("=========================================================\n");
    printf(">> PARAMETROS DE ENTRADA:\n");
    printf("  Demanda Anual (D)..........: %.2f\n", D);
    printf("  Custo de Estocagem (H).....: %.2f\n", H);
    printf("  Custo de Setup (S).........: %.2f\n", S);
    printf("  Custo de Falta (PI)........: %.2f\n\n", PI);

    printf(">> INTERVALO DE ISOLAMENTO:\n");
    printf("  [a, b].....................: [%.2f, %.2f]\n", a, b);
    printf("=========================================================\n");

    printf(">> RESULTADOS DA OTIMIZACAO:\n");
    printf("  Lote Otimo (Q*)............: %.2f\n", Q_opt);
    printf("  Faltas Planejadas (B*).....: %.2f\n", B_opt);
    printf("  Estoque Maximo (Imax)......: %.2f\n", Imax);
    printf("  Custo Total (CT)...........: R$ %.2f\n", CT);
    printf("=========================================================\n");

    return 0;
}
