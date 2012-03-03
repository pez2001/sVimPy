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

#include "io.h"

#ifdef USE_ARDUINO_FUNCTIONS

object *a_pinMode(vm *vm,stack * stack)
{
 //pin,mode
	object *pin = stack_Pop(stack,vm->garbage);
	object *mode = stack_Pop(stack,vm->garbage);
	//printf("pinMode\n");
	//TODO only a temp solution
	if(pin->type == TYPE_KV)
		pin = (object*)((kv_object*)pin)->value;
	if(mode->type == TYPE_KV)
		mode = (object*)((kv_object*)mode)->value;
	if(pin->type == TYPE_INT && mode->type == TYPE_INT)
		pinMode(((int_object*)pin)->value,((int_object*)mode)->value);
	
	object *tmp =CreateEmptyObject(TYPE_NONE,0);
	return (tmp);	
}

object *a_digitalRead(vm *vm,stack * stack)
{
 //pin
	//printf("digitalRead\n");
	object *pin = stack_Pop(stack,vm->garbage);
	int r = 0;
	if(pin->type == TYPE_INT)
		r = digitalRead(((int_object*)pin)->value);
	int_object *tmp = CreateIntObject(r,0);
	return ((object*)tmp);	
	
}

object *a_digitalWrite(vm *vm,stack * stack)
{
 //pin,value
	object *pin = stack_Pop(stack,vm->garbage);
	object *value = stack_Pop(stack,vm->garbage);
    printf("pt:%c,vt:%c\r\n",pin->type,value->type);
	//TODO only a temp solution
	if(pin->type == TYPE_KV)
		pin = (object*)((kv_object*)pin)->value;
	if(value->type == TYPE_KV)
		value = (object*)((kv_object*)value)->value;
	if(pin->type == TYPE_INT && value->type == TYPE_INT)
	{
		printf("digitalWrite:%d,val:%d\r\n",((int_object*)pin)->value,((int_object*)value)->value);
		digitalWrite(((int_object*)pin)->value,((int_object*)value)->value);
	}
	object *tmp =CreateEmptyObject(TYPE_NONE,0);
	return (tmp);	
}

object *a_analogRead(vm *vm,stack * stack)
{
 //pin
 	//printf("analogRead\n");
 	object *pin = stack_Pop(stack,vm->garbage);
	int r = 0;
	if(pin->type == TYPE_INT)
		r = analogRead(((int_object*)pin)->value);
	int_object *tmp =CreateIntObject(r,0);
	return ((object*)tmp);	
}

object *a_analogWrite(vm *vm,stack * stack)
{
 //pin,value
 	//printf("analogWrite\n");
	object *pin = stack_Pop(stack,vm->garbage);
	object *value = stack_Pop(stack,vm->garbage);
	if(pin->type == TYPE_INT && value->type == TYPE_INT)
		analogWrite(((int_object*)pin)->value,((int_object*)value)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE,0);
	return (tmp);	
}

object *a_delay(vm *vm,stack * stack)
{
 //ms
 	//printf("delay\n");
 	object *ms = stack_Pop(stack,vm->garbage);
	if(ms->type == TYPE_INT)
		delay(((int_object*)ms)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE,0);
	return (tmp);	
}

object *a_serialprint(vm *vm,stack * stack)
{
 //message
 	//printf("serial.Println\n");
 	object *message = stack_Pop(stack,vm->garbage);
	if(message->type == TYPE_UNICODE)
		Serial.print(((unicode_object*)message)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE,0);
	return (tmp);	
}

object *a_serialBegin(vm *vm,stack * stack)
{
 //baudrate
 	//printf("serial.Begin\n");
 	object *baudrate = stack_Pop(stack,vm->garbage);
	if(baudrate->type == TYPE_INT)
		Serial.begin(((int_object*)baudrate)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE,0);
	return (tmp);	
}

void AddArduinoFunctions(vm *vm)
{
	function_object *a_pin_mode = CreateCFunction(&a_pinMode, "pinMode");
	vm_AddFunctionObject(vm, a_pin_mode);
	function_object *a_digital_read = CreateCFunction(&a_digitalRead, "digitalRead");
	vm_AddFunctionObject(vm, a_digital_read);
	function_object *a_digital_write = CreateCFunction(&a_digitalWrite, "digitalWrite");
	vm_AddFunctionObject(vm, a_digital_write);
	function_object *a_analog_read = CreateCFunction(&a_analogRead, "analogRead");
	vm_AddFunctionObject(vm, a_analog_read);
	function_object *a_analog_write = CreateCFunction(&a_analogWrite, "analogWrite");
	vm_AddFunctionObject(vm, a_analog_write);
	function_object *a_delay_fd = CreateCFunction(&a_delay, "delay");
	vm_AddFunctionObject(vm, a_delay_fd);
	function_object *a_serial_print = CreateCFunction(&a_serialprint, "Serial.print");
	vm_AddFunctionObject(vm, a_serial_print);
	function_object *a_serial_begin = CreateCFunction(&a_serialBegin, "Serial.Begin");
	vm_AddFunctionObject(vm, a_serial_begin);
	//function_object *a_ = CreateCFunction(&a_, "");
	//vm_AddFunctionObject(vm, a_);
}

void AddArduinoGlobals(vm *vm)
{
	code_object *a_globals = AllocCodeObject();
	a_globals->name = str_Copy("Arduino");
	a_globals->argcount = 0;
	a_globals->kwonlyargcount = 0;
	a_globals->nlocals = 4;
	a_globals->stacksize = 0;
	a_globals->co_flags = 0;
	a_globals->code = NULL;
	a_globals->consts = NULL;
	a_globals->varnames = NULL;
	a_globals->freevars = NULL;
	a_globals->cellvars = NULL;
	a_globals->names = (object*)CreateTuple(4,0);

	int_object *vinput = CreateIntObject(0,0);
	int_object *voutput = CreateIntObject(1,0);
	int_object *vlow = CreateIntObject(0,0);
	int_object *vhigh = CreateIntObject(1,0);
	unicode_object *kinput = CreateUnicodeObject(str_Copy("INPUT"),0);
	unicode_object *koutput = CreateUnicodeObject(str_Copy("OUTPUT"),0);
	unicode_object *klow = CreateUnicodeObject(str_Copy("LOW"),0);
	unicode_object *khigh = CreateUnicodeObject(str_Copy("HIGH"),0);
	kv_object *kvinput = CreateKVObject((object*)kinput,(object*)vinput,0);
	kv_object *kvoutput = CreateKVObject((object*)koutput,(object*)voutput,0);
	kv_object *kvlow = CreateKVObject((object*)klow,(object*)vlow,0);
	kv_object *kvhigh = CreateKVObject((object*)khigh,(object*)vhigh,0);
	SetItem(a_globals->names,0,(object*)kvinput);
	SetItem(a_globals->names,1,(object*)kvoutput);
	SetItem(a_globals->names,2,(object*)kvlow);
	SetItem(a_globals->names,3,(object*)kvhigh);
	
	vm_AddGlobal(vm,a_globals);
}

#endif
