#include "betris_game.h"
#include "betris_control.h"


betris_error_t betris_tick(betris_game_t* game)
{
    betris_board_t* board;

    // Error checking
    if (!game) {
        return BETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return BETRIS_ERROR_NULL_BOARD;
    }

    // Shuffle the shuffle queue 
    betris_rand_swap(game);

    // Dont worry about handling game logic if the game isn't running. 
    if (!game->isRunning) {
        return BETRIS_SUCCESS;
    }

    // Update game runtime
    game->tmicro += BETRIS_TICK_PERIOD;

    // Calculate the number of times tetromino should be dropped
    int drop_cnt;   
    if (game->level > 19) 
    {
        drop_cnt = (game->tmicro - game->tdrop) / BETRIS_SPEED_CURVE[19];
        game->tdrop += drop_cnt * BETRIS_SPEED_CURVE[19];
    }
    else 
    {
        drop_cnt = (game->tmicro - game->tdrop) / BETRIS_SPEED_CURVE[game->level];
        game->tdrop += drop_cnt * BETRIS_SPEED_CURVE[game->level];
    }
    
    // Drop the tetromino
    betris_error_t retval;
    for (int i = 0; i < drop_cnt; i++) 
    {
        // Try to perform soft drop
        retval = betris_sdrop(game);

        // If there is a colision, the tetromino is now locked, can't drop it anymore 
        if (retval == BETRIS_ERROR_COLLISION) {
            break;
        }
    }

}

betris_error_t betris_rand_entropy(betris_game_t* game, int entropy)
{
    // Error checking
    if (!game) {
        return BETRIS_ERROR_NULL_GAME;
    }
    
    // Feels too simple but felt that it should be a function
    game->randx += entropy;

    return BETRIS_SUCCESS;
}

betris_error_t betris_rand_swap(betris_game_t* game)
{
    betris_color_t temp;

    // Error checking
    if (!game) {
        return BETRIS_ERROR_NULL_GAME;
    }

    // Find swap indexes
    int idx1 = game->randx % 7; // Get random position from randx
    int idx2 = game->qidx;      // Other swap position from queue idx, guarantees every piece gets swapped around

    // Swap 
    temp = game->shuffle_queue[idx1];
    game->shuffle_queue[idx1] = game->shuffle_queue[idx2];
    game->shuffle_queue[idx2] = temp;

    // Update randx
    game->randx = game->randx + BETRIS_RAND_INCR;

    return BETRIS_SUCCESS;
}