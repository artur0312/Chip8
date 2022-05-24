#include "memory.h"
#include "interpreter.h"
#include "screen.h"

//Get the unsigned short at the address
unsigned short fetch_ushort(Memory* memory,int offset){
  char* address=memory->RAM+offset;
  //Using the fact that chip 8 is big endian
  unsigned char first_byte=(*address);
  unsigned char second_byte=*(address+1);
  unsigned short value=0x100*(first_byte)+second_byte;
  return value;
}

//Set the unsigned short at the address
void set_ushort(Memory* memory,int offset,unsigned short value){
  char* address=memory->RAM+offset;
  char high_byte=(value)&(0xFF>>8);
  char low_byte=(value)&(0xFF);
  *address=high_byte;
  *(address+1)=low_byte;
}

//fetch instruction and increment the program counter
Instruction fetch_instruction(Memory *memory){
  unsigned short value=fetch_ushort(memory,memory->PC);
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

  //Return from a subroutine
  if(instr.instruction==0x00EE){
    //Fetch the value at the PC
    unsigned short address=fetch_ushort(memory, memory->SP);
    memory->PC=address;
    memory->SP+=2;
  }

  //Jump
  else if(instr.instruction_type==1){
    memory->PC=instr.address;
  }

  //Call a function
  else if(instr.instruction_type==2){
    memory->SP-=2;
    set_ushort(memory,memory->SP,memory->PC);
    memory->PC=instr.address;
  }

  //Jump if register equals to number
  else if(instr.instruction_type==3){
    int register1=instr.register_1;
    if(memory->registers[register1]==instr.number){
      memory->PC+=2;
    }
  }

  //Jump if register different from
  else if(instr.instruction_type==4){
    int register1=instr.register_1;
    if(memory->registers[register1]!=instr.number){
      memory->PC+=2;
    }
  }

  //Jump if the value in both registers are equal
  else if(instr.instruction_type==5 && instr.last_nibble==0){
    int register1=instr.register_1;
    int register2=instr.register_2;
    if(memory->registers[register1]==memory->registers[register2]){
      memory->PC+=2;
    }
  }

  //Set register
  else if(instr.instruction_type==6){
    memory->registers[instr.register_1]=instr.number;
  }

  //Add to register without carry flag
  else if(instr.instruction_type==7){
    memory->registers[instr.register_1]+=instr.number;
  }

  //Logic and arithmetic
  else if(instr.instruction_type==8){
    //Set the value of VX to the value of VY
    if(instr.last_nibble==0){
      memory->registers[instr.register_1]=memory->registers[instr.register_2];
    }
    //Binary OR
    else if(instr.last_nibble==1){
      unsigned char register1=memory->registers[instr.register_1];
      unsigned char register2=memory->registers[instr.register_2];
      unsigned char value=register1 | register2;
      memory->registers[instr.register_1]=value;
    }
    //Binary AND
    else if(instr.last_nibble==2){
      unsigned char register1=memory->registers[instr.register_1];
      unsigned char register2=memory->registers[instr.register_2];
      unsigned char value=register1 & register2;
      memory->registers[instr.register_1]=value;
    }
    //Binary XOR
    else if(instr.last_nibble==3){
      unsigned char register1=memory->registers[instr.register_1];
      unsigned char register2=memory->registers[instr.register_2];
      unsigned char value=register1 ^ register2;
      memory->registers[instr.register_1]=value;
    }

    //Sum registers with the carry flag
    else if(instr.last_nibble==4){
      unsigned char register1=memory->registers[instr.register_1];
      unsigned char register2=memory->registers[instr.register_2];
      short result=register1+register2;
      memory->registers[instr.register_1]=result%256;
      memory->registers[15]= (result<256)? 0 : 1;
    }

    //Subtract VX-VY
    else if(instr.last_nibble==5){
      unsigned char register1=memory->registers[instr.register_1];
      unsigned char register2=memory->registers[instr.register_2];
      short result=register1-register2;
      memory->registers[instr.register_1]=result;
      memory->registers[15]= (register1>register2) ? 0 : 1;
    }

    //Subtract VX-VY
    else if(instr.last_nibble==5){
      unsigned char register1=memory->registers[instr.register_1];
      unsigned char register2=memory->registers[instr.register_2];
      short result=register1-register2;
      memory->registers[instr.register_1]=result;
      memory->registers[15]= (register1>register2) ? 0 : 1;
    }

  }

  //Right shift operation to the original COSMAC VIP
  else if(instr.last_nibble==6){
      unsigned char register2=memory->registers[instr.register_2];
      unsigned char result=register2>>1;
      memory->registers[instr.register_1]=result;
      memory->registers[15]= register2&1; 
  }

  //Subtract VX-VY
    else if(instr.last_nibble==7){
      unsigned char register1=memory->registers[instr.register_1];
      unsigned char register2=memory->registers[instr.register_2];
      short result=register2-register1;
      memory->registers[instr.register_1]=result;
      memory->registers[15]= (register1<register2) ? 0 : 1;
    }

  //Right shift operation to the original COSMAC VIP
  else if(instr.last_nibble==0xE){
      unsigned char register2=memory->registers[instr.register_2];
      unsigned char result=register2<<1;
      memory->registers[instr.register_1]=result;
      memory->registers[15]= register2&8; 
  }

  //Jump if the value in both registers are equal
  else if(instr.instruction_type==9 && instr.last_nibble==0){
    int register1=instr.register_1;
    int register2=instr.register_2;
    if(memory->registers[register1]!=memory->registers[register2]){
      memory->PC+=2;
    }
  }
  //Set register I
  else if(instr.instruction_type==0xA){
    memory->I=instr.address;
  }

  //Jump with offset aaccording to the COSMAC VIP
  else if(instr.instruction_type==0xB){
    memory->PC=memory->registers[0]+instr.number;
  }

  //Random
  else if(instr.instruction_type==0xC){
    unsigned char result=rand()%256;
    unsigned char mask=get_nibbles(instr.instruction,3,4);
    memory->registers[instr.register_1]=result&mask;
  }

  //Draw sprite
  else if(instr.instruction_type==0xD){
    //Set VF to 0
    memory->registers[15]=0;
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
