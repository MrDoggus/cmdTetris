#include "betris_game.h"

betris_error_t betris_init(betris_gamestate_t* gamestate)
{
    // Return null board error
    if (!gamestate) {
        return BETRIS_NULL_GAMESTATE;
    }
    // --- Init structure --- //

    gamestate->score = 0;
    gamestate->level = 0;
    gamestate->highscore = 0;

    gamestate->start = 0;
    gamestate->paused = 0;

    // Initialize playfield arrays
    for (int h = 0; h < BETRIS_WIDTH + BETRIS_HEIGHT_BUFF; h++)
    {
        for (int w = 0; w < BETRIS_HEIGHT; w++)
        {
            gamestate->playfield.arr[h][w] = BETRIS_BLANK;
            gamestate->playfield_display.arr[h][w] = BETRIS_BLANK;
        }
    }

    gamestate->falling_tetromino = BETRIS_TETROMINO_START[0];

    // Set init value last 
    gamestate->initialized = BETRIS_INIT;
}