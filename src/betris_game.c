#include "betris_game.h"


betris_error_t betris_tick(betris_game_t* game)
{
    // Error checking
    if (!game) {
        return BETRIS_NULL_GAME;
    }

    // Shuffle the shuffle queue 
    betris_rand_swap(game);
}

betris_error_t betris_rand_entropy(betris_game_t* game, int entropy)
{
    // Error checking
    if (!game) {
        return BETRIS_NULL_GAME;
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
        return BETRIS_NULL_GAME;
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