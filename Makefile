GCC=gcc 
FLAGS=-std=c99 -Wall 
SRC=ping.c
DST=ping
all:
	$(GCC)$(FLAGS)$(SRC) -ledit  -lm -o $(DST) 


clean:
	rm -rf $(DST) a.out
