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

#include "shift_register.h"

ShiftRegister::ShiftRegister() {
  BitOrder = BITORDER;
  _clk_pin = CLK;
  _str_pin = STR;
  _data_pin = DOUT; 
  NumBytes = 3;
  pinMode(_clk_pin, OUTPUT);
  pinMode(_str_pin, OUTPUT);
  pinMode(_data_pin, OUTPUT);
  for(int i = 0;i<NumBytes;i++)
   Bits[i] = 0;
  send();
}
ShiftRegister::ShiftRegister(uint8_t clock, uint8_t strobe, uint8_t data, uint8_t bit_order,uint8_t num_bytes) {
  BitOrder = bit_order;
  _clk_pin = clock;
  _str_pin = strobe;
  _data_pin = data; 
  NumBytes = num_bytes;
  pinMode(_clk_pin, OUTPUT);
  pinMode(_str_pin, OUTPUT);
  pinMode(_data_pin, OUTPUT);
  for(int i = 0;i<num_bytes;i++)
   Bits[i] = 0;
  send();
}
void ShiftRegister::set(uint8_t value,uint8_t byte_num) {
  Bits[byte_num] = value;
  send();
}
uint8_t ShiftRegister::getPin(uint8_t pin, uint8_t byte_num) {
 if(Bits[byte_num] & 1<<(pin-1))
  return(true);
 return(false);
}
void ShiftRegister::setPin(uint8_t pin,uint8_t value) {
 int byte_num = pin % 8;
 if(value == HIGH)
  Bits[byte_num] |= 1<<((pin-byte_num*8)-1);
 else 
  Bits[byte_num] &= ~(1<<((pin-byte_num*8)-1));
 send();
}
void ShiftRegister::setPin(uint8_t pin,uint8_t value,uint8_t byte_num) {
 if(value == HIGH)
  Bits[byte_num] |= 1<<(pin-1);
 else 
  Bits[byte_num] &= ~(1<<(pin-1));
 send();
}
void ShiftRegister::presetPin(uint8_t pin,uint8_t value,uint8_t byte_num) {
 if(value == HIGH)
  Bits[byte_num] |= 1<<(pin-1);
 else 
  Bits[byte_num] &= ~(1<<(pin-1));
}
void ShiftRegister::update() {
 send();
}
void ShiftRegister::setBitsHigh(uint8_t bits,uint8_t byte_num) {
  Bits[byte_num] |= bits;
  send();
}
void ShiftRegister::setBitsLow(uint8_t bits,uint8_t byte_num) {
  Bits[byte_num] &= ~bits;
  send();
}
void ShiftRegister::send() {
 if(_clk_pin != 255)
 {
  digitalWrite(_str_pin,LOW);
  delayMicroseconds(130);
  int i = NumBytes - 1;
  while(i>=0)
  {
   shiftOut(_data_pin, _clk_pin, BitOrder, Bits[i]);
   i--;
  }
  digitalWrite(_str_pin, HIGH);
  delayMicroseconds(130);
  digitalWrite(_str_pin,LOW);
  delayMicroseconds(130);
 }
}
/*
void ShiftRegister2::write(uint8_t value) 
{
  set(value,0);//TODO shift through sub bytes
}
*/


