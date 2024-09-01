
#include "btetris_control.h"
#include "tdraw.h"
#include <stdlib.h>
#include <sys/time.h>

tetris_board_t _board;
tetris_game_t _game;

int main()
{
    tetris_board_t* board = &_board;
    tetris_game_t* game = &_game;

    // itits screen. sets up memory and clears screen
    initscr();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);

    // Refreshes screen to match whats in buffer
    refresh();

    tdraw_wininit();
    tdraw_initcolor();
    tetris_init(game, board, rand());

    int ch;
    struct timeval tstruct;
    uint64_t trefresh = 0;
    uint64_t tprev = 0;
    uint64_t tnow = 0;

    tdraw_start(game);

    gettimeofday(&tstruct, NULL);
    tnow =      (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;
    tprev =     (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;
    trefresh =  (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;
    
    while(true)
    {
        ch = getch();

        if (ch != ERR) {
            mvprintw(0, 0, "%d", ch);
            clrtoeol();

            if (debug_window) {
                wprintw(debug_window, "%s\n", keyname(ch));
                wrefresh(debug_window);
            }

            tetris_rand_entropy(game, ch);
        }

        switch (ch)
        {
        case ERR:
            break;

        #ifdef KEY_RESIZE
        case KEY_RESIZE:
            erase();
            tdraw_winupdate();
            tdraw_touchwin();
            break;
        #endif

        case 's':
        case 'S':
        case KEY_DOWN:
            tetris_sdrop(game);
            break;

        case 'a':
        case 'A':
        case KEY_LEFT:
            tetris_leftshift(game);
            break;

        case 'd':
        case 'D':
        case KEY_RIGHT:
            tetris_rightshift(game);
            break;

        case 'w':
        case 'W':
        case 'e':
        case 'E':
        case KEY_UP:
            tetris_rotcw(game);
            break;

        case 'z':
        case 'Z':
        case 'q':
        case 'Q':
            tetris_rotcntrcw(game);
            break;

        case ' ':
            tetris_hdrop(game);
            break;

        case 'p':
        case 'P':
        case 27:
            // Draw pause menu
            tdraw_pause(game);

            // Reset stopwatch
            gettimeofday(&tstruct, NULL);
            tprev = (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;
            break;
        }

        gettimeofday(&tstruct, NULL);
        tnow = (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;
        if (tnow - trefresh > 1000000/100 /*100hz*/)
        {
            tdraw_pfield(game);
            tdraw_pprev(game);
            tdraw_score(game);
            tdraw_ginfo(game);

            trefresh = tnow;

            tetris_tick(game, tnow - tprev);

            tprev = tnow;
        }
    }

    //  ends ncurse
    getch();
    endwin();

    return 0;
}