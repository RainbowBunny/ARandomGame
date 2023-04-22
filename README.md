# Game?
This is a game for Math Open Day. It was created by SDL2 in C++. \

The game may consume a lot of memory due to the fact that the image are all in high resolution.

The idea of the game is very simple, place the water in some cell to contain the fire, do not let the cats catch fire or let the fire spread too large. Despite the simplicity of the idea, the code turned out to be quite long, maybe there are many things that can be optimized (?)

## Playing the game

- To play the game, you need to download the file from [DLL file for SDL2](https://drive.google.com/drive/folders/1gFK0X09jtiQ-tbHdz3fUYf0CyvdnWhuq).
Put all file in the directory, and merge the lib directory (for window). The game might be run directly after installing the SDL2 library in Linux, I haven't test the game on Mac yet.
 - Run theses command to create .exe file.
```
>>> make create
>>> make all
```
- Run the exe file to play the game.
```
>>> main.exe
```
- To choose a cell, put the mouse on the cell and then click mouse button.

- To pause the game, use the ESC key.

- To modify the data of the game, change the txt file in data/ 

Have fun with the game! Feel free too have any comment!
