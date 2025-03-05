#include "scoreboard.h"
#include <ncurses.h>
#include "config.h"

// Inizializza scoreboard
void initScoreBoard(ScoreBoard* sb, int vite, int timeEachRound){
    sb->vite      = vite;
    sb->punteggio = 0;
    sb->timeLeft  = timeEachRound;
}

// Disegna scoreboard in alto (riga 0)
void drawScoreBoard(const ScoreBoard* sb){
    move(0, 0);
    printw("Vite: %d | Tempo: %d | Score: %d",
           sb->vite, sb->timeLeft, sb->punteggio);
    refresh();
}

// Aggiunge punteggio
void addScore(ScoreBoard* sb, int amount){
    sb->punteggio += amount;
    if (sb->punteggio < 0) {
        sb->punteggio = 0;
    }
}

// Decrementa vite
void decrementLife(ScoreBoard* sb){
    sb->vite--;
}
