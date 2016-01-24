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

typedef enum {
    PARSE_STRICT,
    PARSE_LENIENT,
} Parsetype;

typedef struct {
    char* shortarg;
    char* longarg;
    Argtype type;
    void* arg;
    void (*callback)();
    int parsed;
} argstruct;

typedef struct {
    int argc;
    char** argv;
    Parsetype type;
    argstruct* args;
    size_t size;
    size_t cap;
} argparser;

argparser argparser_create(int argc, char* argv[], Parsetype type);
void argparser_destroy(argparser* ap);
void argparser_add(argparser* ap, const char* shortarg, const char* longarg, Argtype type, void* arg, void (*callback)());
void argparser_parse(argparser* ap);

#ifdef __cplusplus
}
#endif

#endif
