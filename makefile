progetto.out: main.o frog.o croc.o graphics.o
	gcc main.o frog.o croc.o graphics.o-lncurses -o progetto.out

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
	rm -f *.out
