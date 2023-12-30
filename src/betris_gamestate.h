#include <stdint.h>

#ifndef __BETRIS_GAMESTATE__
#define __BETRIS_GAMESTATE__

// https://tetris.wiki/Tetris_Guideline

// TODO: Check width and height of board at compile time to ensure reasonable dimensions are used

// --- CONSTANTS --- //

#define BETRIS_WIDTH 10         // Width of tetris board
#define BETRIS_HEIGHT 20        // Height of tetris board
#define BETRIS_HEIGHT_BUFF 4    // Additional row buffer at top of the board

#define BETRIS_INIT 25  // Value that the 'initialized' field of 'betris_game_struct' will have when it is initialized

// Array of starting positions for the 7 tetrominos
const betris_tetromino_t BETRIS_TETROMINO_START[8] = {
    {   // Blank
        {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},       // Position array
        0,                                              // Rotation
        BETRIS_BLANK                                    // Color 
    },
    {   // Red (Z)
        {{BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}},     // Position array
        0,                                                                                                                                                              // Rotation
        BETRIS_RED                                                                                                                                                      // Color
    },
    {   // Orange (L)
        {{BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)}},       // Position array
        0,                                                                                                                                                              // Rotation
        BETRIS_ORANGE                                                                                                                                                   // Color
    },
    {   // Yellow (O)
        {{BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}},       // Position array
        0,                                                                                                                                                              // Rotation
        BETRIS_YELLOW                                                                                                                                                   // Color
    },
    {   // Green (S)
        {{BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)}},     // Position array
        0,                                                                                                                                                              // Rotation
        BETRIS_GREEN                                                                                                                                                    // Color
    },
    {   // Cyan (I)
        {{BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)+1}},     // Position array
        0,                                                                                                                                                              // Rotation
        BETRIS_CYAN                                                                                                                                                     // Color
    },
    {   // Blue (J)
        {{BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}},     // Position array
        0,                                                                                                                                                              // Rotation
        BETRIS_BLUE                                                                                                                                                     // Color
    },
    {   // Purple (T)
        {{BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}},     // Position array
        0,                                                                                                                                                              // Rotation
        BETRIS_PURPLE                                                                                                                                                   // Color
    }
};

// --- Data Structures --- //

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
    betris_playfield_t playfield;           // Contains only settled tetrominos
    betris_playfield_t playfield_display;   // Contains playfield merged with falling tetromino

    // Falling tetromino info
    betris_tetromino_t falling_tetromino;     // Stores position of the squares of the falling tetromino
} betris_gamestate_t;

typedef struct betris_playfield {
    betris_square_t arr[BETRIS_HEIGHT+BETRIS_HEIGHT_BUFF][BETRIS_WIDTH];
} betris_playfield_t;

typedef struct betris_coord {
    int8_t h;  // Height
    int8_t w;  // Width
} betris_coord_t;

typedef struct betris_tetromino{
    betris_coord_t pos[4];      // Position of the tetromino's squares
    uint8_t rot;                // Current rotation of tetromino

    betris_square_t color;
} betris_tetromino_t;

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

typedef enum betris_error {
    BETRIS_SUCCESS = 0,
    BETRIS_NULL_GAMESTATE = 1,
    BETRIS_NOT_INITIALIZED = 2
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
betris_coord_t betris_addCoord(betris_coord_t left, betris_coord_t right);

/// @brief Subtracts two coordinate structures
/// @param left operand 1
/// @param right operand 2
/// @return result coordinate
betris_coord_t betris_subCoord(betris_coord_t left, betris_coord_t right);


#endif