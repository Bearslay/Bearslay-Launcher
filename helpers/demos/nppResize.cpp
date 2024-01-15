#include "/home/bearslay/Bearslay-Launcher/helpers/ncursespp.hpp"

int main() {
    npp::init(true);

    npp::Window win(LINES / 2 - 12, COLS / 2 - 25, 25, 50);
    npp::Button btn(LINES / 2 - 12, COLS / 2 - 25, 25, 50);
    btn.tset({M1_CLICK, M2_CLICK});

    int ch;
    char mode = 0;
    char style1 = 0, style2 = 0;
    char color = 1;
    bool m4;
    std::wstring modeNames[7] = {L"Move Y", L"Move X", L"Resize Y", L"Resize X", L"Style 1", L"Style 2", L"Color"};

    win.dbox({style1, style2}, color);
    win.wstr(1, 1, L"Mouse: " + modeNames[mode], color);

    while (true) {
        if ((ch = win.gchar(false)) == KEY_MOUSE) {
            npp::Mouse.gmouse(ch);

            if (btn.cclick() != M_UNKNOWN) {
                win.wstr(2, 1, L"Pressed", color);
            } else {
                win.wstr(2, 1, L"       ");
                switch (npp::Mouse.ginput()) {
                    case M4_PRESS:
                    case M5_PRESS:
                        m4 = npp::Mouse.ginput() == M4_PRESS ? true : false;

                        switch (mode) {
                            case 0:
                                win.move(win.gposy() + (m4 ? (win.gposy() == 0 ? 0 : -1) : 1), win.gposx());
                                btn.move(btn.gposy() + (m4 ? (win.gposy() == 0 ? 0 : -1) : 1), btn.gposx());
                                break;
                            case 1:
                                win.move(win.gposy(), win.gposx() + (m4 ? 1 : (win.gposx() == 0 ? 0 : -1)));
                                btn.move(btn.gposy(), btn.gposx() + (m4 ? 1 : (win.gposx() == 0 ? 0 : -1)));
                                break;
                            case 2:
                                win.reset();
                                win.resize(win.gdimy() + (m4 ? 1 : -1), win.gdimx());
                                btn.resize(btn.gdimy() + (m4 ? 1 : -1), btn.gdimx());
                                break;
                            case 3:
                                win.reset();
                                win.resize(win.gdimy(), win.gdimx() + (m4 ? 1 : -1));
                                btn.resize(btn.gdimy(), btn.gdimx() + (m4 ? 1 : -1));
                                break;
                            case 4:
                                style1--;
                                style1 = (style1 < 0) ? 7 : style1;
                                style1 = (style1 > 7) ? 0 : style1;
                                break;
                            case 5:
                                style2--;
                                style2 = (style2 < 0) ? 5 : style2;
                                style2 = (style2 > 5) ? 0 : style2;
                                break;
                            case 6:
                                color--;
                                color = (color < 1) ? 7 : color;
                                color = (color > 7) ? 1 : color;
                                break;
                        }
                        break;
                    case M1_CLICK:
                        mode++;
                        mode = (mode > 6) ? 0 : mode;
                        break;
                    case M2_CLICK:
                        mode--;
                        mode = (mode < 0) ? 6 : mode;
                        break;
                }
            }
        } else {
            switch (ch) {
                case 'q':
                    return npp::end(true);
                case KEY_UP:
                case KEY_DOWN:
                    win.move(win.gposy() + (ch == KEY_UP ? -1 : 1), win.gposx());
                    btn.move(btn.gposy() + (ch == KEY_UP ? -1 : 1), btn.gposx());
                    break;
                case KEY_LEFT:
                case KEY_RIGHT:
                    win.move(win.gposy(), win.gposx() + (ch == KEY_RIGHT ? 1 : -1));
                    btn.move(btn.gposy(), btn.gposx() + (ch == KEY_RIGHT ? 1 : -1));
                    break;
                case 'w':
                case 's':
                    win.reset();
                    win.resize(win.gdimy() + (ch == 'w' ? 1 : -1), win.gdimx());
                    btn.resize(btn.gdimy() + (ch == 'w' ? 1 : -1), btn.gdimx());
                    break;
                case 'a':
                case 'd':
                    win.reset();
                    win.resize(win.gdimy(), win.gdimx() + (ch == 'd' ? 1 : -1));
                    btn.resize(btn.gdimy(), btn.gdimx() + (ch == 'd' ? 1 : -1));
                    break;
                case '1':
                    color = 1;
                    break;
                case '2':
                    color = 2;
                    break;
                case '3':
                    color = 3;
                    break;
                case '4':
                    color = 4;
                    break;
                case '5':
                    color = 5;
                    break;
                case '6':
                    color = 6;
                    break;
                case '7':
                    color = 7;
                    break;
                case ';':
                    style1--;
                    style1 = (style1 < 0) ? 7 : style1;
                    style1 = (style1 > 7) ? 0 : style1;
                    break;
                case '\'':
                    style2--;
                    style2 = (style2 < 0) ? 7 : style2;
                    style2 = (style2 > 7) ? 0 : style2;
                    break;
            }
        }

        win.dbox({style1, style2}, color);
        win.wstr(1, 1, L"Mouse: " + modeNames[mode], color);
    }

    return npp::end(true);
}
