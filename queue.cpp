#include <stdlib.h>
#include <string.h>
#include "queue.h"

static void sift_up   (void *, size_t, size_t, int (*)(const void *, const void *));
static void sift_down (void *, size_t, size_t, int (*)(const void *, const void *));
static void swap	  (void *, void *, size_t);

void top(
    void *where,
    void *topElement,
    size_t sizeOfTypeArray
) {
    memcpy(topElement, where, sizeOfTypeArray);
}

void pop(
    void *wherePop,
    void *whatPop,
    size_t *sizeOfArray,
    size_t sizeOfTypeArray,
    int (*cmp)(const void *, const void *)
) {
    memcpy(whatPop, wherePop, sizeOfTypeArray);
    *sizeOfArray -= 1;
    memcpy(wherePop, ((char *)wherePop + *sizeOfArray * sizeOfTypeArray), sizeOfTypeArray);
    sift_down(wherePop, *sizeOfArray, sizeOfTypeArray, cmp);
}

static void sift_down(
    void *a,
    size_t sizeOfArray,
    size_t sizeOfTypeArray,
    int (*cmp)(const void *, const void *)
) {
    size_t i = 0, j;
    while(i * 2 + 1 < sizeOfArray) {
        j = i;
        if(cmp(((char *)a + j * sizeOfTypeArray), ((char *)a + (i * 2 + 1) * sizeOfTypeArray)) > 0) {
            j = i * 2 + 1;
        }
        if(j * 2 + 2 < sizeOfArray && cmp(((char *)a + j * sizeOfTypeArray), ((char *)a + (i * 2 + 2) * sizeOfTypeArray)) > 0) {
            j = i * 2 + 2;
        }
        if(i == j) {
            break;
        }
        swap(((char *)a + i * sizeOfTypeArray), ((char *)a + j * sizeOfTypeArray), sizeOfTypeArray);
        i = j;
    }
}

void push(
    void *wherePush,
    void *whatPush,
    size_t *sizeOfArray,
    size_t sizeOfTypeArray,
    int (*cmp)(const void *, const void *)
) {
    memcpy(((char *)wherePush + *sizeOfArray * sizeOfTypeArray), whatPush, sizeOfTypeArray);
    sift_up(wherePush, *sizeOfArray, sizeOfTypeArray, cmp);
    *sizeOfArray += 1;
}

static void sift_up(
    void *a,
    size_t index,
    size_t sizeOfTypeArray,
    int (*cmp)(const void *, const void *)
) {
    size_t j = !(index % 2) ? index / 2 - 1 : index / 2;
    while(j >= 0 && cmp(((char *)a + index * sizeOfTypeArray),  ((char *)a + j * sizeOfTypeArray)) < 0) {
        swap(((char *)a + index * sizeOfTypeArray), ((char *)a + j * sizeOfTypeArray), sizeOfTypeArray);
        index = j;
        j = !(index % 2) ? index / 2 - 1 : index / 2;
    }
}

static void swap(void *a, void *b, size_t size) {
    void *t = malloc(size);
    memcpy(t, a, size);
    memcpy(a, b, size);
    memcpy(b, t, size);
    free(t);
}
