/*
 * Rotate Clockwise
 * Rotate Counter-Clockwise
 * Soft drop
 * Locking hard drop
 * Left shift
 * Right shift
*/
#include "betris_control.h"
#include "betris_board.h"


// https://tetris.fandom.com/wiki/SR

// Used to keep rotation in the range of [0,3]
#define MOD4(val) (val & 0b0011)

void betris_lockTetromino(betris_board_t* board);
uint8_t betris_checkCollision(betris_board_t* board, betris_coord_t tetromino[4]);

// Rotates the falling tetromino clockwise
betris_error_t betris_rotcw(betris_board_t* board) 
{
    betris_coord_t tmpT[4];
    uint8_t rotPossible;

    // Verify gamestate object is valid
    if (!board) {
        return BETRIS_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_INACTIVE_TETROMINO;
    }

    // Copy tetromino
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if rotation is possible in current position
    for (int i = 0; i < 4; i++) {   // Apply transformation
        tmpT[i] = betris_addCoord(tmpT[i], BETRIS_TETROMINO_ROTATE[board->fcol][board->frot][i]);
    }
    rotPossible = betris_checkCollision(board, tmpT);    // Check if there is room for rotation

    // Apply transformation if there are no collisions
    if (rotPossible) {
        board->frot = MOD4(board->frot+1);

        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }

    return BETRIS_SUCCESS;
}

// Rotates the falling tetromino counter-clockwise
betris_error_t betris_rotcntrcw(betris_board_t* board) 
{
    betris_coord_t tmpT[4];
    uint8_t rotPossible;

    // Verify gamestate object is valid
    if (!board) {
        return BETRIS_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_INACTIVE_TETROMINO;
    }

    // Copy tetromino
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if rotation is possible in current position
    for (int i = 0; i < 4; i++) {   // Apply transformation
        tmpT[i] = betris_subCoord(tmpT[i], BETRIS_TETROMINO_ROTATE[board->fcol][MOD4(board->frot-1)][i]);
    }
    rotPossible = betris_checkCollision(board, tmpT);    // Check if there is room for rotation

    // Apply transformation if there are no collisions
    if (rotPossible) {
        board->frot = MOD4(board->frot-1);    // Update rotation

        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }

    return BETRIS_SUCCESS;
}

// Shifts falling tetromino to the left
betris_error_t betris_leftshift(betris_board_t* board)
{
    betris_coord_t tmpT[4];
    uint8_t shiftPossible;

    // Verify gamestate object is valid
    if (!board) {
        return BETRIS_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_INACTIVE_TETROMINO;
    }

    // Copy tetromino
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if left shift is possible
    for (int i = 0; i < 4; i++) {   // Apply left shift
        tmpT[i].w = tmpT[i].w - 1;
    }
    shiftPossible = betris_checkCollision(board, tmpT);    // Check if there is room for shift

    // Apply shift if there are no collisions
    if (shiftPossible) {
        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }

    return BETRIS_SUCCESS;
}

// Shifts falling tetromino to the right
betris_error_t betris_rightshift(betris_board_t* board)
{
    betris_coord_t tmpT[4];
    uint8_t shiftPossible;

    // Verify gamestate object is valid
    if (!board) {
        return BETRIS_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_INACTIVE_TETROMINO;
    }

    // Copy tetromino
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if right shift is possible
    for (int i = 0; i < 4; i++) {   // Apply right shift
        tmpT[i].w = tmpT[i].w + 1;
    }
    shiftPossible = betris_checkCollision(board, tmpT);    // Check if there is room for shift

    // Apply shift if there are no collisions
    if (shiftPossible) {
        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }

    return BETRIS_SUCCESS;
}

// Performs non-locking soft drop
betris_error_t betris_sdrop(betris_board_t* board)
{
    betris_coord_t tmpT[4];
    uint8_t dropPossible;

    // Verify gamestate object is valid
    if (!board) {
        return BETRIS_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_INACTIVE_TETROMINO;
    }

    // Copy tetromino
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];
    
    
    // Apply soft drop 
    for (int i = 0; i < 4; i++) {   
        tmpT[i].h = tmpT[i].h - 1;
    }

    // Check if soft drop is possible
    dropPossible = betris_checkCollision(board, tmpT);    // Check if there is room for shift
    
    // Apply drop if there are no collisions
    if (dropPossible) { 
        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }
    // Lock tetromino if there is a collision
    else {
        betris_lockTetromino(board);
    }

    return BETRIS_SUCCESS;
}

// Performs locking hard drop
betris_error_t betris_hdrop(betris_board_t* board)
{
    // betris_coord_t tmpT[4];
    uint8_t dropPossible;

    // Verify gamestate object is valid
    if (!board) {
        return BETRIS_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_INACTIVE_TETROMINO;
    }

    // Drop tetromino until cant anymore
    do {
        for (int i = 0; i < 4; i++) {   // Apply soft drop
            board->fpos[i].h = board->fpos[i].h - 1;
        }
        dropPossible = betris_checkCollision(board, board->fpos);  // Check if there is space to drop
    }
    while (dropPossible);

    // Apply corrective jump
    for (int i = 0; i < 4; i++) {
        board->fpos[i].h = board->fpos[i].h + 1;
    }

    // Lock tetromino
    betris_lockTetromino(board);

    return BETRIS_SUCCESS;
}

/// @brief Checks if a given tetromino has any collisions with the playfield
/// @param playfield Playfield array used to check collisions with falling tetromino
/// @param tetromino Tetromino to check collisions
/// @return returns 1 if a collision was NOT found, returns 0 if a collision was found. 
uint8_t betris_checkCollision(betris_board_t* board, betris_coord_t tetromino[4]) 
{

    // Check border bounds collisions before checking playfield array
    for (int i = 0; i < 4; i++) 
    {
        if (tetromino[i].h < 0 || tetromino[i].h >= BETRIS_HEIGHT+BETRIS_HEIGHT_BUFF) {
            return 0;
        }

        if (tetromino[i].w < 0 || tetromino[i].w >= BETRIS_WIDTH) {
            return 0;
        }
    }

    // Check collisions with locked/fallen tetrominos
    for (int i = 0; i < 4; i++) 
    {
        if (board->pf[tetromino[i].h][tetromino[i].w] != BETRIS_BLANK) {
            return 0;
        }
    }

    // All checks passed, return 1 to declare no collisions
    return 1;
}

/// @brief Places the falling tetromino into the playfield. Afterwords, tetromino color is removed to indicate that it was placed on field. 
/// @param board Gamestate object
void betris_lockTetromino(betris_board_t* board) 
{
    // Set tetromino in playfield
    for (int i = 0; i < 4; i++) {
        board->pf[board->fpos[i].h][board->fpos[i].w] = board->fcol;
    }

    // Clear tetromino color to indicate that it is no longer active
    board->fcol = BETRIS_BLANK;

    return;
}