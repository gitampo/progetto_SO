#include "coccodrillo.h"
#include "bullet.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

static void singleCoccodrillo(CrocData* c, int speed, int pipeWrite){
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    srand(getpid()*time(NULL));

    while(c->alive){
        // invia posizione
        write(pipeWrite, c, sizeof(CrocData));

        // spostamento
        c->x += c->dir*speed;
        // se esce
        if(c->x + c->width < 0 || c->x > maxX){
            c->alive=0;
            break;
        }
        // chance di sparare random
        int r = rand()%50; // 2% di sparare ogni ciclo
        if(r==0){
            // fork bullet
            pid_t pb = fork();
            if(pb==0){
                BulletData bd;
                bd.id = 4; // proiettile
                bd.alive=1;
                bd.dir = c->dir;
                bd.y = c->y; 
                if(c->dir>0) bd.x = c->x + c->width;
                else         bd.x = c->x - 1;
                bullet(&bd, pipeWrite);
            }
        }
        usleep(200000);
    }
    // ultima scrittura (alive=0)
    write(pipeWrite, c, sizeof(CrocData));

    endwin();
    _exit(0);
}

void generatoreCoccodrilli(int flussoY, int dir, int speed, int pipeWrite){
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    srand(getpid()*time(NULL));

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    while(1){
        // dimensioni random
        int w = CROCODILE_MIN_WIDTH + 
                (rand()%(CROCODILE_MAX_WIDTH - CROCODILE_MIN_WIDTH +1));
        CrocData c;
        c.id = 2;
        c.width = w;
        c.height= FROG_HEIGHT; 
        c.dir   = dir;
        c.alive = 1;
        c.y = flussoY;

        if(dir>0) c.x = -w;       // parte da sinistra
        else      c.x = maxX;     // parte da destra

        pid_t pid = fork();
        if(pid==0){
            singleCoccodrillo(&c, speed, pipeWrite);
        }

        // attesa random tra coccodrilli
        int waitSec = 1 + rand()%3; 
        sleep(waitSec);
    }

    endwin();
    _exit(0);
}
