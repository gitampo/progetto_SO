CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: frogger

frogger: main.o frog.o croc.o collision.o graphics.o
	$(CC) $(CFLAGS) -o frogger main.o frog.o croc.o collision.o graphics.o -lncurses

main.o: main.c frog.h croc.h graphics.h collision.h entity.h
	$(CC) $(CFLAGS) -c main.c

frog.o: frog.c frog.h graphics.h collision.h entity.h
	$(CC) $(CFLAGS) -c frog.c

croc.o: croc.c croc.h graphics.h collision.h entity.h
	$(CC) $(CFLAGS) -c croc.c

collision.o: collision.c collision.h
	$(CC) $(CFLAGS) -c collision.c

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) -c graphics.c

clean:
	rm -f *.o frogger
