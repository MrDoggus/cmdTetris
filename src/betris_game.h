#include <stdint.h>

#ifndef __BETRIS_GAME__
#define __BETRIS_GAME__

#include "betris_board.h"

// https://tetris.wiki/Tetris_Guideline

#ifndef  BETRIS_PP_SIZE
    #define BETRIS_PP_SIZE 2
#elif BETRIS_PP_SIZE <= 1
    #error invalid piece preview size, too small
#elif BETRIS_PP_SIZE > 6
    #error invalid piece preview size, too large
#endif

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

} betris_game_t;

#endif