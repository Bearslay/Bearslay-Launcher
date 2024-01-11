#include </home/bearslay/Bearslay-Launcher/monopoly/monopolyProperty.h>
#include </home/bearslay/Bearslay-Launcher/monopoly/monopolyPlayer.h>

void activateColors() {
    // Defaults
    init_color(0, 0, 0, 0);
    init_color(1, 999, 0, 0);
    init_color(2, 0, 999, 0);
    init_color(3, 999, 999, 0);
    init_color(4, 0, 0, 999);
    init_color(5, 999, 0, 999);
    init_color(6, 0, 999, 999);
    init_color(7, 999, 999, 999);

    init_pair(COLOR_RED, 1, COLOR_BLACK);
    init_pair(COLOR_GREEN, 2, COLOR_BLACK);
    init_pair(COLOR_YELLOW, 3, COLOR_BLACK);
    init_pair(COLOR_BLUE, 4, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, 5, COLOR_BLACK);
    init_pair(COLOR_CYAN, 6, COLOR_BLACK);
    init_pair(COLOR_WHITE, 7, COLOR_BLACK);

    // Player colors
    init_color(8,  999, 0, 0);
    init_color(9,  999, 999, 999);
    init_color(10, 999, 999, 999);
    init_color(11, 999, 999, 999);
    init_color(12, 999, 999, 999);
    init_color(13, 999, 999, 999);
    init_color(14, 999, 999, 999);
    init_color(15, 999, 999, 999);

    init_pair(8, 8, COLOR_BLACK);
    init_pair(9, 9, COLOR_BLACK);
    init_pair(10, 10, COLOR_BLACK);
    init_pair(11, 11, COLOR_BLACK);
    init_pair(12, 12, COLOR_BLACK);
    init_pair(13, 13, COLOR_BLACK);
    init_pair(14, 14, COLOR_BLACK);
    init_pair(15, 15, COLOR_BLACK);

    // Color sets (properties)
    init_color(16, 396, 0, 612);
    init_color(17, 608, 729, 780);
    init_color(18, 639, 141, 541);
    init_color(19, 988, 494, 0);
    init_color(20, 921, 47, 47);
    init_color(21, 949, 855, 0);
    init_color(22, 63, 612, 0);
    init_color(23, 0, 55, 851);

    init_pair(16, 16, COLOR_BLACK);
    init_pair(17, 17, COLOR_BLACK);
    init_pair(18, 18, COLOR_BLACK);
    init_pair(19, 19, COLOR_BLACK);
    init_pair(20, 20, COLOR_BLACK);
    init_pair(21, 21, COLOR_BLACK);
    init_pair(22, 22, COLOR_BLACK);
    init_pair(23, 23, COLOR_BLACK);

    // Money
    init_color(24, 870, 686, 16);

    init_pair(24, 24, COLOR_BLACK);

    // Houses and Hotels
    init_color(25, 110, 439, 39);
    init_color(26, 940, 75, 75);

    init_pair(25, 25, COLOR_BLACK);
    init_pair(26, 26, COLOR_BLACK);
}

int main() {
    startScreen();

    activateColors();

    BSCREEN asdada = BSCREEN(50, 100, 20, 40);
    std::vector<monopolyProperty> Properties;
    std::vector<monopolyPlayer> Players;

    for (int i = 0; i < 22; i++) {
        Properties.emplace_back(monopolyProperty(i, true));
        Properties[i].renderTile(mainScreen, (i >= 11) ? 7 : 0, (i >= 11) ? i * 14 - (11 * 14): i * 14);
    }

    Players.emplace_back(monopolyPlayer(asdada));
    // /Players[0].renderStats(asdada, 0, 0);

    mainScreen.ri();
    mainScreen.gch();

    asdada.ri();
    asdada.gch();

    endwin();
    return 0;
}
