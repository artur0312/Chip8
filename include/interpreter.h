#ifndef INTERPRETER_H
#define  INTERPRETER_H
#include "memory.h"
#include "screen.h"

//Fetch instruction and increment the program counter
Instruction fetch_instruction(Memory *memory);

//Decode a instruction and execute it
void decode_instruction(Instruction instr, Window* window, Memory* memory);

//Decode the instruction
#endif
