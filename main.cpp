// Standard Library
#include <ncurses.h>
#include <ctime>
#include <cmath>
#include <locale.h>

// In order for different applications to run, there needs to be enough available characters in the terminal
// Return true if the terminal can fit the application, false if it is too small
bool checkDims(int dimY = 25, int dimX = 50) {    
    if (LINES - 1 < dimY || COLS - 1 < dimX) {
        mvprintw(LINES / 2 - 3, 0, "[Error] The dimensions of your terminal are too small");
        mvprintw(LINES / 2 - 2, 0, " - Try decreasing the text size\n - Try increasing the window size of the terminal");
        mvprintw(LINES / 2 + 5, 0, "Press any key to quit...\n");

        getch();
        return false;
    }

    return true;
}

int main() {
    setlocale(LC_ALL, "");

    initscr();
    noecho();
    curs_set(0);

    srand(time(NULL));

    // If colors can't be used by the terminal, then a lot of applications become much harder to use
    if (!has_colors()) {
        mvprintw(LINES / 2, 0, "[Error] Your terminal does not support colors");
        mvprintw(LINES / 2 + 1, 0, " - Try using a different terminal");
        mvprintw(LINES / 2 + 5, 0, "Press any key to quit...\n");

        getch();
        return 0;
    }
    if (!can_change_color()) {
        mvprintw(LINES / 2 + 2, 0, "[Error] Your terminal does not support changing colors");
        mvprintw(LINES / 2 + 3, 0, " - Try using a different terminal or something");
        mvprintw(LINES / 2 + 5, 0, "Press any key to quit...\n");

        getch();
        return 0;
    }

    start_color();

    // End Terminal Validation

    // The real action begins
    

    endwin();
    return 0;
}
