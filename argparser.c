#include "argparser.h"
#include "argstruct.h"

#include <stdlib.h>
#include <string.h>

// Create new argparser
argparser argparser_create() {
    size_t init_cap = 5;

    argparser ap;
    ap.args = calloc(init_cap, sizeof(argstruct));
    ap.size = 0;
    ap.cap  = init_cap;

    return ap;
}

// Destroy argparser args
void argparser_destroy(argparser* ap) {
    free(ap->args);
}

// Add arg to argparser
void argparser_add(argparser* ap, argstruct as) {
    if (ap->size < ap->cap) {
        ap->args[ap->size++] = as;
    } else {
        size_t new_cap = ap->cap * 2;

        ap->args = realloc(ap->args, new_cap * sizeof(argstruct));
        ap->size = ap->size + 1;
        ap->cap  = new_cap;

        ap->args[ap->size - 1] = as;

        memset(ap->args + ap->size, 0, sizeof(argstruct) * (new_cap - ap->size));
    }
}

// Retrieve element from argparser
argstruct argparser_at(argparser* ap, size_t i) {
    return ap->args[i];
}
