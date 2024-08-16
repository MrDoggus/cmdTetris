
#include "tdraw.h"


int main()
{
    // itits screen. sets up memory and clears screen
    initscr();
    noecho();
    nodelay(stdscr, true);

    // Refreshes screen to match whats in buffer
    refresh();
    resize_term(0, 0);  // Force curses to update term size

    tdraw_wininit();

    int ch;
    while(true)
    {
        ch = getch();
        switch (ch)
        {
        case ERR:
            break;
        case '\n':
        case KEY_ENTER:
            if (debug_window) {
                waddch(debug_window, '\n');
                wrefresh(debug_window);
                refresh();
            }
            break;
        default:
            if (debug_window) {
                wprintw(debug_window, "%s", keyname(ch));
                wrefresh(debug_window);
                refresh();
            }
            break;
        }
    }

    //  ends ncurse
    getch();
    endwin();

    return 0;
}