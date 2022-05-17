#ifndef SCREEN_H
#define SCREEN_H
#include<SDL2/SDL.h>

#define PIXEL_HEIGHT 32
#define PIXEL_WIDTH 64
//The structure for a screen
typedef struct Window{

  //The width of the window
  int width;
  //The height of the window
  int height;

  //The window we will be rendering to
  SDL_Window* window;

  //The surface contained by the window
  SDL_Renderer* gRenderer;

  //The array indicating the color of the pixels
  unsigned char pixelColor[PIXEL_HEIGHT][PIXEL_WIDTH];
  SDL_Rect pixelCoordinate[PIXEL_HEIGHT][PIXEL_WIDTH];

} Window;

//Initialise and destructor
Window createScreen();
void closeWindow(Window window);

//Draw pixels
void drawPixel(Window window);
void updatePixel(int line, int column, Window* window, int value);
#endif
