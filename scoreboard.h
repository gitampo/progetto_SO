#ifndef SCOREBOARD_H
#define SCOREBOARD_H

typedef struct {
    int vite;       
    int punteggio;
    int timeLeft;  
} ScoreBoard;

void initScoreBoard(ScoreBoard* sb, int vite, int timeEachRound);
void drawScoreBoard(const ScoreBoard* sb);
void addScore(ScoreBoard* sb, int amount);
void decrementLife(ScoreBoard* sb);

#endif // SCOREBOARD_H
