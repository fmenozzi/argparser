Simple command line parser for C/C++ programs. Use `make example` to run example code.

The API is quite simple, with only four functions: `argparser_create`, `argparser_add`, `argparser_parse`, and `argparser_destroy`

### argparser_create
Takes system arguments (i.e. `argc` and `argv`) and returns an `argparser` object

    argparser ap = argparser_create(argc, argv);
    
### argparser_destroy
Destroys an `argparser` object

    argparser_destroy(&ap);
    
### argparser_add
Adds an argument to the parser. There are six parameters to this function:
+ A pointer to an `argparser` object
+ The shortarg string (e.g. `-h`)
+ The longarg string (e.g. `--help`)
+ An enum describing the type of argument. One of `ARGTYPE_INT`, `ARGTYPE_DOUBLE`, `ARGTYPE_STRING`, or `ARGTYPE_VOID`
+ A pointer containing the address of the variable you wish to populate. `NULL` can safely be passed if you're using `ARGTYPE_VOID`
+ A callback function pointer. `NULL` can safely be passed if you don't need a callback.

For example, the following line of code

    argparser_add(&ap, "-h", "--help", ARGTYPE_VOID, NULL, help_callback);
    
will register the arguments `-h` and `--help` and call the function `help_callback` once `argparse_parse` is called. Check out `example.c` for more.

### argparser_parse
Begins parsing the command line args, populating values and calling callbacks

    argparser_parse(&ap);
