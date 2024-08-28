
#include "tdraw.h"
#include <stdlib.h>
#include <sys/time.h>

tetris_board_t tmp_board;
tetris_game_t tmp_game;

int main()
{
    // itits screen. sets up memory and clears screen
    initscr();
    noecho();
    nodelay(stdscr, true);

    // Refreshes screen to match whats in buffer
    refresh();

    tdraw_wininit();
    tdraw_initcolor();

    int ch;
    struct timeval tstruct;
    uint64_t trefresh = 0;
    uint64_t tnow = 0;

    while(true)
    {
        ch = getch();

        switch (ch)
        {
        case ERR:
            break;

        #ifdef KEY_RESIZE
        case KEY_RESIZE:
            erase();
            tdraw_winupdate();
            tdraw_touchwin();

            mvprintw(0, 0, "%d", ch);
            clrtoeol();

            if (debug_window) {
                wprintw(debug_window, "REFRESH\n");
                wrefresh(debug_window);
            }
            refresh();
            
            break;
        #endif

        case '\n':
        case KEY_ENTER:
            mvprintw(0, 0, "%d", ch);
            clrtoeol();

            if (debug_window) {
                waddch(debug_window, '\n');
                wrefresh(debug_window);
            }
            refresh();
            break;

        default:
            mvprintw(0, 0, "%d", ch);
            clrtoeol();

            if (debug_window) {
                wprintw(debug_window, "%s", keyname(ch));
                wrefresh(debug_window);
            }
            refresh();
            break;
        }


        gettimeofday(&tstruct, NULL);
        tnow = (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;
        if (tnow - trefresh > 500000 /*50ms*/)
        {
            for (int y = 0; y < TETRIS_HEIGHT; y++)
            {
                for (int x = 0; x < TETRIS_WIDTH; x++)
                {
                    tmp_board.pf[y][x] = rand()%8;
                }
            }

            for (int pp = 0; pp < TETRIS_PP_SIZE; pp++)
            {
                tmp_game.ppreview[pp] = rand()%8;
            }

            tdraw_pfield(&tmp_board);
            tdraw_pprev(&tmp_game);

            trefresh = tnow;
        }
    }

    //  ends ncurse
    getch();
    endwin();

    return 0;
}