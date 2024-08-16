#include <stdint.h>

#ifdef _WIN32
    #include <pdcurses/curses.h>
#else 
    #include <ncurses.h>
#endif

#ifndef __DTETRIS__
#define __DTETRIS__



int tdraw_wininit();

#endif