
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
 */
extern const tetris_coord_t TETRIS_TETROMINO_ROTATE[8][4][4];

/* https://tetris.wiki/Super_Rotation_System
 * Wall Kick offsets for J, L, S, T, and Z tetrominoes. 
 * Indexed [TestNumber][StartingRotation][CounterClockwise]
 * - TestNumber: There are 5 test for SRS. The first is the original position for the rotation, and isnt included in the array. 
 * - StartingRotation: frot value
 * - CounterClockwise: if rotation is clockwise, idx = 0. CounterClockwise, idx = 1. 
 */
extern const tetris_coord_t TETRIS_TETROMINO_DEFAULTKICK[4][4][2];

/* https://tetris.wiki/Super_Rotation_System
 * Wall Kick offsets for the I tetromino. 
 */
extern const tetris_coord_t TETRIS_TETROMINO_IKICK[4][4][2];

#endif