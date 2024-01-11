#include </home/bearslay/Bearslay-Launcher/blokus/Game.h>
#include </home/bearslay/Bearslay-Launcher/helpers/stringConversions.h>

int main() {
    startScreen();
    srand(time(NULL));

    // std::string input1, input2;
    // while (true) {
    //     input1 = mainScreen.gstr(2);
    //     if (input1 == "0") {
    //         break;
    //     }
    //     input2 = mainScreen.gstr(1);
    //     if (input1 == "0") {
    //         break;
    //     } else if (canstoi(input1) && canstoi(input2)) {
    //         Blokus::Game g(stoi(input1), stoi(input2));
    //         g.start();
    //     }
    //     mainScreen.reset();
    // }

    Blokus::Game g;
    g.start();

    endwin();
    return 0;    
}
