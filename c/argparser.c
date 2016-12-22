/*
The MIT License (MIT)

Copyright (c) 2016 Federico Menozzi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "argparser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Find index of a char in a string
 */
static int indexof(char* str, char c) {
    int i;
    for (i = 0; i < (int)strlen(str); i++)
        if (str[i] == c)
            return i;
    return -1;
}

/*
 * Reformat argv to allow for --arg=val
 */
static char** remove_equals(int* argc, char* argv[]) {
    int old_argc, new_argc;
    char** new_argv;
    int i, j;

    /* Find new argc */
    old_argc = *argc;
    new_argc = old_argc;
    for (i = 0; i < old_argc; i++)
        if (indexof(argv[i], '=') != -1)
            new_argc++;

    /* Allocate new argv */
    new_argv = (char**)malloc(new_argc * sizeof(char*));
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

    /* Update argc */
    *argc = new_argc;

    return new_argv;
}

/*
 * Determine if arg is of form -abc
 */
static int is_multi_shortarg(char* str) {
    return str[0] == '-' && str[1] != '-' && strlen(str) > 2;
}

/*
 * Reformat argv to allow for ./test -abc equivalent of ./test -a -b -c
 */
static char** expand_shortargs(int* argc, char* argv[]) {
    int old_argc, new_argc;
    char** new_argv;
    int i, j;

    /* Find new argc */
    old_argc = *argc;
    new_argc = old_argc;
    for (i = 0; i < old_argc; i++)
        if (is_multi_shortarg(argv[i]))
            new_argc += strlen(argv[i]) - 2;

    /* Allocate new argv */
    new_argv = (char**)malloc(new_argc * sizeof(char*));
    for (i = 0, j = 0; i < old_argc; i++) {
        int len = strlen(argv[i]);
        if (is_multi_shortarg(argv[i])) {
            int k;
            for (k = 0; k < len-1; k++) {
                new_argv[j+k] = (char*)calloc(3, 1);
                memcpy(new_argv[j+k],   "-",         1);
                memcpy(new_argv[j+k]+1, argv[i]+k+1, 1);
            }
            j += len-1;
        } else {
            new_argv[j] = (char*)calloc(len + 1, 1);
            memcpy(new_argv[j], argv[i], len);
            j += 1;
        }
    }

    /* Since argv here is dynamically-allocated, free it */
    for (i = 0; i < *argc; i++)
        free(argv[i]);
    free(argv);

    /* Update argc */
    *argc = new_argc;

    return new_argv;
}

/*
 * Print help string with argparser's current args
 */
static void print_help_string(argparser* ap) {
    int i, j;
    int max_len = strlen("-h, --help");
    int rightpad = 4;

    const char* leftpadstr = "    ";

    const char* lbrak = ap->mode == PARSEMODE_LENIENT ? "[" : "";
    const char* rbrak = ap->mode == PARSEMODE_LENIENT ? "]" : "";

    fprintf(stdout, "Usage: %s [-h,--help] ", ap->argv[0]);
    for (i = 0; i < (int)ap->size; i++) {
        argstruct* as = &ap->args[i];
        if (as->shortarg) {
            if (as->longarg)
                fprintf(stdout, "%s%s,%s%s ", lbrak, as->shortarg, as->longarg, rbrak);
            else
                fprintf(stdout, "%s%s%s ", lbrak, as->shortarg, rbrak);
        } else {
            fprintf(stdout, "%s%s%s ", lbrak, as->longarg, rbrak);
        }
    }
    fprintf(stdout, "\n\n");

    for (i = 0; i < (int)ap->size; i++) {
        argstruct* as = &ap->args[i];

        int shortlen = as->shortarg ? strlen(as->shortarg) : 0;
        int longlen  = as->longarg  ? strlen(as->longarg)  : 0;

        int arg_len = (shortlen && longlen) ? (shortlen + 2 + longlen) : (shortlen + longlen);
        if (arg_len > max_len)
            max_len = arg_len;
    }

    fprintf(stdout, "Arguments:\n");
    fprintf(stdout, "%s-h, --help", leftpadstr);
    for (i = 0; i < (int)(max_len + rightpad - strlen("-h, --help")); i++)
        fprintf(stdout, " ");
    fprintf(stdout, "Show this help message and exit\n");
    for (i = 0; i < (int)ap->size; i++) {
        argstruct* as = &ap->args[i];

        if (as->shortarg) {
            if (as->longarg)
                fprintf(stdout, "%s%s, %s", leftpadstr, as->shortarg, as->longarg);
            else
                fprintf(stdout, "%s%s", leftpadstr, as->shortarg);
        } else {
            fprintf(stdout, "%s%s", leftpadstr, as->longarg);
        }

        int shortlen = as->shortarg ? strlen(as->shortarg) : 0;
        int longlen  = as->longarg  ? strlen(as->longarg)  : 0;
        int arg_len = (shortlen && longlen) ? (shortlen + 2 + longlen) : (shortlen + longlen);
        for (j = 0; j < max_len + rightpad - arg_len; j++)
            fprintf(stdout, " ");

        fprintf(stdout, "%s\n", as->helpstr);
    }
}

/*
 * Destroy argparser args
 */
static void argparser_destroy(argparser* ap) {
    if (ap) {
        int i;

        for (i = 0; i < (int)ap->size; i++) {
            if (ap->args[i].shortarg)
                free(ap->args[i].shortarg);
            if (ap->args[i].longarg)
                free(ap->args[i].longarg);
            if (ap->args[i].helpstr)
                free(ap->args[i].helpstr);
        }
        free(ap->args);

        for (i = 0; i < ap->argc; i++)
            free(ap->argv[i]);
        free(ap->argv);
    }
}

/*
 * Cleanup resources and abort program
 */
static void argparser_abort(argparser* ap, const char* msg) {
    fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "Aborting\n");
    argparser_destroy(ap);
    exit(EXIT_FAILURE);
}

/*
 * Create new argparser
 */
argparser argparser_create(int argc, char* argv[], Parsemode mode) {
    size_t init_cap = 5;
    argparser ap;

    /* Reformat argv in case --arg=val notation is used */
    argv = remove_equals(&argc, argv);

    /* Expand shortargs in case -ab notation is used */
    argv = expand_shortargs(&argc, argv);

    ap.argc = argc;
    ap.argv = argv;
    ap.mode = mode;
    ap.args = (argstruct*)calloc(init_cap, sizeof(argstruct));
    ap.size = 0;
    ap.cap  = init_cap;

    return ap;
}

/*
 * Add arg to argparser
 */
void argparser_add(argparser* ap, const char* shortarg, const char* longarg, Argtype type, void* arg, const char* helpstr) {
    argstruct as;
    as.shortarg = NULL;
    as.longarg  = NULL;
    as.type     = type;
    as.arg      = arg;
    as.helpstr  = NULL;
    as.parsed   = 0;
   
    /* No null ap */
    if (!ap)
        argparser_abort(ap, "Passed NULL argparser pointer to argparser_add");

    /* No null or empty argstrings */
    if (!shortarg && !longarg)
        argparser_abort(ap, "Passed NULL pointers to both argstrings");
    if (shortarg && strcmp(shortarg, "") == 0)
        argparser_abort(ap, "Passed empty string to shortarg");
    if (longarg && strcmp(longarg, "") == 0)
        argparser_abort(ap, "Passed empty string to longarg");

    /* Shortarg must be a dash followed by a single character */
    if (shortarg && (strlen(shortarg) != 2 || shortarg[0] != '-' || shortarg[1] == '-'))
        argparser_abort(ap, "Shortarg must be a single dash followed by a single character");

    /* Longarg must be two dashes followed by any number of additional characters */
    if (longarg && (strlen(longarg) <= 2 || longarg[0] !=  '-' || longarg[1] != '-'))
        argparser_abort(ap, "Longarg must be two dashes followed by any number of additional characters");

    /* -h, --help are reserved */
    if (shortarg && strcmp(shortarg, "-h") == 0)
        argparser_abort(ap, "-h is reserved short arg");
    if (longarg && strcmp(longarg, "--help") == 0)
        argparser_abort(ap, "--help is reserved long arg");

    /* No null arg */
    if (!arg)
        argparser_abort(ap, "Passed NULL arg pointer to argparser_add");

    /* No null help string */
    if (!helpstr)
        argparser_abort(ap, "Passed NULL help string to argparser_add");

    /* Copy argstrings */
    if (shortarg) {
        as.shortarg = (char*)malloc(strlen(shortarg) + 1);
        strcpy(as.shortarg, shortarg);
    }
    if (longarg) {
        as.longarg  = (char*)malloc(strlen(longarg) + 1);
        strcpy(as.longarg, longarg);
    }

    /* Copy help string */
    if (helpstr) {
        as.helpstr = (char*)malloc(strlen(helpstr) + 1);
        strcpy(as.helpstr, helpstr);
    }

    if (as.type == ARGTYPE_BOOL)
        *(int*)as.arg = 0;

    /* Add argstruct to dynamic array */
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

/*
 * Parse arguments
 */
void argparser_parse(argparser* ap) {
    int i, j;

    int help_passed = 0;

    for (i = 0; i < ap->argc; i++) {
        for (j = 0; j < (int)ap->size; j++) {
            argstruct* as = &ap->args[j];

            int shortmatch = as->shortarg ? strcmp(ap->argv[i], as->shortarg) == 0 : 0;
            int longmatch  = as->longarg  ? strcmp(ap->argv[i], as->longarg)  == 0 : 0;

            if (shortmatch || longmatch) {
                /* Assign arg */
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
                        *(int*)as->arg = 1;
                        as->parsed = 1;
                        break;
                }
            }
        }
    }

    /* Check if -h, --help was passed as only arg */
    if (ap->argc == 2) {
        char* arg = ap->argv[1];

        int shorthelp = strcmp(arg, "-h") == 0;
        int longhelp  = strcmp(arg, "--help") == 0;

        if (shorthelp || longhelp) {
            print_help_string(ap);
            help_passed = 1;
        }
    }

    if (help_passed) {
        argparser_destroy(ap);
        exit(EXIT_SUCCESS);
    }

    /* If strict, make sure all args were passed */
    if (!help_passed && ap->mode == PARSEMODE_STRICT) {
        int failed = 0;
        for (i = 0; i < (int)ap->size; i++) {
            argstruct as = ap->args[i];
            if (!as.parsed) {
                const char* sa = as.shortarg;
                const char* la = as.longarg;
                if (sa && la)
                    fprintf(stderr, "Failed to provide arg %s, %s\n", sa, la);
                else
                    fprintf(stderr, "Failed to provide arg %s\n", sa ? sa : la);

                failed = 1;
            }
        }
        if (failed) {
            argparser_abort(ap, "Failed to provide mandatory argument(s)");
        }
    }

    /* Cleanup */
    argparser_destroy(ap);
}
