#include "operations.h"

#include <stdio.h>
#include <stdlib.h>

//Kończy program błędem
void error(int i) {
    fprintf(stderr, "ERROR %d\n", i);
    exit(1);
}

size_t ceil_divide(size_t a, size_t b) { return (a + b - 1) / b; }

void free_labyrinth(Labyrinth* lab) {
    free(lab->data);
    free(lab->prefix);
    for (int i = 0; i < 3; i++)
        free(lab->tab[i]);
}

size_t labyrinth_size(size_t** tab, int k) {
    size_t size = 1;
    for (int i = 0; i < k; i++) {
        if (SIZE_MAX / tab[0][i] < size)
            error(0);
        else
            size *= tab[0][i];
    }
    return size;
}

// Liczy iloczyn prefiksowy kolejno: 1, n_1, n_1*n_2 itd
void prefix(Labyrinth* lab) {
    lab->prefix = (size_t*)malloc((lab->k) * sizeof(size_t));
    if (lab->prefix == NULL)
        error(0);
    lab->prefix[0] = 1;

    for (size_t i = 1; i < lab->k; i++)
        lab->prefix[i] = lab->prefix[i - 1] * lab->tab[0][i - 1];
}

size_t find_index(size_t* tab, Labyrinth* lab) {
    size_t x = 0;
    for (size_t i = 0; i < lab->k; i++)
        x += (tab[i] - 1) * lab->prefix[i];
    return x;
}

bool check_index(size_t index, Labyrinth* lab) { // Czy pełna kostka
    return (lab->data[index / UINT8T_SIZE] & (1 << (index % UINT8T_SIZE)));
}