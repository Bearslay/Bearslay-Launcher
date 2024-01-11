#ifndef BLOKUS_EXTRANEOUS
#define BLOKUS_EXTRANEOUS

#include </home/bearslay/Bearslay-Launcher/helpers/render.h>
#include <fstream>

namespace Blokus {
    /// @brief Absolute path to "mono_pentominoes.txt"
    #define MONO_PENTOMINOES "/home/bearslay/Bearslay-Launcher/blokus/Polyominoes/mono_pentominoes.txt"
    /// @brief Absolute path to "hexominoes.txt"
    #define HEXOMINOES "/home/bearslay/Bearslay-Launcher/blokus/Polyominoes/hexominoes.txt"
    /// @brief Absolute path to "heptominoes.txt"
    #define HEPTOMINOES "/home/bearslay/Bearslay-Launcher/blokus/Polyominoes/heptominoes.txt"

    struct {
        // Players in the game
        unsigned char players = 4;

        // Side length of the Blokus Board
        unsigned char bdim = 20;

        // Dimensions of the Blokus Piece
        unsigned char pdim = 5;

        // Number of mono_pentomino sets
        unsigned char sBase = 1;
        // Number of hexomino sets
        unsigned char sHex = 0;
        // Number of heptomino sets
        unsigned char sHept = 0;

        // Max number of tiles each player could place
        unsigned short mTiles = 89;
    } Vals;

    // Polyominoes where n = 1-5 (Base Blokus Pieces)
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

    std::vector<std::vector<std::vector<bool>>> enlargePolyominoes(std::vector<std::vector<std::vector<bool>>> input, unsigned char increase = 1) {
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
}

#endif
