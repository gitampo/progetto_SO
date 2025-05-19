# Makefile per il progetto Frogger

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2 -g -pthread
LDFLAGS = -lncursesw -pthread -L/opt/homebrew/opt/ncurses/lib

# Elenco dei sorgenti: usa collision.c al posto di collisioni.c
SOURCES = main.c bullet.c collision.c croc.c frog.c graphics.c buffer.c 
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
