/**
* Author: Devon Guinane
*/

main:	main.o Chip8.o
	g++ -o main main.o Chip8.o

main.o:	main.cpp
	g++ -c main.cpp

Chip8.o:	Chip8.cpp Chip8.h
	g++ -c Chip8.cpp

