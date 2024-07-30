#include "btetris_control.h"
#include "btetris_board.h"


// https://tetris.fandom.com/wiki/SR

// Used to keep rotation in the range of [0,3]
#define MOD4(val) ((val) & 0b0011)

// --- Function Declarations --- //

/// @brief Checks if a given tetromino has any collisions with the playfield
/// @param board Board object 
/// @param tetromino Tetromino to check collisions with board playfield
/// @return returns 1 if a collision was NOT found, returns 0 if a collision was found. 
int8_t tetris_collisionCheck(tetris_board_t* board, tetris_coord_t tetromino[4]);

/// @brief Places the falling tetromino into the playfield.
/// @param board Board object
void tetris_lockTetromino(tetris_board_t* board);


// --- Function Definitions --- //

// Rotates the falling tetromino clockwise
tetris_error_t tetris_rotcw(tetris_game_t* game) 
{
    tetris_board_t* board;

    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return TETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == TETRIS_BLANK) {
        return TETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    tetris_rand_entropy(game, 1);

    // Dont worry about handling game logic if the game isn't running. 
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted)
    {
        return TETRIS_ERROR_NOT_STARTED;
    }
    if (!game->isRunning) {
        return TETRIS_ERROR_GAME_PAUSED;
    }

    // If falling tetromino is 'O' (yellow), rotation doesn't have an effect. No need perform computations.
    if (board->fcol == TETRIS_YELLOW) {
        return TETRIS_SUCCESS;
    }

    // Copy tetromino
    tetris_coord_t tmpT[4];
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if rotation is possible in current position
    int8_t rotPossible;
    for (int i = 0; i < 4; i++) {   // Apply transformation
        tmpT[i] = tetris_addCoord(tmpT[i], TETRIS_TETROMINO_ROTATE[board->fcol][board->frot][i]);
    }

    // Check if there is room for rotation
    rotPossible = tetris_collisionCheck(board, tmpT);

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
        return TETRIS_ERROR_COLLISION;
    }

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    return TETRIS_SUCCESS;
}

// Rotates the falling tetromino counter-clockwise
tetris_error_t tetris_rotcntrcw(tetris_game_t* game) 
{
    tetris_board_t* board;
    
    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return TETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == TETRIS_BLANK) {
        return TETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    tetris_rand_entropy(game, 2);

    // Dont worry about handling game logic if the game isn't running. 
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted)
    {
        return TETRIS_ERROR_NOT_STARTED;
    }
    if (!game->isRunning) {
        return TETRIS_ERROR_GAME_PAUSED;
    }
    
    // If falling tetromino is 'O' (yellow), rotation doesn't have an effect. No need perform computations.
    if (board->fcol == TETRIS_YELLOW) {
        return TETRIS_SUCCESS;
    }

    // Copy tetromino
    tetris_coord_t tmpT[4];
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if rotation is possible in current position
    int8_t rotPossible;
    for (int i = 0; i < 4; i++) {   // Apply transformation
        tmpT[i] = tetris_subCoord(tmpT[i], TETRIS_TETROMINO_ROTATE[board->fcol][MOD4(board->frot-1)][i]);
    }

    // Check if there is room for rotation
    rotPossible = tetris_collisionCheck(board, tmpT);

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
        return TETRIS_ERROR_COLLISION;
    }

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    return TETRIS_SUCCESS;
}

// Shifts falling tetromino to the left
tetris_error_t tetris_leftshift(tetris_game_t* game)
{
    tetris_board_t* board;
    
    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return TETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == TETRIS_BLANK) {
        return TETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    tetris_rand_entropy(game, 3);

    // Dont worry about handling game logic if the game isn't running. 
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted)
    {
        return TETRIS_ERROR_NOT_STARTED;
    }
    if (!game->isRunning) {
        return TETRIS_ERROR_GAME_PAUSED;
    }

    // Copy tetromino
    tetris_coord_t tmpT[4];
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if left shift is possible
    int8_t shiftPossible;
    for (int i = 0; i < 4; i++) {   // Apply left shift
        tmpT[i].w = tmpT[i].w - 1;
    }

    // Check if there is room for shift
    shiftPossible = tetris_collisionCheck(board, tmpT);    

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
        return TETRIS_ERROR_COLLISION;
    }

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    return TETRIS_SUCCESS;
}

// Shifts falling tetromino to the right
tetris_error_t tetris_rightshift(tetris_game_t* game)
{
    tetris_board_t* board;

    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return TETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == TETRIS_BLANK) {
        return TETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    tetris_rand_entropy(game, 4);

    // Dont worry about handling game logic if the game isn't running. 
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted)
    {
        return TETRIS_ERROR_NOT_STARTED;
    }
    if (!game->isRunning) {
        return TETRIS_ERROR_GAME_PAUSED;
    }

    // Copy tetromino
    tetris_coord_t tmpT[4];
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];

    // Check if right shift is possible
    int8_t shiftPossible;
    for (int i = 0; i < 4; i++) {   // Apply right shift
        tmpT[i].w = tmpT[i].w + 1;
    }

    // Check if there is room for shift
    shiftPossible = tetris_collisionCheck(board, tmpT);    

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
        return TETRIS_ERROR_COLLISION;
    }

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    return TETRIS_SUCCESS;
}

// Performs non-locking soft drop
tetris_error_t tetris_sdrop(tetris_game_t* game)
{
    tetris_board_t* board;

    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return TETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == TETRIS_BLANK) {
        return TETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Use user input to add entropy
    tetris_rand_entropy(game, 5);

    // Dont worry about handling game logic if the game isn't running. 
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted)
    {
        return TETRIS_ERROR_NOT_STARTED;
    }
    if (!game->isRunning) {
        return TETRIS_ERROR_GAME_PAUSED;
    }

    // Copy tetromino
    tetris_coord_t tmpT[4];
    tmpT[0] = board->fpos[0];
    tmpT[1] = board->fpos[1];
    tmpT[2] = board->fpos[2];
    tmpT[3] = board->fpos[3];
    
    
    // Apply soft drop 
    int8_t dropPossible;
    for (int i = 0; i < 4; i++) {   
        tmpT[i].h = tmpT[i].h - 1;
    }

    // Check if soft drop is possible
    dropPossible = tetris_collisionCheck(board, tmpT);
    
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
        tetris_lockTetromino(board);
        return TETRIS_ERROR_COLLISION;
    }

    // No need to invalidate ghost piece cache because operation results in the same ghost position

    return TETRIS_SUCCESS;
}

// Performs locking hard drop
tetris_error_t tetris_hdrop(tetris_game_t* game)
{
    tetris_error_t error; 
    
    // Hard drop tetromino using genGhostCoords() to calculate the drop position 
    error = tetris_calcGhostCoords(game);
    if (error) {
        return error;
    }

    // Use user input to add entropy
    tetris_rand_entropy(game, 6);

    // Dont worry about handling game logic if the game isn't running. 
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted)
    {
        return TETRIS_ERROR_NOT_STARTED;
    }
    if (!game->isRunning) {
        return TETRIS_ERROR_GAME_PAUSED;
    }

    // game pointer is null checked in `calcGhostCoords()` function
    tetris_board_t* board;
    board = game->board;

    // Set falling tetromino position to ghost piece position
    board->fpos[0] = board->gc_pos[0];
    board->fpos[1] = board->gc_pos[1];
    board->fpos[2] = board->gc_pos[2];
    board->fpos[3] = board->gc_pos[3];

    // Lock tetromino
    tetris_lockTetromino(board);

    return TETRIS_SUCCESS;
}

// Calculates the position of the falling tetromino if it were to be hard dropped. 
tetris_error_t tetris_calcGhostCoords(tetris_game_t* game)
{
    tetris_board_t* board;

    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return TETRIS_ERROR_NULL_BOARD;
    }
    if (board->fcol == TETRIS_BLANK) {
        return TETRIS_ERROR_INACTIVE_TETROMINO;
    }

    // Dont worry about handling game logic if the game isn't running. 
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted)
    {
        return TETRIS_ERROR_NOT_STARTED;
    }

    // If cache is still valid, no need to recalculate
    if (board->gc_valid) {
        return TETRIS_SUCCESS;
    }

    // Copy falling tetromino to cache
    board->gc_pos[0] = board->fpos[0];
    board->gc_pos[1] = board->fpos[1];
    board->gc_pos[2] = board->fpos[2];
    board->gc_pos[3] = board->fpos[3];

    // Drop tetromino until cant anymore
    int8_t dropPossible;
    do 
    {
        // Apply soft drop
        for (int i = 0; i < 4; i++) {   
            board->gc_pos[i].h = board->gc_pos[i].h - 1;
        }

        // Check if there is space to drop
        dropPossible = tetris_collisionCheck(board, board->gc_pos);
    }
    while (dropPossible);

    // Apply corrective jump
    for (int i = 0; i < 4; i++) {
        board->gc_pos[i].h = board->gc_pos[i].h + 1;
    }

    // Ghost piece cache is now valid
    board->gc_valid = 1;

    return TETRIS_SUCCESS;
}


// Checks if a given tetromino has any collisions with the playfield
int8_t tetris_collisionCheck(tetris_board_t* board, tetris_coord_t tetromino[4]) 
{
    // NOTE: Checked in reverse because coord array is sorted from highest to lowest. Lower blocks are more likely to have a colision. 

    // Check collisions with locked/fallen tetrominos
    for (int i = 3; i >= 0; i--) 
    {
        if (board->pf[tetromino[i].h][tetromino[i].w] != TETRIS_BLANK) {
            return 0;
        }
    }

    // Check border bounds collisions before checking playfield array
    for (int i = 3; i >= 0; i--) 
    {
        if (tetromino[i].h < 0 || tetromino[i].h >= TETRIS_HEIGHT+TETRIS_HEIGHT_BUFF) {
            return 0;
        }

        if (tetromino[i].w < 0 || tetromino[i].w >= TETRIS_WIDTH) {
            return 0;
        }
    }

    // All checks passed, return 1 to declare no collisions
    return 1;
}

// Places the falling tetromino into the playfield. 
void tetris_lockTetromino(tetris_board_t* board)
{
    // Lock the tetromino
    for (int i = 0; i < 4; i++) {
        board->pf[board->fpos[i].h][board->fpos[i].w] = board->fcol;
    }

    // Clear tetromino color to indicate that it is no longer active
    board->fcol = TETRIS_BLANK;

    // Invalidate ghost piece cache
    board->gc_valid = 0;

    // Update playfield height
    if (board->pf_height < board->fpos[0].h) {
        board->pf_height = board->fpos[0].h;
    }

    return;
}

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

    // Yellow (O)
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},

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