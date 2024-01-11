#ifndef CATAN_GAME
#define CATAN_GAME

#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

namespace Catan {
    class tile;
    class harbor;

    BSCREEN Board;
    // 11 tiles tall and 15 tiles wide
    std::vector<std::vector<tile>> Tiles;
    // 11 tiles tall and 15 tiles wide
    std::vector<std::vector<harbor>> Harbors;

    // Initialize colors and pairs needed for catan as well as the various screens
    void initialize() {
        // Board screen initialization
        Board = BSCREEN(75, 164, LINES / 2 - 38, COLS / 2 - 82);

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

    class tile {
        private:
            // Location of the tile within the grid
            std::pair<unsigned char, unsigned char> Location = {0, 0};

            // -1 = No Tile, 0 = Sheep, 1 = Rock, 2 = Wood, 3 = Brick, 4 = Wheat, 5 = Gold, 6 = Water, 7 = Desert
            char Type = 7;

            // -1 = No Rarity, 2-12 (Excluding 7) = Rarity
            char Rarity = 0;

            // First = Robber/Pirate, Second = Merchant
            std::pair<bool, bool> Visitors = {false, false};

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
            std::pair<char, std::pair<char, char>> Intersections[6] = {
                {-1, {-1, 0}},
                {-1, {-1, 0}},
                {-1, {-1, 0}},
                {-1, {-1, 0}},
                {-1, {-1, 0}},
                {-1, {-1, 0}}
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
            tile(std::pair<unsigned char, unsigned char> location, char type = 7, char rarity = 0) {
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

            const char getType() {return Type;}
            const char checkForBuilding(unsigned char intersection) {return (Intersections[intersection].first != -1 && Intersections[intersection].second.first < 3) ? Intersections[intersection].first : -1;}

            void temp(unsigned char intersection, char player, char type, char attribute = 0) {
                Intersections[intersection].first = player;
                Intersections[intersection].second.first = type;
                Intersections[intersection].second.second = attribute;
            }

            void render() {
                // Find the top-left corner of the tile
                std::pair<unsigned char, unsigned char> origin = {((Location.second % 2 == 0) ? 3 : 6) + Location.first * 6, 5 + Location.second * 10};

                // Display the coordinates of the tile, for debugging
                Board.wint(origin.first, origin.second + 6, Location.first, 2, 1, true);
                Board.wint(origin.first + 1, origin.second + 6, Location.second, 2, 1, true);

                // Empty tiles shall not be rendered
                if (Type == -1) {
                    return;
                }

                // Render the borders (roads/boats/pathes)
                const std::string borderPieces[3][3] = {{"─", "┼", "╌"}, {"╲", "╳", "\\"}, {"╱", "╳", "/"}};
                // Top
                for (unsigned char i = 0; i < 2; i++) {
                    for (unsigned char j = 0; j < 6; j++) {
                        if (Borders[0][i].first != -1) {
                            Board.wu(origin.first + i, origin.second + 4 + j, borderPieces[0][Borders[0][i].second], 8 + Borders[0][i].first);
                        }
                        else {
                            Board.wu(origin.first + i, origin.second + 4 + j, "─", 32);
                        }
                    }
                }
                // Diagonals
                for (unsigned char i = 0; i < 4; i++) {
                    for (unsigned char j = 0; j < 2; j++) {
                        if (Borders[i + 1][j].first == -1) {
                            Board.wu(origin.first + 2 + ((i > 1) ? 3 : 0), origin.second + 1 + ((i % 2 != 0) ? 11: 0) + ((i % 2 == 0) ? j : -j), (i < 1 || i > 2) ? "╱" : "╲", 32);
                        }
                        else {
                            Board.wu(origin.first + 2 + ((i > 1) ? 3 : 0), origin.second + 1 + ((i % 2 != 0) ? 11: 0) + ((i % 2 == 0) ? j : -j), borderPieces[(i < 1 || i > 2) ? 2 : 1][Borders[i + 1][j].second], 8 + Borders[i + 1][j].first);
                        }
                    }
                }
                // Bottom
                for (unsigned char i = 0; i < 2; i++) {
                    for (unsigned char j = 0; j < 6; j++) {
                        if (Borders[5][i].first != -1) {
                            Board.wu(origin.first + 7 - i, origin.second + 4 + j, borderPieces[0][Borders[5][i].second], 8 + Borders[5][i].first);
                        }
                        else {
                            Board.wu(origin.first + 7 - i, origin.second + 4 + j, "─", 32);
                        }
                    }
                }

                // Render the intersections (settlements/cities/etc)
                // For each building: {Top Left, Top Right, Bottom Left, Bottom Right}
                const std::string intersectionArt[6][2][4] = {
                    {{"▗", "▖", "▀", "▀"}, {"▗", "▖", "█", "█"}}, // Settlement
                    {{"▖", "▌", "▀", "▀"}, {"▖", "▌", "█", "█"}},  // City w/ walls
                    {{"▙", "▟", "▀", "▀"}, {"▙", "▟", "█", "█"}}, // Metropolis w/ walls
                    {{"▐", "▌", "0", "0"}},                          // Knight
                    {{"", "▖", "▝", "▝"}},                         // Outpost
                    {{"▚", "▚", "0", "0"}}                         // Raiding Party
                };

                // For each intersection, the first set is used when the tile has none next to it and the second is used when there is one
                const std::string intersectionFiller[6][2][4] = {
                    {{" ", "╶", "╱", "╶"}, {"╲", "╶", "╱", "╶"}},
                    {{"╴", " ", "╴", "╲"}, {"╴", "╱", "╴", "╲"}},
                    {{"╱", "╱", "╲", "╲"}, {"╴", "╱", "╴", "╲"}},
                    {{"╲", "╲", "╱", "╱"}, {"╲", "╶", "╱", "╶"}},
                    {{"╲", "╶", " ", "╶"}, {"╲", "╶", "╱", "╶"}},
                    {{"╴", "╱", "╴", " "}, {"╴", "╱", "╴", "╲"}}
                };

                for (unsigned char i = 0; i < 6; i++) {
                    std::pair<unsigned char, unsigned char> offset = {0, 2};

                    // Offset each intersection relative to the origin of the tile
                    switch (i) {
                        case 1:
                            offset = {0, 10};
                            break;
                        case 2:
                            offset = {3, 0};
                            break;
                        case 3:
                            offset = {3, 12};
                            break;
                        case 4:
                            offset = {6, 2};
                            break;
                        case 5:
                            offset = {6, 10};
                            break;
                    }
                    
                    if (Intersections[i].first == -1) {
                        // If either is true, then the second art set is used
                        bool upTile = true, downTile = true;

                        // If any tiles are found to be around the current one, then the art set used is changed
                        switch (i) {
                            case 0:
                            case 1:
                                if (Location.first == 0) {
                                    upTile = false;
                                    downTile = false;
                                    break;
                                }

                                if (Location.second % 2 != 0) {
                                    if (Location.first == 1) {
                                        upTile = false;
                                    }
                                    else if (Tiles[Location.first - 1][Location.second].getType() == -1) {
                                        upTile = false;
                                    }

                                    if ((i == 0 && Location.second == 0) || (i == 1 && Location.second == 14)) {
                                        downTile = false;
                                    }
                                    else if (Tiles[Location.first - 1][Location.second + ((i == 0) ? -1 : 1)].getType() == -1) {
                                        downTile = false;
                                    }
                                }
                                else {
                                    if (Tiles[Location.first - 1][Location.second].getType() == -1) {
                                        upTile = false;
                                    }

                                    if (Tiles[Location.first][Location.second + ((i == 0) ? -1 : 1)].getType() == -1) {
                                        downTile = false;
                                    }
                                }
                                break;
                            case 2:
                            case 3:
                                if ((i == 2 && Location.second == 0) || (i == 3 && Location.second == 14)) {
                                    upTile = false;
                                    downTile = false;
                                    break;
                                }

                                if (Location.second % 2 != 0) {
                                    if (Tiles[Location.first - 1][Location.second + ((i == 2) ? -1 : 1)].getType() == -1) {
                                        upTile = false;
                                    }

                                    if (Tiles[Location.first][Location.second + ((i == 2) ? -1 : 1)].getType() == -1) {
                                        downTile = false;
                                    }
                                }
                                else {
                                    if (Location.first == 0) {
                                        upTile = false;
                                    }
                                    else if (Tiles[Location.first][Location.second + ((i == 2) ? -1 : 1)].getType() == -1) {
                                        upTile = false;
                                    }

                                    if (Location.first == 10) {
                                        downTile = false;
                                    }
                                    else if (Tiles[Location.first + 1][Location.second + ((i == 2) ? -1 : 1)].getType() == -1) {
                                        downTile = false;
                                    }
                                }
                                break;
                            case 4:
                            case 5:
                                if (Location.first == 10 && Location.second % 2 == 0) {
                                    upTile = false;
                                    downTile = false;
                                    break;
                                }

                                if (Location.second % 2 != 0) {
                                    if ((i == 4 && Location.second == 0) || (i == 5 && Location.second == 14)) {
                                        upTile = false;
                                    }
                                    else if (Tiles[Location.first][Location.second + ((i == 4) ? -1 : 1)].getType() == -1) {
                                        upTile = false;
                                    }

                                    if (Location.first == 10) {
                                        downTile = false;
                                    }
                                    else if (Tiles[Location.first + 1][Location.second].getType() == -1) {
                                        downTile = false;
                                    }
                                }
                                else {
                                    if (Tiles[Location.first + 1][Location.second + ((i == 4) ? -1 : 1)].getType() == -1) {
                                        upTile = false;
                                    }

                                    if (Tiles[Location.first + 1][Location.second].getType() == -1) {
                                        downTile = false;
                                    }
                                }
                                break;
                        }

                        Board.wu(origin.first + offset.first, origin.second + offset.second, intersectionFiller[i][((upTile || downTile) ? 1 : 0)][0], 32);
                        Board.wu(origin.first + offset.first, origin.second + offset.second + 1, intersectionFiller[i][((upTile || downTile) ? 1 : 0)][1], 32);
                        Board.wu(origin.first + offset.first + 1, origin.second + offset.second, intersectionFiller[i][((upTile || downTile) ? 1 : 0)][2], 32);
                        Board.wu(origin.first + offset.first + 1, origin.second + offset.second + 1, intersectionFiller[i][((upTile || downTile) ? 1 : 0)][3], 32);
                    }
                    else {
                        std::pair<unsigned char, unsigned char> artIndex = {Intersections[i].second.first, ((Intersections[i].second.first < 3) ? Intersections[i].second.second : 0)};

                        Board.wu(origin.first + offset.first, origin.second + offset.second, intersectionArt[artIndex.first][artIndex.second][0], 8 + Intersections[i].first);
                        Board.wu(origin.first + offset.first, origin.second + offset.second + 1, intersectionArt[artIndex.first][artIndex.second][1], 8 + Intersections[i].first);
                        Board.wu(origin.first + offset.first + 1, origin.second + offset.second, intersectionArt[artIndex.first][artIndex.second][2], 8 + Intersections[i].first);
                        Board.wu(origin.first + offset.first + 1, origin.second + offset.second + 1, intersectionArt[artIndex.first][artIndex.second][3], 8 + Intersections[i].first);
                    
                        if (Intersections[i].second.first == 3 || Intersections[i].second.first == 5) {
                            Board.wint(origin.first + offset.first + 1, origin.second + offset.second, Intersections[i].second.second, 2, 8 + Intersections[i].first, true);
                        }
                    }
                }

                // Render the type art for the tile
                const std::string typeArt[8][4][8] = {
                    {
                        {" ", "⊼", "°", " ", "~", "~", " ", " "},
                        {" ", "~", " ", " ", " ", " ", "⊼", "°"},
                        {"~", " ", " ", " ", " ", " ", " ", "~"},
                        {" ", "°", "⊼", " ", "~", "°", "⊼", " "}
                        
                    }, {
                        {" ", "╱", "╲", " ", "_", "╱", "╲", " "},
                        {"⋀", " ", " ", " ", " ", " ", "⋀", "╲"},
                        {" ", "╱", " ", " ", " ", " ", "_", " "},
                        {"╱", "⋀", " ", "╲", "_", " ", "╱", "╲"}
                    }, {
                        {" ", "↟", "^", "↟", "⇈", "^", " ", " "},
                        {"⇈", "↟", " ", " ", " ", " ", "⇈", "↟"},
                        {"↟", "⇈", " ", " ", " ", " ", "↟", "⇈"},
                        {" ", "⇈", "↟", "^", "^", "⇈", "↟", " "}
                        
                    }, {
                        {" ", "▭", "▱", "◫", "◻", "▭", " ", "▱"},
                        {"▭", "◻", " ", " ", " ", " ", "▭", "▱"},
                        {"◫", "▭", " ", " ", " ", " ", "◻", "◫"},
                        {" ", "▱", "◻", "◫", "◻", "▱", " ", " "}
                    }, {
                        {"⌵", "^", "⋎", "w", "⌵", "⋎", "⋎", " "},
                        {"^", "⋎", " ", " ", " ", " ", "w", "⋎"},
                        {"⋎", "⌵", " ", " ", " ", " ", "⋎", "⌵"},
                        {"v", "⋎", "⌵", "⌵", "⋎", "^", "^", "w"}
                    }, {
                        {"╲", "~", "╱", " ", "◊", "⚬", "╲", " "},
                        {" ", "▏", " ", " ", " ", " ", " ", "⚬"},
                        {"⚬", "╲", " ", " ", " ", " ", "╱", "◊"},
                        {" ", "╱", "~", "╲", "◊", "⚬", " ", " "}
                    }, {
                        {" ", "~", " ", "≈", "≈", "≈", "~", " "},
                        {"≈", "~", " ", "~", " ", "≈", " ", "~"},
                        {" ", "~", "≈", " ", "~", " ", "~", "≈"},
                        {"≈", " ", "≈", "~", " ", "~", " ", " "}
                    }, {
                        {" ", "Y", " ", "~", "~", " ", " ", " "},
                        {"~", " ", "~", " ", "Y", " ", "~", "Y"},
                        {"Y", "~", " ", "~", " ", " ", "Y", " "},
                        {" ", " ", "~", "Y", " ", "~", " ", " "}
                    }
                };
                const unsigned char typeColors[8][4][8] = {
                    {
                        {0, 1, 1, 0, 2, 2, 0, 0}, //  ⊼° ~~  
                        {0, 2, 0, 0, 0, 0, 1, 1}, //  ~╭06╮⊼°
                        {2, 0, 0, 0, 0, 0, 0, 2}, // ~ ╰──╯ ~
                        {0, 1, 1, 0, 2, 1, 1, 0}  //  °⊼ ~°⊼ 
                        
                    }, {
                        {0, 2, 2, 0, 1, 1, 2, 0}, //  ╱╲ _╱╲ 
                        {1, 0, 0, 0, 0, 0, 1, 2}, // ⋀ ╭06╮⋀╲
                        {0, 1, 0, 0, 0, 0, 2, 0}, //  ╱╰──╯_ 
                        {1, 1, 0, 1, 1, 0, 1, 2}  // ╱⋀ ╲_ ╱╲
                    }, {
                        {0, 1, 2, 2, 1, 1, 0, 0}, //  ↟^↟⇈^  
                        {2, 1, 0, 0, 0, 0, 1, 2}, // ⇈↟╭06╮⇈↟
                        {1, 2, 0, 0, 0, 0, 2, 1}, // ↟⇈╰──╯↟⇈
                        {0, 1, 1, 2, 2, 1, 2, 0}  //  ⇈↟^^⇈↟ 
                        
                    }, {
                        {0, 1, 2, 1, 1, 2, 0, 2}, //  ▭▱◫◻▭ ▱
                        {1, 1, 0, 0, 0, 0, 2, 1}, // ▭◻╭06╮▭▱
                        {2, 2, 0, 0, 0, 0, 1, 2}, // ◫▭╰──╯◻◫
                        {0, 1, 1, 2, 2, 1, 2, 0}  //  ▱◻◫◻▱  
                        
                    }, {
                        {1, 1, 2, 1, 1, 2, 2, 0}, // ⌵^⋎w⌵⋎⋎ 
                        {1, 2, 0, 0, 0, 0, 1, 1}, // ^⋎╭06╮w⋎
                        {2, 1, 0, 0, 0, 0, 2, 2}, // ⋎⌵╰──╯⋎⌵
                        {1, 1, 2, 2, 1, 2, 1, 1}  // v⋎⌵⌵⋎^^w
                    }, {
                        {1, 2, 1, 0, 1, 2, 1, 0}, // ╲~╱ ◊⚬╲ 
                        {0, 1, 0, 0, 0, 0, 0, 1}, //  ▏╭06╮ ⚬
                        {2, 1, 0, 0, 0, 0, 1, 1}, // ⚬╲╰──╯╱◊
                        {0, 1, 2, 1, 1, 2, 0, 0}  //  ╱~╲◊⚬  
                    }, {
                        {0, 1, 0, 2, 2, 1, 1, 0}, //  ~ ≈≈≈~ 
                        {2, 1, 0, 1, 0, 2, 0, 2}, // ≈~ ~ ≈ ~
                        {0, 2, 1, 0, 2, 0, 1, 2}, //  ~≈ ~ ~≈
                        {1, 0, 2, 1, 0, 2, 0, 0}  // ≈ ≈~ ~  
                    }, {
                        {0, 1, 0, 1, 2, 0, 0, 0}, //  Y ~~   
                        {2, 0, 2, 0, 1, 0, 2, 1}, // ~ ~ Y ~Y
                        {1, 1, 0, 1, 0, 0, 1, 0}, // Y~ ~  Y 
                        {0, 0, 2, 1, 0, 2, 0, 0}  //   ~Y ~  
                    }
                };

                for (unsigned char i = 0; i < 4; i++) {
                    for (unsigned char j = 0; j < 8; j++) {
                        Board.wu(origin.first + 2 + i, origin.second + 3 + j, typeArt[Type][i][j], (typeColors[Type][i][j] == 0) ? 1 : typeColors[Type][i][j] + 17 + Type * 2);
                    }
                }
            
                // Render the rarity/visitor indicators for the tile
                if (Type != 6 && Type != 7) {
                    // Decoration
                    Board.wu(origin.first + 3, origin.second + 5, "╭", 32);
                    Board.wu(origin.first + 3, origin.second + 8, "╮", 32);
                    Board.wu(origin.first + 4, origin.second + 5, "╰", 32);
                    Board.wu(origin.first + 4, origin.second + 8, "╯", 32);

                    // Actual rarity of the tile
                    Board.wint(origin.first + 3, origin.second + 6, Rarity, 2, (Rarity == 6 || Rarity == 8) ? 17 : 16, true);

                    // Visitors on the tile (or more decoration if there isn't one)
                    Board.wu(origin.first + 4, origin.second + 6, (Visitors.first == false) ? "─" : "☠", (Visitors.first == false) ? 32 : 42);
                    Board.wu(origin.first + 4, origin.second + 7, (Visitors.second == false) ? "─" : "♟", (Visitors.second == false) ? 32 : 43);
                }
                else if (Visitors.first == true) {
                    // Decoration
                    Board.wu(origin.first + 3, origin.second + 5, " ", 32);
                    Board.wu(origin.first + 3, origin.second + 8, " ", 32);
                    Board.wu(origin.first + 4, origin.second + 5, " ", 32);
                    Board.wu(origin.first + 4, origin.second + 8, " ", 32);

                    if (Type == 6) {
                        // Render the pirate
                        Board.wu(origin.first + 3, origin.second + 6, "◀", 42);
                        Board.wu(origin.first + 3, origin.second + 7, "▍", 42);
                        Board.wu(origin.first + 4, origin.second + 6, "▜", 42);
                        Board.wu(origin.first + 4, origin.second + 7, "▛", 42);
                    }
                    else {
                        // Render the robber
                        Board.wu(origin.first + 3, origin.second + 6, "▟", 42);
                        Board.wu(origin.first + 3, origin.second + 7, "▙", 42);
                        Board.wu(origin.first + 4, origin.second + 6, "▀", 42);
                        Board.wu(origin.first + 4, origin.second + 7, "▀", 42);
                    }
                }
            
                // Display the coordinates of the tile, for debugging
                Board.wint(origin.first, origin.second + 6, Location.first, 2, 1, true);
                Board.wint(origin.first + 1, origin.second + 6, Location.second, 2, 1, true);
            }

            // During map creation, the initial placement phase, and the inventor progress card, numbers rating the value of each tile are displayed
            // These numbers are not displayed during regular play because of visual clutter and because they really don't need to be
            void renderAltRarity() {
                // Empty tiles shall not be rendered
                if (Type == -1) {
                    return;
                }

                // Find the top-left corner of the tile
                std::pair<unsigned char, unsigned char> origin = {((Location.second % 2 == 0) ? 0 : 3) + Location.first * 6, 5 + Location.second * 10};

                // Decoration
                Board.wu(origin.first + 3, origin.second + 5, "╭", 32);
                Board.wu(origin.first + 3, origin.second + 8, "╮", 32);
                Board.wu(origin.first + 4, origin.second + 5, "╰", 32);
                Board.wu(origin.first + 4, origin.second + 8, "╯", 32);

                // Actual rarity of the tile
                Board.wint(origin.first + 3, origin.second + 6, Rarity, 2, (Rarity == 6 || Rarity == 8) ? 17 : 16, true);

                // Value based on how statistically common each rarity is to be rolled
                unsigned char rarity = 5;
                switch (Rarity) {
                    case 2:
                    case 12:
                        rarity = 1;
                        break;
                    case 3:
                    case 11:
                        rarity = 2;
                        break;
                    case 4:
                    case 10:
                        rarity = 3;
                        break;
                    case 5:
                    case 9:
                        rarity = 4;
                        break;
                }

                Board.wint(origin.first + 4, origin.second + 6, rarity, 2, (Rarity == 6 || Rarity == 8) ? 17 : 16, true);
            }
    };

    class harbor {
        private:
            // Location of the harbor within the grid
            std::pair<unsigned char, unsigned char> Location = {0, 0};

            // Tracks which trades each side of a harbor tile would have
            // -1 = No Trade, 0 = 2:1 Sheep, 1 = 2:1 Rock, 2 = 2:1 Wood, 3 = 2:1 Brick, 4 = 2:1 Wheat, 5 = 3:1 General
            char Sides[6] = {-1, -1, -1, -1, -1, -1};

            // Tracks which players have built a tradable building on an intersection
            // -1 = No Player, 0+ = Player ID
            char Players[6] = {-1, -1, -1, -1, -1, -1};

        public:
            harbor(std::pair<unsigned char, unsigned char> location) {
                Location = location;
            }

            // location: 0 = Top, 1 = Top Left, 2 = Top Right, 3 = Bottom Left, 4 = Bottom Right, 5 = Bottom
            // type: -1 = No trade, 0 = 2:1 Sheep, 1 = 2:1 Rock, 2 = 2:1 Wood, 3 = 2:1 Brick, 4 = 2:1 Wheat, 5 = 3:1 General
            void changeTrade(unsigned char location, char type) {
                if (location < 0 || location > 5 || type < -1 || type > 5) {return;}

                if (location == 0) {

                }
                else if (location == 1) {

                }
                else if (location == 2) {

                }
                else if (location == 3) {

                }
                else if (location == 4) {

                }
                else if (location == 5) {

                }

                Sides[location] = type;
            }

            void update() {
                if (Sides[0] != -1 && Location.first > 1) {

                }
            }

            void render() {
                // Find the top-left corner of the tile
                std::pair<short, short> origin = {((Location.second % 2 == 0) ? -3 : 0) + Location.first * 6 - 6, 5 + Location.second * 10 - 10};

                if (Sides[0] != -1) {
                    Board.wu(origin.first + 2, origin.second + 4, "\\", ((Players[0] == -1) ? 32 : 8 + Players[0]));
                    Board.wu(origin.first + 2, origin.second + 9, "/", ((Players[1] == -1) ? 32 : 8 + Players[1]));
                    Board.wstr(origin.first + 2, origin.second + 5, (Sides[0] < 5) ? "2->1" : "3->1", (Sides[0] < 5) ? 18 + Sides[0] * 2 : 34, true);
                }
                if (Sides[1] != -1) {
                    Board.wu(origin.first + 2, origin.second + 3, "\\", ((Players[0] == -1) ? 32 : 8 + Players[0]));
                    Board.wu(origin.first + 3, origin.second + 2, "-", ((Players[2] == -1) ? 32 : 8 + Players[1]));
                    Board.wstr(origin.first + 3, origin.second + 3, (Sides[1] < 5) ? "2:1" : "3:1", (Sides[1] < 5) ? 18 + Sides[1] * 2 : 34, true);
                }
                if (Sides[2] != -1) {
                    Board.wu(origin.first + 2, origin.second + 10, "/", ((Players[1] == -1) ? 32 : 8 + Players[0]));
                    Board.wu(origin.first + 3, origin.second + 11, "-", ((Players[3] == -1) ? 32 : 8 + Players[1]));
                    Board.wstr(origin.first + 3, origin.second + 8, (Sides[2] < 5) ? "2:1" : "3:1", (Sides[2] < 5) ? 18 + Sides[2] * 2 : 34, true);
                }
                if (Sides[3] != -1) {
                    Board.wu(origin.first + 4, origin.second + 2, "-", ((Players[2] == -1) ? 32 : 8 + Players[1]));
                    Board.wu(origin.first + 5, origin.second + 3, "/", ((Players[4] == -1) ? 32 : 8 + Players[0]));
                    Board.wstr(origin.first + 4, origin.second + 3, (Sides[3] < 5) ? "2:1" : "3:1", (Sides[3] < 5) ? 18 + Sides[3] * 2 : 34, true);
                }
                if (Sides[4] != -1) {
                    Board.wu(origin.first + 4, origin.second + 11, "-", ((Players[3] == -1) ? 32 : 8 + Players[1]));
                    Board.wu(origin.first + 5, origin.second + 10, "\\", ((Players[5] == -1) ? 32 : 8 + Players[0]));
                    Board.wstr(origin.first + 4, origin.second + 8, (Sides[4] < 5) ? "2:1" : "3:1", (Sides[4] < 5) ? 18 + Sides[4] * 2 : 34, true);
                }
                if (Sides[5] != -1) {
                    Board.wu(origin.first + 5, origin.second + 4, "/", ((Players[4] == -1) ? 32 : 8 + Players[0]));
                    Board.wu(origin.first + 5, origin.second + 9, "\\", ((Players[5] == -1) ? 32 : 8 + Players[1]));
                    Board.wstr(origin.first + 5, origin.second + 5, (Sides[5] < 5) ? "2->1" : "3->1", (Sides[5] < 5) ? 18 + Sides[5] * 2 : 34, true);
                }
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
                Tiles[i].emplace_back(tile({i, j}, rand() % 8, 2 + rand() % 11));
                // Tiles[i].emplace_back(tile({i, j}, (intersectionTest[i][j] != -1) ? rand() % 8 : -1, 2 + rand() % 11));
                // Tiles[i].emplace_back(tile({i, j}, defaultBoard[i][j], defaultRarity[i][j]));
            }
        }

        Tiles[5][7].temp(4, 0, 0);
        Tiles[6][6].temp(3, 0, 0);

        Tiles[5][7].temp(5, 1, 0);
        Tiles[6][8].temp(2, 1, 0);

        Tiles[6][8].temp(4, 2, 0);
        Tiles[7][8].temp(0, 2, 0);

        Tiles[7][8].temp(2, 3, 0);
        Tiles[7][7].temp(1, 3, 0);

        Tiles[7][7].temp(0, 4, 0);
        Tiles[7][6].temp(3, 4, 0);

        Tiles[7][6].temp(1, 5, 0);
        Tiles[6][6].temp(5, 5, 0);

        Tiles[6][7].temp(0, 0, 0);
        Tiles[6][7].temp(1, 1, 0);
        Tiles[6][7].temp(2, 5, 0);
        Tiles[6][7].temp(3, 4, 0);
        Tiles[6][7].temp(4, 2, 0);
        Tiles[6][7].temp(5, 3, 0);

        for (unsigned char i = 0; i < 11; i++) {
            for (unsigned char j = 0; j < 15; j++) {
                Tiles[i][j].render();
            }
        }

        for (unsigned char i = 0; i < 11; i++) {
            Harbors.emplace_back();

            for (unsigned char j = 0; j < 15; j++) {
                Harbors[i].emplace_back(harbor({i, j}));
            }
        }

        Harbors[8][8].changeTrade(0, 0);
        Harbors[8][8].changeTrade(1, 1);
        Harbors[8][8].changeTrade(2, 2);
        Harbors[8][8].changeTrade(3, 3);
        Harbors[8][8].changeTrade(4, 4);
        Harbors[8][8].changeTrade(5, 5);
        Harbors[8][8].update();
        Harbors[8][8].render();
        
        Board.ri();
        Board.gch();

        return 0;
    }
}

#endif

int main() {
    startScreen();
    srand(time(NULL));

    Catan::initialize();
    Catan::start();

    endwin();
    return 0;
}
