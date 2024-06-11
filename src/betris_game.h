#include <stdint.h>
#include "betris_board.h"

#ifndef __BETRIS_GAME__
#define __BETRIS_GAME__



// https://tetris.wiki/Tetris_Guideline

#ifndef BETRIS_PP_SIZE
    #define BETRIS_PP_SIZE 2
#elif BETRIS_PP_SIZE <= 1
    #error invalid piece preview size, too small
#elif BETRIS_PP_SIZE > 6
    #error invalid piece preview size, too large
#endif

// BETRIS_TICK_PERIOD units are in microseconds
#ifndef BETRIS_TICK_PERIOD
    #define BETRIS_TICK_PERIOD 100
#elif BETRIS_TICK_PERIOD  > 823
    #error Tick period too large
#endif

// Random INCR. Dont want to be 0 or multiples of 7 because it would produce a constant value
#ifndef BETRIS_RAND_INCR
    #define BETRIS_RAND_INCR 2
#elif BETRIS_RAND_INCR < 1
    #error Random increment is too small
#elif BETRIS_RAND_INCR > 6
    #error Random increment is too large
#endif


// --- Game Structures --- //

typedef enum betris_error {
    BETRIS_SUCCESS = 0,
    BETRIS_NULL_GAME,
    BETRIS_NULL_BOARD,
    BETRIS_INACTIVE_TETROMINO,
    BETRIS_COLLISION
} betris_error_t;

typedef struct betris_game
{
    // Game state
    int8_t isStarted;

    // Tetris board
    betris_board_t* board;

    // Score
    int8_t  level;
    int64_t score;

    // Tetromino queue
    betris_color_t  ppreview[BETRIS_PP_SIZE];   // Piece preview
    int8_t          qidx;               // Position of next color in queue
    betris_color_t  queue[7];           // Queue that feeds into ppreview
    betris_color_t  shuffle_queue[7];   // Gets swapped with queue when it empties

    // RNG state
    int randx;

} betris_game_t;


// --- Function Declarations --- //

/// @brief Adds entropy to the random number generator
/// @param game Game object
/// @param entropy Any integer
/// @return Error code
betris_error_t betris_rand_entropy(betris_game_t* game, int entropy);

/// @brief Swaps two tetrominoes in the shuffle queue using the RNG
/// @param game Game object
/// @return Error code
betris_error_t betris_rand_swap(betris_game_t* game);

/// @brief Should be called at the set tick rate. Handles 
/// @param game Game object
/// @return Error code
betris_error_t betris_tick(betris_game_t* game);


// --- CONSTANTS --- //

const int64_t BETRIS_SPEED_CURVE[20] = {
    0,
    (1              * 1000000) / BETRIS_TICK_PERIOD, // 1
    (0.793          * 1000000) / BETRIS_TICK_PERIOD, // 2
    (0.617796       * 1000000) / BETRIS_TICK_PERIOD, // 3
    (0.472729139000 * 1000000) / BETRIS_TICK_PERIOD, // 4
    (0.355196928256 * 1000000) / BETRIS_TICK_PERIOD, // 5
    (0.262003549978 * 1000000) / BETRIS_TICK_PERIOD, // 6
    (0.189677245333 * 1000000) / BETRIS_TICK_PERIOD, // 7
    (0.134734730816 * 1000000) / BETRIS_TICK_PERIOD, // 8
    (0.093882248904 * 1000000) / BETRIS_TICK_PERIOD, // 9
    (0.064151584960 * 1000000) / BETRIS_TICK_PERIOD, // 10
    (0.042976258297 * 1000000) / BETRIS_TICK_PERIOD, // 11
    (0.028217677801 * 1000000) / BETRIS_TICK_PERIOD, // 12
    (0.018153328544 * 1000000) / BETRIS_TICK_PERIOD, // 13
    (0.011439342347 * 1000000) / BETRIS_TICK_PERIOD, // 14
    (0.007058616221 * 1000000) / BETRIS_TICK_PERIOD, // 15
    (0.004263556954 * 1000000) / BETRIS_TICK_PERIOD, // 16
    (0.002520083970 * 1000000) / BETRIS_TICK_PERIOD, // 17
    (0.001457138733 * 1000000) / BETRIS_TICK_PERIOD, // 18
    (0.000823906896 * 1000000) / BETRIS_TICK_PERIOD, // 19
};

#endif