#ifndef BETTER_SCREEN
#define BETTER_SCREEN

#include <ncurses.h>
#include <vector>
#include <utility>
#include <locale.h>
#include <chrono>
#include <thread>
#include <string>
#include <math.h>
#include <cmath>
#include <ctime>

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

// Line drawing styles
#define LD_Light_Hard 0
#define LD_Light_Soft 1
#define LD_Heavy_Both 2
#define LD_Heavy_Horizontal 3
#define LD_Heavy_Vertical 4
#define LD_Doubled_Both 5
#define LD_Doubled_Horizontal 6
#define LD_Doubled_Vertical 7

#define LD_Dashed_None 0
#define LD_Dashed_Double 1
#define LD_Dashed_Triple 2
#define LD_Dashed_Quadruple 3
#define LD_Dashed_Single_1 4
#define LD_Dashed_Single_2 5

// Constructor Arguments: (dimy, dimx, y, x)
class BSCREEN {
    private:
        std::pair<unsigned short, unsigned short> Dims;
        std::pair<unsigned short, unsigned short> Origin;
        WINDOW *Window;
        bool canSkip = true;

        struct Cell {
            // Stored as a string to allow for unicode
            std::string character = " ";
            // References a color pair, so FG color and BG color are both accounted for here
            unsigned char color = 1;
            // Other basic attributes
            bool bold = false;
            bool italic = false;
            bool underlined = false;
        };
        std::vector<std::vector<Cell>> Grid;

        // Print a character from the grid to an ncurses window
        void print(unsigned short y, unsigned short x) {
            if (y >= 0 && y < Dims.first && x >= 0 && x < Dims.second) {
                wattron(Window, COLOR_PAIR(Grid[y][x].color));

                if (Grid[y][x].bold) {
                    wattron(Window, A_BOLD);
                }
                if (Grid[y][x].italic) {
                    wattron(Window, A_ITALIC);
                }
                if (Grid[y][x].underlined) {
                    wattron(Window, A_UNDERLINE);
                }

                if (Grid[y][x].character == "%") {
                    mvwprintw(Window, y, x, "%%");
                } else {
                    mvwprintw(Window, y, x, Grid[y][x].character.c_str());
                }

                if (Grid[y][x].bold) {
                    wattroff(Window, A_BOLD);
                }
                if (Grid[y][x].italic) {
                    wattroff(Window, A_ITALIC);
                }
                if (Grid[y][x].underlined) {
                    wattroff(Window, A_UNDERLINE);
                }

                wattroff(Window, COLOR_PAIR(Grid[y][x].color));
            }
        }

        // Apply the ms delay as well as provide an opportunity for the user to skip the animation
        bool delay(int ms, bool canSkip) {
            for (int i = 0; i < ms; i++) {
                if (canSkip) {
                    if (wgetch(Window) != ERR) {
                        return true;
                    }
                }

                sleep_for(1ms);
            }

            return false;
        }

    public:
        //
        // GENERAL STUFF
        //

        // By default, a new screen will basically occupy stdscr
        BSCREEN(unsigned short dimy = LINES, unsigned short dimx = COLS, unsigned short y = 0, unsigned short x = 0) {
            y = (y < 0 || y > LINES - 1) ? 0 : y;
            x = (x < 0 || x > COLS - 1) ? 0 : x;

            // Automatically sizes dimensions that are too large to fit into the current terminal screen
            dimy = (dimy < 1 || LINES - dimy - y < 0) ? LINES - y : dimy;
            dimx = (dimx < 1 || COLS - dimx - x < 0) ? COLS - x : dimx;

            Window = newwin(dimy, dimx, y, x);
            Dims.first = dimy;
            Dims.second = dimx;
            Origin.first = y;
            Origin.second = x;

            // No segfaults no segfaults no segfaults no segfaults
            for (int i = 0; i < Dims.first; i++) {
                Grid.emplace_back();

                for (int j = 0; j < Dims.second; j++) {
                    Grid[i].emplace_back();
                }
            }
        }

        // Returns the height of the window
        const unsigned short gety() {return Dims.first;}
        // Returns the width of the window
        const unsigned short getx() {return Dims.second;}
        // Returns the top-left y-coord of the window
        const unsigned short getpy() {return Origin.first;};
        // Returns the top-left x-coord of the window
        const unsigned short getpx() {return Origin.second;};
        
        //
        // WRITING TO SCREEN
        //

        // Visually reset the window, but keep the values of the cells intact
        void clear() {wclear(Window);}

        // Reset all cells in the screen as well as the window
        void reset() {
            for (unsigned short i = 0; i < Dims.first; i++) {
                for (unsigned short j = 0; j < Dims.second; j++) {
                    Grid[i][j] = Cell();
                }
            }

            wclear(Window);
        }

        // Write a string to the screen, wraps automatically
        void wstr(unsigned short y, unsigned short x, std::string input, unsigned char color = 1, bool bold = false, bool italic = false, bool underline = false) {
            if (y < 0 || y > Dims.first - 1 || x < 0 || x > Dims.second - 1) {return;}

            unsigned short yOffest = 0, xOffset = 0;
            for (unsigned short i = 0; i < input.length(); i++) {
                if (x + i - xOffset > Dims.second - 1) {
                    yOffest++;
                    xOffset += Dims.second;
                }

                if (y + yOffest > Dims.first - 1 || y + yOffest < 0 || x + i - xOffset < 0) {
                    break;
                }

                Grid[y + yOffest][x + i - xOffset].character = input[i];
                Grid[y + yOffest][x + i - xOffset].color = color;
                Grid[y + yOffest][x + i - xOffset].bold = bold;
                Grid[y + yOffest][x + i - xOffset].italic = italic;
                Grid[y + yOffest][x + i - xOffset].underlined = underline;
            }
        }

        // Doesn't really do anything special, just assigns the indicated position to the string without wrapping or anything
        // Intended primarily for writing unicode characters to the screen
        void wu(unsigned short y, unsigned short x, std::string input, unsigned char color = 1, bool bold = false, bool italic = false, bool underline = false) {
            if (y < 0 || y > Dims.first - 1 || x < 0 || x > Dims.second - 1) {return;}

            input = (input == "") ? " " : input;
            
            Grid[y][x].character = input;
            Grid[y][x].color = color;
            Grid[y][x].bold = bold;
            Grid[y][x].italic = italic;
            Grid[y][x].underlined = underline;
        }

        // Write an integer to the screen
        // minWidth can be used to add on leading zeros to a number if the amount of digits in input is less than the value of minWidth
        void wint(unsigned short y, unsigned short x, int input, unsigned short minWidth = 0, unsigned char color = 1, bool bold = false, bool italic = false, bool underline = false) {
            if (y < 0 || y > Dims.first - 1 || x < 0 || x > Dims.second - 1) {return;}
            
            std::string number = std::to_string(input);
            unsigned short length = number.length();

            for (int i = 0; i < minWidth - length; i++) {
                number.insert(0, "0");
            }

            wstr(y, x, number, color, bold, italic, underline);
        }

        // Write a whole set of stuff to the screen, not intended for long strings that would require wrapping
        // Best approximation I can do to something like printf/printw
        //
        // Input template: {{{0 = string/1 = integer/2 = unicode, color, bold, italic, underline}, {{int, width}, "String"}}}
        // String input:  {{0}, {{}, "String"}}
        // Integer input: {{1}, {{int, width}, ""}}
        // Unicode input: {{2}, {{}, "char"}}
        void wa(unsigned short y, unsigned short x, std::vector<std::pair<std::vector<unsigned char>, std::pair<std::pair<int, unsigned short>, std::string>>> input) {
            if (y < 0 || y > Dims.first - 1 || x < 0 || x > Dims.second - 1) {return;}

            // Verify that all entries in the input have a valid writing type (either string, integer, or unicode character)
            for (unsigned char i = 0; i < input.size(); i++) {
                if (input[i].first.size() == 0) {
                    return;
                }
                else if (input[i].first[0] < 0 || input[i].first[0] > 2) {
                    return;
                }
            }

            // Do the crazy stuff
            unsigned short newx = x;
            for (unsigned char i = 0; i < input.size(); i++) {
                for (unsigned char j = input[i].first.size(); j < 5; j++) {
                    // Add a "false" for everything except the color, which defaults to 1
                    input[i].first.emplace_back((j == 1) ? 1 : 0);
                }

                if (input[i].first[0] == 0) {
                    wstr(y, newx, input[i].second.second, input[i].first[1], input[i].first[2], input[i].first[3], input[i].first[4]);
                    newx += input[i].second.second.length();
                }
                else if (input[i].first[0] == 1) {
                    wint(y, newx, input[i].second.first.first, input[i].second.first.second, input[i].first[1], input[i].first[2], input[i].first[3], input[i].first[4]);
                    newx += (input[i].second.first.second > std::to_string(input[i].second.first.first).length()) ? input[i].second.first.second : std::to_string(input[i].second.first.first).length();
                }
                else if (input[i].first[0] == 2) {
                    wu(y, newx, input[i].second.second, input[i].first[1], input[i].first[2], input[i].first[3], input[i].first[4]);
                    newx++;
                }
            }
        }

        //
        // READING FROM SCREEN
        //

        // Return the character requested (as a string because unicode), a space by default
        std::string sch(unsigned short y, unsigned short x) {
            if (y >= 0 && y < Dims.first && x >= 0 && x < Dims.second) {
                return Grid[y][x].character;
            }
            
            return " ";
        }

        // Return the color of the requested cell, 1 by default
        int sco(unsigned short y, unsigned short x) {
            if (y >= 0 && y < Dims.first && x >= 0 && x < Dims.second) {
                return Grid[y][x].color;
            }
            
            return 1;
        }

        // Return the state of the requested attribute for the requested cell
        // id: 0 = bold, 1 = italic, 2 = underline
        bool sat(unsigned short y, unsigned short x, unsigned char id) {
            if (y >= 0 && y < Dims.first && x >= 0 && x < Dims.second) {
                switch (id) {
                    case 0:
                        return Grid[y][x].bold;
                        break;
                    case 1:
                        return Grid[y][x].italic;
                        break;
                    case 2:
                        return Grid[y][x].underlined;
                        break;
                }
            }
            
            return false;
        }

        // Return the entire set of information associated with the requested cell
        std::pair<std::pair<std::string, int>, std::vector<bool>> sce(unsigned short y, unsigned short x) {
            std::pair<std::pair<std::string, int>, std::vector<bool>> output = {{" ", 1}, {false, false, false}};

            if (y >= 0 && y < Dims.first && x >= 0 && x < Dims.second) {
                output.first.first = Grid[y][x].character;
                output.first.second = Grid[y][x].color;
                output.second[0] = Grid[y][x].bold;
                output.second[1] = Grid[y][x].italic;
                output.second[2] = Grid[y][x].underlined;
            }
            
            return output;
        }

        //
        // USER INPUTS
        //

        // Replicate the wgetch() ncruses fucntion
        int gch(bool enableKeypad = true, bool pause = true, bool autoRender = true) {
            if (autoRender) {
                ri();
            }

            if (enableKeypad) {
                keypad(Window, true);
            }
            else {
                keypad(Window, false);
            }
            if (!pause) {
                nodelay(Window, true);
            }
            else {
                nodelay(Window, false);
            }

            int input = wgetch(Window);

            return input;
        }

        // Replicate the wgetnstr() ncurses function
        std::string gstr(int max = 255, bool enableKeypad = true, bool showStr = true, bool showCursor = true, unsigned char echoColor = 1) {
            keypad(Window, false);
            noecho();
            curs_set(0);

            if (enableKeypad) {
                keypad(Window, true);
            }
            if (showStr) {
                echo();
            }
            if (showCursor) {
                curs_set(1);
            }

            char input[max] = "";
            wattron(Window, COLOR_PAIR(echoColor));
            wgetnstr(Window, input, max);
            wattroff(Window, COLOR_PAIR(echoColor));

            std::string output = "";
            for (int i = 0; i < max; i++) {
                if (input[i] == '\0') {
                    break;
                }

                output += input[i];
            }

            keypad(Window, false);
            noecho();
            curs_set(0);

            return output;
        }

        //
        // RENDERING/ANIMATIONS
        //

        // Instantly render the screen
        void ri() {
            for (unsigned short i = 0; i < Dims.first; i++) {
                for (unsigned short j = 0; j < Dims.second; j++) {
                    print(i, j);
                }
            }

            wrefresh(Window);
        }

        // Render the screen line-by-line
        // When canSkip is true: any keypress during the animation will cause the screen to instantly render and skip the remainder of the animation
        //
        // originDirection: 0 = top, 1 = bottom, 2 = left, 3 = right
        // instant: true = render full line at once, false = render each character seperately
        // reverse (applied only when instant = false): true = l>r or t>b, false = r>l or b>t
        // ms: delay between each screen refresh in milliseconds
        void rl(unsigned char lineDirection = 0, bool instant = true, bool reverse = false, int ms = 20) {
            if (canSkip) {
                nodelay(Window, true);
            }

            bool skip = false;

            std::pair<unsigned short, unsigned short> limits = Dims;
            if (lineDirection > 1) {
                limits.first = Dims.second;
                limits.second = Dims.first;
            }

            for (unsigned short i = 0; i < limits.first; i++) {
                for (unsigned short j = 0; j < limits.second; j++) {
                    if (lineDirection == 0) {
                        if (!reverse) {
                            print(i, j);
                        }
                        else {
                            print(i, Dims.second - 1 - j);
                        }
                    }
                    else if (lineDirection == 1) {
                        if (!reverse) {
                            print(Dims.first - 1 - i, j);
                        }
                        else {
                            print(Dims.first - 1 - i, Dims.second - 1 - j);
                        }
                    }
                    else if (lineDirection == 2) {
                        if (!reverse) {
                            print(j, i);
                        }
                        else {
                            print(Dims.first - 1 - j, i);
                        }
                    }
                    else if (lineDirection == 3) {
                        if (!reverse) {
                            print(j, Dims.second - 1 - i);
                        }
                        else {
                            print(Dims.first - 1 - j, Dims.second - 1 - i);
                        }
                    }

                    if (!skip && !instant) {
                        wrefresh(Window);
                        skip = (delay(ms, canSkip)) ? true : false;
                    }
                }

                // The user can skip the animation if permitted
                if (!skip && canSkip) {
                    if (wgetch(Window) != ERR) {
                        skip = true;
                    }
                }

                if (!skip) {
                    wrefresh(Window);
                    skip = (delay(ms, canSkip)) ? true : false;
                }
            }

            if (canSkip) {
                nodelay(Window, false);
            }

            wrefresh(Window);
        }

        // Render the screen in a circle-like motion
        // When canSkip is true: any keypress during the animation will cause the screen to instantly render and skip the remainder of the animation
        //
        // ccw: true = render counter-clockwise, false = render clockwise
        // doubleRender: true = render two quadrants at once, false = render only one quadrant at a time
        // resolution: the amount of steps the renderer uses, a lower value should be used for larger screens
        // ms: delay between each screen refresh in milliseconds
        void rc(bool ccw = true, bool doubleRender = false, double resolution = 0.005, int ms = 5) {
            if (canSkip) {
                nodelay(Window, true);
            }
            bool skip = false;

            unsigned char round = (doubleRender) ? 1 : 2;
            for (double i = 0; i < round * M_PI; i += resolution) {
                // Render the line that goes from one end of the circle to the other
                double slope1 = ((ccw) ? 1 : -1) * (((-25 * fabs(sin(i))) - (25 * fabs(sin(i)))) / ((-50 * fabs(cos(i))) - (50 * fabs(cos(i)))));
                for (double j = 0; j < Dims.second / 2 + 1; j += resolution * 100) {
                    if (i < M_PI / 2) {
                        print(Dims.first / 2 - slope1 * j, Dims.second / 2 + j * 2);

                        if (doubleRender) {
                            print(Dims.first / 2 + slope1 * j, Dims.second / 2 - j * 2);
                        }
                    }
                    else if (i < M_PI) {
                        print(Dims.first / 2 - slope1 * j, Dims.second / 2 - j * 2);

                        if (doubleRender) {
                            print(Dims.first / 2 + slope1 * j, Dims.second / 2 + j * 2);
                        }
                    }
                    else if (i < 3 * M_PI / 2) {
                        print(Dims.first / 2 + slope1 * j, Dims.second / 2 - j * 2);
                    }
                    else if (i < 2 * M_PI) {
                        print(Dims.first / 2 + slope1 * j, Dims.second / 2 + j * 2);
                    }
                }

                // The closer the slope gets to undefined, the more characters the regular function misses, so these increase the resolution around those areas
                if (i < M_PI / 2 + resolution * 16 && i > M_PI / 2 - resolution * 16) {
                    for (double j = 0; j < Dims.second / 2 + 1; j += resolution * 10) {
                        if (i < M_PI / 2) {
                            print(Dims.first / 2 - slope1 * j, Dims.second / 2 + j * 2);

                            if (doubleRender) {
                                print(Dims.first / 2 + slope1 * j, Dims.second / 2 - j * 2);
                            }
                        }
                        else if (i < M_PI) {
                            print(Dims.first / 2 - slope1 * j, Dims.second / 2 - j * 2);

                            if (doubleRender) {
                                print(Dims.first / 2 + slope1 * j, Dims.second / 2 + j * 2);
                            }
                        }
                    }
                }
                if (i < 3 * M_PI / 2 + resolution * 16 && i > 3 * M_PI / 2 - resolution * 16) {
                    for (double j = 0; j < Dims.second / 2 + 1; j += resolution * 10) {
                        if (i < 3 * M_PI / 2) {
                            print(Dims.first / 2 + slope1 * j, Dims.second / 2 - j * 2);
                        }
                        else if (i < 2 * M_PI) {
                            print(Dims.first / 2 + slope1 * j, Dims.second / 2 + j * 2);
                        }
                    }
                }

                if (!skip) {
                    wrefresh(Window);
                    skip = (delay(ms, canSkip)) ? true : false;
                }
            }

            // Fills in the few blanks the animation tends to leave
            ri();
            wrefresh(Window);

            if (canSkip) {
                nodelay(Window, false);
            }
        }

    //
    // LINE DRAWING
    //
    private:
        // Most constants used for line drawing is declared here so that huge amounts of memory isn't allocated for the same values
        // This mainly includes the character map as well as a few special cases and dash maps
        struct {
            // Store each border piece's directions that they point and value in each direction
            // Directions stored in the order: Up, Right, Down, Left (N, E, S, W)
            // 0 = Nothing, 1 = Light line, 2 = Heavy line, 3 = Double line
            struct Border {std::string border; unsigned char directions[4];};
            std::vector<Border> key = {
                // Horizontal and vertical lines off all 6 dash styles (none, single 1, single 2, double, triple, quadruple)
                {"─", {0, 1, 0, 1}}, {"╌", {0, 1, 0, 1}}, {"┄", {0, 1, 0, 1}}, {"┈", {0, 1, 0, 1}}, {"╴", {0, 1, 0, 1}}, {"╶", {0, 1, 0, 1}},
                {"━", {0, 2, 0, 2}}, {"╍", {0, 2, 0, 2}}, {"┅", {0, 2, 0, 2}}, {"┉", {0, 2, 0, 2}}, {"╸", {0, 2, 0, 2}}, {"╺", {0, 2, 0, 2}},
                {"│", {1, 0, 1, 0}}, {"╎", {1, 0, 1, 0}}, {"┆", {1, 0, 1, 0}}, {"┊", {1, 0, 1, 0}}, {"╵", {1, 0, 1, 0}}, {"╷", {1, 0, 1, 0}},
                {"┃", {2, 0, 2, 0}}, {"╏", {2, 0, 2, 0}}, {"┇", {2, 0, 2, 0}}, {"┋", {2, 0, 2, 0}}, {"╹", {2, 0, 2, 0}}, {"╻", {2, 0, 2, 0}},

                // Corners
                {"┌", {0, 1, 1, 0}}, {"┍", {0, 2, 1, 0}}, {"┎", {0, 1, 2, 0}}, {"┏", {0, 2, 2, 0}}, {"┐", {0, 0, 1, 1}}, {"┑", {0, 0, 1, 2}}, {"┒", {0, 0, 2, 1}}, {"┓", {0, 0, 2, 2}},
                {"└", {1, 1, 0, 0}}, {"┕", {1, 2, 0, 0}}, {"┖", {2, 1, 0, 0}}, {"┗", {2, 2, 0, 0}}, {"┘", {1, 0, 0, 1}}, {"┙", {1, 0, 0, 2}}, {"┚", {2, 0, 0, 1}}, {"┛", {2, 0, 0, 2}},

                // Tees
                {"├", {1, 1, 1, 0}}, {"┝", {1, 2, 1, 0}}, {"┞", {2, 1, 1, 0}}, {"┟", {1, 1, 2, 0}}, {"┠", {2, 1, 2, 0}}, {"┡", {2, 2, 1, 0}}, {"┢", {1, 2, 2, 0}}, {"┣", {2, 2, 2, 0}},
                {"┤", {1, 0, 1, 1}}, {"┥", {1, 0, 1, 2}}, {"┦", {2, 0, 1, 1}}, {"┧", {1, 0, 2, 1}}, {"┨", {2, 0, 2, 1}}, {"┩", {2, 0, 1, 2}}, {"┪", {1, 0, 2, 2}}, {"┫", {2, 0, 2, 2}},
                {"┬", {0, 1, 1, 1}}, {"┭", {0, 1, 1, 2}}, {"┮", {0, 2, 1, 1}}, {"┯", {0, 2, 1, 2}}, {"┰", {0, 1, 2, 1}}, {"┱", {0, 1, 2, 2}}, {"┲", {0, 2, 2, 1}}, {"┳", {0, 2, 2, 2}},
                {"┴", {1, 1, 0, 1}}, {"┵", {1, 1, 0, 2}}, {"┶", {1, 2, 0, 1}}, {"┷", {1, 2, 0, 2}}, {"┸", {2, 1, 0, 1}}, {"┹", {2, 1, 0, 2}}, {"┺", {2, 2, 0, 1}}, {"┻", {2, 2, 0, 2}},

                // Crosses
                {"┼", {1, 1, 1, 1}}, {"┽", {1, 1, 1, 2}}, {"┾", {1, 2, 1, 1}}, {"┿", {1, 2, 1, 2}}, {"╀", {2, 1, 1, 1}}, {"╁", {1, 1, 2, 1}}, {"╂", {2, 1, 2, 1}}, {"╃", {2, 1, 1, 2}},
                {"╄", {2, 2, 1, 1}}, {"╅", {1, 1, 2, 2}}, {"╆", {1, 2, 2, 1}}, {"╇", {2, 2, 2, 1}}, {"╈", {1, 2, 2, 2}}, {"╉", {1, 1, 2, 2}}, {"╊", {2, 2, 2, 1}}, {"╋", {2, 2, 2, 2}},

                // Double
                {"═", {0, 3, 0, 3}}, {"║", {3, 0, 3, 0}}, {"╒", {0, 3, 1, 0}}, {"╓", {0, 1, 3, 0}}, {"╔", {0, 3, 3, 0}}, {"╕", {0, 0, 1, 3}}, {"╖", {0, 0, 3, 1}}, {"╗", {0, 0, 3, 3}},
                {"╘", {1, 3, 0, 0}}, {"╙", {3, 1, 0, 0}}, {"╚", {3, 3, 0, 0}}, {"╛", {1, 0, 0, 3}}, {"╜", {3, 0, 0, 1}}, {"╝", {3, 0, 0, 3}},
                {"╞", {1, 3, 1, 0}}, {"╟", {3, 1, 3, 0}}, {"╠", {3, 3, 3, 0}}, {"╡", {1, 0, 1, 3}}, {"╢", {3, 0, 3, 1}}, {"╣", {3, 0, 3, 3}}, {"╤", {0, 3, 1, 3}}, {"╥", {0, 1, 3, 1}},
                {"╦", {0, 3, 3, 3}}, {"╧", {1, 3, 0, 3}}, {"╨", {3, 1, 0, 1}}, {"╩", {3, 3, 0, 3}},
                {"╪", {1, 3, 1, 3}}, {"╫", {3, 1, 3, 1}}, {"╬", {3, 3, 3, 3}},
                
                // Soft corners and thickness transitions
                {"╭", {0, 1, 1, 0}}, {"╮", {0, 0, 1, 1}}, {"╯", {1, 0, 0, 1}}, {"╰", {1, 1, 0, 0}}, {"╼", {0, 2, 0, 1}}, {"╽", {1, 0, 2, 0}}, {"╾", {0, 1, 0, 2}}, {"╿", {2, 0, 1, 0}}   
            };
            // Straight lines and the light corner pieces need special attention
            unsigned char specialCases[10][4] = {
                {0, 1, 0, 1},
                {1, 0, 1, 0},
                {0, 2, 0, 2},
                {2, 0, 2, 0},
                {0, 3, 0, 3},
                {3, 0, 3, 0},
                {0, 1, 1, 0},
                {1, 0, 0, 1},
                {1, 1, 0, 0},
                {0, 0, 1, 1}
            };
            // Dashes that can modify each set
            std::string setDashes[2][2][6] = {{{"─", "╌", "┄", "┈", "╴", "╶"}, {"│", "╎", "┆", "┊", "╵", "╷"}}, {{"━", "╍", "┅", "┉", "╸", "╺"}, {"┃", "╏", "┇", "┋", "╹", "╻"}}};
        } LD_Constants;

        // Following functions are only used to draw lines

        unsigned char getDirection(std::string input, unsigned char direction) {
            for (unsigned short i = 0; i < LD_Constants.key.size(); i++) {
                if (LD_Constants.key[i].border == input) {
                    if (direction == 0) {
                        return LD_Constants.key[i].directions[2];
                        break;
                    }
                    else if (direction == 1) {
                        return LD_Constants.key[i].directions[0];
                        break;
                    }
                    else if (direction == 2) {
                        return LD_Constants.key[i].directions[1];
                        break;
                    }
                    else if (direction == 3) {
                        return LD_Constants.key[i].directions[3];
                        break;
                    }
                }
            }

            return 0;
        }

        unsigned char styleToDirection(unsigned char style, bool vertical) {
            switch (style) {
                case 0:
                case 1:
                    return 1;
                    break;
                case 2:
                    return 2;
                    break;
                case 3:
                    return (vertical) ? 1: 2;
                    break;
                case 4:
                    return (vertical) ? 2: 1;
                    break;
                case 5:
                    return 3;
                    break;
                case 6:
                    return (vertical) ? 1: 3;
                    break;
                case 7:
                    return (vertical) ? 3: 1;
                    break;
            }

            return 1;
        }

        std::string getPiece(unsigned char directions[4], bool vertical, std::pair<unsigned char, unsigned char> style) {
            char special = -1;
            for (unsigned char j = 0; j < 10; j++) {
                if (LD_Constants.specialCases[j][0] == directions[0] && LD_Constants.specialCases[j][2] == directions[1] && LD_Constants.specialCases[j][3] == directions[2] && LD_Constants.specialCases[j][1] == directions[3]) {
                    special = j;
                    break;
                }
            }

            if (special >= 0 && special < 6) {
                if (style.first <= 1) {
                    return LD_Constants.setDashes[0][vertical][style.second];
                }
                else if (style.first == 2) {
                    return LD_Constants.setDashes[1][vertical][style.second];
                }
                else if (style.first == 3) {
                    return LD_Constants.setDashes[!vertical][vertical][style.second];
                }
                else if (style.first == 4) {
                    return LD_Constants.setDashes[vertical][vertical][style.second];
                }
                else if (style.first == 5) {
                    return vertical ? "║" : "═";
                }
                else if (style.first == 6) {
                    return (vertical) ? LD_Constants.setDashes[0][1][style.second] : "═";
                }
                else if (style.first == 7) {
                    return (vertical) ? "║" : LD_Constants.setDashes[0][0][style.second];
                }
            }
            else {
                // The search starts at a later location if the character has been determined to be one of the rounded corners
                for (unsigned short j = (style.first == 1 && special >= 6) ? 39 : 0; j < LD_Constants.key.size(); j++) {
                    if (directions[0] == LD_Constants.key[j].directions[0] && directions[1] == LD_Constants.key[j].directions[2] && directions[2] == LD_Constants.key[j].directions[3] && directions[3] == LD_Constants.key[j].directions[1]) {
                        return LD_Constants.key[j].border;
                    }
                }
            }

            return "a";
        }

    public:
        // Draw either a vertical or horizontal line using the unicode box drawing characters
        // The line will also automatically generate tees and crosses when it intersects other box drawing elements
        // If the line attempts to go outside of the screen it is drawn on, then the length of the line is shortened to stay within the screen's borders
        //
        // starty: y position of the initial point
        // startx: x position of the initial point
        // length: length of the line in characters
        // vertical: creates a vertical line when true and a horizontal one when false
        // style: standard style/dash set used throughout BSCREEN
        // color: color pair number to use when drawing the grid (does not fill in cells with a background color)
        void dline(unsigned short starty, unsigned short startx, unsigned short length, bool vertical = false, std::pair<unsigned char, unsigned char> style = {LD_Light_Hard, LD_Dashed_None}, unsigned char color = 1) {
            // Don't let the line get drawn outside of the screen because then our good old friend segfault will show up and that would just be too much fun
            if (starty < 0 || startx < 0 || starty > Dims.first - 1 || startx > Dims.second - 1) {return;}
            
            // If a line is longer than the distance between the line origin and the border of the window that is in the direction the line is heading,
            // then the line length can only be as long as that distance because nothing past that would render anyways and segfaults would probably happen
            // If the length is acceptable, then the cutoff remains the length of the line as per normal
            unsigned short cutoff = abs(length);
            if (!vertical) {
                if (length > 0 && Dims.second - length < startx) {
                    cutoff = Dims.second - startx;
                }
                else if (length < 0 && startx - length < 0) {
                    cutoff = startx;
                }
            }
            else {
                if (length > 0 && Dims.first - length < starty) {
                    cutoff = Dims.first - starty;
                }
                else if (length < 0 && starty - length < 0) {
                    cutoff = starty;
                }
            }

            // The new position increases in both the x and y directions, but then either the x or y part of the new position is multiplied by 0 to
            // create a line that has a slope of 0 or undefined, this line function does not support different slopes
            unsigned char multy = (vertical) ? 1 : 0;
            unsigned char multx = (vertical) ? 0 : 1;

            // The "length" variable is treated as a vector, so a negative value for it makes it go up or left rather than down or right
            char step = (length < 0) ? -1 : 1;
            std::string unit = "a";
            std::pair<unsigned short, unsigned short> pos;

            unsigned char directions[4] = {0, 0, 0, 0};
            std::string neighbors[4] = {" ", " ", " ", " "};

            for (unsigned short i = 0; abs(i) < cutoff; i += step) {
                unit = "a";

                // Figure out the current position
                pos = {starty + (i * multy), startx + (i * multx)};

                for (char j = 0; j < 4; j++) {
                    directions[j] = 0;
                    neighbors[j] = " ";
                }

                // Scan the surrounding characters
                // If the current position is on the edge of a window, the program can't scan outside of the window because of course it cant
                if (pos.first > 0) {
                    neighbors[0] = sch(pos.first - 1, pos.second);
                }
                if (pos.first < Dims.first - 1) {
                    neighbors[1] = sch(pos.first + 1, pos.second);
                }
                if (pos.second > 0) {
                    neighbors[2] = sch(pos.first, pos.second - 1);
                }
                if (pos.second < Dims.second - 1) {
                    neighbors[3] = sch(pos.first, pos.second + 1);
                }

                // Get the directions based off of surroundings
                for (char j = 0; j < 4; j++) {
                    directions[j] = getDirection(neighbors[j], j);
                }

                // Alter the directions based on a few other things
                // Other than the first and last unit in the line, there absolutely has to be a full line
                if (abs(i) > 0 && abs(i) < cutoff - 1) {
                    if (!vertical) {
                        directions[2] = (directions[2] == 0) ? styleToDirection(style.first, vertical) : directions[2];
                        directions[3] = (directions[3] == 0) ? styleToDirection(style.first, vertical) : directions[3];
                    }
                    else {
                        directions[0] = (directions[0] == 0) ? styleToDirection(style.first, vertical) : directions[0];
                        directions[1] = (directions[1] == 0) ? styleToDirection(style.first, vertical) : directions[1];
                    }
                }
                else {
                    if (!vertical) {
                        if ((length > 0 && i == 0) || (length < 0 && abs(i) == cutoff - 1)) {
                            directions[2] = (directions[2] == 0 && directions[0] == 0 && directions[1] == 0 ) ? styleToDirection(style.first, vertical) : directions[2];
                            directions[3] = (directions[3] == 0) ? styleToDirection(style.first, vertical) : directions[3];
                        }
                        else {
                            directions[3] = (directions[3] == 0 && directions[0] == 0 && directions[1] == 0 ) ? styleToDirection(style.first, vertical) : directions[3];
                            directions[2] = (directions[2] == 0) ? styleToDirection(style.first, vertical) : directions[2];
                        }
                    }
                    else {
                        if ((length > 0 && i == 0) || (length < 0 && abs(i) == cutoff - 1)) {
                            directions[0] = (directions[0] == 0 && directions[2] == 0 && directions[3] == 0 ) ? styleToDirection(style.first, vertical) : directions[0];
                            directions[1] = (directions[1] == 0) ? styleToDirection(style.first, vertical) : directions[1];
                        }
                        else {
                            directions[1] = (directions[1] == 0 && directions[2] == 0 && directions[3] == 0 ) ? styleToDirection(style.first, vertical) : directions[1];
                            directions[0] = (directions[0] == 0) ? styleToDirection(style.first, vertical) : directions[0];
                        }
                    }
                }

                unit = getPiece(directions, vertical, style);

                // This should only trigger when a heavy and a double line try to merge with each other, those two sets aren't compatible
                // The change that is made depends on the current style so that the remainder of the line doesn't get corrupted
                if (unit == "a") {
                    if (style.first == 2 || style.first == 3 || style.first == 4) {
                        directions[0] = (directions[0] == 3) ? 2 : directions[0];
                        directions[1] = (directions[1] == 3) ? 2 : directions[1];
                        directions[2] = (directions[2] == 3) ? 2 : directions[2];
                        directions[3] = (directions[3] == 3) ? 2 : directions[3];
                    }
                    else {
                        directions[0] = (directions[0] == 2) ? 3 : directions[0];
                        directions[1] = (directions[1] == 2) ? 3 : directions[1];
                        directions[2] = (directions[2] == 2) ? 3 : directions[2];
                        directions[3] = (directions[3] == 2) ? 3 : directions[3];
                    }
                    
                    unit = getPiece(directions, vertical, style);
                }
                
                wu(pos.first, pos.second, unit, color);
            }
        }

        // Draw a box made out of the unicode box drawing characters
        //
        // starty: y position of the top-left corner of the box
        // startx: x position of the top-left corner of the box
        // dimy: length of the vertical lines that make up the box (characters between top and bottom borders = dimy - 2)
        // dimx: length of the horizontal lines that make up the box (characters between left and right borders = dimx - 2)
        // style: standard style/dash set used throughout BSCREEN
        // color: color pair number to use when drawing the box (does not fill in the center with a background color)
        void dbox(unsigned short starty, unsigned short startx, unsigned short dimy, unsigned short dimx, std::pair<unsigned char, unsigned char> style = {LD_Light_Hard, LD_Dashed_None}, unsigned char color = 1) {
            // Box must be fully within the screen to be made
            if (starty < 0 || startx < 0 || starty > Dims.first - 1 || startx > Dims.second - 1 || starty + dimy > Dims.first || startx + dimx > Dims.second) {return;}

            if (dimy < 1 || dimx < 1) {return;}
            
            if (dimy == 1 && dimx == 1) {
                wu(starty, startx, "□", color);
                return;
            }
            else if (dimy == 1) {
                dline(starty, startx, dimx, false, style, color);
                return;
            }
            else if (dimx == 1) {
                dline(starty, startx, dimy, true, style, color);
                return;
            }

            // Draw the box
            dline(starty, startx, dimy, true, style, color);
            dline(starty, startx + dimx - 1, dimy, true, style, color);
            dline(starty, startx, dimx, false, style, color);
            dline(starty + dimy - 1, startx, dimx, false, style, color);
        }
        void dbox(std::pair<unsigned char, unsigned char> style = {LD_Light_Hard, LD_Dashed_None}, unsigned char color = 1) {dbox(0, 0, Dims.first, Dims.second, style, color);}

        // Draw a grid made out of the unicode box drawing characters
        //
        // starty: y position of the top-left corner of the grid
        // startx: x position of the top-left corner of the grid
        // rows: amount of rows in the grid
        // columns: amount of columns in the grid
        // sizey: amount of characters in between the top and bottom borders of a cell
        // sizex: amount of characters in between the left and right borders of a cell
        // style: standard style/dash set used throughout BSCREEN
        // color: color pair number to use when drawing the grid (does not fill in cells with a background color)
        void dgrid(unsigned short starty, unsigned short startx, unsigned short rows, unsigned short columns, unsigned short sizey, unsigned short sizex, std::pair<unsigned char, unsigned char> style = {LD_Light_Hard, LD_Dashed_None}, unsigned char color = 1) {
            if (starty < 0 || startx < 0 || starty > Dims.first - 1 || startx > Dims.second - 1 || rows < 1 || columns < 1 || sizey < 0 || sizex < 0) {return;}

            for (unsigned short i = 0; i < columns + 1; i++) {
                dline(starty, startx + sizex * i + i, calcGridDims(rows, columns, sizey, sizex).first, true, style, color);
            }

            for (unsigned short i = 0; i < rows + 1; i++) {
                dline(starty + sizey * i + i, startx, calcGridDims(rows, columns, sizey, sizex).second, false, style, color);
            }
        }

        // Can be used in conjunction with the dgrid() function to help with positioning the grid rather than manually calculating it
        //
        // starty: y position of the top-left corner of the grid
        // startx: x position of the top-left corner of the grid
        // rows: amount of rows in the grid
        // columns: amount of columns in the grid
        // sizey: amount of characters in between the top and bottom borders of a cell
        // sizex: amount of characters in between the left and right borders of a cell
        std::pair<unsigned short, unsigned short> calcGridDims(unsigned short rows, unsigned short columns, unsigned short sizey, unsigned short sizex) {
            return {rows * sizey + (rows + 1), columns * sizex + (columns + 1)};
        }
} mainScreen; // Similarly to stdscr, mainScreen is a BSCREEN created globally that matches stdscr when its created

// Should be ran at the start of every program that uses BSCREEN
// endwin() will need to be called at the end of the program as well to clean up ncurses
void startScreen() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);
    start_color();
    srand(time(NULL));
    
    init_color(0, 0, 0, 0);       // Black
    init_color(1, 999, 999, 999); // White
    init_color(2, 999, 0, 0);     // Red
    init_color(3, 0, 999, 0);     // Green
    init_color(4, 999, 999, 0);   // Yellow
    init_color(5, 0, 0, 999);     // Blue
    init_color(6, 999, 0, 999);   // Magenta
    init_color(7, 0, 999, 999);   // Cyan
    
    init_pair(0, 1, 0);

    for (unsigned char i = 0; i < 8; i++) {
        init_pair(i + 1, i + 1, 0);
    }
    
    mainScreen = BSCREEN();
}

#endif
