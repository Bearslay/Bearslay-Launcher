#ifndef NCURSES_PLUSPLUS
#define NCURSES_PLUSPLUS

#include <ncurses.h>
#include <vector>
#include <utility>
#include <locale.h>
#include <string>
#include <math.h>
#include <cmath>
#include <ctime>
#include <unordered_map>

namespace npp {
    /// @brief Light box drawing characters with square corners
    #define NPP_LH 0
    /// @brief Light box drawing characters with rounded corners
    #define NPP_LS 1
    /// @brief Heavy box drawing characters in both the horiztonal and vertical directions
    #define NPP_HB 2
    /// @brief Heavy box drawing characters in the horiztonal direction, light in the vertical directions
    #define NPP_HH 3
    /// @brief Heavy box drawing characters in the vertical direction, light in the horizontal directions
    #define NPP_HV 4
    /// @brief Doubled box drawing characters in both the horizontal and vertical directions
    #define NPP_DB 5
    /// @brief Doubled box drawing characters in the horiztonal direction, light in the vertical directions
    #define NPP_DH 6
    /// @brief Doubled box drawing characters in the vertical direction, light in the horizontal directions
    #define NPP_DV 7

    /// @brief Solid Line
    #define NPP_DN 0
    /// @brief Dashed line with one gap per character
    #define NPP_DD 1
    /// @brief Dashed line with two gaps per character
    #define NPP_DT 2
    /// @brief Dashed line with three gap per character
    #define NPP_DQ 3
    /// @brief Half of a solid line used as a dashed line (left/up)
    #define NPP_DS1 4
    /// @brief Half of a solid line used as a dashed line (right/down)
    #define NPP_DS2 5

    static struct {
        /// @brief Color pair
        unsigned char Col = 1;

        /// @brief Bold
        bool Bo = false;
        /// @brief Italic
        bool It = false;
        /// @brief Underline
        bool Un = false;
        /// @brief Reversed
        bool Re = false;
        /// @brief Blinking
        bool Bl = false;
        /// @brief Dim
        bool Di = false;
        /// @brief Invisible
        bool In = false;
        /// @brief Standout
        bool St = false;
        /// @brief Protected
        bool Pr = false;
        /// @brief Alternate Character Set
        bool Al = false;

        /// @brief Whether to wrap to the starting x or to the window's x
        bool W = true;
    } Defaults;

    class Window {
        private:
            /// @brief ncurses window that makes this class function
            WINDOW *W;

            /// @brief Y-dimension (rows) of the window
            unsigned short Dy;
            /// @brief X-dimension (cols) of the window
            unsigned short Dx;
            /// @brief Y-position (row) of the top-left corner of the window
            unsigned short Py;
            /// @brief X-position (col) of the top-left corner of the window
            unsigned short Px;

            /// @brief If the user can skip in the wait() function
            bool CS = true;

            /// @brief Contain the data (character/color/attributes) for each cell
            struct Cell {
                /// @brief Character contained in the cell
                wchar_t Ch = ' ';

                /// @brief Color pair used for the cell
                unsigned char Col = 1;

                /// @brief Whether the cell is bolded or not
                bool Bo = false;
                /// @brief Whether the cell is italicized or not
                bool It = false;
                /// @brief Whether the cell is underlined or not
                bool Un = false;
                /// @brief Whether the cell is reversed (color pair) or not
                bool Re = false;
                /// @brief Whether the cell blinks or not
                bool Bl = false;
                /// @brief Whether the cell is dim or not
                bool Di = false;
                /// @brief Whether the cell is invisible or not
                bool In = false;
                /// @brief Whether the cell should stand out or not
                bool St = false;
                /// @brief Whether the cell should be protected or not
                bool Pr = false;
                /// @brief Whether the cell should use the alternate character set or not
                bool Al = false;               
            };
            std::vector<std::vector<Cell>> G;

            /// @brief Write a cell to the ncurses window
            /// @param y Y-position (row) of the cell to write
            /// @param x X-position (col) of the cell to write
            void write(unsigned short y, unsigned short x) {
                // No printing characters outside of the ncurses window
                if (y < 0 || y >= Dy || x < 0 || x >= Dx) {return;}

                wattron(W, COLOR_PAIR(G[y][x].Col));
                toggleAttributes(G[y][x].Bo, G[y][x].It, G[y][x].Un, G[y][x].Re, G[y][x].Bl, G[y][x].Di, G[y][x].In, G[y][x].St, G[y][x].Pr, G[y][x].Al);

                if (G[y][x].Ch == '%') {mvwprintw(W, y, x, "%%");}
                else {
                    std::wstring str; str += G[y][x].Ch;
                    mvwaddwstr(W, y, x, str.c_str());
                }

                wattroff(W, COLOR_PAIR(G[y][x].Col));
                toggleAttributes(G[y][x].Bo, G[y][x].It, G[y][x].Un, G[y][x].Re, G[y][x].Bl, G[y][x].Di, G[y][x].In, G[y][x].St, G[y][x].Pr, G[y][x].Al);
            }

            /// @brief Extract a string input into a set of booleans
            /// @param in Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @returns A vector (size of 10) of booleans representing attributes
            std::vector<bool> extractAttributes(std::string in) {
                for (unsigned int i = 0; i < in.length(); i++) {
                    in[i] = tolower(in[i]);
                }

                std::vector<bool> out = {false, false, false, false, false, false, false, false, false, false};

                if (in.find("bo") != std::string::npos) {out[0] = true;}
                if (in.find("it") != std::string::npos) {out[1] = true;}
                if (in.find("un") != std::string::npos) {out[2] = true;}
                if (in.find("re") != std::string::npos) {out[3] = true;}
                if (in.find("bl") != std::string::npos) {out[4] = true;}
                if (in.find("di") != std::string::npos) {out[5] = true;}
                if (in.find("in") != std::string::npos) {out[6] = true;}
                if (in.find("st") != std::string::npos) {out[7] = true;}
                if (in.find("pr") != std::string::npos) {out[8] = true;}
                if (in.find("al") != std::string::npos) {out[9] = true;}

                return out;
            }
            
            /// @brief Toggle attributes - boolean based (acts as wattron() or wattroff())
            /// @param bo Bold
            /// @param it Italic
            /// @param un Underline
            /// @param re Reverse
            /// @param bl Blink
            /// @param di Dim
            /// @param in Invisible
            /// @param st Standout
            /// @param pr Protected
            /// @param al Altset
            void toggleAttributes(bool bo, bool it, bool un, bool re, bool bl, bool di, bool in, bool st, bool pr, bool al) {
                if (bo) {wattron(W, A_BOLD);}
                else {wattroff(W, A_BOLD);}
                if (it) {wattron(W, A_ITALIC);}
                else {wattroff(W, A_ITALIC);}
                if (un) {wattron(W, A_UNDERLINE);}
                else {wattroff(W, A_UNDERLINE);}
                if (re) {wattron(W, A_REVERSE);}
                else {wattroff(W, A_REVERSE);}
                if (bl) {wattron(W, A_BLINK);}
                else {wattroff(W, A_BLINK);}
                if (di) {wattron(W, A_DIM);}
                else {wattroff(W, A_DIM);}
                if (in) {wattron(W, A_INVIS);}
                else {wattroff(W, A_INVIS);}
                if (st) {wattron(W, A_STANDOUT);}
                else {wattroff(W, A_STANDOUT);}
                if (pr) {wattron(W, A_PROTECT);}
                else {wattroff(W, A_PROTECT);}
                if (al) {wattron(W, A_ALTCHARSET);}
                else {wattroff(W, A_ALTCHARSET);}
            }
            /// @brief Toggle attributes - string based (acts as wattron() or wattroff())
            void toggleAttributes(std::string in) {
                std::vector<bool> a = extractAttributes(in);

                if (a[0]) {wattron(W, A_BOLD);}
                else {wattroff(W, A_BOLD);}
                if (a[1]) {wattron(W, A_ITALIC);}
                else {wattroff(W, A_ITALIC);}
                if (a[2]) {wattron(W, A_UNDERLINE);}
                else {wattroff(W, A_UNDERLINE);}
                if (a[3]) {wattron(W, A_REVERSE);}
                else {wattroff(W, A_REVERSE);}
                if (a[4]) {wattron(W, A_BLINK);}
                else {wattroff(W, A_BLINK);}
                if (a[5]) {wattron(W, A_DIM);}
                else {wattroff(W, A_DIM);}
                if (a[6]) {wattron(W, A_INVIS);}
                else {wattroff(W, A_INVIS);}
                if (a[7]) {wattron(W, A_STANDOUT);}
                else {wattroff(W, A_STANDOUT);}
                if (a[8]) {wattron(W, A_PROTECT);}
                else {wattroff(W, A_PROTECT);}
                if (a[9]) {wattron(W, A_ALTCHARSET);}
                else {wattroff(W, A_ALTCHARSET);}
            }

            /// @brief Pause everything for an amount of milliseconds
            /// @param m Milliseconds to wait for
            /// @param s Whether the wait can be skipped (with a keyboard input) or not
            /// @returns True if the wait was skipped, false if the wait wasn't
            bool wait(unsigned long m, bool s = true) {
                for (unsigned long i = 0; i < m; i++) {
                    if (s) {
                        if (gch(true, false, false) != ERR) {return true;}
                    }
                    napms(1);
                }
                return false;
            }

            /// @brief Check if the coordinates are within the window or not
            /// @param p Pairy-position (row) of the coordinate to check
            /// @returns True if the coordinates are in the window, false if the coordinates aren't
            bool checkCoord(std::pair<unsigned short, unsigned short> p) {return (p.first < 0 || p.first >= Dy || p.second < 0 || p.second >= Dx);}
            /// @brief Check if the coordinates are within the window or not
            /// @param y y-position (row) of the coordinate to check
            /// @param x x-position (col) of the coordinate to check
            /// @returns True if the coordinates are in the window, false if the coordinates aren't
            bool checkCoord(unsigned short y, unsigned short x) {return checkCoord({y, x});}

        public:
            /// @brief Create an ncursespp Window
            /// @param y Y position (row) for the top-left corner of the Window
            /// @param x X position (col) for the top-left corner of the Window
            /// @param dy Height (rows) of the Window
            /// @param dx Length (cols) of the Window
            Window(unsigned short y = 0, unsigned short x = 0, unsigned short dy = LINES, unsigned short dx = COLS) {
                // Prevent the window from being made out of bounds (and automatically resize ones that may)
                y = (y < 0 || y >= LINES) ? 0 : y;
                x = (x < 0 || x >= COLS) ? 0 : x;
                dy = (dy < 1 || LINES - dy - y < 0) ? LINES - y : dy;
                dx = (dx < 1 || COLS - dx - x < 0) ? COLS - x : dx;

                W = newwin(dy, dx, y, x);
                Py = y;
                Px = x;
                Dy = dy;
                Dx = dx;

                for (unsigned short i = 0; i < Dy; i++) {
                    G.emplace_back();
                    for (unsigned short j = 0; j < Dx; j++) {
                        G[i].emplace_back();
                    }
                }
            }
            /// @brief Create an auto-centered Window relative to the screen
            /// @param dy Height (rows) of the Window
            /// @param dx Length (cols) of the Window
            Window(unsigned short dy, unsigned short dx) {
                unsigned short y = LINES / 2 - dy / 2;
                unsigned short x = COLS / 2 - dx / 2;
                Window(y, x, dy, dx);
            }
            /// @brief Create an auto-centered Window relative to another window
            /// @param w Another window to center the new one to
            /// @param dy Height (rows) of the Window
            /// @param dx Length (cols) of the Window
            Window(Window &w, unsigned short dy, unsigned short dx) {
                dy = (w.gdy() < dy) ? w.gdy() : dy;
                dx = (w.gdx() < dx) ? w.gdx() : dx;
                unsigned short y = w.gpy() + w.gdy() / 2 - dy / 2;
                unsigned short x = w.gpx() + w.gdx() / 2 - dx / 2;
                Window(y, x, dy, dx);
            }

            //
            // GETTING WINDOW/CELL ATTRIBUTES
            //

            /// @brief Get the y-dimension (rows) of a window
            /// @returns The y-dimension (rows) of a window
            const unsigned short gdy() {return Dy;}
            /// @brief Get the x-dimension (cols) of a window
            /// @returns The x-dimension (cols) of a window
            const unsigned short gdx() {return Dx;}
            /// @brief Get the y-position (row) of the top-left corner of a window
            /// @returns The y-dimension (row) of the top-left corner of a window
            const unsigned short gpy() {return Py;}
            /// @brief Get the x-position (col) of the top-left corner of a window
            /// @returns The x-dimension (col) of the top-left corner of a window
            const unsigned short gpx() {return Px;}

            /// @brief Get the character of a cell from a window
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A string (length of 1) representing a cell's character or a space if the requested cell doesn't exist
            const wchar_t sch(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? ' ' : G[y][x].Ch;}
            /// @brief Get the color pair of a cell from a window
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns The number of the color pair a cell uses or the default pair for the window if the requested cell doesn't exist
            const unsigned char scol(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.Col : G[y][x].Col;}
            /// @brief Get whether a cell from a window is bolded or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is bolded or the default state for the window if the requested cell doesn't exist
            const bool sbo(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.Bo : G[y][x].Bo;}
            /// @brief Get whether a cell from a window is italicized or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is italicized or the default state for the window if the requested cell doesn't exist
            const bool sit(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.It : G[y][x].It;}
            /// @brief Get whether a cell from a window is underlined or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is underlined or the default state for the window if the requested cell doesn't exist
            const bool sun(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.Un : G[y][x].Un;}
            /// @brief Get whether a cell from a window is reversed or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is reversed or the default state for the window if the requested cell doesn't exist
            const bool sre(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.Re : G[y][x].Re;}
            /// @brief Get whether a cell from a window is blinking or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is blinking or the default state for the window if the requested cell doesn't exist
            const bool sbl(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.Bl : G[y][x].Bl;}
            /// @brief Get whether a cell from a window is dim or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is dim or the default state for the window if the requested cell doesn't exist
            const bool sdi(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.Di : G[y][x].Di;}
            /// @brief Get whether a cell from a window is invisible or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is invisible or the default state for the window if the requested cell doesn't exist
            const bool sin(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.In : G[y][x].In;}
            /// @brief Get whether a cell from a window is standing out or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is standing out or the default state for the window if the requested cell doesn't exist
            const bool sst(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.St : G[y][x].St;}
            /// @brief Get whether a cell from a window is protected or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is protected or the default state for the window if the requested cell doesn't exist
            const bool spr(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.Pr : G[y][x].Pr;}
            /// @brief Get whether a cell from a window is using the alternate character set or not
            /// @param y y-position (row) of the cell to scan
            /// @param x x-position (col) of the cell to scan
            /// @returns A true or false for if the cell is using the alternate character set or the default state for the window if the requested cell doesn't exist
            const bool sal(unsigned short y, unsigned short x) {return (checkCoord(y, x)) ? Defaults.Al : G[y][x].Al;}

            //
            // WRITING TO WINDOW
            //

            /// @brief Clear the window without removing cell data
            void clear() {wclear(W);}
            /// @brief Clear the window and remove cell data
            void reset() {
                for (unsigned short i = 0; i < Dy; i++) {
                    for (unsigned short j = 0; j < Dx; j++) {
                        G[i][j] = Cell();
                    }
                }
                wclear(W);
            }

            /// @brief Write a string to the window - pair pos, pair return
            /// @param p Pair consisting of a y-position (row) and an x-position (col) indicating the start of the string
            /// @param in String input to be written (unicode friendly)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns A pair consisting of the y-position (row) and x-position (col) of the last character of the string that was written
            std::pair<unsigned short, unsigned short> wsp(std::pair<unsigned short, unsigned short> p, std::wstring in, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {
                if (p.first < 0 || p.first >= Dy || p.second < 0 || p.second >= Dx) {return p;}

                std::vector<bool> att = extractAttributes(a);
            
                unsigned short yo = 0, xo = 0;
                for (unsigned short i = 0; i < in.length(); i++) {
                    // Automatically wrap the string back to the starting x-pos
                    if (p.second + i - xo > Dx - 1) {
                        yo++;
                        xo += ((w) ? (Dx - p.second) : Dx);
                    }

                    // Quit early if the string goes out of bounds
                    if (p.first + yo > Dy - 1 || p.first + yo < 0 || p.second + i - xo < 0) {break;}

                    G[p.first + yo][p.second + i - xo].Ch = in[i];
                    G[p.first + yo][p.second + i - xo].Col = c;
                    
                    G[p.first + yo][p.second + i - xo].Bo = att[0];
                    G[p.first + yo][p.second + i - xo].It = att[1];
                    G[p.first + yo][p.second + i - xo].Un = att[2];
                    G[p.first + yo][p.second + i - xo].Re = att[3];
                    G[p.first + yo][p.second + i - xo].Bl = att[4];
                    G[p.first + yo][p.second + i - xo].Di = att[5];
                    G[p.first + yo][p.second + i - xo].In = att[6];
                    G[p.first + yo][p.second + i - xo].St = att[7];
                    G[p.first + yo][p.second + i - xo].Pr = att[8];
                    G[p.first + yo][p.second + i - xo].Al = att[9];
                }

                return {p.first + yo + o.first, p.second + in.length() - xo + o.second - 1};
            }
            /// @brief Write a string to the window - short short pos, pair return
            /// @param y y-position (row) of the start of the string
            /// @param x x-position (col) of the start of the string
            /// @param in String input to be written (unicode friendly)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns A pair consisting of the y-position (row) and x-position (col) of the last character of the string that was written
            std::pair<unsigned short, unsigned short> wsp(unsigned short y, unsigned short x, std::wstring in, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wsp({y, x}, in, c, a, o, w);}
            /// @brief Write a string to the window - pair pos, short return
            /// @param p Pair consisting of a y-position (row) and an x-position (col) indicating the start of the string
            /// @param in String input to be written (unicode friendly)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns The y-position (row) of the last character of the string that was written
            unsigned short wsy(std::pair<unsigned short, unsigned short> p, std::wstring in, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wsp(p, in, c, a, o, w).first;}
            /// @brief Write a string to the window - short short pos, short return
            /// @param y y-position (row) of the start of the string
            /// @param x x-position (col) of the start of the string
            /// @param in String input to be written (unicode friendly)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns The y-position (row) of the last character of the string that was written
            unsigned short wsy(unsigned short y, unsigned short x, std::wstring in, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wsy({y, x}, in, c, a, o, w);}
            /// @brief Write a string to the window - pair pos, short return
            /// @param p Pair consisting of a y-position (row) and an x-position (col) indicating the start of the string
            /// @param in String input to be written (unicode friendly)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns The x-position (col) of the last character of the string that was written
            unsigned short wsx(std::pair<unsigned short, unsigned short> p, std::wstring in, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wsp(p, in, c, a, o, w).second;}
            /// @brief Write a string to the window - short short pos, short return
            /// @param y y-position (row) of the start of the string
            /// @param x x-position (col) of the start of the string
            /// @param in String input to be written (unicode friendly)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns The x-position (col) of the last character of the string that was written
            unsigned short wsx(unsigned short y, unsigned short x, std::wstring in, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wsx({y, x}, in, c, a, o, w);}
            /// @brief Write a string to the window - pair pos, no return
            /// @param p Pair consisting of a y-position (row) and an x-position (col) indicating the start of the string
            /// @param in String input to be written (unicode friendly)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param w Whether to wrap to starting x or not
            void ws(std::pair<unsigned short, unsigned short> p, std::wstring in, unsigned char c = Defaults.Col, std::string a = "", bool w = Defaults.W) {wsp(p, in, c, a, {0, 0}, w);}
            /// @brief Write a string to the window - short short pos, no return
            /// @param y y-position (row) of the start of the string
            /// @param x x-position (col) of the start of the string
            /// @param in String input to be written (unicode friendly)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param w Whether to wrap to starting x or not
            void ws(unsigned short y, unsigned short x, std::wstring in, unsigned char c = Defaults.Col, std::string a = "", bool w = Defaults.W) {ws({y, x}, in, c, a, w);}

            /// @brief Write an integer to the window - pair pos, pair return
            /// @param p Pair consisting of a y-position (row) and an x-position (col) indicating the start of the integer
            /// @param in Integer input to be written
            /// @param m Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns A pair consisting of the y-position (row) and x-position (col) of the last character of the integer that was written
            std::pair<unsigned short, unsigned short> wip(std::pair<unsigned short, unsigned short> p, int in, unsigned short m = 0, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {
                if (p.first < 0 || p.first >= Dy || p.second < 0 || p.second >= Dx) {return p;}

                // Convert number to a string that can be written to the window
                std::wstring n = std::to_wstring(in);
                unsigned short l = n.length();

                // Add leading zeros; negative numbers are accounted for by keeping the negative sign in front
                for (int i = 0; i < m - l; i++) {
                    n.insert(((in < 0) ? n.begin() + 1 : n.begin()), 1, '0');
                }

                return wsp(p, n, c, a, o, w);
            }
            /// @brief Write an integer to the window - short short pos, pair return
            /// @param y y-position (row) of the start of the integer
            /// @param x x-position (col) of the start of the integer
            /// @param in Integer input to be written
            /// @param m Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns A pair consisting of the y-position (row) and x-position (col) of the last character of the integer that was written
            std::pair<unsigned short, unsigned short> wip(unsigned short y, unsigned short x, int in, unsigned short m = 0, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wip({y, x}, in, m, c, a, o, w);}
            /// @brief Write an integer to the window - pair pos, short return
            /// @param p Pair consisting of a y-position (row) and an x-position (col) indicating the start of the integer
            /// @param in Integer input to be written
            /// @param m Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns The y-position (row) of the last character of the integer that was written
            unsigned short wiy(std::pair<unsigned short, unsigned short> p, int in, unsigned short m = 0, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wip(p, in, m, c, a, o, w).first;}
            /// @brief Write an integer to the window - short short pos, short return
            /// @param y y-position (row) of the start of the integer
            /// @param x x-position (col) of the start of the integer
            /// @param in Integer input to be written
            /// @param m Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns The y-position (row) of the last character of the integer that was written
            unsigned short wiy(unsigned short y, unsigned short x, int in, unsigned short m = 0, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wiy({y, x}, in, m, c, a, o, w);}
            /// @brief Write an integer to the window - pair pos, short return
            /// @param p Pair consisting of a y-position (row) and an x-position (col) indicating the start of the integer
            /// @param in Integer input to be written
            /// @param m Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns The x-position (col) of the last character of the integer that was written
            unsigned short wix(std::pair<unsigned short, unsigned short> p, int in, unsigned short m = 0, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wip(p, in, m, c, a, o, w).second;}
            /// @brief Write an integer to the window - short short pos, short return
            /// @param y y-position (row) of the start of the integer
            /// @param x x-position (col) of the start of the integer
            /// @param in Integer input to be written
            /// @param m Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param o Pair consisiting of a y-offset (rows) and an x-offset (cols) that changes the return value
            /// @param w Whether to wrap to starting x or not
            /// @returns The x-position (col) of the last character of the integer that was written
            unsigned short wix(unsigned short y, unsigned short x, int in, unsigned short m = 0, unsigned char c = Defaults.Col, std::string a = "", std::pair<unsigned short, unsigned short> o = {0, 1}, bool w = Defaults.W) {return wix({y, x}, in, m, c, a, o, w);}
            /// @brief Write an integer to the window - pair pos, no return
            /// @param p Pair consisting of a y-position (row) and an x-position (col) indicating the start of the integer
            /// @param in Integer input to be written
            /// @param m Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param w Whether to wrap to starting x or not
            void wi(std::pair<unsigned short, unsigned short> p, int in, unsigned short m = 0, unsigned char c = Defaults.Col, std::string a = "", bool w = Defaults.W) {wip(p, in, m, c, a, {0, 0}, w);}
            /// @brief Write an integer to the window - short short pos, no return
            /// @param y y-position (row) of the start of the integer
            /// @param x x-position (col) of the start of the integer
            /// @param in Integer input to be written
            /// @param m Minimum width for the integer to be (leading zeros will be added to meet the width)
            /// @param c Color pair to use when writing each cell
            /// @param a Set of attributes to unapply (in any order): bo = Bold, it = Italic, un = Underline, re = Reverse, bl = Blink, di = Dim, in = Invisible, st = Standout, pr = Protected, al = Altset
            /// @param w Whether to wrap to starting x or not
            void wi(unsigned short y, unsigned short x, int in, unsigned short m = 0, unsigned char c = Defaults.Col, std::string a = "", bool w = Defaults.W) {wi({y, x}, in, m, c, a, w);}

            //
            // GET USER INPUT
            //

            /// @brief Get a single character input from the user (acts as the ncursespp version of wgetch())
            /// @param enableKeypad Allow the use of arrow keys and such
            /// @param pause Pause the program until an input is read
            /// @param autoRender Automatically render the window when the function is called
            /// @returns An integer that can be used in the same way that it is in the ncurses getch() function
            int gch(bool enableKeypad = true, bool pause = true, bool autoRender = true) {
                if (autoRender) {ri();}

                if (enableKeypad) {keypad(W, true);}
                else {keypad(W, false);}
                if (pause) {nodelay(W, false);}
                else {nodelay(W, true);}

                return wgetch(W);
            }

            /// @brief Get a string input from the user (acts as the ncursespp version of mvwgetnstr())
            /// @param y y-position (row) of the start of the input location
            /// @param x x-position (col) of the start of the input location
            /// @param max Maximum amount of characters the string can contain
            /// @param echoColor Color pair to use when echoing the user's input
            /// @param echoAtt Set of attributes to apply to the echoed string
            /// @param showStr Whether to echo the user's input or not
            /// @param showCursor Whether to show the cursor when taking an input or not
            /// @param enableKeypad Whether to allow the use of the keypad when taking the input
            /// @returns A wide string (not unicode friendly :/)
            std::wstring gstr(unsigned short y, unsigned short x, int m = 255, unsigned char c = 1, std::string a = "", bool autoWrite = true, bool showStr = true, bool showCursor = true, bool enableKeypad = true) {
                char in[m];

                if (enableKeypad) {keypad(W, true);}
                if (showStr) {echo();}
                if (showCursor) {curs_set(1);}

                wattron(W, COLOR_PAIR(c));
                toggleAttributes(a);

                mvwgetnstr(W, y, x, in, m);

                wattroff(W, COLOR_PAIR(c));
                toggleAttributes(a);

                std::wstring out;
                for (int i = 0; i < m; i++) {
                    if (in[i] == '\0') {break;}
                    out += in[i];
                }

                if (autoWrite) {
                    wsx(y, x, out, c, a);
                }

                curs_set(0);
                noecho();
                keypad(W, false);

                return out;
            }

            //
            // RENDERING THE WINDOW
            //

            /// @brief Render the window instantly
            void ri() {
                for (unsigned short i = 0; i < Dy; i++) {
                    for (unsigned short j = 0; j < Dx; j++) {
                        write(i, j);
                    }
                }
                wrefresh(W);
            }

            /// @brief Render the window line-by-line (and char-by-char if indicated)
            /// @param d Direction to start from: 0 = Top-Left, 1 = Bottom-Left, 2 = Left-Top, 3 = Right-Top
            /// @param f Whether to render the full line at once or not
            /// @param r Whether to reverse the char-by-char direction of rendering (has no effect if f is true)
            /// @param m Milliseconds to wait in between each line/character rendering
            void rl(unsigned char d = 0, bool f = true, bool r = false, unsigned long m = 20) {
                if (d < 0 || d > 3) {return ri();}

                unsigned short l1 = d < 2 ? Dy : Dx;
                unsigned short l2 = d < 2 ? Dx : Dy;
                
                for (unsigned short i = 0; i < l1; i++) {
                    for (unsigned short j = 0; j < l2; j++) {
                        switch (d) {
                            case 0:
                                write(i, r ? Dx - 1 - j : j);
                                break;
                            case 1:
                                write(Dy - 1 - i, r ? Dx - 1 - j : j);
                                break;
                            case 2:
                                write(r ? Dy - 1 - j : j, i);
                                break;
                            case 3:
                                write(r ? Dy - 1 - j : j, Dx - 1 -i);
                                break;
                        }

                        if (!f) {
                            wrefresh(W);
                            if (wait(m, CS)) {return ri();}
                        }
                    }
                    
                    wrefresh(W);
                    if (wait(m, CS)) {return ri();}
                }
            }
            /// @brief Render the window line-by-line (and char-by-char if indicated) from the top of the window
            /// @param f Whether to render the full line at once or not
            /// @param r Whether to reverse the char-by-char direction of rendering (has no effect if f is true)
            /// @param m Milliseconds to wait in between each line/character rendering
            void rlt(bool f = true, bool r = false, unsigned long m = 20) {rl(0, f, r, m);}
            /// @brief Render the window line-by-line (and char-by-char if indicated) from the bottom of the window
            /// @param f Whether to render the full line at once or not
            /// @param r Whether to reverse the char-by-char direction of rendering (has no effect if f is true)
            /// @param m Milliseconds to wait in between each line/character rendering
            void rlb(bool f = true, bool r = false, unsigned long m = 20) {rl(1, f, r, m);}
            /// @brief Render the window line-by-line (and char-by-char if indicated) from the left of the window
            /// @param f Whether to render the full line at once or not
            /// @param r Whether to reverse the char-by-char direction of rendering (has no effect if f is true)
            /// @param m Milliseconds to wait in between each line/character rendering
            void rll(bool f = true, bool r = false, unsigned long m = 20) {rl(2, f, r, m);}
            /// @brief Render the window line-by-line (and char-by-char if indicated) from the right of the window
            /// @param f Whether to render the full line at once or not
            /// @param r Whether to reverse the char-by-char direction of rendering (has no effect if f is true)
            /// @param m Milliseconds to wait in between each line/character rendering
            void rlr(bool f = true, bool r = false, unsigned long m = 20) {rl(3, f, r, m);}

        private:
            std::vector<std::pair<wchar_t, std::vector<unsigned char>>> key = {
                // Horizontal and vertical lines off all 6 dash styles (none, single 1, single 2, double, triple, quadruple)
                {L'─', {0, 0, 1, 1}}, {L'╌', {0, 0, 1, 1}}, {L'┄', {0, 0, 1, 1}}, {L'┈', {0, 0, 1, 1}}, {L'╴', {0, 0, 1, 1}}, {L'╶', {0, 0, 1, 1}},
                {L'━', {0, 0, 2, 2}}, {L'╍', {0, 0, 2, 2}}, {L'┅', {0, 0, 2, 2}}, {L'┉', {0, 0, 2, 2}}, {L'╸', {0, 0, 2, 2}}, {L'╺', {0, 0, 2, 2}},
                {L'│', {1, 1, 0, 0}}, {L'╎', {1, 1, 0, 0}}, {L'┆', {1, 1, 0, 0}}, {L'┊', {1, 1, 0, 0}}, {L'╵', {1, 1, 0, 0}}, {L'╷', {1, 1, 0, 0}},
                {L'┃', {2, 2, 0, 0}}, {L'╏', {2, 2, 0, 0}}, {L'┇', {2, 2, 0, 0}}, {L'┋', {2, 2, 0, 0}}, {L'╹', {2, 2, 0, 0}}, {L'╻', {2, 2, 0, 0}},
                // Corners
                {L'┌', {0, 1, 0, 1}}, {L'┍', {0, 1, 0, 2}}, {L'┎', {0, 2, 0, 1}}, {L'┏', {0, 2, 0, 2}}, {L'┐', {0, 1, 1, 0}}, {L'┑', {0, 1, 2, 0}}, {L'┒', {0, 2, 1, 0}}, {L'┓', {0, 2, 2, 0}},
                {L'└', {1, 0, 0, 1}}, {L'┕', {1, 0, 0, 2}}, {L'┖', {2, 0, 0, 1}}, {L'┗', {2, 0, 0, 2}}, {L'┘', {1, 0, 1, 0}}, {L'┙', {1, 0, 2, 0}}, {L'┚', {2, 0, 1, 0}}, {L'┛', {2, 0, 2, 0}},
                // Tees
                {L'├', {1, 1, 0, 1}}, {L'┝', {1, 1, 0, 2}}, {L'┞', {2, 1, 0, 1}}, {L'┟', {1, 2, 0, 1}}, {L'┠', {2, 2, 0, 1}}, {L'┡', {2, 1, 0, 2}}, {L'┢', {1, 2, 0, 2}}, {L'┣', {2, 2, 0, 2}},
                {L'┤', {1, 1, 1, 0}}, {L'┥', {1, 1, 2, 0}}, {L'┦', {2, 1, 1, 0}}, {L'┧', {1, 2, 1, 0}}, {L'┨', {2, 2, 1, 0}}, {L'┩', {2, 1, 2, 0}}, {L'┪', {1, 2, 2, 0}}, {L'┫', {2, 2, 2, 0}},
                {L'┬', {0, 1, 1, 1}}, {L'┭', {0, 1, 2, 1}}, {L'┮', {0, 1, 1, 2}}, {L'┯', {0, 1, 2, 2}}, {L'┰', {0, 2, 1, 1}}, {L'┱', {0, 2, 2, 1}}, {L'┲', {0, 2, 1, 2}}, {L'┳', {0, 2, 2, 2}},
                {L'┴', {1, 0, 1, 1}}, {L'┵', {1, 0, 2, 1}}, {L'┶', {1, 0, 1, 2}}, {L'┷', {1, 0, 2, 2}}, {L'┸', {2, 0, 1, 1}}, {L'┹', {2, 0, 2, 1}}, {L'┺', {2, 0, 1, 2}}, {L'┻', {2, 0, 2, 2}},
                // Crosses
                {L'┼', {1, 1, 1, 1}}, {L'┽', {1, 1, 2, 1}}, {L'┾', {1, 1, 1, 2}}, {L'┿', {1, 1, 2, 2}}, {L'╀', {2, 1, 1, 1}}, {L'╁', {1, 2, 1, 1}}, {L'╂', {2, 2, 1, 1}}, {L'╃', {2, 1, 2, 1}},
                {L'╄', {2, 1, 1, 2}}, {L'╅', {1, 2, 2, 1}}, {L'╆', {1, 2, 1, 2}}, {L'╇', {2, 2, 1, 2}}, {L'╈', {1, 2, 2, 2}}, {L'╉', {2, 2, 2, 1}}, {L'╊', {2, 2, 1, 2}}, {L'╋', {2, 2, 2, 2}},
                // Double
                {L'═', {0, 0, 3, 3}}, {L'║', {3, 3, 0, 0}}, {L'╒', {0, 1, 0, 3}}, {L'╓', {0, 3, 0, 1}}, {L'╔', {0, 3, 0, 3}}, {L'╕', {0, 1, 3, 0}}, {L'╖', {0, 0, 3, 1}}, {L'╗', {0, 0, 3, 3}},
                {L'╘', {1, 0, 0, 3}}, {L'╙', {3, 0, 0, 1}}, {L'╚', {3, 0, 0, 3}}, {L'╛', {1, 0, 3, 0}}, {L'╜', {3, 0, 1, 0}}, {L'╝', {3, 0, 3, 0}},
                {L'╞', {1, 1, 0, 3}}, {L'╟', {3, 3, 0, 1}}, {L'╠', {3, 3, 0, 3}}, {L'╡', {1, 1, 3, 0}}, {L'╢', {3, 3, 1, 0}}, {L'╣', {3, 3, 3, 0}}, {L'╤', {0, 1, 3, 3}}, {L'╥', {0, 3, 1, 1}},
                {L'╦', {0, 3, 3, 3}}, {L'╧', {1, 0, 3, 3}}, {L'╨', {3, 0, 1, 1}}, {L'╩', {3, 0, 3, 3}},
                {L'╪', {1, 1, 3, 3}}, {L'╫', {3, 3, 1, 1}}, {L'╬', {3, 3, 3, 3}},
                // Soft corners and thickness transitions
                {L'╭', {0, 1, 0, 1}}, {L'╮', {0, 1, 1, 0}}, {L'╯', {1, 0, 1, 0}}, {L'╰', {1, 0, 0, 1}}, {L'╼', {0, 0, 1, 2}}, {L'╽', {1, 2, 0, 0}}, {L'╾', {0, 0, 2, 1}}, {L'╿', {2, 1, 0, 0}}
            };

            /// @brief Get a number corresponding to the type of line that would be needed to connect to the specified character in the specified direction
            /// @param c The character to connect to
            /// @param d The direction to check: 0 = Up/North, 1 = Down/South, 2 = Left/West, 3 = Right/East
            /// @returns A number 0-3: 0 = Nothing, 1 = Light, 2 = Heavy, 3 = Double
            const unsigned char getConnectStyle(wchar_t c, unsigned char d) {
                for (unsigned char i = 0; i < key.size(); i++) {
                    if (c == key[i].first) {
                        return key[i].second[d];
                    }
                }

                return 0;
            }
            /// @brief Get a number corresponding to the type of line that would be needed to connect to the specified character in the specified direction
            /// @param y y-position (row) of the character to connect to
            /// @param x x-position (col) of the character to connect to
            /// @param d The direction to check: 0 = Up/North, 1 = Down/South, 2 = Left/West, 3 = Right/East
            /// @returns A number 0-3: 0 = Nothing, 1 = Light, 2 = Heavy, 3 = Double
            const unsigned char getConnectStyle(unsigned short y, unsigned short x, unsigned char d) {
                if (y < 0 || y >= Dy || x < 0 || x >= Dx || d < 0 || d > 3) {return 0;}

                return getConnectStyle(sch(y, x), d);
            }

        public:
            void test(unsigned short y, unsigned short x) {
                wi(y - 1, x, getConnectStyle(y, x, 0));
                wi(y + 1, x, getConnectStyle(y, x, 1));
                wi(y, x - 1, getConnectStyle(y, x, 2));
                wi(y, x + 1, getConnectStyle(y, x, 3));
            }

            void dvl(unsigned short y, unsigned short x, int l, std::pair<unsigned char, unsigned char> s = {NPP_LH, NPP_DN}, unsigned char c = 1) {
                if ((l == 0) || (y < 0 && l < 0) || (y >= Dy && l > 0)) {return;}
                y = (y < 0) ? 0 : ((y >= Dy) ? Dy - 1 : y);
                x = (x < 0) ? 0 : ((x >= Dx) ? Dx - 1 : x);

                /// @brief Cutoff: The amount of characters the line will take up (taking into account boundaries of the window)
                const unsigned short cf = ((l > 0 && Dy - l < y) ? Dy - y : ((l < 0 && y - l < 0) ? y : abs(l)));

                // For each character that the line is long, do the things to draw the line
                for (int i = 0; abs(i) < cf; i += ((l < 0) ? -1 : 1)) {
                    /// @brief Current Y: The y-position (row) that the loop will be looking to write the next character
                    unsigned char cy = y + i;

                    /// @brief The connection styles that the surrounding characters use
                    std::vector<unsigned char> cs = {getConnectStyle(cy - 1, x, 0), getConnectStyle(cy + 1, x, 1), getConnectStyle(cy, x - 1, 2), getConnectStyle(cy, x + 1, 3)};
                


                    // Write the appropriate character to the window
                    ws(cy, x, L"", c);
                }
            }
            
    } main;

    /// @brief Acts as the ncursespp version of initscr(); endwin() must still be called at the end of a program
    /// @returns True or false for whether the user's terminal will have full color support
    bool init() {
        setlocale(LC_ALL, "");
        initscr();
        noecho();
        curs_set(0);
        srand(time(NULL));

        if (!has_colors() || !can_change_color()) {
            mvwprintw(stdscr, LINES / 2 - 1, COLS / 2 - 50, "Warning: Your terminal doesn't support full color capabilites...");
            mvwprintw(stdscr, LINES / 2, COLS / 2 - 50, "ncursespp requires a terminal that allows for the changing of colors, so go find one that will work");
            mvwprintw(stdscr, LINES / 2 + 1, COLS / 2 - 50, "Press any key to continue...");
            wgetch(stdscr);
            wclear(stdscr);

            return false;
        }

        start_color();

        init_color(0, 0, 0, 0);        // Black
        init_color(1, 999, 999, 999);  // White
        init_color(2, 999, 0, 0);      // Red
        init_color(3, 0, 999, 0);      // Green
        init_color(4, 999, 999, 0);    // Yellow
        init_color(5, 0, 0, 999);      // Blue
        init_color(6, 999, 0, 999);    // Magenta
        init_color(7, 0, 999, 999);    // Cyan

        init_pair(0, 1, 0);
        for (unsigned char i = 0; i < 8; i++) {
            init_pair(i + 1, i + 1, 0);
        }

        main = Window();

        return true;
    }
}

#endif
