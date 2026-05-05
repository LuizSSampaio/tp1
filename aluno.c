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

void adicionaPontoTransicao(Ponto *pontos, int *pontosSize, int capacidade,
                            double start, double end) {
    static const double eps = 1e-7;

    if (*pontosSize >= capacidade) {
        return;
    }

    if (*pontosSize > 0 && fabs(pontos[*pontosSize - 1].x - start) < eps &&
        fabs(pontos[*pontosSize - 1].y - end) < eps) {
        return;
    }

    pontos[*pontosSize].x = start;
    pontos[*pontosSize].y = end;
    (*pontosSize)++;
}

void analisarPolinomio(const Polinomio *pol, int maxSize, Intervalo *cresc,
                       int *crescSize, Intervalo *decresc, int *decrescSize,
                       Ponto *pontos, int *pontosSize, double startRange,
                       double endRange) {
    static const double step = 0.1;
    static const double eps = 1e-7;
    static int inicializado = 0;

    if (!inicializado) {
        *crescSize = 0;
        *decrescSize = 0;
        *pontosSize = 0;
        inicializado = 1;
    }

    if (startRange >= endRange - eps) {
        inicializado = 0;
        return;
    }

    int capacidade = maxSize + 2;
    double x = startRange;
    double next = x + step;

    if (next > endRange) {
        next = endRange;
    }

    if (fabs(x) < eps) {
        x = 0.0;
    }
    if (fabs(next) < eps) {
        next = 0.0;
    }

    double dx = avaliarPolinomio(pol, x);
    double dnext = avaliarPolinomio(pol, next);

    int sinalX = 0;
    int sinalNext = 0;

    if (dx > eps) {
        sinalX = 1;
    } else if (dx < -eps) {
        sinalX = -1;
    }

    if (dnext > eps) {
        sinalNext = 1;
    } else if (dnext < -eps) {
        sinalNext = -1;
    }

    if (sinalX == 0) {
        adicionaPontoTransicao(pontos, pontosSize, capacidade, x, x);
    }

    int sinalIntervalo = 0;
    int deveUnir = 0;

    if (sinalX == sinalNext) {
        sinalIntervalo = sinalX;
        deveUnir = 1;
    } else if (sinalX == 0) {
        sinalIntervalo = sinalNext;
    } else if (sinalNext == 0) {
        sinalIntervalo = sinalX;
        deveUnir = 1;
        adicionaPontoTransicao(pontos, pontosSize, capacidade, next, next);
    } else {
        adicionaPontoTransicao(pontos, pontosSize, capacidade, x, next);
    }

    if (sinalIntervalo != 0) {
        Intervalo *intervalos = sinalIntervalo > 0 ? cresc : decresc;
        int *intervalosSize = sinalIntervalo > 0 ? crescSize : decrescSize;

        if (*intervalosSize < capacidade) {
            if (deveUnir && *intervalosSize > 0 &&
                fabs(intervalos[*intervalosSize - 1].end - x) < eps) {
                intervalos[*intervalosSize - 1].end = next;
            } else {
                intervalos[*intervalosSize].start = x;
                intervalos[*intervalosSize].end = next;
                (*intervalosSize)++;
            }
        }
    }

    analisarPolinomio(pol, maxSize, cresc, crescSize, decresc, decrescSize,
                      pontos, pontosSize, next, endRange);
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

    int maxSize = (endRange - startRange) / 0.1 + 2;

    Intervalo *cresc = malloc(sizeof(Intervalo) * maxSize);
    int crescSize = 0;

    Intervalo *decresc = malloc(sizeof(Intervalo) * maxSize);
    int decrescSize = 0;

    Ponto *pontos = malloc(sizeof(Ponto) * maxSize);
    int pontosSize = 0;

    analisarPolinomio(&der, maxSize, cresc, &crescSize, decresc, &decrescSize,
                      pontos, &pontosSize, startRange, endRange);
    destroiPolinomio(&der);

    printf("Intervalos de crescimento: ");
    imprimirIntervalo(cresc, crescSize);

    printf("Intervalos de decrescimento: ");
    imprimirIntervalo(decresc, decrescSize);

    printf("Pontos de transicao: ");
    imprimirPonto(pontos, pontosSize);

    free(cresc);
    free(decresc);
    free(pontos);
}
