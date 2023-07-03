#include "read.h"
#include "operations.h"

#define _GNU_SOURCE // for getline()
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Wczytuje 3 pierwsze linie
void read_line(Labyrinth* lab, int line_number, size_t* d) {
    char* str_line = NULL;
    size_t buffer_size = 0;
    int len = (int)getline(&str_line, &buffer_size, stdin);
    if (len < 0 || str_line[len - 1] != '\n') {
        free(str_line);
        free_labyrinth(lab);
        error(line_number);
    }
    *d = 0;
    char* line_copy = strdup(str_line);
    if (line_copy == NULL)
        error(0);
    char* token = strtok(str_line, WHITESPACES);
    while (token) {
        char* end;
        unsigned long long a = strtoull(token, &end, 10);
        if (0 < a && a <= SIZE_MAX && errno == 0 && *end == '\0')
            (*d)++;
        else {
            free_labyrinth(lab);
            free(str_line);
            free(line_copy);
            error(line_number);
        }
        token = strtok(NULL, WHITESPACES);
    }
    free(str_line);
    if ((*d) != lab->k) {
        free_labyrinth(lab);
        free(line_copy);
        error(line_number);
    }

    lab->tab[line_number - 1] = (size_t*)malloc(lab->k * sizeof(size_t));
    if (lab->tab[line_number - 1] == NULL)
        error(0);

    token = strtok(line_copy, WHITESPACES);

    //Przypisujemy wymiary bądż współrzędne początku i końca
    for (size_t i = 0; i < lab->k; i++) {
        char* end;
        unsigned long long a = strtoull(token, &end, 10);
        lab->tab[line_number - 1][i] = (size_t)a;
        if (lab->tab[0][i] < lab->tab[line_number - 1][i]) {
            free(line_copy);
            free_labyrinth(lab);
            error(line_number);
        }
        token = strtok(NULL, WHITESPACES);
    }
    free(line_copy);
}

bool read_in_16(char* input, Labyrinth* lab) {
    while (input[0] == '0')
        ++input;
    size_t hex_data_size = strlen(input);

    if (hex_data_size > ceil_divide(lab->length_in_bits, 4))
        return true;

    for (long i = hex_data_size - 1; i >= 0; --i) {
        uint8_t cur;

        if (input[i] >= '0' && input[i] <= '9')
            cur = input[i] - '0';
        else if (input[i] >= 'a' && input[i] <= 'f')
            cur = 10 + input[i] - 'a';
        else if (input[i] >= 'A' && input[i] <= 'F')
            cur = 10 + input[i] - 'A';
        else
            return true;

        size_t chars_processed = hex_data_size - 1 - i;
        lab->data[chars_processed / 2] +=
            cur << (chars_processed % 2
                    ? 4
                    : 0); // Jeśli mod 2==1 to przesunięte 4 bity
    }
    if (lab->length_in_bits % 8 != 0)
        if (lab->data[lab->length_in_bytes - 1] >=
                (1 << (lab->length_in_bits % 8)))
            return true;
    return false;
}

//Czwarta linia wejścia
void read_labyrinth(Labyrinth* lab) {

    prefix(lab);
    char* str_line = NULL;
    size_t buffer_size = 0;
    int len = (int)getline(&str_line, &buffer_size, stdin);
    if (len < 0) {
        free_labyrinth(lab);
        free(str_line);
        error(4);
    }

    char* token = strtok(str_line, WHITESPACES);
    if (token == NULL) {
        free_labyrinth(lab);
        free(str_line);
        error(4);
    }

    lab->data = (uint8_t*)malloc(lab->length_in_bytes * sizeof(uint8_t));
    if (lab->data == NULL)
        error(0);
    memset(lab->data, 0, lab->length_in_bytes);

    if (token[0] == '0' && token[1] == 'x') {
        if (read_in_16(token + 2, lab)) {
            free_labyrinth(lab);
            free(str_line);
            error(4);
        }
        token = strtok(NULL, WHITESPACES);
        if (token != NULL) {
            free_labyrinth(lab);
            free(str_line);
            error(4);
        }
    } else if (token[0] == 'R') {
        if (token[1] == '\0')
            token = strtok(NULL, WHITESPACES);
        else
            ++token;
        int decimal[5]; // a,b,m,r,s_0
        for (size_t i = 0; i < 5; ++i) {
            if (token == NULL) {
                free_labyrinth(lab);
                free(str_line);
                error(4);
            }

            char* end;
            long long a = strtoll(token, &end, 10);
            if (0 <= a && a <= UINT32_MAX && errno == 0 && *end == '\0')
                decimal[i] = a;
            else {
                free_labyrinth(lab);
                free(str_line);
                error(4);
            }
            token = strtok(NULL, WHITESPACES);
        }
        if (token != NULL) {
            free_labyrinth(lab);
            free(str_line);
            error(4);
        }
        if (decimal[2] == 0) {
            free_labyrinth(lab);
            free(str_line);
            error(4);
        }
        size_t* s;
        s = (size_t*)malloc((size_t)(decimal[3]) * sizeof(size_t));
        if (s == NULL)
            error(0);
        decimal[0] = decimal[0] % decimal[2];
        decimal[1] = decimal[1] % decimal[2];
        s[0] = ((decimal[0] * (decimal[4] % decimal[2])) % decimal[2] +
                decimal[1]) %
               decimal[2];
        for (int i = 1; i < decimal[3]; i++) {
            s[i] = ((decimal[0] * s[i - 1]) % decimal[2] + decimal[1]) %
                   decimal[2]; //(a*s_(i−1)+b)%m .
        }

        for (int i = 0; i < decimal[3]; i++) {
            s[i] = s[i] % lab->length_in_bits;
            s[i] = s[i] % (1ll << 32); // Teraz to w[i]
            for (size_t w = s[i]; w < lab->length_in_bits; w += 1ll << 32) {
                if (check_index(w, lab))
                    break;
                lab->data[w / UINT8T_SIZE] += 1 << (w % UINT8T_SIZE);
            }
        }
        free(s);
    } else {
        free_labyrinth(lab);
        free(str_line);
        error(4);
    }

    free(str_line);

    size_t x = find_index(lab->tab[1], lab);
    if (check_index(x, lab)) {
        free_labyrinth(lab);
        error(2);
    }

    x = find_index(lab->tab[2], lab);
    if (check_index(x, lab)) {
        free_labyrinth(lab);
        error(3);
    }

    char next_char = getchar();
    if (next_char != EOF) {
        free_labyrinth(lab);
        error(5);
    }
}
