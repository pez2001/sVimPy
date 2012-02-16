# Change the following values to suit your system.

#windows testing environment for vm

CFLAGS= -Wall -Wstrict-prototypes -O2 -std=c99 
# CFLAGS= -W -w -O2 -std=c99 
CC=gcc
VERSION = 0.4a
AR=ar
LD=ld

#arduino environment
ARDUINO_AVR_DIR = /C/Users/asus/Documents/Arduino-1.0

ARDUINO_TERM = /C/Program\ Files\ \(x86\)/Putty/putty -load arduino_uno
ARDUINO_MCU = atmega328p
ARDUINO_PORT = com3
ARDUINO_UPLOAD_RATE = 115200
ARDUINO_AVRDUDE_PROGRAMMER = arduino
#ARDUINO_TERM = /C/Program\ Files\ \(x86\)/Putty/putty -load arduino_mega
#ARDUINO_MCU = atmega2560
#ARDUINO_PORT = com5
#ARDUINO_UPLOAD_RATE = 115200
#ARDUINO_AVRDUDE_PROGRAMMER = stk500v2

ARDUINO_AVRDUDE_PORT = $(ARDUINO_PORT)
ARDUINO_AVRDUDE_WRITE_FLASH = -U flash:w:arduino.hex:i
# ARDUINO_AVRDUDE_FLAGS = -v -v -F -D -C $(ARDUINO_AVR_DIR)/hardware/tools/avr/etc/avrdude.conf -p $(ARDUINO_MCU) -P $(ARDUINO_AVRDUDE_PORT) -c $(ARDUINO_AVRDUDE_PROGRAMMER) -b $(ARDUINO_UPLOAD_RATE)
# ARDUINO_AVRDUDE_FLAGS =  -F -C $(ARDUINO_AVR_DIR)/hardware/tools/avr/etc/avrdude.conf -p $(ARDUINO_MCU) -P $(ARDUINO_AVRDUDE_PORT) -c $(ARDUINO_AVRDUDE_PROGRAMMER) -b $(ARDUINO_UPLOAD_RATE)
ARDUINO_AVRDUDE_FLAGS = -D -C $(ARDUINO_AVR_DIR)/hardware/tools/avr/etc/avrdude.conf -p $(ARDUINO_MCU) -P $(ARDUINO_AVRDUDE_PORT) -c $(ARDUINO_AVRDUDE_PROGRAMMER) -b $(ARDUINO_UPLOAD_RATE)
# -e -i 10

ARDUINO_INCLUDES = $(ARDUINO_AVR_DIR)/hardware/cores/arduino
ARDUINO_STANDARD_INCLUDES = $(ARDUINO_AVR_DIR)/hardware/arduino/variants/standard
ARDUINO_AVR_TOOLS_PATH = $(ARDUINO_AVR_DIR)/hardware/tools/avr/bin
ARDUINO_CSTANDARD = -std=c99
ARDUINO_CWARN = -Wall -Wstrict-prototypes
# ARDUINO_CWARN = -W -w
# ARDUINO_CTUNING = -O2 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fno-jump-tables -mcall-prologues -finline-functions
ARDUINO_CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct 
ARDUINO_CC = $(ARDUINO_AVR_TOOLS_PATH)/avr-gcc
ARDUINO_CXX = $(ARDUINO_AVR_TOOLS_PATH)/avr-g++
ARDUINO_OBJCOPY = $(ARDUINO_AVR_TOOLS_PATH)/avr-objcopy
ARDUINO_OBJDUMP = $(ARDUINO_AVR_TOOLS_PATH)/avr-objdump
ARDUINO_AR  = $(ARDUINO_AVR_TOOLS_PATH)/avr-ar
ARDUINO_SIZE = $(ARDUINO_AVR_TOOLS_PATH)/avr-size
ARDUINO_NM = $(ARDUINO_AVR_TOOLS_PATH)/avr-nm
ARDUINO_AVRDUDE = $(ARDUINO_AVR_TOOLS_PATH)/avrdude
ARDUINO_PYTHON_FEATURES = -DUSE_ARDUINO_FUNCTIONS
ARDUINO_FLAGS = $(ARDUINO_PYTHON_FEATURES) -fno-tree-scev-cprop -ffreestanding -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fno-move-loop-invariants -fno-tree-loop-optimize -mcall-prologues -fno-inline-small-functions -Os -Wl,--gc-sections,--relax -g -fno-exceptions -ffunction-sections -fdata-sections -DF_CPU=16000000L -DARDUINO=100 -mmcu=$(ARDUINO_MCU)
#-fno-inline-small-functions  -finline-limit=6 -fno-jump-tables
ARDUINO_CINCS = -I$(ARDUINO_INCLUDES) -I$(ARDUINO_STANDARD_INCLUDES)
ARDUINO_CFLAGS = $(ARDUINO_FLAGS) $(ARDUINO_CINCS) $(ARDUINO_CWARN) $(ARDUINO_CSTANDARD) $(ARDUINO_CTUNING)
ARDUINO_CXXFLAGS =  $(ARDUINO_FLAGS) $(ARDUINO_CINCS)
# ARDUINO_LDFLAGS = -lm
ARDUINO_LIB_DIR = arduino/libarduino

ARDUINO_CORE_SRC = $(ARDUINO_LIB_DIR)/wiring.c $(ARDUINO_LIB_DIR)/wiring_analog.c $(ARDUINO_LIB_DIR)/wiring_digital.c $(ARDUINO_LIB_DIR)/wiring_pulse.c $(ARDUINO_LIB_DIR)/wiring_shift.c $(ARDUINO_LIB_DIR)/WInterrupts.c 
ARDUINO_LIB_OBJ = $(ARDUINO_LIB_DIR)/wiring.ao $(ARDUINO_LIB_DIR)/wiring_analog.ao $(ARDUINO_LIB_DIR)/wiring_digital.ao $(ARDUINO_LIB_DIR)/wiring_pulse.ao $(ARDUINO_LIB_DIR)/wiring_shift.ao $(ARDUINO_LIB_DIR)/WInterrupts.ao 
ARDUINO_CXXSRC = $(ARDUINO_LIB_DIR)/HardwareSerial.cpp $(ARDUINO_LIB_DIR)/WMath.cpp $(ARDUINO_LIB_DIR)/Print.cpp $(ARDUINO_LIB_DIR)/Stream.cpp $(ARDUINO_LIB_DIR)/Tone.cpp $(ARDUINO_LIB_DIR)/WString.cpp
ARDUINO_LIB_CXX_OBJ = $(ARDUINO_LIB_DIR)/HardwareSerial.aopp $(ARDUINO_LIB_DIR)/WMath.aopp $(ARDUINO_LIB_DIR)/Print.aopp $(ARDUINO_LIB_DIR)/Stream.aopp $(ARDUINO_LIB_DIR)/Tone.aopp $(ARDUINO_LIB_DIR)/WString.aopp
ARDUINO_VM_OBJ = vm.ao strops.ao numops.ao opcodes.ao object.ao memory.ao stack.ao lists.ao internal_functions.ao stream.ao garbage.ao debug.ao iterators.ao


VM_FILES = vm.c strops.c numops.c opcodes.c object.c memory.c stack.c lists.c internal_functions.c stream.c garbage.c debug.c iterators.c types.c
VM_INCLUDES = vm.h strops.h numops.h opcodes.h object.h memory.h stack.h lists.h internal_functions.h stream.h garbage.h debug.h iterators.h types.h
VM_OBJ = vm.o strops.o numops.o opcodes.o object.o memory.o stack.o lists.o internal_functions.o stream.o garbage.o debug.o iterators.o types.o
UT_FILES = unit_tests.c arduino/io.c
UT_OBJ = unit_tests.o arduino/io.o
UT_INCLUDES = unit_tests.h arduino/io.h

ARDUINO_FILES = arduino.cpp arduino/io.cpp arduino/debug.cpp
ARDUINO_INCLUDE_FILES = arduino.h arduino/io.h arduino/debug.h
ARDUINO_OBJ = arduino.aopp arduino/io.aopp arduino/debug.aopp
ARDUINO_LIBS = core.a

test: clean all
	unit_tests.exe

all: unit_tests

unit_tests: vm_static $(UT_OBJ) 
	$(CC) $(UT_OBJ) vm.a  -o unit_tests 

unit_test_o: $(VM_OBJ) $(UT_OBJ)
	$(CC) $(VM_OBJ) $(UT_OBJ)  -o unit_tests 

vm_static: $(VM_OBJ)
	$(AR) -rs vm.a $(VM_OBJ)

vm_dynamic: $(VM_OBJ)
	$(CC) -DCREATELIB -shared $(VM_OBJ) $(LDFLAGS) -o vm.dll
	
%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

core.a: $(ARDUINO_LIB_OBJ) $(ARDUINO_LIB_CXX_OBJ) $(ARDUINO_VM_OBJ)
	$(ARDUINO_AR) rcs core.a $(ARDUINO_LIB_OBJ) $(ARDUINO_LIB_CXX_OBJ) $(ARDUINO_VM_OBJ)

%.ao: %.c
	$(ARDUINO_CC) $(ARDUINO_CFLAGS) $(ARDUINO_LDFLAGS) -c -o $@ $^

%.aopp: %.cpp
	$(ARDUINO_CXX) $(ARDUINO_CXXFLAGS) $(ARDUINO_LDFLAGS) -c -o $@ $^

arduino.elf: $(ARDUINO_OBJ) core.a
	$(ARDUINO_CXX) $(ARDUINO_CXXFLAGS) $(ARDUINO_LDFLAGS) -o $@ $^ 

arduino.hex: arduino.elf
	$(ARDUINO_OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $@
	$(ARDUINO_OBJCOPY) -g --strip-unneeded -O ihex -R .eeprom $< $@

upload: clean arduino.hex
	printf "0" >$(ARDUINO_PORT)
	$(ARDUINO_AVRDUDE) $(ARDUINO_AVRDUDE_FLAGS) $(ARDUINO_AVRDUDE_WRITE_FLASH)

uploadview: upload
	$(ARDUINO_TERM)
	
clean:
	rm -f unit_tests.exe *.ao *.aopp *.hex *.o *.a *.so arduino/*.ao arduino/aopp arduino/libarduino/*.aopp arduino/libarduino/*.ao svimpy.src.tar.gz
	
srcdist:	
	mkdir svimpy-$(VERSION)
	cp $(VM_FILES) svimpy-$(VERSION)
	cp $(VM_INCLUDES) svimpy-$(VERSION)
	cp $(UT_FILES) svimpy-$(VERSION)
	cp $(UT_INCLUDES) svimpy-$(VERSION)
	cp $(ARDUINO_FILES) svimpy-$(VERSION)
	cp $(ARDUINO_INCLUDE_FILES) svimpy-$(VERSION)
	cp -r arduino svimpy-$(VERSION)
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
