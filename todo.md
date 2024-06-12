_______________________________________________________
Create new file to store and handle gamestate information
 - [x] Stores started or paused state here
 - [x] Stores score, level and high score
 - [x] Shuffle function to shuffle the tetrominos in the shuffle queue. 
 - [x] Don't rotate tetromino if tetromino is an 'o'
 - [x] Rename error enum names to make it more clear that its for an error. ex: TETRIS_COLLISION -> TETRIS_ERROR_COLLISION
 - [x] Rename betris to something else
 - [x] Tick function: handle timing of falling tetrominoes
 - [ ] Tick function: Handle clearing rows and corresponding scoring 
 - [ ] Tick function: handle pulling tetrominoes from queue when previous tetormino is locked
 - [ ] Tick function: Handle combo scoring 
 - [ ] Use game state variables to add more variation to rng algorithm
 - [ ] Init function to initialize tetris_game structure
 - [ ] Reset function to reset the game to a playable state after a previous game. 
 - [ ] Function documentation 

____________________________________________________
Features
 - [x] ghostCoords caching to avoid recalculations of the ghost position
 - [ ] disable piece preview/next queue option 
 - [ ] disable ghost piece cache option 
 - [ ] disable null board check option

___________________________________________________
Later Features
 - [ ] update display event option 
 - [ ] Super Rotation System 
 - [ ] T-spin scoring
 - [ ] Lock down
 - [ ] Holding
 - [ ] Option for shuffle queue size other than 7
 - [ ] Non constant tick length feature
 - [ ] Custom random functions
 - [ ] Perfect clear scoring
