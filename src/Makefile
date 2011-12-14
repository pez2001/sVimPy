# Change the following values to suit your system.

CFLAGS=-W -w -O2 -std=c99 

CC=gcc
all: vm.o unit_tests.o opcodes.o object.o stack.o strops.o numops.o memory.o internal_functions.o lists.o unit_test 

unit_test: vm.o stack.o opcodes.o object.o strops.o numops.o stack.o memory.o internal_functions.o lists.o
	$(CC)  unit_tests.o vm.o opcodes.o object.o stack.o strops.o numops.o memory.o internal_functions.o lists.o -o unit_test 
unit_tests.o: unit_tests.c
	$(CC) $(CFLAGS) -c unit_tests.c 
vm.o: vm.c
	$(CC) $(CFLAGS) -c vm.c 
strops.o: strops.c
	$(CC) $(CFLAGS) -c strops.c 
numops.o: numops.c
	$(CC) $(CFLAGS) -c numops.c 
opcodes.o: opcodes.c
	$(CC) $(CFLAGS) -c opcodes.c 
object.o: object.c
	$(CC) $(CFLAGS) -c object.c 
memory.o: memory.c
	$(CC) $(CFLAGS) -c memory.c 
stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c 
lists.o: lists.c
	$(CC) $(CFLAGS) -c lists.c 
internal_functions.o: internal_functions.c
	$(CC) $(CFLAGS) -c internal_functions.c 
	
clean:
	rm vm.o
	rm stack.o
	rm strops.o
	rm numops.o
	rm opcodes.o	
	rm object.o	
	rm memory.o	
	rm unit_test.exe
	rm unit_tests.o

dist:	
	tar -cf sgasm.src.tar assembler.c assembler.h opcodes.h first.asm first.out license.gpl Makefile read.me 
	gzip -9 sgasm.src.tar