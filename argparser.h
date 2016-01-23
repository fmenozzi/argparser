#ifndef ARGPARSER_H_ 
#define ARGPARSER_H_

#include <stdlib.h>

#include "argstruct.h"

typedef struct {
    int argc;
    char** argv;
    argstruct* args;
    size_t size;
    size_t cap;
} argparser;

argparser argparser_create(int argc, char* argv[]);
void argparser_destroy(argparser* ap);
void argparser_add(argparser* ap, char shortarg[], char longarg[], Argtype type, void* arg, void (*callback)());
void argparser_parse(argparser* ap);

#endif
