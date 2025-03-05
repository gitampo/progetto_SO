CC = gcc
CFLAGS = -Wall -Wextra
OBJS = main.o frog.o croc.o gameLogic.o graphics.o scoreboard.o

frogger: $(OBJS)
	$(CC) $(OBJS) -o frogger -lncurses

main.o: main.c config.h graphics.h gameLogic.h frog.h croc.h scoreboard.h
	$(CC) -c main.c $(CFLAGS)

frog.o: frog.c frog.h
	$(CC) -c frog.c $(CFLAGS)

croc.o: croc.c croc.h
	$(CC) -c croc.c $(CFLAGS)

gameLogic.o: gameLogic.c gameLogic.h
	$(CC) -c gameLogic.c $(CFLAGS)

graphics.o: graphics.c graphics.h
	$(CC) -c graphics.c $(CFLAGS)

scoreboard.o: scoreboard.c scoreboard.h
	$(CC) -c scoreboard.c $(CFLAGS)

clean:
	rm -f *.o frogger
