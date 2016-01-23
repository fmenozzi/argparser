#include <stdio.h>

#include "argparser.h"

void help_func() {
    printf("You passed the help verbose\n");
}

void verbose_func() {
    printf("You passed the verbose flag\n");
}

void argprint(argparser* ap) {
    int i;
    for (i = 0; i < ap->size; i++) {
        printf("Passed: %s, %s\n", ap->args[i].shortarg, ap->args[i].longarg);
    }
}

int main(int argc, char* argv[]) {
    int num;

    argparser ap = argparser_create(argc, argv);

    argparser_add(&ap, "-h", "--help",    ARGTYPE_VOID, NULL, help_func);
    argparser_add(&ap, "-v", "--verbose", ARGTYPE_VOID, NULL, verbose_func);
    argparser_add(&ap, "-n", "--num",     ARGTYPE_INT,  &num, NULL);

    argprint(&ap);

    argparser_destroy(&ap);

    return 0;
}
