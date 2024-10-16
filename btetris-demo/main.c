
#include "btetris_control.h"
#include "tdraw.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

tetris_board_t _board;
tetris_game_t _game;

int main()
{
    // Pointer to globaly allocated game objects
    tetris_board_t* board = &_board;
    tetris_game_t* game = &_game;

    // itits screen. sets up memory and clears screen
    initscr();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);

    // Refreshes screen to match whats in buffer
    refresh();

    // Init UI windows
    tdraw_wininit();
    tdraw_initcolor();

    // Init tetris game objects
    tetris_init(game, board, rand());

    // Open start menu
    tdraw_start(game);

    // Init time info
    int ch;
    struct timeval tstruct;
    uint64_t trefresh = 0;
    uint64_t tprev = 0;
    uint64_t tnow = 0;
    gettimeofday(&tstruct, NULL);
    tnow =      (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;
    tprev =     (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;
    trefresh =  (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;

    // Main keypress loop 
    tetris_error_t tick_result;
    while(true)
    {
        // Get keypress
        ch = getch();

        if (ch != ERR) {
            // Output char for debug info
            if (debug_window) {
                wprintw(debug_window, "%s\n", keyname(ch));
                wrefresh(debug_window);
            }

            // Generate entropy from keypress
            tetris_rand_entropy(game, ch);
        }

        // Handle keypress
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

        // Get current time 
        gettimeofday(&tstruct, NULL);
        tnow = (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;

        // Tick game 100 times a second
        if (tnow - trefresh > 1000000/100 /*100hz*/)
        {
            // Tick the game
            trefresh = tnow;
            tick_result = tetris_tick(game, tnow - tprev);

            // Draw UI
            tdraw_pfield(game);
            tdraw_pprev(game);
            tdraw_score(game);
            tdraw_ginfo(game);
            
            // Handle gameover
            if (tick_result == TETRIS_ERROR_GAME_OVER)
            {
                // Draw gameover menu
                tdraw_gameover(game);
                
                // Reset stopwatch after new game starts
                gettimeofday(&tstruct, NULL);
                tprev = (uint64_t)tstruct.tv_sec * 1000000 + (uint64_t)tstruct.tv_usec;
            }
            else 
            {
                tprev = tnow;
            }
        }
        usleep(1000);
    }

    // ends ncurse
    getch();
    endwin();

    return 0;
}