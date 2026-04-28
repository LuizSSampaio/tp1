#include "aluno.h"

Polinomio derivarPolinomio(Polinomio self) {
    Polinomio derivate = criarPolinomio(self.size);
    for (int i = 0; i < self.size; i++) {
        derivate.termos[i].coeficiente =
            self.termos[i].coeficiente * self.termos[i].expoente;
        derivate.termos[i].expoente = self.termos[i].expoente - 1;
    }

    return derivate;
}
