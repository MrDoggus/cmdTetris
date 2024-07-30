#include <stdint.h>
#include "console.h"
#include "btetris_game.h"

#ifndef __DTETRIS__
#define __DTETRIS__

#define TDRAW_PLAYFIELD_XOFFSET 23
#define TDRAW_PLAYFIELD_YOFFSET 23
#define TDRAW_TITLE_YOFFSET 4
#define TDRAW_PPREVIEW_YOFFSET 11
#define TDRAW_PPREVIEW_XOFFSET 11
#define TDRAW_DEBUG_XOFFSET 11

typedef enum tfront_regions {
    TDRAW_REG_PLAYFIELD =  0x01,
    TDRAW_REG_PPREVIEW =   0x02,
    TDRAW_REG_SCORE =      0x08,
    TDRAW_REG_TITLE =      0x10,

    TDRAW_REG_DEBUGR =     0x100,
} tdraw_regions_t;

typedef union tdraw_coord {
    int isRendered;
    struct {
        short x;
        short y;
    } coord;
} tdraw_coord_t;

typedef struct tdraw_info {
    tdraw_coord_t title_offset;
    tdraw_coord_t pfield_offset;
    tdraw_coord_t pprev_offset;
    tdraw_coord_t score_offset;
    tdraw_coord_t debug_offset;
} tdraw_info_t;

console_error_t tdraw_playfield(console_info_t* cinfo, tetris_board_t* board);

console_error_t tdraw_ppreview(console_info_t* cinfo, tetris_game_t* game);

console_error_t tdraw_boxes(console_info_t* cinfo);

console_error_t tdraw_title(console_info_t* cinfo);

console_error_t tdraw_bplayfield(console_info_t* cinfo);

console_error_t tdraw_bppreview(console_info_t* cinfo);

console_error_t tdraw_bscore(console_info_t* cinfo);

console_error_t tdraw_block(console_info_t* cinfo, tetris_color_t color);

console_error_t tdraw_window_event(console_info_t* cinfo, PWINDOW_BUFFER_SIZE_RECORD record);

#endif