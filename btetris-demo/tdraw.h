#include <stdint.h>
#include <ncurses.h>
#include "btetris_game.h"

#ifndef __DTETRIS__
#define __DTETRIS__

extern WINDOW* debug_window;

typedef enum tdraw_error {
    SUCCESS = 0,
    ERROR_NULL_INARG = 1,
    ERROR_NULL_GVAR = 2,
    ERROR_NCURSES_NOCOLOR = 16,
    ERROR_NCURSES_WINDOW = 17,
    ERROR_NCURSES_PRINT = 18
} tdraw_error_t;


/// @brief Draws the start menu. Starts game before returning. 
/// @param game Game object
/// @return Error value
int tdraw_start(tetris_game_t* game);

/// @brief Draws pause menu, pauses game. Unpauses game before returning/
/// @param game Game object
/// @return Error value
int tdraw_pause(tetris_game_t* game);

/// @brief Draws gameover menu. Restarts game before returning
/// @param game Game object
/// @return Error value
int tdraw_gameover(tetris_game_t* game);

/// @brief Draws tetris playfield. 
/// @param game Game object
/// @return Error value
int tdraw_pfield(tetris_game_t* game);

/// @brief Draws tetris piece preview 
/// @param game Game object
/// @return Error value
int tdraw_pprev(tetris_game_t* game);

/// @brief Draws score info
/// @param game Game object
/// @return Error value
int tdraw_score(tetris_game_t* game);

/// @brief Draws debug info
/// @param game Game object
/// @return Error value
int tdraw_ginfo(tetris_game_t* game);

/// @brief Inits ncurses color. Sets up color values.  
/// @return Error value
int tdraw_initcolor();

/// @brief Draws the UI for the first time. 
/// @return Error value
int tdraw_wininit();

/// @brief Updates UI windows. 
/// @return Error value
int tdraw_winupdate();

/// @brief Forces an update for all UI windows
/// @return Error value
int tdraw_touchwin();

#endif