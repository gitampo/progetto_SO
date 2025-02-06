#include "bullet.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

void bullet(BulletData* bd, int pipeWrite){
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    while(bd->alive){
        write(pipeWrite, bd, sizeof(BulletData));
        bd->x += bd->dir * BULLET_SPEED;
        if(bd->x<0 || bd->x>maxX){
            bd->alive=0;
            break;
        }
        usleep(100000);
    }
    write(pipeWrite, bd, sizeof(BulletData));
    endwin();
    _exit(0);
}
