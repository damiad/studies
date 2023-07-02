#include <stdio.h>
#include <stdlib.h>

#ifndef FIELDS
# define FIELDS 5
#endif

#ifndef ROWS
# define ROWS 26
#endif

#ifndef COLUMNS
# define COLUMNS 26
#endif

#ifndef WYBOR
# define WYBOR 1000
#endif

int max(int a, int b) {
    if (a >= b) return a;
    return b;
}

int zmiana_mozliwosci_lewego(int i, int k, char plansza[][COLUMNS]) {
    int p = 0;
    int q = 0;
    for (int a = i - 1; a >= 0 && plansza[a][k] == '.'; a--) p++;
    for (int a = i + 1; a < ROWS  && plansza[a][k] == '.'; a++) q++;
    return ((p + q + 1) / FIELDS - (p / FIELDS) - (q / FIELDS));
}

int zmiana_mozliwosci_prawego(int i, int j, char plansza[][COLUMNS]) {
    int p = 0;
    int q = 0;
    for (int a = j - 1; a >= 0 && plansza[i][a] == '.'; a--) p++;
    for (int a = j + FIELDS; a < COLUMNS && plansza[i][a] == '.'; a++) q++;
    return (-1 - (p + q) / FIELDS + p / FIELDS + q / FIELDS);
}

void zmien_wartosci_tablicy(int od_kolumny, int do_kolumny, int tab[][COLUMNS],
                            int wartosc) { //w kazdym wierszu dla przedziału kolumn
    for (int i = 0; i < ROWS; i++) {
        for (int j = od_kolumny; j < do_kolumny; j++)
            tab[i][j] = wartosc;
    }
}

void wykropkuj_plansze(char plansza[][COLUMNS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++)
            plansza[i][j] = '.';
    }
}

int zaktualizuj_plansze_prawy(int i, int j, char plansza[][COLUMNS],
                              int plansza_prawego[][COLUMNS], int mozliwosci_prawego) {
    for (int k = j; k < j + FIELDS; k++) plansza[i][k] = '#';
    for (int k = max(0, j - FIELDS + 1); k < j + FIELDS; k++) {
        if (plansza_prawego[i][k] != -1) {
            plansza_prawego[i][k] = -1; //z tych pól prawy nie moze zacząć układu
            mozliwosci_prawego--;
        }
    }
    return mozliwosci_prawego;
}

int ruch_prawego(int najlepszy_ruch[][2], char plansza[][COLUMNS],
                 int  plansza_prawego[][COLUMNS], int mozliwosci_prawego) {
    int najlepszy_wynik = -4;
    int ile_ruchow = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (plansza_prawego[i][j] !=
                    -1) { // pomocnicze plansze prawy gdy -1 to nie można wstawić FIELDS zaczynająć w tej komórce
                int zmiana_oceny = (zmiana_mozliwosci_prawego(i, j,
                                                              plansza)); //zwraca -1 albo -2
                for (int k = j; k < j + FIELDS;
                        k++) { // dla każdej kolumny sprawdzamy o ile możliwości lewego się zmieniły;
                    zmiana_oceny += zmiana_mozliwosci_lewego(i, k, plansza); //zwraca 1 albo 0
                }
                if (najlepszy_wynik < zmiana_oceny) {
                    najlepszy_wynik = zmiana_oceny;
                    ile_ruchow = 1;
                    najlepszy_ruch[0][0] = i;
                    najlepszy_ruch[0][1] = j;
                } else if (najlepszy_wynik == zmiana_oceny) {
                    najlepszy_ruch[ile_ruchow][0] = i;
                    najlepszy_ruch[ile_ruchow][1] = j;
                    ile_ruchow++;
                }
            }
        }
    }
    ile_ruchow = WYBOR % ile_ruchow;
    putchar(najlepszy_ruch[ile_ruchow][0] - 0 + 'a');
    putchar(najlepszy_ruch[ile_ruchow][1] - 0 + 'A');
    printf("\n");
    return zaktualizuj_plansze_prawy(najlepszy_ruch[ile_ruchow][0],
                                     najlepszy_ruch[ile_ruchow][1], plansza, plansza_prawego, mozliwosci_prawego);
}

int ruch_lewego(int n, int m, char plansza[][COLUMNS],
                int plansza_prawego[][COLUMNS],
                int mozliwosci_prawego) { //tozsame z macierzą M mxn
    for (int i = m; i < m + FIELDS; i++) {
        plansza[i][n] = '#';
        for (int j = max(0, n - FIELDS + 1); j <= n; j++) {
            if (plansza_prawego[i][j] != -1) {
                plansza_prawego[i][j] = -1;
                mozliwosci_prawego--;
            }
        }
    }
    return mozliwosci_prawego;
}

int main(void) {
    int znak1, znak2;
    char plansza[ROWS][COLUMNS];
    wykropkuj_plansze(plansza);
    int najlepszy_ruch[ROWS * COLUMNS][2];
    int plansza_prawego[ROWS][COLUMNS];
    int mozliwosci_prawego = (max(0, COLUMNS - FIELDS + 1)) * ROWS;
    if (mozliwosci_prawego > 0) {
        zmien_wartosci_tablicy(0, COLUMNS - FIELDS + 1, plansza_prawego, 1);
        zmien_wartosci_tablicy(COLUMNS - FIELDS + 1, COLUMNS, plansza_prawego, -1);
    } else
        zmien_wartosci_tablicy(0, COLUMNS, plansza_prawego, -1);
    if ((znak1 = getchar()) == '-') {
        if (mozliwosci_prawego == 0) {
            printf(".\n");
            return 0;
        }
        mozliwosci_prawego = ruch_prawego(najlepszy_ruch, plansza, plansza_prawego,
                                          mozliwosci_prawego);
        getchar(); //wczytujemy znak końca lini
        znak1 = getchar(); //wczytujemy znak nowej lini
    }
    while (znak1 != '.') {
        znak2 = getchar();
        mozliwosci_prawego = ruch_lewego((znak1 - 'A'), (znak2 - 'a'), plansza,
                                         plansza_prawego, mozliwosci_prawego);
        if (mozliwosci_prawego == 0) {
            printf(".\n");
            return 0;
        }
        mozliwosci_prawego = ruch_prawego(najlepszy_ruch, plansza, plansza_prawego,
                                          mozliwosci_prawego);
        getchar(); //wczytujemy znak końca lini
        znak1 = getchar(); //pierwszy znak nowej lini
    }
    return 0;
}
