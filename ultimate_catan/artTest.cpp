#include </home/bearslay/Bearslay-Launcher/helpers/render.h>

int main() {
    startScreen();

    std::vector<std::string> outline = {
        "              .-------.             .-------.             .-------.             .-------.             .-------.             .-------.              ",
        "             /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\             ",
        "            //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\            ",
        "   .-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------.   ",
        "  /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\  ",
        " //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\ ",
        "//           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\\",
        "\\\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           //",
        " \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         // ",
        "  \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/  ",
        "  /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\  ",
        " //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\ ",
        "//           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\\",
        "\\\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           //",
        " \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         // ",
        "  \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/  ",
        "  /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\  ",
        " //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\ ",
        "//           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\\",
        "\\\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           //",
        " \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         // ",
        "  \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/  ",
        "  /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\  ",
        " //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\ ",
        "//           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\\",
        "\\\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           //",
        " \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         // ",
        "  \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/  ",
        "  /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\  ",
        " //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\ ",
        "//           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\\",
        "\\\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           //",
        " \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         // ",
        "  \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/  ",
        "  /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\  ",
        " //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\ ",
        "//           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\\",
        "\\\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           //",
        " \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         // ",
        "  \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/  ",
        "  /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\  ",
        " //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\ ",
        "//           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\\",
        "\\\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           //",
        " \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         // ",
        "  \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/  ",
        "  /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\  ",
        " //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\ ",
        "//           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\\",
        "\\\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           //",
        " \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         // ",
        "  \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/  ",
        "   \'-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------.\\           /.-------\'   ",
        "            \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //         \\\\         //            ",
        "             \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/           \\\'-------\'/             ",
        "              \'-------\'             \'-------\'             \'-------\'             \'-------\'             \'-------\'             \'-------\'              "
    };

    BSCREEN testScreen = BSCREEN(62, 157, mainScreen.gety() / 2 - 62 / 2, mainScreen.getx() / 2 - 157 / 2);

    testScreen.dbox(0, 0, testScreen.gety(), testScreen.getx(), {LD_Heavy_Both, LD_Dashed_None});
    testScreen.dbox(1, 2, testScreen.gety() - 2, testScreen.getx() - 4, {LD_Light_Hard, LD_Dashed_None});

    for (unsigned short i = 0; i < outline.size(); i++) {
        testScreen.wstr(3 + i, 5, outline[i]);
    }

    testScreen.wu(5, 17, "╱", 2);

    testScreen.wu(6, 7, " ", 2);
    testScreen.wu(6, 8, "▖", 2);
    testScreen.wu(7, 7, "▝", 2);
    testScreen.wu(7, 8, "▝", 2);

    testScreen.wu(6, 16, "▗", 2);
    testScreen.wu(6, 17, "▖", 2);
    testScreen.wu(7, 16, "▀", 2);
    testScreen.wu(7, 17, "▀", 2);

    testScreen.wu(6, 29, " ", 2);
    testScreen.wu(6, 30, "▖", 2);
    testScreen.wu(7, 29, "▟", 2);
    testScreen.wu(7, 30, "█", 2);

    testScreen.wu(6, 38, "▖", 2);
    testScreen.wu(6, 39, "▗", 2);
    testScreen.wu(7, 38, "█", 2);
    testScreen.wu(7, 39, "█", 2);

    testScreen.wu(6, 51, "▐", 2);
    testScreen.wu(6, 52, "▌", 2);
    testScreen.wint(7, 51, 3, 2, 2, true);

    testScreen.wu(6, 60, "▚", 2);
    testScreen.wu(6, 61, "▚", 2);
    testScreen.wint(7, 60, 7, 2, 2, true);

    testScreen.ri();
    testScreen.gch();

    std::vector<std::vector<std::vector<std::string>>> Tiles = {
        {
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "},
            {" ", "╱", " ", "▭", "▱", "◫", "◻", "▭", " ", "▱", "╲", " "},
            {"╱", " ", "▭", "◻", "0", "6", "-", "5", "▭", "▱", " ", "╲"},
            {"╲", " ", "◫", "▭", "╰", "─", "─", "╯", "◻", "◫", " ", "╱"},
            {" ", "╲", " ", "▱", "◻", "◫", "◻", "▱", " ", " ", "╱", " "},
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "}
        }, {
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "},
            {" ", "╱", " ", "╱", "╲", " ", "_", "╱", "╲", " ", "╲", " "},
            {"╱", " ", "⋀", " ", "0", "6", "-", "5", "⋀", "╲", " ", "╲"},
            {"╲", " ", " ", "╱", "╰", "─", "─", "╯", "_", " ", " ", "╱"},
            {" ", "╲", "╱", "⋀", " ", "╲", "_", " ", "╱", "╲", "╱", " "},
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "}
        }, {
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "},
            {" ", "╱", "⌵", "^", "⋎", "w", "⌵", "⋎", "⋎", " ", "╲", " "},
            {"╱", " ", "^", "⋎", "0", "6", "-", "5", "w", "⋎", " ", "╲"},
            {"╲", " ", "⋎", "⌵", "╰", "─", "─", "╯", "⋎", "⌵", " ", "╱"},
            {" ", "╲", "v", "⋎", "⌵", "⌵", "⋎", "^", "^", "w", "╱", " "},
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "}
        }, {
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "},
            {" ", "╱", " ", "↟", "^", "↟", "⇈", "^", " ", " ", "╲", " "},
            {"╱", " ", "⇈", "↟", "0", "6", "-", "5", "⇈", "↟", " ", "╲"},
            {"╲", " ", "↟", "⇈", "╰", "─", "─", "╯", "↟", "⇈", " ", "╱"},
            {" ", "╲", " ", "⇈", "↟", "^", "^", "⇈", "↟", " ", "╱", " "},
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "}
        }, {
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "},
            {" ", "╱", " ", "⊼", "°", " ", "~", "~", " ", " ", "╲", " "},
            {"╱", " ", " ", "~", "0", "6", "-", "5", "⊼", "°", " ", "╲"},
            {"╲", " ", "~", " ", "╰", "─", "─", "╯", " ", "~", " ", "╱"},
            {" ", "╲", " ", "°", "⊼", " ", "~", "°", "⊼", " ", "╱", " "},
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "}
        }, {
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "},
            {" ", "╱", "╲", "~", "╱", " ", "◊", "⚬", "╲", " ", "╲", " "},
            {"╱", " ", " ", "▏", "0", "6", "-", "5", " ", "⚬", " ", "╲"},
            {"╲", " ", "⚬", "╲", "╰", "─", "─", "╯", "╱", "◊", " ", "╱"},
            {" ", "╲", " ", "╱", "~", "╲", "◊", "⚬", " ", " ", "╱", " "},
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "}
        }, {
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "},
            {" ", "╱", " ", "~", " ", "≈", "≈", "≈", "~", " ", "╲", " "},
            {"╱", " ", "≈", "~", " ", "~", " ", "≈", " ", "~", " ", "╲"},
            {"╲", " ", " ", "~", "≈", " ", "~", " ", "~", "≈", " ", "╱"},
            {" ", "╲", "≈", " ", "≈", "~", " ", "~", " ", " ", "╱", " "},
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "}
        }, {
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "},
            {" ", "╱", " ", "Y", " ", "~", "~", " ", " ", " ", "╲", " "},
            {"╱", " ", "~", " ", "~", " ", "Y", " ", "~", "Y", " ", "╲"},
            {"╲", " ", "Y", "~", " ", "~", " ", " ", "Y", " ", " ", "╱"},
            {" ", "╲", " ", " ", "~", "Y", " ", "~", " ", " ", "╱", " "},
            {" ", " ", "╶", "─", "─", "─", "─", "─", "─", "╴", " ", " "}
        }
    };

    std::vector<std::vector<std::vector<unsigned char>>> Colors = {
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 2, 1, 1, 2, 0, 2, 0, 0}, //  ▭▱◫◻▭ ▱
            {0, 0, 1, 1, 0, 0, 0, 0, 2, 1, 0, 0}, // ▭◻╭06╮▭▱
            {0, 0, 2, 2, 0, 0, 0, 0, 1, 2, 0, 0}, // ◫▭╰──╯◻◫
            {0, 0, 0, 1, 1, 2, 2, 1, 2, 0, 0, 0}, //  ▱◻◫◻▱  
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        }, {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 2, 2, 0, 1, 1, 2, 0, 0, 0}, //  ╱╲ _╱╲ 
            {0, 0, 1, 0, 0, 0, 0, 0, 1, 2, 0, 0}, // ⋀ ╭06╮⋀╲
            {0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0}, //  ╱╰──╯_ 
            {0, 0, 1, 1, 0, 1, 1, 0, 1, 2, 0, 0}, // ╱⋀ ╲_ ╱╲
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        }, {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 2, 1, 1, 2, 2, 0, 0, 0}, // ⌵^⋎w⌵⋎⋎ 
            {0, 0, 1, 2, 0, 0, 0, 0, 1, 1, 0, 0}, // ^⋎╭06╮w⋎
            {0, 0, 2, 1, 0, 0, 0, 0, 2, 2, 0, 0}, // ⋎⌵╰──╯⋎⌵
            {0, 0, 1, 1, 2, 2, 1, 2, 1, 1, 0, 0}, // v⋎⌵⌵⋎^^w
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        }, {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 2, 2, 1, 1, 0, 0, 0, 0}, //  ↟^↟⇈^  
            {0, 0, 2, 1, 0, 0, 0, 0, 1, 2, 0, 0}, // ⇈↟╭06╮⇈↟
            {0, 0, 1, 2, 0, 0, 0, 0, 2, 1, 0, 0}, // ↟⇈╰──╯↟⇈
            {0, 0, 0, 1, 1, 2, 2, 1, 2, 0, 0, 0}, //  ⇈↟^^⇈↟ 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        }, {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 2, 2, 0, 0, 0, 0}, //  ⊼° ~~  
            {0, 0, 0, 2, 0, 0, 0, 0, 1, 1, 0, 0}, //  ~╭06╮⊼°
            {0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0}, // ~ ╰──╯ ~
            {0, 0, 0, 1, 1, 0, 2, 1, 1, 0, 0, 0}, //  °⊼ ~°⊼ 
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        }, {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 2, 1, 0, 1, 2, 1, 0, 0, 0}, // ╲~╱ ◊⚬╲ 
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0}, //  ▏╭06╮ ⚬
            {0, 0, 2, 1, 0, 0, 0, 0, 1, 1, 0, 0}, // ⚬╲╰──╯╱◊
            {0, 0, 0, 1, 2, 1, 1, 2, 0, 0, 0, 0}, //  ╱~╲◊⚬  
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        }, {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 2, 2, 1, 1, 0, 0, 0}, //  ~ ≈≈≈~ 
            {0, 0, 2, 1, 0, 1, 0, 2, 0, 2, 0, 0}, // ≈~ ~ ≈ ~
            {0, 0, 0, 2, 1, 0, 2, 0, 1, 2, 0, 0}, //  ~≈ ~ ~≈
            {0, 0, 1, 0, 2, 1, 0, 2, 0, 0, 0, 0}, // ≈ ≈~ ~  
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        }, {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 1, 2, 0, 0, 0, 0, 0}, //  Y ~~   
            {0, 0, 2, 0, 2, 0, 1, 0, 2, 1, 0, 0}, // ~ ~ Y ~Y
            {0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0}, // Y~ ~  Y 
            {0, 0, 0, 0, 2, 1, 0, 2, 0, 0, 0, 0}, //   ~Y ~  
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        }
    };

    // Brick
    init_color(8, 900, 150, 200);
    init_color(9, 729, 67, 110);

    // Ore
    init_color(10, 512, 512, 512);
    init_color(11, 639, 639, 639);

    // Wheat
    init_color(12, 832, 600, 0);
    init_color(13, 388, 831, 75);

    // Wood
    init_color(14, 0, 800, 150);
    init_color(15, 298, 561, 51);

    // Sheep
    init_color(16, 832, 832, 832);
    init_color(17, 55, 780, 16);

    // Gold
    init_color(18, 561, 561, 561);
    init_color(19, 78, 482, 729);

    // Water
    init_color(20, 173, 192, 721);
    init_color(21, 78, 482, 729);

    // Desert
    init_color(22, 888, 692, 472);
    init_color(23, 631, 498, 94);

    for (unsigned char i = 0; i < 16; i++) {
        init_pair(i + 8, i + 8, 0);
    }

    for (unsigned char i = 0; i < Tiles.size(); i++) {
        for (unsigned char j = 0; j < Tiles[i].size(); j++) {
            for (unsigned char k = 0; k < Tiles[i][j].size(); k++) {
                mainScreen.wu(i * Tiles[i].size() + j, k, Tiles[i][j][k], (Colors[i][j][k] == 0) ? 1 : Colors[i][j][k] + 7 + i * 2);
            }
        }
    }

    mainScreen.ri();
    mainScreen.gch();

    endwin();
    return 0;
}
