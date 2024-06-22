#include <stdint.h>
#include "btetris_board.h"

#ifndef __TETRIS_GAME__
#define __TETRIS_GAME__



// https://tetris.wiki/Tetris_Guideline

#ifndef TETRIS_PP_SIZE
    #define TETRIS_PP_SIZE 2
#elif TETRIS_PP_SIZE <= 1
    #error invalid piece preview size, too small
#elif TETRIS_PP_SIZE > 6
    #error invalid piece preview size, too large
#endif

// TETRIS_TICK_PERIOD units are in microseconds
#ifndef TETRIS_TICK_PERIOD
    #define TETRIS_TICK_PERIOD 100
#elif TETRIS_TICK_PERIOD  > 823
    #error Tick period too large
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
    TETRIS_ERROR_COLLISION
} tetris_error_t;

typedef struct tetris_game
{
    // Game state
    int8_t isRunning;

    // Tetris board
    tetris_board_t* board;

    // Score
    int8_t  level;
    int64_t score;
    int8_t  combo;
    int8_t  lines;

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

/// @brief Resets a game to a playable state without reseting random state
/// @param game Game object
/// @return Error code
tetris_error_t tetris_reset(tetris_game_t* game);

/// @brief Initializes a tetris game struct
/// @param game Pointer to allocated game struct
/// @param board Pointer to allocated board struct
/// @param randx_init Starting number for RNG
/// @return Error code
tetris_error_t tetris_init(tetris_game_t* game, tetris_board_t* board, int32_t randx_init);

/// @brief Starts the tetris game
/// @param game Game object
/// @return Error code
tetris_error_t tetris_start(tetris_game_t* game);

/// @brief Should be called at the set tick rate. Handles 
/// @param game Game object
/// @return Error code
tetris_error_t tetris_tick(tetris_game_t* game);

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

const int64_t TETRIS_SPEED_CURVE[20] = {
    (1.23915737299  * 1000000) / TETRIS_TICK_PERIOD, // 1
    (1              * 1000000) / TETRIS_TICK_PERIOD, // 1
    (0.793          * 1000000) / TETRIS_TICK_PERIOD, // 2
    (0.617796       * 1000000) / TETRIS_TICK_PERIOD, // 3
    (0.472729139000 * 1000000) / TETRIS_TICK_PERIOD, // 4
    (0.355196928256 * 1000000) / TETRIS_TICK_PERIOD, // 5
    (0.262003549978 * 1000000) / TETRIS_TICK_PERIOD, // 6
    (0.189677245333 * 1000000) / TETRIS_TICK_PERIOD, // 7
    (0.134734730816 * 1000000) / TETRIS_TICK_PERIOD, // 8
    (0.093882248904 * 1000000) / TETRIS_TICK_PERIOD, // 9
    (0.064151584960 * 1000000) / TETRIS_TICK_PERIOD, // 10
    (0.042976258297 * 1000000) / TETRIS_TICK_PERIOD, // 11
    (0.028217677801 * 1000000) / TETRIS_TICK_PERIOD, // 12
    (0.018153328544 * 1000000) / TETRIS_TICK_PERIOD, // 13
    (0.011439342347 * 1000000) / TETRIS_TICK_PERIOD, // 14
    (0.007058616221 * 1000000) / TETRIS_TICK_PERIOD, // 15
    (0.004263556954 * 1000000) / TETRIS_TICK_PERIOD, // 16
    (0.002520083970 * 1000000) / TETRIS_TICK_PERIOD, // 17
    (0.001457138733 * 1000000) / TETRIS_TICK_PERIOD, // 18
    (0.000823906896 * 1000000) / TETRIS_TICK_PERIOD, // 19
};

#endif