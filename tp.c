#include <stdio.h>

#include "aluno.h"

int main() {
    int terms;
    if (scanf("%d", &terms) != 1 || terms < 0) {
        printf("Falha ao ler a quantidade de termos\n");
        return -1;
    }

    Polinomio pol = criarPolinomio(terms);
    for (int i = 0; i < terms; i++) {
        int coe, exp;
        if (scanf("%d%d", &coe, &exp) != 2) {
            printf("Falha ao ler termos\n");
            destroiPolinomio(&pol);
            return -1;
        }

        if (terms == 1 && exp == 0) {
            printf("Funcao constante em y = %.2f.", (double)coe);
            destroiPolinomio(&pol);
            return 0;
        }

        if (exp < 0) {
            printf("Erro: expoente negativo.");
            destroiPolinomio(&pol);
            return -1;
        }

        pol.termos[i].coeficiente = coe;
        pol.termos[i].expoente = exp;
    }

    destroiPolinomio(&pol);
    return 0;
}
