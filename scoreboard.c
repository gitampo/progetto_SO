#include "scoreboard.h"
#include <ncurses.h>
#include "config.h"

void initScoreBoard(ScoreBoard* sb, int vite, int timeEachRound){
    sb->vite = vite;
    sb->punteggio=0;
    sb->timeLeft=timeEachRound;
}

void drawScoreBoard(const ScoreBoard* sb){
    move(0,0);
    printw("Vite: %d | Tempo: %d | Score: %d", sb->vite, sb->timeLeft, sb->punteggio);
    refresh();
}

void addScore(ScoreBoard* sb, int amount){
    sb->punteggio += amount;
    if(sb->punteggio<0) sb->punteggio=0;
}

void decrementLife(ScoreBoard* sb){
    sb->vite--;
}
