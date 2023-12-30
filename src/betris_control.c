/*
 * Rotate Clockwise
 * Rotate Counter-Clockwise
 * Soft drop
 * Locking hard drop
 * Left shift
 * Right shift
*/
#include "betris_control.h"
#include "betris_gamestate.h"


// https://tetris.fandom.com/wiki/SR

// Used to keep rotation in the range of [0,3]
#define MOD4(val) val & 0b0011

uint8_t betris_checkCollision(betris_playfield_t* playfield, betris_tetromino_t tetromino);

// Rotates the falling tetromino clockwise
betris_error_t betris_rotcw(betris_gamestate_t* gs) 
{
    betris_tetromino_t tmpT;
    uint8_t rotPossible;

    // Verify gamestate object is valid
    if (!gs) {
        return BETRIS_NULL_GAMESTATE;
    }
    if (gs->initialized == BETRIS_INIT) {
        return BETRIS_NOT_INITIALIZED;
    }

    // Check if rotation is possible in current position
    tmpT = gs->falling_tetromino;   // Copy tetromino
    for (int i = 0; i < 4; i++) {   // Apply transformation
        tmpT.pos[i] = betris_addCoord(tmpT.pos[i], BETRIS_TETROMINO_ROTATE[tmpT.color][tmpT.rot][i]);
    }
    rotPossible = betris_checkCollision(&(gs->playfield), tmpT);    // Check if there is room for rotation

    // Apply transformation if there is room
    if (rotPossible) {
        tmpT.rot = MOD4(tmpT.rot+1);    // Update rotation
        gs->falling_tetromino = tmpT;   // Update falling tetromino with transformed tetromino
    }

    return BETRIS_SUCCESS;
}

// Rotates the falling tetromino counter-clockwise
betris_error_t betris_rotcntrcw(betris_gamestate_t* gs) 
{
    betris_tetromino_t tmpT;
    uint8_t rotPossible;

    // Verify gamestate object is valid
    if (!gs) {
        return BETRIS_NULL_GAMESTATE;
    }
    if (gs->initialized == BETRIS_INIT) {
        return BETRIS_NOT_INITIALIZED;
    }

    // Check if rotation is possible in current position
    tmpT = gs->falling_tetromino;   // Copy tetromino
    for (int i = 0; i < 4; i++) {   // Apply transformation
        tmpT.pos[i] = betris_subCoord(tmpT.pos[i], BETRIS_TETROMINO_ROTATE[tmpT.color][MOD4(tmpT.rot-1)][i]);
    }
    rotPossible = betris_checkCollision(&(gs->playfield), tmpT);    // Check if there is room for rotation

    // Apply transformation if there is room
    if (rotPossible) {
        tmpT.rot = MOD4(tmpT.rot-1);    // Update rotation
        gs->falling_tetromino = tmpT;   // Update falling tetromino with transformed tetromino
    }

    return BETRIS_SUCCESS;
}

/// @brief Checks if a given tetromino has any collisions with the playfield
/// @param playfield Playfield array used to check collisions with falling tetromino
/// @param tetromino Tetromino to check collisions
/// @return returns 1 if a collision was NOT found, returns 0 if a collision was found. 
/// @note This is a private function. Doesn't need to be included in header file.
uint8_t betris_checkCollision(betris_playfield_t* playfield, betris_tetromino_t tetromino) 
{

    // Check border bounds collisions before checking playfield array
    for (int i = 0; i < 4; i++) 
    {
        if (tetromino.pos[i].h < 0 || tetromino.pos[i].h >= BETRIS_HEIGHT+BETRIS_HEIGHT_BUFF) {
            return 0;
        }

        if (tetromino.pos[i].w < 0 || tetromino.pos[i].w >= BETRIS_WIDTH) {
            return 0;
        }
    }

    // Check collisions with locked/fallen tetrominos
    for (int i = 0; i < 4; i++) 
    {
        if (playfield->arr[tetromino.pos[i].h][tetromino.pos[i].w] != BETRIS_BLANK) {
            return 0;
        }
    }

    // All checks passed, return 1 to declare no collisions
    return 1;
}