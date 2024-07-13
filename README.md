# Console Graphics Engine
Allows the user to render a screen buffer onto the console window.
## Structure
- "include" folder contains the engine
- "demos" folder showcases the capabilities of the engine

## Features of "consoleGraphics.h"
- ``` int getColor(int hexValue) ``` converts hex color into a format understood by the console.
- ``` int createConsole(int width, int height, int fontw, int fonth) ``` initializes the console class.
- ``` void pixel(int x, int y) ``` puts a pixel onto buffer using values from ``` drawChar ``` variable.
- ``` void character(int x, int y, char c) ``` writes a character in the specified position. (this is for writing text)
- ``` float getFPS() ``` returns the frames displayed per second.
- ``` void clear(int val) ``` fills the buffer with a specified int value.
- ``` int render() ``` displays the contents of the screen buffer.
- ``` void setColor(int hexValue) ``` sets ``` drawChar ``` variable accordingly to the specified hex value.
- ``` BOOL key(int value) ``` returns if the specified key was hit on the keyboard.

## Usage
(Well demonstrated by the demos)
1. ``` #include ".../include/consoleGraphics.h" ```
2. Create graphicConsole object.
``` graphicConsole gc;
    gc.createConsole(SW,SH,PIXEL_W, PIXEL_H);
    //SW = stage width
    //SH = stage height
    //PIXEL_W = width for a single pixel
    //PIXEL_H = height for a single pixel
```
3. Make main loop
```
while(1){
    gc.render();
}
```
4. Use pixel function in main loop to draw on the screen.
eg.:
```
gc.setColor(0x000000);
while(1){
    gc.pixel(SW/2,SH/2); //Draw pixel in the middle
    gc.render();
}
```