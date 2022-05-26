#include<stdbool.h>
#include<SDL2/SDL.h>
#include <stdio.h>
#include<time.h>


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

  //Holds the value of the last time the screen was upddated
  clock_t start_frame=clock();


  //Main loop
  while(!quit){
    //Handle events on queue
    while(SDL_PollEvent(&e)!=0){
      if(e.type==SDL_QUIT){
        quit=true;
      }
      //TODO: clear this code, maybe using a dictionary
      //Get keys pressed
      else if(e.type==SDL_KEYDOWN){
        switch(e.key.keysym.sym){
        case SDLK_1:
          memory.keys_pressed[0]=true;
          break;
        case SDLK_2:
          memory.keys_pressed[1]=true;
          break;
        case SDLK_3:
          memory.keys_pressed[2]=true;
          break;
        case SDLK_4:
          memory.keys_pressed[3]=true;
          break;
        case SDLK_q:
          memory.keys_pressed[4]=true;
          break;
        case SDLK_w:
          memory.keys_pressed[5]=true;
          break;
        case SDLK_e:
          memory.keys_pressed[6]=true;
          break;
        case SDLK_r:
          memory.keys_pressed[7]=true;
          break;
        case SDLK_a:
          memory.keys_pressed[8]=true;
          break;
        case SDLK_s:
          memory.keys_pressed[9]=true;
          break;
        case SDLK_d:
          memory.keys_pressed[10]=true;
          break;
        case SDLK_f:
          memory.keys_pressed[11]=true;
          break;
        case SDLK_z:
          memory.keys_pressed[12]=true;
          break;
        case SDLK_x:
          memory.keys_pressed[13]=true;
          break;
        case SDLK_c:
          memory.keys_pressed[14]=true;
          break;
        case SDLK_v:
          memory.keys_pressed[15]=true;
          break;
        }
      }
      //Release the keys
      else if(e.type==SDL_KEYUP){
        switch(e.key.keysym.sym){
        case SDLK_1:
          memory.keys_pressed[0]=false;
          break;
        case SDLK_2:
          memory.keys_pressed[1]=false;
          break;
        case SDLK_3:
          memory.keys_pressed[2]=false;
          break;
        case SDLK_4:
          memory.keys_pressed[3]=false;
          break;
        case SDLK_q:
          memory.keys_pressed[4]=false;
          break;
        case SDLK_w:
          memory.keys_pressed[5]=false;
          break;
        case SDLK_e:
          memory.keys_pressed[6]=false;
          break;
        case SDLK_r:
          memory.keys_pressed[7]=false;
          break;
        case SDLK_a:
          memory.keys_pressed[8]=false;
          break;
        case SDLK_s:
          memory.keys_pressed[9]=false;
          break;
        case SDLK_d:
          memory.keys_pressed[10]=false;
          break;
        case SDLK_f:
          memory.keys_pressed[11]=false;
          break;
        case SDLK_z:
          memory.keys_pressed[12]=false;
          break;
        case SDLK_x:
          memory.keys_pressed[13]=false;
          break;
        case SDLK_c:
          memory.keys_pressed[14]=false;
          break;
        case SDLK_v:
          memory.keys_pressed[15]=false;
          break;
        }
      } 
 
    }

    //Fetch the next instruction
    Instruction instr=fetch_instruction(&memory);

    //Run the instruction
    decode_instruction(instr, &window, &memory);
    
    //Time functionality
    double t=((double)(clock()-start_frame))/CLOCKS_PER_SEC;
    //Update with a frequency of 60Hz
    if(t>1.0/60){
      //Clear screen
      SDL_SetRenderDrawColor(window.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderClear(window.gRenderer);

      //Draw to the screen
      drawPixel(window);

      //Change the screen
      SDL_RenderPresent(window.gRenderer);

      //Update the DT and ST
      if(memory.DT>0){
        memory.DT--;
      }
      if(memory.ST>0){
        memory.ST--;
      }
    }
  }

  //Quit
  closeWindow(window);
  clear_memory(&memory);
}
