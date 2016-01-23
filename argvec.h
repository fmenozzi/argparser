#ifndef ARGVEC_H_
#define ARGVEC_H_

#include <stdlib.h>

#include "argstruct.h"

// Define vector of argstructs
typedef struct {
    argstruct* args;
    size_t size;
    size_t cap;
} argvec;

argvec argvec_create(size_t init_cap);
void argvec_destroy(argvec* av);
void argvec_add(argvec* av, argstruct as);
argstruct argvec_at(argvec* av, size_t i);
void argvec_clear(argvec* av);

#endif
