#include <stdint.h>

#ifndef __TETRIS_BOARD__
#define __TETRIS_BOARD__

// Width of tetris board
#ifndef  TETRIS_WIDTH
    #define TETRIS_WIDTH 10         
#elif TETRIS_WIDTH <= 4
    #error invalid width, too small
#elif TETRIS_WIDTH > 127
    #error invalid width, too large
#endif

// Height of tetris board
#ifndef TETRIS_HEIGHT
    #define TETRIS_HEIGHT 20        
#elif TETRIS_HEIGHT <= 4
    #error invalid height, too small
#elif TETRIS_HEIGHT > 127-4
    #error invalid height, too large
#endif 

#define TETRIS_HEIGHT_BUFF 4    // Additional row buffer at top of the board


// --- Game Structures --- //

typedef enum tetris_color {
    TETRIS_BLANK    = 0,
    TETRIS_CYAN     = 1,
    TETRIS_BLUE     = 2,
    TETRIS_ORANGE   = 3,
    TETRIS_YELLOW   = 4,
    TETRIS_GREEN    = 5,
    TETRIS_PURPLE   = 6,
    TETRIS_RED      = 7
} tetris_color_t;

typedef struct tetris_coord {
    int8_t h;  // Height
    int8_t w;  // Width
} tetris_coord_t;

typedef struct tetris_board
{
    // playfield, contains only locked tetrominos
    tetris_color_t pf[TETRIS_HEIGHT+TETRIS_HEIGHT_BUFF][TETRIS_WIDTH];
    int8_t pf_height;   // Index of heighest row in playfield

    // Falling tetromino info
    tetris_coord_t  fpos[4];    // Position of tetromino's squares
    int8_t          frot;       // Current rotation of tetromino
    tetris_color_t  fcol;       // Tetromino color

    // Ghost piece cache
    int8_t          gc_valid;   // True when ghost piece cache is valid
    tetris_coord_t  gc_pos[4];  // Ghost piece position data
 
} tetris_board_t;

// --- Function Declarations --- //

/// @brief Adds two coordinate structures
/// @param left operand 1
/// @param right operand 2
/// @return result coordinate
inline tetris_coord_t tetris_addCoord(tetris_coord_t left, tetris_coord_t right);

/// @brief Subtracts two coordinate structures
/// @param left operand 1
/// @param right operand 2
/// @return result coordinate
inline tetris_coord_t tetris_subCoord(tetris_coord_t left, tetris_coord_t right);


// --- CONSTANTS --- //

// Array of starting positions for the 7 tetrominos
const tetris_coord_t TETRIS_TETROMINO_START[8][4] = {
    {   // Blank
        {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1},
    },
    {   // Cyan (I)
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-2},
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)+1},
    },
    {   // Blue (J)
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)-2}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-2}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)},
    },
    {   // Orange (L)
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)},
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-2}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)}, 
    },
    {   // Yellow (O)
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)},
    },
    {   // Green (S)
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)},
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-2}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-1}, 
    },
    {   // Purple (T)
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-2}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)},
    },
    {   // Red (Z)
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)-2}, 
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)},
    }
};

// Adds two coordinate structures
inline tetris_coord_t tetris_addCoord(tetris_coord_t left, tetris_coord_t right) 
{
    tetris_coord_t retval;

    retval.h = left.h + right.h;
    retval.w = left.w + right.w;

    return retval;
}

// Subtracts two coordinate structures
inline tetris_coord_t tetris_subCoord(tetris_coord_t left, tetris_coord_t right) 
{
    tetris_coord_t retval;

    retval.h = left.h - right.h;
    retval.w = left.w - right.w;

    return retval;
}

#endif