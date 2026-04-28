#include <stdio.h>

#include "aluno.h"

int main() {
    int terms;
    if (scanf("%d", &terms) == -1) {
        printf("Falha ao ler a quantidade de termos\n");
    }

    Polinomio pol = criarPolinomio(terms);

    destroiPolinomio(&pol);
    return 0;
}
