#include "argparser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Create new argparser
argparser argparser_create(int argc, char* argv[]) {
    size_t init_cap = 5;

    argparser ap;
    ap.argc = argc;
    ap.argv = argv;
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
void argparser_add(argparser* ap, char shortarg[], char longarg[], Argtype type, void* arg, void (*callback)()) {
    argstruct as;
    strcpy(as.shortarg, shortarg);
    strcpy(as.longarg, longarg);
    as.type     = type;
    as.arg      = arg;
    as.callback = callback;

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

// Parse arguments
void argparser_parse(argparser* ap) {
    int i, j;
    for (i = 0; i < ap->argc; i++) {
        for (j = 0; j < ap->size; j++) {
            argstruct as = ap->args[j];

            int shortmatch = strcmp(ap->argv[i], as.shortarg) == 0;
            int longmatch  = strcmp(ap->argv[i], as.longarg) == 0;

            if (shortmatch || longmatch) {
                // Assign arg, if applicable
                if (as.arg) {
                    switch (as.type) {
                    case ARGTYPE_INT:
                        if (i+1 >= ap->argc)
                            break;
                        *(int*)as.arg = atoi(ap->argv[++i]);
                        break;
                    case ARGTYPE_DOUBLE:
                        if (i+1 >= ap->argc)
                            break;
                        *(double*)as.arg = atof(ap->argv[++i]);
                        break;
                    case ARGTYPE_STRING:
                        if (i+1 >= ap->argc)
                            break;
                        strcpy((char*)as.arg, ap->argv[++i]);
                        break;
                    case ARGTYPE_VOID:
                        *(int*)as.arg += 1;
                    default:
                        ;
                    }
                }

                // Call callback, if applicable
                if (as.callback)
                    as.callback();
            }
        }
    }
}
