progetto.out: main.o frog.o croc.o grenade.o gamelogic.o graphics.o scoreboard.o
	gcc main.o frog.o croc.o grenade.o gamelogic.o graphics.o scoreboard.o -lncurses -o progetto.out

main.o: main.c
	gcc -c main.c

frog.o: frog.c frog.h
	gcc -c frog.c

croc.o: croc.c croc.h
	gcc -c croc.c

grenade.o: grenade.c grenade.h
	gcc -c grenade.c

gamelogic.o: gamelogic.c gamelogic.h
	gcc -c gamelogic.c

graphics.o: graphics.c graphics.h
	gcc -c graphics.c

scoreboard.o: scoreboard.c scoreboard.h
	gcc -c scoreboard.c

clean:
	rm -f *.o
	rm -f *.out
