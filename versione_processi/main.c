#include "game.c"

int main() {
   
    setlocale(LC_ALL, "");

    srand(time(NULL)); 
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    signal(SIGCHLD, SIG_IGN); // Ignora i segnali SIGCHLD per evitare zombie
    
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    init_pair(3, COLOR_RED, COLOR_YELLOW);
    init_pair(4, COLOR_GREEN, COLOR_BLUE);
    if (can_change_color()) {
        init_color(COLOR_YELLOW, 500, 250, 0);
    }
    init_pair(5, COLOR_GREEN, COLOR_GREEN);
    init_pair(6, COLOR_BLACK, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
    init_pair(8, COLOR_RED, COLOR_BLACK);
    init_pair(9, COLOR_WHITE, COLOR_WHITE);

    nodelay(stdscr, TRUE);

    // Creazione delle pipe

    bool play_again = true;

    while (play_again) {
        runGameLoop();
        play_again = getchar() == 's';
    }

    endwin();
    
    return 0;
}

