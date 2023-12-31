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

    // Apply transformation if there are no collisions
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

    // Apply transformation if there are no collisions
    if (rotPossible) {
        tmpT.rot = MOD4(tmpT.rot-1);    // Update rotation
        gs->falling_tetromino = tmpT;   // Update falling tetromino with transformed tetromino
    }

    return BETRIS_SUCCESS;
}

// Shifts falling tetromino to the left
betris_error_t betris_leftshift(betris_gamestate_t* gs)
{
    betris_tetromino_t tmpT;
    uint8_t shiftPossible;

    // Verify gamestate object is valid
    if (!gs) {
        return BETRIS_NULL_GAMESTATE;
    }
    if (gs->initialized == BETRIS_INIT) {
        return BETRIS_NOT_INITIALIZED;
    }

    // Check if left shift is possible
    tmpT = gs->falling_tetromino;   // Copy tetromino
    for (int i = 0; i < 4; i++) {   // Apply left shift
        tmpT.pos[i].w = tmpT.pos[i].w - 1;
    }
    shiftPossible = betris_checkCollision(&(gs->playfield), tmpT);    // Check if there is room for shift

    // Apply shift if there are no collisions
    if (shiftPossible) {
        gs->falling_tetromino = tmpT;
    }

    return BETRIS_SUCCESS;
}

// Shifts falling tetromino to the right
betris_error_t betris_rightshift(betris_gamestate_t* gs)
{
    betris_tetromino_t tmpT;
    uint8_t shiftPossible;

    // Verify gamestate object is valid
    if (!gs) {
        return BETRIS_NULL_GAMESTATE;
    }
    if (gs->initialized == BETRIS_INIT) {
        return BETRIS_NOT_INITIALIZED;
    }

    // Check if right shift is possible
    tmpT = gs->falling_tetromino;   // Copy tetromino
    for (int i = 0; i < 4; i++) {   // Apply right shift
        tmpT.pos[i].w = tmpT.pos[i].w + 1;
    }
    shiftPossible = betris_checkCollision(&(gs->playfield), tmpT);    // Check if there is room for shift

    // Apply shift if there are no collisions
    if (shiftPossible) {
        gs->falling_tetromino = tmpT;
    }

    return BETRIS_SUCCESS;
}

// Performs non-locking soft drop
betris_error_t betris_sdrop(betris_gamestate_t* gs)
{
    betris_tetromino_t tmpT;
    uint8_t dropPossible;

    // Verify gamestate object is valid
    if (!gs) {
        return BETRIS_NULL_GAMESTATE;
    }
    if (gs->initialized == BETRIS_INIT) {
        return BETRIS_NOT_INITIALIZED;
    }

    // Check if soft drop is possible
    tmpT = gs->falling_tetromino;   // Copy tetromino
    for (int i = 0; i < 4; i++) {   // Apply soft drop 
        tmpT.pos[i].h = tmpT.pos[i].h - 1;
    }
    dropPossible = betris_checkCollision(&(gs->playfield), tmpT);    // Check if there is room for shift

    // Apply drop if there are no collisions
    if (dropPossible) {
        gs->falling_tetromino = tmpT;
    }
    else {  // Lock tetromino if there is a collision
        lockTetromino(gs);
    }

    return BETRIS_SUCCESS;
}

// Performs locking hard drop
betris_error_t betris_hdrop(betris_gamestate_t* gs)
{
    // betris_tetromino_t tmpT;
    uint8_t dropPossible;

    // Verify gamestate object is valid
    if (!gs) {
        return BETRIS_NULL_GAMESTATE;
    }
    if (gs->initialized == BETRIS_INIT) {
        return BETRIS_NOT_INITIALIZED;
    }

    // Drop tetromino until cant anymore
    do {
        for (int i = 0; i < 4; i++) {   // Apply soft drop
            gs->falling_tetromino.pos[i].h = gs->falling_tetromino.pos[i].h - 1;
        }
        dropPossible = betris_checkCollision(&(gs->playfield), gs->falling_tetromino);  // Check if there is space to drop
    }
    while (dropPossible);

    // Apply corrective jump
    for (int i = 0; i < 4; i++) {
        gs->falling_tetromino.pos[i].h = gs->falling_tetromino.pos[i].h + 1;
    }

    // Lock tetromino
    lockTetromino(gs);

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

/// @brief Places the falling tetromino into the playfield. Afterwords, tetromino color is removed to indicate that it was placed on field. 
/// @param gs Gamestate object
/// @note This is a private function. Doesn't need to be included in header file.
void lockTetromino(betris_gamestate_t* gs) 
{
    // Set tetromino in playfield
    for (int i = 0; i < 4; i++) {
        gs->playfield.arr[gs->falling_tetromino.pos[i].h][gs->falling_tetromino.pos[i].w] = gs->falling_tetromino.color;
    }

    // Clear tetromino color to indicate that it is no longer active
    gs->falling_tetromino.color = BETRIS_BLANK;

    return;
}