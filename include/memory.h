#ifndef MEMORY_H
#define MEMORY_H
//Represents the memory of the chip8
typedef struct Memory{
  //The 4kb of ram memory
  char* RAM=(char*) malloc(4096);

  //The program counter
  unsigned int PC;

  //The I register, used to point to location on the memory
  unsigned int I;

  //The stack pointer
  unsigned int SP;

  //Registers VA through VF
  unsigned char registers[16];
}Memory;

void initialise_memory(Memory* memory);
#endif
