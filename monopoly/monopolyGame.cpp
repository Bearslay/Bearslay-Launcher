#ifndef MONOPOLY_GAME
#define MONOPOLY_GAME

// Helpers
#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

// Monopoly
#include </home/bearslay/Bearslay-Launcher/monopoly/monopolyProperty.h>
#include </home/bearslay/Bearslay-Launcher/monopoly/monopolyPlayer.h>

class monopolyGame {
    private:
        monopolyProperty Properties[22];
        monopolyPlayer Players[8];
        BSCREEN GameScreen;

        void activateColors() {
            // Defaults
            init_color(0, 0, 0, 0);
            init_color(1, 999, 0, 0);
            init_color(2, 0, 999, 0);
            init_color(3, 999, 999, 0);
            init_color(4, 0, 0, 999);
            init_color(5, 999, 0, 999);
            init_color(6, 0, 999, 999);
            init_color(7, 999, 999, 999);

            init_pair(COLOR_RED, 1, COLOR_BLACK);
            init_pair(COLOR_GREEN, 2, COLOR_BLACK);
            init_pair(COLOR_YELLOW, 3, COLOR_BLACK);
            init_pair(COLOR_BLUE, 4, COLOR_BLACK);
            init_pair(COLOR_MAGENTA, 5, COLOR_BLACK);
            init_pair(COLOR_CYAN, 6, COLOR_BLACK);
            init_pair(COLOR_WHITE, 7, COLOR_BLACK);

            // Player colors
            init_color(8, 0, 0, 0);
            init_color(9, 0, 0, 0);
            init_color(10, 0, 0, 0);
            init_color(11, 0, 0, 0);
            init_color(12, 0, 0, 0);
            init_color(13, 0, 0, 0);
            init_color(14, 0, 0, 0);
            init_color(15, 0, 0, 0);

            init_pair(8, 8, COLOR_BLACK);
            init_pair(9, 9, COLOR_BLACK);
            init_pair(10, 10, COLOR_BLACK);
            init_pair(11, 11, COLOR_BLACK);
            init_pair(12, 12, COLOR_BLACK);
            init_pair(13, 13, COLOR_BLACK);
            init_pair(14, 14, COLOR_BLACK);
            init_pair(15, 15, COLOR_BLACK);

            // Color sets (properties)
            init_color(16, 396, 0, 612);
            init_color(17, 608, 729, 780);
            init_color(18, 639, 141, 541);
            init_color(19, 988, 494, 0);
            init_color(20, 921, 47, 47);
            init_color(21, 949, 855, 0);
            init_color(22, 63, 612, 0);
            init_color(23, 0, 55, 851);

            init_pair(16, 16, COLOR_BLACK);
            init_pair(17, 17, COLOR_BLACK);
            init_pair(18, 18, COLOR_BLACK);
            init_pair(19, 19, COLOR_BLACK);
            init_pair(20, 20, COLOR_BLACK);
            init_pair(21, 21, COLOR_BLACK);
            init_pair(22, 22, COLOR_BLACK);
            init_pair(23, 23, COLOR_BLACK);
        }

        void renderBoard() {
            GameScreen.dbox(0, 0, 70, 196);
            GameScreen.dbox(1, 2, 68, 34);
            GameScreen.dbox(1, 37, 68, 157, {LD_Heavy_Both, LD_Dashed_None});
        }

    public:
        monopolyGame() {}

        // true = stay in program, false = leave program
        bool start(int y, int x) {
            activateColors();
            GameScreen = BSCREEN(70, 196, y, x);

            renderBoard();
            GameScreen.gch();
        }
};

int main() {
    startScreen();
    monopolyGame game;
    game.start(LINES / 2 - 35, COLS / 2 - 98);
    endwin();
    return 0;
}

#endif
