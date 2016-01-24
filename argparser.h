#ifndef ARGPARSER_H_ 
#define ARGPARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef enum {
    ARGTYPE_VOID,
    ARGTYPE_INT,
    ARGTYPE_DOUBLE,
    ARGTYPE_STRING,
} Argtype;

// TODO: Either use malloc or come up with better default sizes
typedef struct {
    char shortarg[10];
    char longarg[20];
    Argtype type;
    void* arg;
    void (*callback)();
} argstruct;

typedef struct {
    int argc;
    char** argv;
    argstruct* args;
    size_t size;
    size_t cap;
} argparser;

argparser argparser_create(int argc, char* argv[]);
void argparser_destroy(argparser* ap);
void argparser_add(argparser* ap, const char* shortarg, const char* longarg, Argtype type, void* arg, void (*callback)());
void argparser_parse(argparser* ap);

#ifdef __cplusplus
}
#endif

#endif
