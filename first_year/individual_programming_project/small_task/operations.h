#ifndef __OPERATIONS_H
#define __OPERATIONS_H

#define _GNU_SOURCE // for getline()
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define UINT8T_SIZE 8

static const char WHITESPACES[] = " \t\v\f\r\n";

typedef struct {
    size_t k;
    uint8_t* data;
    size_t* prefix; // Iloczyn kolejnych wymiarów
    size_t* tab[3]; // Wymiary labiryntu, Pozycja początkowa, Pozycja końcowa
    size_t length_in_bits;
    size_t length_in_bytes;
} Labyrinth;

void error(int i);

size_t ceil_divide(size_t a, size_t b);

void free_labyrinth(Labyrinth* lab);

size_t labyrinth_size(size_t** tab, int k);

void prefix(Labyrinth* lab); // Liczy iloczyn prefiksowy

size_t find_index(size_t* tab,
                  Labyrinth* lab); //Zamienia współrzędne tablicowe na liczbowe

bool check_index(size_t index, Labyrinth* lab); // Czy pełna kostka

#endif