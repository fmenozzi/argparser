CC = gcc -Wall

CC_DEBUG = @$(CC) -g
CC_RELEASE = @$(CC) -O3

all: clean example

example:
	$(CC_DEBUG) example.c argparser.c -o example

clean:
	@rm -rf example
