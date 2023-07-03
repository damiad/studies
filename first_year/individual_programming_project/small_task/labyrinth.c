#include "bfs.h"
#include "operations.h"
#include "read.h"

#include <stddef.h>

int main(void) {
    Labyrinth labyrinth;

    labyrinth.data = NULL;
    labyrinth.prefix = NULL;
    for (int i = 0; i < 3; i++)
        labyrinth.tab[i] = NULL;

    read_line(&labyrinth, 1, &labyrinth.k);

    size_t d; //Pomocnicza
    for (int i = 2; i <= 3; i++)
        read_line(&labyrinth, i, &d);

    labyrinth.length_in_bits = labyrinth_size(labyrinth.tab, labyrinth.k);
    labyrinth.length_in_bytes = ceil_divide(labyrinth.length_in_bits, UINT8T_SIZE);

    read_labyrinth(&labyrinth);

    bfs(&labyrinth);

    return 0;
}