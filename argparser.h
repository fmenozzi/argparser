#ifndef ARGPARSER_H_ 
#define ARGPARSER_H_

#include <stdlib.h>

#include "argstruct.h"

typedef struct {
    argstruct* args;
    size_t size;
    size_t cap;
} argparser;

argparser argparser_create();
void argparser_destroy(argparser* ap);
void argparser_add(argparser* ap, argstruct as);
argstruct argparser_at(argparser* ap, size_t i);

#endif
