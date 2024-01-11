#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

namespace Catan {
    // Various definitions used for clarity
    // Land/Water
    #define EARTH true
    #define WATER false

    // Geography
    #define RANGE 0
    #define PEAKS 1
    #define WOODS 2
    #define HILLS 3
    #define FIELD 4
    #define RIVER 5

    // Resources
    #define SHEEP 0
    #define ROCK  1
    #define WOOD  2
    #define BRICK 3
    #define WHEAT 4
    #define GOLD  5

    struct {
        // Determines whether players can view each other's resources/commodities
        bool openHand = false;

        // Determines if either one or two buildings can be placed on a border
        bool doubleBorder = false;

        // Determines the distance rule variation
        // 0 = Full, 1 = Half, 2 = None
        bool dRule = 0;

        // Amount of players in the game
        unsigned char players = 2;

        // Offset for the tiles from the top-left of the board screen (in characters)
        unsigned char oX = 5, oY = 2;

        // Size of the tile grid (largest should be 15x11)
        unsigned char dX = 15, dY = 11;
    } Config;

    class Tile;
    class Game;
    //class Harbor;

    BSCREEN S;

    std::vector<std::vector<Tile>> T;
    //std::vector<std::vector<Harbor>> H;

    class Tile {
        private:
            // Location of the tile
            unsigned char X = 0, Y = 0;

            // If tile is land or water
            bool L = EARTH;

            // What kind of geography the tile has (-1 for none)
            char G = -1;

            // Which number produces resources (-1 for none)
            char R = -1;

            // Whether a visitor is on the tile
            // Indexes: Robber, Pirate, Merchant
            std::vector<bool> V = {false, false, false};

            // Each side of the tile's border has two spots for structures
            // Order: tr, t, tl, bl, b, br; inside, outside
            //
            // .first (Player ID): -1 = None, 0-8 = ID
            // .second (Building ID): -1 = None, 0 = Path, 1 = Road, 2 = Boat
            std::vector<std::vector<std::pair<char, char>>> B = {
                {{-1, 0}, {-1, 0}}, {{-1, 0}, {-1, 0}}, {{-1, 0}, {-1, 0}},
                {{-1, 0}, {-1, 0}}, {{-1, 0}, {-1, 0}}, {{-1, 0}, {-1, 0}}
            };

            // For each intersection there is a player ID, an Object (building/military) ID, and a level for the object:
            // i = 0 (Player ID): -1 = None, 0-8 = ID
            //
            // i = 1 (Object ID): -1 = None, 0 = Outpost, 1 = Settlement, 2 = City, 3 = Metropolis, 4 = Knight, 5 = Raiding Party
            //
            // i = 2 (Object Level):
            // Outpost: 0 = Outpost
            // Settlement: 0 = Settlement, 1 = Settlement + Trench
            // City: 0 = City, 1 = City + Walls
            // Metropolis: 0 = Metropolis, 1 = Metropolis + Walls
            // Knight: 0-2 is inactive, 3-5 is active; Increments by strength
            // Raiding Party: 0 = lvl 1 ... 6 = lvl 7
            std::vector<std::vector<char>> I = {
                {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0},
                {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}
            };

        public:
            Tile(unsigned char x, unsigned char y, char l, char g, char r) {
                // Automatically make the border tiles of Catan water tiles
                if (x == 0 || y == 0 || x == Config.dX - 1 || y == Config.dY - 1) {
                    L = WATER;
                    G = -1;
                    R = -1;
                } else {
                    L = l;
                    G = g;
                    R = r;
                }

                X = x;
                Y = y;
            }

            void brender() {
                // Find top-left corner of the tile

                unsigned char x = Config.oX + X * 10;
                unsigned char y = Config.oY + ((X % 2 == 0) ? 0 : 3) + Y * 6;

                std::string temp[8][14] = {
                    {" ", " ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " ", " "},
                    {" ", " ", "╱", "╶", "─", "─", "─", "─", "─", "─", "╴", "╲", " ", " "},
                    {" ", "╱", "╱", " ", " ", " ", " ", " ", " ", " ", " ", "╲", "╲", " "},
                    {"╴", "╱", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "╲", "╶"},
                    {"╴", "╲", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "╱", "╶"},
                    {" ", "╲", "╲", " ", " ", " ", " ", " ", " ", " ", " ", "╱", "╱", " "},
                    {" ", " ", "╲", "╶", "─", "─", "─", "─", "─", "─", "╴", "╱", " ", " "},
                    {" ", " ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " ", " "}
                };

                for (unsigned char i = 0; i < 8; i++) {
                    for (unsigned char j = 0; j < 14; j++) {
                        S.wu(y + i, x + j, temp[i][j]);
                    }
                }

                // Display the coordinates of the tile, for debugging
                S.wint(y + 2, x + 4, X, 2, 1, true);
                S.wint(y + 2, x + 8, Y, 2, 1, true);

                if (G != -1) {
                    std::string geo[6] = {"Range", "Peaks", "Woods", "Hills", "Field", "River"};

                    S.wstr(y + 3, x + 3, "G " + geo[G]);
                    S.wstr(y + 4, x + 3, "R ");
                    S.wint(y + 4, x + 5, R, 2);
                }
                else {
                    S.wstr(y + 3, x + 3, "G None");
                    S.wstr(y + 4, x + 3, "R None");
                }
                S.wstr(y + 5, x + 4, (L == true) ? "Earth" : "Water");
            }
    };

    class Game {
        public:
            Game() {}

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

                S.dbox();

                for (unsigned char i = 0; i < Config.dX; i++) {
                    T.emplace_back();

                    for (unsigned char j = 0; j < Config.dY; j++) {
                        T[i].emplace_back(Tile(i, j, EARTH, rand() % 6, 8));
                        // Tiles[i].emplace_back(tile({i, j}, (intersectionTest[i][j] != -1) ? rand() % 8 : -1, 2 + rand() % 11));
                        // Tiles[i].emplace_back(tile({i, j}, defaultBoard[i][j], defaultRarity[i][j]));
                    }
                }

                for (unsigned char i = 0; i < Config.dX; i++) {
                    for (unsigned char j = 0; j < Config.dY; j++) {
                        T[i][j].brender();
                    }
                }

                // T.emplace_back();
                // T[0].emplace_back(Tile(1, 1, EARTH, 0, 0));
                // T[0][0].brender();
                
                S.ri();
                S.gch();

                return 0;
            }

            void initialize() {
                // Board screen initialization
                S = BSCREEN(75, 164, LINES / 2 - 38, COLS / 2 - 82);

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
    };
}

int main() {
    startScreen();
    srand(time(NULL));

    Catan::Game c;
    c.initialize();
    c.start();

    endwin();
    return 0;
}
