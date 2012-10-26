/**
* Author: Devon Guinane
*/

#include <iostream>
#include "Chip8.h"

using std::cout;
using std::endl;

int main(int argc, const char *argv[])
{
	Chip8 cpu;
	cpu.cycle();

	cpu.dump();
	return 0;
}
