#include <stdint.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

typedef struct cmdtetris_game_struct
{
    // Game state
    uint8_t initialized : 1;
    uint8_t start       : 1;
    uint8_t paused      : 1;

    // Stats
    uint32_t score;
    uint8_t level;
    uint32_t highscore;

    // Board
    cmdtetris_square_t* board;

    // Falling tetromino data
    uint16_t falling_idx[4];                // Stores position of the squares of the falling tetromino
    cmdtetris_square_t falling_color;       // Stores color of falling tetromino

} cmdtetris_board_t;

typedef enum 
{
    BLANK = 0,
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    PURPLE
} cmdtetris_square_t