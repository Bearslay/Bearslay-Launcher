#ifndef BLOKUS_POLYOMINOES
#define BLOKUS_POLYOMINOES
#include </home/bearslay/Bearslay-Launcher/helpers/render.h>
#include <fstream>

namespace Blokus {
    // Polyominoes where n = 1-5 (Polyominoes used in base Blokus)
    std::vector<std::vector<std::vector<bool>>> mono_pentominoes;
    // Polyominoes where n = 6
    std::vector<std::vector<std::vector<bool>>> hexominoes;
    // Polyominoes where n = 7
    std::vector<std::vector<std::vector<bool>>> heptominoes;

    // Transfers the data from formatted text files containing polyominoes into a vector to be used
    std::vector<std::vector<std::vector<bool>>> readPolyominoFile(std::string fileName) {
        std::vector<std::vector<std::vector<bool>>> o;
        
        std::ifstream f;
        f.open(fileName.c_str(), std::ios::in);

        if (f.is_open()) {
            int c, d;

            // Get both the amount of polyominoes in the file and the dimensions of each polyomino
            std::string l;

            std::getline(f, l);
            try {
                d = stoi(l);
            } catch (...) {
                f.close();
                return o;
            }
            d = stoi(l);

            std::getline(f, l);
            try {
                c = stoi(l);
            } catch (...) {
                f.close();
                return o;
            }
            c = stoi(l);
            
            // Place each polyomino into the array correctly
            for (int i = 0; i < c; i++) {
                o.emplace_back();
                std::getline(f, l);

                for (unsigned char j = 0; j < d; j++) {
                    o[i].emplace_back();
                    std::getline(f, l);

                    for (unsigned char k = 0; k < l.length(); k++) {
                        o[i][j].emplace_back((l[k] == '0') ? false : true);
                    }
                }
            }

            f.close();
        }

        return o;
    }
    
    void polyominoesLarge(BSCREEN &win, std::vector<std::vector<std::vector<bool>>> input, unsigned char mWidth, unsigned char d) {
        unsigned char oY = 0, oX = 0;

        for (unsigned char i = 0; i < input.size(); i++) {
            if (i % mWidth == 0 && i != 0) {
                oY++;
                oX = 0;
            }

            for (unsigned char j = 0; j < input[i].size(); j++) {
                for (unsigned char k = 0; k < input[i][j].size(); k++) {
                    win.wu(oY * (d + 1) + j, (oX * (d + 2) + k) * 2, (input[i][j][k]) ? "█" : " ");
                    win.wu(oY * (d + 1) + j, (oX * (d + 2) + k) * 2 + 1, (input[i][j][k]) ? "█" : " ");
                }
            }

            oX++;
        }
    }

    void polyominoesSmall(BSCREEN &win, std::vector<std::vector<std::vector<bool>>> input, unsigned char mWidth, unsigned char d) {
        unsigned char y = 0, oY = 0, oX = 0;
        std::string output = "";

        for (unsigned char i = 0; i < input.size(); i++) {
            y = (input[i].size() % 2 == 0) ? input[i].size() / 2 : (input[i].size() - 1) / 2;
            if (i % mWidth == 0 && i != 0) {
                oY++;
                oX = 0;
            }

            for (unsigned char j = 0; j < y; j++) {
                for (unsigned char k = 0; k < input[i][j].size(); k++) {
                    bool top = input[i][j * 2][k], bot = input[i][j * 2 + 1][k];

                    if (top && bot) {
                        output = "█";
                    } else if (!top && !bot) {
                        output = " ";
                    } else if (top && !bot) {
                        output = "▀";
                    } else {
                        output = "▄";
                    }

                    win.wu(oY * ((d + 1) / 2) + j, oX * (d + 2) + k, output);
                }
            }

            if (input[i].size() % 2 != 0) {
                for (unsigned char j = 0; j < input[i].size(); j++) {
                    win.wu(oY * ((d + 1) / 2) + (d / 2), oX * (d + 2) + j, (input[i][input[i].size() - 1][j]) ? "▀" : " ");
                }
            }

            oX++;
        }
    }

    std::vector<std::vector<std::vector<bool>>> polyominoesEnlarge(std::vector<std::vector<std::vector<bool>>> input, unsigned char increase = 1) {
        for (unsigned char I = 0; I < increase; I++) {
            unsigned char s = input[0].size();

            for (unsigned char i = 0; i < input.size(); i++) {
                // Add a new row
                input[i].emplace(input[i].begin() + ((s % 2 == 0) ? 0 : s));

                // Fill new row with false
                for (unsigned char j = 0; j < s + 1; j++) {
                    input[i][(s % 2 == 0) ? 0 : s].emplace_back(false);
                }

                // Add a new column
                for (unsigned char j = ((s % 2 == 0) ? 1 : 0); j < s + ((s % 2 == 0) ? 1 : 0); j++) {
                    input[i][j].emplace(input[i][j].begin() + ((s % 2 == 0) ? 0 : s), 0);
                }
            }
        }

        return input;
    }

    void polyominoesArray(BSCREEN &win, std::vector<std::vector<std::vector<bool>>> input, unsigned char mWidth, unsigned char d) {
        unsigned char oY = 0, oX = 0;

        for (unsigned char i = 0; i < input.size(); i++) {
            if (i % mWidth == 0 && i != 0) {
                oY++;
                oX = 0;
            }

            for (unsigned char j = 0; j < input[i].size(); j++) {
                for (unsigned char k = 0; k < input[i][j].size(); k++) {
                    win.wstr(oY * (d + 1) + j, oX * (d + 2) + k, (input[i][j][k]) ? "1" : "0");
                }
            }

            oX++;
        }
    }
}

#endif
