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


#ifndef ARDUINO_IO_H
#define ARDUINO_IO_H

#ifdef USE_ARDUINO_FUNCTIONS

//#extern "C" {
#include "../types.h"
#include "../object.h"
#include "../stack.h"
#include "../vm.h"
#include "../debug.h"
//}


#include "libarduino/arduino.h"

//#define INPUT 0
//#define OUTPUT 1
//#define LOW 0
//#define HIGH 1

object *a_pinMode(vm *vm,tuple_object *locals,tuple_object *kw_locals); //(pin,mode);
object *a_digitalRead(vm *vm,tuple_object *locals,tuple_object *kw_locals); //(pin);
object *a_digitalWrite(vm *vm,tuple_object *locals,tuple_object *kw_locals); //(pin,value);
object *a_analogRead(vm *vm,tuple_object *locals,tuple_object *kw_locals); //(pin,);
object *a_analogWrite(vm *vm,tuple_object *locals,tuple_object *kw_locals); //(pin,value);
object *a_delay(vm *vm,tuple_object *locals,tuple_object *kw_locals); //(ms);
object *a_serialprint(vm *vm,tuple_object *locals,tuple_object *kw_locals); //(message);
object *a_serialBegin(vm *vm,tuple_object *locals,tuple_object *kw_locals); //(baudrate);

void AddArduinoFunctions(vm *vm);
void AddArduinoGlobals(vm *vm);

#endif


#endif
