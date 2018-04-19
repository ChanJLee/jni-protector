build: main.o
	g++ main.o -o main

main.o: main.cpp ../yasc/elf.h
	g++ -c main.cpp

clean:
	rm -rf *.o~