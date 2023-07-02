#ifndef NAPIERNAF_H
#define NAPIERNAF_H

void iton(int x, int **a, int *n);

int ntoi(int *a, int n);

void nadd(int *a, int an, int *b, int bn, int **c, int *cn);

void nsub(int *a, int an, int *b, int bn, int **c, int *cn);

void nmul(int *a, int an, int *b, int bn, int **c, int *cn);

void nexp(int *a, int an, int *b, int bn, int **c, int *cn);

void ndivmod(int *a, int an, int *b, int bn, int **q, int *qn, int **r, int *rn);

#endif
