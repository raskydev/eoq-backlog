#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Estrutura para armazenar os parâmetros do modelo EOQ com backlog
typedef struct {
    double D, H, S, PI;
} Params;

// Variável global para armazenar os parâmetros do modelo EOQ com backlog
Params paramsGlobais;


//  Como a bissecção precisa de uma função que receba apenas um argumento, criamos uma função auxiliar que utiliza os parâmetros globais para calcular o valor da função f.
double f(double Q){

   double D  = paramsGlobais.D;
    double S  = paramsGlobais.S;
    double H  = paramsGlobais.H;
    double PI = paramsGlobais.PI;
    return -(D * S) / (Q * Q) + (H / 2.0) * (1.0 - H / (H + PI));

}

// Função para isolar o intervalo [a, b] onde a função f muda de sinal
//Função reestruturada para chamar a função f com apenas um argumento
void funcao_isolamento(double *a, double *b) {
    *a = 1.0;
    double passo = 1.0;
    *b = *a + passo;

    while (f(*a) * f(*b) >= 0) {
        *a = *b;
        *b = *a + passo;
    }
}


//metodo da bisseção -- A.4

double metodoBisseccao(double a, double b,
                       double (*f)(double),
                       double epsilon, int maxIter) {

    if (f(a) * f(b) > 0) {
        return -1;
    }

    int iter = 0;
    double x = (a + b) / 2.0;

    while (fabs(b - a) > epsilon && fabs(f(x)) > epsilon && iter < maxIter) {

        if (f(a) * f(x) > 0) {
            a = x;
        } else {
            b = x;
        }

        x = (a + b) / 2.0;
        iter++;
    }

    if (iter >= maxIter) {
        return -1;
    }

    return x;
}
//Calcula o custo total para uma quantidade Q e backlog B, dado os parâmetros do modelo EOQ com backlog
double custoTotal(double Q, double B, Params p) {
    double setup      = (p.D / Q) * p.S;
    double estocagem  = ((Q - B) * (Q - B) / (2.0 * Q)) * p.H;
    double penalidade = (B * B / (2.0 * Q)) * p.PI;
  
    return setup + estocagem + penalidade;
}


// Função principal para resolver o modelo EOQ com backlog, a bissecção é utilizada para encontrar o valor ótimo de Q que minimiza o custo total. A função retorna 0 em caso de sucesso e -1 em caso de falha.
int resolverEOQ(Params p, double epsilon, int maxIter,
                double *Qstar, double *Bstar, double *CT) {

    paramsGlobais = p;

    double a, b;
    funcao_isolamento(&a, &b);

    double Q = metodoBisseccao(a, b, f, epsilon, maxIter);

    if (Q == -1) {
        printf("Bisseccao nao convergiu.\n");
        return -1;
    }

    double B   = Q * (p.H / (p.H + p.PI));
    double custo = custoTotal(Q, B, p);

    //os elementos são salvos nos ponteiros passados como argumento para a função, excluindo a necessidade de retornar múltiplos valores diretamente.
    //ou seja salva em posição de memoria e retorna na variavel.
    *Qstar = Q;
    *Bstar = B;
    *CT    = custo;

    return 0;
}


int main(int argc, char **argv) {

    if (argc != 5) {
        printf("Para executar o programa passe os parametros: \n");
        printf("./solver <D> <H> <S> <PI>\n");
        return 1;
    }

//recebe os parâmetros de entrada do usuário e os armazena na estrutura na struct Params
    Params p;
    p.D  = atof(argv[1]);
    p.H  = atof(argv[2]);
    p.S  = atof(argv[3]);
    p.PI = atof(argv[4]);

//chama a função resolverEOQ para calcular os valores ótimos de Q, B e CT
    double Qstar, Bstar, CT;
    //chamada da função resolverEOQ com os parâmetros de entrada, tolerância e número máximo de iterações
    
    int ok = resolverEOQ(p, 1e-6, 10000, &Qstar, &Bstar, &CT);

//verifica se a função resolverEOQ retornou um erro
    if (ok == -1) return 1;
    //calcula o estoque máximo Imax
    double Imax = Qstar - Bstar;

//imprime o relatório de dimensionamento de estoques, começando nos parâmetros de entrada

    printf("=========================================================\n");
    printf(" RELATORIO DE DIMENSIONAMENTO DE ESTOQUES (RAIZES)      \n");
    printf("=========================================================\n");
    printf(">> PARAMETROS DE ENTRADA:\n");
    printf(" Demanda Anual (D)..........: %.2f\n",    p.D);
    printf(" Custo de Estocagem (H).....: %.2f\n",    p.H);
    printf(" Custo de Setup (S).........: %.2f\n",    p.S);
    printf(" Custo de Falta (PI)........: %.2f\n\n",  p.PI);

    printf(">> INTERVALO DE ISOLAMENTO:\n");

     //chama a função funcao_isolamento para determinar o intervalo [a, b] onde a função f muda de sinal
    double a, b;
    // Armazena os parâmetros globais para uso na função f
    paramsGlobais = p;
    funcao_isolamento(&a, &b);
    printf(" [a, b].....................: [%.2f, %.2f]\n", a, b);

    printf(">> RESULTADOS DA OTIMIZACAO:\n");
    //qstar é calculado como a quantidade ótima de pedido que minimiza o custo total
    printf(" Lote Otimo (Q*)............: %.2f\n",    Qstar);
    //bstar é calculado como a quantidade de faltas planejadas, que é uma função do lote ótimo e dos custos de estocagem e falta
    printf(" Faltas Planejadas (B*).....: %.2f\n",    Bstar);
    //imax é calculado como a diferença entre o lote ótimo e as faltas planejadas
    printf(" Estoque Maximo (Imax)......: %.2f\n",    Imax);

    printf(" Custo Total (CT)...........: R$ %.2f\n", CT);
    printf("=========================================================\n");

    return 0;
}

// A.1 - entrada dos dados- Henrique

 // equação 14 --A.2



// isolamento do intervalo entre a e b -- A.3



