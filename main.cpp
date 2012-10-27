/**
* Author: Devon Guinane
*/

#include <iostream>
#include "Chip8.h"
#include "Disassembler.h"
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>


using std::ifstream;
using std::ofstream;
using std::vector;
using std::cout;
using std::endl;
using std::string;

int main(int argc, const char *argv[])
{
	//Chip8 cpu;
	//cpu.cycle();

	//cpu.dump();
	
	
	srand(time(0));
	int randByte = rand() % 255 + 1;
	cout << "RANDOM BYTE:" << randByte << endl;
	
	Disassembler d;
	
	FILE *f= fopen(argv[1], "rb");
	if (f==NULL)
	{
		printf("error: Couldn't open %s\n", argv[1]);
		exit(1);
	}

	//Get the file size
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

	//CHIP-8 convention puts programs in memory at 0x200
	// They will all have hardcoded addresses expecting that
	//
	//Read the file into memory at 0x200 and close it.
	unsigned char *buffer=(unsigned char*)malloc(fsize+0x200);
	fread(buffer+0x200, fsize, 1, f);
	fclose(f);

	int pc = 0x200;
	
	while (pc < (fsize+0x200))
	{
		d.disassembleOpcode(buffer, pc);
		pc += 2;
		printf ("\n");
	}
	
	return 0;

}
