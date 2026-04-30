#pragma once

typedef struct {
    double start, end;
} Intervalo;

typedef struct {
    double x, y;
} Ponto;

typedef struct {
    double coeficiente;
    int expoente;
} Termo;

typedef struct {
    Termo *termos;
    int size;
} Polinomio;

Polinomio criarPolinomio(int size);

void destroiPolinomio(Polinomio *self);

double avaliarPolinomio(const Polinomio *self, double x);

Polinomio derivarPolinomio(const Polinomio *self);

void analisarPolinomio(const Polinomio *pol, const Polinomio *der,
                       Intervalo *cresc, int *crescSize, Intervalo *decresc,
                       int *decrescSize, Ponto *pontos, int *pontosSize,
                       double startRange, double endRange);

void imprimirDados(const Polinomio *pol, double startRange, double endRange);
