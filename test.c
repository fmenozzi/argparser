#include <stdio.h>

#include "argstruct.h"
#include "argvec.h"

void printvec(argvec* vec) {
    int i;
    for (i = 0; i < vec->cap; i++)
        printf("%d ", argvec_at(vec, i).data);
    printf("\nsize: %lu, cap: %lu\n\n", vec->size, vec->cap);
}

argstruct as(int data) {
    argstruct a = {data};
    return a;
}

int main() {
    argvec av = argvec_create(5);

    int i;
    for (i = 0; i < 6; i++) {
        argvec_add(&av, as(i));
        printvec(&av);
    }
    
    for (i = 6; i < 21; i++) {
        argvec_add(&av, as(i));
        printvec(&av);
    }

    argvec_destroy(&av);

    return 0;
}
