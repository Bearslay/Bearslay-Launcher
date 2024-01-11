#include </home/bearslay/Bearslay-Launcher/blokus/Polyominoes/Polyominoes.h>

int main() {
    startScreen();

    Blokus::mono_pentominoes = Blokus::readPolyominoFile("/home/bearslay/Bearslay-Launcher/blokus/Polyominoes/mono_pentominoes.txt");
    Blokus::hexominoes = Blokus::readPolyominoFile("/home/bearslay/Bearslay-Launcher/blokus/Polyominoes/hexominoes.txt");
    Blokus::heptominoes = Blokus::readPolyominoFile("/home/bearslay/Bearslay-Launcher/blokus/Polyominoes/heptominoes.txt");

    Blokus::polyominoesSmall(mainScreen, Blokus::polyominoesEnlarge(Blokus::mono_pentominoes, 2), 7, 7);
    mainScreen.gch();
    mainScreen.reset();
    Blokus::polyominoesSmall(mainScreen, Blokus::polyominoesEnlarge(Blokus::hexominoes), 7, 7);
    mainScreen.gch();
    mainScreen.reset();
    Blokus::polyominoesSmall(mainScreen, Blokus::heptominoes, 12, 7);
    mainScreen.gch();
    mainScreen.reset();

    endwin();
    return 0;
}
