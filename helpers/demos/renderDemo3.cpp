#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

int main() {
    startScreen();

    mainScreen.wa(1, 2, {{{0, 2, 1}, {{}, "random ass string with color "}}, {{1, 7, 0, 1}, {{42, 3}, ""}}, {{2, 3, 0, 0, 1}, {{}, "♔"}}, {{2, 3, 0, 0, 1}, {{}, "♕"}}, {{2, 3, 0, 0, 1}, {{}, "♖"}}, {{2, 3, 0, 0, 1}, {{}, "♗"}}, {{2, 3, 0, 0, 1}, {{}, "♘"}}, {{2, 3, 0, 0, 1}, {{}, "♙"}}});
    mainScreen.gch();

    endwin();
    return 0;
}
