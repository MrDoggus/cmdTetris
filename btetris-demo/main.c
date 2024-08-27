
#include "tdraw.h"


int main()
{
    // itits screen. sets up memory and clears screen
    initscr();
    noecho();
    // raw();
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
        #ifdef KEY_RESIZE
        case KEY_RESIZE:
            erase();
            tdraw_winupdate();
            tdraw_touchwin();

            mvprintw(0, 0, "%d", ch);
            clrtoeol();

            if (debug_window) {
                wprintw(debug_window, "REFRESH\n");
                wrefresh(debug_window);
            }
            refresh();
            
            break;
        #endif
        case '\n':
        case KEY_ENTER:
            mvprintw(0, 0, "%d", ch);
            clrtoeol();

            if (debug_window) {
                waddch(debug_window, '\n');
                wrefresh(debug_window);
            }
            refresh();
            break;
        default:
            mvprintw(0, 0, "%d", ch);
            clrtoeol();

            if (debug_window) {
                wprintw(debug_window, "%s", keyname(ch));
                wrefresh(debug_window);
            }
            refresh();
            break;
        }
    }

    //  ends ncurse
    getch();
    endwin();

    return 0;
}