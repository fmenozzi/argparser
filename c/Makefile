GCC = gcc -Wall -pedantic
GPP = g++ -Wall -pedantic
CLANG = clang -Wall -pedantic
CLANGPP = clang++ -Wall -pedantic

GCC_DEBUG = @$(GCC) -g
GCC_RELEASE = @$(GCC) -O3
GPP_DEBUG = @$(GPP) -g
GPP_RELEASE = @$(GPP) -O3
CLANG_DEBUG = @$(CLANG) -g
CLANG_RELEASE = @$(CLANG) -O3
CLANGPP_DEBUG = @$(CLANGPP) -g
CLANGPP_RELEASE = @$(CLANGPP) -O3

CMD = test.c argparser.c -o test && ./test -pf --help -v --integer 2 --double=2.2 -s="Hello"

all: test-gcc clean

test-all: test-gcc test-gpp test-clang test-clangpp clean

test-gcc:
	@echo "Testing with debug gcc..."
	@$(GCC_DEBUG) $(CMD)
	@echo "Testing with release gcc..."
	@$(GCC_RELEASE) $(CMD)

test-gpp:
	@echo "Testing with g++..."
	@$(GPP_DEBUG) $(CMD)
	@echo "Testing with release g++..."
	@$(GPP_RELEASE) $(CMD)

test-clang:
	@echo "Testing with clang..."
	@$(CLANG_DEBUG) $(CMD)
	@echo "Testing with release clang..."
	@$(CLANG_RELEASE) $(CMD)

test-clangpp:
	@echo "Testing with clang++..."
	@$(CLANGPP_DEBUG) $(CMD)
	@echo "Testing with release clang++..."
	@$(CLANGPP_RELEASE) $(CMD)

clean:
	@rm -rf test
