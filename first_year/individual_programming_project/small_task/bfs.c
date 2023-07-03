#include "bfs.h"
#include "operations.h"
#include "read.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct List_t {
    struct List_t* next;
    size_t index, distance;
};

typedef struct List_t List;

typedef struct {
    List* begin, *end;
} Que;

void createEmpty(Que* q) { q->begin = NULL; }

bool isEmpty(Que* q) { return q->begin == NULL; }

void insert_after(size_t index, size_t distance, List* q) { /* q ≠ NULL */
    List* sup = (List*)malloc(sizeof(List));
    if (sup == NULL)
        error(0);
    sup->next = q->next;
    sup->index = index;
    sup->distance = distance;
    q->next = sup;
}

void insert(Que* q, size_t index, size_t distance) {
    if (q->begin == NULL) {
        q->begin = (List*)malloc(sizeof(List));
        if (q->begin == NULL)
            error(0);
        q->begin->index = index;
        q->begin->distance = distance;
        q->end = q->begin;
    } else {
        insert_after(index, distance, q->end);
        q->end = q->end->next;
    }
}

void removeFirst(Que* q) {
    List* sup = q->begin;
    if (q->begin == q->end)
        q->begin = NULL;
    else
        q->begin = q->begin->next;
    free(sup);
}

void removeALL(Que* q) {
    while (!isEmpty(q))
        removeFirst(q);
}

//Wyznacza wszystkie pozycje, do których można dotrzeć z pola o danym indexie
void neighbors(Labyrinth* lab, size_t index, size_t distance, Que* q) {

    size_t val = index;
    size_t* geo;
    geo = (size_t*)malloc((lab->k) * sizeof(size_t));
    if (geo == NULL)
        error(0);

    for (long i = lab->k - 1; i >= 0;
            i--) { // Znajdż geolokacje (odszyfruj index)
        geo[i] = val / lab->prefix[i];
        val = val % lab->prefix[i];
    }
    for (size_t i = 0; i < lab->k; i++) {
        if (geo[i] > 0) { // Odejmujemy 1 wstawimy w kolejke
            val = index - lab->prefix[i];
            if (!check_index(val, lab)) {
                lab->data[val / UINT8T_SIZE] += 1 << (val % UINT8T_SIZE);
                insert(q, val, distance);
            }
        }
        if (geo[i] + 1 < lab->tab[0][i]) { // Dodajemy 1 i wrzucamy do kolejki
            val = index + lab->prefix[i];
            if (!check_index(val, lab)) {
                lab->data[val / UINT8T_SIZE] += 1 << (val % UINT8T_SIZE);
                insert(q, val, distance);
            }
        }
    }
    free(geo);
    removeFirst(q);
}

void bfs(Labyrinth* lab) {

    Que q;
    createEmpty(&q);

    size_t x = find_index(lab->tab[1], lab);

    size_t y = find_index(lab->tab[2], lab);

    size_t distance = 0;
    size_t index;

    insert(&q, x, distance);
    lab->data[x / UINT8T_SIZE] += 1 << (x % UINT8T_SIZE);

    while (!isEmpty(&q)) {
        index = q.begin->index;
        distance = q.begin->distance;
        if (index == y) {
            removeALL(&q);
            free_labyrinth(lab);
            printf("%ld\n", distance);
            exit(0);
        } else {
            distance++;
            neighbors(lab, index, distance, &q);
        }
    }

    free_labyrinth(lab);
    printf("NO WAY\n");
    exit(0);
}