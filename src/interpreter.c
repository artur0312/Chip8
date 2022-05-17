#include "memory.h"
#include "interpreter.h"

//fetch instruction and increment the program counter
unsigned short fetch_instruction(Memory *memory){
  unsigned int address=memory.RAM+memory.PC;
  unsigned short result=0x100*(*address)+*(memory+1);
  memory.PC+=2;
  return result;
}

//


