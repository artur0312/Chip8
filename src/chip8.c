#include<stdbool.h>
#include<SDL2/SDL.h>
#include <stdio.h>


#include "screen.h"
#include "memory.h"
#include "interpreter.h"

int main(int argc, char*argv[]){
  if(argc!=2){
    printf("Usage chip8 <file name>");
    exit(1);
  }

  //Initialise the memory
  Memory memory;
  initialise_memory(&memory);

  //Read the file
  FILE* program;
  program=fopen(argv[1],"r");

  if(program==NULL){
    printf("Error: file not found");
    exit(1);
  }

  //Load it in the ram
  fseek(program,0,SEEK_END);
  int filelen=ftell(program);
  rewind(program);
  fread(memory.RAM+memory.PC, filelen, 1, program);
  fclose(program);

  //Sanity check: print the bytes on the file
  /*printf("Size: %d\n", filelen);
    for(int i=0;i<filelen-1;i++){
    unsigned char c=memory.RAM[memory.PC+i];
    printf("%X: %02X\n", memory.PC,c);
    }*/
   
  //Initialise the Screen
  Window window=createScreen();

  bool quit=false;

  SDL_Event e;


  //Main loop
  while(!quit){
    //Handle events on queue
    while(SDL_PollEvent(&e)!=0){
      if(e.type==SDL_QUIT){
        quit=true;
      }
    }

    //Fetch the next instruction
    Instruction instr=fetch_instruction(&memory);

    //Run the instruction
    decode_instruction(instr, &window, &memory);
    

    //Clear screen
    SDL_SetRenderDrawColor(window.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(window.gRenderer);

    //Draw to the screen
    drawPixel(window);

      
    //Update screen
    SDL_RenderPresent(window.gRenderer);
    SDL_Delay(200);
  }

  //Quit
  closeWindow(window);
  clear_memory(&memory);
}
