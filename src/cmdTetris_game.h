#include <stdint.h>

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 20

#define TETRIS_INIT 25

typedef struct cmdtetris_game_struct
{
    // Game state
    uint8_t initialized : 6;    // Struct is initialized
    uint8_t start       : 1;    // Game is started?
    uint8_t paused      : 1;    // Game is paused

    // Stats
    uint32_t score;
    uint8_t level;
    uint32_t highscore;

    // Board
    cmdtetris_square_t board[BOARD_HEIGHT][BOARD_WIDTH];

    // Falling tetromino data
    cmdtetris_tetromino_t falling_tetromino;     // Stores position of the squares of the falling tetromino
} cmdtetris_board_t;

typedef struct cmdtetris_coord
{
    int8_t h;  // Height
    int8_t w;  // Width
} cmdtetris_coord_t;

typedef struct cmdtetris_tetromino
{
    cmdtetris_coord_t pos[4];   //Position of the tetromino's squares
    uint8_t rot;                //

    cmdtetris_square_t color;
} cmdtetris_tetromino_t;

typedef enum 
{
    TETRIS_BLANK    = 0,
    TETRIS_CYAN     = 1,
    TETRIS_BLUE     = 2,
    TETRIS_ORANGE   = 3,
    TETRIS_YELLOW   = 4,
    TETRIS_GREEN    = 5,
    TETRIS_PURPLE   = 6,
    TETRIS_RED      = 7,
} cmdtetris_square_t;

// https://tetris.fandom.com/wiki/SRS

const cmdtetris_tetromino_t[] TETROMINO_START = {
    {   // Blank
        {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},       // Position array
        TETRIS_BLANK                                    // Color 
    },
    {   // Red (Z)
        {{23, 3}, {23, 4}, {22, 4}, {22, 5}},           // Position array
        TETRIS_RED
    },
    {   // Orange (L)
        {{22, 3}, {22, 4}, {22, 5}, {23, 5}},           // Position array
        TETRIS_ORANGE
    },
    {   // Yellow (O)
        {{23, 4}, {23, 5}, {22, 4}, {22, 5}},           // Position array
        TETRIS_YELLOW
    },
    {   // Green (S)
        {{22, 3}, {22, 4}, {23, 4}, {23, 5}},           // Position array
        TETRIS_GREEN
    },
    {   // Cyan (I)
        {{22, 3}, {22, 4}, {22, 5}, {22, 6}},           // Position array
        TETRIS_CYAN
    },
    {   // Blue (J)
        {{23, 3}, {22, 3}, {22, 4}, {22, 5}},           // Position array
        TETRIS_BLUE
    },
    {   // Purple (T)
        {{22, 3}, {22, 4}, {23, 4}, {22, 5}},           // Position array
        TETRIS_PURPLE
    }
};



