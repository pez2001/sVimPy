#ifndef shift_register_h
#define shift_register_h

#include <inttypes.h>
//#include "../lib/Print.h"
#include "../libarduino/Arduino.h"

#define DOUT 8
#define STR 9
#define CLK 7
#define BITORDER LSBFIRST
#define MAX_BYTES 3

class ShiftRegister { 
// : public Print {
public:
  ShiftRegister();
  ShiftRegister(uint8_t, uint8_t, uint8_t, uint8_t,uint8_t);
  void set(uint8_t,uint8_t); 
  void setPin(uint8_t, uint8_t); 
  uint8_t getPin(uint8_t, uint8_t); 
  void setPin(uint8_t, uint8_t, uint8_t); 
  void presetPin(uint8_t, uint8_t, uint8_t); 
  void setBitsHigh(uint8_t,uint8_t);
  void setBitsLow(uint8_t,uint8_t);
  void update();

  uint8_t Bits[MAX_BYTES];
  uint8_t NumBytes;
  uint8_t BitOrder;
  //virtual void write(uint8_t);//virtual

  
private:
  void send(); 
  uint8_t _clk_pin; 
  uint8_t _str_pin;
  uint8_t _data_pin;
};
extern ShiftRegister ShiftReg;


#endif
