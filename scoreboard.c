#include "scoreboard.h"
#include <ncurses.h>
#include "config.h"  // se ti serve per costanti, altrimenti puoi rimuovere

// Inizializza la scoreboard (vite, punteggio=0, tempo=timeEachRound)
void initScoreBoard(ScoreBoard* sb, int vite, int timeEachRound){
    sb->vite      = vite;
    sb->punteggio = 0;
    sb->timeLeft  = timeEachRound;
}

// Disegna la scoreboard in alto (riga 0, colonna 0)
void drawScoreBoard(const ScoreBoard* sb){
    move(0, 0);
    printw("Vite: %d | Tempo: %d | Score: %d",
           sb->vite, sb->timeLeft, sb->punteggio);
    refresh();
}

// Aggiunge punteggio (se scende sotto zero, rimane a 0)
void addScore(ScoreBoard* sb, int amount){
    sb->punteggio += amount;
    if(sb->punteggio < 0) {
        sb->punteggio = 0;
    }
}

// Decrementa di 1 il numero di vite
void decrementLife(ScoreBoard* sb){
    sb->vite--;
}
