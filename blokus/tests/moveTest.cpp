#include </home/bearslay/Bearslay-Launcher/blokus/Piece.h>
#include </home/bearslay/Bearslay-Launcher/blokus/Extraneous.h>

int main() {
    startScreen();

    Blokus::heptominoes = Blokus::readPolyominoFile(HEPTOMINOES);
    Blokus::Piece p(0, Blokus::heptominoes[rand() % 108]);

    BSCREEN bg = BSCREEN(22, 42, LINES / 2 - 11, COLS / 2 - 21);
    BSCREEN win = BSCREEN(20, 40, bg.getpy() + 1, bg.getpx() + 1);
    bg.dbox();
    bg.ri();

    p.place(win);

    endwin();
    return 0;
}
