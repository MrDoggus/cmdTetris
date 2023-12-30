
#include "betris_gamestate.h"

#ifndef __BETRIS_CONTROL__
#define __BETRIS_CONTROL__

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


// --- Function Declarations --- //

/// @brief Rotates the falling tetromino clockwise
/// @param gs Gamestate object
/// @return Error code
betris_error_t betris_rotcw(betris_gamestate_t* gs);

/// @brief Rotates the falling tetromino counter-clockwise
/// @param gs Gamestate object
/// @return Error code
betris_error_t betris_rotcntrcw(betris_gamestate_t* gs);

/// @brief Shifts falling tetromino to the left
/// @param gs Gamestate object
/// @return Error code
betris_error_t betris_leftshift(betris_gamestate_t* gs);

/// @brief Shifts falling tetromino to the right
/// @param gs Gamestate object
/// @return Error code
betris_error_t betris_rightshift(betris_gamestate_t* gs);


#endif