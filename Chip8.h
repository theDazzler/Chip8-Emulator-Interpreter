/**
* Author: Devon Guinane
*/

#ifndef CHIP8_HH
#define CHIP8_HH

/**
Memory Map:
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
*/

class Chip8 
{
    typedef unsigned char BYTE;
    
    //4k of RAM(4096 bytes)
    static const int RAM_SIZE = 4096;
    
    //number of general purpose 8-bit registers
    static const int NUM_REGISTERS = 16;
    
    //stack size. 16 levels of nested subroutines
    static const int STACK_SIZE = 16;
    
    //Most Chip-8 programs start at location 0x200(512)
    static const int PC_START = 0x200;
    
    //more readable format for the carry flag. Instead of this->V[0x0F], we can do this->V[F]
    static const int F = 0x0F;
    
	public:
	    //4K of RAM (4096 bytes)
	    BYTE ram[RAM_SIZE];
	    
	    //16 8-bit general purpose registers referred to as Vx where x is a hex digit 0-F
	    BYTE V[NUM_REGISTERS];
	    
	    //16-bit register used to store memory addresses(only rightmost(lowest) 12 bits are used)
	    unsigned short I;
        
        //16-bit program counter
        unsigned short PC;
        
        //8-bit stack pointer
        BYTE SP;
        
        //stack is an array of 16 16-bit values. Allows for up to 16 levels of nested subroutines
        unsigned short stack[STACK_SIZE];
	    
	    /**
	    * 8-bit delay and sound timers
	    * When these 2 registers are non-zero, they are automatically decremented 
	    * at a rate of 60Hz
	    */
	    //8-bit register for delay timer
	    BYTE delayTimer;
	    
	    //8-bit register for sound timer
	    BYTE soundTimer;
	    	    
	    Chip8 ();
		virtual ~Chip8 ();
	    
	    //initialize CPU
	    void init();
	    
	    //emulate one cycle
	    void cycle();
	    
	    //decode an opcode
	    void decode(unsigned short opcode);
	    
	    /**
        * 1nnn - JP addr
        * Jump to location nnn.
        * The interpreter sets the program counter to nnn.
        */
	    void JP(unsigned short address);
	    
	    /**
        * 2nnn - CALL addr
        * Call subroutine at nnn
        * The interpreter increments the stack pointer, then puts the current PC on 
        * the top of the stack. The PC is then set to nnn.
        */
	    void CALL(unsigned short address);
	    
	    /**
        * 3xkk - SE Vx, byte
        * Skip next instruction if Vx = kk.
        * The interpreter compares register Vx to kk, and if they are equal, 
        * increments the program counter by 2.
        */
	    void SE3(unsigned short x, unsigned short kk);
	    
	    /**
		* 4xkk - SNE Vx, byte
		* Skip next instruction if Vx != kk.
		* The interpreter compares register Vx to kk, and if they are not equal,
		* increments the program counter by 2.
		*/
		void SNE4(unsigned short x, unsigned short kk);
		
		/**
		* 5xy0 - SE Vx, Vy
		* Skip next instruction if Vx = Vy.
		* The interpreter compares register Vx to register Vy, and if they are equal,
		* increments the program counter by 2.
		*/
		void SE5(unsigned short x, unsigned short y);
		
		/**
		* Set Vx = kk.
		* The interpreter puts the value kk into register Vx.
		*/
		void LD6(unsigned short x, unsigned short kk);
		
		/**
		* Set Vx = Vx + kk.
		* Adds the value kk to the value of register Vx, then stores the result in Vx. 
		*/
		void ADD7(unsigned short x, unsigned short kk);
		
		/**
		* 8xy0 - LD Vx, Vy
		* Set Vx = Vy.
		* Stores the value of register Vy in register Vx.. 
		*/
		void LD8(unsigned short x, unsigned short y);
		
		/**
		* Set Vx = Vx OR Vy.
		* Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. 
		*/
		void OR8(unsigned short x, unsigned short y);
		
		/**
		* Set Vx = Vx AND Vy.
		* Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
		*/
		void AND8(unsigned short x, unsigned short y);
		
		/**
		*  Vx = Vx XOR Vy.
		* Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx
		*/
		void XOR8(unsigned short x, unsigned short y);
		
		/**
		* Set Vx = Vx + Vy, set VF = carry.
		* The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF * is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
		*/
		void ADD8(unsigned short x, unsigned short y);
		
		/**
		* Set Vx = Vx - Vy, set VF = NOT borrow.
		* If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from
		* Vx, and the results stored in Vx.
		*/
		void SUB8(unsigned short x, unsigned short y);
		
		/**
		* 8xy6 - SHR Vx {, Vy}
		* Set Vx = Vx SHR 1.
		* If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. 
		* Then Vx is divided by 2.
		*/
		void SHR8(unsigned short x, unsigned short y);
		
		/**
		* 8xy7 - SUBN Vx, Vy
		* Set Vx = Vy - Vx, set VF = NOT borrow.
		* If Vy > Vx, then VF is set to 1, otherwise 0. 
		* Then Vx is subtracted from Vy, and the results stored in Vx.
		*/
		void SUBN(unsigned short x, unsigned short y);
		
		/**
		* 8xyE - SHL Vx {, Vy}
		* Set Vx = Vx SHL 1.
		* If the most-significant bit of Vx is 1, then VF is set to 1,
		* otherwise to 0. Then Vx is multiplied by 2.
		*/
		void SHL(unsigned short x, unsigned short y);
		
		/**
		* 9xy0 - SNE Vx, Vy
		* Skip next instruction if Vx != Vy.
		* The values of Vx and Vy are compared, and if they are not equal,
		* the program counter is increased by 2
		*/
		void SNE9(unsigned short x, unsigned short y);
		
		/**
		* Annn - LD I, addr
		* Set I = nnn.
		* The value of register I is set to nnn.
		*/
		void LDA(unsigned short nnn);
		
		/**
		* Bnnn - JP V0, addr
		* Jump to location nnn + V0.
		* The program counter is set to nnn plus the value of V0.
		*/
		void JPB(unsigned short nnn);
		
		/**
		* Cxkk - RND Vx, byte
		* Set Vx = random byte AND kk.
		* The interpreter generates a random number from 0 to 255, which is then
		* ANDed with the value kk. The results are stored in Vx.
		*/
		void RND(unsigned short x, unsigned short kk);
		
		/**
		* Dxyn - DRW Vx, Vy, nibble
		* Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
		* The interpreter reads n bytes from memory, starting at the address stored in I. 
		* These bytes are then displayed as * sprites on screen at coordinates (Vx, Vy). Sprites are 
		* XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, 
		* otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates 
		* of the display, it wraps around to the opposite side of the screen.
		*/
		void DRW(unsigned short x, unsigned short y, unsigned short n);
		
		/**
		* Ex9E - SKP Vx
		* Skip next instruction if key with the value of Vx is pressed.
		* Checks the keyboard, and if the key corresponding to the value of Vx is
		* currently in the down position, PC is increased by 2.
		*/
		void SKP(unsigned short x);
		
		/**
		* ExA1 - SKNP Vx
		* Skip next instruction if key with the value of Vx is not pressed.
		* Checks the keyboard, and if the key corresponding to the value of
		* Vx is currently in the up position, PC is increased by 2.
		*/
		void SKNP(unsigned short x);
		
		/**
		* Fx07 - LD Vx, DT
		* Set Vx = delay timer value.
		* The value of DT is placed into Vx.
		*/
		void LDF7(unsigned short x);
		
	    
	    void dump();
		

	private:
	    
	    
};

#endif
