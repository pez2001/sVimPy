MCU = atmega328p
PORT = com3
UPLOAD_RATE = 115200
AVRDUDE_PROGRAMMER = arduino
# PROJECT_DIR = c:/Users/pez2001/Documents/projects/mypy/src/arduino
PROJECT_DIR = .
LIB_DIR = $(PROJECT_DIR)/libarduino
AVR_DIR = /C/Users/pez2001/Documents/Arduino-1.0
ARDUINO = $(AVR_DIR)/hardware/cores/arduino
ARDUINO_STANDARD = $(AVR_DIR)/hardware/arduino/variants/standard
AVR_TOOLS_PATH = $(AVR_DIR)/hardware/tools/avr/bin
CSTANDARD = -std=gnu99
CWARN = -Wall -Wstrict-prototypes
CTUNING = -O2 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fno-jump-tables -mcall-prologues -finline-functions
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_FLAGS = -V -F -C $(AVR_DIR)/hardware/tools/avr/etc/avrdude.conf -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b $(UPLOAD_RATE)
CC = $(AVR_TOOLS_PATH)/avr-gcc
CXX = $(AVR_TOOLS_PATH)/avr-g++
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR  = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm
AVRDUDE = $(AVR_TOOLS_PATH)/avrdude
REMOVE = rm -f
MV = mv -f
ARDUINO_FLAGS = -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -DF_CPU=16000000L -DARDUINO=100 -mmcu=$(MCU)

CFLAGS = $(ARDUINO_FLAGS) $(CINCS) $(CWARN) $(CSTANDARD) $(CEXTRA) $(CTUNING)
CXXFLAGS =  $(ARDUINO_FLAGS) $(CDEFS) $(CINCS)
LDFLAGS = -lm

CINCS = -I$(ARDUINO) -I$(ARDUINO_STANDARD)
CXXINCS = -I$(ARDUINO) -I$(ARDUINO_STANDARD)

TARGET    = basic

OBJ        = $(TARGET).o $(LIB_DIR)/core.a



all: $(TARGET).hex $(LIB_DIR)/core.a

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

upload: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)



%.elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.srec: %.elf
	$(OBJCOPY) -O srec -R .eeprom $< $@

%.bin: %.elf
	$(OBJCOPY) -O binary -R .eeprom $< $@
	
