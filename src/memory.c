#include <stdlib.h>
#include "memory.h"
#include<stdio.h>

void initialise_memory(Memory* memory){
  //Set the size of the RAM
  memory->RAM=(char*)malloc(4096);

  //The first address to run is 0x200
  memory->PC=0x200;

  //I register start at 0
  memory->I=0;

  //The stack pointer starts at one past the
  //end of the memory
  memory->SP=4097;

  //All the registers start as 0
  for(int i=0;i<17;i++){
    memory->registers[i]=0;
  }
}

unsigned short get_nibbles(unsigned short x, int start, int end){
  unsigned short initial_mask=0xF000>>(4*(start-1));
  unsigned short mask=0;
  for(int i=start;i<=end; i++){
    mask=mask|initial_mask;
    initial_mask=initial_mask>>4;
  }
  unsigned short result=x & mask;
  return result>>(4*(4-end));
}

void initialise_instruction(Instruction* instruction,
                            unsigned short value){
  instruction->instruction=value;
  instruction->instruction_type=get_nibbles(value,1,1);
  instruction->last_nibble=get_nibbles(value,4,4);
  instruction->register_1=get_nibbles(value,2,2);
  instruction->register_2=get_nibbles(value,3,3);

  instruction->number=get_nibbles(value,3,4);
  instruction->address=get_nibbles(value,2,4);
}

//Free the ram
void clear_memory(Memory* memory){
  free(memory->RAM);
}
