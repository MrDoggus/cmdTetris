#include "tdraw.h"

// --- DEFINITIONS --- //

#define TDRAW_PLAYFIELD_XOFFSET 23
#define TDRAW_PLAYFIELD_YOFFSET 23
#define TDRAW_TITLE_YOFFSET 6
#define TDRAW_PPREVIEW_XOFFSET 11
#define TDRAW_PPREVIEW_YOFFSET 6
#define TDRAW_SCORE_YOFFSET 6
#define TDRAW_DEBUG_XOFFSET 11

typedef union tdraw_coord {
    uint32_t isRendered;
    struct {
        uint16_t x;
        uint16_t y;
    } offset;
} tdraw_coord_t;

typedef struct tdraw_winoff {
    tdraw_coord_t title;
    tdraw_coord_t pfield;
    tdraw_coord_t pprev;
    tdraw_coord_t score;
    tdraw_coord_t debug;
} tdraw_winoff_t;

/// @brief 
/// @return 
tdraw_winoff_t tdraw_calc_offsets();

// --- GLOBAL VARS --- //

tdraw_winoff_t win_offsets;

WINDOW* wintitle;
WINDOW* winpfield;
WINDOW* winpprev;
WINDOW* winscore;
WINDOW* windebug;

tdraw_winoff_t tdraw_calc_offsets()
{
    int c_height, c_width;
    tdraw_winoff_t new_winoff;

    // Get terminal size
    getmaxyx(stdscr, c_height, c_width);

    // Calculate offsets
    if (c_height >= TDRAW_TITLE_YOFFSET + TDRAW_PLAYFIELD_YOFFSET) 
    {
        if (c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET) 
        {
            new_winoff.title.offset.x = 1;
            new_winoff.title.offset.y = 1;

            new_winoff.pfield.offset.x = 1;
            new_winoff.pfield.offset.y = 1 + TDRAW_TITLE_YOFFSET;

            new_winoff.pprev.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.pprev.offset.y = 1 + TDRAW_TITLE_YOFFSET;
            
            new_winoff.score.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.score.offset.y = 1 + TDRAW_TITLE_YOFFSET + TDRAW_PPREVIEW_YOFFSET;

            new_winoff.debug.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.debug.offset.y = 1 + TDRAW_TITLE_YOFFSET + TDRAW_PPREVIEW_YOFFSET + TDRAW_SCORE_YOFFSET;
        }
        else if (c_width >= TDRAW_PLAYFIELD_XOFFSET) 
        {
            new_winoff.title.offset.x = 1;
            new_winoff.title.offset.y = 1;

            new_winoff.pfield.offset.x = 1;
            new_winoff.pfield.offset.y = 1 + TDRAW_TITLE_YOFFSET;

            new_winoff.pprev.isRendered = 0;
            new_winoff.score.isRendered = 0;
            new_winoff.title.isRendered = 0;
        }
    }
    else if (c_height >= TDRAW_PLAYFIELD_YOFFSET) 
    {
        if (c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET + TDRAW_DEBUG_XOFFSET) 
        {
            new_winoff.pfield.offset.x = 1;
            new_winoff.pfield.offset.y = 1;

            new_winoff.pprev.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.pprev.offset.y = 1;

            new_winoff.score.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.score.offset.y = 1 + TDRAW_PPREVIEW_YOFFSET;

            new_winoff.debug.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET;
            new_winoff.debug.offset.y = 1 + TDRAW_PPREVIEW_YOFFSET;

            new_winoff.title.isRendered = 0;
        }
        else if (c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET) 
        {            
            new_winoff.pfield.offset.x = 1;
            new_winoff.pfield.offset.y = 1;

            new_winoff.pprev.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.pprev.offset.y = 1;

            new_winoff.score.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.score.offset.y = 1 + TDRAW_PPREVIEW_YOFFSET;

            new_winoff.title.isRendered = 0;
            new_winoff.debug.isRendered = 0;
        }
        else if (c_width >= TDRAW_PLAYFIELD_XOFFSET) 
        {
            new_winoff.pfield.offset.x = 1;
            new_winoff.pfield.offset.y = 1;

            new_winoff.title.isRendered = 0;
            new_winoff.pprev.isRendered = 0;
            new_winoff.score.isRendered = 0;
            new_winoff.debug.isRendered = 0;
        }
        else 
        {
            new_winoff.pfield.isRendered = 0;
            new_winoff.title.isRendered = 0;
            new_winoff.pprev.isRendered = 0;
            new_winoff.score.isRendered = 0;
            new_winoff.debug.isRendered = 0;
        }
    }
    else 
    {
        new_winoff.pfield.isRendered = 0;
        new_winoff.title.isRendered = 0;
        new_winoff.pprev.isRendered = 0;
        new_winoff.score.isRendered = 0;
        new_winoff.debug.isRendered = 0;
    }

    return new_winoff;
}

int tdraw_wininit()
{
    win_offsets = tdraw_calc_offsets();

    if (win_offsets.title.isRendered)
    {
        wintitle = newwin(6, 42, win_offsets.title.offset.y, win_offsets.title.offset.x);
        box(wintitle, 0, 0);
        wrefresh(wintitle);
    }

    if (win_offsets.pfield.isRendered)
    {
        winpfield = newwin(22, 22, win_offsets.pfield.offset.y, win_offsets.pfield.offset.x);
        box(winpfield, 0, 0);
        wrefresh(winpfield);
    }

    if (win_offsets.pprev.isRendered)
    {
        winpprev = newwin(6, 19, win_offsets.pprev.offset.y, win_offsets.pprev.offset.x);
        box(winpprev, 0, 0);
        wrefresh(winpprev);
    }

    if (win_offsets.score.isRendered)
    {
        winscore = newwin(6, 19, win_offsets.score.offset.y, win_offsets.score.offset.x);
        box(winscore, 0, 0);
        wrefresh(winscore);
    }

    if (win_offsets.debug.isRendered)
    {
        windebug = newwin(10, 19, win_offsets.debug.offset.y, win_offsets.debug.offset.x);
        box(windebug, 0, 0);
        wrefresh(windebug);
    }

    return 0;
}