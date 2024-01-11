#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

void activateColors() {
    init_color(0, 0, 0, 0);
    init_color(1, 999, 999, 999);
    init_color(2, 999, 0, 0);
    init_color(3, 0, 999, 0);
    init_color(4, 999, 999, 0);
    init_color(5, 0, 0, 999);
    init_color(6, 999, 0, 999);
    init_color(7, 0, 999, 999);
    
    init_pair(1, 1, COLOR_BLACK);
    init_pair(2, 2, COLOR_BLACK);
    init_pair(3, 3, COLOR_BLACK);
    init_pair(4, 4, COLOR_BLACK);
    init_pair(5, 5, COLOR_BLACK);
    init_pair(6, 6, COLOR_BLACK);
    init_pair(7, 7, COLOR_BLACK);
}

int main() {
    startScreen();
    activateColors();

    unsigned short color = 1, storeColor = 7, input = 0, posy = LINES / 2, posx = COLS / 2;
    std::string storeChar = "█", brushToggleDisplay = "Off", eraseToggleDisplay = "Off";
    std::string colorNames[7] = {"White", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan"};
    bool brushToggle = false, eraseToggle = false;

    mainScreen.dbox(0, 0, mainScreen.gety() - 1, mainScreen.getx(), {LD_Heavy_Both, LD_Dashed_None});
    mainScreen.wstr(mainScreen.gety() - 1, mainScreen.getx() - 108, "Paint: \'b\' | Toggle Painting: \'B\' | Erase: \'e\' | Toggle Erasing: \'E\' | Clear Canvas: \'c\' | Quit Program: \'q\'");

    while (true) {
        mainScreen.ri();

        // Toggle indicators
        mainScreen.wstr(mainScreen.gety() - 1, 0, "Brush Toggle: " + brushToggleDisplay + " | Eraser Toggle: " + eraseToggleDisplay + " | Current Color: ");
        mainScreen.wstr(mainScreen.gety() - 1, 50 + brushToggleDisplay.length() + eraseToggleDisplay.length(), colorNames[color - 1] + "          ", color);

        // Cursor
        storeChar = mainScreen.sch(posy, posx);
        storeColor = mainScreen.sco(posy, posx);
        if (storeChar == "█") {
            mainScreen.wu(posy, posx, "▒", color);
        }
        else {
            mainScreen.wu(posy, posx, "_", color);
        }

        input = mainScreen.gch();

        mainScreen.wu(posy, posx, storeChar, storeColor);

        // Quit program
        if (input == 'q') {
            break;
        }

        // Change color
        if (input == '1') {
            color = 1;
        }
        else if (input == '2') {
            color = 2;
        }
        else if (input == '3') {
            color = 3;
        }
        else if (input == '4') {
            color = 4;
        }
        else if (input == '5') {
            color = 5;
        }
        else if (input == '6') {
            color = 6;
        }
        else if (input == '7') {
            color = 7;
        }
        
        // Movement keys
        if (input == KEY_DOWN) {
            posy = (posy == mainScreen.gety() - 3) ? 1 : posy + 1;
        }
        else if (input == KEY_UP) {
            posy = (posy == 1) ? mainScreen.gety() - 3 : posy - 1;
        }
        else if (input == KEY_RIGHT) {
            posx = (posx == mainScreen.getx() - 2) ? 1 : posx + 1;
        }
        else if (input == KEY_LEFT) {
            posx = (posx == 1) ? mainScreen.getx() - 2 : posx - 1;
        }

        // Painting
        if (input == 'b' || brushToggle) {
            mainScreen.wu(posy, posx, "█", color);
        }
        if (input == 'B') {
            brushToggle = !brushToggle;
            brushToggleDisplay = (brushToggle) ? "On" : "Off";
            
            eraseToggle = false;
            eraseToggleDisplay = "Off";
        }

        // Erasing
        if (input == 'e' || eraseToggle) {
            mainScreen.wstr(posy, posx, " ");
        }
        if (input == 'E') {
            eraseToggle = !eraseToggle;
            eraseToggleDisplay = (eraseToggle) ? "On" : "Off";

            brushToggle = false;
            brushToggleDisplay = "Off";
        }

        // Clear canvas
        if (input == 'c') {
            mainScreen.reset();
            mainScreen.dbox(0, 0, mainScreen.gety() - 1, mainScreen.getx(), {LD_Heavy_Both, LD_Dashed_None});
            mainScreen.wstr(mainScreen.gety() - 1, mainScreen.getx() - 108, "Paint: \'b\' | Toggle Painting: \'B\' | Erase: \'e\' | Toggle Erasing: \'E\' | Clear Canvas: \'c\' | Quit Program: \'q\'");
        }
    }

    endwin();
    return 0;
}
