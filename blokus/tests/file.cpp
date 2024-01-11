#include <fstream>
#include <iostream>
#include <vector>

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

int main() {
    std::vector<std::vector<std::vector<bool>>> polyomino = readPolyominoFile("/home/bearslay/Bearslay-Launcher/blokus/Polyominoes/mono_pentominoes.txt");

    for (unsigned char i = 0; i < polyomino.size(); i++) {
        for (unsigned char j = 0; j < polyomino[i].size(); j++) {
            for (unsigned char k = 0; k < polyomino[i][j].size(); k++) {
                std::cout << polyomino[i][j][k];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
