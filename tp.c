#include <stdio.h>

#include "aluno.h"

int main() {
    int terms;
    if (scanf("%d", &terms) != 1) {
        printf("Falha ao ler a quantidade de termos\n");
        return -1;
    }

    Polinomio pol = criarPolinomio(terms);
    for (int i = 0; i < terms; i++) {
        double coe;
        int exp;
        if (scanf("%lf%d", &coe, &exp) != 2) {
            printf("Falha ao ler termos\n");
            destroiPolinomio(&pol);
            return -1;
        }

        if (exp < 0) {
            printf("Erro: expoente negativo.");
            destroiPolinomio(&pol);
            return 0;
        }

        pol.termos[i].coeficiente = coe;
        pol.termos[i].expoente = exp;
    }

    double startRange, endRange;
    if (scanf("%lf%lf", &startRange, &endRange) != 2) {
        printf("Falha ao ler intervalo.\n");
        destroiPolinomio(&pol);
        return -1;
    }
    imprimirDados(&pol, startRange, endRange);

    destroiPolinomio(&pol);
    return 0;
}
