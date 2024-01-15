#include "/home/bearslay/Bearslay-Launcher/helpers/ncursespp.hpp"

int main() {
    npp::init(true);

    npp::Window window(5, 10, 10, 20);
    npp::Button button(5, 10, 10, 20);

    button.tadd(M1_CLICK);
    window.dbox();
    npp::mwin.dbox(4, 9, 12, 22);

    int ch;
    while (true) {
        if ((ch = window.gchar(false)) == KEY_MOUSE) {
            if (npp::Mouse.gmouse(ch)) {
                switch (button.cclick()) {
                    case M1_CLICK:
                        window.wstr(1, 1, L"Pressed    ");
                        break;
                    default:
                        window.wstr(1, 1, L"Not Pressed");
                        break;
                }

                npp::mwin.wstr(0, 0, L"x = ---, y = ---");
                npp::mwin.wint(npp::mwin.wintp(0, 4, npp::Mouse.gx(), 3, 1, "", {0, 7}), npp::Mouse.gy(), 3);
                npp::mwin.wint(npp::mwin.wstrp(1, 0, L"Button = "), npp::Mouse.ginput(), 2);
                npp::mwin.rinst();
            }
        } else {
            switch (ch) {
                case 'q':
                    return npp::end(true);
            }
        }
    }

    return npp::end(true);
}
