#include "argparser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Find index of a char in a string
static int indexof(char* str, char c) {
    int i;
    for (i = 0; i < strlen(str); i++)
        if (str[i] == c)
            return i;
    return -1;
}

// Reformat argv to allow for --arg=val
static char** remove_equals(int* argc, char* argv[]) {
    // Find new argc
    int old_argc = *argc;
    int new_argc = old_argc;
    int i, j;
    for (i = 0; i < old_argc; i++)
        if (indexof(argv[i], '=') != -1)
            new_argc++;

    // Allocate new argv
    char** new_argv = (char**)malloc(new_argc * sizeof(char*));
    for (i = 0, j = 0; i < old_argc; i++) {
        int len = strlen(argv[i]);
        int idx = indexof(argv[i], '=');
        if (idx != -1) {
            new_argv[j]   = (char*)calloc(idx + 1,         1);
            new_argv[j+1] = (char*)calloc((len-idx-1) + 1, 1);

            memcpy(new_argv[j],   argv[i],       idx);
            memcpy(new_argv[j+1], argv[i]+idx+1, len-idx-1);

            j += 2;
        } else {
            new_argv[j] = (char*)calloc(len + 1, 1);

            memcpy(new_argv[j], argv[i], len);

            j += 1;
        }
    }

    // Update argc
    *argc = new_argc;

    return new_argv;
}

// Create new argparser
argparser argparser_create(int argc, char* argv[], Parsemode mode) {
    size_t init_cap = 5;

    // Reformat argv in case --arg=val notation is used
    argv = remove_equals(&argc, argv);

    argparser ap;
    ap.argc = argc;
    ap.argv = argv;
    ap.mode = mode;
    ap.args = (argstruct*)calloc(init_cap, sizeof(argstruct));
    ap.size = 0;
    ap.cap  = init_cap;

    return ap;
}

// Destroy argparser args
static void argparser_destroy(argparser* ap) {
    int i;

    for (i = 0; i < ap->size; i++) {
        free(ap->args[i].shortarg);
        free(ap->args[i].longarg);
    }
    free(ap->args);

    for (i = 0; i < ap->argc; i++)
        free(ap->argv[i]);
    free(ap->argv);
}

// Add arg to argparser
void argparser_add(argparser* ap, const char* shortarg, const char* longarg, Argtype type, void* arg, void (*callback)()) {
    argstruct as;
    as.shortarg = (char*)malloc(strlen(shortarg) + 1);
    as.longarg  = (char*)malloc(strlen(longarg) + 1);
    as.type     = type;
    as.arg      = arg;
    as.callback = callback;
    as.parsed   = 0;

    strcpy(as.shortarg, shortarg);
    strcpy(as.longarg, longarg);

    if (ap->size < ap->cap) {
        ap->args[ap->size++] = as;
    } else {
        size_t new_cap = ap->cap * 2;

        ap->args = (argstruct*)realloc(ap->args, new_cap * sizeof(argstruct));
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
            argstruct* as = &ap->args[j];

            int shortmatch = strcmp(ap->argv[i], as->shortarg) == 0;
            int longmatch  = strcmp(ap->argv[i], as->longarg) == 0;

            if (shortmatch || longmatch) {
                // Assign arg, if applicable
                if (as->arg) {
                    switch (as->type) {
                    case ARGTYPE_INT:
                        if (i+1 < ap->argc) {
                            *(int*)as->arg = atoi(ap->argv[++i]);
                            as->parsed = 1;
                        }
                        break;
                    case ARGTYPE_DOUBLE:
                        if (i+1 < ap->argc) {
                            *(double*)as->arg = atof(ap->argv[++i]);
                            as->parsed = 1;
                        }
                        break;
                    case ARGTYPE_STRING:
                        if (i+1 < ap->argc) {
                            strcpy((char*)as->arg, ap->argv[++i]);
                            as->parsed = 1;
                        }
                        break;
                    case ARGTYPE_BOOL:
                        *(int*)as->arg += 1;
                        as->parsed = 1;
                    default:
                        ;
                    }
                } else if (as->type == ARGTYPE_VOID) {
                    as->parsed = 1;
                }
            }
        }
    }

    // If strict, make sure all args were passed
    if (ap->mode== PARSEMODE_STRICT) {
        int failed = 0;
        for (i = 0; i < ap->size; i++) {
            argstruct as = ap->args[i];
            if (!as.parsed) {
                fprintf(stderr, "Failed to provide arg %s, %s\n", as.shortarg, as.longarg);
                failed = 1;
            }
        }
        if (failed) {
            argparser_destroy(ap);
            exit(-1);
        }
    }

    // Call callbacks, if applicable
    for (i = 0; i < ap->size; i++)
        if (ap->args[i].callback && ap->args[i].parsed)
            ap->args[i].callback();

    // Cleanup
    argparser_destroy(ap);
}
