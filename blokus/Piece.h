#ifndef BLOKUS_PIECE
#define BLOKUS_PIECE
#include </home/bearslay/Bearslay-Launcher/blokus/Extraneous.h>

namespace Blokus {
    class Piece {
        private:
            // Base state of the piece
            std::vector<std::vector<bool>> Array;
            
            // Size of the piece
            unsigned char S = 5;
            // Tiles in the piece
            unsigned char T = 5;

            // X-pos of the piece
            char X = 0;
            // Y-pos of the piece
            char Y = 0;

            // Player who has the piece
            unsigned char P = 0;

            void flip(bool v = true) {
                if (v) {
                    for (unsigned char i = 0; i < S / 2; i++) {
                        for (unsigned char j = 0; j < S; j++) {
                            bool temp = Array[i][j];

                            Array[i][j] = Array[S - 1 - i][j];
                            Array[S - 1 - i][j] = temp;
                        }
                    }
                } else {
                    for (unsigned char i = 0; i < S; i++) {
                        for (unsigned char j = 0; j < S / 2; j++) {
                            bool temp = Array[i][j];

                            Array[i][j] = Array[i][S - 1 - j];
                            Array[i][S - 1 - j] = temp;
                        }
                    }
                }

                fixPos();
            }

            void rotate(bool ccw = true) {
                if (ccw) {
                    for (unsigned char i = 0; i < S / 2; i++) {
                        for (unsigned char j = i; j < S - 1 - i; j++) {
                            bool temp = Array[i][j];

                            Array[i][j] = Array[j][S - 1 - i];
                            Array[j][S - 1 - i] = Array[S - 1 - i][S - 1 - j];
                            Array[S - 1 - i][S - 1 - j] = Array[S - 1 - j][i];
                            Array[S - 1 - j][i] = temp;
                        }
                    }
                } else {
                    for (unsigned char i = 0; i < S / 2; i++) {
                        for (unsigned char j = i; j < S - 1 - i; j++) {
                            bool temp = Array[i][j];

                            Array[i][j] = Array[S - 1 - j][i];
                            Array[S - 1 - j][i] = Array[S - 1 - i][S - 1 - j];
                            Array[S - 1 - i][S - 1 - j] = Array[j][S - 1 - i];
                            Array[j][S - 1 - i] = temp;
                        }
                    }
                }

                fixPos();
            }

            void fixPos() {
                bool ok = false;
                bool error = false;
                while (!ok) {
                    ok = true;

                    for (unsigned char i = 0; i < S; i++) {
                        for (unsigned char j = 0; j < S; j++) {
                            if (X + i < 0 && Array[j][i]) {
                                X++;
                                error = true;
                            } if (X + i >= Vals.bdim && Array[S - 1 - j][i]) {
                                X--;
                                error = true;
                            } if (Y + i < 0 && Array[i][j]) {
                                Y++;
                                error = true;
                            } if (Y + i >= Vals.bdim && Array[i][S - 1 - j]) {
                                Y--;
                                error = true;
                            }

                            if (error) {
                                error = false;
                                ok = false;
                                break;
                            }
                        }
                    }
                }
            }

        public:
            Piece(unsigned char player, std::vector<std::vector<bool>> input) {
                Array = input;
                S = input.size();
                P = player;

                // Determine how many tiles the piece will take up
                for (unsigned char i = 0; i < Array.size(); i++) {
                    for (unsigned char j = 0; j < Array.size(); j++) {
                        T += (Array[i][j]) ? 1 : 0;
                    }
                }
            }

            void renderBig(BSCREEN &win) {
                for (unsigned char i = 0; i < S; i++) {
                    for (unsigned char j = 0; j < S; j++) {
                        if (Array[i][j]) {
                            win.wu(Y + i, X * 2 + j * 2, "█", 8 + P);
                            win.wu(Y + i, X * 2 + j * 2 + 1, "█", 8 + P);
                        }
                    }
                }
            }

            void renderSmall(BSCREEN &win) {
                for (unsigned char i = 0; i < ((S % 2 == 0) ? S / 2 : (S - 1) / 2); i++) {
                    for (unsigned char j = 0; j < S; j++) {
                        bool top = Array[i * 2][j], bot = Array[i * 2 + 1][j];

                        if (top && bot) {
                            win.wu(Y + i, X + j, "█", 8 + P);
                        } else if (!top && bot) {
                            win.wu(Y + i, X + j, "▄", 8 + P);
                        } else if (top && !bot) {
                            win.wu(Y + i, X + j, "▀", 8 + P);
                        }
                    }
                }

                if (S % 2 != 0) {
                    for (unsigned char i = 0; i < S; i++) {
                        if (Array[S - 1][i]) {
                            win.wu(Y + (S / 2), X + i, "▀", 8 + P);
                        }
                    }
                }
            }

            /// @returns Tiles the piece takes up
            const unsigned char getTiles() {return T;}

            // -1 = Quit
            char place(BSCREEN &overlay) {
                while (true) {
                    overlay.reset();
                    renderBig(overlay);
                    overlay.ri();

                    switch (overlay.gch()) {
                        case KEY_UP:
                            Y--;
                            break;
                        case KEY_DOWN:
                            Y++;
                            break;
                        case KEY_LEFT:
                            X--;
                            break;
                        case KEY_RIGHT:
                            X++;
                            break;
                        case 'q':
                            return -1;
                            break;
                        case 'a':
                            rotate();
                            break;
                        case 's':
                            rotate(false);
                            break;
                        case 'd':
                            flip();
                            break;
                        case 'f':
                            flip(false);
                            break;
                    }

                    // No need to fix the position of the piece unless it actually crosses the border
                    if (X < 0 || X + S - 1 > Vals.bdim || Y < 0 || Y + S - 1 > Vals.bdim) {fixPos();}
                }
            }
    };
}

#endif
