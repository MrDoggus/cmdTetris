#include "betris_control.h"
#include "betris_board.h"


// https://tetris.fandom.com/wiki/SR

// Used to keep rotation in the range of [0,3]
#define MOD4(val) (val & 0b0011)

// --- Private Function Declarations --- //
// Don't see why a user would need to use these functions, so they are not included in the header file. 

/// @brief Checks if a given tetromino has any collisions with the playfield
/// @param board Board object 
/// @param tetromino Tetromino to check collisions with board playfield
/// @return returns 1 if a collision was NOT found, returns 0 if a collision was found. 
int8_t betris_collisionCheck(betris_board_t* board, betris_coord_t tetromino[4]);

/// @brief Places the falling tetromino into the playfield.
/// @param board Board object
void betris_lockTetromino(betris_board_t* board);

// --- Function Definitions --- //

// Rotates the falling tetromino clockwise
betris_error_t betris_rotcw(betris_game_t* game) 
{
    betris_board_t* board;
    betris_coord_t tmpT[4];
    int8_t rotPossible;

    // Error checking
    if (!game) {
        return BETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return BETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    betris_rand_entropy(game, 1);

    // If falling tetromino is 'O' (yellow), rotation doesn't have an effect. No need perform computations.
    if (board->fcol == BETRIS_YELLOW) {
        return BETRIS_SUCCESS;
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
    rotPossible = betris_collisionCheck(board, tmpT);    // Check if there is room for rotation

    // Apply transformation if there are no collisions
    if (rotPossible) 
    {
        board->frot = MOD4(board->frot+1);

        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }
    // Indicate that operation is not possible due to a colision
    else {
        return BETRIS_ERROR_COLLISION;
    }

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    return BETRIS_SUCCESS;
}

// Rotates the falling tetromino counter-clockwise
betris_error_t betris_rotcntrcw(betris_game_t* game) 
{
    betris_board_t* board;
    betris_coord_t tmpT[4];
    int8_t rotPossible;

    // Error checking
    if (!game) {
        return BETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return BETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    betris_rand_entropy(game, 2);
    
    // If falling tetromino is 'O' (yellow), rotation doesn't have an effect. No need perform computations.
    if (board->fcol == BETRIS_YELLOW) {
        return BETRIS_SUCCESS;
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
    rotPossible = betris_collisionCheck(board, tmpT);    // Check if there is room for rotation

    // Apply transformation if there are no collisions
    if (rotPossible) 
    {
        board->frot = MOD4(board->frot-1);    // Update rotation

        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }
    // Indicate that operation is not possible due to a colision
    else {
        return BETRIS_ERROR_COLLISION;
    }

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    return BETRIS_SUCCESS;
}

// Shifts falling tetromino to the left
betris_error_t betris_leftshift(betris_game_t* game)
{
    betris_board_t* board;
    betris_coord_t tmpT[4];
    int8_t shiftPossible;

    // Error checking
    if (!game) {
        return BETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return BETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    betris_rand_entropy(game, 3);

    // Copy tetromino
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if left shift is possible
    for (int i = 0; i < 4; i++) {   // Apply left shift
        tmpT[i].w = tmpT[i].w - 1;
    }
    shiftPossible = betris_collisionCheck(board, tmpT);    // Check if there is room for shift

    // Apply shift if there are no collisions
    if (shiftPossible) 
    {
        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }
    // Indicate that operation is not possible due to a colision
    else {
        return BETRIS_ERROR_COLLISION;
    }

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    return BETRIS_SUCCESS;
}

// Shifts falling tetromino to the right
betris_error_t betris_rightshift(betris_game_t* game)
{
    betris_board_t* board;
    betris_coord_t tmpT[4];
    int8_t shiftPossible;

    // Error checking
    if (!game) {
        return BETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return BETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    betris_rand_entropy(game, 4);

    // Copy tetromino
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if right shift is possible
    for (int i = 0; i < 4; i++) {   // Apply right shift
        tmpT[i].w = tmpT[i].w + 1;
    }
    shiftPossible = betris_collisionCheck(board, tmpT);    // Check if there is room for shift

    // Apply shift if there are no collisions
    if (shiftPossible) 
    {
        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }
    // Indicate that operation is not possible due to a colision
    else {
        return BETRIS_ERROR_COLLISION;
    }

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    return BETRIS_SUCCESS;
}

// Performs non-locking soft drop
betris_error_t betris_sdrop(betris_game_t* game)
{
    betris_board_t* board;
    betris_coord_t tmpT[4];
    int8_t dropPossible;

    // Error checking
    if (!game) {
        return BETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return BETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    betris_rand_entropy(game, 5);

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
    dropPossible = betris_collisionCheck(board, tmpT);    // Check if there is room for shift
    
    // Apply drop if there are no collisions
    if (dropPossible) 
    { 
        board->fpos[0] = tmpT[0];
        board->fpos[1] = tmpT[1];
        board->fpos[2] = tmpT[2];
        board->fpos[3] = tmpT[3];
    }
    // Indicate that operation is not possible due to a colision
    else {
        betris_lockTetromino(board);
        return BETRIS_ERROR_COLLISION;
    }

    // No need to invalidate ghost piece cache because operation results in the same ghost position

    return BETRIS_SUCCESS;
}

// Performs locking hard drop
betris_error_t betris_hdrop(betris_game_t* game)
{
    betris_board_t* board;
    betris_error_t error; 
    
    // Hard drop tetromino using genGhostCoords() to calculate the drop position 
    error = betris_calcGhostCoords(game);
    if (error) {
        return error;
    }

    // Use user input to add entropy
    betris_rand_entropy(game, 6);

    // game pointer is null checked in calcGhostCoords
    board = game->board;

    // Set falling tetromino position to ghost piece position
    board->fpos[0] = board->gc_pos[0];
    board->fpos[1] = board->gc_pos[1];
    board->fpos[2] = board->gc_pos[2];
    board->fpos[3] = board->gc_pos[3];

    // Lock tetromino
    betris_lockTetromino(board);

    return BETRIS_SUCCESS;
}

// Calculates the position of the falling tetromino if it were to be hard dropped. 
betris_error_t betris_calcGhostCoords(betris_game_t* game)
{
    betris_board_t* board;
    int8_t dropPossible;

    // Error checking
    if (!game) {
        return BETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return BETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == BETRIS_BLANK) {
        return BETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // If cache is still valid, no need to recalculate
    if (board->gc_valid) {
        return BETRIS_SUCCESS;
    }

    // Copy falling tetromino to cache
    board->gc_pos[0] = board->fpos[0];
    board->gc_pos[1] = board->fpos[1];
    board->gc_pos[2] = board->fpos[2];
    board->gc_pos[3] = board->fpos[3];

    // Drop tetromino until cant anymore
    do 
    {
        // Apply soft drop
        for (int i = 0; i < 4; i++) {   
            board->gc_pos[i].h = board->gc_pos[i].h - 1;
        }

        // Check if there is space to drop
        dropPossible = betris_collisionCheck(board, board->gc_pos);
    }
    while (dropPossible);

    // Apply corrective jump
    for (int i = 0; i < 4; i++) {
        board->gc_pos[i].h = board->gc_pos[i].h + 1;
    }

    // Ghost piece cache is now valid
    board->gc_valid = 1;

    return BETRIS_SUCCESS;
}



// Checks if a given tetromino has any collisions with the playfield
int8_t betris_collisionCheck(betris_board_t* board, betris_coord_t tetromino[4]) 
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

// Places the falling tetromino into the playfield. 
void betris_lockTetromino(betris_board_t* board)
{
    // Lock the tetromino
    for (int i = 0; i < 4; i++) {
        board->pf[board->fpos[i].h][board->fpos[i].w] = board->fcol;
    }

    // Clear tetromino color to indicate that it is no longer active
    board->fcol = BETRIS_BLANK;

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    return;
}