#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include "haffman.h"

int main(void) {
    setlocale(LC_ALL, "rus");
    clock_t begin = clock();
    //codify("c:\\in.txt", "c:\\binary.bin");
    decifer("c:\\binary.bin", "c:\\output.txt");
    printf("%.2lf\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
    return 0;
}
