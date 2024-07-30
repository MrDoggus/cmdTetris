#include <stdint.h>
#include "console.h"
#include "btetris_game.h"

#ifndef __DTETRIS__
#define __DTETRIS__

#define TFRONT_PLAYFIELD_XOFFSET 23
#define TFRONT_TITLE_YOFFSET 6
#define TFRONT_PPREVIEW_YOFFSET 11
#define TFRONT_PPREVIEW_XOFFSET 11

typedef enum tfront_regions {
    TFRONT_REG_PLAYFIELD =  0x01,
    TFRONT_REG_PPREVIEW =   0x02,
    TFRONT_REG_SCORE =      0x08,
    TFRONT_REG_TITLE =      0x10,

    TFRONT_REG_DEBUGR =     0x100,
} tfront_regions_t;

typedef union tfront_coord {
    int isRendered;
    struct {
        short x;
        short y;
    } coord;
} tfront_coord_t;

typedef struct tfront {
    tfront_coord_t title_offset;
    tfront_coord_t pfield_offset;
    tfront_coord_t pprev_offset;
    tfront_coord_t score_offset;
    tfront_coord_t debug_offset;
} tfront_t;

console_error_t tfront_draw_playfield(console_info_t* cinfo, tetris_board_t* board);

console_error_t tfront_draw_boxes(console_info_t* cinfo);

console_error_t tfront_draw_title(console_info_t* cinfo);

console_error_t tfront_draw_bplayfield(console_info_t* cinfo);

console_error_t tfront_draw_bppreview(console_info_t* cinfo);

console_error_t tfront_draw_bscore(console_info_t* cinfo);

console_error_t tfront_window_event(console_info_t* cinfo, PWINDOW_BUFFER_SIZE_RECORD record);

#endif