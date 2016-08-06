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

#ifndef ARGPARSER_H_ 
#define ARGPARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef enum {
    ARGTYPE_INT,
    ARGTYPE_DOUBLE,
    ARGTYPE_STRING,
    ARGTYPE_BOOL
} Argtype;

typedef enum {
    PARSEMODE_STRICT,
    PARSEMODE_LENIENT
} Parsemode;

typedef struct {
    char* shortarg;
    char* longarg;
    Argtype type;
    void* arg;
    char* helpstr;
    int parsed;
} argstruct;

typedef struct {
    int argc;
    char** argv;
    Parsemode mode;
    argstruct* args;
    size_t size;
    size_t cap;
} argparser;

argparser argparser_create(int argc, char* argv[], Parsemode mode);
void argparser_add(argparser* ap, const char* shortarg, const char* longarg, Argtype type, void* arg, const char* helpstr);
void argparser_parse(argparser* ap);

#ifdef __cplusplus
}
#endif

#endif
