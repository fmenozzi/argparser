CC = gcc -Wall

CC_DEBUG = @$(CC) -g
CC_RELEASE = @$(CC) -O3

SRC = *.c
INC = *.h

all: example 

exmaple:
	$(CC_DEBUG) $(INC) $(SRC) -o example 

clean:
	@rm -rf test
