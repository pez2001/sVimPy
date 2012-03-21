/* 
 * sVimPy - small Virtual interpreting machine for Python
 * (c) 2012 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */

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
