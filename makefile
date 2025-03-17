CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: frogger

frogger: main.o frog.o croc.o graphics.o
	$(CC) $(CFLAGS) -o frogger main.o frog.o croc.o graphics.o -lncurses

main.o: main.c frog.h croc.h graphics.h
	$(CC) $(CFLAGS) -c main.c

frog.o: frog.c frog.h graphics.h
	$(CC) $(CFLAGS) -c frog.c

croc.o: croc.c croc.h graphics.h
	$(CC) $(CFLAGS) -c croc.c

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) -c graphics.c

clean:
	rm -f *.o frogger
