_______________________________________________________
Demo app:
 - [ ] Do proper error handling in tdraw.c
 - [ ] Make sure demo app works with different options


_______________________________________________________
Create new file to store and handle gamestate information
 - [x] Stores started or paused state here
 - [x] Stores score, level and high score
 - [x] Shuffle function to shuffle the tetrominos in the shuffle queue. 
 - [x] Don't rotate tetromino if tetromino is an 'o'
 - [x] Rename error enum names to make it more clear that its for an error. ex: TETRIS_COLLISION -> TETRIS_ERROR_COLLISION
 - [x] Rename betris to something else
 - [x] Tick function: handle timing of falling tetrominoes
 - [x] Tick function: Handle clearing rows
 - [x] Tick function: handle pulling tetrominoes from queue when previous tetormino is locked
 - [x] Tick function: Handle row clear and combo scoring 
 - [x] Tick function: handle leveling
 - [x] Tick function: Handle game over
 - [x] Use game state variables to add more variation to rng algorithm
 - [x] Init function to initialize tetris_game structure
 - [x] Reset function to reset the game to a playable state after a previous game. 
 - [x] Start function
 - [x] Pause and unpase function
 - [x] Return error when game is over
 - [x] Function documentation 
 - [x] Consider atomicity issues with shuffling tetrominoes and swaping 
 - [x] Option documentation
 - [x] Look through code and clean up. 
 - [ ] Create a demo app for library


___________________________________________________
Later Features
 - [ ] Add option to reset function to set randx variable
 - [ ] Better RNG
 - [ ] Super Rotation System 
 - [ ] T-spin scoring
 - [ ] Holding
 - [ ] Lock down
 - [ ] Option for shuffle queue size other than 7
 - [ ] Non constant tick length feature
 - [ ] Custom random functions
 - [ ] update display event option 
 - [ ] Perfect clear scoring
 - [ ] Option to start game at a level other than 1
 - [ ] Control queueing so falling tetromino is only moved in the tick() thread