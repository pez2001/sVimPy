# Change the following values to suit your system.

CFLAGS=-W -w -O2 -std=c99 
CC=gcc

all: vm.o unit_tests.o opcodes.o object.o stack.o strops.o numops.o memory.o internal_functions.o lists.o stream.o unit_test 

unit_test: vm.o stack.o opcodes.o object.o strops.o numops.o stack.o memory.o internal_functions.o lists.o stream.o
	$(CC)  unit_tests.o vm.o opcodes.o object.o stack.o strops.o numops.o memory.o internal_functions.o lists.o stream.o -o unit_test 
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
stream.o: stream.c
	$(CC) $(CFLAGS) -c stream.c 
	
clean:
	rm vm.o
	rm stack.o
	rm strops.o
	rm numops.o
	rm opcodes.o	
	rm object.o	
	rm memory.o	
	rm lists.o
	rm stream.o
	rm internal_functions.o
	rm unit_test.exe
	rm unit_tests.o
	rm svimpy.src.tar.gz

srcdist:	
	tar -cf svimpy.src.tar unit_tests.c unit_tests.h opcodes.c opcodes.h memory.c memory.h object.c object.h stack.c stack.h stream.c stream.h internal_functions.c internal_functions.h vm.c vm.h lists.c lists.h strops.c strops.h numops.c numops.h object.c object.h Makefile
	gzip -9 svimpy.src.tar

git:	
	git add *
	git commit
	git push openstrike.de master
	git push -u origin master
	
