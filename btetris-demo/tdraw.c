#include "tdraw.h"

// --- DEFINITIONS --- //

#define TDRAW_PLAYFIELD_XOFFSET 23
#define TDRAW_PLAYFIELD_YOFFSET 23
#define TDRAW_TITLE_YOFFSET 6
#define TDRAW_PPREVIEW_XOFFSET 20
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
WINDOW* debug_window;

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

            new_winoff.debug.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.debug.offset.y = 1 + TDRAW_PPREVIEW_YOFFSET + TDRAW_SCORE_YOFFSET;

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

            new_winoff.debug.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.debug.offset.y = 1 + TDRAW_PPREVIEW_YOFFSET + TDRAW_SCORE_YOFFSET;

            new_winoff.title.isRendered = 0;
            
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

        // Create subwindow inside of the box so the box isnt overwritten
        debug_window = derwin(windebug, 8, 17, 1, 1);
        scrollok(debug_window, true);
        touchwin(windebug);
        wrefresh(debug_window);
    }

    return 0;
}

int tdraw_winupdate()
{
    tdraw_winoff_t new_winoff;

    new_winoff = tdraw_calc_offsets();

    if (win_offsets.title.isRendered != new_winoff.title.isRendered)
    {
        // Not enough room for title window, delete window
        if (win_offsets.title.isRendered && !new_winoff.title.isRendered) {
            delwin(wintitle);
            wintitle = NULL;
        }
        // There is room to create title window
        else if (!win_offsets.title.isRendered && new_winoff.title.isRendered) {
            wintitle = newwin(6, 42, new_winoff.title.offset.y, new_winoff.title.offset.x);
            box(winpfield, 0, 0);
            wrefresh(wintitle);
        }
        else {
            mvwin(wintitle, new_winoff.title.offset.y, new_winoff.title.offset.x);
            wrefresh(wintitle);
        }
    }

    if (win_offsets.pfield.isRendered != new_winoff.pfield.isRendered)
    {
        // Not enough room for playfield window, delete window
        if (win_offsets.pfield.isRendered && !new_winoff.pfield.isRendered) {
            delwin(winpfield);
            winpfield = NULL;
        }
        // There is room to create playfield window
        else if (!win_offsets.pfield.isRendered && new_winoff.pfield.isRendered) {
            winpfield = newwin(22, 22, new_winoff.pfield.offset.y, new_winoff.pfield.offset.x);
            box(winpfield, 0, 0);
            wrefresh(winpfield);
        }
        // Playfield window needs to be moved
        else {
            mvwin(winpfield, new_winoff.pfield.offset.y, new_winoff.pfield.offset.x);
            wrefresh(winpfield);
        }
    }

    if (win_offsets.pprev.isRendered != new_winoff.pprev.isRendered)
    {
        // Not enough room for piece preview window, delete window
        if (win_offsets.pprev.isRendered && !new_winoff.pprev.isRendered) {
            delwin(winpprev);
            winpprev = NULL;
        }
        // There is room to create piece preview window
        else if (!win_offsets.pprev.isRendered && new_winoff.pprev.isRendered) {
            winpprev = newwin(6, 19, new_winoff.pprev.offset.y, new_winoff.pprev.offset.x);
            box(winpprev, 0, 0);
            wrefresh(winpprev);
        }
        // Piece preview window needs to be moved
        else {
            mvwin(winpprev, new_winoff.pprev.offset.y, new_winoff.pprev.offset.x);
            wrefresh(winpprev);
        }
    }

    if (win_offsets.score.isRendered != new_winoff.score.isRendered)
    {
        // Not enough room for score window, delete window
        if (win_offsets.score.isRendered && !new_winoff.score.isRendered) {
            delwin(winscore);
            winscore = NULL;
        }
        // There is room to create score window
        else if (!win_offsets.score.isRendered && new_winoff.score.isRendered) {
            winscore = newwin(6, 19, new_winoff.score.offset.y, new_winoff.score.offset.x);
            box(winscore, 0, 0);
            wrefresh(winscore);
        }
        // Score window needs to be moved
        else {
            mvwin(winscore, new_winoff.score.offset.y, new_winoff.score.offset.x);
            wrefresh(winscore);
        }
    }

    if (win_offsets.debug.isRendered != new_winoff.debug.isRendered)
    {
        // Not enough room for debug window, delete window
        if (win_offsets.debug.isRendered && !new_winoff.debug.isRendered) {
            delwin(debug_window);
            delwin(windebug);
            debug_window = NULL;
            windebug = NULL;
        }
        // There is room to create debug window
        else if (!win_offsets.debug.isRendered && new_winoff.debug.isRendered) {
            windebug = newwin(10, 19, new_winoff.debug.offset.y, new_winoff.debug.offset.x);
            box(windebug, 0, 0);
            wrefresh(windebug);

            // Create subwindow inside of the box so the box isnt overwritten
            debug_window = subwin(windebug, 8, 17, getbegy(windebug)+1, getbegx(windebug)+1);
            scrollok(debug_window, true);
            touchwin(windebug);
            wrefresh(debug_window);
        }
        // debug window needs to be moved
        else {
            mvwin(windebug, new_winoff.debug.offset.y, new_winoff.debug.offset.x);
            mvwin(debug_window, getbegy(windebug)+1, getbegx(windebug)+1);
            wrefresh(windebug);
            wrefresh(debug_window);
        }
    }

    win_offsets = new_winoff;

    refresh();

    return 0;
}

int tdraw_touchwin()
{
    if (wintitle != NULL) {
        touchwin(wintitle);
        wrefresh(wintitle);
    }
    if (winpfield != NULL) {
        touchwin(winpfield);
        wrefresh(winpfield);
    }
    if (winpprev != NULL) {
        touchwin(winpprev);
        wrefresh(winpprev);
    }
    if (winscore != NULL) {
        touchwin(winscore);
        wrefresh(winscore);
    }
    if (windebug != NULL) {
        touchwin(windebug);
        wrefresh(windebug);
    }
    if (debug_window != NULL) {
        touchwin(debug_window);
        wrefresh(debug_window);
    }

    return 0;
}