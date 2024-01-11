#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

// https://en.wikipedia.org/wiki/List_of_Unicode_characters#Box_Drawing
std::string unicode_BoxDrawing[8][16] = {
    {
        "─", "━", "│", "┃", "┄", "┅", "┆", "┇", "┈", "┉", "┊", "┋", "┌", "┍", "┎", "┏"
    },
    {
        "┐", "┑", "┒", "┓", "└", "┕", "┖", "┗", "┘", "┙", "┚", "┛", "├", "┝", "┞", "┟"
    },
    {
        "┠", "┡", "┢", "┣", "┤", "┥", "┦", "┧", "┨", "┩", "┪", "┫", "┬", "┭", "┮", "┯"
    },
    {
        "┰", "┱", "┲", "┳", "┴", "┵", "┶", "┷", "┸", "┹", "┺", "┻", "┼", "┽", "┾", "┿"
    },
    {
        "╀", "╁", "╂", "╃", "╄", "╅", "╆", "╇", "╈", "╉", "╊", "╋", "╌", "╍", "╎", "╏"
    },
    {
        "═", "║", "╒", "╓", "╔", "╕", "╖", "╗", "╘", "╙", "╚", "╛", "╜", "╝", "╞", "╟"
    },
    {
        "╠", "╡", "╢", "╣", "╤", "╥", "╦", "╧", "╨", "╩", "╪", "╫", "╬", "╭", "╮", "╯"
    },
    {
        "╰", "╱", "╲", "╳", "╴", "╵", "╶", "╷", "╸", "╹", "╺", "╻", "╼", "╽", "╾", "╿"
    }
};

int main() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);
    start_color();

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

    BSCREEN screen;

    // A few random lines
    screen.dline(20, 50, 10, false, {LD_Light_Soft, LD_Dashed_Double});
    screen.wu(21, 49, "╌");
    screen.wu(22, 49, "┉");
    screen.dline(20, 50, 10, true,  {LD_Doubled_Vertical, LD_Dashed_Double});
    screen.dline(25, 45, 10, false, {LD_Doubled_Vertical, LD_Doubled_Vertical});
    screen.dline(40, 50, 10, false, {LD_Light_Hard, LD_Dashed_Double});

    screen.dbox(20, 70, 10, 20, {LD_Doubled_Both, LD_Dashed_None}, COLOR_RED);
    screen.dbox(30, 80, 2, 2);
    screen.dbox(0, 0, screen.gety(), screen.getx());

    screen.dline(24, 65, 30, false, {LD_Light_Hard, LD_Dashed_Single_2});
    screen.dline(25, 65, 30, false, {LD_Light_Hard, LD_Dashed_Single_2});
    screen.dline(26, 65, 30, false, {LD_Light_Hard, LD_Dashed_Single_2});
    screen.dline(15, 75, 20, true,  {LD_Doubled_Vertical, LD_Dashed_None});
    screen.dline(15, 77, 20, true,  {LD_Heavy_Both, LD_Dashed_Quadruple});
    screen.dline(15, 79, 20, true,  {LD_Heavy_Vertical, LD_Doubled_Vertical});
    screen.dline(24, 65, 3, true,   {LD_Heavy_Both, LD_Dashed_None});
    screen.dline(28, 65, 30, false, {LD_Doubled_Horizontal, LD_Doubled_Vertical});
    screen.dline(22, 65, 30, false, {LD_Doubled_Horizontal, LD_Dashed_None}, COLOR_MAGENTA);

    screen.ri();
    screen.gch();
    screen.reset();

    for (int i = 0; i < screen.getx(); i++) {
        screen.dline(0, i, screen.gety(), true, {LD_Heavy_Horizontal, LD_Dashed_None});
    }

    screen.gch();
    screen.clear();

    for (int i = 0; i < screen.gety(); i++) {
        screen.dline(i, 0, screen.getx(), false, {LD_Light_Hard, LD_Dashed_None});
    }

    screen.gch();
    screen.reset();

    // Test the string aquisition function
    screen.wstr(5, 0, "\"" + screen.gstr(10) + "\"");
    screen.gch();
    screen.reset();
    
    // test a smaller screen
    BSCREEN test(20, 40, LINES / 2 - 10, COLS / 2 - 20);

    for (int i = 0; i < test.gety(); i++) {
        for (int j = 0; j < test.getx(); j++) {
            test.wstr(i, j, "a", rand() % 8);
        }
    }
    
    test.wstr(0, 100, "Adsd");

    test.dbox(0, 0, 20, 40);
    test.ri();
    test.gch();
    test.reset();
    test.ri();

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            int color = rand() % 8;

            for (int k = 0; k < 5; k++) {
                screen.dline(5 + i * 12 + k * 2, 10 + j * 12 * 2, 17, false, {j, i}, color);
            }

            for (int k = 0; k < 5; k++) {
                screen.dline(5 + i * 12, 10 + (j * 12 + k * 2) * 2, 9, true, {j, i}, color);
            }
        }
    }

    screen.ri();
    screen.gch();
    screen.reset();

    int gridValues[4] = {5, 5, 5, 11};
    for (int i = 0; i < 100; i++) {
        gridValues[0] = rand() % 5 + 1;
        gridValues[1] = rand() % 5 + 1;
        gridValues[2] = rand() % 5 + 1;
        gridValues[3] = rand() % 11 + 1;

        std::pair<int, int> gridDims =                                             screen.calcGridDims(gridValues[0], gridValues[1], gridValues[2], gridValues[3]);
        screen.dgrid(screen.gety() / 2 - gridDims.first / 2, screen.getx() / 2 - gridDims.second / 2, gridValues[0], gridValues[1], gridValues[2], gridValues[3], {rand() % 8, rand() % 6}, rand() % 8);

        screen.ri();
        sleep_for(100ms);
        screen.reset();
    }
        
    endwin();
    return 0;
}
