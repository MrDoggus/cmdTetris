
#include "betris_board.h"

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

typedef enum betris_error {
    BETRIS_SUCCESS = 0,
    BETRIS_NULL_BOARD,
    BETRIS_INACTIVE_TETROMINO,
    BETRIS_COLLISION
} betris_error_t;

// --- Function Declarations --- //

/// @brief Rotates the falling tetromino clockwise
/// @param board Board object
/// @return Error code
betris_error_t betris_rotcw(betris_board_t* board);

/// @brief Rotates the falling tetromino counter-clockwise
/// @param board Board object
/// @return Error code
betris_error_t betris_rotcntrcw(betris_board_t* board);

/// @brief Shifts falling tetromino to the left
/// @param board Board object
/// @return Error code
betris_error_t betris_leftshift(betris_board_t* board);

/// @brief Shifts falling tetromino to the right
/// @param board Board object
/// @return Error code
betris_error_t betris_rightshift(betris_board_t* board);

/// @brief Performs non-locking soft drop
/// @param board Board object
/// @return Error code
betris_error_t betris_sdrop(betris_board_t* board);

/// @brief Performs locking hard drop
/// @param board Board object
/// @return Error code
betris_error_t betris_hdrop(betris_board_t* board);

/// @brief Calculates the position of the falling tetromino if it were to be hard dropped. 
/// @param board Board object
/// @param tetromino Pointer to store the calculated position. 
/// @return Error code
betris_error_t betris_calcGhostCoords(betris_board_t* board);

#endif