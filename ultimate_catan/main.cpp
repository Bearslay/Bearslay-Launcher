#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

namespace Catan {
    class Tile;
    //class Harbor;
    BSCREEN Board;

    std::vector<std::vector<Tile>> Tiles;
    //std::vector<std::vector<Harbor>> Harbors;

    // Initialize colors and pairs needed for catan as well as the various screens
    void initialize() {
        // Board screen initialization
        Board = BSCREEN(75, 164, LINES / 2 - 75 / 2, COLS / 2 - 82);

        // Default 8 colors
        init_color(0, 0, 0, 0);         // Black
        init_color(1, 999, 999, 999);   // White
        init_color(2, 999, 0, 0);       // Red
        init_color(3, 0, 999, 0);       // Green
        init_color(4, 999, 999, 0);     // Yellow
        init_color(5, 0, 0, 999);       // Blue
        init_color(6, 999, 0, 999);     // Magenta
        init_color(7, 0, 999, 999);     // Cyan

        // Player colors
        init_color(8, 999, 0, 0);       // Player 1 (red)
        init_color(9, 920, 304, 0);     // Player 2 (orange)
        init_color(10, 0, 0, 999);      // Player 3 (blue)
        init_color(11, 750, 750, 750);  // Player 4 (white)
        init_color(12, 0, 431, 0);      // Player 5 (green)
        init_color(13, 431, 235, 0);    // Player 6 (brown)
        init_color(14, 619, 0, 557);    // Player 7 (magenta)
        init_color(15, 0, 839, 784);    // Player 8 (aqua)

        // Tile rarity indicators
        init_color(16, 706, 706, 706);   // 2, 3, 4, 5, 9, 10, 11, 12
        init_color(17, 941, 161, 161);   // 6, 8

        // Resource/Tile colors
        init_color(18, 832, 832, 832);  // Sheep
        init_color(19, 55, 780, 16);
        init_color(20, 512, 512, 512);  // Rock
        init_color(21, 639, 639, 639);
        init_color(22, 0, 800, 150);    // Wood
        init_color(23, 298, 561, 51);
        init_color(24, 900, 150, 200);  // Brick
        init_color(25, 729, 67, 110);
        init_color(26, 832, 600, 0);    // Wheat
        init_color(27, 388, 831, 75);
        init_color(28, 561, 561, 561);  // Gold (Tile only)
        init_color(29, 78, 482, 729);
        init_color(30, 173, 192, 721);  // Water (Tile only)
        init_color(31, 78, 482, 729);
        init_color(32, 888, 692, 472);  // Desert (Tile only), unavailable action, empty border/intersection, a whole bunch of stuff
        init_color(33, 631, 498, 94);
        init_color(34, 584, 160, 888);  // General Resource (Mainly for showing 4:1 and 3:1 Trades)

        // Commodities, Progress Cards, and City Calendars
        init_color(35, 619, 749, 141);  // Cloth/Trade
        init_color(36, 24, 500, 48);    // Coins/Politics
        init_color(37, 24, 500, 48);    // Paper/Science

        // Dev Card colors
        init_color(38, 772, 448, 816);  // General (classic) dev card
        init_color(39, 760, 304, 896);  // Victory point dev card and counter
        init_color(40, 868, 388, 200);  // Knight and army size
        init_color(41, 24, 500, 48);    // Monopoly, Year of Plenty, and Road Building (econ cards)

        // Robber/Pirate
        init_color(42, 644, 684, 836);
        // Merchant
        init_color(43, 588, 231, 482);

        // Color initialization
        init_pair(0, 1, 0);
        for (unsigned char i = 1; i <= 43; i++) {
            init_pair(i, i, 0);
        }
    }

    // Various settings used in Catan
    struct {
        std::pair<unsigned char, unsigned char> boardDims = {11, 15};

        // true; Allows players to inspect others' resources/commodities
        // false; All players' resources/commodities are always hidden from each other
        bool openHand = false;

        // true;  Two roads (from two different players) can be placed on one tile border
        // false; Only one road can be placed on a tile border, as in normal Catan
        bool doubleRoads = false;

        // 0 = Full; Players may not place settlements on an intersection next to existing buildings (except for an outpost)
        // 1 = Half; Players can place settlements on intersections next to other players' buildings, but not next to their own (even outposts)
        // 2 = None; Players can place settlements onto any unoccupied intersection
        unsigned char distanceMode = 0;

        // Amount of players in the game
        unsigned char players = 2;

    } Settings;

    class Tile {
        private:
            // Location of the tile within the grid
            std::pair<unsigned char, unsigned char> Location = {0, 0};

            // -1 = No Tile, 0 = Sheep, 1 = Rock, 2 = Wood, 3 = Brick, 4 = Wheat, 5 = Gold, 6 = Water, 7 = Desert
            char Type = 7;

            // -1 = No Rarity, 2-12 (Excluding 7) = Rarity
            char Rarity = 0;

            // Indexes:
            // 0 = Robber, 1 = Pirate, 2 = Merchant
            bool Visitors[3] = {false, false, false};

            // For each border, there are two halves that a road/boat/etc can be placed (if the settings allow for it)
            // The borders are ordered from top to bottom, left to right
            //
            // For the first part: -1 = No Player, 0+ = Player ID
            //
            // For the second part: -1 = No building, 0 = Road, 1 = Boat, 2 = Path
            std::pair<char, char> Borders[6][2] = {
                {{-1, 0}, {-1, 0}}, {{-1, 0}, {-1, 0}}, {{-1, 0}, {-1, 0}},
                {{-1, 0}, {-1, 0}}, {{-1, 0}, {-1, 0}}, {{-1, 0}, {-1, 0}}
            };

            // For the first part of each intersection: -1 = No building, 0+ = Player ID
            //
            // For the second part of each intersection, there is one value to determine the type of building on it and another to add additional data
            //
            // First part of the second part: -1 = No building, 0 = Settlement, 1 = City, 2 = Metropolis, 3 = Knight, 4 = Outpost, 5 = Raiding Party
            // 
            // Second part of the second part:
            // Settlement: 0 = Settlement, 1 = Settlement w/ trench
            // City: 0 = City, 1 = City w/ walls
            // Metropolis: 0 = Metropolis, 1 = Metropolis w/ walls
            // Knight: 0 = Inactive Basic, 1 = Active Basic, 2 = Inactive Strong, 3 = Active Strong, 4 = Inactive Mighty, 5 = Active Mighty
            // Outpost: 0 = Outpost
            // Raiding Party: Level of RP - 1 (0 = lvl.1, 1 = lvl.2, etc)
            char Intersections[6][3] = {
                {-1, -1, 0},
                {-1, -1, 0},
                {-1, -1, 0},
                {-1, -1, 0},
                {-1, -1, 0},
                {-1, -1, 0}
            };

            // Tracks which harbors each intersection grants access to when built upon
            // Indexes: 0 = 2:1 Sheep, 1 = 2:1 Rock, 2 = 2:1 Wood, 3 = 2:1 Brick, 4 = 2:1 Wheat, 5 = 3:1 General
            bool Harbors[6][6] = {
                {false, false, false, false, false, false},
                {false, false, false, false, false, false},
                {false, false, false, false, false, false},
                {false, false, false, false, false, false},
                {false, false, false, false, false, false},
                {false, false, false, false, false, false}
            };
        
        public:
            Tile(std::pair<unsigned char, unsigned char> location, char type = 7, char rarity = 0) {
                if (location.first == 0 || location.first == 10 || location.second == 0 || location.second == 14) {
                    Type == -1;
                }
                else {
                    // Assign the new tile a type and rarity
                    Type = (type >= -1 && type <= 7) ? type : 7;
                    Rarity = ((rarity >= 2 && rarity <= 12 && rarity != 7) || rarity == 0) ? rarity : 0;
                }
                
                Location = location;
            }

            void render() {

            }
    };

    int start() {
        char intersectionTest[9][13] = {
            {-1, -1, -1,  7, -1, -1, -1,  7, -1,  7, -1,  7, -1},
            {-1,  7,  7,  7,  7,  7, -1,  7, -1, -1,  7,  7, -1},
            {-1,  7,  7,  7,  7,  7, -1,  7, -1, -1,  7, -1,  7},
            {-1,  7,  7,  7,  7,  7, -1,  7, -1, -1, -1, -1, -1},
            {-1, -1,  7,  7,  7, -1, -1, -1, -1, -1, -1,  7, -1},
            { 7, -1, -1, -1, -1, -1, -1,  7, -1, -1,  7,  7,  7},
            {-1,  7,  7, -1,  7, -1,  7, -1,  7, -1,  7,  7,  7},
            { 7,  7,  7, -1,  7, -1,  7,  7,  7, -1, -1,  7, -1},
            {-1, -1, -1,  7,  7,  7, -1, -1,  7,  7,  7,  7, -1}
        };

        char defaultBoard[9][13] = {
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1,  0,  2,  3, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1,  1,  3,  0,  2,  1, -1, -1, -1, -1},
            {-1, -1, -1, -1,  4,  2,  7,  4,  0, -1, -1, -1, -1},
            {-1, -1, -1, -1,  4,  2,  1,  4,  0, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
        };

        char defaultRarity[9][13] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 2, 9, 10, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 10, 6, 4, 3, 8, 0, 0, 0, 0},
            {0, 0, 0, 0, 2, 11, 0, 4, 5, 0, 0, 0, 0},
            {0, 0, 0, 0, 9, 8, 3, 6, 11, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };

        Board.dbox(0, 0, Board.gety(), Board.getx(), {LD_Heavy_Both, LD_Dashed_None}, 1);

        for (unsigned char i = 0; i < 11; i++) {
            Tiles.emplace_back();

            for (unsigned char j = 0; j < 15; j++) {
                Tiles[i].emplace_back(Tile({i, j}, rand() % 8, 2 + rand() % 11));
                // Tiles[i].emplace_back(tile({i, j}, (intersectionTest[i][j] != -1) ? rand() % 8 : -1, 2 + rand() % 11));
                // Tiles[i].emplace_back(tile({i, j}, defaultBoard[i][j], defaultRarity[i][j]));
            }
        }

        for (unsigned char i = 0; i < 11; i++) {
            for (unsigned char j = 0; j < 15; j++) {
                Tiles[i][j].render();
            }
        }

        Board.ri();
        Board.gch();

        return 0;
    }
}

int main() {
    startScreen();
    srand(time(NULL));

    Catan::initialize();
    Catan::start();

    endwin();
    return 0;
}
