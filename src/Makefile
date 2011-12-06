# Change the following values to suit your system.

CFLAGS=-W -w -O2 -std=c99 

CC=gcc
all: vm.o unit_tests.o opcodes.o object.o stack.o unit_test 

unit_test: vm.o stack.o opcodes.o object.o stack.o
	$(CC)  unit_tests.o opcodes.o object.o stack.o -o unit_test 
unit_tests.o: unit_tests.c
	$(CC) $(CFLAGS) -c unit_tests.c 
vm.o: vm.c
	$(CC) $(CFLAGS) -c vm.c 
stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c 
opcodes.o: opcodes.c
	$(CC) $(CFLAGS) -c opcodes.c 
object.o: object.c
	$(CC) $(CFLAGS) -c object.c 
	
clean:
	rm vm.o
	rm stack.o
	rm opcodes.o	
	rm object.o	
	rm unit_test.exe
	rm unit_tests.o

dist:	
	tar -cf sgasm.src.tar assembler.c assembler.h opcodes.h first.asm first.out license.gpl Makefile read.me 
	gzip -9 sgasm.src.tar