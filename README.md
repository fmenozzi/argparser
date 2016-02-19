Simple command line parser for C/C++ programs. Use `make test` to run test code.

The API is quite simple, with only three functions: `argparser_create`, `argparser_add`, and `argparser_parse`.

### argparser_create
Takes system arguments (i.e. `argc` and `argv`) and a parse mode and returns an `argparser` object. Parse mode is either `PARSEMODE_STRICT`, where the program exits if all args haven't been supplied, or `PARSEMODE_LENIENT`, which will allow unspecified args.

    argparser ap = argparser_create(argc, argv, PARSEMODE_LENIENT);
    
### argparser_add
Adds an argument to the parser. There are six parameters to this function:
+ A pointer to an `argparser` object
+ The shortarg string (e.g. `-h`)
+ The longarg string (e.g. `--help`)
+ An enum describing the type of argument. One of `ARGTYPE_INT`, `ARGTYPE_DOUBLE`, `ARGTYPE_STRING`, `ARGTYPE_BOOL`, or `ARGTYPE_VOID`
+ A pointer containing the address of the variable you wish to populate. `NULL` can safely be passed if you're using `ARGTYPE_VOID`
+ A callback function pointer. `NULL` can safely be passed if you don't need a callback.

For test, the following line of code

    argparser_add(&ap, "-h", "--help", ARGTYPE_VOID, NULL, help_callback);
    
will register the arguments `-h` and `--help` and call the function `help_callback` once `argparse_parse` is called. Check out `test.c` for more.

### argparser_parse
Begins parsing the command line args, populating values and calling callbacks. All resources will be cleanup up at the end of this function call.

    argparser_parse(&ap);
