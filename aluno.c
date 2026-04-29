#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "aluno.h"

#define STEP 0.1
#define EPS 1e-9
#define MAX_PTS 500

typedef struct {
    double start;
    double end;
} Intervalo;

static double evalPol(const Polinomio *pol, double x) {
    double result = 0.0;
    for (int i = 0; i < pol->size; i++)
        result += pol->termos[i].coeficiente * pow(x, pol->termos[i].expoente);
    return result;
}

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
        double coeficienteDerivado =
            self.termos[i].coeficiente * self.termos[i].expoente;

        if (self.termos[i].expoente > 0 && coeficienteDerivado != 0) {
            derivedSize++;
        }
    }

    Polinomio derivada = criarPolinomio(derivedSize);
    int j = 0;

    for (int i = 0; i < self.size; i++) {
        double coeficienteDerivado =
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

void imprimirDados(const Polinomio *pol, double startRange, double endRange) {
    Polinomio derivada = derivarPolinomio(*pol);

    if (derivada.size == 0) {
        double value = evalPol(pol, 0.0);
        printf("Funcao constante em y = %.2f.\n", value);
        destroiPolinomio(&derivada);
        return;
    }

    printf("Polinomio: ");
    imprimirPolinomio(pol);
    printf("Derivada: ");
    imprimirPolinomio(&derivada);

    /* Build list of evaluation points: 0.1-step grid plus endpoint if off-grid */
    double points[MAX_PTS];
    int num_pts = 0;
    int k = 0;
    while (num_pts < MAX_PTS) {
        double p = startRange + k * STEP;
        if (p > endRange + EPS)
            break;
        points[num_pts++] = p;
        k++;
    }
    if ((num_pts == 0 || fabs(points[num_pts - 1] - endRange) > EPS) &&
        num_pts < MAX_PTS)
        points[num_pts++] = endRange;

    /* Evaluate derivative at each point */
    double dvals[MAX_PTS];
    for (int i = 0; i < num_pts; i++)
        dvals[i] = evalPol(&derivada, points[i]);

    /* Identify transitions: zero-derivative points and sign-change sub-intervals */
    Intervalo trans[MAX_PTS];
    int n_trans = 0;
    for (int i = 0; i < num_pts; i++) {
        if (fabs(dvals[i]) < EPS) {
            Intervalo t;
            t.start = points[i];
            t.end = points[i];
            trans[n_trans++] = t;
        } else if (i > 0 && fabs(dvals[i - 1]) >= EPS) {
            int sp = (dvals[i - 1] > 0) ? 1 : -1;
            int sc = (dvals[i] > 0) ? 1 : -1;
            if (sp != sc) {
                Intervalo t;
                t.start = points[i - 1];
                t.end = points[i];
                trans[n_trans++] = t;
            }
        }
    }

    /* Build monotonic regions in the gaps between transitions */
    Intervalo cresc[MAX_PTS];
    int n_cresc = 0;
    Intervalo decresc[MAX_PTS];
    int n_decresc = 0;

    double prev_end = startRange;
    for (int j = 0; j <= n_trans; j++) {
        double r_start = prev_end;
        double r_end = (j < n_trans) ? trans[j].start : endRange;

        if (r_end - r_start > EPS) {
            double mid = (r_start + r_end) / 2.0;
            double d = evalPol(&derivada, mid);
            if (d > EPS) {
                Intervalo r;
                r.start = r_start;
                r.end = r_end;
                cresc[n_cresc++] = r;
            } else if (d < -EPS) {
                Intervalo r;
                r.start = r_start;
                r.end = r_end;
                decresc[n_decresc++] = r;
            }
        }

        if (j < n_trans)
            prev_end = trans[j].end;
    }

    /* Print results */
    printf("Intervalos de crescimento: ");
    for (int j = 0; j < n_cresc; j++)
        printf("[%.2f, %.2f] ", cresc[j].start, cresc[j].end);
    printf("\n");

    printf("Intervalos de decrescimento: ");
    for (int j = 0; j < n_decresc; j++)
        printf("[%.2f, %.2f] ", decresc[j].start, decresc[j].end);
    printf("\n");

    printf("Pontos de transicao: ");
    for (int j = 0; j < n_trans; j++) {
        if (j > 0)
            printf(" ");
        printf("[%.2f, %.2f]", trans[j].start, trans[j].end);
    }
    printf("\n");

    destroiPolinomio(&derivada);
}
