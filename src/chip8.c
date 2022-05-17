#include<stdbool.h>
#include<SDL2/SDL.h>


#include "screen.h"

int main(){
  Window window=createScreen();

  bool quit=false;

  SDL_Event e;

  int counter=0;
  while(!quit){
    //Handle events on queue
    while(SDL_PollEvent(&e)!=0){
      if(e.type==SDL_QUIT){
        quit=true;
      }
    }

    //Clear screen
    SDL_SetRenderDrawColor(window.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(window.gRenderer);

    drawPixel(window);

    //Update screen
    SDL_RenderPresent(window.gRenderer);
    SDL_Delay(1000);

    //Update the pixel
    int line=counter/PIXEL_WIDTH;
    int column=counter%PIXEL_WIDTH;
    if(line<PIXEL_HEIGHT){
      updatePixel(line,column,&window,1);
    }
    counter++;
  }

  //Quit
  closeWindow(window);
}
