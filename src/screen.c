#include<stdio.h>
#include<SDL2/SDL.h>

#include "screen.h"

void closeWindow(Window window){
  //Destroy window
  SDL_DestroyRenderer(window.gRenderer);
  SDL_DestroyWindow(window.window);
  window.window=NULL;
  window.gRenderer=NULL;

  //Quit SDL
  //IMG_Quit();
  SDL_Quit();
 
}

//Set the pixels of the screen to 0
void resetPixel(Window *window){
  for(int i=0;i<PIXEL_HEIGHT;i++){
    for(int j=0;j<PIXEL_WIDTH;j++){
      window->pixelColor[i][j]=0;
    }
  }
}

//Create the rectangles
void createRectangles(Window *window){
  for(int i=0;i<PIXEL_HEIGHT;i++){
    for(int j=0;j<PIXEL_WIDTH;j++){
      int x_coord=j*window->width/PIXEL_WIDTH;
      int y_coord=i*window->height/PIXEL_HEIGHT;
      int x_size=window->width/PIXEL_WIDTH;
      int y_size=window->height/PIXEL_HEIGHT;
      SDL_Rect temp={x_coord,y_coord,x_size,y_size};
      window->pixelCoordinate[i][j]=temp;
    }
  }
}

//Draw the pixels
void drawPixel(Window window){
  for(int i=0;i<PIXEL_HEIGHT;i++){
    for(int j=0;j<PIXEL_WIDTH;j++){
      if(window.pixelColor[i][j]==0){
        SDL_SetRenderDrawColor(window.gRenderer,0x00, 0x00, 0x00,0xFF);
        }
      else{
        SDL_SetRenderDrawColor(window.gRenderer,0xFF, 0xFF, 0xFF,0xFF);
      }
      SDL_RenderFillRect(window.gRenderer, &window.pixelCoordinate[i][j]);
    }
  }
}

void updatePixel(int line,int column, Window* window, int value){
  window->pixelColor[line][column]=value;
}



Window createScreen(){
  Window screen={.width=1280,.height=640,.window=NULL,.gRenderer=NULL};

  //Initialise the pixels to be all black
  resetPixel(&screen);
  createRectangles(&screen);


  //Initialize SDL
  if(SDL_Init(SDL_INIT_VIDEO)<0){
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else{
    //Set texture filtering to linear
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")){
      printf("Warning: Linear texture filtering not enabled");
    }


    //Create window
    screen.window=SDL_CreateWindow("Chip 8", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, screen.width,
                            screen.height, SDL_WINDOW_SHOWN);
    if(screen.window==NULL){
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else{
      //Create renderer for window
      screen.gRenderer=SDL_CreateRenderer(screen.window, -1, SDL_RENDERER_ACCELERATED);
      if(screen.gRenderer==NULL){
        printf("Renderer could not be created! SDL Error: %s\n",SDL_GetError());
      }
      else{
        //Initial rendering color
        SDL_SetRenderDrawColor(screen.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      }
    }
  }

  return screen;
}

