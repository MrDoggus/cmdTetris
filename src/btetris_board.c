#include "btetris_board.h"

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
    {   // Yellow (O)
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+2, (TETRIS_WIDTH/2)}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)-1}, 
        {TETRIS_HEIGHT+1, (TETRIS_WIDTH/2)},
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

