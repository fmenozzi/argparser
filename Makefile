CC = gcc -Wall

CC_DEBUG = @$(CC) -g
CC_RELEASE = @$(CC) -O3

all: clean test

test:
	$(CC_DEBUG) test.c argparser.c -o test

clean:
	@rm -rf test
