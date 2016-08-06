Simple command line parser for C/C++ programs. Use `make` to run test code with `gcc`. Check out the Makefile for more.

The API is quite simple, with only three functions: `argparser_create`, `argparser_add`, and `argparser_parse`.

### argparser_create
Takes system arguments (i.e. `argc` and `argv`) and a parse mode and returns an `argparser` object. Parse mode is either `PARSEMODE_STRICT`, where the program exits if all args haven't been supplied, or `PARSEMODE_LENIENT`, which will allow unspecified args.

    argparser ap = argparser_create(argc, argv, PARSEMODE_LENIENT);
    
### argparser_add
Adds an argument to the parser. There are five parameters to this function:
+ A pointer to an `argparser` object
+ The shortarg string
    + Must be a single dash followed by a single character (e.g. `-v`)
+ The longarg string
    + Must be two dashes followed by any number of additional characters (e.g. `--verbose`)
+ An enum describing the type of argument. One of `ARGTYPE_INT`, `ARGTYPE_DOUBLE`, `ARGTYPE_STRING`, or `ARGTYPE_BOOL`
+ A pointer containing the address of the variable you wish to populate. Must be non-NULL.

At least one of `shortarg` and `longarg` must be non-NULL strings, and neither can be the empty string.

For example, the following line of code

    argparser_add(&ap, "-v", "--verbose", ARGTYPE_BOOL, &v);
    
will register the arguments `-v` and `--verbose` and increment the variable `h` by one once `argparser_parse` is called. Check out `test.c` for more.

### argparser_parse
Begins parsing the command line args, populating values. All resources will be cleaned up at the end of this function call.

    argparser_parse(&ap);
