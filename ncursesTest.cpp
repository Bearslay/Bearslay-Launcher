#include <ncurses.h>
#include <iostream>

int main() {
    initscr();
    noecho();

    std::string test = "BlItReBo";
    
    for (unsigned int i = 0; i < test.length(); i++) {
        test[i] = tolower(test[i]);
    }

    if (test.find("bo") != std::string::npos) {
        printw("found");
    }

    getch();

    endwin();
    return 0;
}
