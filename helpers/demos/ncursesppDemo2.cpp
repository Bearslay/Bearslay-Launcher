#include </home/bearslay/Bearslay-Launcher/helpers/ncursespp.hpp>

int main() {
    npp::init();

    npp::main.wsp(npp::main.wip(npp::main.wsp(1, 2, L"random ass string with color", 2, true, false, false, {0, 2}), 42, 3, 7, false, true), L"♔♕♖♗♘♙", 3, false, false, true);
    npp::main.gch();

    endwin();
    return 0;
}
