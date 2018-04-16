all: main.o
	g++ main.o -o main

main.o: main.cpp elf.h
	g++ -c main.cpp