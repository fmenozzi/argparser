CC = gcc -Wall

CC_DEBUG = @$(CC) -g
CC_RELEASE = @$(CC) -O3

SRC = *.c
INC = *.h

all: test

test:
	$(CC_DEBUG) $(INC) $(SRC) -o test
