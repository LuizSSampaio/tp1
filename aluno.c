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
