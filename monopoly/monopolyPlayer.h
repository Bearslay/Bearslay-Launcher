#ifndef MONOPOLY_PLAYER
#define MONOPOLY_PLAYER

// Standard Library
#include <ncurses.h>
#include <string>
#include <utility>
#include <vector>

// Helpers
#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

// Monopoly
#include </home/bearslay/Bearslay-Launcher/monopoly/monopolyProperty.h>

class monopolyPlayer {
    private:
        // Customization/identification
        unsigned char Id = 0;
        std::string Name;

        // [0] = TELEPHONE  ☎
        // [1] = ANCHOR     ⚓
        // [2] = YIN YANG   ☯
        // [3] = SHAMROCK   ☘
        // [4] = BALANCE    ⚖
        // [5] = FLAG       ⚐
        // [6] = CROSSBONES ☠
        // [7] = CLOUD      ☁
        // The player's id determines which piece they will use
        unsigned char ChosenPiece = 0;
        std::pair<std::string, std::string> GamePieces[8] = {
            {"TELEPHONE",  "☎"},
            {"ANCHOR",     "⚓"},
            {"YIN YANG",   "☯"},
            {"SHAMROCK",   "☘"},
            {"BALANCE",    "⚖"},
            {"FLAG",       "⚐"},
            {"CROSSBONES", "☠"},
            {"CLOUD",      "☁"}
        };

        // Game stats
        unsigned int Money = 1500;
        unsigned char JailCards = 0;
        struct {
            unsigned char totalProperties, totalColorSets, totalRailroads, totalUtilities;

            bool properties[22] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            bool colorSets[8] =   {0, 0, 0, 0, 0, 0, 0, 0};
            bool railroads[4] =   {0, 0, 0, 0};
            bool utilities[2] =   {0, 0};
        } Tiles;

        BSCREEN &Screen;

    public:
        monopolyPlayer(BSCREEN &screen, unsigned char id = 0, std::string name = "Player 00") : Screen(screen) {

            Id = id;
            Name = (name == "Player 00") ? "Player 0" + std::to_string(id + 1) : name;
        }

        void renderStats(BSCREEN &screen, unsigned short y, unsigned short x) {
            // Borders
            Screen.dbox(y, x, 13, 60);
            Screen.dbox(y + 9, x + 43, 4, 17);
            Screen.dline(y + 7, x, 60, false, {LD_Light_Hard, LD_Dashed_Single_1});
            Screen.dline(y + 10, x, 44, false);

            // Player name
            std::string playerTitle = "PLAYER  -  - ";
            playerTitle.insert(7, std::to_string(Id + 1));
            playerTitle.insert(11, Name);
            playerTitle.insert(14 + Name.length(), GamePieces[ChosenPiece].first);
            Screen.wstr(y + 11, x + 1, playerTitle, Id + 8, true);

            // Money box
            Screen.wstr(y + 10, x + 44, "JAIL CARDS:");
            Screen.wint(y + 10, x + 56, JailCards, 2);
            Screen.wstr(y + 11, x + 44, "FUNDS: $", 24);
            Screen.wint(y + 11, x + 52, Money, 6, 24);

            // Headers
            Screen.wstr(y + 1, x + 1, "PROPERTIES (00/22, 0/8 COLOR SETS):");
            Screen.wint(y + 1, x + 13, Tiles.totalProperties, 2);

            // Render properties
            for (int i = 0; i < 22; i++) {

            }
        }

        void addProperty(int id) {
            Tiles.properties[id] = true;

            // Scan for a new color set and add it in sorted order if there is one
            if (Tiles.properties[0] && Tiles.properties[1]) {
                Tiles.colorSets[0] = true;
            }
            if (Tiles.properties[20] && Tiles.properties[21]) {
                Tiles.colorSets[7] = true;
            }
            for (int i = 0; i < 6; i += 3) {
                if (Tiles.properties[2 + i] && Tiles.properties[3 + i] && Tiles.properties[4 + i]) {
                    Tiles.colorSets[i + 1] = true;
                }
            }
        }
};

#endif
