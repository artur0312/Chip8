#ifndef MEMORY_H
#define MEMORY_H
#include<stdbool.h>

//Represents the memory of the chip8
typedef struct Memory{
  //The 4kb of ram memory
  char* RAM;


  //The program counter
  unsigned int PC;

  //The I register, used to point to location on the memory
  unsigned int I;

  //The stack pointer
  unsigned int SP;

  //Registers VA through VF
  unsigned char registers[16];

  //The state of key presses
  //The keys used are 1-4, q-r, a-f, z-v
  //TODO: make this keys configurable
  bool keys_pressed[16];

  //The delay timer
  unsigned char DT;

  //The sound timer
  unsigned char ST;
  
}Memory;

typedef struct Instruction{
  //The entire instruction
  unsigned short instruction;
  
  //The first nibble
  unsigned short instruction_type;

  //The second nibble, used to reference registers
  unsigned short register_1;

  //The second nibble, used to reference registers
  unsigned short register_2;

  //The third and fourth nibbles, used to represent a 8-bit number
  unsigned short number;

  //The 3 last nibbles, used to reference a memory address
  unsigned short address;

  //The last nibble, used by some operations
  unsigned short last_nibble;
  
}Instruction;

//Get the nibbles from start to end. Each nibble is a number from 0 to 15
//The first nibble is the leftmost one.
unsigned short get_nibbles(unsigned short x, int start,int end);

void initialise_instruction(Instruction* instruction,
                            unsigned short value);
void initialise_memory(Memory* memory);

//Clear the RAM malloc
void clear_memory(Memory* memory);

#endif
