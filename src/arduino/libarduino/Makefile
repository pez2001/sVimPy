MCU = atmega328p
AVR_DIR = /C/Users/pez2001/Documents/Arduino-1.0

ARDUINO = $(AVR_DIR)/hardware/cores/arduino
ARDUINO_STANDARD = $(AVR_DIR)/hardware/arduino/variants/standard
AVR_TOOLS_PATH = $(AVR_DIR)/hardware/tools/avr/bin
CSTANDARD = -std=gnu99
CWARN = -Wall -Wstrict-prototypes
CTUNING = -O2 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fno-jump-tables -mcall-prologues -finline-functions
CC = $(AVR_TOOLS_PATH)/avr-gcc
CXX = $(AVR_TOOLS_PATH)/avr-g++
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR  = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm
ARDUINO_FLAGS = -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -DF_CPU=16000000L -DARDUINO=100 -mmcu=$(MCU)

CINCS = -I$(ARDUINO) -I$(ARDUINO_STANDARD)
CXXINCS = -I$(ARDUINO) -I$(ARDUINO_STANDARD)
CFLAGS = $(ARDUINO_FLAGS) $(CINCS) $(CWARN) $(CSTANDARD) $(CTUNING)
CXXFLAGS =  $(ARDUINO_FLAGS) $(CINCS)
LDFLAGS = -lm


SRC = wiring.c wiring_analog.c wiring_digital.c wiring_pulse.c wiring_shift.c WInterrupts.c 

LIB_OBJ = wiring.o wiring_analog.o wiring_digital.o wiring_pulse.o wiring_shift.o WInterrupts.o 

CXXSRC = HardwareSerial.cpp WMath.cpp Print.cpp Stream.cpp Tone.cpp WString.cpp

LIB_CXX_OBJ = HardwareSerial.opp WMath.opp Print.opp Stream.opp Tone.opp WString.opp

all: core.a

core.a: $(LIB_OBJ) $(LIB_CXX_OBJ)
	$(AR) rcs core.a $(LIB_OBJ) $(LIB_CXX_OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

%.opp: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $^

clean:
	rm -rf *.o *.a

	
