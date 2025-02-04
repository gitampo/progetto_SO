#ifndef CONFIG_H
#define CONFIG_H

// Dimensioni minime finestra
#define MIN_COLS 80
#define MIN_LINES 24

// Numero di vite iniziali
#define FROG_LIVES 3

// Numero di tane
#define NUM_TANE 5

// Numero di flussi (corsie d’acqua)
#define NUM_FLUSSI 8

// Tempo massimo per ogni manche (in secondi)
#define ROUND_TIME 30

// Dimensioni rana
#define FROG_WIDTH  5
#define FROG_HEIGHT 3

// Dimensioni coccodrilli (altezza = FROG_HEIGHT, larghezza min=2*FROG_WIDTH)
#define CROCODILE_MIN_WIDTH (2*FROG_WIDTH)
#define CROCODILE_MAX_WIDTH (3*FROG_WIDTH)

// Simboli
#define SYMBOL_FROG_1 " @..@ "
#define SYMBOL_FROG_2 "(>__<)"
#define SYMBOL_FROG_3 " ^^ ^^"

// Granate
#define SYMBOL_GRENADE '*'
#define GRENADE_SPEED 1

// Proiettili coccodrillo
#define SYMBOL_BULLET '>'
#define BULLET_SPEED 1

#endif