
#include "betris_board.h"
#include "betris_game.h"

#ifndef __BETRIS_CONTROL__
#define __BETRIS_CONTROL__


// --- Function Declarations --- //

/// @brief Rotates the falling tetromino clockwise
/// @param game Game object
/// @return Error code
betris_error_t betris_rotcw(betris_game_t* game);

/// @brief Rotates the falling tetromino counter-clockwise
/// @param game Game object
/// @return Error code
betris_error_t betris_rotcntrcw(betris_game_t* game);

/// @brief Shifts falling tetromino to the left
/// @param game Game object
/// @return Error code
betris_error_t betris_leftshift(betris_game_t* game);

/// @brief Shifts falling tetromino to the right
/// @param game Game object
/// @return Error code
betris_error_t betris_rightshift(betris_game_t* game);

/// @brief Performs non-locking soft drop
/// @param game Game object
/// @return Error code
betris_error_t betris_sdrop(betris_game_t* game);

/// @brief Performs locking hard drop
/// @param game Game object
/// @return Error code
betris_error_t betris_hdrop(betris_game_t* game);

/// @brief Calculates the position of the falling tetromino if it were to be hard dropped. 
/// @param game Game object
/// @return Error code
betris_error_t betris_calcGhostCoords(betris_game_t* game);


// --- Constants --- //

/* 
 * Block transformations for clockwise rotations. 
 * Indexed [Color][CurrRotation][BlockIdx].
 * Inverse can be applied for counter-clockwise rotations. 
 * 
 * If there is a better way to make this more readable, I would love to know 
 */


const betris_coord_t BETRIS_TETROMINO_ROTATE[8][4][4] = {

    // Blank
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},

    // Red (Z) 
    {{{0, 2}, {-1, 1}, {0, 0}, {-1, -1}},
    {{-2, 0}, {-1, -1}, {0, 0}, {1, -1}},
    {{0, -2}, {1, -1}, {0, 0}, {1, 1}},
    {{-2, 0}, {1, 1}, {0, 0}, {-1, 1}}},

    // Orange (L)
    {{{1, 1}, {0, 0}, {-1, -1}, {-2, 0}},
    {{-1, 1}, {0, 0}, {1, -1}, {0, -2}},
    {{-1, -1}, {0, 0}, {1, 1}, {2, 0}},
    {{1, -1}, {0, 0}, {-1, 1}, {0, 2}}},

    // Yellow (O)
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},

    // Green (S)
    {{{1, 1}, {0, 0}, {-1, 1}, {-2, 0}},
    {{-1, 1}, {0, 0}, {-1, -1}, {0, -2}},
    {{-1, -1}, {0, 0}, {1, -1}, {2, 0}},
    {{1, -1}, {0, 0}, {1, 1}, {0, 2}}},

    // Cyan (I)
    {{{1, 2}, {0, 1}, {-1, 0}, {-2, -1}},
    {{-2, 1}, {-1, 0}, {0, -1}, {1, -2}},
    {{-1, -2}, {0, -1}, {1, 0}, {2, 1}},
    {{2, -1}, {1, 0}, {0, 1}, {-1, 2}}},

    // Blue (J)
    {{{0, 2}, {1, 1}, {0, 0}, {-1, -1}},
    {{-2, 0}, {-1, 1}, {0, 0}, {1, -1}},
    {{0, -2}, {-1, -1}, {0, 0}, {1, 1}},
    {{2, 0}, {1, -1}, {0, 0}, {-1, 1}}},

    // Purple (T)
    {{{1, 1}, {0, 0}, {-1, 1}, {-1, -1}},
    {{-1, 1}, {0, 0}, {-1, -1}, {1, -1}},
    {{-1, -1}, {0, 0}, {1, -1}, {1, 1}},
    {{1, -1}, {0, 0}, {1, 1}, {-1, 1}}}
};

#endif