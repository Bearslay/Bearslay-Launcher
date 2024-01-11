#include </home/bearslay/Bearslay-Launcher/helpers/ncursespp.hpp>
using namespace npp;

int main() {
    init();

    mwin.dline(3, 25, 10, true, false, {LIGHT_HARD, DASHED_NONE}, 1);
    mwin.gchar();

    endwin();
    return 0;
}
