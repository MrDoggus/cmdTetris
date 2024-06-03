Betris_control changes
 - [x] Return error if control operation cannot be completed 
 - [ ] Create function to find the location of the current tetromino if it were to be hard dropped. 
 - [ ] When locking a tetromino, determine lines that would be cleared and communicate that info somehow
 - [ ] rename references to falling tetromino to active tetromino

-----
Create new file to store and handle gamestate information
 - [ ] Stores started or paused state here
 - [ ] Stores score, level and high score
 - [ ] Store function pointers for cunstomization. RNG algorithm for example
 - [ ] glues game logic from other files together
