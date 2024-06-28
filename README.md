# BTetris

## What is BTetris??

BTetris is a backend library designed to implement the core logic of the Tetris game. 
By handling the game's logic, BTetris leaves user interaction, display, and timing for implementation. 
The vision for this project is to make it easy to implement Tetris on a given computer system, whether its a desktop application with a graphical user interface or an embedded system interacting with a user via GPIO pins. 

Since this library can be potentially be used in a wide array of devices, implementation of the game logic will avoid standard functions that invoke system calls. 
Actions such as allocating game structures will be up to the frontend application's implementation. Write atomicity to game structures isn't guaranteed and should be implemented by frontend if using threading or something similar.

> **Note**: The code in this repository is currently untested.


## Creating a Tetris game using BTetris

There are three steps to using BTetris: Display, User Interaction, and Game Runtime. 
Functions and structures related to game runtime can be found in [`btetris_game.h`](src/btetris_game.h).
Information relating to the Tetris playfield can be found in the `tetris_board_t` struct located in [`btetris_board.h`](src/btetris_board.h).
Additional information about the game state can be found in `tetris_game_t`, which can be found in [`btetris_game.h`](src/btetris_game.h).
Finally, user interaction with the game can be done through functions found in [`btetris_control.h`](src/btetris_control.h).

### Game Runtime

To run Tetris using this library, two game structs should be allocated. 
The first is `tetris_board_t`, located in [`btetris_board.h`](src/btetris_board.h) and the second is `tetris_game_t`, located in [`btetris_game.h`](src/btetris_game.h).
Once allocated, these structures should be initialized using `tetris_init()`. 
To start the game, calling `tetris_start()` allows `tetris_tick()` to manage the game. 
Use `tetris_pause()` and `tetris_unpause()` to stop and resume game management from `tetris_tick()` during a started game. 
After a played game is over, use `tetris_reset()` to put the game back to a playable state without clearing the RNG state. 

The `tetris_tick()` function is a critical part of the game exection. 
It handles tetromino bag shuffling, automatic dropping of tetrominoes, row clearing, leveling and scoring.
Then the game is over, `tetris_tick()` returns `TETRIS_ERROR_GAME_OVER` to communicate this. 
This function should be called at a fixed time interval. 
By default, this interval is 100 microseconds, but this can be changed by defining `TETRIS_TICK_PERIOD` to a value other than 100.
`TETRIS_TICK_PERIOD` is in units of microseconds and cannot be a value higher than 823.
The tick function should be called even if the game isn't running since it shuffles the tetromino bag. 

The random number generator used is similar to the Linear Congruential Generator algorithm.
The random number generator has a period of 7, so the sequence of generated number repeats every 7 numbers. 
For this reason, an external source of entropy for the RNG algorithm is practically necissary. 
Calling `tetris_rand_entropy()` occasionally with an externally generated number can be useful to make the shuffled items less predictable. 
A source of entropy is especially useful at the beginning of the game when the tetrominoes are not shuffled well. 

### Display

The Tetris playfield is stored as an array in `tetris_board_t->pf[height][width]`. 
The width is defined by `TETRIS_WIDTH` and height by `TETRIS_HEIGHT` + an additional 4 row buffer. 
The falling tetromino is not located in the playfield array and should be rendered on top of playfield. 
It is stored as an array of 4 coordinates that define its shape in `tetris_board_t->fpos[4]`. 
This tetromino's color is stored in `tetris_board_t->fcol`.

The `tetris_board_t` struct also stores the position of the ghost piece if it is calculated. 
The ghost piece shows the position of the falling tetromino if it were to be hard dropped. 
Similar  to `fpos`, it is also stored as an array of 4 coordinates, found in `tetris_board_t->gc_pos[4]`.
This information may not be accurate if the falling tetromino was moved or if the ghost piece position wasn't calculated already. 
To ensure it is accurate, call `tetris_calcGhostCoords()` before drawing the ghost piece. 

Other information outside of `tetris_board_t` may be useful to display. 
One example is the piece preview, which shows the list of incoming tetrominoes. 
It is stored as an array of colors in `tetris_game_t->ppreview[]` with a size of 2 by default. 
The shapes corresponding to the colors in the list can be found in the `TETRIS_TETROMINO_START` array.
Another example is the current score and level for the current game, found in `tetris_game_t->score` and `tetris_game_t->level` respectivelly. 

### User Interaction 

Most of the functions in [`btetris_control.h`](src/btetris_control.h) are useful for user input. 
These functions are described in the list below: 
 - `tetris_rotcw()`: Rotates the falling tetromino clockwise.
 - `tetris_rotcntrcw()`: Rotates the falling tetromino counter-clockwise.
 - `tetris_leftshift()`: Shifts the falling tetromino to the left.
 - `tetris_rightshift()`: Shifts the falling tetromino to the right.
 - `tetris_sdrop()`: Drops the falling tetromino by one position. 
 - `tetris_hdrop()`: Drops the falling tetromino as far as it can, then locks it in place. 


## Options

There are various defines created to allow small tweaks to the library. 
To configure these options, simply redefine the definition before the include headers or through compiler arguments.
These defines are described below: 

 - `TETRIS_WIDTH`: Width of the tetris playfield. 
   - Default := `10`
   - Range := `[4:127]`
 - `TETRIS_HEIGHT`: Height of the tetris playfield. 
   - Default := `20`
   - Range := `[4:123]`
 - `TETRIS_TICK_PERIOD`: Time period (microseconds) between `tetris_tick()` function calls.
   - Default := `100`
   - Range := `(inf, 823]`
 - `TETRIS_PP_SIZE`: Number of tetrominoes in the piece preview array
   - Default := `2`
   - Range := `[1, 6]`
 - `TETRIS_RAND_INCR`: Increment parameter for the random number generator. 
   - Default := `2`
   - Range := `[1, 6]`