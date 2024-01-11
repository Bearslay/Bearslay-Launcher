#include </home/bearslay/Bearslay-Launcher/blokus/Piece.h>

int main() {
    startScreen();

    // Color initialization

    // Default 8 colors
    init_color(0, 0, 0, 0);         // Black
    init_color(1, 999, 999, 999);   // White
    init_color(2, 999, 0, 0);       // Red
    init_color(3, 0, 999, 0);       // Green
    init_color(4, 999, 999, 0);     // Yellow
    init_color(5, 0, 0, 999);       // Blue
    init_color(6, 999, 0, 999);     // Magenta
    init_color(7, 0, 999, 999);     // Cyan

    // Player colors
    init_color(8, 999, 0, 0);       // Player 1
    init_color(9, 129, 482, 859);   // Player 2
    init_color(10, 0, 999, 0);      // Player 3
    init_color(11, 999, 999, 0);    // Player 4


    Blokus::mono_pentominoes = Blokus::readPolyominoFile(MONO_PENTOMINOES);
    Blokus::hexominoes = Blokus::readPolyominoFile(HEXOMINOES);
    Blokus::heptominoes = Blokus::readPolyominoFile(HEPTOMINOES);

    //Blokus::Piece p1(0, Blokus::mono_pentominoes[15]);
    Blokus::Piece p1(0, {{1, 1, 1, 0, 0}, {1, 0, 0, 1, 0}, {0, 1, 0, 1, 1}, {1, 0, 1, 1, 0}, {0, 0, 1, 1, 0}});
    Blokus::Piece p2(0, Blokus::hexominoes[30]);
    Blokus::Piece p3(0, Blokus::heptominoes[107]);

    p1.renderSmall(mainScreen);
    mainScreen.gch();
    mainScreen.reset();
    p1.rotate();
    p1.renderSmall(mainScreen);
    mainScreen.gch();
    mainScreen.reset();
    p1.rotate(false);
    p1.renderSmall(mainScreen);
    mainScreen.gch();
    mainScreen.reset();

    p2.renderSmall(mainScreen);
    mainScreen.gch();
    mainScreen.reset();
    p2.rotate();
    p2.renderSmall(mainScreen);
    mainScreen.gch();
    mainScreen.reset();
    p2.rotate(false);
    p2.renderSmall(mainScreen);
    mainScreen.gch();
    mainScreen.reset();

    p3.renderSmall(mainScreen);
    mainScreen.gch();
    mainScreen.reset();
    p3.rotate();
    p3.renderSmall(mainScreen);
    mainScreen.gch();
    mainScreen.reset();
    p3.rotate(false);
    p3.renderSmall(mainScreen);
    mainScreen.gch();
    mainScreen.reset();

    endwin();
    return 0;
}
