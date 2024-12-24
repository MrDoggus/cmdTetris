#include "btetris_game.h"
#include "btetris_control.h"

#define MOD4(val) ((val) & 0b0011)

// --- Function Declarations --- //

/// @brief Pops a tetromino from the queue
/// @param game game struct
/// @return Popped tetromino
tetris_color_t tetris_tqueue_pop(tetris_game_t* game);

/// @brief Swaps tetromino queue with shuffle queue
/// @param game game struct
void tetris_tqueue_swap(tetris_game_t* game);

// Starts the tetris game
tetris_error_t tetris_start(tetris_game_t* game)
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
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }

    // Already started
    if (game->isStarted)
    {
        return TETRIS_SUCCESS;
    }

    // Swap queues to get shuffled tetrominoes
    tetris_tqueue_swap(game);

    // Populate piece preview
    for (int i = 0; i < TETRIS_PP_SIZE; i++)
    {
        game->ppreview[i] = tetris_tqueue_pop(game);
    }

    // Set falling tetromino
    board->fcol = tetris_tqueue_pop(game);
    board->fpos[0] = TETRIS_TETROMINO_START[board->fcol][0];
    board->fpos[1] = TETRIS_TETROMINO_START[board->fcol][1];
    board->fpos[2] = TETRIS_TETROMINO_START[board->fcol][2];
    board->fpos[3] = TETRIS_TETROMINO_START[board->fcol][3];

    // Flag game as running
    game->isStarted = 1;
    game->isRunning = 1;

    return TETRIS_SUCCESS;
}

// Resets a game to a playable state without reseting random state
tetris_error_t tetris_reset(tetris_game_t* game)
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

    game->isStarted = 0;
    game->isRunning = 0;
    game->isGameover = 0;


    // --- Reset board struct --- //

    for (int h = 0; h <= board->pf_height; h++)
    {
        for (int w = 0; w < TETRIS_WIDTH; w++) 
        {
            board->pf[h][w] = TETRIS_BLANK;
        }
    }
    board->pf_height = 0;

    board->fpos[0] = (tetris_coord_t){-1, -1};
    board->fpos[1] = (tetris_coord_t){-1, -1};
    board->fpos[2] = (tetris_coord_t){-1, -1};
    board->fpos[3] = (tetris_coord_t){-1, -1};

    board->frot = 0;
    board->fcol = TETRIS_BLANK;

    board->gc_valid = 0;
    board->gc_pos[0] = (tetris_coord_t){-1, -1};
    board->gc_pos[1] = (tetris_coord_t){-1, -1};
    board->gc_pos[2] = (tetris_coord_t){-1, -1};
    board->gc_pos[3] = (tetris_coord_t){-1, -1};


    // --- Initialize game struct --- //

    game->level = 1;
    game->score = 0;
    game->combo = -1;
    game->lines = 0;

    for (int i = 0; i < TETRIS_PP_SIZE; i++) {
        game->ppreview[i] = TETRIS_BLANK;
    }
    game->qidx = 0;

    game->tmicro = 0;
    game->tdrop = 0;

    return TETRIS_SUCCESS;
}

// Initializes a tetris game struct
tetris_error_t tetris_init(tetris_game_t* game, tetris_board_t* board, int32_t randx_init)
{
    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    if (!board) {
        return TETRIS_ERROR_NULL_BOARD;
    }


    // --- Initialize board struct --- //

    for (int h = 0; h < TETRIS_HEIGHT+TETRIS_HEIGHT_BUFF; h++)
    {
        for (int w = 0; w < TETRIS_WIDTH; w++) 
        {
            board->pf[h][w] = TETRIS_BLANK;
        }
    }
    board->pf_height = 0;

    board->fpos[0] = (tetris_coord_t){-1, -1};
    board->fpos[1] = (tetris_coord_t){-1, -1};
    board->fpos[2] = (tetris_coord_t){-1, -1};
    board->fpos[3] = (tetris_coord_t){-1, -1};

    board->frot = 0;
    board->fcol = TETRIS_BLANK;

    board->gc_valid = 0;
    board->gc_pos[0] = (tetris_coord_t){-1, -1};
    board->gc_pos[1] = (tetris_coord_t){-1, -1};
    board->gc_pos[2] = (tetris_coord_t){-1, -1};
    board->gc_pos[3] = (tetris_coord_t){-1, -1};


    // --- Initialize game struct --- //

    game->isStarted = 0;
    game->isRunning = 0;
    game->isGameover = 0;

    game->board = board;

    game->level = 1;
    game->score = 0;
    game->combo = -1;
    game->lines = 0;

    for (int i = 0; i < TETRIS_PP_SIZE; i++) {
        game->ppreview[i] = TETRIS_BLANK;
    }
    game->qidx = 0;
    game->queue[0] = TETRIS_CYAN;
    game->queue[1] = TETRIS_BLUE;
    game->queue[2] = TETRIS_ORANGE;
    game->queue[3] = TETRIS_YELLOW;
    game->queue[4] = TETRIS_GREEN;
    game->queue[5] = TETRIS_PURPLE;
    game->queue[6] = TETRIS_RED;
    game->shuffle_queue[0] = TETRIS_RED;
    game->shuffle_queue[1] = TETRIS_ORANGE;
    game->shuffle_queue[2] = TETRIS_YELLOW;
    game->shuffle_queue[3] = TETRIS_GREEN;
    game->shuffle_queue[4] = TETRIS_CYAN;
    game->shuffle_queue[5] = TETRIS_BLUE;
    game->shuffle_queue[6] = TETRIS_PURPLE;

    game->tmicro = 0;
    game->tdrop = 0;

    game->randx = randx_init;

    return TETRIS_SUCCESS;
}

// Pauses the tetris game
tetris_error_t tetris_pause(tetris_game_t* game)
{
    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted)
    {
        return TETRIS_ERROR_NOT_STARTED;
    }

    game->isRunning = 0;

    return TETRIS_SUCCESS;
}

// Unpauses the tetris game
tetris_error_t tetris_unpause(tetris_game_t* game)
{
    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted)
    {
        return TETRIS_ERROR_NOT_STARTED;
    }

    game->isRunning = 1;

    return TETRIS_SUCCESS;
}

// Computes game logic, should be called at the set tick rate. 
tetris_error_t tetris_tick(tetris_game_t* game, uint64_t tmicro)
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
    if (game->isGameover)
    {
        return TETRIS_ERROR_GAME_OVER;
    }
    if (!game->isStarted || !game->isRunning) {
        return TETRIS_SUCCESS;
    }

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

        // Loop through tetromino coords, find rows to clear
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
        int isPerfectClr;           // 1 if perfect clear, 0 if not
        row_cidx = 0;               // Reset cidx
        isPerfectClr = (board->pf_height - row_ccnt) < 1;

        // Update line clear score
        switch (row_ccnt)
        {
        case 1:
            game->score += (100 + isPerfectClr * 800) * game->level;
            break;
        case 2: 
            game->score += (300 + isPerfectClr * 1200) * game->level;
            break;
        case 3: 
            game->score += (500 + isPerfectClr * 1800) * game->level;
            break;
        case 4:
            game->score += (800 + isPerfectClr * 2000) * game->level;
            break;
        default:
            break;
        }

        // Update combo count
        if (row_ccnt == 0) {
            game->combo = -1;
        }
        else {
            game->combo += 1;
        }
        
        // Update combo scoring
        if (game->combo >= 0)
        {
            game->score += 50 * game->combo * game->level;
        }

        // Update level
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

        // Get the color of the next falling tetromino from piece preview
        board->fcol = game->ppreview[0];

        // Shift tetrominoes in piece preview 
        for (int i = 1; i < TETRIS_PP_SIZE; i++)
        {
            game->ppreview[i-1] = game->ppreview[i];
        }

        // Pop tetromino from queue
        game->ppreview[TETRIS_PP_SIZE-1] = tetris_tqueue_pop(game);

        // Copy starting position for next falling tetromino
        board->frot = 0;
        board->fpos[0] = TETRIS_TETROMINO_START[board->fcol][0];
        board->fpos[1] = TETRIS_TETROMINO_START[board->fcol][1];
        board->fpos[2] = TETRIS_TETROMINO_START[board->fcol][2];
        board->fpos[3] = TETRIS_TETROMINO_START[board->fcol][3];

        // If there is a colision with the starting position, the game is over
        for (int i = 3; i >= 0; i--) 
        {
            if (board->pf[board->fpos[i].h][board->fpos[i].w] != TETRIS_BLANK) 
            {
                game->isRunning = 0;
                game->isGameover = 1;
                board->fcol = TETRIS_BLANK;

                if (board->pf_height < board->fpos[0].h) {
                    board->pf_height = board->fpos[0].h;
                }

                return TETRIS_ERROR_GAME_OVER;
            }
        }
    } // end `if (board->fcol == TETRIS_BLANK)`

    // Update game runtime
    game->tmicro += tmicro;

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

    return TETRIS_SUCCESS;
}

// Adds entropy to the random number generator
tetris_error_t tetris_rand_entropy(tetris_game_t* game, int entropy)
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
    
    // Add user generated entropy
    game->randx += entropy;

    // Get entropy from game runtime
    game->randx += game->tmicro;

    // Get entropy from tetromino position
    int rot = board->frot;
    game->randx += board->fpos[0].h ^ board->fpos[rot].w;
    game->randx += (board->fpos[1].h ^ board->fpos[MOD4(rot+1)].w) >> 8;
    game->randx += (board->fpos[2].h ^ board->fpos[MOD4(rot+2)].w) >> 16;
    game->randx += (board->fpos[3].h ^ board->fpos[MOD4(rot+3)].w) >> 24;

    return TETRIS_SUCCESS;
}

// Swaps two tetrominoes in the shuffle queue using the RNG
tetris_error_t tetris_rand_swap(tetris_game_t* game)
{
    tetris_color_t temp;
    int idx1, idx2;

    // Error checking
    if (!game) {
        return TETRIS_ERROR_NULL_GAME;
    }

    // Find swap indexes
    idx1 = game->randx % 7;             // Get random position from randx
    idx1 = (idx1<0) ? idx1+7 : idx1;    // Get mod instead of remainder
    idx2 = game->qidx;                  // Other swap position from queue idx, guarantees every piece gets swapped around
    

    // Swap 
    temp = game->shuffle_queue[idx1];
    game->shuffle_queue[idx1] = game->shuffle_queue[idx2];
    game->shuffle_queue[idx2] = temp;

    // Update randx
    game->randx = game->randx + TETRIS_RAND_INCR;

    return TETRIS_SUCCESS;
}

// Pops a tetromino from the queue
tetris_color_t tetris_tqueue_pop(tetris_game_t* game)
{
    tetris_color_t retval;

    retval = game->queue[game->qidx];
    game->qidx++;

    if (game->qidx > 6) 
    {
        tetris_tqueue_swap(game);

        // Reset queue length
        game->qidx = 0;
    }

    return retval;
}

// Swaps tetromino queue with shuffle queue
void tetris_tqueue_swap(tetris_game_t* game)
{
    // Swap queues
    tetris_color_t tempc;
    for (int i = 0; i < 7; i++)
    {
        tempc = game->queue[i];
        game->queue[i] = game->shuffle_queue[i];
        game->shuffle_queue[i] = tempc;
    }
}

// tick() calls per line drop
const int64_t TETRIS_SPEED_CURVE[20] = {
    (1.23915737299  * 1000000), // 0
    (1              * 1000000), // 1
    (0.793          * 1000000), // 2
    (0.617796       * 1000000), // 3
    (0.472729139000 * 1000000), // 4
    (0.355196928256 * 1000000), // 5
    (0.262003549978 * 1000000), // 6
    (0.189677245333 * 1000000), // 7
    (0.134734730816 * 1000000), // 8
    (0.093882248904 * 1000000), // 9
    (0.064151584960 * 1000000), // 10
    (0.042976258297 * 1000000), // 11
    (0.028217677801 * 1000000), // 12
    (0.018153328544 * 1000000), // 13
    (0.011439342347 * 1000000), // 14
    (0.007058616221 * 1000000), // 15
    (0.004263556954 * 1000000), // 16
    (0.002520083970 * 1000000), // 17
    (0.001457138733 * 1000000), // 18
    (0.000823906896 * 1000000), // 19
};