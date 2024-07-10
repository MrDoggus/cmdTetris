#include "tfront.h"
#include "console.h"

tfront_t tfront_info;

console_error_t tfront_draw_boxes(console_info_t* cinfo)
{
    // Save previous list of rendered regions 
    uint16_t prev_rr = 0;
    uint16_t new_rr = 0;
    if (tfront_info.title_offset.isRendered) {
        prev_rr |= TFRONT_REG_TITLE;
    }
    if (tfront_info.pfield_offset.isRendered) {
        prev_rr |= TFRONT_REG_PLAYFIELD;
    }
    if (tfront_info.pprev_offset.isRendered) {
        prev_rr |= TFRONT_REG_PPREVIEW;
    }
    if (tfront_info.score_offset.isRendered) {
        prev_rr |= TFRONT_REG_SCORE;
    }
    if (tfront_info.debug_offset.isRendered) {
        prev_rr |= TFRONT_REG_DEBUGR;
    }

    // Find regions that should be rendered based on console size

    if (cinfo->c_height >= 26) {
        if (cinfo->c_width >= 42) {
            new_rr = TFRONT_REG_TITLE | TFRONT_REG_PLAYFIELD | TFRONT_REG_PPREVIEW | TFRONT_REG_SCORE | TFRONT_REG_DEBUGR;
            tfront_info.title_offset.coord.x = 1;
            tfront_info.title_offset.coord.y = 1;

            tfront_info.pfield_offset.coord.x = 1;
            tfront_info.pfield_offset.coord.y = 1 + TFRONT_TITLE_YOFFSET;

            tfront_info.pprev_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET;
            tfront_info.pprev_offset.coord.y = 1 + TFRONT_TITLE_YOFFSET;

            tfront_info.score_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET;
            tfront_info.score_offset.coord.y = 1 + TFRONT_TITLE_YOFFSET + TFRONT_PPREVIEW_YOFFSET;

            tfront_info.debug_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET + TFRONT_PPREVIEW_XOFFSET;
            tfront_info.debug_offset.coord.y = 1 + TFRONT_TITLE_YOFFSET + TFRONT_PPREVIEW_YOFFSET;
        }
        else if (cinfo->c_width >= 32) {
            new_rr = TFRONT_REG_TITLE | TFRONT_REG_PLAYFIELD | TFRONT_REG_PPREVIEW | TFRONT_REG_SCORE;

            tfront_info.title_offset.coord.x = 1;
            tfront_info.title_offset.coord.y = 1;

            tfront_info.pfield_offset.coord.x = 1;
            tfront_info.pfield_offset.coord.y = 1 + TFRONT_TITLE_YOFFSET;

            tfront_info.pprev_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET;
            tfront_info.pprev_offset.coord.y = 1 + TFRONT_TITLE_YOFFSET;
            
            tfront_info.score_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET;
            tfront_info.score_offset.coord.y = 1 + TFRONT_TITLE_YOFFSET + TFRONT_PPREVIEW_YOFFSET;

            tfront_info.debug_offset.isRendered = 0;
        }
        else if (cinfo->c_width >= 22) {
            new_rr = TFRONT_REG_TITLE | TFRONT_REG_PLAYFIELD;

            tfront_info.title_offset.coord.x = 1;
            tfront_info.title_offset.coord.y = 1;

            tfront_info.pfield_offset.coord.x = 1;
            tfront_info.pfield_offset.coord.y = 1 + TFRONT_TITLE_YOFFSET;

            tfront_info.pprev_offset.isRendered = 0;
            tfront_info.score_offset.isRendered = 0;
            tfront_info.title_offset.isRendered = 0;
        }
    }
    else if (cinfo->c_height >= 22) {
        if (cinfo->c_width >= 42) {
            new_rr = TFRONT_REG_PLAYFIELD | TFRONT_REG_PPREVIEW | TFRONT_REG_SCORE | TFRONT_REG_DEBUGR;

            tfront_info.pfield_offset.coord.x = 1;
            tfront_info.pfield_offset.coord.y = 1;

            tfront_info.pprev_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET;
            tfront_info.pprev_offset.coord.y = 1;

            tfront_info.score_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET;
            tfront_info.score_offset.coord.y = 1 + TFRONT_PPREVIEW_YOFFSET;

            tfront_info.debug_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET + TFRONT_PPREVIEW_XOFFSET;
            tfront_info.debug_offset.coord.y = 1 + TFRONT_PPREVIEW_YOFFSET;

            tfront_info.title_offset.isRendered = 0;
        }
        else if (cinfo->c_width >= 32) {
            new_rr = TFRONT_REG_PLAYFIELD | TFRONT_REG_PPREVIEW | TFRONT_REG_SCORE;
            
            tfront_info.pfield_offset.coord.x = 1;
            tfront_info.pfield_offset.coord.y = 1;

            tfront_info.pprev_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET;
            tfront_info.pprev_offset.coord.y = 1;

            tfront_info.score_offset.coord.x = 1 + TFRONT_PLAYFIELD_XOFFSET;
            tfront_info.score_offset.coord.y = 1 + TFRONT_PPREVIEW_YOFFSET;

            tfront_info.title_offset.isRendered = 0;
            tfront_info.debug_offset.isRendered = 0;
        }
        else if (cinfo->c_width >= 22) {
            new_rr = TFRONT_REG_PLAYFIELD;

            tfront_info.pfield_offset.coord.x = 1;
            tfront_info.pfield_offset.coord.y = 1;

            tfront_info.title_offset.isRendered = 0;
            tfront_info.pprev_offset.isRendered = 0;
            tfront_info.score_offset.isRendered = 0;
            tfront_info.debug_offset.isRendered = 0;
        }
        else {
            new_rr = 0;
            tfront_info.pfield_offset.isRendered = 0;
            tfront_info.title_offset.isRendered = 0;
            tfront_info.pprev_offset.isRendered = 0;
            tfront_info.score_offset.isRendered = 0;
            tfront_info.debug_offset.isRendered = 0;
        }
    }
    else {
        new_rr = 0;
        tfront_info.pfield_offset.isRendered = 0;
        tfront_info.title_offset.isRendered = 0;
        tfront_info.pprev_offset.isRendered = 0;
        tfront_info.score_offset.isRendered = 0;
        tfront_info.debug_offset.isRendered = 0;
    }
    
    if (prev_rr != new_rr) 
    {
        clear_display(cinfo);
    }
    else
    {
        return CONSOLE_SUCCESS;
    }

    // Draw region borders

    if (tfront_info.pfield_offset.isRendered)
    {
        tfront_draw_bplayfield(cinfo);
    }

    if (tfront_info.pprev_offset.isRendered)
    {
        tfront_draw_bppreview(cinfo);
    }

    if (tfront_info.score_offset.isRendered)
    {
        tfront_draw_bscore(cinfo);
    }

    if (tfront_info.title_offset.isRendered)
    {
        tfront_draw_title(cinfo);
    }

    return CONSOLE_SUCCESS;
}

console_error_t tfront_draw_title(console_info_t* cinfo)
{
    set_cursor_pos(cinfo, tfront_info.title_offset.coord.x, tfront_info.title_offset.coord.y);
    WriteConsoleW(cinfo->outHandle, L"\x2550\x2550\x2566\x2550\x2550 \x2554\x2550\x2550\x2550 \x2550\x2550\x2566\x2550\x2550 \x2554\x2550\x2550\x2557 \x2550\x2566\x2550 \x2554\x2550\x2550\x2550", 32, NULL, NULL);

    set_cursor_pos(cinfo, tfront_info.title_offset.coord.x, tfront_info.title_offset.coord.y + 1);
    WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x2560\x2550\x2550\x0020 \x0020\x0020\x2551\x0020\x0020 \x2560\x2550\x2566\x255D \x0020\x2551\x0020 \x255A\x2550\x2550\x2557", 32, NULL, NULL);

    set_cursor_pos(cinfo, tfront_info.title_offset.coord.x, tfront_info.title_offset.coord.y + 2);
    WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x0020\x0020 \x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x2551\x0020 \x0020\x2551\x0020 \x0020\x0020\x0020\x2551", 32, NULL, NULL);

    set_cursor_pos(cinfo, tfront_info.title_offset.coord.x, tfront_info.title_offset.coord.y + 3);
    WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x255A\x2550\x2550\x2550 \x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x2551\x0020 \x2550\x2569\x2550 \x2550\x2550\x2550\x255D", 32, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tfront_draw_bplayfield(console_info_t* cinfo)
{
    set_cursor_pos(cinfo, tfront_info.pfield_offset.coord.x, tfront_info.pfield_offset.coord.y);
    WriteConsoleW(cinfo->outHandle, L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557", 23, NULL, NULL);

    for (int i = 1; i <= 20; i++)
    {
        set_cursor_pos(cinfo, tfront_info.pfield_offset.coord.x, tfront_info.pfield_offset.coord.y + i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 23, NULL, NULL);
    }

    set_cursor_pos(cinfo, tfront_info.pfield_offset.coord.x, tfront_info.pfield_offset.coord.y + 21);
    WriteConsoleW(cinfo->outHandle, L"\x255A\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255D", 23, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tfront_draw_bppreview(console_info_t* cinfo)
{
    set_cursor_pos(cinfo, tfront_info.pprev_offset.coord.x, tfront_info.pprev_offset.coord.y);
    WriteConsoleW(cinfo->outHandle, L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557", 11, NULL, NULL);

    for (int i = 1; i <= 4; i++)
    {
        set_cursor_pos(cinfo, tfront_info.pprev_offset.coord.x, tfront_info.pprev_offset.coord.y + i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 11, NULL, NULL);
    }

    set_cursor_pos(cinfo, tfront_info.pprev_offset.coord.x, tfront_info.pprev_offset.coord.y + 5);
    WriteConsoleW(cinfo->outHandle, L"\x255F\x2500\x2500\x2500\x2500\x2500\x2500\x2500\x2500\x2562", 11, NULL, NULL);

    for (int i = 6; i <= 9; i++)
    {
        set_cursor_pos(cinfo, tfront_info.pprev_offset.coord.x, tfront_info.pprev_offset.coord.y + i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 11, NULL, NULL);
    }

    set_cursor_pos(cinfo, tfront_info.pprev_offset.coord.x, tfront_info.pprev_offset.coord.y + 10);
    WriteConsoleW(cinfo->outHandle, L"\x255A\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255D", 11, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tfront_draw_bscore(console_info_t* cinfo)
{
    set_cursor_pos(cinfo, tfront_info.score_offset.coord.x, tfront_info.score_offset.coord.y);
    WriteConsoleW(cinfo->outHandle, L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557", 11, NULL, NULL);

    for (int i = 1; i <= 4; i++)
    {
        set_cursor_pos(cinfo, tfront_info.score_offset.coord.x, tfront_info.score_offset.coord.y + i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 11, NULL, NULL);
    }

    set_cursor_pos(cinfo, tfront_info.score_offset.coord.x, tfront_info.score_offset.coord.y + 5);
    WriteConsoleW(cinfo->outHandle, L"\x255A\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255D", 11, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tfront_window_event(console_info_t* cinfo, PWINDOW_BUFFER_SIZE_RECORD record)
{

    cinfo->c_width = record->dwSize.X;
    cinfo->c_height = record->dwSize.Y;

    tfront_draw_boxes(cinfo);

    return CONSOLE_SUCCESS;
}

