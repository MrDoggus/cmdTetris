
#include "btetris_board.h"
#include "btetris_game.h"

#ifndef __TETRIS_CONTROL__
#define __TETRIS_CONTROL__


// --- Function Declarations --- //

/// @brief Rotates the falling tetromino clockwise
/// @param game Game object
/// @return Error code
tetris_error_t tetris_rotcw(tetris_game_t* game);

/// @brief Rotates the falling tetromino counter-clockwise
/// @param game Game object
/// @return Error code
tetris_error_t tetris_rotcntrcw(tetris_game_t* game);

/// @brief Shifts falling tetromino to the left
/// @param game Game object
/// @return Error code
tetris_error_t tetris_leftshift(tetris_game_t* game);

/// @brief Shifts falling tetromino to the right
/// @param game Game object
/// @return Error code
tetris_error_t tetris_rightshift(tetris_game_t* game);

/// @brief Performs non-locking soft drop
/// @param game Game object
/// @return Error code
tetris_error_t tetris_sdrop(tetris_game_t* game);

/// @brief Performs locking hard drop
/// @param game Game object
/// @return Error code
tetris_error_t tetris_hdrop(tetris_game_t* game);

/// @brief Calculates the position of the falling tetromino if it were to be hard dropped. 
/// @param game Game object
/// @return Error code
tetris_error_t tetris_calcGhostCoords(tetris_game_t* game);


// --- Constants --- //

/* 
 * Block transformations for clockwise rotations. 
 * Indexed [Color][CurrRotation][BlockIdx].
 * Inverse can be applied for counter-clockwise rotations. 
 * 
 * If there is a better way to make this more readable, I would love to know 
 */


const tetris_coord_t TETRIS_TETROMINO_ROTATE[8][4][4] = {

    // Blank
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},

    // Cyan (I)
    {{{1, 2}, {0, 1}, {-1, 0}, {-2, -1}},
    {{-2, -2}, {-1, -1}, {0, 0}, {1, 1}},
    {{2, 1}, {1, 0}, {0, -1}, {-1, -2}},
    {{-1, -1}, {0, 0}, {1, 1}, {2, 2}}},

    // Blue (J)
    {{{0, 1}, {1, 2}, {0, 0}, {-1, -1}},
    {{-1, -1}, {-1, -1}, {0, 1}, {0, 1}},
    {{1, 1}, {0, 0}, {-1, -2}, {0, -1}},
    {{0, -1}, {0, -1}, {1, 1}, {1, 1}}},

    // Orange (L)
    {{{0, -1}, {0, 1}, {-1, 0}, {-1, 0}},
    {{-1, -1}, {0, 0}, {1, 1}, {0, -2}},
    {{1, 0}, {1, 0}, {0, -1}, {0, 1}},
    {{0, 2}, {-1, -1}, {0, 0}, {1, 1}}},

    // Yellow (O)
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},

    // Green (S)
    {{{0, 0}, {-1, -1}, {0, 2}, {-1, 1}},
    {{-1, 0}, {0, 1}, {-1, -2}, {0, -1}},
    {{1, -1}, {0, -2}, {1, 1}, {0, 0}},
    {{0, 1}, {1, 2}, {0, -1}, {1, 0}}},

    // Purple (T)
    {{{0, 0}, {0, 1}, {0, 1}, {-1, -1}},
    {{-1, -1}, {0, 0}, {0, 0}, {0, 0}},
    {{1, 1}, {0, -1}, {0, -1}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {1, 1}}},

    // Red (Z) 
    {{{0, 2}, {-1, 0}, {0, 1}, {-1, -1}},
    {{-1, -2}, {0, 0}, {-1, -1}, {0, 1}},
    {{1, 1}, {0, -1}, {1, 0}, {0, -2}},
    {{0, -1}, {1, 1}, {0, 0}, {1, 2}}}
};

#endif