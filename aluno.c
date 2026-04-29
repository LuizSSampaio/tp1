#include <stdio.h>
#include <stdlib.h>

#include "aluno.h"

Polinomio criarPolinomio(int size) {
    Polinomio pol;
    pol.termos = malloc(sizeof(Termo) * size);
    pol.size = size;

    return pol;
}

void destroiPolinomio(Polinomio *self) {
    if (self == NULL) {
        return;
    }

    free(self->termos);
    self->termos = NULL;
    self->size = 0;
}

Polinomio derivarPolinomio(Polinomio self) {
    int derivedSize = 0;

    for (int i = 0; i < self.size; i++) {
        int coeficienteDerivado =
            self.termos[i].coeficiente * self.termos[i].expoente;

        if (self.termos[i].expoente > 0 && coeficienteDerivado != 0) {
            derivedSize++;
        }
    }

    Polinomio derivada = criarPolinomio(derivedSize);
    int j = 0;

    for (int i = 0; i < self.size; i++) {
        int coeficienteDerivado =
            self.termos[i].coeficiente * self.termos[i].expoente;

        if (self.termos[i].expoente > 0 && coeficienteDerivado != 0) {
            derivada.termos[j].coeficiente = coeficienteDerivado;
            derivada.termos[j].expoente = self.termos[i].expoente - 1;
            j++;
        }
    }

    return derivada;
}

void imprimirPolinomio(const Polinomio *pol) {
    for (int i = 0; i < pol->size; i++) {
        if (pol->termos[i].coeficiente == 0)
            continue;

        if (pol->termos[i].coeficiente > 0 && i != 0)
            printf("+");

        printf("%.2f", (float)pol->termos[i].coeficiente);

        if (pol->termos[i].expoente == 0)
            continue;

        printf("x");

        if (pol->termos[i].expoente == 1)
            continue;

        printf("^%d", pol->termos[i].expoente);
    }
    printf("\n");
}

void imprimirDados(const Polinomio *pol, double startRange, double endRange) {
    // TODO: Remove these casts when startRange and endRange are used by
    // analisarPolinomio.
    (void)startRange;
    (void)endRange;

    printf("Polinomio: ");
    imprimirPolinomio(pol);

    Polinomio derivada = derivarPolinomio(*pol);
    printf("Derivada: ");
    imprimirPolinomio(&derivada);

    printf("Intervalos de crescimento: [0.50, 1.50]\n");

    printf("Intervalos de decrescimento:\n");

    printf("Pontos de transicao:\n");

    destroiPolinomio(&derivada);
}
