#ifndef MONOPOLY_PROPERTY
#define MONOPOLY_PROPERTY

// Standard Library
#include <ncurses.h>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

// Helpers
#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

class monopolyProperty {
    private:
        // Stores which of the 8 players owns the properties, -1 if no one owns it
        int Id = 0;
        int ownerId = -1;

        // first: full name, second: shortened name
        std::pair<std::string, std::string> Name;
        // Indicate which color set the property belongs to
        int colorSet;

        int Rent[7];

        int Price;
        int BuildingPrice;
        // 0 = Base, 1 = Set, 2 = 1 house, 3 = 2 houses, 4 = 3 houses, 5 = 4 houses, 6 = hotel
        int Stage = 5;
        // First: Mortgage cost
        // Second: Unmortgage cost (x1.1 mortgage cost)
        std::pair<int, int> Mortgage;

    public:
        monopolyProperty(int id = 0, bool american = true) {
            Id = id;
            
            Name.first = american;
            std::fstream file;
            file.open("propertyAttributes.txt");

            if (file.is_open()) {
                std::string line;

                // Get the correct line from the file
                for (int i = 0; i < 22; i++) {
                    getline(file, line);

                    if (i == id) {
                        break;
                    }
                }

                // Remove any spaces from the line
                bool noSpaces = false;
                while (!noSpaces) {
                    for (int i = 0; i < line.length(); i++) {
                        if (line[i] == ' ') {
                            line.erase(i, 1);
                            break;
                        }

                        if (i == line.length() - 1) {
                            noSpaces = true;
                        }
                    }
                }

                // Replace underscores with spaces
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == '_') {
                        line[i] = ' ';
                    }
                }

                // Divide the values into individual inputs
                int buildIndex = 0;
                std::string build[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == ':') {
                        buildIndex++;
                    }
                    else {
                        build[buildIndex] += line[i];
                    }
                }

                // Apply all of the values
                Name.first = (american) ? build[0] : build[1];
                Name.second = (american) ? build[2] : build[3];
                for (int i = 0; i < 7; i++) {
                    Rent[i] = std::stoi(build[4 + i]);
                }
                Price = std::stoi(build[11]);
                BuildingPrice = std::stoi(build[12]);
                Mortgage.first = std::stoi(build[13]);
                Mortgage.second = std::stoi(build[14]);
                colorSet = std::stoi(build[15]);
            }
        }

        void renderTile(BSCREEN &screen, int y, int x, std::string ownerName = "- Unowned -") {
            // Box
            screen.dbox(y, x, 6, 13, {}, (Stage >= 1) ? colorSet + 16 : 7);
            screen.wstr(y, x + 1, Name.second, colorSet + 16, true);

            // Owner's name
            if (ownerName.length() < 12) {
                screen.wstr(y + 3, x + 1, ownerName, ownerId + 8);
            }
            else {
                screen.wstr(y + 3, x + 1, ownerName.substr(0, 4) + "..." + ownerName.substr(ownerName.length() - 4, 4), ownerId + 8);
            }

            // Either cost or rent
            if (ownerId == -1) {
                screen.wstr(y + 4, x + 1, "Buy:");
                screen.wstr(y + 4, x + 8, "$", 24);
                screen.wint(y + 4, x + 9, Price, 3, 24);
            }
            else {
                screen.wstr(y + 4, x + 1, "Rent:");
                screen.wstr(y + 4, x + 7, "$", 24);
                screen.wint(y + 4, x + 8, Rent[Stage], 4, 24);
            }

            // Houses/hotels
            if (Stage == 6) {
                screen.wu(y + 1, x + 6, "▀", 26);
            }
            else {
                if (Stage >= 2) {
                    screen.wu(y + 1, x + 2, "☗", 25);
                }
                if (Stage >= 3) {
                    screen.wu(y + 1, x + 4, "☗", 25);
                }
                if (Stage >= 4) {
                    screen.wu(y + 1, x + 8, "☗", 25);
                }
                if (Stage >= 5) {
                    screen.wu(y + 1, x + 10, "☗", 25);
                }
            }
        }
};

#endif
