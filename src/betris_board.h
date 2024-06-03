#include <stdint.h>

#ifndef __BETRIS_BOARD__
#define __BETRIS_BOARD__

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

typedef struct betris_board
{
    // playfield, contains only locked tetrominos
    betris_square_t pf[BETRIS_HEIGHT+BETRIS_HEIGHT_BUFF][BETRIS_WIDTH];

    // Falling tetromino info
    betris_coord_t  fpos[4];    // Position of tetromino's squares
    uint8_t         frot;       // Current rotation of tetromino
    betris_square_t fcol;       // Tetromino color
} betris_board_t;

// --- Function Declarations --- //

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
const betris_coord_t BETRIS_TETROMINO_START[8][4] = {
    {   // Blank
        {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1},
    },
    {   // Cyan (I)
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2},
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)+1},
    },
    {   // Blue (J)
        {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-2}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)},
    },
    {   // Orange (L)
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)}, 
        {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)},
    },
    {   // Yellow (O)
        {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)},
    },
    {   // Green (S)
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)},
    },
    {   // Purple (T)
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-2}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)},
    },
    {   // Red (Z)
        {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-2}, 
        {BETRIS_HEIGHT+2, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)-1}, 
        {BETRIS_HEIGHT+1, (BETRIS_WIDTH/2)},
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