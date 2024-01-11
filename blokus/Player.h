#ifndef BLOKUS_PLAYER
#define BLOKUS_PLAYER
#include </home/bearslay/Bearslay-Launcher/blokus/Piece.h>

namespace Blokus {
    class Player {
        private:
            // Id of the player
            unsigned char Id = 0;
            std::string Name = "[16 ch max name]";

            BSCREEN Win;
            bool Compact = true;

            // The list of pieces the player has available
            std::vector<Blokus::Piece> P;
            unsigned char Page = 0;

            // Amount of base pieces the player has placed
            unsigned char NumBase = 21;
            // Amount of hexominoes the player has placed
            unsigned char NumHex = 0;
            // Amount of heptominoes the player has placed
            unsigned char NumHept = 0;
            // Amount of tiles the player has taken up
            unsigned short NumTiles = 0;

        public:
            Player (unsigned char id, std::string name) {
                // Give the player their id and name
                Id = id;
                Name = name;

                // Determine if the player will have a full or compact display
                // and the y-offset from the top of the main window
                bool full[4] = {false, false, false, false};
                unsigned char yO = 0;

                for (unsigned char i = 0; i <= Id; i++) {
                    if (Vals.players == 2) {
                        full[i] = true;
                    } else if (Vals.players == 3) {
                        full[i] = (Vals.bdim >= 20 + i * 4) ? true : false;
                    } else {
                        full[i] = (Vals.bdim >= 24 + i * 4) ? true : false;
                    }

                    if (i > 0) {yO += (full[i - 1]) ? 8 : 4;}
                }

                // Update whether the player has a compact or full display
                Compact = (!full[Id]) ? true : false;

                // Create the player's window
                Win = BSCREEN((!Compact) ? 8 : 4, 38, (LINES / 2 - ((Vals.bdim < 22) ? 25 : Vals.bdim + 4) / 2) + 1 + yO, (COLS / 2 - (48 + Vals.bdim * 2 + Vals.pdim) / 2) + 8 + Vals.bdim * 2 + Vals.pdim);

                // Add base pieces
                for (unsigned char i = 0; i < Vals.sBase; i++) {
                    for (unsigned char j = 0; j < Blokus::mono_pentominoes.size(); j++) {
                        P.emplace_back(Blokus::Piece(Id, Blokus::enlargePolyominoes(Blokus::mono_pentominoes, ((Vals.sHept > 0) ? 2 : ((Vals.sHex > 0) ? 1 : 0)))[j]));
                    }
                }

                // Add hexominoes
                for (unsigned char i = 0; i < Vals.sHex; i++) {
                    for (unsigned char j = 0; j < Blokus::hexominoes.size(); j++) {
                        P.emplace_back(Blokus::Piece(Id, Blokus::enlargePolyominoes(Blokus::hexominoes, ((Vals.sHept > 0) ? 1 : 0))[j]));
                    }
                }

                // Add heptominoes
                for (unsigned char i = 0; i < Vals.sHept; i++) {
                    for (unsigned char j = 0; j < Blokus::heptominoes.size(); j++) {
                        P.emplace_back(Blokus::Piece(Id, Blokus::heptominoes[j]));
                    }
                }
            }

            void render() {
                // Tile count
                Win.wint(1, 18 + ((Vals.mTiles >= 1000) ? 0 : ((Vals.mTiles >= 100) ? 2 : 4)), NumTiles, ((Vals.mTiles >= 1000) ? 4 : ((Vals.mTiles >= 100) ? 3 : 2)));
                Win.wint(1, 31, NumTiles / pow(Vals.bdim, 2) * 100, 3, 12, true);

                if (Compact) {
                    Win.wint(2, (Vals.sBase > 4) ? 9 : 13, NumBase, (Vals.sBase > 4) ? 3 : 2, 8 + Id, true);
                    Win.wint(2, 21, NumHex, 2, 8 + Id, true);
                    Win.wint(2, 29, NumHept, 3, 8 + Id, true);
                } else {
                    Win.wint(4, (Vals.sBase > 4) ? 20 : 22, NumBase, (Vals.sBase > 4) ? 3 : 2, 8 + Id, true);
                    Win.wint(4, 31, NumBase / (Vals.sBase * 21) * 100, 3, 12, true);
                    Win.wint(5, 22, NumHex, 2, 8 + Id, true);
                    Win.wint(5, 31, ((Vals.sHex == 0) ? 0 : NumHex / (Vals.sHex * 35) * 100), 3, 12, true);
                    Win.wint(6, 20, NumHept, 3, 8 + Id, true);
                    Win.wint(6, 31, ((Vals.sHept == 0) ? 0 : NumHept / (Vals.sHept * 108) * 100), 3, 12, true);
                }

                Win.ri();
            }

            void fullRender() {
                Win.reset();
                Win.dbox();

                // Header
                Win.wa(0, 1, {{{2}, {{}, "┐"}}, {{0, (unsigned char)(Id + 8), true}, {{}, " PLAYER "}}, {{1, (unsigned char)(Id + 8), true}, {{Id + 1, 1}, ""}},
                        {{0}, {{}, " - "}}, {{0, (unsigned char)(Id + 8), true}, {{}, Name}}, {{0}, {{}, " "}}, {{2}, {{}, "┌"}}});

                // Remaining tiles label
                Win.wa(1, 1, {{{0}, {{}, ((Vals.mTiles >= 1000) ? "Tiles Remaining:     /" : ((Vals.mTiles >= 100) ? "Tiles Remaining:      /" : "Tiles Remaining:       /"))}},
                        {{1, 12, true}, {{Vals.mTiles, ((Vals.mTiles >= 1000) ? 4 : ((Vals.mTiles >= 100) ? 3 : 2))}, ""}}, {{0}, {{}, " | (   %)"}}});

                if (Compact) {
                    Win.wa(2, 1, {{{0}, {{}, ((Vals.sBase > 4) ? "Pieces:     /" : "Pieces:       /")}}, {{1, 12, true}, {{Vals.sBase * 21, ((Vals.sBase > 4) ? 3 : 2)}, ""}},
                        {{0}, {{}, " |   /"}}, {{1, 12, true}, {{Vals.sHex * 35, 2}, ""}},
                        {{0}, {{}, " |    /"}}, {{1, 12, true}, {{Vals.sHept * 108, 3}, ""}}});
                } else {
                    Win.dline(2, 0, Win.getx());
                    Win.wstr(3, 1, "Pieces Remaining (By Polyomino):");
                    Win.wa(4, 3, {{{0}, {{}, ((Vals.sBase > 4) ? "Default (1-5):      /" : "Default (1-5):       /")}}, {{1, 12, true}, {{Vals.sBase * 21, ((Vals.sBase > 4) ? 3 : 2)}, ""}}, {{0}, {{}, " | (   %)"}}});
                    Win.wa(5, 3, {{{0}, {{}, "Hexomino  (6):       /"}}, {{1, 12, true}, {{Vals.sHex * 35, 2}, ""}}, {{0}, {{}, " | (   %)"}}});
                    Win.wa(6, 3, {{{0}, {{}, "Heptomino (7):      /"}}, {{1, 12, true}, {{Vals.sHept * 108, 3}, ""}}, {{0}, {{}, " | (   %)"}}});
                }

                render();
            }
    
            void renderPreview(BSCREEN &win) {
                // The max amount of pieces that can fit into the preview window
                unsigned char m = (win.gety() - (win.gety() % Vals.pdim)) / Vals.pdim;
                for (unsigned char i = 0; i < m; i++) {
                    
                }
            }
    };
}

#endif
