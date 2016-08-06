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

#include <stdio.h>
#include <string.h>

#include "argparser.h"

void assert(int pred, const char* msg) {
    if (!pred)
        fprintf(stderr, "ERROR: %s\n", msg);
}

/*
 * For arguments that take values (such as -d, --double), you can
 * use any of the following formats:
 *
 *      ./test -d 3
 *      ./test -d=3
 *      ./test --double 3
 *      ./test --double=3
 *
 * If you just want to check whether a flag is present (e.g. ./test -p),
 * use the ARGTYPE_BOOL option. These flags can be combined during
 * program execution (e.g. "./test -hv" is equivalent to "./test -h -v")
 */

int main(int argc, char* argv[]) {
    int h = 0;
    int v = 0;
    int i = 0;
    double d = 0.0;
    char s[50] = "Zero";
    int p = 0;
    int f = 0;

    argparser ap = argparser_create(argc, argv, PARSEMODE_STRICT);

    argparser_add(&ap, "-h", "--help",    ARGTYPE_BOOL,   &h);
    argparser_add(&ap, "-v", NULL,        ARGTYPE_BOOL,   &v);
    argparser_add(&ap, NULL, "--integer", ARGTYPE_INT,    &i);
    argparser_add(&ap, "-d", "--double",  ARGTYPE_DOUBLE, &d);
    argparser_add(&ap, "-s", "--string",  ARGTYPE_STRING, &s);
    argparser_add(&ap, "-p", "--present", ARGTYPE_BOOL,   &p);
    argparser_add(&ap, "-f", "--flag",    ARGTYPE_BOOL,   &f);

    argparser_parse(&ap);

    /* Args */
    assert(h != 0,                 "h was not modified");
    assert(v != 0,                 "v was not modified");
    assert(i != 0,                 "i was not modified");
    assert(d != 0.0,               "d was not modified");
    assert(strcmp(s, "Zero") != 0, "s was not modified");
    assert(p == 1,                 "p was not modified");
    assert(f == 1,                 "f was not modified");

    /* Print arg values */
    fprintf(stdout, "h: %d\n", h);
    fprintf(stdout, "v: %d\n", v);
    fprintf(stdout, "i: %d\n", i);
    fprintf(stdout, "d: %f\n", d);
    fprintf(stdout, "s: %s\n", s);
    fprintf(stdout, "p: %d\n", p);
    fprintf(stdout, "f: %d\n", f);

    return 0;
}
