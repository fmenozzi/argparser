#include "argvec.h"
#include "argstruct.h"

#include <stdlib.h>
#include <string.h>

// Create new argvec
argvec argvec_create(size_t init_cap) {
    argvec v = {
        calloc(init_cap, sizeof(argstruct)),
        0,
        init_cap
    };
    return v;
}

// Destroy argvec
void argvec_destroy(argvec* v) {
    free(v->args);
}

// Add arg to argvec
void argvec_add(argvec* av, argstruct as) {
    if (av->size < av->cap) {
        av->args[av->size++] = as;
    } else {
        size_t new_cap = av->cap * 2;

        av->args = realloc(av->args, new_cap * sizeof(argstruct));
        av->size = av->size + 1;
        av->cap  = new_cap;

        av->args[av->size - 1] = as;

        memset(av->args + av->size, 0, sizeof(argstruct) * (new_cap - av->size));
    }
}

// Retrieve element from argvec
argstruct argvec_at(argvec* av, size_t i) {
    return av->args[i];
}

// Clear all elements from argvec
void argvec_clear(argvec* av) {
    free(av->args);
    av->args = calloc(av->cap, sizeof(argstruct));
    av->size = 0;
}

