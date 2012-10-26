/**
* Author: Devon Guinane
*/

#include "Chip8.h"
#include <string>
#include<iostream>

using std::string;
using std::cout;
using std::endl;

//8-bits
typedef unsigned char BYTE;

Chip8::Chip8()
{
    this->init();
    
}

void Chip8::init()
{
    //reset Program Counter
    this->PC = PC_START;
    
    //clear RAM
    for(int i = 0; i < RAM_SIZE; i++)
    {
        this->ram[i] = 0;
    }
    
    //clear registers
    for(int i = 0; i < NUM_REGISTERS; i++)
    {
        this->V[i] = 0;
    }
    
    //clear stack
    for(int i = 0; i < STACK_SIZE; i++)
    {
        this->stack[i] = 0;
    }
    
    //clear I register
    this->I = 0;
    
    //reset stack pointer
    this->SP = 0;
    
}

void Chip8::cycle()
{
    //fetch opcode
    //need to get next 2 instructions from memory since each instruction is only 1 byte in ram. We need 2 bytes
    unsigned short opcode = (this->ram[this->PC] << 8) | this->ram[this->PC + 1];
    
    //decode
    this->decode(opcode);
    
    //execute
}

void Chip8::decode(unsigned short opcode)
{
    //mask to get first(leftmost) 4 bits
    unsigned short OP_MASK = 0xF000;
    
    //mask to get second 4 bits
    unsigned short SECOND_MASK = 0x0F00;
    
    //mask to get 3rd set of 4 bits
    unsigned short THIRD_MASK = 0x00F0;
    
    //mask to get last(rightmost) 8 bits
    unsigned short LAST_HALF_MASK = 0x00FF;
    
    //mask to get last(rightmost) 12 bits
    unsigned short DATA_MASK = 0x0FFF;
    
    switch(opcode & OP_MASK)
    {
        case 0x0000:
            switch(opcode & 0x000F)
                //00E0 - CLS
                case 0x0000:
                break;
                
                //00EE - RET
                case 0x000E:
                break;     
        break;
        
        //1nnn - JP addr
        case 0x1000:
            this->JP(opcode & DATA_MASK);
        break;
        
        //2nnn - CALL addr
        case 0x2000:
            this->CALL(opcode & DATA_MASK);
        break;
        
        //3xkk - SE3 Vx, byte
        case 0x3000:
            this->SE3(opcode & SECOND_MASK, opcode & LAST_HALF_MASK);
        break;
        
        //4xkk - SNE Vx, byte
        case 0x4000:
        	this->SNE(opcode & SECOND_MASK, opcode & LAST_HALF_MASK);
        break;
        
        //5xy0 - SE Vx, Vy
        case 0x5000:
        	this->SE5(opcode & SECOND_MASK, opcode & THIRD_MASK);
        break;
    }
}

/**
* 1nnn - JP addr
* Jump to location nnn.
* The interpreter sets the program counter to nnn.
*/
void Chip8::JP(unsigned short address)
{
    this->PC = address;
}

/**
* 2nnn - CALL addr
* Call subroutine at nnn
* The interpreter increments the stack pointer, then puts the current PC on 
* the top of the stack. The PC is then set to nnn.
*/
void Chip8::CALL(unsigned short nnn)
{
    /**store current address of program counter on stack to remember where
    * to jump back to after calling subroutine 
    */
    this->stack[this->SP] = this->PC;

    //increase stack pointer
    ++this->SP;
    
    //jump to subroutine address
    this->PC = nnn;
}

/**
* 3xkk - SE Vx, byte
* Skip next instruction if Vx = kk.
* The interpreter compares register Vx to kk, and if they are equal, 
* increments the program counter by 2
*/
void Chip8::SE3(unsigned short x, unsigned short kk)
{
    if(this->V[x] == kk)
        this->PC += 4;
    else
        this->PC += 2;
}

/**
* 4xkk - SNE Vx, byte
* Skip next instruction if Vx != kk.
* The interpreter compares register Vx to kk, and if they are not equal,
* increments the program counter by 2.
*/
void Chip8::SNE(unsigned short x, unsigned short kk)
{
    if(this->V[x] != kk)
        this->PC += 4;
    else
        this->PC += 2;
}

/**
* 5xy0 - SE Vx, Vy
* Skip next instruction if Vx = Vy.
* The interpreter compares register Vx to register Vy, and if they are equal,
* increments the program counter by 2.
*/
void Chip8::SE5(unsigned short x, unsigned short y)
{
    if(this->V[x] == this->V[y])
        this->PC += 4;
    else
        this->PC += 2;
}

void Chip8::dump()
{
    cout << "PC:" << this->PC << endl;
    cout << "I:" << this->I << endl;
    cout << "SP:" << this->SP << endl;
    
    for(int i = 0; i < NUM_REGISTERS; i++)
    {
        cout << "V" << i << ":" << this->V[i] << endl;
    }
}

Chip8::~Chip8()
{

}
