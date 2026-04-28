#include "aluno.h"

Polinomio derivarPolinomio(Polinomio self) {
    Polinomio derivada = criarPolinomio(self.size);
    for (int i = 0; i < self.size; i++) {
        derivada.termos[i].coeficiente =
            self.termos[i].coeficiente * self.termos[i].expoente;
        derivada.termos[i].expoente = self.termos[i].expoente - 1;
    }

    return derivada;
}
