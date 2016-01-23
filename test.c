#include <stdio.h>

#include "argparser.h"

void printargs(argparser* ap) {
    int i;
    for (i = 0; i < ap->cap; i++)
        printf("%d ", argparser_at(ap, i).data);
    printf("\nsize: %lu, cap: %lu\n\n", ap->size, ap->cap);
}

argstruct as(int data) {
    argstruct a = {data};
    return a;
}

int main() {
    argparser av = argparser_create(5);

    int i;
    for (i = 0; i < 6; i++) {
        argparser_add(&av, as(i));
        printargs(&av);
    }
    
    for (i = 6; i < 21; i++) {
        argparser_add(&av, as(i));
        printargs(&av);
    }

    argparser_destroy(&av);

    return 0;
}
