#include "betris_game.h"

void betris_init(betris_gamestate_t* gamestate)
{
    // Return null board 
    if (!board) {

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