
main:	main.o Chip8.o Disassembler.o
	g++ -o main main.o Chip8.o Disassembler.o

main.o:	main.cpp
	g++ -c main.cpp

Chip8.o:	Chip8.cpp Chip8.h
	g++ -c Chip8.cpp
	
Disassembler.o:	Disassembler.cpp Disassembler.h
	g++ -c Disassembler.cpp

