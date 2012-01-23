# Change the following values to suit your system.

CFLAGS=-W -w -O2 -std=c99 
CC=gcc
VERSION = 0.1.8
AR=ar
LD=ld
#LDFLAGS=-no-undefined --enable-runtime-pseudo-reloc


VM_FILES = vm.c strops.c numops.c opcodes.c object.c memory.c stack.c lists.c internal_functions.c stream.c garbage.c debug.c iterators.c
VM_INCLUDES = vm.h strops.h numops.h opcodes.h object.h memory.h stack.h lists.h internal_functions.h stream.h garbage.h debug.h iterators.h
VM_OBJ = vm.o strops.o numops.o opcodes.o object.o memory.o stack.o lists.o internal_functions.o stream.o garbage.o debug.o iterators.o
UT_FILES = unit_tests.c
UT_OBJ = unit_tests.o
UT_INCLUDES = unit_tests.h


test: clean all
	unit_test.exe

all: unit_test 

unit_test: vm_static $(UT_OBJ) 
	$(CC) $(UT_OBJ) vm.a  -o unit_test 

unit_test_o: $(VM_OBJ) $(UT_OBJ)
	$(CC) $(VM_OBJ) $(UT_OBJ)  -o unit_test 

vm_static: $(VM_OBJ)
	$(AR) -rs vm.a $(VM_OBJ)

vm_dynamic: $(VM_OBJ)
	$(CC) -DCREATELIB -shared $(VM_OBJ) $(LDFLAGS) -o vm.dll
	
%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -f unit_test.exe *.o *.a *.so svimpy.src.tar.gz
srcdist:	
	mkdir svimpy-$(VERSION)
	cp $(VM_FILES) svimpy-$(VERSION)
	cp $(VM_INCLUDES) svimpy-$(VERSION)
	cp $(UT_FILES) svimpy-$(VERSION)
	cp $(UT_INCLUDES) svimpy-$(VERSION)
	cp -r tests svimpy-$(VERSION)
	cp Makefile svimpy-$(VERSION)
	tar -cf svimpy.src-$(VERSION).tar svimpy-$(VERSION)
	gzip -9 svimpy.src-$(VERSION).tar
	rm -r svimpy-$(VERSION)

git:	
	git add *
	git commit
	git push openstrike.de master
	git push -u origin master
	
indent:	
	indent -bap -bli0 -i4 -l79 -ncs -npcs -npsl -fca -lc79 -fc1 -ts4 *.c *.h
