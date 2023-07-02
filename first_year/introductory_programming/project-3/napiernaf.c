#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void iton(int x, int **a, int *n) {
    // zapisuje reprezentację liczby x w dynamicznie utworzonej *n-elementowej
    // tablicy *a
    int i = 0;
    *n = 0;
    *a = NULL;
    (*a) = (int *)malloc(sizeof(int) * 16);
    if (x >= 0) {
        while (x != 0) {
            if (x % 4 == 1) {
                (*n)++;
                (*a)[(*n) - 1] = i;
                x = (x - 1) / 2;
            } else if (x % 4 == 3) {
                (*n)++;
                (*a)[(*n) - 1] = (-1) * (i + 1);
                x = (x + 1) / 2;
            } else {
                x /= 2;
            }
            i++;
        }
    } else {
        x = (-1) * x;
        while (x > 0) {
            if (x % 4 == 1) {
                (*n)++;
                (*a)[(*n) - 1] = (-1) * (i + 1);
                x = (x - 1) / 2;
            } else if (x % 4 == 3) {
                (*n)++;
                (*a)[(*n) - 1] = i;
                x = (x + 1) / 2;
            } else {
                x /= 2;
            }
            i++;
        }
    }
    (*a) = (int *)realloc(*a, sizeof(int) * (size_t)(*n));
}

int ntoi(int *a, int n) {
    // daje jako wynik wartość liczby reprezentowanej przez n-elementową tablicę
    // a, lub 0, jeśli wartość ta nie mieści się w zakresie typu int
    // szybkie potęgowanie ?? czyli funkcja pow
    // przypadek int min int max osobno...
    if (n == 0)
        return 0;
    if (a[n - 1] > 31 || a[n - 1] < -32)
        return 0;
    if (n > 1) {
        if (a[n - 1] == 31 && a[n - 2] > 0)
            return 0;
        if (a[n - 1] == -32 && a[n - 2] < 0)
            return 0;
    }
    if (n == 1 && a[n - 1] == 31)
        return 0;
    int wynik = 0;
    int k = 0;
    if (a[n - 1] >= 0) {
        int skladnik = 1;
        for (int i = 0; i <= a[n - 1]; i++) {
            if (a[k] == i) {
                k++;
                wynik += skladnik;
            } else if (a[k] == -i - 1) {
                k++;
                wynik -= skladnik;
            }
            skladnik *= 2;
        }
    } else {
        int skladnik = -1;
        for (int i = 0; i <= -a[n - 1] - 1; i++) {
            if (a[k] == i) {
                k++;
                wynik -= skladnik;
            } else if (a[k] == -i - 1) {
                k++;
                wynik += skladnik;
            }
            skladnik *= 2;
        }
    }
    return wynik;
}

int przeciwna(int wartosc) {
    if (wartosc == INT_MAX)
        return INT_MIN;
    else
        return -(wartosc + 1);
}

int wyk(int wartosc) {
    if (wartosc < 0)
        return przeciwna(wartosc);
    else
        return wartosc;
}

void wstaw(int wartosc, int *i, int *c, int *cn) {
    if (*cn == 0) {
        c[0] = wartosc;
        (*cn)++;
    } else {
        int p = *cn - 1;

        if (wyk(wartosc) == wyk(c[p])) {
            if (c[p] == wartosc) {
                if (wartosc >= 0)
                    c[p]++;
                else
                    c[p]--;
            } else {
                (*cn)--; // usuwamy element
            }
        } else if (wyk(wartosc) == wyk(c[p]) + 1) {
            if (c[p] < 0) {
                if (wartosc < 0) {
                    c[p] = przeciwna(c[p]);
                    c[p + 1] = wartosc - 1;
                    (*cn)++;
                } else { // wartosc >= 0
                    c[p] = przeciwna(c[p]);
                }
            } else { // c[p]>=0
                if (wartosc < 0) {
                    c[p] = przeciwna(c[p]);
                } else { // wartosc >= 0
                    c[p] = przeciwna(c[p]);
                    c[p + 1] = wartosc + 1;
                    (*cn)++;
                }
            }
        } else {
            c[p + 1] = wartosc;
            (*cn)++;
        }
    }
    (*i)++;
}

void ncopy(int *a, int an, int **b, int *bn) {
    *b = (int *)malloc(sizeof(int) * (size_t)an);
    *bn = an;
    for (int i = 0; i < an; i++)
        (*b)[i] = a[i];
}

void nadd(int *a, int an, int *b, int bn, int **c, int *cn) {
    // zapisuje w dynamicznie utworzonej *cn-elementowej tablicy *c sumę liczb
    // reprezentowanych przez an-elementową tablicę a i bn-elementową tablicę b,
    if (an == 0 && bn == 0) {
        *cn = 0;
        *c = NULL;
        return;
    }
    if (an == 0) {
        ncopy(b, bn, c, cn);
        return;
    } else if (bn == 0) {
        ncopy(a, an, c, cn);
        return;
    }

    int k = 0;
    if (INT_MAX - an < bn)
        k = INT_MAX;
    else
        k = an + bn;
    *c = (int *)malloc(sizeof(int) * (size_t)k);
    *cn = 0;

    int i = 0, j = 0;
    while (i < an || j < bn) {
        if (i == an) {
            wstaw(b[j], &j, *c, cn);
        } else if (j == bn) {
            wstaw(a[i], &i, *c, cn);
        } else {
            if (wyk(a[i]) == wyk(b[j])) {
                if (a[i] == b[j]) {
                    if (a[i] < 0)
                        wstaw(a[i] - 1, &i, *c, cn);
                    else
                        wstaw(a[i] + 1, &i, *c, cn);
                } else
                    i++;
                j++;
            } else if (wyk(a[i]) < wyk(b[j])) {
                wstaw(a[i], &i, *c, cn);
            } else { // wyk(a[i]) > wyk(b[j])
                wstaw(b[j], &j, *c, cn);
            }
        }
    }
    (*c) = (int *)realloc(*c, sizeof(int) * (size_t)(*cn));
}

void nsub(int *a, int an, int *b, int bn, int **c, int *cn) {
    // zapisuje w dynamicznie utworzonej *cn-elementowej tablicy *c różnicę
    // liczb reprezentowanych przez
    //  an-elementową tablicę a i bn-elementową tablicę b,
    int *b_odwrotnie = (int *)malloc(sizeof(int) * (size_t)bn);
    for (int i = 0; i < bn; i++) {
        b_odwrotnie[i] = przeciwna(b[i]);
    }
    nadd(a, an, b_odwrotnie, bn, c, cn);
    free(b_odwrotnie);
    // dodajemy liczbę przeciwną :)
}

void nmul(int *a, int an, int *b, int bn, int **c, int *cn) {
    // zapisuje w dynamicznie utworzonej *cn-elementowej tablicy *c iloczyn
    // liczb reprezentowanych przez
    //  an-elementową tablicę a i bn-elementową tablicę b,
    *c = NULL;
    *cn = 0;
    if (an > 0 && bn > 0) {
        int *pomoc = (int *)malloc(sizeof(int) * (size_t)an);
        for (int i = 0; i < bn; i++) {
            if (b[i] < 0) {
                for (int j = 0; j < an; j++) {
                    if (a[j] < 0)
                        pomoc[j] = -a[j] - b[i] - 2;
                    else
                        pomoc[j] = -a[j] + b[i];
                }
            } else { // b[i]>=0
                for (int j = 0; j < an; j++) {
                    if (a[j] < 0)
                        pomoc[j] = a[j] - b[i];
                    else
                        pomoc[j] = a[j] + b[i];
                }
            }
            int dn = 0;
            int *d = NULL;
            nadd(*c, *cn, pomoc, an, &d, &dn);
            free(*c);
            *c = d;
            *cn = dn;
        }
        free(pomoc);
    }
}

// a *= b
void nmul_by(int **a, int *an, int *b, int bn) {
    int *c = NULL;
    int cn = 0;
    nmul(*a, *an, b, bn, &c, &cn);
    free(*a);
    *a = c;
    *an = cn;
}

void nexp(int *a, int an, int *b, int bn, int **c, int *cn) {
    // zapisuje w dynamicznie utworzonej *cn-elementowej tablicy *c wynik
    // potęgowania, którego podstawa jest reprezentowana przez an-elementową
    // tablicę a, a nieujemny wykładnik jest reprezentowany przez bn-elementową
    // tablicę b,
    *cn = 0;
    *c = NULL;

    int *a_copy;
    int acn;
    ncopy(a, an, &a_copy, &acn);

    if (an > 0) {
        if (bn == 0) {
            *cn = 1;
            (*c) = (int *)malloc(sizeof(int));
            (*c)[0] = 0;
            return;
        }

        ncopy(a, an, c, cn);
        int k = 0;

        for (int i = 1; i <= b[bn - 1]; i++) {
            if (i - 1 == b[k]) {
                k++;
                nmul_by(&a_copy, &acn, a_copy, acn);
                nmul_by(c, cn, a_copy, acn);
            } else if (i == -b[k]) {
                k++;
                nmul_by(&a_copy, &acn, a_copy, acn);
            } else {
                nmul_by(c, cn, a_copy, acn);
                nmul_by(&a_copy, &acn, a_copy, acn);
            }
        }
    }
    free(a_copy);
}

// a >= b, działa tylko dla nieujemnych
bool wiekszerowne(int *a, int an, int *b, int bn) {
    for (int i = 1; i <= an && i <= bn; i++) {
        if (a[an - i] != b[bn - i])
            return a[an - i] > b[bn - i];
    }
    if (an < bn)
        return 0 > b[bn - an - 1];
    else if (an > bn)
        return a[an - bn - 1] >= 0;
    else
        return true; // równe
}

bool czy_nieujemne(int *a, int an) { return an == 0 || a[an - 1] >= 0; }

void ndivmod(int *a, int an, int *b, int bn, int **q, int *qn, int **r,
             int *rn) {
    // dzieli liczbę, której reprezentacją jest an-elementowa tablica a, przez
    // niezerową liczbę, której reprezentacją jest bn-elementowa tablica b.
    // Iloraz zapisuje w dynamicznie utworzonej *qn-elementowej tablicy *q.
    //  Resztę z dzielenia zapisuje w dynamicznie utworzonej *rn-elementowej
    //  tablicy *r.
    // Funkcja daje wynik zgodny z algorytmem dzielenia Euklidesa .
    // Wynikiem dzielenia całkowitej liczby a przez niezerową całkowitą liczbę b
    // jest całkowity iloraz q i całkowita reszta r,
    //  które spełniają warunki:
    // a = b * q + r,
    // 0 <= r < abs(b),
    // gdzie abs(b) jest wartością bezwzględną liczby b.

    int *a_copy;
    int acn;
    ncopy(a, an, &a_copy, &acn);

    int *b_copy;
    int bcn;
    ncopy(b, bn, &b_copy, &bcn);

    bool a_ujemna = false, b_ujemna = false;

    if (!czy_nieujemne(a_copy, acn)) {
        a_ujemna = true;
        for (int i = 0; i < acn; i++)
            a_copy[i] = przeciwna(a[i]);
    }

    if (!czy_nieujemne(b_copy, bcn)) {
        b_ujemna = true;
        for (int i = 0; i < bcn; i++)
            b_copy[i] = przeciwna(b[i]);
    }

    *q = NULL;
    *qn = 0;

    while (acn > 0) {
        int dif = a_copy[acn - 1] - b_copy[bcn - 1];
        if (dif < 0)
            break;
        int *c, cn;
        ncopy(b_copy, bcn, &c, &cn);
        for (int i = 0; i < cn; i++) {
            if (c[i] < 0)
                c[i] -= dif;
            else
                c[i] += dif;
        }

        if (wiekszerowne(a_copy, acn, c, cn)) { // a >= c
            int *d, dn;
            nsub(a_copy, acn, c, cn, &d, &dn);
            free(a_copy);
            a_copy = d;
            acn = dn;

            int e[1];
            e[0] = dif;
            nadd(*q, *qn, e, 1, &d, &dn);
            free(*q);
            *q = d;
            *qn = dn;
        } else if (dif > 0) {
            for (int i = 0; i < cn; i++) {
                if (c[i] < 0)
                    c[i] += 1;
                else
                    c[i] -= 1;
            }

            int *d, dn;
            nsub(a_copy, acn, c, cn, &d, &dn);
            free(a_copy);
            a_copy = d;
            acn = dn;

            int e[1];
            e[0] = dif - 1;
            nadd(*q, *qn, e, 1, &d, &dn);
            free(*q);
            *q = d;
            *qn = dn;
        } else {
            free(c);
            break;
        }

        free(c);
    }

    *r = a_copy;
    *rn = acn;

    if (a_ujemna) {
        int *c, cn;
        int one[1];
        one[0] = 0;
        nadd(*q, *qn, one, 1, &c, &cn);
        free(*q);
        *q = c;
        *qn = cn;

        nsub(*r, *rn, b_copy, bcn, &c, &cn);
        free(*r);
        *r = c;
        *rn = cn;
    }

    if (b_ujemna) {
        int minus_one[1];
        minus_one[0] = -1;
        nmul_by(q, qn, minus_one, 1);
    }

    free(b_copy); // a_copy przepisaliśmy do r
}
