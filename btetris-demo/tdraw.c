#include "tdraw.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// --- DEFINITIONS --- //

#define TDRAW_PLAYFIELD_XOFFSET (TETRIS_WIDTH*2 + 2 + 1)
#define TDRAW_PLAYFIELD_YOFFSET (TETRIS_HEIGHT + 2 + 1)
#define TDRAW_TITLE_YOFFSET 6
#define TDRAW_PPREVIEW_XOFFSET 20
#define TDRAW_PPREVIEW_YOFFSET 6
#define TDRAW_SCORE_YOFFSET 4
#define TDRAW_GINFO_XOFFSET 32

typedef union tdraw_coord {
    uint32_t isRendered;
    struct {
        uint16_t x;
        uint16_t y;
    } offset;
} tdraw_coord_t;

typedef struct tdraw_winoff {
    int draw_width;
    int draw_height;
    tdraw_coord_t title;
    tdraw_coord_t pfield;
    tdraw_coord_t pprev;
    tdraw_coord_t score;
    tdraw_coord_t debug;
    tdraw_coord_t ginfo;
} tdraw_winoff_t;

typedef struct tdraw_menuitem {
    char* name;
    char* desc;
    int loffset;
} tdraw_menuitem;


// --- GLOBAL VARS --- //

tdraw_winoff_t win_offsets;

WINDOW* wintitle;
WINDOW* winpfield;
WINDOW* winpprev;
WINDOW* winscore;
WINDOW* windebug;
WINDOW* winginfo;
WINDOW* debug_window;


// --- Private Functions --- //

/// @brief Prints a tetromino pixel to the screen at given window
/// @param window Window to print char to
/// @param bcolor Color of tetromino to use
/// @return Error value
int tdraw_block(WINDOW* window, tetris_color_t color);

/// @brief Calculate the offsets for the  UI windows
/// @return return window offset data structure. 
tdraw_winoff_t tdraw_calc_offsets();

/// @brief Draws keybind info window 
/// @return Error value
int tdraw_keybinds();

int tdraw_block(WINDOW* window, tetris_color_t bcolor)
{
    chtype solid_block; // Character used to display pixel
    tdraw_error_t retval = SUCCESS;

    if (!window) {
        return ERROR_NULL_INARG;
    }

    // If terminal has colors, use color pair attributes 
    if (has_colors())
    {
        // Add color attribute to char before printing
        if (bcolor) {
            solid_block = ('0'+bcolor) | COLOR_PAIR(bcolor+8);
        }
        else {
            solid_block = ' ' | COLOR_PAIR(8);
        }
        
        // Print characters to window. Continue on error
        if (waddch(window, solid_block) == ERR) retval = ERROR_NCURSES_PRINT;
        if (waddch(window, solid_block) == ERR) retval = ERROR_NCURSES_PRINT;
    }
    else
    {
        // Use number to represent the color of the tetromino since color isnt available. 
        if (bcolor) {
            solid_block = ('0'+bcolor);
        }
        else {
            solid_block = ' ';
        }
        
        // Print characters to window. Continue on error
        if (waddch(window, solid_block) == ERR) retval = ERROR_NCURSES_PRINT;
        if (waddch(window, solid_block) == ERR) retval = ERROR_NCURSES_PRINT;
    }

    return retval;
}

tdraw_winoff_t tdraw_calc_offsets()
{
    int c_height, c_width;      // Console width and height
    tdraw_winoff_t new_winoff;  // Return value

    // Get terminal size
    getmaxyx(stdscr, c_height, c_width);

    // --- Calculate offsets --- //

    // Tall enough for title and playfield
    if (c_height >= TDRAW_TITLE_YOFFSET + TDRAW_PLAYFIELD_YOFFSET) 
    {
        new_winoff.draw_height = TDRAW_TITLE_YOFFSET + TDRAW_PLAYFIELD_YOFFSET;

        // Wide enough for Playfield and two side bars (ppreview and ginfo)
        if (c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET + TDRAW_GINFO_XOFFSET) 
        {
            new_winoff.draw_width = TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET + TDRAW_GINFO_XOFFSET;

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

            new_winoff.ginfo.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET;
            new_winoff.ginfo.offset.y = 1 + TDRAW_TITLE_YOFFSET;
        }
        // Wide enough for Playfield and one side bars (ppreview)
        else if (c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET) 
        {
            new_winoff.draw_width = TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET;

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

            new_winoff.ginfo.isRendered = 0;
        }
        // Wide enough for Playfield only
        else if (c_width >= TDRAW_PLAYFIELD_XOFFSET) 
        {
            new_winoff.draw_width = TDRAW_PLAYFIELD_XOFFSET;

            new_winoff.title.offset.x = 1;
            new_winoff.title.offset.y = 1;

            new_winoff.pfield.offset.x = 1;
            new_winoff.pfield.offset.y = 1 + TDRAW_TITLE_YOFFSET;

            new_winoff.pprev.isRendered = 0;
            new_winoff.score.isRendered = 0;
            new_winoff.title.isRendered = 0;
            new_winoff.ginfo.isRendered = 0;
        }
        // Not wide enough to display any game information
        else
        {
            new_winoff.draw_width = 0;

            new_winoff.pfield.isRendered = 0;
            new_winoff.title.isRendered = 0;
            new_winoff.pprev.isRendered = 0;
            new_winoff.score.isRendered = 0;
            new_winoff.debug.isRendered = 0;
            new_winoff.ginfo.isRendered = 0;
        }
    }
    // Not tall enough to display title
    else if (c_height >= TDRAW_PLAYFIELD_YOFFSET) 
    {
        new_winoff.draw_height = TDRAW_PLAYFIELD_YOFFSET;

        // Wide enough for Playfield and one side bars (ppreview)
        if (c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET + TDRAW_GINFO_XOFFSET) 
        {
            new_winoff.draw_width = TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET + TDRAW_GINFO_XOFFSET;

            new_winoff.pfield.offset.x = 1;
            new_winoff.pfield.offset.y = 1;

            new_winoff.pprev.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.pprev.offset.y = 1;

            new_winoff.score.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.score.offset.y = 1 + TDRAW_PPREVIEW_YOFFSET;

            new_winoff.debug.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.debug.offset.y = 1 + TDRAW_PPREVIEW_YOFFSET + TDRAW_SCORE_YOFFSET;

            new_winoff.ginfo.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET;
            new_winoff.ginfo.offset.y = 1;

            new_winoff.title.isRendered = 0;
        }
        // Wide enough for Playfield and one side bars (ppreview)
        else if (c_width >= TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET) 
        {            
            new_winoff.draw_width = TDRAW_PLAYFIELD_XOFFSET + TDRAW_PPREVIEW_XOFFSET;

            new_winoff.pfield.offset.x = 1;
            new_winoff.pfield.offset.y = 1;

            new_winoff.pprev.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.pprev.offset.y = 1;

            new_winoff.score.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.score.offset.y = 1 + TDRAW_PPREVIEW_YOFFSET;

            new_winoff.debug.offset.x = 1 + TDRAW_PLAYFIELD_XOFFSET;
            new_winoff.debug.offset.y = 1 + TDRAW_PPREVIEW_YOFFSET + TDRAW_SCORE_YOFFSET;

            new_winoff.title.isRendered = 0;
            new_winoff.ginfo.isRendered = 0;
        }
        // Wide enough for Playfield only
        else if (c_width >= TDRAW_PLAYFIELD_XOFFSET) 
        {
            new_winoff.draw_width = TDRAW_PLAYFIELD_XOFFSET;

            new_winoff.pfield.offset.x = 1;
            new_winoff.pfield.offset.y = 1;

            new_winoff.title.isRendered = 0;
            new_winoff.pprev.isRendered = 0;
            new_winoff.score.isRendered = 0;
            new_winoff.debug.isRendered = 0;
            new_winoff.ginfo.isRendered = 0;
        }
        // Not wide enough to display any game information
        else 
        {
            new_winoff.draw_width = 0;

            new_winoff.pfield.isRendered = 0;
            new_winoff.title.isRendered = 0;
            new_winoff.pprev.isRendered = 0;
            new_winoff.score.isRendered = 0;
            new_winoff.debug.isRendered = 0;
            new_winoff.ginfo.isRendered = 0;
        }
    }
    // Not tall enough to display any game information
    else 
    {
        new_winoff.draw_height = 0;
        new_winoff.draw_width = 0;
        
        new_winoff.pfield.isRendered = 0;
        new_winoff.title.isRendered = 0;
        new_winoff.pprev.isRendered = 0;
        new_winoff.score.isRendered = 0;
        new_winoff.debug.isRendered = 0;
        new_winoff.ginfo.isRendered = 0;
    }

    return new_winoff;
}

int tdraw_keybinds()
{
    WINDOW* winkeybinds;

    // Menu options
    #define KEYMENULEN 8
    tdraw_menuitem menu[KEYMENULEN] = {
        {.name = "Leftshift",                   .desc = "aA \'KEY_LEFT\'",  .loffset = 1},
        {.name = "Rightshift",                  .desc = "aD \'KEY_RIGHT\'", .loffset = 1},
        {.name = "Clockwise rotate",            .desc = "wWeE \'KEY_UP\'",  .loffset = 1},
        {.name = "Counter-Clockwise rotate",    .desc = "zZqQ",             .loffset = 1},
        {.name = "Soft Drop",                   .desc = "sS \'KEY_DOWN\'",  .loffset = 1},
        {.name = "Hard Drop",                   .desc = "\'SPACE\'",        .loffset = 1},
        {.name = "Pause",                       .desc = "qQ \'ESC\'",       .loffset = 1},
        {.name = "EXIT",                        .desc = "EXIT keybinds",    .loffset = 1}
    };

    // Window height and width constants
    const int winheight = KEYMENULEN + 6;
    const int winwidth = 27;

    // Center window if there is enough space in the console 
    if (win_offsets.draw_height >= winheight && win_offsets.draw_width >= winwidth)
    {
        winkeybinds = newwin(winheight, winwidth, (win_offsets.draw_height - winheight)/2, (win_offsets.draw_width - winwidth)/2);
        if (!winkeybinds) {
            return ERROR_NCURSES_WINDOW;
        }

        for (int i = 0; i < KEYMENULEN; i++)
        {
            menu[i].loffset = (winwidth - strlen(menu[i].name))/2;
        }

        // Draw keybinds header
        mvwaddstr(winkeybinds, 1, (winwidth - 10) / 2, "KEYBINDS:");
    }
    // Place window at top left of screen if not enough space
    else
    {
        winkeybinds = newwin(win_offsets.draw_height, win_offsets.draw_width, 0, 0);
        if (!winkeybinds) {
            return ERROR_NCURSES_WINDOW;
        }

        // Draw keybinds header
        mvwaddstr(winkeybinds, 1, 1, "KEYBINDS:");
    }
    // Draw border on window, refresh it.
    box(winkeybinds, 0, 0); 
    refresh();

    // ---  Key press handle loop --- //
    char doloop = 1;
    int key;
    int highlight = 0;
    while (doloop)
    {
        // Draw menu and highlight current selected option
        for (int i = 0; i < KEYMENULEN + 1; i++)
        {
            if (KEYMENULEN == i)
            {
                wmove(winkeybinds, 4 + KEYMENULEN, 1);
                wclrtoeol(winkeybinds);
                mvwaddstr(winkeybinds, 4 + KEYMENULEN, (winwidth - strlen(menu[highlight].desc))/2, menu[highlight].desc);
                continue;
            }
            else if (highlight == i) 
            {
                wattron(winkeybinds, A_STANDOUT);
            }

            mvwaddstr(winkeybinds, 3 + i, menu[i].loffset, menu[i].name);
            wattroff(winkeybinds, A_STANDOUT);
        }
        box(winkeybinds, 0, 0);
        wrefresh(winkeybinds);

        // Handle keypress
        key = getch();
        switch (key)
        {
        case ERR:
            break;

        case 'd':
        case 'D':
        case KEY_RIGHT:
        case 's':
        case 'S':
        case KEY_DOWN:
            highlight = (highlight+1) % KEYMENULEN;
            break;

        case 'a':
        case 'A':
        case KEY_LEFT:
        case 'w':
        case 'W':
        case KEY_UP:
            highlight = (highlight-1) % KEYMENULEN;
            highlight = (highlight<0) ? highlight+KEYMENULEN : highlight;
            break;

        case '\n':
        case KEY_ENTER:
            switch (highlight)
            {
            case KEYMENULEN-1:
                doloop = 0;
                break;
            
            default:
                break;
            }
            break;

        default:
            break;
        }

        // Reduce resource usage
        usleep(1000);
    }

    // Clean up window and exit
    werase(winkeybinds);
    wrefresh(winkeybinds);
    delwin(winkeybinds);
    tdraw_touchwin();

    return SUCCESS;
}


// --- Public Functions --- //

int tdraw_start(tetris_game_t* game)
{
    WINDOW* winstart;

    // Menu options
    #define SMENULEN 3
    tdraw_menuitem menu[SMENULEN] = {
        {.name = "KEYBINDS",    .loffset = 1},
        {.name = "START",       .loffset = 1},
        {.name = "QUIT",        .loffset = 1}
    };

    if (!game) {
        return ERROR_NULL_INARG;
    }

    // Window height and width constants
    const int winheight = SMENULEN + 5;
    const int winwidth = 20;
    
    // Center window if there is enough space in the console 
    if (win_offsets.draw_height >= winheight && win_offsets.draw_width >= winwidth)
    {
        winstart = newwin(winheight, winwidth, (win_offsets.draw_height - winheight)/2, (win_offsets.draw_width - winwidth)/2);
        if (!winstart) {
            return ERROR_NCURSES_WINDOW;
        }

        for (int i = 0; i < SMENULEN; i++)
        {
            menu[i].loffset = (winwidth - strlen(menu[i].name))/2;
        }
    }
    // Place window at top left of screen if not enough space
    else
    {
        winstart = newwin(win_offsets.draw_height, win_offsets.draw_width, 0, 0);
        if (!winstart) {
            return ERROR_NCURSES_WINDOW;
        }
    }

    // Draw header, border. Refresh window.
    mvwaddstr(winstart, 1, 1, "TETRIS START MENU");
    mvwaddstr(winstart, 2, 1, "(ENTER to select)");
    box(winstart, 0, 0);
    refresh();

    // ---  Key press handle loop --- //
    char doloop = 1;
    int key;
    int highlight = 0;
    while (doloop)
    {
        // Draw menu and highlight current selected option
        for (int i = 0; i < SMENULEN; i++)
        {
            if (highlight == i) 
            {
                wattron(winstart, A_STANDOUT);
            }

            mvwaddstr(winstart, 4 + i, menu[i].loffset, menu[i].name);
            wattroff(winstart, A_STANDOUT);
        }
        wrefresh(winstart);

        // Get keypress
        key = getch();

        // Generate entropy from keypress
        if (key != ERR)
        {
            tetris_rand_entropy(game, key);
        }

        // Handle keypress
        switch (key)
        {
        case ERR:
            break;

        case 'd':
        case 'D':
        case KEY_RIGHT:
        case 's':
        case 'S':
        case KEY_DOWN:
            highlight = (highlight+1) % SMENULEN;
            break;

        case 'a':
        case 'A':
        case KEY_LEFT:
        case 'w':
        case 'W':
        case KEY_UP:
            highlight = (highlight-1) % SMENULEN;
            highlight = (highlight<0) ? highlight+SMENULEN : highlight;
            break;

        case '\n':
        case KEY_ENTER:
            switch (highlight)
            {
            case 0:
                tdraw_keybinds();
                touchwin(winstart);
                break;
            case 1:
                doloop = 0;
                break;
            case 2: 
                endwin();
                exit(0);
            default:
                break;
            }
            break;

        default:
            break;
        }

        usleep(1000);

        tetris_tick(game, 1000);
    }

    // Start the tetris game
    tetris_start(game);

    // Clean up window and exit
    werase(winstart);
    wrefresh(winstart);
    delwin(winstart);
    tdraw_touchwin();

    return SUCCESS;
}

int tdraw_pause(tetris_game_t* game)
{
    WINDOW* winpause;

    // Menu options
    #define PMENULEN 3
    tdraw_menuitem menu[PMENULEN] = {
        {.name = "KEYBINDS",    .loffset = 1},
        {.name = "RESUME",      .loffset = 1},
        {.name = "QUIT",        .loffset = 1}
    };

    if (!game) {
        return ERROR_NULL_INARG;
    }

    // Pause the tetris game
    tetris_pause(game);

    // Window height and width constants
    const int winheight = PMENULEN + 5;
    const int winwidth = 20;

    // Center window if there is enough space in the console 
    if (win_offsets.draw_height >= winheight && win_offsets.draw_width >= winwidth)
    {
        winpause = newwin(winheight, winwidth, (win_offsets.draw_height - winheight)/2, (win_offsets.draw_width - winwidth)/2);

        for (int i = 0; i < PMENULEN; i++)
        {
            menu[i].loffset = (winwidth - strlen(menu[i].name))/2;
        }
    }
    // Place window at top left of screen if not enough space
    else
    {
        winpause = newwin(win_offsets.draw_height, win_offsets.draw_width, 0, 0);
    }

    // Draw header and window border. Refresh window.
    mvwaddstr(winpause, 1, 1, "TETRIS PAUSE MENU");
    mvwaddstr(winpause, 2, 1, "(ENTER to select)");
    box(winpause, 0, 0);
    refresh();

    // ---  Key press handle loop --- //
    char doloop = 1;
    int key;
    int highlight = 0;
    while (doloop)
    {
        // Draw menu and highlight current selected option
        for (int i = 0; i < PMENULEN; i++)
        {
            if (highlight == i) 
            {
                wattron(winpause, A_STANDOUT);
            }

            mvwaddstr(winpause, 4 + i, menu[i].loffset, menu[i].name);
            wattroff(winpause, A_STANDOUT);
        }
        wrefresh(winpause);

        // Get keypress
        key = getch();

        // Generate entropy from keypress
        if (key != ERR)
        {
            tetris_rand_entropy(game, key);
        }

        // Handle keypress
        switch (key)
        {
        case ERR:
            break;

        case 'd':
        case 'D':
        case KEY_RIGHT:
        case 's':
        case 'S':
        case KEY_DOWN:
            highlight = (highlight+1) % PMENULEN;
            break;

        case 'a':
        case 'A':
        case KEY_LEFT:
        case 'w':
        case 'W':
        case KEY_UP:
            highlight = (highlight-1) % PMENULEN;
            highlight = (highlight<0) ? highlight+PMENULEN : highlight;
            break;

        case '\n':
        case KEY_ENTER:
            switch (highlight)
            {
            case 0:
                tdraw_keybinds();
                touchwin(winpause);
                break;
            case 1:
                doloop = 0;
                break;
            case 2:
                endwin();
                exit(0);
            default:
                break;
            }
            break;

        default:
            break;
        }

        usleep(1000);
    }

    // Clean up window
    werase(winpause);
    wrefresh(winpause);
    delwin(winpause);
    tdraw_touchwin();

    // Unpause game after deleting pause window
    tetris_unpause(game);

    return SUCCESS;
}

int tdraw_gameover(tetris_game_t* game)
{
    WINDOW* wingover;

    // Menu options
    #define GMENULEN 3
    tdraw_menuitem menu[GMENULEN] = {
        {.name = "KEYBINDS",    .loffset = 1},
        {.name = "PLAY AGAIN",  .loffset = 1},
        {.name = "QUIT",        .loffset = 1}
    };

    if (!game) {
        return ERROR_NULL_INARG;
    }

    // Window height and width constants
    const int winheight = GMENULEN + 6;
    const int winwidth = 20;

    // Center window if there is enough space in the console 
    char strbuff[18];
    int sprintret;
    if (win_offsets.draw_height >= winheight && win_offsets.draw_width >= winwidth)
    {
        // Create window
        wingover = newwin(winheight, winwidth, (win_offsets.draw_height - winheight)/2, (win_offsets.draw_width - winwidth)/2);

        for (int i = 0; i < GMENULEN; i++)
        {
            menu[i].loffset = (winwidth - strlen(menu[i].name))/2;
        }

        // Draw game over header
        mvwaddstr(wingover, 1, (winwidth - 12)/2, "GAME OVER!!");

        // Draw score info
        sprintret = snprintf(strbuff, 18, "Score: %ld", game->score);
        mvwaddstr(wingover, 2, (winwidth - sprintret)/2, strbuff);
        sprintret = snprintf(strbuff, 18, "Level: %d", game->level);
        mvwaddstr(wingover, 3, (winwidth - sprintret)/2, strbuff);
    }
    // Place window at top left of screen if not enough space
    else
    {
        // Create window
        wingover = newwin(win_offsets.draw_height, win_offsets.draw_width, 0, 0);

        // Draw game over header
        mvwaddstr(wingover, 1, 1, "GAME OVER!!");

        // Draw score info
        snprintf(strbuff, 18, "Score: %ld", game->score);
        mvwaddstr(wingover, 2, 1, strbuff);
        snprintf(strbuff, 18, "Level: %d", game->level);
        mvwaddstr(wingover, 3, 1, strbuff);
    }
    box(wingover, 0, 0);
    refresh();

    // ---  Key press handle loop --- //
    char doloop = 1;
    int key;
    int highlight = 0;
    while (doloop)
    {
        // Draw menu and highlight current selected option
        for (int i = 0; i < GMENULEN; i++)
        {
            if (highlight == i) 
            {
                wattron(wingover, A_STANDOUT);
            }

            mvwaddstr(wingover, 5 + i, menu[i].loffset, menu[i].name);
            wattroff(wingover, A_STANDOUT);
        }

        wrefresh(wingover);

        // Get keypress
        key = getch();

        // Generate entropy from keypress
        if (key != ERR)
        {
            tetris_rand_entropy(game, key);
        }

        // Handle keypress
        switch (key)
        {
        case ERR:
            break;

        case 'd':
        case 'D':
        case KEY_RIGHT:
        case 's':
        case 'S':
        case KEY_DOWN:
            highlight = (highlight+1) % GMENULEN;
            break;

        case 'a':
        case 'A':
        case KEY_LEFT:
        case 'w':
        case 'W':
        case KEY_UP:
            highlight = (highlight-1) % GMENULEN;
            highlight = (highlight<0) ? highlight+GMENULEN : highlight;
            break;

        case '\n':
        case KEY_ENTER:
            switch (highlight)
            {
            case 0:
                tdraw_keybinds();
                touchwin(wingover);
                break;
            case 1:
                doloop = 0;
                break;
            case 2:
                endwin();
                exit(0);
            default:
                break;
            }
            break;

        default:
            break;
        }

        usleep(1000);
    }

    // Clean up the window
    werase(wingover);
    wrefresh(wingover);
    delwin(wingover);
    tdraw_touchwin();

    // Restart the game
    tetris_reset(game);
    tetris_start(game);

    return SUCCESS;
}

int tdraw_pfield(tetris_game_t* game)
{
    char fdrawn;    // true if falling tetromino was drawn at current position
    int fidx = 0;   // Last drawn falling tetromino index. Allows taking advantage of sorted fpos array. 

    // Input arg check
    if (!game) {
        return ERROR_NULL_INARG;
    }
    if (!winpfield) {
        return ERROR_NULL_GVAR;
    }

    // Draw tetris playfield array
    for (int y = TETRIS_HEIGHT-1; y >= 0; y--)
    {
        // Move cursor to row position
        wmove(winpfield, TETRIS_HEIGHT - y, 1);
        for (int x = 0; x < TETRIS_WIDTH; x++)
        {
            if (game->board->pf[y][x] == TETRIS_BLANK)
            {
                fdrawn = 0;

                // Check if falling tetromino is at this coord.
                for (int i = fidx; i < 4; i++)
                {
                    if (game->board->fpos[i].h == y && game->board->fpos[i].w == x) {
                        tdraw_block(winpfield, game->board->fcol);
                        fidx++;
                        fdrawn = 1;
                        break;
                    }
                }

                // Draw blank pixel if falling tetromino isn't here
                if (!fdrawn) {
                    tdraw_block(winpfield, TETRIS_BLANK);
                }
            }
            // Draw colored pixel here
            else {
                tdraw_block(winpfield, game->board->pf[y][x]);
            }
        }
    }

    // Reset cursor and refresh window
    wmove(winpfield, 1, 1);
    box(winpfield, 0, 0);
    wrefresh(winpfield);

    return SUCCESS;
}

int tdraw_pprev(tetris_game_t* game)
{
    // Input arg check
    if (!game) {
        return ERROR_NULL_INARG;
    }
    if (!winpprev) {
        return ERROR_NULL_GVAR;
    }

    const tetris_coord_t PPOFFSET = (tetris_coord_t){
        .h = (TETRIS_HEIGHT),
        .w = ((TETRIS_WIDTH/2)) - 2
    };

    char isOddWidth;            // True of tetromino has an odd width
    tetris_coord_t tmp_coord; 

    // Clear piece preview window
    werase(winpprev);

    // Add separator bar to window
    for (int i = 1; i < 5; i++)
    {
        mvwaddch(winpprev, i, 9, '|');
    }

    // Loop through pieces in piece preview 
    for (int ppi = 0; ppi < TETRIS_PP_SIZE; ppi++)
    {
        if (game->ppreview[ppi] == TETRIS_BLANK) {
            continue;
        }

        if (game->ppreview[ppi] > 2) {
            isOddWidth = 1;
        }
        else {
            isOddWidth = 0;
        }

        // Loop through blocks for a piece in piece preview
        for (int ppj = 0; ppj < 4; ppj++)
        {
            tmp_coord = tetris_subCoord(TETRIS_TETROMINO_START[game->ppreview[ppi]][ppj], PPOFFSET);
            tmp_coord.h = 3 - tmp_coord.h;

            wmove(winpprev, tmp_coord.h + 1, tmp_coord.w*2 + ppi * 9 + isOddWidth + 1);
            tdraw_block(winpprev, game->ppreview[ppi]);
        }
    }

    // Draw border and refresh window
    box(winpprev, 0, 0);
    wrefresh(winpprev);

    return SUCCESS;
}

int tdraw_score(tetris_game_t* game)
{
    // Input arg check
    if (!game) {
        return ERROR_NULL_INARG;
    }
    if (!winscore) {
        return ERROR_NULL_GVAR;
    }

    // Draw score and level info
    mvwprintw(winscore, 1, 1, "Score: %ld", game->score);
    wclrtoeol(winscore);
    mvwprintw(winscore, 2, 1, "Level: %d", game->level);
    wclrtoeol(winscore);

    // Draw border and refresh window
    box(winscore, 0, 0);
    wrefresh(winscore);

    return SUCCESS;
}

int tdraw_ginfo(tetris_game_t* game)
{
    // Input arg check
    if (!game) {
        return ERROR_NULL_INARG;
    }
    if (!win_offsets.ginfo.isRendered) {
        return ERROR_NULL_GVAR;
    }

    // Erase window and redraw border
    werase(winginfo);
    box(winginfo, 0, 0);

    // --- Print Debug Information --- //

    mvwprintw(winginfo, 1, 1, "tetris_game: state");
    mvwprintw(winginfo, 2, 3, "Started: %s", game->isStarted ? "true" : "false");
    mvwprintw(winginfo, 3, 3, "Running: %s", game->isRunning ? "true" : "false");
    mvwprintw(winginfo, 4, 3, "Gameover: %s", game->isGameover ? "true" : "false");

    mvwprintw(winginfo, 5, 1, "tetris_game: score");
    mvwprintw(winginfo, 6, 3, "combo: %d", game->combo);
    mvwprintw(winginfo, 7, 3, "lines: %d", game->lines);

    mvwprintw(winginfo, 8, 1, "tetris_game: tetromino queue");
    mvwprintw(winginfo, 9, 3, "queue idx: %d", game->qidx);
    mvwprintw(winginfo, 10, 3, "pqueue: ");
    for (int i = 0; i < 7; i++)
    {
        tdraw_block(winginfo, game->queue[i]);
    }
    mvwprintw(winginfo, 11, 3, "squeue: ");
    for (int i = 0; i < 7; i++)
    {
        tdraw_block(winginfo, game->shuffle_queue[i]);
    }
    mvwprintw(winginfo, 12, 3, "randx: %d", game->randx);

    mvwprintw(winginfo, 13, 1, "tetris_game: time");
    mvwprintw(winginfo, 14, 3, "tmicro: %ld", game->tmicro);
    mvwprintw(winginfo, 15, 3, "tdrop: %ld", game->tdrop);

    mvwprintw(winginfo, 17, 1, "tetris_board:");
    mvwprintw(winginfo, 18, 3, "falling color: ");
    tdraw_block(winginfo, game->board->fcol);
    mvwprintw(winginfo, 19, 3, "falling rotation: %d", game->board->frot);
    mvwprintw(winginfo, 20, 3, "gc_valid: %s", game->board->gc_valid ? "true" : "false");

    wrefresh(winginfo);

    return SUCCESS;
}

int tdraw_initcolor()
{
    if (has_colors())
    {
        // Start ncurses color
        start_color();

        // Define largest posible numbers for custom colors
        int TCOLOR_CYAN = COLORS-7;
        int TCOLOR_YELLOW = COLORS-6;
        int TCOLOR_BLUE = COLORS-5;
        int TCOLOR_ORANGE = COLORS-4;
        int TCOLOR_GREEN = COLORS-3;
        int TCOLOR_PURPLE = COLORS-2;
        int TCOLOR_RED = COLORS-1;

        // Define RGB values for custom colors
        init_color(TCOLOR_CYAN, 0, 900, 900);
        init_color(TCOLOR_YELLOW, 900, 900, 0);
        init_color(TCOLOR_BLUE, 100, 100, 900);
        init_color(TCOLOR_ORANGE, 900, 500, 0);
        init_color(TCOLOR_GREEN, 0, 900, 0);
        init_color(TCOLOR_PURPLE, 500, 0, 900);
        init_color(TCOLOR_RED, 900, 0, 0);
        
        // Define color pairs used for drawing tetromino pixels
        init_pair(TETRIS_CYAN+8, TCOLOR_CYAN, TCOLOR_CYAN);
        init_pair(TETRIS_YELLOW+8, TCOLOR_YELLOW, TCOLOR_YELLOW);
        init_pair(TETRIS_BLUE+8, TCOLOR_BLUE, TCOLOR_BLUE);
        init_pair(TETRIS_ORANGE+8, TCOLOR_ORANGE, TCOLOR_ORANGE);
        init_pair(TETRIS_GREEN+8, TCOLOR_GREEN, TCOLOR_GREEN);
        init_pair(TETRIS_PURPLE+8, TCOLOR_PURPLE, TCOLOR_PURPLE);
        init_pair(TETRIS_RED+8, TCOLOR_RED, TCOLOR_RED);
        init_pair(TETRIS_BLANK+8, COLOR_BLACK, COLOR_BLACK);

        // Smile because we have color!
        if (win_offsets.debug.isRendered)
        {
            wprintw(debug_window, "Color :)\n");
            wprintw(debug_window, "COLOR_PAIRS: %d, COLORS: %d\n", COLOR_PAIRS, COLORS);
        }
    }
    else 
    {
        // Cry because we dont have color :(
        if (win_offsets.debug.isRendered)
        {
            wprintw(debug_window, "No color :(\n");
        }
        return ERROR_NCURSES_NOCOLOR;
    }

    return SUCCESS;
}

int tdraw_wininit()
{
    // Calculate offsets
    win_offsets = tdraw_calc_offsets();


    // --- Allocate windows and apply offsets --- //

    if (win_offsets.title.isRendered)
    {
        wintitle = newwin(6, 42, win_offsets.title.offset.y, win_offsets.title.offset.x);
        box(wintitle, 0, 0);
        wrefresh(wintitle);
    }

    if (win_offsets.pfield.isRendered)
    {
        winpfield = newwin(TETRIS_HEIGHT + 2, TETRIS_WIDTH*2 + 2, win_offsets.pfield.offset.y, win_offsets.pfield.offset.x);
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
        winscore = newwin(4, 19, win_offsets.score.offset.y, win_offsets.score.offset.x);
        box(winscore, 0, 0);
        wrefresh(winscore);
    }

    if (win_offsets.debug.isRendered)
    {
        windebug = newwin(12, 19, win_offsets.debug.offset.y, win_offsets.debug.offset.x);
        box(windebug, 0, 0);
        wrefresh(windebug);

        // Create subwindow inside of the box so the border isnt overwritten
        debug_window = newwin(10, 17, getbegy(windebug)+1, getbegx(windebug)+1);
        scrollok(debug_window, true);
        wrefresh(debug_window);
        }

    if (win_offsets.ginfo.isRendered)
    {
        winginfo = newwin(22, 31, win_offsets.ginfo.offset.y, win_offsets.ginfo.offset.x);
        box(winginfo, 0, 0);
        wrefresh(winginfo);
    }

    return SUCCESS;
}

int tdraw_winupdate()
{
    tdraw_winoff_t new_winoff;

    // Calculate window offsets. 
    new_winoff = tdraw_calc_offsets();


    //  --- Apply window offsets. Allocate and delete windows as needed --- //

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
            winpfield = newwin(TETRIS_HEIGHT + 2, TETRIS_WIDTH*2 + 2, new_winoff.pfield.offset.y, new_winoff.pfield.offset.x);
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
            winscore = newwin(4, 19, new_winoff.score.offset.y, new_winoff.score.offset.x);
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
            windebug = newwin(12, 19, new_winoff.debug.offset.y, new_winoff.debug.offset.x);
            box(windebug, 0, 0);
            wrefresh(windebug);

            // Create subwindow inside of the box so the box isnt overwritten
            debug_window = newwin(10, 17, getbegy(windebug)+1, getbegx(windebug)+1);
            scrollok(debug_window, true);
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

    if (win_offsets.ginfo.isRendered != new_winoff.ginfo.isRendered)
    {
        // Not enough room for score window, delete window
        if (win_offsets.ginfo.isRendered && !new_winoff.ginfo.isRendered)
        {
            delwin(winginfo);
            winginfo = NULL;
        }

        // There is room to create score window
        else if (!win_offsets.ginfo.isRendered && new_winoff.ginfo.isRendered) {
            winginfo = newwin(22, 31, new_winoff.ginfo.offset.y, new_winoff.ginfo.offset.x);
            box(winginfo, 0, 0);
            wrefresh(winginfo);
        }

        // Score window needs to be moved
        else {
            mvwin(winginfo, new_winoff.ginfo.offset.y, new_winoff.ginfo.offset.x);
            wrefresh(winginfo);
        }
    }

    win_offsets = new_winoff;

    refresh();

    return SUCCESS;
}


int tdraw_touchwin()
{
    // Touch and refresh all windows
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
    if (winginfo != NULL) {
        touchwin(winginfo);
        wrefresh(winginfo);
    }

    return SUCCESS;
}