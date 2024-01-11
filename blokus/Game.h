#ifndef BLOKUS_GAME
#define BLOKUS_GAME
#include </home/bearslay/Bearslay-Launcher/blokus/Player.h>

namespace Blokus {
    class Game {
        private:
            BSCREEN Window, Board, Overlay, Preview, View, Action;
            std::vector<Blokus::Player> Players;

            // Current Player's Turn
            unsigned char Turn = rand() % Vals.players;
            // Game Round
            unsigned char Round = 1;
            // Total tiles used on the board
            unsigned short TotalTiles = 0;

            unsigned char Lx = 0;
            unsigned char Ly = 0;

            // For each cell: 1 = Player 1, 2 = PLayer 2, 3 = Player 3, 4 = Player 4
            std::vector<std::vector<unsigned char>> Tiles;

            // Visually update the screen with any changes, cells that do not change during the course of the game are not adjusted here
            void render() {
                // Put elsewhere
                TotalTiles = 0;
                for (unsigned char i = 0; i < Vals.bdim; i++) {
                    for (unsigned char j = 0; j < Vals.bdim; j++) {
                        TotalTiles += (Tiles[i][j] > 0) ? 1 : 0;
                    }
                }

                // main window
                // Main window header
                Window.wint(0, 30, Turn + 1, 1, 8 + Turn, true);
                Window.wint(0, 43, Round, (Vals.sHex > 0 || Vals.sHept > 0) ? 3 : 2, 12, true);

                // Board header
                Window.wint(1, 24, TotalTiles, ((Vals.bdim == 100) ? 5 : ((Vals.bdim > 31) ? 4 : 3)), 12, true);
                Window.wint(1, 29 + ((Vals.bdim == 100) ? 10 : ((Vals.bdim > 31) ? 8 : 6)), TotalTiles / pow(Vals.bdim, 2) * 100, 3, 12, true);

                // Pieces on the board
                for (unsigned char i = 0; i < Vals.bdim; i++) {
                    for (unsigned char j = 0; j < Vals.bdim; j++) {
                        if (Tiles[i][j] > 0) {
                            Board.wu(i, j * 2, "▓", Tiles[i][j]);
                            Board.wu(i, j * 2 + 1, "▓", Tiles[i][j]);
                        }
                    }
                }

                // Action window
                // Update cursor location
                for (unsigned char i = 0; i < 4; i++) {Action.wu(1 + i, 2, " ");}
                if (Lx == 0) {Action.wu(1 + Ly, 2, "≫", 8 + Turn);}

                // View window
                View.wstr(1, 2, "  Player 1", (Turn == 0) ? 12 : 8);
                View.wstr(2, 2, "  Player 2", (Turn == 1) ? 12 : 9);
                View.wstr(3, 2, "  Player 3", (Turn == 2 || Vals.players < 3) ? 12 : 10);
                View.wstr(4, 2, "  Player 4", (Turn == 3 || Vals.players < 4) ? 12 : 11);
                // Update cursor location
                if (Lx == 1) {View.wu(1 + Ly, 2, "≫", 8 + Turn);}

                Window.ri();
                Board.ri();
                Action.ri();
                View.ri();
                Preview.ri();

                // Render players
                for (unsigned char i = 0; i < Vals.players; i++) {
                    Players[i].render();
                }
            }

            // Fully reset all windows and then re-render them
            void fullRender() {
                // Reset screens
                Window.reset();
                Board.reset();
                Preview.reset();
                View.reset();
                Action.reset();

                // Draw the correct boxes
                Window.dbox({LD_Heavy_Both, LD_Dashed_None});
                Window.dbox(1, 2, Board.gety() + 2, Board.getx() + 2, {LD_Heavy_Both, LD_Dashed_None});
                Preview.dbox();
                View.dbox();
                Action.dbox();

                // Main window header
                Window.wa(0, 2, {{{2}, {{}, "┓"}}, {{0, 12, true}, {{}, " Blokus "}}, {{2}, {{}, "┏"}}, {{2}, {{}, "┓"}},
                        {{0, 1, true}, {{}, " Current Player:  "}}, {{0}, {{}, "/"}}, {{1, 12, true}, {{Vals.players, 1}, ""}},
                        {{0}, {{}, " | "}}, {{0, 1, true}, {{}, (Vals.sHex > 0 || Vals.sHept > 0) ? "Round:     " : "Round:    "}}, {{2}, {{}, "┏"}}});
                Window.wa(0, Window.getx() - 31, {{{2}, {{}, "┓"}}, {{0, 1, true}, {{}, " New "}}, {{0}, {{}, "|"}},
                        {{0, 1, true}, {{}, " Reset "}}, {{0}, {{}, "|"}}, {{0, 1, true}, {{}, " Back "}}, {{0}, {{}, "|"}}, {{0, 1, true}, {{}, " Quit "}}, {{2}, {{}, "┏"}}});

                // Board header
                Window.wa(1, 2, {{{2}, {{}, "┏"}}, {{2}, {{}, "┓"}}, {{0, 12, true}, {{}, " Board "}}, {{2}, {{}, "┏"}}, {{2}, {{}, "┓"}},
                        {{0, 1, true}, {{}, (Vals.bdim == 100) ? " Progress:      " : ((Vals.bdim > 31) ? " Progress:     " : " Progress:    ")}},
                        {{0}, {{}, "/"}}, {{1, 12, true}, {{pow(Vals.bdim, 2), 0}, ""}}, {{0}, {{}, " | (   %) "}}, {{2}, {{}, "┏"}}});

                // Action and View headers
                Action.wa(0, 1, {{{2}, {{}, "┐"}}, {{0, 12, true}, {{}, " GAME ACTIONS "}}, {{2}, {{}, "┌"}}});
                View.wa(0, 1, {{{2}, {{}, "┐"}}, {{0, 12, true}, {{}, " VIEW "}}, {{2}, {{}, "┌"}}});

                // Action actions
                Action.wstr(1, 4, "Add/Edit Piece");
                Action.wstr(2, 4, "End Current Turn");
                Action.wstr(3, 4, "Run Out of Moves");
                Action.wstr(4, 4, "Manage Game");

                // Run the regular rendering function
                render();

                // Render players
                for (unsigned char i = 0; i < Vals.players; i++) {
                    Players[i].fullRender();
                }
            }

            // -1 = Quit, 0 = Back, 1 = Reset, 2 = New Game
            char manage() {
                BSCREEN win = BSCREEN(1, 25, Window.getpy(), Window.getpx() + Window.getx() - 29);
                unsigned char l = 2;

                while (true) {
                    win.wa(0, 0, {{{0, (unsigned char)((l == 0) ? 8 + Turn : 1), true}, {{}, "New"}}, {{0}, {{}, " | "}},
                            {{0, (unsigned char)((l == 1) ? 8 + Turn : 1), true}, {{}, "Reset"}}, {{0}, {{}, " | "}},
                            {{0, (unsigned char)((l == 2) ? 8 + Turn : 1), true}, {{}, "Back"}}, {{0}, {{}, " | "}},
                            {{0, (unsigned char)((l == 3) ? 8 + Turn : 1), true}, {{}, "Quit"}}});

                    switch (win.gch()) {
                        case KEY_LEFT:
                            l = (l == 0) ? 3 : l - 1;
                            break;
                        case KEY_RIGHT:
                            l = (l == 3) ? 0 : l + 1;
                            break;    
                        case 'q':
                            return -1;
                        case 'x':
                            return 0;
                        case 'z':
                            switch (l) {
                                case 0:
                                    return 2;
                                case 1:
                                    return 1;
                                case 2:
                                    return 0;
                                case 3:
                                    return -1;
                            }
                    }
                }
            }

            // Reset game
            // Reset players except for name and color
            void reset() {
                Round = 1;
                TotalTiles = 0;
                Lx = 0;
                Ly = 0;
                Turn = rand() % Vals.players;

                for (unsigned char i = 0; i < Vals.bdim; i++) {
                    Tiles.emplace_back();

                    for (unsigned char j = 0; j < Vals.bdim; j++) {
                        Tiles[i].emplace_back(0);
                    }
                }
            }

        public:
            /// @brief Create and Initate Game
            /// @param bdim Board Dimensions
            /// @param p Players
            /// @param sBase Sets of Base Pieces
            /// @param sHex Sets of Hexominoes
            /// @param sHept Sets of Heptominoes
            Game(unsigned char p = 3, unsigned char bdim = 20, unsigned char sBase = 1, unsigned char sHex = 0, unsigned char sHept = 0) {
                // Catch any bad inputs
                bdim = (bdim > 100) ? 100 : bdim;
                bdim = (bdim < 20) ? 20 : bdim;
                p = (p > 4) ? 4 : p;
                p = (p < 2) ? 2 : p;
                sBase = (sBase > 6) ? 6 : sBase;
                sBase = (sBase < 1) ? 1 : sBase;
                sHex = (sHex > 2) ? 2 : sHex;
                sHex = (sHex < 0) ? 0 : sHex;
                sHept = (sHept > 2) ? 2 : sHept;
                sHept = (sHept < 0) ? 0 : sHept;

                // Update the configuration struct
                Vals.players = p;
                Vals.bdim = bdim;
                Vals.pdim = (sHept > 0) ? 7 : (sHex > 0) ? 6 : 5;
                Vals.sBase = sBase;
                Vals.sHex = sHex;
                Vals.sHept = sHept;
                Vals.mTiles = 89 * Vals.sBase + 210 * Vals.sHex + 756 * Vals.sHept;

                Turn = rand() % Vals.players;

                // X-Dimension of the main window
                unsigned char dX = 48 + Vals.bdim * 2 + Vals.pdim;
                // Y-Dimension of the main window
                unsigned char dY = (Vals.bdim < 22) ? 24 : Vals.bdim + 4;

                // Create a set of windows based off of the dimensions of the board
                Window = BSCREEN(dY, dX, LINES / 2 - dY / 2, COLS / 2 - dX / 2);
                Board = BSCREEN(Vals.bdim, Vals.bdim * 2, Window.getpy() + 2, Window.getpx() + 3);
                Overlay = Board;
                Preview = BSCREEN(Vals.bdim + 2, Vals.pdim + 2, Window.getpy() + 1, Window.getpx() + Board.getx() + 5);

                // Determine the y-offest of the action and viewing windows from the top of the main window
                unsigned char yO = 0;
                for (unsigned char i = 0; i < Vals.players; i++) {
                    if (Vals.players == 2) {
                        yO += 8;
                    } else if (Vals.players == 3) {
                        yO += (Vals.bdim >= 20 + i * 4) ? 8 : 4;
                    } else {
                        yO += (Vals.bdim >= 24 + i * 4) ? 8 : 4;
                    }
                }

                // Create the final two windows using the previously calculated offsets
                Action = BSCREEN(6, 23, Window.getpy() + 1 + yO, Window.getpx() + 8 + Vals.bdim * 2 + Vals.pdim);
                View = BSCREEN(6, 14, Window.getpy() + 1 + yO, Window.getpx() + 32 + Vals.bdim * 2 + Vals.pdim);

                // Fill the correct polyomino vectors with their respective sets
                Blokus::mono_pentominoes = Blokus::readPolyominoFile(MONO_PENTOMINOES);

                if (Vals.sHex >0) {
                    Blokus::hexominoes = Blokus::readPolyominoFile(HEXOMINOES);
                } else {
                    Blokus::hexominoes.clear();
                }

                if (Vals.sHept > 0) {
                    Blokus::heptominoes = Blokus::readPolyominoFile(HEPTOMINOES);
                } else {
                    Blokus::heptominoes.clear();
                }

                // Create players
                for (unsigned char i = 0; i < Vals.players; i++) {
                    Players.emplace_back(Blokus::Player(i, "[16 ch max name]"));
                }

                // Fill the board with empty tiles
                for (unsigned char i = 0; i < Vals.bdim; i++) {
                    Tiles.emplace_back();

                    for (unsigned char j = 0; j < Vals.bdim; j++) {
                        Tiles[i].emplace_back(0);
                    }
                }

                // Color initialization

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
                init_color(8, 999, 0, 0);       // Player 1
                init_color(9, 129, 482, 859);   // Player 2
                init_color(10, 0, 999, 0);      // Player 3
                init_color(11, 999, 999, 0);    // Player 4

                // Blokus General Color
                init_color(12, 745, 745, 745);

                // Color initialization
                init_pair(0, 1, 0);
                for (unsigned char i = 1; i <= 12; i++) {
                    init_pair(i, i, 0);
                }
            }

            /// @brief Start Blokus
            /// @returns -1 = Quit, 0 = New Game
            char start() {
                fullRender();

                while (true) {
                    render();

                    switch ((Lx == 0) ? Action.gch() : View.gch()) {
                        case KEY_UP:
                            Ly = (Ly == 0) ? 3 : Ly - 1;
                            break;
                        case KEY_DOWN:
                            Ly = (Ly == 3) ? 0 : Ly + 1;
                            break;
                        case KEY_LEFT:
                        case KEY_RIGHT:
                            Lx = (Lx == 0) ? 1 : 0;
                            break;
                        case 'q':
                            switch (manage()) {
                                case -1:
                                    return -1;
                                    break;
                                case 1:
                                    break;
                                case 2:
                                    return 0;
                                    break;
                            }
                            break;
                        case 'z':
                            if (Lx == 0) {
                                switch (Ly) {
                                    case 0:
                                        break;
                                    case 1:
                                        break;
                                    case 2:
                                        break;
                                    case 3:
                                        switch (manage()) {
                                            case -1:
                                                return -1;
                                                break;
                                            case 1:
                                                break;
                                            case 2:
                                                return 0;
                                                break;
                                        }
                                        break;
                                }
                            } else {
                                switch (Ly) {
                                    case 0:
                                        break;
                                    case 1:
                                        break;
                                    case 2:
                                        break;
                                    case 3:
                                        break;
                                }
                            }
                            break;
                    }
                }

                return 0;
            }
    };
}

#endif
