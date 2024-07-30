#include <stdlib.h>
#include "tdraw.h"
#include "console.h"

BOOL ctrlhandler(DWORD dwCtrlType);

console_info_t* cinfo;

tetris_board_t tboard;

int main()
{
    // --- Setup console --- //
    
    cinfo = setup_console();
    if (!cinfo) {
        return 1;
    }

    cinfo->window_event = &tdraw_window_event;
    cinfo->focus_event = &focus_event;

    if (cinfo->c_height == 0 && cinfo->c_width == 0) {
        cinfo->c_height = 22;
        cinfo->c_width = 22;
    }

    const int STRBUFF_LEN = 32;
    char strbuff[STRBUFF_LEN];

    int retval;
    retval = SetConsoleMode(cinfo->inHandle, ENABLE_PROCESSED_INPUT | /*ENABLE_MOUSE_INPUT |*/ ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS );
    if (!retval) return 2;
    retval = SetConsoleMode(cinfo->outHandle, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    if (!retval) return 2;

    // const int STR_BUFFLEN = 64;
    // char strbuff[STR_BUFFLEN];

    // set CTRL handler
    SetConsoleCtrlHandler(&ctrlhandler, 1);
    
    hide_cursor(cinfo);

    snprintf(strbuff, STRBUFF_LEN, "(x: %d, y: %d)\n", cinfo->c_width, cinfo->c_height);
    WriteConsoleA(cinfo->outHandle, strbuff, strlen(strbuff), NULL, NULL);

    clear_display(cinfo);

    tdraw_boxes(cinfo);

    tetris_color_t tcol;
    while(1)
    {
        handle_events(cinfo, 0);

        for (int y = 0; y < TETRIS_HEIGHT; y++)
        {
            for (int x = 0; x < TETRIS_WIDTH; x++)
            {
                tcol = rand()%8;
                tboard.pf[y][x] = tcol;
            }
        }

        tdraw_playfield(cinfo, &tboard);

        Sleep(200);
    }

    return 0;
}

BOOL ctrlhandler(DWORD dwCT)
{
    if (dwCT == CTRL_C_EVENT || dwCT == CTRL_BREAK_EVENT)
    {
        show_cursor(cinfo);
        clear_display(cinfo);
        ExitProcess(0);
    }
    else 
    {
        clear_display(cinfo);
        ExitProcess(0);
    }
}