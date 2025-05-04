# Makefile per il progetto Frogger

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
LDFLAGS = -I/opt/homebrew/opt/ncurses/include -L/opt/homebrew/opt/ncurses/lib -lncurses

# Elenco dei sorgenti: usa collision.c al posto di collisioni.c
SOURCES = main.c bullet.c collision.c croc.c frog.c graphics.c
OBJECTS = $(SOURCES:.c=.o)

# Nome dell'eseguibile
TARGET = frogger

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
