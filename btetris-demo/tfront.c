#include "tfront.h"
#include "console.h"

tfront_t tfront_info;

console_error_t tfront_draw_boxes(console_info_t* cinfo)
{
    // Find regions that should be rendered based on size
    uint16_t new_rr = 0;
    if (cinfo->c_height >= 22) {
        if (cinfo->c_width >= 42) {
            new_rr |= TFRONT_REG_PLAYFIELD | TFRONT_REG_SCORE_INFO | TFRONT_REG_PPREVIEW | TFRONT_REG_DEBUGR;
        }
        if (cinfo->c_width >= 32) {
            new_rr |= TFRONT_REG_PLAYFIELD | TFRONT_REG_SCORE_INFO | TFRONT_REG_PPREVIEW;
        }
        else if (cinfo->c_width >= 22) {
            new_rr |= TFRONT_REG_PLAYFIELD;
        }
    }
    if (cinfo->c_height >= 26 && cinfo->c_width >= 30) {
        new_rr |= TFRONT_REG_TITLE;
    }


    
    if (new_rr != tfront_info.render_regions) 
    {
        clear_display(cinfo);
        tfront_info.render_regions = new_rr;
    }
    else
    {
        return CONSOLE_SUCCESS;
    }

    // Draw region borders

    if (tfront_info.render_regions & TFRONT_REG_PLAYFIELD)
    {
        tfront_draw_bplayfield(cinfo);
    }

    if (tfront_info.render_regions & TFRONT_REG_PPREVIEW)
    {
        tfront_draw_bppreview(cinfo);
    }

    if (tfront_info.render_regions & TFRONT_REG_SCORE_INFO)
    {
        tfront_draw_bscore(cinfo);
    }

    if (tfront_info.render_regions & TFRONT_REG_TITLE)
    {
        tfront_draw_title(cinfo);
    }

    return CONSOLE_SUCCESS;
}

console_error_t tfront_draw_title(console_info_t* cinfo)
{
    set_cursor_pos(cinfo, 1, 1);
    WriteConsoleW(cinfo->outHandle, L"\x2550\x2550\x2566\x2550\x2550 \x2554\x2550\x2550\x2550 \x2550\x2550\x2566\x2550\x2550 \x2554\x2550\x2550\x2557 \x2550\x2566\x2550 \x2554\x2550\x2550\x2550", 32, NULL, NULL);
    // set_cursor_pos(cinfo, 1, 2);
    // WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x0020\x0020 \x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x0020\x2551 \x0020\x2551\x0020 \x2551\x0020\x0020\x0020\x0020", 30, NULL, NULL);
    set_cursor_pos(cinfo, 1, 2);
    WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x2560\x2550\x2550\x0020 \x0020\x0020\x2551\x0020\x0020 \x2560\x2550\x2566\x255D \x0020\x2551\x0020 \x255A\x2550\x2550\x2557", 32, NULL, NULL);
    set_cursor_pos(cinfo, 1, 3);
    WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x0020\x0020 \x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x2551\x0020 \x0020\x2551\x0020 \x0020\x0020\x0020\x2551", 32, NULL, NULL);
    set_cursor_pos(cinfo, 1, 4);
    WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x255A\x2550\x2550\x2550 \x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x2551\x0020 \x2550\x2569\x2550 \x2550\x2550\x2550\x255D", 32, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tfront_draw_bplayfield(console_info_t* cinfo)
{
    int yoffset = 1;
    if (tfront_info.render_regions & TFRONT_REG_TITLE)
    {
        yoffset += TFRONT_TITLE_YOFFSET;
    }

    set_cursor_pos(cinfo, 1, yoffset);
    WriteConsoleW(cinfo->outHandle, L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557", 23, NULL, NULL);

    for (int i = 1; i <= 20; i++)
    {
        set_cursor_pos(cinfo, 1, yoffset + i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 23, NULL, NULL);
    }

    set_cursor_pos(cinfo, 1, yoffset + 21);
    WriteConsoleW(cinfo->outHandle, L"\x255A\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255D", 23, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tfront_draw_bppreview(console_info_t* cinfo)
{
    int xoffset = 1 + TFRONT_PLAYFIELD_XOFFSET;
    int yoffset = 1;
    
    if (tfront_info.render_regions & TFRONT_REG_TITLE)
    {
        yoffset += TFRONT_TITLE_YOFFSET;
    }

    set_cursor_pos(cinfo, xoffset, yoffset);
    WriteConsoleW(cinfo->outHandle, L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557", 11, NULL, NULL);

    for (int i = 1; i <= 4; i++)
    {
        set_cursor_pos(cinfo, xoffset, yoffset+i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 11, NULL, NULL);
    }

    set_cursor_pos(cinfo, xoffset, yoffset + 5);
    WriteConsoleW(cinfo->outHandle, L"\x255F\x2500\x2500\x2500\x2500\x2500\x2500\x2500\x2500\x2562", 11, NULL, NULL);

    for (int i = 6; i <= 9; i++)
    {
        set_cursor_pos(cinfo, xoffset, yoffset+i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 11, NULL, NULL);
    }

    set_cursor_pos(cinfo, xoffset, yoffset+10);
    WriteConsoleW(cinfo->outHandle, L"\x255A\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255D", 11, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tfront_draw_bscore(console_info_t* cinfo)
{
    int yoffset = 1;
    int xoffset = 1 + TFRONT_PLAYFIELD_XOFFSET;

    if (tfront_info.render_regions & TFRONT_REG_TITLE)
    {
        yoffset += TFRONT_TITLE_YOFFSET;
    }

    if (tfront_info.render_regions & TFRONT_REG_PPREVIEW)
    {
        yoffset += TFRONT_PPREVIEW_YOFFSET;
    }

    set_cursor_pos(cinfo, xoffset, yoffset);
    WriteConsoleW(cinfo->outHandle, L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557", 11, NULL, NULL);

    for (int i = 1; i <= 4; i++)
    {
        set_cursor_pos(cinfo, xoffset, yoffset+i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 11, NULL, NULL);
    }

    set_cursor_pos(cinfo, xoffset, yoffset+5);
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

