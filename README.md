# BTetris

## What is BTetris??

BTetris is a backend library for Tetris. 
This means that the library will implement all of the logic of the game, leaving only the handling of input, output and timing left for implementation. 
The vision for this project is to make it easy to implement Tetris on a given computer system, whether its for a fully fledged desktop application interacting with a user via a GUI and keyboard, or an embedded system interacting with a user via GPIO pins. 

Since this library can be potentially be used in a wide array of devices, implementation of the game logic will avoid standard functions that can potentially invoke system calls. 
Actions such as allocating game structures will be up to the frontend application's implementation. 

> **Note**: The code in this repository is currently untested