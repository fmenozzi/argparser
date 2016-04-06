#include <stdio.h>
#include <string.h>

#include "argparser.h"

void assert(int pred, const char* msg) {
    if (!pred)
        fprintf(stderr, "ERROR: %s\n", msg);
}

int help_func_called = 0;
int verbose_func_called = 0;
int string_func_called = 0;

void help_func()    { help_func_called = 1; }
void verbose_func() { verbose_func_called = 1; }
void string_func()  { string_func_called = 1; }

/*
 * For arguments that take values (such as -d --double), you can 
 * use any of the following formats:
 *
 *      ./example -d 3
 *      ./example -d=3
 *      ./example --double 3
 *      ./example --double=3
 *
 * If you just want to check whether a flag is present (e.g. ./test -p),
 * use the ARGTYPE_BOOL option
 */

int main(int argc, char* argv[]) {
    int i = 0;
    double d = 0.0;
    char s[50] = "Zero";
    int p = 0;

    argparser ap = argparser_create(argc, argv, PARSEMODE_STRICT);

    argparser_add(&ap, "-h", "--help",    ARGTYPE_VOID,   NULL, help_func);
    argparser_add(&ap, "-v", NULL,        ARGTYPE_VOID,   NULL, verbose_func);
    argparser_add(&ap, NULL, "--integer", ARGTYPE_INT,    &i,   NULL);
    argparser_add(&ap, "-d", "--double",  ARGTYPE_DOUBLE, &d,   NULL);
    argparser_add(&ap, "-s", "--string",  ARGTYPE_STRING, &s,   string_func);
    argparser_add(&ap, "-p", "--present", ARGTYPE_BOOL,   &p,   NULL);

    argparser_parse(&ap);

    /* Args */
    assert(i != 0,                 "i was not modified");
    assert(d != 0.0,               "d was not modified");
    assert(strcmp(s, "Zero") != 0, "s was not modified");
    assert(p == 1,                 "p was not modified");

    /* Callbacks */
    assert(help_func_called,    "help_func() was not called");
    assert(verbose_func_called, "verbose_func() was not called");
    assert(string_func_called,  "string_func() was not called");

    /* Print arg values */
    fprintf(stdout, "i: %d\n", i);
    fprintf(stdout, "d: %f\n", d);
    fprintf(stdout, "s: %s\n", s);
    fprintf(stdout, "p: %d\n", p);

    return 0;
}
