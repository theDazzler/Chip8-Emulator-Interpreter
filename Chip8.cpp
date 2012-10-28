/**
* Author: Devon Guinane
*/

#include "Chip8.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

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
	//ex. 1234 - 1=>first nibble, 2=>second nibble, etc.
    //mask to get first(leftmost) NIBBLE(4 bits)
    const unsigned short FIRST_NIBBLE_MASK = 0xF000;
    
    //mask to get second NIBBLE
    const unsigned short SECOND_NIBBLE_MASK = 0x0F00;
    
    //mask to get third NIBBLE
    const unsigned short THIRD_NIBBLE_MASK = 0x00F0;
    
    //mask to get fourth NIBBLE
    const unsigned short FOURTH_NIBBLE_MASK = 0x000F;
    
    //mask to get second BYTE
    const unsigned short SECOND_BYTE_MASK = 0x00FF;
    
    //mask to get last(rightmost) 12 bits
    const unsigned short DATA_MASK = 0x0FFF;
    
    switch(opcode & FIRST_NIBBLE_MASK)
    {
        case 0x0000:
            switch(opcode & FOURTH_NIBBLE_MASK)
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
            this->SE3(opcode & SECOND_NIBBLE_MASK, opcode & SECOND_BYTE_MASK);
        break;
        
        //4xkk - SNE Vx, byte
        case 0x4000:
        	this->SNE4(opcode & SECOND_NIBBLE_MASK, opcode & SECOND_BYTE_MASK);
        break;
        
        //5xy0 - SE Vx, Vy
        case 0x5000:
        	this->SE5(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        break;
        
        //6xkk - LD Vx, byte
        case 0x6000:
        	this->LD6(opcode & SECOND_NIBBLE_MASK, opcode & SECOND_BYTE_MASK);
        break;
        
        //7xkk - ADD Vx, byte
        case 0x7000:
        	this->ADD7(opcode & SECOND_NIBBLE_MASK, opcode & SECOND_BYTE_MASK);        	
        break;
        
        
        case 0x8000:
        	switch(opcode & FOURTH_NIBBLE_MASK)
        	{
        		//8xy0 - LD Vx, Vy
        		case 0x0000:
        			this->LD8(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        		break;
        		
        		//8xy1 - OR Vx, Vy
        		case 0x0001:
        			this->OR8(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        		break;
        		
        		//8xy2 - AND Vx, Vy
        		case 0x0002:
        			this->AND8(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        		break;
        		
        		//8xy3 - XOR Vx, Vy
        		case 0x0003:
        			this->XOR8(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        		break;
        		
        		//8xy4 - ADD Vx, Vy
        		case 0x0004:
        			this->ADD8(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        		break;
        		
        		//8xy5 - SUB Vx, Vy
        		case 0x0005:
        			this->SUB8(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        		break;
        		
        		//8xy6 - SHR Vx, {, Vy}
        		case 0x0006:
        			this->SHR8(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        		break;
        		
        		//8xy7 - SUBN Vx, Vy
        		case 0x0007:
        			this->SUBN(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        		break;
        		
        		//8xyE - SHL Vx {, Vy}
        		case 0x000E:
        			this->SHL(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        		break;       	
        		
        	}
        break;
        
        //9xy0 - SNE Vx, Vy
        case 0x9000:
        	this->SNE9(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK);
        break;
        
        //Annn - LD I, addr
        case 0xA000:
        	this->LDA(opcode & DATA_MASK);
        break;
        
        //Bnnn - JP V0, addr
        case 0xB000:
        	this->JPB(opcode & DATA_MASK);
        break;
        
        //Cxkk - RND Vx, byte
        case 0xC000:
        	this->RND(opcode & SECOND_NIBBLE_MASK, opcode & SECOND_BYTE_MASK);
        break;
        
        //Dxyn - DRW Vx, Vy, nibble
        case 0xD000:
        	this->DRW(opcode & SECOND_NIBBLE_MASK, opcode & THIRD_NIBBLE_MASK, opcode & FOURTH_NIBBLE_MASK);
        break;
        
        case 0xE000:
        	switch(opcode & FOURTH_NIBBLE_MASK)
        	{
        		//Ex9E - SKP Vx
        		case 0x000E:
        			this->SKP(opcode & SECOND_NIBBLE_MASK);
        		break;
        		
        		//ExA1 - SKNP Vx
        		case 0x0001:
        			this->SKNP(opcode & SECOND_NIBBLE_MASK);
        		break;
        	}
        break;
        
        case 0xF000:
        	switch(opcode & SECOND_BYTE_MASK)
        	{
        		//Fx07 - LD Vx, DT
        		case 0x0007:
        			this->LDF07(opcode & SECOND_NIBBLE_MASK);
        		break;
        		
        		//Fx0A - LD Vx, K
        		case 0x000A:
        			this->LDF0A(opcode & SECOND_NIBBLE_MASK);
        		break;
        		
        		//Fx15 - LD DT, Vx
        		case 0x0015:
        			this->LDF15(opcode & SECOND_NIBBLE_MASK);
        		break;
        		
        		//Fx18 - LD ST, Vx
        		case 0x0018:
        			this->LDF18(opcode & SECOND_NIBBLE_MASK);
        		break;
        		
        		//Fx1E - ADD I, Vx
        		case 0x001E:
        			this->LDF1E(opcode & SECOND_NIBBLE_MASK);
        		break;
        		
        		//Fx29 - LD F, Vx
        		case 0x0029:
        		break;
        		
        		//Fx33 - LD B, Vx
        		case 0x0033:
        		break;
        		
        		//Fx55 - LD [I], Vx
        		case 0x0055:
        		break;

				//Fx65 - LD Vx, [I]
				case 0x0065:
				break;
        	}
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
void Chip8::SNE4(unsigned short x, unsigned short kk)
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

/**
* 6xkk - LD Vx, byte
* Set Vx = kk.
* The interpreter puts the value kk into register Vx.
*/
void Chip8::LD6(unsigned short x, unsigned short kk)
{
    this->V[x] = kk;
    this->PC += 2;
}

/**
* 7xkk - ADD Vx, byte
* Set Vx = Vx + kk.
* Adds the value kk to the value of register Vx, then stores the result in Vx. 
*/
void Chip8::ADD7(unsigned short x, unsigned short kk)
{
    this->V[x] += kk;
    this->PC += 2;
}

/**
* 8xy0 - LD Vx, Vy
* Set Vx = Vy.
* Stores the value of register Vy in register Vx.. 
*/
void Chip8::LD8(unsigned short x, unsigned short y)
{
    this->V[x] = this->V[y];
    this->PC += 2;
}

/**
* 8xy1 - OR Vx, Vy
* Set Vx = Vx OR Vy.
* Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. 
*/
void Chip8::OR8(unsigned short x, unsigned short y)
{
    this->V[x] |= this->V[y];
    this->PC += 2;
}

/**
* 8xy2 - AND Vx, Vy
* Set Vx = Vx AND Vy.
* Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
*/
void Chip8::AND8(unsigned short x, unsigned short y)
{
    this->V[x] &= this->V[y];
    this->PC += 2;
}

/**
* 8xy3 - XOR Vx, Vy
* Vx = Vx XOR Vy.
* Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx
*/
void Chip8::XOR8(unsigned short x, unsigned short y)
{
    this->V[x] ^= this->V[y];
    this->PC += 2;
}

/**
* 8xy4 - ADD Vx, Vy
* Set Vx = Vx + Vy, set VF = carry.
* The values of Vx and Vy are added together. If the result is greater 
* than 8 bits (i.e., > 255,) VF * is set to 1, otherwise 0. Only the lowest
* 8 bits of the result are kept, and stored in Vx.
*/
void Chip8::ADD8(unsigned short x, unsigned short y)
{
	unsigned short sum = this->V[x] + this->V[y];
    if(sum > 255)
    	this->V[F] = 1;
    else
    	this->V[F] = 0;
    this->V[x] = sum & 0x00FF; //only store lowest 8 bits
    this->PC += 2;
}

/**
* 8xy5 - SUB Vx, Vy
* Set Vx = Vx - Vy, set VF = NOT borrow.
* If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from
* Vx, and the results stored in Vx.
*/
void Chip8::SUB8(unsigned short x, unsigned short y)
{
	if(this->V[x] > this->V[y])
		this->V[0x0F] = 1;
	else
		this->V[0x0F] = 0;
		
	this->V[x] -= this->V[y];
    this->PC += 2;
}

/**
* 8xy6 - SHR Vx {, Vy}
* Set Vx = Vx SHR 1.
* If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. 
* Then Vx is divided by 2.
*/
void Chip8::SHR8(unsigned short x, unsigned short y)
{
	if(this->V[x] & 0x0001 == 1)
		this->V[F] = 1;
	else
		this->V[F] = 0;
	this->V[x] >>= 1;
	
    this->PC += 2;
}

/**
* 8xy7 - SUBN Vx, Vy
* Set Vx = Vy - Vx, set VF = NOT borrow.
* If Vy > Vx, then VF is set to 1, otherwise 0. 
* Then Vx is subtracted from Vy, and the results stored in Vx.
*/
void Chip8::SUBN(unsigned short x, unsigned short y)
{
	if(this->V[y] > this->V[x])
		this->V[F] = 1;
	else
		this->V[F] = 0;
	
	this->V[x] = this->V[y] - this->V[x];
	
    this->PC += 2;
}

/**
* 8xyE - SHL Vx {, Vy}
* Set Vx = Vx SHL 1.
* If the most-significant bit of Vx is 1, then VF is set to 1,
* otherwise to 0. Then Vx is multiplied by 2.
*/
void Chip8::SHL(unsigned short x, unsigned short y)
{
	if(this->V[x] & 0x8000 == 1)
		this->V[F] = 1;
	else
		this->V[F] = 0;
	
	this->V[x] <<= 1;
	
    this->PC += 2;
}

/**
* 9xy0 - SNE Vx, Vy
* Skip next instruction if Vx != Vy.
* The values of Vx and Vy are compared, and if they are not equal,
* the program counter is increased by 2
*/
void Chip8::SNE9(unsigned short x, unsigned short y)
{
	if(this->V[x] != this->V[y])
		this->PC += 4;
	else
		this->PC += 2;
}

/**
* Annn - LD I, addr
* Set I = nnn.
* The value of register I is set to nnn.
*/
void Chip8::LDA(unsigned short nnn)
{
	this->I = nnn;
	this->PC += 2;
}

/**
* Bnnn - JP V0, addr
* Jump to location nnn + V0.
* The program counter is set to nnn plus the value of V0.
*/
void Chip8::JPB(unsigned short nnn)
{
	this->stack[this->SP] = this->PC;
	++this->SP;
	
	this->PC = nnn + this->V[0];
}

/**
* Cxkk - RND Vx, byte
* Set Vx = random byte AND kk.
* The interpreter generates a random number from 0 to 255, which is then
* ANDed with the value kk. The results are stored in Vx.
*/
void Chip8::RND(unsigned short x, unsigned short kk)
{
	srand(time(0));
	int randByte = rand() % 255 + 1;
	this->V[x] = randByte & kk;
	
	this->PC += 2;
}

/**
* Dxyn - DRW Vx, Vy, nibble
* Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
* The interpreter reads n bytes from memory, starting at the address stored in I. 
* These bytes are then displayed as * sprites on screen at coordinates (Vx, Vy). Sprites are 
* XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, 
* otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates 
* of the display, it wraps around to the opposite side of the screen.
*/
void Chip8::DRW(unsigned short x, unsigned short y, unsigned short n)
{
	
}

/**
* Ex9E - SKP Vx
* Skip next instruction if key with the value of Vx is pressed.
* Checks the keyboard, and if the key corresponding to the value of Vx is
* currently in the down position, PC is increased by 2.
*/
void Chip8::SKP(unsigned short x)
{
	
}

/**
* ExA1 - SKNP Vx
* Skip next instruction if key with the value of Vx is not pressed.
* Checks the keyboard, and if the key corresponding to the value of
* Vx is currently in the up position, PC is increased by 2.
*/
void Chip8::SKNP(unsigned short x)
{
	
}

/**
* Fx07 - LD Vx, DT
* Set Vx = delay timer value.
* The value of DT is placed into Vx.
*/
void Chip8::LDF07(unsigned short x)
{
	this->V[x] = this->delayTimer;
	this->PC += 2;
}

/**
* Fx0A - LD Vx, K
* Wait for a key press, store the value of the key in Vx.
* All execution stops until a key is pressed, then the value of that key is stored in Vx.
*/
void Chip8::LDF0A(unsigned short x)
{
	
}

/**
* Fx15 - LD DT, Vx
* Set delay timer = Vx.
* DT is set equal to the value of Vx.
*/
void Chip8::LDF15(unsigned short x)
{
	this->delayTimer = this->V[x];
	this->PC += 2;
}

/**
* Fx18 - LD ST, Vx
* Set sound timer = Vx.
* ST is set equal to the value of Vx..
*/
void Chip8::LDF18(unsigned short x)
{
	this->soundTimer = this->V[x];
	this->PC += 2;
}

/**
* Fx1E - ADD I, Vx
* Set I = I + Vx.
* The values of I and Vx are added, and the results are stored in I.
*/
void Chip8::LDF1E(unsigned short x)
{
	this->I += this->V[x];
	this->PC += 2;
}

/**
* Fx29 - LD F, Vx
* Set I = location of sprite for digit Vx.
* The value of I is set to the location for the hexadecimal sprite 
* corresponding to the value of Vx.
*/
void Chip8::LDF29(unsigned short x)
{
	
}

/**
* Fx33 - LD B, Vx
* Store BCD representation of Vx in memory locations I, I+1, and I+2.
* The interpreter takes the decimal value of Vx, and places the hundreds 
* digit in memory at location in I, the tens digit at location I+1, and the
* ones digit at location I+2.
*/
void Chip8::LDF33(unsigned short x)
{
	
}

/**
* Fx55 - LD [I], Vx
* Store registers V0 through Vx in memory starting at location I.
* The interpreter copies the values of registers V0 through Vx into memory,
* starting at the address in I.
*/
void Chip8::LDF55(unsigned short x)
{
	for(int i = 0; i <= x; i++)
	{
		this->ram[this->I + i] = this->V[i];
	}
	this->PC += 2;
}

/**
* Fx65 - LD Vx, [I]
* Read registers V0 through Vx from memory starting at location I.
* The interpreter reads values from memory starting at location I into registers V0 through Vx.
*/
void Chip8::LDF65(unsigned short x)
{
	for(int i = 0; i <= x; i++)
	{
		this->V[i] = this->ram[this->I + i];
	}
	
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
