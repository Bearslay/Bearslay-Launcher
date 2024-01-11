#include </home/bearslay/Bearslay-Launcher/helpers/render.h>
#include </home/bearslay/Bearslay-Launcher/helpers/stringConversions.h>

#include <cmath>
#include <ctime>

void activateColors() {
    init_color(0, 0, 0, 0); // Black
    init_color(1, 999, 999, 999); // White
    init_color(2, 0, 999, 999); // Cursor
    init_color(3, 999, 999, 999);
    init_color(4, 999, 999, 999);

    init_pair(0, 0, 0);
    init_pair(1, 1, 0);
    init_pair(2, 2, 0);
    init_pair(3, 3, 0);
    init_pair(4, 4, 0);

    // Initialize the 250 colors that will be used to draw as just random colors for now
    for (unsigned char i = 5; i < 255; i++) {
        init_color(i, rand() % 999, rand() % 999, rand() % 999);
        init_pair(i, i, 0);
    }

    init_color(5, 999, 0, 0);
    init_color(6, 999, 337, 0);
    init_color(7, 999, 999, 0);
    init_color(8, 0, 999, 0);
    init_color(9, 0, 0, 999);
    init_color(10, 635, 0, 859);
    init_color(11, 999, 0, 999);
    init_color(12, 0, 999, 999);
    init_color(13, 999, 999, 999);
    init_color(14, 498, 498, 498);
}

class painter_colorPanel {
    private:
        BSCREEN Screen = BSCREEN(20, 35, 1, mainScreen.getx() - 37);
        short Page = 0, Position = 0, CurrentColor = 0;

        struct Color {
            short r = 999;
            short g = 999;
            short b = 999;
        };
        std::vector<Color> Colors;

        bool editColor() {
            int input;
            short r, g, b, pos = 0;
            unsigned char increment = 1;
            color_content(CurrentColor + 5, &r, &g, &b);

            while (true) {
                Screen.wa(17, 2, {{{0}, {{0, 0}, "Editing Color #"}}, {{1}, {{Page * 10 + Position + 1, 3}, ""}}, {{0}, {{0, 0}, " | "}}, {{0, CurrentColor + 5, 1}, {{0, 0}, "###"}}, {{0}, {{0, 0}, " -> "}}, {{0, 4, 1}, {{0, 0}, "###"}}});
                Screen.wa(18, 2, {{{0}, {{0, 0}, "Step: "}}, {{1}, {{increment, 3}, ""}}, {{0}, {{0, 0}, " ("}}, {{0, 1, (pos == 0) ? 1 : 0}, {{0, 0}, "R "}}, {{1, 1, (pos == 0) ? 1 : 0}, {{r, 3}, ""}}, {{0}, {{0, 0}, ", "}}, {{0, 1, (pos == 1) ? 1 : 0}, {{0, 0}, "G "}}, {{1, 1, (pos == 1) ? 1 : 0}, {{g, 3}, ""}}, {{0}, {{0, 0}, ", "}}, {{0, 1, (pos == 2) ? 1 : 0}, {{0, 0}, "B "}}, {{1, 1, (pos == 2) ? 1 : 0}, {{b, 3}, ""}}, {{0}, {{0, 0}, ")"}}});
                if (pos == 0) {
                    Screen.wstr(18, 13, "R", 1, true);
                }
                else if (pos == 1) {
                    Screen.wstr(18, 20, "G", 1, true);
                }
                else if (pos == 2) {
                    Screen.wstr(18, 27, "B", 1, true);
                }

                input = Screen.gch();

                init_color(4, r, g, b);

                if (input == 'q') {
                    return false;
                }
                else if (input == KEY_LEFT) {
                    pos--;
                    pos = (pos < 0) ? 2 : pos;
                }
                else if (input == KEY_RIGHT) {
                    pos++;
                    pos = (pos > 2) ? 0 : pos;
                }
                else if (input == KEY_DOWN) {
                    switch (pos) {
                        case 0:
                            r -= increment;
                            r = (r < 0) ? 0 : r;
                            break;
                        case 1:
                            g -= increment;
                            g = (g < 0) ? 0 : g;
                            break;
                        case 2:
                            b -= increment;
                            b = (b < 0) ? 0 : b;
                            break;
                    }
                }
                else if (input == KEY_UP) {
                    switch (pos) {
                        case 0:
                            r += increment;
                            r = (r > 999) ? 999 : r;
                            break;
                        case 1:
                            g += increment;
                            g = (g > 999) ? 999 : g;
                            break;
                        case 2:
                            b += increment;
                            b = (b > 999) ? 999 : b;
                            break;
                    }
                }
                else if (input == 'i') {
                    increment *= 10;
                    increment = (increment > 100) ? 1 : increment;
                }
                else if (input == 'z') {
                    Colors[CurrentColor].r = r;
                    Colors[CurrentColor].g = g;
                    Colors[CurrentColor].b = b;
                    init_color(CurrentColor + 5, r, g, b);
                    return true;
                }
                else if (input == 'x') {
                    return true;
                }
            }
        }

    public:
        painter_colorPanel() {
            Screen.dbox(0, 0, Screen.gety(), Screen.getx(), {LD_Heavy_Both, LD_Dashed_None});
            Screen.dline(2, 0, Screen.getx(), false, {LD_Heavy_Both, LD_Dashed_Single_1});
            Screen.dline(16, 0, Screen.getx(), false, {LD_Heavy_Both, LD_Dashed_Single_1});
            Screen.rl(2);

            for (unsigned char i = 0; i < 250; i++) {
                short r, g, b;
                color_content(i + 5, &r, &g, &b);

                Colors.emplace_back();
                Colors[i].r = r;
                Colors[i].g = g;
                Colors[i].b = b;
            }
        }

        void render() {
            // Display the current color
            Screen.wa(1, 2, {{{0, 1, 1}, {{0, 0}, "Using Color ("}}, {{1, 1, 1}, {{Colors[CurrentColor].r, 3}, ""}}, {{0, 1, 1}, {{0, 0}, ", "}}, {{1, 1, 1}, {{Colors[CurrentColor].g, 3}, ""}}, {{0, 1, 1}, {{0, 0}, ", "}}, {{1, 1, 1}, {{Colors[CurrentColor].b, 3}, ""}}, {{0, 1, 1}, {{0, 0}, ") "}}, {{0, CurrentColor + 5, 1}, {{0, 0}, "###"}}});

            // List the colors on the current page
            Screen.wstr(3, 2, "Key    Num    R    G    B", 1, true);
            for (unsigned char i = 0; i < 10; i++) {
                Screen.wa(4 + i, 2, {{{0}, {{0, 0}, "\'"}}, {{1}, {{(i + 1) % 10, 0}, ""}}, {{0}, {{0, 0}, "\' "}}, {{0, 2}, {{0, 0}, "~"}}, {{0}, {{0, 0}, " ["}}, {{1}, {{Page * 10 + i + 1, 3}, ""}}, {{0}, {{0, 0}, "] ("}}, {{1}, {{Colors[Page * 10 + i].r, 3}, ""}}, {{0}, {{0, 0}, ", "}}, {{1}, {{Colors[Page * 10 + i].g, 3}, ""}}, {{0}, {{0, 0}, ", "}}, {{1}, {{Colors[Page * 10 + i].b, 3}, ""}}, {{0}, {{0, 0}, ") "}}, {{0, Page * 10 + i + 5, 1}, {{0, 0}, "# "}},{{0, 2}, {{0, 0}, "~"}}});
            }

            // Page indicators
            Screen.wa(15, 2, {{{0}, {{0, 0}, "Listing "}}, {{1}, {{Page * 10 + 1, 3}, ""}}, {{0}, {{0, 0}, "->"}}, {{1}, {{Page * 10 + 10, 3}, ""}}, {{0}, {{0, 0}, "/250 | Pg "}}, {{1}, {{Page + 1, 2}, ""}}, {{0}, {{0, 0}, "/25"}}});
            
            // Color editing area default
            Screen.wstr(17, 1, "          Not Currently          ");
            Screen.wstr(18, 1, "         Editing A Color         ");
            
            Screen.ri();
        }

        unsigned char getColor(unsigned char input) {
            Position = input - 1;
            CurrentColor = Page * 10 + input - 1;
            render();
            return Page * 10 + input + 4;
        }

        // 0 = Quit Program, 1+ = Use new color with canvas
        unsigned char interact() {
            while (true) {
                CurrentColor = Page * 10 + Position;
                render();

                Screen.wstr(4 + Position, 6, "<", 2, true);
                Screen.wstr(4 + Position, 32, ">", 2, true);

                int input = Screen.gch();

                for (unsigned char i = 0; i < 10; i++) {
                    Screen.wstr(4 + i, 6, "~", 2);
                    Screen.wstr(4 + i, 32, "~", 2);
                }

                Screen.ri();

                if (input == KEY_UP) {
                    Position--;
                    if (Position < 0) {
                        Position = 9;

                        Page--;
                        if (Page < 0) {
                            Page = 0;
                            Position = 0;
                        }
                    }    
                }
                else if (input == KEY_DOWN) {
                    Position++;
                    if (Position > 9) {
                        Position = 0;

                        Page++;
                        if (Page > 24) {
                            Page = 24;
                            Position = 9;
                        }
                    }
                }
                else if (input == 'x' || input == 'd') {
                    return CurrentColor + 5;
                }
                else if (input == 'z') {
                    if (!editColor()) {
                        return 0;
                    }
                }
                else if (input == 'q') {
                    return 0;
                }
                else if (input == '1') {
                    Position = 0;
                }
                else if (input == '2') {
                    Position = 1;
                }
                else if (input == '3') {
                    Position = 2;
                }
                else if (input == '4') {
                    Position = 3;
                }
                else if (input == '5') {
                    Position = 4;
                }
                else if (input == '6') {
                    Position = 5;
                }
                else if (input == '7') {
                    Position = 6;
                }
                else if (input == '8') {
                    Position = 7;
                }
                else if (input == '9') {
                    Position = 8;
                }
                else if (input == '0') {
                    Position = 9;
                }
            }
        }
};

class painter_toolPanel {
    private:
        BSCREEN Screen = BSCREEN(12, 35, 21, mainScreen.getx() - 37);

        struct {
            unsigned char sizey = 1;
            unsigned char sizex = 1;
            bool toggled = false;
            unsigned char stepy = 1;
            unsigned char stepx = 1;
        } Brush, Eraser;

        struct {
            // true = copy, false = paste
            bool copy = true;
            bool toggled = false;
            unsigned short dimy = 1, dimx = 1;
        } Stamp;

        struct {
            unsigned short starty = 0, startx = 0, dimy = 1, dimx = 1;
        } Select;

        unsigned char MoveStep = 1;

        struct {
            unsigned short oldVals[3] = {999, 999, 999};
            unsigned char oldIndex = 0;
            unsigned short newVals[3] = {999, 999, 999};
            unsigned char newIndex = 0;
            // true = always overwrite current color, false = search for matching color before overwriting
            bool overwrite = false;
        } Picker;

        // 0 = Brush, 1 = Eraser, 2 = Stamp, 3 = Select, 4 = Move, 5 = Picker, 6 = Line
        char CurrentTool = 0;
        char Editpos = 0;

        bool guides = false;

        void renderBrushMenu(char cursorPos = -1) {
            Screen.wstr(3, 21, "Size", 1, true);
            Screen.wa(4, 21, {{{0}, {{0, 0}, "x "}}, {{0, 2, (cursorPos == 0) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 0) ? "<" : "~"}}, {{0}, {{NULL, NULL}, " - "}}, {{1, 1, 1}, {{Brush.sizex, 2}, ""}}, {{0}, {{NULL, NULL}, " + "}}, {{0, 2, (cursorPos == 0) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 0) ? ">" : "~"}}});
            Screen.wa(5, 21, {{{0}, {{0, 0}, "y "}}, {{0, 2, (cursorPos == 1) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 1) ? "<" : "~"}}, {{0}, {{NULL, NULL}, " - "}}, {{1, 1, 1}, {{Brush.sizey, 2}, ""}}, {{0}, {{NULL, NULL}, " + "}}, {{0, 2, (cursorPos == 1) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 1) ? ">" : "~"}}});

            Screen.wstr(6, 21, "Step", 1, true);
            Screen.wa(7, 21, {{{0}, {{0, 0}, "x "}}, {{0, 2, (cursorPos == 2) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 2) ? "<" : "~"}}, {{0}, {{NULL, NULL}, " - "}}, {{1, 1, 1}, {{Brush.stepx, 2}, ""}}, {{0}, {{NULL, NULL}, " + "}}, {{0, 2, (cursorPos == 2) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 2) ? ">" : "~"}}});
            Screen.wa(8, 21, {{{0}, {{0, 0}, "y "}}, {{0, 2, (cursorPos == 3) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 3) ? "<" : "~"}}, {{0}, {{NULL, NULL}, " - "}}, {{1, 1, 1}, {{Brush.stepy, 2}, ""}}, {{0}, {{NULL, NULL}, " + "}}, {{0, 2, (cursorPos == 3) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 3) ? ">" : "~"}}});

            Screen.wstr(9, 21, "Toggled", 1, true);
            Screen.wa(10, 21, {{{0, 2, (cursorPos == 4) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 4) ? "<" : "~"}}, {{0, 1, (Brush.toggled) ? 1 : 0}, {{NULL, NULL}, " On"}}, {{0}, {{NULL, NULL}, "/"}}, {{0, 1, (Brush.toggled) ? 0 : 1}, {{NULL, NULL}, "Off "}}, {{0, 2, (cursorPos == 4) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 4) ? ">" : "~"}}});
        }

        void renderEraserMenu(char cursorPos = -1) {
            Screen.wstr(3, 21, "Size", 1, true);
            Screen.wa(4, 21, {{{0}, {{0, 0}, "x "}}, {{0, 2, (cursorPos == 0) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 0) ? "<" : "~"}}, {{0}, {{NULL, NULL}, " - "}}, {{1, 1, 1}, {{Eraser.sizex, 2}, ""}}, {{0}, {{NULL, NULL}, " + "}}, {{0, 2, (cursorPos == 0) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 0) ? ">" : "~"}}});
            Screen.wa(5, 21, {{{0}, {{0, 0}, "y "}}, {{0, 2, (cursorPos == 1) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 1) ? "<" : "~"}}, {{0}, {{NULL, NULL}, " - "}}, {{1, 1, 1}, {{Eraser.sizey, 2}, ""}}, {{0}, {{NULL, NULL}, " + "}}, {{0, 2, (cursorPos == 1) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 1) ? ">" : "~"}}});

            Screen.wstr(6, 21, "Step", 1, true);
            Screen.wa(7, 21, {{{0}, {{0, 0}, "x "}}, {{0, 2, (cursorPos == 2) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 2) ? "<" : "~"}}, {{0}, {{NULL, NULL}, " - "}}, {{1, 1, 1}, {{Eraser.stepx, 2}, ""}}, {{0}, {{NULL, NULL}, " + "}}, {{0, 2, (cursorPos == 2) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 2) ? ">" : "~"}}});
            Screen.wa(8, 21, {{{0}, {{0, 0}, "y "}}, {{0, 2, (cursorPos == 3) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 3) ? "<" : "~"}}, {{0}, {{NULL, NULL}, " - "}}, {{1, 1, 1}, {{Eraser.stepy, 2}, ""}}, {{0}, {{NULL, NULL}, " + "}}, {{0, 2, (cursorPos == 3) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 3) ? ">" : "~"}}});

            Screen.wstr(9, 21, "Toggled", 1, true);
            Screen.wa(10, 21, {{{0, 2, (cursorPos == 4) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 4) ? "<" : "~"}}, {{0, 1, (Eraser.toggled) ? 1 : 0}, {{NULL, NULL}, " On"}}, {{0}, {{NULL, NULL}, "/"}}, {{0, 1, (Eraser.toggled) ? 0 : 1}, {{NULL, NULL}, "Off "}}, {{0, 2, (cursorPos == 4) ? 1 : 0}, {{NULL, NULL}, (cursorPos == 4) ? ">" : "~"}}});
        }

        bool editBrush() {
            while (true) {
                renderBrushMenu(Editpos);

                int input = Screen.gch();

                if (input == KEY_UP) {
                    Editpos--;
                    Editpos = (Editpos < 0) ? 0 : Editpos;
                }
                else if (input == KEY_DOWN) {
                    Editpos++;
                    Editpos = (Editpos > 4) ? 4 : Editpos;
                }
                else if (input == KEY_LEFT) {
                    if (Editpos == 0) {
                        Brush.sizex--;
                        Brush.sizex = (Brush.sizex < 1) ? 1 : Brush.sizex;
                    }
                    else if (Editpos == 1) {
                        Brush.sizey--;
                        Brush.sizey = (Brush.sizey < 1) ? 1 : Brush.sizey;
                    }
                    else if (Editpos == 2) {
                        Brush.stepx--;
                        Brush.stepx = (Brush.stepx < 1) ? 1 : Brush.stepx;
                    }
                    else if (Editpos == 3) {
                        Brush.stepy--;
                        Brush.stepy = (Brush.stepy < 1) ? 1 : Brush.stepy;
                    }
                    else if (Editpos == 4) {
                        Brush.toggled = !Brush.toggled;
                    }
                }
                else if (input == KEY_RIGHT) {
                    if (Editpos == 0) {
                        Brush.sizex++;
                        Brush.sizex = (Brush.sizex > 50) ? 50 : Brush.sizex;
                    }
                    else if (Editpos == 1) {
                        Brush.sizey++;
                        Brush.sizey = (Brush.sizey > 25) ? 25 : Brush.sizey;
                    }
                    else if (Editpos == 2) {
                        Brush.stepx++;
                        Brush.stepx = (Brush.stepx > 50) ? 50 : Brush.stepx;
                    }
                    else if (Editpos == 3) {
                        Brush.stepy++;
                        Brush.stepy = (Brush.stepy > 25) ? 25 : Brush.stepy;
                    }
                    else if (Editpos == 4) {
                        Brush.toggled = !Brush.toggled;
                    }
                }
                else if (input == 'z') {
                    return true;
                }
                else if (input == 'x') {
                    Brush.sizex = 1;
                    Brush.sizey = 1;
                    Brush.stepx = 1;
                    Brush.stepy = 1;
                }
                else if (input == 'q') {
                    return false;
                }
                else if (input == 'c') {
                    Brush.sizex = Eraser.sizex;
                    Brush.sizey = Eraser.sizey;
                    Brush.stepx = Eraser.stepx;
                    Brush.stepy = Eraser.stepy;
                }
            }
        }

        bool editEraser() {
            while (true) {
                renderEraserMenu(Editpos);

                int input = Screen.gch();

                if (input == KEY_UP) {
                    Editpos--;
                    Editpos = (Editpos < 0) ? 0 : Editpos;
                }
                else if (input == KEY_DOWN) {
                    Editpos++;
                    Editpos = (Editpos > 4) ? 4 : Editpos;
                }
                else if (input == KEY_LEFT) {
                    if (Editpos == 0) {
                        Eraser.sizex--;
                        Eraser.sizex = (Eraser.sizex < 1) ? 1 : Eraser.sizex;
                    }
                    else if (Editpos == 1) {
                        Eraser.sizey--;
                        Eraser.sizey = (Eraser.sizey < 1) ? 1 : Eraser.sizey;
                    }
                    else if (Editpos == 2) {
                        Eraser.stepx--;
                        Eraser.stepx = (Eraser.stepx < 1) ? 1 : Eraser.stepx;
                    }
                    else if (Editpos == 3) {
                        Eraser.stepy--;
                        Eraser.stepy = (Eraser.stepy < 1) ? 1 : Eraser.stepy;
                    }
                    else if (Editpos == 4) {
                        Eraser.toggled = !Eraser.toggled;
                    }
                }
                else if (input == KEY_RIGHT) {
                    if (Editpos == 0) {
                        Eraser.sizex++;
                        Eraser.sizex = (Eraser.sizex > 50) ? 50 : Eraser.sizex;
                    }
                    else if (Editpos == 1) {
                        Eraser.sizey++;
                        Eraser.sizey = (Eraser.sizey > 25) ? 25 : Eraser.sizey;
                    }
                    else if (Editpos == 2) {
                        Eraser.stepx++;
                        Eraser.stepx = (Eraser.stepx > 50) ? 50 : Eraser.stepx;
                    }
                    else if (Editpos == 3) {
                        Eraser.stepy++;
                        Eraser.stepy = (Eraser.stepy > 25) ? 25 : Eraser.stepy;
                    }
                    else if (Editpos == 4) {
                        Eraser.toggled = !Eraser.toggled;
                    }
                }
                else if (input == 'z') {
                    return true;
                }
                else if (input == 'x') {
                    Eraser.sizex = 1;
                    Eraser.sizey = 1;
                    Eraser.toggled = false;
                    Eraser.stepx = 1;
                    Eraser.stepy = 1;
                }
                else if (input == 'q') {
                    return false;
                }
                else if (input == 'c') {
                    Eraser.sizex = Brush.sizex;
                    Eraser.sizey = Brush.sizey;
                    Eraser.stepx = Brush.stepx;
                    Eraser.stepy = Brush.stepy;
                }
            }
        }

    public:
        painter_toolPanel() {
            Screen.dbox(0, 0, Screen.gety(), Screen.getx(), {LD_Heavy_Both, LD_Dashed_None});
            Screen.dline(2, 19, Screen.gety(), true, {LD_Heavy_Both, LD_Dashed_Single_1});
            Screen.dline(2, 0, Screen.getx(), false, {LD_Heavy_Both, LD_Dashed_Single_1});
            Screen.rl(2);
        }

        void render() {
            // List of tools
            std::string toolNames[7] = {"Brush ", "Eraser", "Stamp ", "Select", "Move  ", "Picker", "Line  "};
            std::string keys[7] = {"w", "e", "c", "s", "m", "a", "l"};
            for (int i = 0; i < 7; i++) {
                Screen.wa(4 + i, 2, {{{0}, {{NULL, NULL}, "\'"}}, {{0}, {{NULL, NULL}, keys[i]}}, {{0}, {{NULL, NULL}, "\' "}}, {{0, 2}, {{NULL, NULL}, "~"}}, {{0}, {{NULL, NULL}, " ["}}, {{0}, {{NULL, NULL}, toolNames[i]}}, {{0}, {{NULL, NULL}, "] "}}, {{0, 2}, {{NULL, NULL}, "~"}}});
            }
            Screen.wstr(3, 2, "Key     Tool", 1, true);

            // Info menu
            if (CurrentTool == 0) {
                renderBrushMenu();
            }
            else if (CurrentTool == 1) {
                renderEraserMenu();
            }

            // Title
            Screen.wa(1, 2, {{{0, 1, 1}, {{0, 0}, "Using "}}});

            Screen.ri();
        }

        void renderCursor(BSCREEN &canvas, BSCREEN &storage, unsigned short y, unsigned short x, unsigned char color) {            
            // Restore the canvas
            for (unsigned short i = 0; i < storage.gety(); i++) {
                for (unsigned short j = 0; j < storage.getx(); j++) {
                    canvas.wu(i + 1, j + 1, (storage.sch(i, j) == "█") ? "█" : " ", storage.sco(i, j));
                }
            }
            
            // Back up the canvas to storage
            for (unsigned short i = 1; i < canvas.gety() - 1; i++) {
                for (unsigned short j = 1; j < canvas.getx() - 1; j++) {
                    storage.wu(i - 1, j - 1, (canvas.sch(i, j) == "█") ? "█" : " ", canvas.sco(i, j));
                }
            }
            
            canvas.dbox(0, 0, canvas.gety(), canvas.getx(), {LD_Heavy_Both, LD_Dashed_None});

            if (CurrentTool == 0) {                
                if (guides) {
                    int infoy = y - 2, infox = x - 10;
                    if (infoy <= 0) {
                        infoy = y + Brush.sizey;
                    }
                    if (infox <= 0) {
                        infox = x + Brush.sizex;
                    }
                    canvas.wa(infoy, infox, {{{0}, {{0, 0}, "("}}, {{1}, {{x, 3}, ""}}, {{0}, {{0, 0}, ", "}}, {{1}, {{y, 3}, ""}}, {{0}, {{0, 0}, ")"}}});
                    canvas.wa(infoy + 1, infox, {{{0}, {{0, 0}, "("}}, {{1}, {{x + Brush.sizex - 1, 3}, ""}}, {{0}, {{0, 0}, ", "}}, {{1}, {{y + Brush.sizey - 1, 3}, ""}}, {{0}, {{0, 0}, ")"}}});

                    canvas.dline(0, x, canvas.gety(), true, {LD_Light_Hard, LD_Dashed_Single_1});
                    canvas.dline(y, 0, canvas.getx(), false, {LD_Light_Hard, LD_Dashed_Single_1});
                    canvas.dline(0, x + Brush.sizex - 1, canvas.gety(), true, {LD_Light_Hard, LD_Dashed_Single_1});
                    canvas.dline(y + Brush.sizey - 1, 0, canvas.getx(), false, {LD_Light_Hard, LD_Dashed_Single_1});
                }

                canvas.dbox(y, x, Brush.sizey, Brush.sizex, {LD_Light_Soft, LD_Dashed_None}, color);

                for (unsigned short i = 0; i < Brush.sizey; i++) {
                    for (unsigned short j = 0; j < Brush.sizex; j++) {
                        if (storage.sch(y + i - 1, x + j - 1) == "█") {
                            canvas.wu(y + i, x + j, "▒", color);
                        }
                    }
                }
            }
            else if (CurrentTool == 1) {
                if (guides) {
                    int infoy = y - 2, infox = x - 10;
                    if (infoy <= 0) {
                        infoy = y + Eraser.sizey;
                    }
                    if (infox <= 0) {
                        infox = x + Eraser.sizex;
                    }
                    canvas.wa(infoy, infox, {{{0}, {{0, 0}, "("}}, {{1}, {{x, 3}, ""}}, {{0}, {{0, 0}, ", "}}, {{1}, {{y, 3}, ""}}, {{0}, {{0, 0}, ")"}}});
                    canvas.wa(infoy + 1, infox, {{{0}, {{0, 0}, "("}}, {{1}, {{x + Eraser.sizex - 1, 3}, ""}}, {{0}, {{0, 0}, ", "}}, {{1}, {{y + Eraser.sizey - 1, 3}, ""}}, {{0}, {{0, 0}, ")"}}});

                    canvas.dline(0, x, canvas.gety(), true, {LD_Light_Hard, LD_Dashed_Single_1});
                    canvas.dline(y, 0, canvas.getx(), false, {LD_Light_Hard, LD_Dashed_Single_1});
                    canvas.dline(0, x + Eraser.sizex - 1, canvas.gety(), true, {LD_Light_Hard, LD_Dashed_Single_1});
                    canvas.dline(y + Eraser.sizey - 1, 0, canvas.getx(), false, {LD_Light_Hard, LD_Dashed_Single_1});
                }

                canvas.dbox(y, x, Eraser.sizey, Eraser.sizex, {LD_Light_Soft, LD_Dashed_Single_1}, color);

                for (unsigned short i = 0; i < Eraser.sizey; i++) {
                    for (unsigned short j = 0; j < Eraser.sizex; j++) {
                        if (storage.sch(y + i - 1, x + j - 1) == "█") {
                            canvas.wu(y + i, x + j, "▒", color);
                        }
                    }
                }
            }

            canvas.ri();
        }

        std::pair<unsigned short, unsigned short> moveCursor(BSCREEN &screen, int input, int y, int x) {
            unsigned short extray = 1, extrax = 1;
            
            if (CurrentTool == 0) {
                extray = Brush.sizey;
                extrax = Brush.sizex;
            }
            else if (CurrentTool == 1) {
                extray = Eraser.sizey;
                extrax = Eraser.sizex;
            }
            else if (CurrentTool == 2) {
                extray = Stamp.dimy;
                extrax = Stamp.dimx;
            }
            else if (CurrentTool == 4) {
                extray = Select.dimy;
                extrax = Select.dimx;
            }
            else if (CurrentTool == 3 || CurrentTool == 5 || CurrentTool == 6) {
                extray = 1;
                extrax = 1;
            }

            if (input == KEY_UP) {
                if (CurrentTool == 0) {
                    y -= Brush.stepy;
                }
                else if (CurrentTool == 1) {
                    y -= Eraser.stepy;
                }
                y = (y < 1) ? 1 : y;
            }
            else if (input == KEY_DOWN) {
                if (CurrentTool == 0) {
                    y += Brush.stepy;
                }
                else if (CurrentTool == 1) {
                    y += Eraser.stepy;
                }
                y = (y > screen.gety() - 1 - extray) ? screen.gety() - 1 - extray : y;
            }
            else if (input == KEY_LEFT) {
                if (CurrentTool == 0) {
                    x -= Brush.stepx;
                }
                else if (CurrentTool == 1) {
                    x -= Eraser.stepx;
                }
                x = (x < 1) ? 1 : x;
            }
            else if (input == KEY_RIGHT) {
                if (CurrentTool == 0) {
                    x += Brush.stepx;
                }
                else if (CurrentTool == 1) {
                    x += Eraser.stepx;
                }
                x = (x > screen.getx() - 1 - extrax) ? screen.getx() - 1 - extrax : x;
            }

            return {y, x};
        }

        void toggleBrush() {
            Brush.toggled = !Brush.toggled;
            Eraser.toggled = false;
            CurrentTool = 0;
        }
        void useBrush(BSCREEN &screen, unsigned short y, unsigned short x, unsigned char color) {
            CurrentTool = 0;
            for (unsigned char i = 0; i < Brush.sizey; i++) {
                for (unsigned char j = 0; j < Brush.sizex; j++) {
                    screen.wu(y + i - 1, x + j - 1, "█", color);
                }
            }
        }

        void toggleEraser() {
            Eraser.toggled = !Eraser.toggled;
            Brush.toggled = false;
            CurrentTool = 1;
        }
        void useEraser(BSCREEN &screen, unsigned short y, unsigned short x) {
            CurrentTool = 1;
            for (unsigned char i = 0; i < Eraser.sizey; i++) {
                for (unsigned char j = 0; j < Eraser.sizex; j++) {
                    screen.wstr(y + i - 1, x + j - 1, " ");
                }
            }
        }

        void toggleGuides() {guides = !guides;}

        // 0 = Quit program, 1 = Quit normally, 2 = Quit while toggling brush, 3 = Quit while toggling eraser
        unsigned char interact() {
            while (true) {
                Editpos = 0;
                render();

                Screen.wstr(4 + CurrentTool, 6, "<", 2, true);
                Screen.wstr(4 + CurrentTool, 17, ">", 2, true);

                int input = Screen.gch();

                for (unsigned char i = 0; i < 7; i++) {
                    Screen.wstr(4 + i, 6, "~", 2);
                    Screen.wstr(4 + i, 17, "~", 2);
                }

                if (input == KEY_UP) {
                    CurrentTool--;
                    CurrentTool = (CurrentTool < 0) ? 0 : CurrentTool;
                }
                else if (input == KEY_DOWN) {
                    CurrentTool++;
                    CurrentTool = (CurrentTool > 6) ? 6 : CurrentTool;
                }
                else if (input == 'q') {
                    return 0;
                }
                else if (input == 'x' || input == 't') {
                    if (Brush.toggled) {
                        return 2;
                    }
                    else if (Eraser.toggled) {
                        return 3;
                    }
                    else {
                        return 1;
                    }
                }
                else if (input == 'z' || input == KEY_RIGHT) {
                    if (CurrentTool == 0) {
                        if (editBrush() == false) {
                            return 0;
                        }
                    }
                    else if (CurrentTool == 1) {
                        if (editEraser() == false) {
                            return 0;
                        }
                    }
                }
            }
        }
};

int main() {
    startScreen();
    srand(int(time(0)));
    activateColors();

    // Show the acutal drawing
    BSCREEN canvas = BSCREEN(mainScreen.gety() - 2, mainScreen.getx() - 40, 1, 2);
    // Store the canvas so that it can be restored after the cursor has been moved
    BSCREEN storeCanvas = BSCREEN(mainScreen.gety() - 4, mainScreen.getx() - 42, 2, 3);

    mainScreen.dbox(0, 0, mainScreen.gety(), mainScreen.getx(), {LD_Heavy_Both, LD_Dashed_None});
    canvas.dbox(0, 0, canvas.gety(), canvas.getx(), {LD_Heavy_Both, LD_Dashed_None});

    mainScreen.rc(true, true);

    painter_colorPanel colorPanel;
    painter_toolPanel toolPanel;

    // Startup sequence because why the hell not lmao
    for (unsigned short i = 1; i < canvas.gety() - 1; i++) {
        for (unsigned short j = 1; j < canvas.getx() - 1; j++) {
            canvas.wu(i, j, "█", 5 + rand() % 250);
        }
    }
    canvas.rc(true, true);
    sleep_for(150ms);
    canvas.reset();
    canvas.dbox(0, 0, canvas.gety(), canvas.getx(), {LD_Heavy_Both, LD_Dashed_None});
    canvas.ri();

    unsigned char color = 5;
    std::pair<unsigned short, unsigned short> pos = {canvas.gety() / 2, canvas.getx() / 2};

    bool tBrush = false, tEraser = false;

    colorPanel.render();
    
    while (true) {
        toolPanel.render();
        toolPanel.renderCursor(canvas, storeCanvas, pos.first, pos.second, color);

        int input = canvas.gch();

        // Exit Program
        if (input == 'q') {
            break;
        }
        // Clear canvas
        if (input == 'O') {
            canvas.reset();
            storeCanvas.reset();
            canvas.dbox(0, 0, canvas.gety(), canvas.getx(), {LD_Heavy_Both, LD_Dashed_None});
        }
        // Activate guides
        if (input == 'g') {
            toolPanel.toggleGuides();
        }
        // Move around the canvas
        if (input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT) {
            pos = toolPanel.moveCursor(canvas, input, pos.first, pos.second);
        }
        // Color editing/selecting
        if (input == 'd') {
            color = colorPanel.interact();

            if (color == 0) {
                break;
            }
        }
        else if (input == '1') {
            color = colorPanel.getColor(1);
        }
        else if (input == '2') {
            color = colorPanel.getColor(2);
        }
        else if (input == '3') {
            color = colorPanel.getColor(3);
        }
        else if (input == '4') {
            color = colorPanel.getColor(4);
        }
        else if (input == '5') {
            color = colorPanel.getColor(5);
        }
        else if (input == '6') {
            color = colorPanel.getColor(6);
        }
        else if (input == '7') {
            color = colorPanel.getColor(7);
        }
        else if (input == '8') {
            color = colorPanel.getColor(8);
        }
        else if (input == '9') {
            color = colorPanel.getColor(9);
        }
        else if (input == '0') {
            color = colorPanel.getColor(10);
        }
        if (input == 't') {
            unsigned char temp = toolPanel.interact();

            tBrush = false;
            tEraser = false;

            if (temp == 0) {
                break;
            }
            else if (temp == 2) {
                tBrush = true;
            }
            else if (temp == 3) {
                tEraser = true;
            }
        }
        // Brush
        if (input == 'w' || tBrush) {
            toolPanel.useBrush(storeCanvas, pos.first, pos.second, color);
        }
        if (input == 'W') {
            toolPanel.toggleBrush();
            tBrush = !tBrush;
            tEraser = false;

            toolPanel.useBrush(storeCanvas, pos.first, pos.second, color);
        }
        // Eraser
        if (input == 'e' || tEraser) {
            toolPanel.useEraser(storeCanvas, pos.first, pos.second);
        }
        if (input == 'E') {
            toolPanel.toggleEraser();
            tEraser = !tEraser;
            tBrush = false;

            toolPanel.useEraser(storeCanvas, pos.first, pos.second);
        }

    }

    endwin();
    return 0;
}
