#include "tdraw.h"

tdraw_info_t draw_info;

console_error_t tdraw_playfield(console_info_t* cinfo, tetris_board_t* board)
{
    if (!draw_info.pfield_offset.isRendered) {
        return CONSOLE_ERR_GENERIC;
    }

    for (int h = 0; h < TETRIS_HEIGHT; h++)
    {
        set_cursor_pos(cinfo, draw_info.pfield_offset.coord.x + 1, draw_info.pfield_offset.coord.y + 1 + h);

        for (int w = 0; w < TETRIS_WIDTH; w++)
        {
            tdraw_block(cinfo, board->pf[h][w]);
        }
    }

    return CONSOLE_SUCCESS;
}

console_error_t tdraw_ppreview(console_info_t* cinfo, tetris_game_t* game)
{
    if (!draw_info.pprev_offset.isRendered) {
        return CONSOLE_ERR_GENERIC;
    }

    // Use tetromino starting position data to render tetrominoes in piece preview
    tetris_coord_t tmp_coord;
    const int HOFFSET = (TETRIS_HEIGHT);
    const int WOFFSET = ((TETRIS_WIDTH/2) - 2);
    char isOddWidth;   // If piece preview tetromino has an even width
    int tblock_idx;     // Index for piece preview tetromino block
    // Loop through piece previews
    for (int pp = 0; pp < TETRIS_PP_SIZE; pp++)
    {
        // Dont draw tetromino if blank
        if (game->ppreview[pp] == TETRIS_BLANK) {
            continue;
        }

        // Used to center tetrominoes with an odd width
        if (game->ppreview[pp] > 2) {
            isOddWidth = 1;
        }
        else {
            isOddWidth = 0;
        }

        tblock_idx = 0;

        // Loop through piece preview grid
        for (int h = 3; h >= 0; h--)
        {
            set_cursor_pos(cinfo, draw_info.pprev_offset.coord.x + 1, draw_info.pprev_offset.coord.y + 1 + pp*5 + h);

            if (isOddWidth) {
                WriteConsoleA(cinfo->outHandle, " ", 2, NULL, NULL);
            }

            for (int w = 0; w < 4 - isOddWidth; w++)
            {
                // Get starting position
                tmp_coord = TETRIS_TETROMINO_START[game->ppreview[pp]][tblock_idx];

                // Apply offset
                tmp_coord = tetris_subCoord(tmp_coord, (tetris_coord_t){.h = HOFFSET, .w = WOFFSET});

                // Check if this position should have a block. Don't need to check every idx of TETRIS_TETROMINO_START because it is sorted
                if (tmp_coord.h == h && tmp_coord.w == w)
                {
                    tdraw_block(cinfo, game->ppreview[pp]);
                    tblock_idx++;
                }
                else 
                {
                    tdraw_block(cinfo, TETRIS_BLANK);
                }
            }

            if (isOddWidth) {
                WriteConsoleA(cinfo->outHandle, " ", 2, NULL, NULL);
            }
        }
    }

    return CONSOLE_SUCCESS;
}

console_error_t tdraw_boxes(console_info_t* cinfo)
{
    // Save previous list of rendered regions 
    uint16_t prev_rr = 0;
    uint16_t new_rr = 0;
    if (draw_info.title_offset.isRendered) {
        prev_rr |= TDRAW_REG_TITLE;
    }
    if (draw_info.pfield_offset.isRendered) {
        prev_rr |= TDRAW_REG_PLAYFIELD;
    }
    if (draw_info.pprev_offset.isRendered) {
        prev_rr |= TDRAW_REG_PPREVIEW;
    }
    if (draw_info.score_offset.isRendered) {
        prev_rr |= TDRAW_REG_SCORE;
    }
    if (draw_info.debug_offset.isRendered) {
        prev_rr |= TDRAW_REG_DEBUGR;
    }

    // Find regions that should be rendered based on console size

    if (cinfo->c_height >= TDRAW_TITLE_YOFFSET + TDRAW_PLAYFIELD_YOFFSET) 
    {
        if (cinfo->c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET + TDRAW_DEBUG_XOFFSET) 
        {
            new_rr = TDRAW_REG_TITLE | TDRAW_REG_PLAYFIELD | TDRAW_REG_PPREVIEW | TDRAW_REG_SCORE | TDRAW_REG_DEBUGR;
            draw_info.title_offset.coord.x = 1;
            draw_info.title_offset.coord.y = 1;

            draw_info.pfield_offset.coord.x = 1;
            draw_info.pfield_offset.coord.y = 1 + TDRAW_TITLE_YOFFSET;

            draw_info.pprev_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            draw_info.pprev_offset.coord.y = 1 + TDRAW_TITLE_YOFFSET;

            draw_info.score_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            draw_info.score_offset.coord.y = 1 + TDRAW_TITLE_YOFFSET + TDRAW_PPREVIEW_YOFFSET;

            draw_info.debug_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET;
            draw_info.debug_offset.coord.y = 1 + TDRAW_TITLE_YOFFSET + TDRAW_PPREVIEW_YOFFSET;
        }
        else if (cinfo->c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET) 
        {
            new_rr = TDRAW_REG_TITLE | TDRAW_REG_PLAYFIELD | TDRAW_REG_PPREVIEW | TDRAW_REG_SCORE;

            draw_info.title_offset.coord.x = 1;
            draw_info.title_offset.coord.y = 1;

            draw_info.pfield_offset.coord.x = 1;
            draw_info.pfield_offset.coord.y = 1 + TDRAW_TITLE_YOFFSET;

            draw_info.pprev_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            draw_info.pprev_offset.coord.y = 1 + TDRAW_TITLE_YOFFSET;
            
            draw_info.score_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            draw_info.score_offset.coord.y = 1 + TDRAW_TITLE_YOFFSET + TDRAW_PPREVIEW_YOFFSET;

            draw_info.debug_offset.isRendered = 0;
        }
        else if (cinfo->c_width >= TDRAW_PLAYFIELD_XOFFSET) 
        {
            new_rr = TDRAW_REG_TITLE | TDRAW_REG_PLAYFIELD;

            draw_info.title_offset.coord.x = 1;
            draw_info.title_offset.coord.y = 1;

            draw_info.pfield_offset.coord.x = 1;
            draw_info.pfield_offset.coord.y = 1 + TDRAW_TITLE_YOFFSET;

            draw_info.pprev_offset.isRendered = 0;
            draw_info.score_offset.isRendered = 0;
            draw_info.title_offset.isRendered = 0;
        }
    }
    else if (cinfo->c_height >= TDRAW_PLAYFIELD_YOFFSET) 
    {
        if (cinfo->c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET + TDRAW_DEBUG_XOFFSET) 
        {
            new_rr = TDRAW_REG_PLAYFIELD | TDRAW_REG_PPREVIEW | TDRAW_REG_SCORE | TDRAW_REG_DEBUGR;

            draw_info.pfield_offset.coord.x = 1;
            draw_info.pfield_offset.coord.y = 1;

            draw_info.pprev_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            draw_info.pprev_offset.coord.y = 1;

            draw_info.score_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            draw_info.score_offset.coord.y = 1 + TDRAW_PPREVIEW_YOFFSET;

            draw_info.debug_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET;
            draw_info.debug_offset.coord.y = 1 + TDRAW_PPREVIEW_YOFFSET;

            draw_info.title_offset.isRendered = 0;
        }
        else if (cinfo->c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET) 
        {
            new_rr = TDRAW_REG_PLAYFIELD | TDRAW_REG_PPREVIEW | TDRAW_REG_SCORE;
            
            draw_info.pfield_offset.coord.x = 1;
            draw_info.pfield_offset.coord.y = 1;

            draw_info.pprev_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            draw_info.pprev_offset.coord.y = 1;

            draw_info.score_offset.coord.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            draw_info.score_offset.coord.y = 1 + TDRAW_PPREVIEW_YOFFSET;

            draw_info.title_offset.isRendered = 0;
            draw_info.debug_offset.isRendered = 0;
        }
        else if (cinfo->c_width >= TDRAW_PLAYFIELD_XOFFSET) 
        {
            new_rr = TDRAW_REG_PLAYFIELD;

            draw_info.pfield_offset.coord.x = 1;
            draw_info.pfield_offset.coord.y = 1;

            draw_info.title_offset.isRendered = 0;
            draw_info.pprev_offset.isRendered = 0;
            draw_info.score_offset.isRendered = 0;
            draw_info.debug_offset.isRendered = 0;
        }
        else 
        {
            new_rr = 0;
            draw_info.pfield_offset.isRendered = 0;
            draw_info.title_offset.isRendered = 0;
            draw_info.pprev_offset.isRendered = 0;
            draw_info.score_offset.isRendered = 0;
            draw_info.debug_offset.isRendered = 0;
        }
    }
    else 
    {
        new_rr = 0;
        draw_info.pfield_offset.isRendered = 0;
        draw_info.title_offset.isRendered = 0;
        draw_info.pprev_offset.isRendered = 0;
        draw_info.score_offset.isRendered = 0;
        draw_info.debug_offset.isRendered = 0;
    }
    
    if (prev_rr != new_rr) {
        clear_display(cinfo);
    }
    else {
        return CONSOLE_SUCCESS;
    }

    // Draw region borders

    if (draw_info.pfield_offset.isRendered) {
        tdraw_bplayfield(cinfo);
    }

    if (draw_info.pprev_offset.isRendered) {
        tdraw_bppreview(cinfo);
    }

    if (draw_info.score_offset.isRendered) {
        tdraw_bscore(cinfo);
    }

    if (draw_info.title_offset.isRendered) {
        tdraw_title(cinfo);
    }

    return CONSOLE_SUCCESS;
}

console_error_t tdraw_title(console_info_t* cinfo)
{
    set_cursor_pos(cinfo, draw_info.title_offset.coord.x, draw_info.title_offset.coord.y);
    WriteConsoleW(cinfo->outHandle, L"\x2550\x2550\x2566\x2550\x2550 \x2554\x2550\x2550\x2550 \x2550\x2550\x2566\x2550\x2550 \x2554\x2550\x2550\x2557 \x2550\x2566\x2550 \x2554\x2550\x2550\x2550", 32, NULL, NULL);

    set_cursor_pos(cinfo, draw_info.title_offset.coord.x, draw_info.title_offset.coord.y + 1);
    WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x2560\x2550\x2550\x0020 \x0020\x0020\x2551\x0020\x0020 \x2560\x2550\x2566\x255D \x0020\x2551\x0020 \x255A\x2550\x2550\x2557", 32, NULL, NULL);

    set_cursor_pos(cinfo, draw_info.title_offset.coord.x, draw_info.title_offset.coord.y + 2);
    WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x0020\x0020 \x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x2551\x0020 \x0020\x2551\x0020 \x0020\x0020\x0020\x2551", 32, NULL, NULL);

    set_cursor_pos(cinfo, draw_info.title_offset.coord.x, draw_info.title_offset.coord.y + 3);
    WriteConsoleW(cinfo->outHandle, L"\x0020\x0020\x2551\x0020\x0020 \x255A\x2550\x2550\x2550 \x0020\x0020\x2551\x0020\x0020 \x2551\x0020\x2551\x0020 \x2550\x2569\x2550 \x2550\x2550\x2550\x255D", 32, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tdraw_bplayfield(console_info_t* cinfo)
{
    set_cursor_pos(cinfo, draw_info.pfield_offset.coord.x, draw_info.pfield_offset.coord.y);
    WriteConsoleW(cinfo->outHandle, L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557", 23, NULL, NULL);

    for (int i = 1; i <= 20; i++)
    {
        set_cursor_pos(cinfo, draw_info.pfield_offset.coord.x, draw_info.pfield_offset.coord.y + i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 23, NULL, NULL);
    }

    set_cursor_pos(cinfo, draw_info.pfield_offset.coord.x, draw_info.pfield_offset.coord.y + 21);
    WriteConsoleW(cinfo->outHandle, L"\x255A\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255D", 23, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tdraw_bppreview(console_info_t* cinfo)
{
    set_cursor_pos(cinfo, draw_info.pprev_offset.coord.x, draw_info.pprev_offset.coord.y);
    WriteConsoleW(cinfo->outHandle, L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557", 11, NULL, NULL);

    for (int i = 1; i <= 4; i++)
    {
        set_cursor_pos(cinfo, draw_info.pprev_offset.coord.x, draw_info.pprev_offset.coord.y + i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 11, NULL, NULL);
    }

    set_cursor_pos(cinfo, draw_info.pprev_offset.coord.x, draw_info.pprev_offset.coord.y + 5);
    WriteConsoleW(cinfo->outHandle, L"\x255F\x2500\x2500\x2500\x2500\x2500\x2500\x2500\x2500\x2562", 11, NULL, NULL);

    for (int i = 6; i <= 9; i++)
    {
        set_cursor_pos(cinfo, draw_info.pprev_offset.coord.x, draw_info.pprev_offset.coord.y + i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 11, NULL, NULL);
    }

    set_cursor_pos(cinfo, draw_info.pprev_offset.coord.x, draw_info.pprev_offset.coord.y + 10);
    WriteConsoleW(cinfo->outHandle, L"\x255A\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255D", 11, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tdraw_bscore(console_info_t* cinfo)
{
    set_cursor_pos(cinfo, draw_info.score_offset.coord.x, draw_info.score_offset.coord.y);
    WriteConsoleW(cinfo->outHandle, L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557", 11, NULL, NULL);

    for (int i = 1; i <= 4; i++)
    {
        set_cursor_pos(cinfo, draw_info.score_offset.coord.x, draw_info.score_offset.coord.y + i);
        WriteConsoleW(cinfo->outHandle, L"\x2551\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x0020\x2551", 11, NULL, NULL);
    }

    set_cursor_pos(cinfo, draw_info.score_offset.coord.x, draw_info.score_offset.coord.y + 5);
    WriteConsoleW(cinfo->outHandle, L"\x255A\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255D", 11, NULL, NULL);

    return CONSOLE_SUCCESS;
}

console_error_t tdraw_block(console_info_t* cinfo, tetris_color_t color)
{
    switch (color)
    {
    case TETRIS_CYAN:
        set_foreground_color(cinfo, CONSOLE_COL_CYAN);
        WriteConsoleW(cinfo->outHandle, L"\x2588\x2588", 3, NULL, NULL);
        break;
    case TETRIS_BLUE:
        set_foreground_color(cinfo, CONSOLE_COL_BLUE);
        WriteConsoleW(cinfo->outHandle, L"\x2588\x2588", 3, NULL, NULL);
        break;
    case TETRIS_ORANGE:
        set_foreground_color(cinfo, CONSOLE_COL_ORANGE);
        WriteConsoleW(cinfo->outHandle, L"\x2588\x2588", 3, NULL, NULL);
        break;
    case TETRIS_YELLOW:
        set_foreground_color(cinfo, CONSOLE_COL_YELLOW);
        WriteConsoleW(cinfo->outHandle, L"\x2588\x2588", 3, NULL, NULL);
        break;
    case TETRIS_GREEN:
        set_foreground_color(cinfo, CONSOLE_COL_GREEN);
        WriteConsoleW(cinfo->outHandle, L"\x2588\x2588", 3, NULL, NULL);
        break;
    case TETRIS_PURPLE:
        set_foreground_color(cinfo, CONSOLE_COL_MAGENTA);
        WriteConsoleW(cinfo->outHandle, L"\x2588\x2588", 3, NULL, NULL);
        break;
    case TETRIS_RED:
        set_foreground_color(cinfo, CONSOLE_COL_RED);
        WriteConsoleW(cinfo->outHandle, L"\x2588\x2588", 3, NULL, NULL);
        break;
    default:
        WriteConsoleA(cinfo->outHandle, "  ", 3, NULL, NULL);
        break;
    }

    reset_color(cinfo);

    return CONSOLE_SUCCESS;
}

console_error_t tdraw_window_event(console_info_t* cinfo, PWINDOW_BUFFER_SIZE_RECORD record)
{

    cinfo->c_width = record->dwSize.X;
    cinfo->c_height = record->dwSize.Y;

    tdraw_boxes(cinfo);

    return CONSOLE_SUCCESS;
}

