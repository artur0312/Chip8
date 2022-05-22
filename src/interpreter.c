#include "memory.h"
#include "interpreter.h"
#include "screen.h"

//fetch instruction and increment the program counter
Instruction fetch_instruction(Memory *memory){
  char* address=memory->RAM+memory->PC;
  //Using the fact that chip 8 is big endian
  unsigned char first_byte=(*address);
  unsigned char second_byte=*(address+1);
  unsigned short value=0x100*(first_byte)+second_byte;
  memory->PC+=2;
  Instruction instruction;
  initialise_instruction(&instruction,value);
  return instruction;
}

void decode_instruction(Instruction instr, Window* window, Memory* memory){
  //CLS
  if(instr.instruction==0x00E0){
    resetPixel(window);
  }
  //Jump
  else if(instr.instruction_type==1){
    memory->PC=instr.address;
  }

  //Set register
  else if(instr.instruction_type==6){
    memory->registers[instr.register_1]=instr.number;
  }

  //Add to register
  else if(instr.instruction_type==7){
    memory->registers[instr.register_1]+=instr.number;
  }

  //Set register I
  else if(instr.instruction_type==0xA){
    memory->I=instr.address;
  }

  //Draw sprite
  else if(instr.instruction_type==0xD){
    int x_coordinate=memory->registers[instr.register_1]%64;
    int y_coordinate=memory->registers[instr.register_2]%32;
    int n=get_nibbles(instr.instruction,4,4);
    for(int i=0;i<n;i++){
      //Reset x_coordinate
      x_coordinate=memory->registers[instr.register_1]%64;
      //Fetch the sprite
      char sprite=memory->RAM[memory->I+i];
      unsigned char select_bit=1<<7;
      for(int j=0;j<8;j++){
        char mask=(sprite&select_bit)>>(7-j);
        char pixel=window->pixelColor[y_coordinate][x_coordinate] ^ mask;
        //Set the register VF
        if(pixel==0 && window->pixelColor[y_coordinate][x_coordinate]==1){
          memory->registers[15]=1;
        }
        window->pixelColor[y_coordinate][x_coordinate]=pixel;

        //Prepare the next iteration
        select_bit=select_bit>>1;
        x_coordinate++;
        if(x_coordinate==64){
          break;
        }
      }

      //Change the y coordinate
      y_coordinate++;
      if(y_coordinate==32){
        break;
      }
    }
  }
}
