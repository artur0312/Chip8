#include <stdlib.h>
#include "memory.h"
#include <stdbool.h>
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
  //And the keys are not pressed
  for(int i=0;i<16;i++){
    memory->registers[i]=0;
    memory->keys_pressed[16]=false;
  }

  memory->DT=0;
  memory->ST=0;

  //Set the font
  unsigned char font[16][5]={
    {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
    {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
    {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
    {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
    {0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
    {0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
    {0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
    {0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
    {0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
    {0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
    {0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
    {0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
    {0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
    {0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
    {0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
    {0xF0, 0x80, 0xF0, 0x80, 0x80}  // F
  };

  //Put the font in the memory starting at 0x50
  for(int i=0;i<16;i++){
    for(int j=0;j<5;j++){
      memory->RAM[0x50+5*i+j]=font[i][j];
    }
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
