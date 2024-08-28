#include <stdint.h>
#include <ncurses.h>
#include "btetris_game.h"

#ifndef __DTETRIS__
#define __DTETRIS__

extern WINDOW* debug_window;

int tdraw_pfield(tetris_board_t* board);

int tdraw_pprev(tetris_game_t* game);

int tdraw_initcolor();

int tdraw_wininit();

int tdraw_winupdate();

int tdraw_touchwin();

#endif