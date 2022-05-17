#include "memory.h"
void initialise_memory(Memory* memory){
  //Set the size of the RAM
  memory->RAM=malloc(4096);

  //The first address to run is 0x200
  memory->PC=0x200;

  //I register start at 0
  memory->I=0;

  //The stack pointer starts at one past the
  //end of the memory
  memory->SP=4097;

  //All the registers start as 0
  for(int i=0;i<16;i++){
    memory->registers[i]=0;
  }


}
