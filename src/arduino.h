#ifndef ARDUINO_H
#define ARDUINO_H

// #define __AVR_ATmega328P__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "arduino/libarduino/Arduino.h"

extern "C" {
#include "debug.h"
#include "vm.h"
#include "object.h"
#include "memory.h"
#include "internal_functions.h"
 }



#endif