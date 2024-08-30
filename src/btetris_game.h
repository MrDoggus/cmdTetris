#include <stdint.h>
#include "btetris_board.h"

#ifndef __TETRIS_GAME__
#define __TETRIS_GAME__

// https://tetris.wiki/Tetris_Guideline

// Piece preview array size
#ifndef TETRIS_PP_SIZE
    #define TETRIS_PP_SIZE 2
#elif TETRIS_PP_SIZE <= 1
    #error invalid piece preview size, too small
#elif TETRIS_PP_SIZE > 6
    #error invalid piece preview size, too large
#endif

// Random INCR. Dont want to be 0 or multiples of 7 because it would produce a constant value
#ifndef TETRIS_RAND_INCR
    #define TETRIS_RAND_INCR 2
#elif TETRIS_RAND_INCR < 1
    #error Random increment is too small
#elif TETRIS_RAND_INCR > 6
    #error Random increment is too large
#endif


// --- Game Structures --- //

typedef enum tetris_error {
    TETRIS_SUCCESS = 0,
    TETRIS_ERROR_NULL_GAME,
    TETRIS_ERROR_NULL_BOARD,
    TETRIS_ERROR_INACTIVE_TETROMINO,
    TETRIS_ERROR_COLLISION,
    TETRIS_ERROR_GAME_OVER,
    TETRIS_ERROR_GAME_PAUSED,
    TETRIS_ERROR_NOT_STARTED
} tetris_error_t;

typedef struct tetris_game
{
    // Game state
    int8_t isStarted;   // Set after start(), cleared after init() or reset()
    int8_t isRunning;   // Set by start() and unpause(). Cleared by pause(), tick(), init() or reset()
    int8_t isGameover;  // Set by tick(), cleared by init() or reset()

    // Tetris board
    tetris_board_t* board;

    // Score
    int8_t  level;
    int64_t score;
    int8_t  combo;  // Number of line clears in a row - 1
    int8_t  lines;  // Lines cleared in this level

    // Tetromino queue
    tetris_color_t  ppreview[TETRIS_PP_SIZE];   // Piece preview
    int8_t          qidx;               // Position of next color in queue
    tetris_color_t  queue[7];           // Queue that feeds into ppreview
    tetris_color_t  shuffle_queue[7];   // Gets swapped with queue when it empties

    // Timing
    int64_t tmicro;
    int64_t tdrop;

    // RNG state
    int32_t randx;

} tetris_game_t;


// --- Function Declarations --- //

/// @brief Initializes a tetris game struct
/// @param game Pointer to allocated game struct
/// @param board Pointer to allocated board struct
/// @param randx_init Starting number for RNG
/// @return Error code
tetris_error_t tetris_init(tetris_game_t* game, tetris_board_t* board, int32_t randx_init);

/// @brief Resets a game to a playable state without reseting random state
/// @param game Game object
/// @return Error code
tetris_error_t tetris_reset(tetris_game_t* game);

/// @brief Starts the tetris game
/// @param game Game object
/// @return Error code
tetris_error_t tetris_start(tetris_game_t* game);

/// @brief Pauses the tetris game
/// @param game Game object
/// @return Error code
tetris_error_t tetris_pause(tetris_game_t* game);

/// @brief Unpauses the tetris game
/// @param game Game object
/// @return Error code
tetris_error_t tetris_unpause(tetris_game_t* game);

/// @brief Automatically handles game logic. 
/// @param game Game object
/// @param tmicro Time passed, in microseconds, since last tetris tick call
/// @return Error code
tetris_error_t tetris_tick(tetris_game_t* game, uint64_t tmicro);

/// @brief Adds entropy to the random number generator
/// @param game Game object
/// @param entropy Any integer
/// @return Error code
tetris_error_t tetris_rand_entropy(tetris_game_t* game, int entropy);

/// @brief Swaps two tetrominoes in the shuffle queue using the RNG
/// @param game Game object
/// @return Error code
tetris_error_t tetris_rand_swap(tetris_game_t* game);


// --- CONSTANTS --- //

// tick() calls per line drop
extern const int64_t TETRIS_SPEED_CURVE[20];

#endif