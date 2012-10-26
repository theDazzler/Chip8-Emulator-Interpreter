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
		void SNE(unsigned short x, unsigned short kk);
		
		/**
		* 5xy0 - SE Vx, Vy
		* Skip next instruction if Vx = Vy.
		* The interpreter compares register Vx to register Vy, and if they are equal,
		* increments the program counter by 2.
		*/
		void SE5(unsigned short x, unsigned short y);
	    
	    void dump();
		

	private:
	    
	    
};

#endif
