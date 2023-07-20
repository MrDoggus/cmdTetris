#include <stdlib.h>
#include "cmdTetris_game.h"

cmdtetris_game_struct_t* cmdTetris_init()
{
    cmdtetris_game_struct_t* retval;      // Value to be returned

    retval = malloc(sizeof(cmdtetris_game_struct_t));     // Allocate memory for game instance

    if (!retval)
    {
        return NULL;
    }

    // --- Init structure --- //

    retval->score = 0;
    retval->level = 0;
    retval->highscore = 0;

    retval->start = 0;
    retval->paused = 0;
    retval->initialized = TETRIS_INIT;

    retval->falling_tetromino = TETROMINO_BLANK;

    for (int h = 0; h < TETRIS_WIDTH; h++)
    {
        for (int w = 0; w < TETRIS_HEIGHT; w++)
        {
            retval->board[h][w] = TETRIS_BLANK;
        }
    }
}