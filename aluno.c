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

/* Recursive: scan grid points and collect transition intervals */
static void coletarTransicoes(const double *points, const double *dvals,
                               int i, int num_pts,
                               Intervalo *trans, int *n_trans) {
    if (i >= num_pts)
        return;

    if (fabs(dvals[i]) < EPS) {
        trans[*n_trans].start = points[i];
        trans[(*n_trans)++].end = points[i];
    } else if (i > 0 && fabs(dvals[i - 1]) >= EPS) {
        int sp = (dvals[i - 1] > 0) ? 1 : -1;
        int sc = (dvals[i] > 0) ? 1 : -1;
        if (sp != sc) {
            trans[*n_trans].start = points[i - 1];
            trans[(*n_trans)++].end = points[i];
        }
    }

    coletarTransicoes(points, dvals, i + 1, num_pts, trans, n_trans);
}

/* Recursive: fill monotonic regions in the gaps between transitions */
static void coletarRegioes(const Polinomio *der,
                            const Intervalo *trans, int j, int n_trans,
                            double endRange,
                            Intervalo *cresc, int *n_cresc,
                            Intervalo *decresc, int *n_decresc,
                            double prev_end) {
    double r_start = prev_end;
    double r_end = (j < n_trans) ? trans[j].start : endRange;

    if (r_end - r_start > EPS) {
        double mid = (r_start + r_end) / 2.0;
        double d = evalPol(der, mid);
        if (d > EPS) {
            cresc[*n_cresc].start = r_start;
            cresc[(*n_cresc)++].end = r_end;
        } else if (d < -EPS) {
            decresc[*n_decresc].start = r_start;
            decresc[(*n_decresc)++].end = r_end;
        }
    }

    if (j < n_trans)
        coletarRegioes(der, trans, j + 1, n_trans, endRange,
                       cresc, n_cresc, decresc, n_decresc, trans[j].end);
}

void analisarPolinomio(Polinomio self, double startRange, double endRange) {
    Polinomio derivada = derivarPolinomio(self);

    if (derivada.size == 0) {
        double value = evalPol(&self, 0.0);
        printf("Funcao constante em y = %.2f.\n", value);
        destroiPolinomio(&derivada);
        return;
    }

    /* Build grid of evaluation points */
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

    /* Evaluate derivative at each grid point */
    double dvals[MAX_PTS];
    for (int i = 0; i < num_pts; i++)
        dvals[i] = evalPol(&derivada, points[i]);

    /* Collect transitions recursively */
    Intervalo trans[MAX_PTS];
    int n_trans = 0;
    coletarTransicoes(points, dvals, 0, num_pts, trans, &n_trans);

    /* Collect monotonic regions recursively */
    Intervalo cresc[MAX_PTS];
    int n_cresc = 0;
    Intervalo decresc[MAX_PTS];
    int n_decresc = 0;
    coletarRegioes(&derivada, trans, 0, n_trans, endRange,
                   cresc, &n_cresc, decresc, &n_decresc, startRange);

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

void imprimirDados(const Polinomio *pol, double startRange, double endRange) {
    Polinomio derivada = derivarPolinomio(*pol);

    if (derivada.size == 0) {
        destroiPolinomio(&derivada);
        analisarPolinomio(*pol, startRange, endRange);
        return;
    }

    printf("Polinomio: ");
    imprimirPolinomio(pol);
    printf("Derivada: ");
    imprimirPolinomio(&derivada);
    destroiPolinomio(&derivada);

    analisarPolinomio(*pol, startRange, endRange);
}
