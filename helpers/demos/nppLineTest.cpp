#include </home/bearslay/Bearslay-Launcher/helpers/ncursespp.hpp>

int main() {
    npp::init();

    npp::mwin.dline(5, 10, 10, false, false, {LIGHT_HARD, DASHED_SINGLE_1}, 1);
    npp::mwin.dline(7, 10, 10, false, false, {HEAVY_HORIZONTAL, DASHED_NONE}, 1);
    npp::mwin.dline(3, 15, 10, true, false, {HEAVY_BOTH, DASHED_DOUBLE}, 3);
    npp::mwin.dline(3, 17, 10, true, false, {DOUBLED_BOTH, DASHED_NONE}, 5);
    npp::mwin.dline(9, 10, 10, false, false, {HEAVY_HORIZONTAL, DASHED_NONE}, 7);
    npp::mwin.dline(5, 10, 10, true, false, {LIGHT_HARD, DASHED_NONE}, 2);
    npp::mwin.dbox({LIGHT_SOFT, DASHED_DOUBLE}, 4);
    if (npp::mwin.gchar() == 'q') {
        endwin();
        return 0;
    }

    std::wstring styles[8] = {L"LIGHT HARD", L"LIGHT SOFT", L"HEAVY BOTH", L"HEAVY HORIZONTAL", L"HEAVY VERTICAL", L"DOUBLED BOTH", L"DOUBLED HORIZONTAL", L"DOUBLED VERTICAL"};
    std::wstring dashes[6] = {L"NONE", L"DOUBLE", L"TRIPLE", L"QUADRUPLE", L"SINGLE 1", L"SINGLE 2"};
    unsigned char o = 0;
    for (unsigned char i = 0; i < 8; i++) {
        npp::mwin.reset();

        o = 0;
        for (unsigned char j = 0; j < 3; j++) {
            npp::mwin.wstr(1, 2, styles[i]);
            npp::mwin.wstr(3, o + 2, dashes[j]);
            npp::mwin.dbox(4, o + 2, 12, 24, {i, j});
            npp::mwin.dbox(6, o + 6, 8, 16, {i, j});
            
            o += 26;
        }

        o = 0;
        for (unsigned char j = 3; j < 6; j++) {
            npp::mwin.wstr(17, o + 2, dashes[j]);
            npp::mwin.dbox(18, o + 2, 12, 24, {i, j});
            npp::mwin.dbox(20, o + 6, 8, 16, {i, j});
            
            o += 26;
        }

        if (npp::mwin.gchar() == 'q') {break;}
    }

    endwin();
    return 0;
}
