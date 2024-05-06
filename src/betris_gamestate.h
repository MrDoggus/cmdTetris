#include <stdint.h>

#ifndef __BETRIS_GAMESTATE__
#define __BETRIS_GAMESTATE__

// https://tetris.wiki/Tetris_Guideline

// Width of tetris board
#ifndef  BETRIS_WIDTH
    #define BETRIS_WIDTH 10         
#elif BETRIS_WIDTH <= 4
    #error invalid width, too small
#elif BETRIS_WIDTH > 256
    #error invalid width, too large
#endif

// Height of tetris board
#ifndef BETRIS_HEIGHT
    #define BETRIS_HEIGHT 20        
#elif BETRIS_HEIGHT <= 4
    #error invalid height, too small
#elif BETRIS_HEIGHT > 256-4
    #error invalid height, too large
#endif 

#define BETRIS_HEIGHT_BUFF 4    // Additional row buffer at top of the board

#define BETRIS_INIT 25  // Value that the 'initialized' field of 'betris_game_struct' will have when it is initialized

// --- Game Structures --- //

typedef enum betris_square {
    BETRIS_BLANK    = 0,
    BETRIS_CYAN     = 1,
    BETRIS_BLUE     = 2,
    BETRIS_ORANGE   = 3,
    BETRIS_YELLOW   = 4,
    BETRIS_GREEN    = 5,
    BETRIS_PURPLE   = 6,
    BETRIS_RED      = 7
} betris_square_t;

typedef struct betris_coord {
    int8_t h;  // Height
    int8_t w;  // Width
} betris_coord_t;

typedef struct betris_tetromino{
    betris_coord_t pos[4];      // Position of the tetromino's squares
    uint8_t rot;                // Current rotation of tetromino

    betris_square_t color;
} betris_tetromino_t;

typedef struct betris_playfield {
    betris_square_t arr[BETRIS_HEIGHT+BETRIS_HEIGHT_BUFF][BETRIS_WIDTH];
} betris_playfield_t;

typedef struct betris_gamestate
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
    betris_playfield_t playfield;           // Contains only locked tetrominos

    // Falling tetromino info
    betris_tetromino_t falling_tetromino;   // Stores position of the falling tetromino
} betris_gamestate_t;

typedef enum betris_error {
    BETRIS_SUCCESS = 0,
    BETRIS_NULL_GAMESTATE = 1,
    BETRIS_NOT_INITIALIZED = 2,
    BETRIS_INACTIVE_TETROMINO = 3
} betris_error_t;

// --- Function Declarations --- //

/// @brief Initializes allocated gamestate object at given address
/// @param gamestate Gamestate object
/// @return Error code
betris_error_t betris_init(betris_gamestate_t* gamestate);

/// @brief Adds two coordinate structures
/// @param left operand 1
/// @param right operand 2
/// @return result coordinate
inline betris_coord_t betris_addCoord(betris_coord_t left, betris_coord_t right);

/// @brief Subtracts two coordinate structures
/// @param left operand 1
/// @param right operand 2
/// @return result coordinate
inline betris_coord_t betris_subCoord(betris_coord_t left, betris_coord_t right);


// --- CONSTANTS --- //

// Array of starting positions for the 7 tetrominos
const betris_tetromino_t BETRIS_TETROMINO_START[8] = {
    {   // Blank
        .pos = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
        .rot = 0,
        .color = BETRIS_BLANK
    },
    {   // Cyan (I)
        .pos = {{BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)+1}},
        .rot = 0,
        .color = BETRIS_CYAN
    },
    {   // Blue (J)
        .pos = {{BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}},
        .rot = 0,
        .color = BETRIS_BLUE
    },
    {   // Orange (L)
        .pos = {{BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)}},
        .rot = 0,
        .color = BETRIS_ORANGE
    },
    {   // Yellow (O)
        .pos = {{BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}},
        .rot = 0,
        .color = BETRIS_YELLOW
    },
    {   // Green (S)
        .pos = {{BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)}},
        .rot = 0,
        .color = BETRIS_GREEN
    },
    {   // Purple (T)
        .pos = {{BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}},
        .rot = 0,
        .color = BETRIS_PURPLE
    },
    {   // Red (Z)
        .pos = {{BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}},
        .rot = 0,
        .color = BETRIS_RED
    }
};

// Adds two coordinate structures
inline betris_coord_t betris_addCoord(betris_coord_t left, betris_coord_t right) 
{
    betris_coord_t retval;

    retval.h = left.h + right.h;
    retval.w = left.w + right.w;

    return retval;
}

// Subtracts two coordinate structures
inline betris_coord_t betris_subCoord(betris_coord_t left, betris_coord_t right) 
{
    betris_coord_t retval;

    retval.h = left.h - right.h;
    retval.w = left.w - right.w;

    return retval;
}

#endif