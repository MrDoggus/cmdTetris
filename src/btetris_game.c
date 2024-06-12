#include "btetris_game.h"
#include "btetris_control.h"


tetris_error_t tetris_tick(tetris_game_t* game)
{
    tetris_board_t* board;

    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    board = game->board;
    if (!board) {
        return TETRIS_ERROR_NULL_BOARD;
    }

    // Shuffle the shuffle queue 
    tetris_rand_swap(game);

    // Dont worry about handling game logic if the game isn't running. 
    if (!game->isRunning) {
        return TETRIS_SUCCESS;
    }

    // Update game runtime
    game->tmicro += TETRIS_TICK_PERIOD;

    // Calculate the number of times tetromino should be dropped
    int drop_cnt;   
    if (game->level > 19) 
    {
        drop_cnt = (game->tmicro - game->tdrop) / TETRIS_SPEED_CURVE[19];
        game->tdrop += drop_cnt * TETRIS_SPEED_CURVE[19];
    }
    else 
    {
        drop_cnt = (game->tmicro - game->tdrop) / TETRIS_SPEED_CURVE[game->level];
        game->tdrop += drop_cnt * TETRIS_SPEED_CURVE[game->level];
    }
    
    // Drop the tetromino
    tetris_error_t retval;
    for (int i = 0; i < drop_cnt; i++) 
    {
        // Try to perform soft drop
        retval = tetris_sdrop(game);

        // If there is a colision, the tetromino is now locked, can't drop it anymore 
        if (retval == TETRIS_ERROR_COLLISION) {
            break;
        }
    }

}

tetris_error_t tetris_rand_entropy(tetris_game_t* game, int entropy)
{
    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    
    // Feels too simple but felt that it should be a function
    game->randx += entropy;

    return TETRIS_SUCCESS;
}

tetris_error_t tetris_rand_swap(tetris_game_t* game)
{
    tetris_color_t temp;

    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }

    // Find swap indexes
    int idx1 = game->randx % 7; // Get random position from randx
    int idx2 = game->qidx;      // Other swap position from queue idx, guarantees every piece gets swapped around

    // Swap 
    temp = game->shuffle_queue[idx1];
    game->shuffle_queue[idx1] = game->shuffle_queue[idx2];
    game->shuffle_queue[idx2] = temp;

    // Update randx
    game->randx = game->randx + TETRIS_RAND_INCR;

    return TETRIS_SUCCESS;
}