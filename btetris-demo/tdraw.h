#include <stdint.h>
#include <ncurses.h>

#ifndef __DTETRIS__
#define __DTETRIS__

extern WINDOW* debug_window;

int tdraw_wininit();

int tdraw_winupdate();

int tdraw_touchwin();

#endif