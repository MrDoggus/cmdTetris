#include "betris_gamestate.h"

// Initializes gamestate structure
betris_error_t betris_init(betris_gamestate_t* gamestate)
{
    // Return null board error
    if (!gamestate) {
        return BETRIS_NULL_GAMESTATE;
    }
    
    // --- Init structure --- //

    gamestate->start = 0;
    gamestate->paused = 0;

    gamestate->score = 0;
    gamestate->level = 0;
    gamestate->highscore = 0;

    gamestate->falling_tetromino = BETRIS_TETROMINO_START[0];

    // Initialize playfield arrays
    for (int w = 0; w < BETRIS_HEIGHT+BETRIS_HEIGHT_BUFF; w++)
    {
        for (int h = 0; h < BETRIS_WIDTH; h++)
        {
            gamestate->playfield.arr[h][w] = BETRIS_BLANK;
            gamestate->playfield_display.arr[h][w] = BETRIS_BLANK;
        }
    }

    // Set init value last for interrupts
    gamestate->initialized = BETRIS_INIT;
}