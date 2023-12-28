#include "betris_game.h"

void cmdTetris_init(betris_gamestate_t* board)
{
    //retval = malloc(sizeof(cmdtetris_game_struct_t));     // Allocate memory for game instance

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
    retval->initialized = BETRIS_INIT;

    retval->falling_tetromino = BETRIS_BLANK;

    for (int h = 0; h < BETRIS_WIDTH; h++)
    {
        for (int w = 0; w < BETRIS_HEIGHT; w++)
        {
            retval->board[h][w] = BETRIS_BLANK;
        }
    }
}