
#include "tdraw.h"


int main()
{
    // itits screen. sets up memory and clears screen
    initscr();

    // Refreshes screen to match whats in buffer
    refresh();
    resize_term(0, 0);  // Force curses to update term size

    tdraw_wininit();

    //  ends ncurse
    getch();
    endwin();

    return 0;
}