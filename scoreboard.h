#ifndef SCOREBOARD_H
#define SCOREBOARD_H

// Struttura di esempio per scoreboard
typedef struct {
    int vite;        // numero di vite
    int punteggio;   // punteggio accumulato
    int timeLeft;    // tempo rimanente
} ScoreBoard;

/**
 * Inizializza la scoreboard con vite, punteggio = 0, e tempo = timeEachRound.
 */
void initScoreBoard(ScoreBoard* sb, int vite, int timeEachRound);

/**
 * Disegna la scoreboard a schermo (vite, tempo, score).
 */
void drawScoreBoard(const ScoreBoard* sb);

/**
 * Aggiunge (amount) punti alla scoreboard.
 * Se il punteggio va sotto zero, lo riporta a 0.
 */
void addScore(ScoreBoard* sb, int amount);

/**
 * Decrementa di 1 il numero di vite.
 */
void decrementLife(ScoreBoard* sb);

#endif // SCOREBOARD_H
