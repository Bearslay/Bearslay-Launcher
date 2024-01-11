#include "/home/bearslay/Bearslay-Launcher/helpers/ncursespp.hpp"
using namespace npp;

int main() {
    init();

    // Lines

    mwin.dline(3, 45, 10, true, false, {LIGHT_HARD, DASHED_NONE}, 1, false);
    mwin.dline(5, 35, 11, false, false, {LIGHT_HARD, DASHED_DOUBLE}, 1);
    mwin.dbox(15, 30, 5, 10);
    mwin.dline(1, 2, 10, true);
    mwin.dline(1, 2, 10, false);
    mwin.dline(5, 2, 10, false, false, {LIGHT_HARD, DASHED_NONE}, 1, true, false);
    mwin.dline(1, 6, 10, true, false, {LIGHT_HARD, DASHED_NONE}, 1, false, true);

    mwin.dline(20, 70, 10, true, true, {HEAVY_BOTH, DASHED_NONE}, 2);
    mwin.dline(19, 51, 10, true, true, {HEAVY_BOTH, DASHED_NONE}, 2);
    mwin.dline(20, 70, 20, false, true, {HEAVY_BOTH, DASHED_NONE}, 2);
    mwin.dline(10, 70, 20, false, true, {HEAVY_BOTH, DASHED_NONE}, 2);
    if (mwin.gchar() == 'q') {return end();}

    // Boxes

    unsigned char o = 0;
    std::wstring styles[8] = {L"LIGHT HARD", L"LIGHT SOFT", L"HEAVY BOTH", L"HEAVY HORIZONTAL", L"HEAVY VERTICAL", L"DOUBLED BOTH", L"DOUBLED HORIZONTAL", L"DOUBLED VERTICAL"};
    std::wstring dashes[6] = {L"NONE", L"DOUBLE", L"TRIPLE", L"QUARDRUPLE", L"SINGLE 1", L"SINGLE 2"};
    for (unsigned char i = 0; i < 8; i++) {
        mwin.reset();
        mwin.wstr(2, 10, styles[i], 1, "boun");

        o = 0;
        for (unsigned char j = 0; j < 3; j++) {
            mwin.wstr(4, 10 + o, dashes[j], 1, "un");
            mwin.dbox(5, 10 + o, 10, 20, {i, j}, 1);
            mwin.dbox(7, 14 + o, 6, 12, {i, j}, 1);
            
            mwin.dline(5, 15 + o, 10, true, false, {HEAVY_BOTH, DASHED_NONE}, 1);
            mwin.dline(5, 24 + o, 10, true, false, {DOUBLED_BOTH, DASHED_NONE}, 1);
            mwin.dline(8, 10 + o, 20, false, false, {HEAVY_BOTH, DASHED_NONE}, 1);
            mwin.dline(11, 10 + o, 20, false, false, {DOUBLED_BOTH, DASHED_NONE}, 1);

            mwin.dline(4, 29 + o, 12, true, false, {LIGHT_HARD, DASHED_NONE}, 2, true, false);
            
            o += 26;
        }
        o = 0;
        for (unsigned char j = 3; j < 6; j++) {
            mwin.wstr(19, 10 + o, dashes[j], 1, "un");
            mwin.dbox(20, 10 + o, 10, 20, {i, j}, 1);
            mwin.dbox(22, 14 + o, 6, 12, {i, j}, 1);
            
            mwin.dline(20, 15 + o, 10, true, false, {HEAVY_BOTH, DASHED_NONE}, 1);
            mwin.dline(20, 24 + o, 10, true, false, {DOUBLED_BOTH, DASHED_NONE}, 1);
            mwin.dline(23, 10 + o, 20, false, false, {HEAVY_BOTH, DASHED_NONE}, 1);
            mwin.dline(26, 10 + o, 20, false, false, {DOUBLED_BOTH, DASHED_NONE}, 1);

            mwin.dline(19, 29 + o, 12, true, false, {LIGHT_HARD, DASHED_NONE}, 2, true, false);
            
            o += 26;
        }
        if (mwin.gchar() == 'q') {break;}
    }

    // Grids

    mwin.reset();
    mwin.dgrid(2, 4, 3, 3, 6, 2);
    mwin.dgrid(25, 4, 3, 3, 0, 0, {HEAVY_VERTICAL, DASHED_QUADRUPLE}, 7);
    mwin.dgrid(2, 20, 5, 3, {DOUBLED_HORIZTONAL, DASHED_DOUBLE}, 2);
    if (mwin.gchar() == 'q') {return end();}

    std::vector<unsigned short> vals;
    for (unsigned char i = 0; i < 100; i++) {
        vals = {(unsigned short)(rand() % 5 + 1), (unsigned short)(rand() % 5 + 1), (unsigned short)(rand() % 5 + 1), (unsigned short)(rand() % 11 + 1)};
        std::pair<unsigned short, unsigned short> dims = mwin.gGridDims(vals[0], vals[1], vals[2], vals[3]);
        mwin.dgrid(mwin.gdimy() / 2 - dims.first / 2, mwin.gdimx() / 2 - dims.second / 2, vals[0], vals[1], vals[2], vals[3], {rand() % 8, rand() % 6}, rand() % 8);

        mwin.rinst();
        if (mwin.twait(50, {'q'})) {break;}
        mwin.reset();
    }

    return end();
}
