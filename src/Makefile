# Change the following values to suit your system.

#CFLAGS=`sdl-config --cflags` -W -Wall -ggdb -O2

#SDL_LIB=`sdl-config --libs`
#	$(SDL_LIB) -lSDL_image

CC=gcc
all: vm.o unit_tests.o unit_test 

unit_test: vm.o
	$(CC)  unit_tests.o -o unit_test 
unit_tests.o: unit_tests.c
	$(CC) $(CFLAGS) -c unit_tests.c 
vm.o: vm.c
	$(CC) $(CFLAGS) -c vm.c 
	
clean:
	rm vm.o
	rm unit_test.exe
	rm unit_tests.o

dist:	
	tar -cf sgasm.src.tar assembler.c assembler.h opcodes.h first.asm first.out license.gpl Makefile read.me 
	gzip -9 sgasm.src.tar