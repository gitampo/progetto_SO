progetto_SO-1: main.o frog.o croc.o graphics.o
	gcc -o progetto_SO-1 main.o frog.o croc.o graphics.o -lncurses 

main.o: main.c
	gcc -c main.c

frog.o: frog.c frog.h
	gcc -c frog.c

croc.o: croc.c croc.h
	gcc -c croc.c

graphics.o: graphics.c graphics.h
	gcc -c graphics.c

clean:
	rm -f *.o
	rm -f progetto_SO-1.o 