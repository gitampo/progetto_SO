#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include <fcntl.h> // Per O_NONBLOCK
#include "graphics.h"
#include "frog.h"
#include "croc.h"
#include "entity.h"  
#include "collision.h"
#include "bullet.h"

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    init_pair(3, COLOR_RED, COLOR_YELLOW);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    if (can_change_color()) {
        init_color(COLOR_YELLOW, 500, 250, 0);
    }
    init_pair(5, COLOR_GREEN, COLOR_GREEN);
    init_pair(6, COLOR_BLACK, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    nodelay(stdscr, TRUE);

    int fileds[2];
    if (pipe(fileds) == -1) {
        perror("Errore creazione pipe");
        endwin();
        exit(EXIT_FAILURE);
    }
    int toFrog[2];
    if (pipe(toFrog) == -1) {
        perror("Errore creazione pipe");
        endwin();
        exit(EXIT_FAILURE);
    }
    int flags = fcntl(fileds[0], F_GETFL, 0);
    fcntl(fileds[0], F_SETFL, flags | O_NONBLOCK);
    fcntl(toFrog[0], F_SETFL, flags | O_NONBLOCK);

    int taneOccupate[NUM_TANE] = {0};
    Entity frog;
    frog.attached = 0;
    frog.attached_crocodile_id = -1;
    frog.type = OBJECT_FROG;
    frog.y = LINES - FROG_HEIGHT; 
    frog.x = (COLS - FROG_WIDTH) / 2;

    int totalCrocs = LANES * CROCS_PER_LANE;
    Entity crocs[totalCrocs];
    int startCol = (COLS - PAVEMENT_WIDTH) / 2;
    int endCol = startCol + PAVEMENT_WIDTH;
    int riverStartRow = LINES - 27;
    int totalGrenades = 20;

    pid_t pid_frog = fork();
    if (pid_frog == -1) {
        perror("Errore fork rana");
        endwin();
        exit(EXIT_FAILURE);
    } else if (pid_frog == 0) {
        close(fileds[0]);
        frogProcess(&frog, fileds, toFrog);
        exit(EXIT_SUCCESS);
    }

    creaCrocodiles(crocs, startCol, endCol, riverStartRow);
    pid_t pid_crocs[totalCrocs];
    for (int i = 0; i < totalCrocs; i++) {
        pid_crocs[i] = fork();
        if (pid_crocs[i] == -1) {
            perror("Errore fork coccodrillo");
            endwin();
            exit(EXIT_FAILURE);
        } else if (pid_crocs[i] == 0) {
            close(fileds[0]);
            crocProcess(&crocs[i], fileds[1]);
            exit(EXIT_SUCCESS);
        }
    }

    pid_t pid_grenades[totalGrenades];
    int i_grenades = 0;
    Entity grenades[totalGrenades];
    for (int i = 0; i < totalGrenades; i++) {
        grenades[i].type = OBJECT_GRENADE;
        grenades[i].inGioco = 0;
    }

    Entity bullets[MAX_BULLETS];
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].inGioco = 0;
    }

    close(toFrog[0]);
    close(fileds[1]);

    Entity temp;
    while (1) {
        if (read(fileds[0], &temp, sizeof(Entity)) > 0) {
            if (temp.type == OBJECT_FROG) {
                frog.y = temp.y;
                frog.x = temp.x;
            } else if (temp.type == OBJECT_CROCODILE) {
                if (!temp.inGioco) {
                    if (frog.attached && frog.attached_crocodile_id == temp.id) {
                        frog.x = temp.x + (CROC_WIDTH / 2) - (FROG_WIDTH / 2);
                        frog.y = temp.y - 1;
                    }
                    kill(pid_crocs[temp.id], SIGKILL);
                    pid_crocs[temp.id] = fork();
                    if (pid_crocs[temp.id] == -1) {
                        perror("Errore fork coccodrillo");
                        endwin();
                        exit(EXIT_FAILURE);
                    } else if (pid_crocs[temp.id] == 0) {
                        close(fileds[0]);
                        crocs[temp.id].inGioco = 1;
                        crocs[temp.id].x = crocs[temp.id].initX;
                        crocProcess(&crocs[temp.id], fileds[1]);
                        exit(EXIT_SUCCESS);
                    }
                } else {
                    crocs[temp.id].y = temp.y;
                    crocs[temp.id].x = temp.x;
                }
            } else if (temp.type == CREATE_GRENADE) {
                for (int i = 0; i < 2; i++) {
                    pid_grenades[i_grenades] = fork();
                    if (pid_grenades[i_grenades] == -1) {
                        perror("Errore fork granata");
                        endwin();
                        exit(EXIT_FAILURE);
                    } else if (pid_grenades[i_grenades] == 0) {
                        close(fileds[0]);
                        grenades[i_grenades].type = OBJECT_GRENADE;
                        grenades[i_grenades].id = i_grenades;
                        grenades[i_grenades].inGioco = 1;
                        grenades[i_grenades].x = temp.x + (i * 2 - 1) * (FROG_WIDTH / 2);
                        grenades[i_grenades].y = temp.y;
                        grenades[i_grenades].direction = i * 2 - 1;
                        grenades[i_grenades].speed = 1;
                        grenadeProcess(&grenades[i_grenades], fileds);
                        exit(EXIT_SUCCESS);
                    }
                    i_grenades = (i_grenades + 1) % totalGrenades;
                }
            } else if (temp.type == OBJECT_GRENADE) {
                grenades[temp.id].y = temp.y;
                grenades[temp.id].x = temp.x;
                if (temp.x < startCol || temp.x > endCol) {
                    kill(pid_grenades[temp.id], SIGKILL);
                    grenades[temp.id].inGioco = 0;
                }
            } else if (temp.type == OBJECT_BULLET) {
                int found = -1;
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (bullets[i].id == temp.id) {
                        found = i;
                        break;
                    }
                }
                if (found != -1) {
                    bullets[found] = temp;
                } else {
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (bullets[i].inGioco == 0) {
                            bullets[i] = temp;
                            break;
                        }
                    }
                }
            }
        }

        // 🔥 AGGIUNTA CRUCIALE: TRASPORTO DELLA RANA
        if (frog.attached) {
            frog.x = crocs[frog.attached_crocodile_id].x + (CROC_WIDTH / 2) - (FROG_WIDTH / 2);
            frog.y = crocs[frog.attached_crocodile_id].y - 1;
        }

        int tanaIndex = isFrogInTana(&frog);
        if (tanaIndex != -1) {
            taneOccupate[tanaIndex] = 1;
            frog.x = (COLS - FROG_WIDTH) / 2;
            frog.y = LINES - FROG_HEIGHT;
            write(toFrog[1], &frog, sizeof(Entity));
        }

        drawRiver();
        drawPavement();
        drawMiddlePavement();

        attron(COLOR_PAIR(4));
        for (int i = 0; i < totalCrocs; i++) {
            if (!crocs[i].inGioco) continue;
            for (int row = 0; row < CROC_HEIGHT; row++) {
                for (int col = 0; col < CROC_WIDTH; col++) {
                    if (crocs[i].x + col >= startCol &&
                        crocs[i].x + col < endCol)
                        mvprintw(crocs[i].y + row, crocs[i].x + col, "%c", SPRITE_CROC);
                }
            }
        }
        attroff(COLOR_PAIR(4));

        // Collisione rana sopra coccodrillo
        int frog_on_croc = 0;
        for (int i = 0; i < totalCrocs; i++) {
            if (!crocs[i].inGioco) continue;
            if ((frog.y + FROG_HEIGHT - 1) == crocs[i].y &&
                frog.x + FROG_WIDTH/2 >= crocs[i].x &&
                frog.x + FROG_WIDTH/2 < crocs[i].x + CROC_WIDTH) {
                frog.attached = 1;
                frog.attached_crocodile_id = crocs[i].id;
                frog_on_croc = 1;
                break;
            }
        }
        if (!frog_on_croc) {
            frog.attached = 0;
            frog.attached_crocodile_id = -1;
        }
        

        drawVoid();
        drawFrog(&frog);
        drawTane(taneOccupate);

        for (int i = 0; i < totalGrenades; i++) {
            if (grenades[i].inGioco) {
                drawGrenade(grenades[i].y, grenades[i].x);
            }
        }

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].inGioco) {
                drawBullet(&bullets[i]);
            }
        }
        refresh();
    }

    endwin();
    kill(pid_frog, SIGKILL);
    for (int i = 0; i < totalCrocs; i++) {
        kill(pid_crocs[i], SIGKILL);
    }
    wait(NULL);
    wait(NULL);
    return 0;
}
