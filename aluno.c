#include <stdlib.h>

#include "aluno.h"

Polinomio criarPolinomio(int size) {
    Polinomio pol;
    pol.termos = malloc(sizeof(Termo) * size);
    pol.size = size;

    return pol;
}

void destroiPolinomio(Polinomio self) { free(self.termos); }
