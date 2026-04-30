#include <math.h>
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

Polinomio derivarPolinomio(const Polinomio *self) {
    int derivedSize = 0;

    for (int i = 0; i < self->size; i++) {
        double coeficienteDerivado =
            self->termos[i].coeficiente * self->termos[i].expoente;

        if (self->termos[i].expoente > 0 && coeficienteDerivado != 0) {
            derivedSize++;
        }
    }

    Polinomio derivada = criarPolinomio(derivedSize);
    int j = 0;

    for (int i = 0; i < self->size; i++) {
        double coeficienteDerivado =
            self->termos[i].coeficiente * self->termos[i].expoente;

        if (self->termos[i].expoente > 0 && coeficienteDerivado != 0) {
            derivada.termos[j].coeficiente = coeficienteDerivado;
            derivada.termos[j].expoente = self->termos[i].expoente - 1;
            j++;
        }
    }

    return derivada;
}

double avaliarPolinomio(const Polinomio *self, double x) {
    double result = 0.0;
    for (int i = 0; i < self->size; i++)
        result +=
            self->termos[i].coeficiente * pow(x, self->termos[i].expoente);
    return result;
}

void analisarPolinomio(const Polinomio *pol, const Polinomio *der,
                       Intervalo *cresc, int *crescSize, Intervalo *decresc,
                       int *decrescSize, Ponto *pontos, int *pontosSize,
                       double startRange, double endRange) {
    static const double step = 0.1;
    if (startRange < endRange)
        analisarPolinomio(pol, der, cresc, crescSize, decresc, decrescSize,
                          pontos, pontosSize, startRange + step, endRange);
}

void imprimirPolinomio(const Polinomio *pol) {
    int printed_any = 0;

    for (int i = 0; i < pol->size; i++) {
        if (pol->termos[i].coeficiente == 0)
            continue;

        if (pol->termos[i].coeficiente > 0 && printed_any)
            printf("+");

        printf("%.2f", pol->termos[i].coeficiente);

        if (pol->termos[i].expoente == 0) {
            printed_any = 1;
            continue;
        }

        printf("x");

        if (pol->termos[i].expoente == 1) {
            printed_any = 1;
            continue;
        }

        printf("^%d", pol->termos[i].expoente);
        printed_any = 1;
    }
    printf("\n");
}

void imprimirIntervalo(const Intervalo self[], int size) {
    for (int i = 0; i < size; i++) {
        printf("[%.2lf, %.2lf] ", self[i].start, self[i].end);
    }
    printf("\n");
}

void imprimirPonto(const Ponto self[], int size) {
    for (int i = 0; i < size; i++) {
        printf("[%.2lf, %.2lf] ", self[i].x, self[i].y);
    }
    printf("\n");
}

void imprimirDados(const Polinomio *pol, double startRange, double endRange) {
    Polinomio der = derivarPolinomio(pol);

    printf("Polinomio: ");
    imprimirPolinomio(pol);
    printf("Derivada: ");
    imprimirPolinomio(&der);
    destroiPolinomio(&der);

    int maxSize = (endRange - startRange) / 0.1;

    Intervalo *cresc = malloc(sizeof(Intervalo) * maxSize);
    int crescSize = maxSize;

    Intervalo *decresc = malloc(sizeof(Intervalo) * maxSize);
    int decrescSize = maxSize;

    Ponto *pontos = malloc(sizeof(Ponto) * maxSize);
    int pontosSize = maxSize;

    analisarPolinomio(pol, &der, cresc, &crescSize, decresc, &decrescSize,
                      pontos, &pontosSize, startRange, endRange);

    printf("Intervalos de crescimento: ");
    imprimirIntervalo(cresc, crescSize);

    printf("Intervalos de decrescimento: ");
    imprimirIntervalo(decresc, decrescSize);

    printf("Pontos de transicao: ");
    imprimirPonto(pontos, pontosSize);
}
