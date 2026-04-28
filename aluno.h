#pragma once

typedef struct {
    int coeficiente;
    int expoente;
} Termo;

typedef struct {
    Termo *termos;
    int size;
} Polinomio;

Polinomio criarPolinomio(int size);

void destroiPolinomio(Polinomio *self);

void avaliarPolinomio(Polinomio self);

Polinomio derivarPolinomio(Polinomio self);

void analisarPolinomio(Polinomio self);
