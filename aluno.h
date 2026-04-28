#pragma once

typedef struct {
    int coeficiente;
    int expoente;

} Termo;

typedef struct {
    Termo *termos;
} Polinomio;

Polinomio criarPolinomio();

void destroiPolinomio();

void avaliarPolinomio();

void derivarPolinomio();

void analisarPolinomio();
