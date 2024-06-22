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
    game->tmicro += 1;

    // If current falling tetromino is "blank" then a tetromino was recently locked. 
    // Need to check if a row needs to be cleared and generate a new tetromino to fall
    if (board->fcol == TETRIS_BLANK) 
    {
        // --- Locate rows to clear --- //

        // NOTE: Tetromino starting positions and rotation table are designed so board->fpos is always sorted by height

        // Keep track of prev and current row
        int row_prev, row_curr;
        row_prev = -1;

        // List of rows to clear
        int row_cidx = 0;
        int row_clist[4] = {-1, -1, -1, -1};

        // Loop through tetromino coords
        for (int i = 0; i < 4; i++) 
        {
            row_curr = board->fpos[i].h;

            // If coord height is equal to a previous height, row was already checked
            if (row_prev != row_curr)
            {
                int w = 0;
                int rowClearable = 1;

                // Check if row is full
                while (w < TETRIS_WIDTH && rowClearable) 
                {
                    // Say row is not clearable if empty position is found
                    if (board->pf[row_curr][w] == TETRIS_BLANK) {
                        rowClearable = 0;
                    }
                    // Otherwise, check next position
                    else {
                        w++;
                    }
                }

                // If row is clearable, save the row index to clear it later
                if (rowClearable) 
                {
                    row_clist[row_cidx] = row_curr;
                    row_cidx++;
                }

                // Update row_prev
                row_prev = board->fpos[i].h;
            }
        }

        int row_ccnt = row_cidx;    // Number of rows to clear
        row_cidx = 0;               // Reset cidx

        // Update line clear score
        switch (row_ccnt)
        {
        case 1:
            game->score += 100 * game->level;
            break;
        case 2: 
            game->score += 300 * game->level;
            break;
        case 3: 
            game->score += 500 * game->level;
            break;
        case 4:
            game->score += 800 * game->level;
            break;
        default:
            break;
        }

        // Update combo scoring
        if (row_ccnt == 0)
        {
            game->combo = -1;
        }
        else if (game->combo >= 0)
        {
            game->score += 50 * game->combo * game->level;
            game->combo += 1;
        }

        game->lines += row_ccnt;
        if (game->lines >= 10) 
        {
            game->level += 1;
            game->lines -= 10;
        }


        // --- Clear rows --- //

        while (row_cidx < row_ccnt)
        {
            // Count number of adjacent rows
            int adj_cnt;
            for (adj_cnt = 1; adj_cnt < row_ccnt-row_cidx; adj_cnt++)
            {
                // If the next row to clear isn't adjacent, break out of loop
                if (row_clist[row_cidx] != row_clist[row_cidx + adj_cnt] - adj_cnt)
                {
                    break;
                }
            } 

            // Move rows above cidx down
            for (int h = row_clist[row_cidx] + 1; h <= board->pf_height; h++)
            {
                // Loop through current row
                for (int w = 0; w < TETRIS_WIDTH; w++) 
                {
                    // Move selected row down by the count of adjacent rows 
                    board->pf[h - adj_cnt][w] = board->pf[h][w];
                }
            }

            // Clear top rows that werent overwritten by the loop above
            for (int h = board->pf_height; h > board->pf_height - adj_cnt; h--) 
            {
                // Loop through current row to clear it
                for (int w = 0; w < TETRIS_WIDTH; w++) 
                {
                    board->pf[h][w] = TETRIS_BLANK;
                }
            }

            // Update cidx
            row_cidx += adj_cnt;

            // Update pf height
            board->pf_height -= adj_cnt;
        }


        // --- Pop Tetromino --- //

        // Get the color of the next falling tetromino
        board->fcol = game->ppreview[0];

        // If queue is empty, need to swap with shuffle queue
        if (game->qidx > 7) 
        {
            // Swap queues
            tetris_color_t tempc;
            for (int i = 0; i < 7; i++)
            {
                tempc = game->queue[i];
                game->queue[i] = game->shuffle_queue[i];
                game->shuffle_queue[i] = tempc;
            }

            // Reset queue length
            game->qidx = 0;
        }

        // Shift tetrominoes in piece preview 
        for (int i = 1; i < TETRIS_PP_SIZE; i++)
        {
            game->ppreview[i-1] = game->ppreview[i];
        }

        // Pop tetromino from queue
        game->ppreview[TETRIS_PP_SIZE-1] = game->queue[game->qidx];
        game->qidx++;

        // Copy starting position for next falling tetromino
        board->frot = 0;
        board->fpos[0] = TETRIS_TETROMINO_START[board->fcol][0];
        board->fpos[1] = TETRIS_TETROMINO_START[board->fcol][1];
        board->fpos[2] = TETRIS_TETROMINO_START[board->fcol][2];
        board->fpos[3] = TETRIS_TETROMINO_START[board->fcol][3];

        // If there is a colision with the starting position, the game is over
        for (int i = 0; i < 4; i++) 
        {
            if (board->pf[board->fpos[i].h][board->fpos[i].w] != TETRIS_BLANK) 
            {
                game->isRunning = 0;
                board->fcol = TETRIS_BLANK;

                return TETRIS_SUCCESS;
            }
        }
    }

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
    
    // Add user generated entropy
    game->randx += entropy;

    // Get entropy from game runtime
    game->randx += game->tmicro;

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