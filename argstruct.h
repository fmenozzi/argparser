#ifndef ARGSTRUCT_H_
#define ARGSTRUCT_H_

typedef enum {
    ARGTYPE_VOID,
    ARGTYPE_INT,
} Argtype;

// TODO: Either use malloc or come up with better default sizes
typedef struct {
    char shortarg[10];
    char longarg[20];
    Argtype type;
    void* arg;
    void (*callback)();
} argstruct;

#endif
