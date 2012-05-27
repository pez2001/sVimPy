# Change the following values to suit your system.

#windows testing environment for vm


#competition pro usb
#dunkel blau button 2
#gruen button 1
#weiss schwarz +5v
#gelb rechts
#orange links
#rot unten
#braun oben
#blau button 3
#grau button 4
#quickshot 
#gruen links
#weiss oben
#braun rechts
#blau unten
#orange button 1
#rot button 2
#schwarz +5v /gnd

USER := $(shell whoami)

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
PLATFORM_EXT = 
PLATFORM_NAME = Linux
endif
ifeq ($(UNAME), MINGW32_NT-6.1)
PLATFORM_NAME = Win32
PLATFORM_EXT = .exe
endif



MAJOR_VERSION = 0
MINOR_VERSION = 77
#builds are counted since version 0.5 roughly
BUILD = 257
DEBUG_BUILD = 1134

#CFLAGS= -Ilibraries/fmod -W -w -O2 -std=c99 -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION)
CFLAGS= -W -w -O2 -std=c99 -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -lm
DEBUG_CFLAGS = -m32 -g3 -O0 -Wall -pedantic -Wstrict-prototypes -std=c99 -fbounds-check -Wuninitialized -DUSE_DEBUGGING -DBUILD=$(BUILD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -DDEBUG_BUILD=$(DEBUG_BUILD) -lm
CC=gcc
AR=ar
LD=ld

#arduino environment
#ARDUINO_MCU = atmega328p
ARDUINO_MCU = atmega2560
ifeq ($(ARDUINO_MCU), atmega328p)
ARDUINO_AVRDUDE_PROGRAMMER = arduino
ARDUINO_TERM = /C/Program\ Files\ \(x86\)/Putty/putty -load arduino_uno
ARDUINO_PORT = com3
endif
ifeq ($(ARDUINO_MCU), atmega2560)
ARDUINO_AVRDUDE_PROGRAMMER = stk500v2
ARDUINO_TERM = /C/Program\ Files\ \(x86\)/Putty/putty -load arduino_mega
ARDUINO_PORT = com5
endif

ARDUINO_AVR_DIR = /C/Users/$(USER)/Documents/Arduino-1.0
ARDUINO_UPLOAD_RATE = 115200
#ARDUINO_UPLOAD_RATE = 57600


ARDUINO_AVRDUDE_PORT = $(ARDUINO_PORT)
ARDUINO_AVRDUDE_WRITE_FLASH = -U flash:w:arduino.hex:i
# ARDUINO_AVRDUDE_FLAGS = -v -v -F -D -C $(ARDUINO_AVR_DIR)/hardware/tools/avr/etc/avrdude.conf -p $(ARDUINO_MCU) -P $(ARDUINO_AVRDUDE_PORT) -c $(ARDUINO_AVRDUDE_PROGRAMMER) -b $(ARDUINO_UPLOAD_RATE)
# ARDUINO_AVRDUDE_FLAGS =  -F -C $(ARDUINO_AVR_DIR)/hardware/tools/avr/etc/avrdude.conf -p $(ARDUINO_MCU) -P $(ARDUINO_AVRDUDE_PORT) -c $(ARDUINO_AVRDUDE_PROGRAMMER) -b $(ARDUINO_UPLOAD_RATE)
ARDUINO_AVRDUDE_FLAGS = -D -C $(ARDUINO_AVR_DIR)/hardware/tools/avr/etc/avrdude.conf -p $(ARDUINO_MCU) -P $(ARDUINO_AVRDUDE_PORT) -c $(ARDUINO_AVRDUDE_PROGRAMMER) -b $(ARDUINO_UPLOAD_RATE)
# -e -i 10

ARDUINO_INCLUDES = $(ARDUINO_	AVR_DIR)/hardware/cores/arduino
ARDUINO_STANDARD_INCLUDES = $(ARDUINO_AVR_DIR)/hardware/arduino/variants/standard
ARDUINO_AVR_TOOLS_PATH = $(ARDUINO_AVR_DIR)/hardware/tools/avr/bin
ARDUINO_CSTANDARD = -std=c99
ARDUINO_CWARN = -Wall -pedantic -Wstrict-prototypes 
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
#ARDUINO_FLAGS = $(ARDUINO_PYTHON_FEATURES)  -g -gdwarf-2 -fno-tree-scev-cprop -ffreestanding -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fno-move-loop-invariants -fno-tree-loop-optimize -mcall-prologues -fno-inline-small-functions -Os -Wl,--gc-sections,--relax -g -fno-exceptions -ffunction-sections -fdata-sections -DF_CPU=16000000L -DARDUINO=100 -mmcu=$(ARDUINO_MCU)
ARDUINO_FLAGS = $(ARDUINO_PYTHON_FEATURES)  -fno-tree-scev-cprop -ffreestanding -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fno-move-loop-invariants -fno-tree-loop-optimize -mcall-prologues -fno-inline-small-functions -Os -Wl,--gc-sections,--relax -fno-exceptions -ffunction-sections -fdata-sections -DF_CPU=16000000L -DARDUINO=100 -mmcu=$(ARDUINO_MCU)
#-fno-inline-small-functions  -finline-limit=6 -fno-jump-tables
ARDUINO_CINCS = -I$(ARDUINO_INCLUDES) -I$(ARDUINO_STANDARD_INCLUDES)
ARDUINO_CFLAGS = $(ARDUINO_FLAGS) $(ARDUINO_CINCS) $(ARDUINO_CWARN) $(ARDUINO_CSTANDARD) $(ARDUINO_CTUNING)
ARDUINO_CXXFLAGS =  $(ARDUINO_FLAGS) $(ARDUINO_CINCS)
# ARDUINO_LDFLAGS = -lm
ARDUINO_LIB_DIR = arduino/libarduino

ARDUINO_CORE_SRC = $(ARDUINO_LIB_DIR)/wiring.c $(ARDUINO_LIB_DIR)/wiring_analog.c $(ARDUINO_LIB_DIR)/wiring_digital.c $(ARDUINO_LIB_DIR)/wiring_pulse.c $(ARDUINO_LIB_DIR)/wiring_shift.c $(ARDUINO_LIB_DIR)/WInterrupts.c 
ARDUINO_LIB_OBJ = $(ARDUINO_LIB_DIR)/wiring.ao $(ARDUINO_LIB_DIR)/wiring_analog.ao $(ARDUINO_LIB_DIR)/wiring_digital.ao $(ARDUINO_LIB_DIR)/wiring_pulse.ao $(ARDUINO_LIB_DIR)/wiring_shift.ao $(ARDUINO_LIB_DIR)/WInterrupts.ao 
#ARDUINO_CXXSRC = $(ARDUINO_LIB_DIR)/HardwareSerial.cpp $(ARDUINO_LIB_DIR)/WMath.cpp $(ARDUINO_LIB_DIR)/Print.cpp $(ARDUINO_LIB_DIR)/Stream.cpp $(ARDUINO_LIB_DIR)/Tone.cpp $(ARDUINO_LIB_DIR)/WString.cpp
ARDUINO_CXXSRC = $(ARDUINO_LIB_DIR)/WMath.cpp $(ARDUINO_LIB_DIR)/WString.cpp
#ARDUINO_LIB_CXX_OBJ = $(ARDUINO_LIB_DIR)/HardwareSerial.aopp $(ARDUINO_LIB_DIR)/WMath.aopp $(ARDUINO_LIB_DIR)/Print.aopp $(ARDUINO_LIB_DIR)/Stream.aopp $(ARDUINO_LIB_DIR)/Tone.aopp $(ARDUINO_LIB_DIR)/WString.aopp
ARDUINO_LIB_CXX_OBJ = $(ARDUINO_LIB_DIR)/WMath.aopp $(ARDUINO_LIB_DIR)/WString.aopp
ARDUINO_VM_OBJ = vm.ao strops.ao numops.ao opcodes.ao object.ao memory.ao stack.ao lists.ao internal_functions.ao internal_classes.ao stream.ao garbage.ao debug.ao iterators.ao


VM_FILES = vm.c strops.c numops.c opcodes.c object.c memory.c stack.c lists.c internal_functions.c internal_classes.c stream.c garbage.c debug.c iterators.c types.c
VM_BINS = vm.a
VM_INCLUDE_FILES = vm.h strops.h numops.h opcodes.h object.h memory.h stack.h lists.h internal_functions.h internal_classes.h stream.h garbage.h debug.h iterators.h types.h
VM_OBJ = vm.o strops.o numops.o opcodes.o object.o memory.o stack.o lists.o internal_functions.o internal_classes.o stream.o garbage.o debug.o iterators.o types.o
VM_DEBUG_OBJ = vm.do strops.do numops.do opcodes.do object.do memory.do stack.do lists.do internal_functions.do internal_classes.do stream.do garbage.do debug.do iterators.do types.do

#UT_FILES = unit_tests.c libraries/fmod_wrapper.c
#UT_OBJ = unit_tests.o libraries/fmod_wrapper.o
#UT_DEBUG_OBJ = unit_tests.do libraries/fmod_wrapper.do
#UT_INCLUDES = unit_tests.h libraries/fmod_wrapper.h

UT_FILES = unit_tests.c
UT_OBJ = unit_tests.o
UT_BINS = unit_tests$(PLATFORM_EXT)
UT_DEBUG_OBJ = unit_tests.do
UT_INCLUDE_FILES = unit_tests.h


ARDUINO_FILES = arduino.cpp arduino/io.cpp arduino/debug.cpp
ARDUINO_BINS = arduino.hex 
ARDUINO_INCLUDE_FILES = arduino.h arduino/io.h arduino/debug.h
ARDUINO_OBJ = arduino.aopp arduino/io.aopp arduino/debug.aopp
ARDUINO_LIBS = core.a

TOOLS_BUILD_INC_FILES = tools/build_inc/build_inc.c
TOOLS_BUILD_INC_BINS = build_inc$(PLATFORM_EXT)
TOOLS_BUILD_INC_INCLUDE_FILES = tools/build_inc/build_inc.h
TOOLS_BUILD_INC_OBJ = tools/build_inc/build_inc.o

TOOLS_COMPRESS_PYC_FILES = tools/compress_pyc/compress_pyc.c
TOOLS_COMPRESS_PYC_BINS = compress_pyc$(PLATFORM_EXT)
TOOLS_COMPRESS_PYC_INCLUDE_FILES = tools/compress_pyc/compress_pyc.h
TOOLS_COMPRESS_PYC_OBJ = tools/compress_pyc/compress_pyc.o

TOOLS_PACK_POS_FILES = tools/pack_pos/pack_pos.c
TOOLS_PACK_POS_BINS = pack_pos$(PLATFORM_EXT)
TOOLS_PACK_POS_INCLUDE_FILES = tools/pack_pos/pack_pos.h
TOOLS_PACK_POS_OBJ = tools/pack_pos/pack_pos.o

#SVIMPY_FILES = svimpy.c libraries/fmod_wrapper.c
#SVIMPY_INCLUDE_FILES = svimpy.h libraries/fmod_wrapper.h
#SVIMPY_OBJ = svimpy.o libraries/fmod_wrapper.o
#SVIMPY_DEBUG_OBJ = svimpy.do libraries/fmod_wrapper.do

SVIMPY_FILES = svimpy.c
SVIMPY_INCLUDE_FILES = svimpy.h
SVIMPY_BINS = svimpy$(PLATFORM_EXT)
SVIMPY_DEBUG_OBJ = svimpy.do
SVIMPY_OBJ = svimpy.o



test: all
	./unit_tests$(PLATFORM_EXT)

test_debug: debug
	./unit_tests_debug$(PLATFORM_EXT)
	
debug: build_inc svimpy_debug unit_tests_debug
	./build_inc$(PLATFORM_EXT) Makefile DEBUG_BUILD

all: clean clean_debug clean_binaries build_inc svimpy unit_tests compress_pyc pack_pos debug
	./build_inc$(PLATFORM_EXT) Makefile BUILD

unit_tests: vm_static $(UT_OBJ) 
#	$(CC) $(UT_OBJ) vm.a libraries/libfmodex.a -o unit_tests 
	$(CC) $(UT_OBJ) vm.a -lm -o unit_tests 

unit_tests_debug: vm_static_debug $(UT_DEBUG_OBJ) 
#	$(CC) $(UT_DEBUG_OBJ) vm.da libraries/libfmodex.a -o unit_tests_debug 
	$(CC) $(UT_DEBUG_OBJ) vm.da -lm -o unit_tests_debug 

unit_tests_o: $(VM_OBJ) $(UT_OBJ)
	$(CC) $(VM_OBJ) $(UT_OBJ)  -lm -o unit_tests 

build_inc: $(TOOLS_BUILD_INC_OBJ) 
	$(CC) $(TOOLS_BUILD_INC_OBJ) -o build_inc

build_inc_o: $(TOOLS_BUILD_INC_OBJ)
	$(CC) $(TOOLS_BUILD_INC_OBJ) -o build_inc 

pack_pos: $(TOOLS_PACK_POS_OBJ) 
	$(CC) $(TOOLS_PACK_POS_OBJ) -o pack_pos

pack_pos_o: $(TOOLS_PACK_POS_OBJ)
	$(CC) $(TOOLS_PACK_POS_OBJ) -o pack_pos 

compress_pyc: vm_static $(TOOLS_COMPRESS_PYC_OBJ) 
	$(CC) $(TOOLS_COMPRESS_PYC_OBJ) vm.a -lm -o compress_pyc

compress_pyc_o: $(TOOLS_COMPRESS_PYC_OBJ)
	$(CC) $(TOOLS_COMPRESS_PYC_OBJ) -o compress_pyc 

svimpy: vm_static $(SVIMPY_OBJ) 
#	$(CC) $(SVIMPY_OBJ) vm.a libraries/libfmodex.a -lkernel32 -o svimpy 
	$(CC) $(SVIMPY_OBJ) vm.a -lm -o svimpy 

svimpy_debug: vm_static_debug $(SVIMPY_DEBUG_OBJ) 
#	$(CC) $(SVIMPY_DEBUG_OBJ) vm.da libraries/libfmodex.a -o svimpy_debug 
	$(CC) $(SVIMPY_DEBUG_OBJ) vm.da -lm -o svimpy_debug 

svimpy_o: $(VM_OBJ) $(SVIMPY_OBJ)
	$(CC) $(VM_OBJ) $(SVIMPY_OBJ)  -lm -o svimpy 
	
vm_static: $(VM_OBJ)
	$(AR) -rs vm.a $(VM_OBJ)

vm_static_debug: $(VM_DEBUG_OBJ)
	$(AR) -rs vm.da $(VM_DEBUG_OBJ)

vm_dynamic: $(VM_OBJ)
	$(CC) -DCREATELIB -shared $(VM_OBJ) $(LDFLAGS) -o vm.dll
	
%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

%.do: %.c 
	$(CC) $(DEBUG_CFLAGS) -c -o $@ $<

core.a: $(ARDUINO_LIB_OBJ) $(ARDUINO_LIB_CXX_OBJ) $(ARDUINO_VM_OBJ)
	$(ARDUINO_AR) rcs core.a $(ARDUINO_LIB_OBJ) $(ARDUINO_LIB_CXX_OBJ) $(ARDUINO_VM_OBJ)

%.ao: %.c
	$(ARDUINO_CC) $(ARDUINO_CFLAGS) $(ARDUINO_LDFLAGS) -c -o $@ $^

%.aopp: %.cpp
	$(ARDUINO_CXX) $(ARDUINO_CXXFLAGS) $(ARDUINO_LDFLAGS) -c -o $@ $^

arduino.elf: $(ARDUINO_OBJ) core.a
ifeq ($(ARDUINO_MCU), atmega328p)
	cp -f arduino/libarduino/pins_arduino.h_328p arduino/libarduino/pins_arduino.h
endif
ifeq ($(ARDUINO_MCU), atmega2560)
	cp -f arduino/libarduino/pins_arduino.h_mega2560 arduino/libarduino/pins_arduino.h
endif
	$(ARDUINO_CXX) $(ARDUINO_CXXFLAGS) $(ARDUINO_LDFLAGS) -o $@ $^ 

arduino.hex: arduino.elf
	$(ARDUINO_OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $@
	$(ARDUINO_OBJCOPY) -g --strip-unneeded -O ihex -R .eeprom $< $@

upload: clean clean_binaries arduino.hex
ifeq ($(ARDUINO_MCU), atmega2560)
	printf "0" >$(ARDUINO_PORT)
endif
	$(ARDUINO_AVRDUDE) $(ARDUINO_AVRDUDE_FLAGS) $(ARDUINO_AVRDUDE_WRITE_FLASH)

uploadview: upload
	$(ARDUINO_TERM)
	
clean:
	rm -f *.elf *.do *.da *.ao *.aopp *.o *.a *.so tools/pack_pos/*.o tools/build_inc/*.o tools/compress_pyc/*.o arduino/*.ao arduino/*.aopp arduino/libarduino/*.aopp arduino/libarduino/*.ao

clean_binaries:
	rm -f svimpy$(PLATFORM_EXT) unit_tests$(PLATFORM_EXT) svimpy_debug$(PLATFORM_EXT) unit_tests_debug$(PLATFORM_EXT) build_inc$(PLATFORM_EXT) compress_pyc$(PLATFORM_EXT) pack_pos$(PLATFORM_EXT) arduino.hex

clean_debug:
	rm -f build_inc$(PLATFORM_EXT) svimpy_debug$(PLATFORM_EXT) unit_tests_debug$(PLATFORM_EXT) *.do *.da 
	
srcdist:	clean clean_binaries
	rm -f -r svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	mkdir svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(VM_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(VM_INCLUDE_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(UT_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(UT_INCLUDE_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(SVIMPY_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(SVIMPY_INCLUDE_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_BUILD_INC_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_BUILD_INC_INCLUDE_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_PACK_POS_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_PACK_POS_INCLUDE_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_COMPRESS_PYC_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_COMPRESS_PYC_INCLUDE_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(ARDUINO_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(ARDUINO_INCLUDE_FILES) svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp -r arduino svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp -r tools svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp -r tests svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp -r ../test_scripts svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp ../readme svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp Makefile svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	tar -cf svimpy.src-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD).tar svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	rm -f svimpy.src-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD).tar.gz
	gzip -9 svimpy.src-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD).tar
	rm -r svimpy-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	mv svimpy.src-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD).tar.gz ..

binariesdist: clean_binaries clean all clean
	rm -f -r svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	mkdir svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(VM_BINS) svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(UT_BINS) svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(SVIMPY_BINS) svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_BUILD_INC_BINS) svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_PACK_POS_BINS) svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_COMPRESS_PYC_BINS) svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp -r tests svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp ../readme svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	tar -cf svimpy.bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	rm -f svimpy.bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar.gz
	gzip -9 svimpy.bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar
	rm -r svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	mv svimpy.bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar.gz ..

hexbinariesdist: clean_binaries clean all arduino.hex clean
	rm -f -r svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	mkdir svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(VM_BINS) svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(UT_BINS) svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(SVIMPY_BINS) svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_BUILD_INC_BINS) svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_PACK_POS_BINS) svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(TOOLS_COMPRESS_PYC_BINS) svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp $(ARDUINO_BINS) svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp -r tests svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	cp ../readme svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	tar -cf svimpy.hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar svimpy-bin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	rm -f svimpy.hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar.gz
	gzip -9 svimpy.hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar
	rm -r svimpy-hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)
	mv svimpy.hexbin-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD)-$(PLATFORM_NAME).tar.gz ..
	
dist:	srcdist binariesdist 
	make clean clean_binaries
hexdist:	srcdist hexbinariesdist 
	make clean clean_binaries
git:	
	git add *
	git commit
	git push openstrike.de master
	git push -u origin master
	
indent:	
	indent -bap -bli0 -i4 -l79 -ncs -npcs -npsl -fca -lc79 -fc1 -ts4 *.c *.h
