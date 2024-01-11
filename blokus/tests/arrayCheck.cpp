#include </home/bearslay/Bearslay-Launcher/blokus/Polyominoes/Polyominoes.h>

int main() {
    startScreen();

    Blokus::mono_pentominoes = Blokus::readPolyominoFile("/home/bearslay/Bearslay-Launcher/blokus/Polyominoes/mono_pentominoes.txt");

    Blokus::polyominoesArray(mainScreen, Blokus::mono_pentominoes, 7, 5);
    mainScreen.gch();
    mainScreen.reset();

    std::vector<std::vector<std::vector<bool>>> newPolyomino = Blokus::mono_pentominoes;
    for (unsigned char i = 0; i < 3; i++) {
        newPolyomino = Blokus::polyominoesEnlarge(newPolyomino);
        Blokus::polyominoesArray(mainScreen, newPolyomino, 7, 6 + i);
        mainScreen.gch();
        mainScreen.reset();
    }

    endwin();
    return 0;
}
