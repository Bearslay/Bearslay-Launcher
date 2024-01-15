#include "/home/bearslay/Bearslay-Launcher/helpers/ncursespp.hpp"
using namespace npp;

int main() {
    init();

    mwin.upall(1);
    mwin.wchar(0, 0, L'a');
    mwin.wchar(mwin.gdimy() - 1, mwin.gdimx() - 1, L'b');
    mwin.wint(mwin.wstrp(0, 5, L"hellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohello"), 42, 3);
    mwin.wint(1, 0, 39);
    mwin.wint(mwin.gdimy() - 2, mwin.gdimx() - 4, 88, 4, 6);
    mwin.gchar();

    return end();
}
