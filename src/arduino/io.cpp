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

object *a_pinMode(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin,mode
	object *pin = GetItem((object*)locals,0);
	object *mode = GetItem((object*)locals,1);
	//printf("pinMode\n");
	//TODO only a temp solution
	if(pin->type == TYPE_KV)
		pin = (object*)((kv_object*)pin)->value;
	if(mode->type == TYPE_KV)
		mode = (object*)((kv_object*)mode)->value;
	if(pin->type == TYPE_INT && mode->type == TYPE_INT)
		pinMode(((int_object*)pin)->value,((int_object*)mode)->value);
	//printf("pinMode: %d to %d\n",((int_object*)pin)->value,((int_object*)mode)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *a_digitalRead(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin
	//printf("digitalRead\n");
	object *pin = GetItem((object*)locals,0);
	int r = 0;
	if(pin->type == TYPE_INT)
		r = digitalRead(((int_object*)pin)->value);
	int_object *tmp = CreateIntObject(r);
	return ((object*)tmp);	
	
}

object *a_digitalWrite(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin,value
	if(locals->list->num < 2)
	{
		//printf("not enough args for digitalWrite();\r\n");
		object *tmp =CreateEmptyObject(TYPE_NONE);
		return (tmp);	
	}
	object *pin = GetItem((object*)locals,0);
	object *val = GetItem((object*)locals,1);
    //PrintObject(pin);
	//printf("\r\nval:");
	//PrintObject(val);
	//printf("\r\n");
	//printf("pt:%c,vt:%c\r\n",pin->type,val->type);
	//printf("digitalWrite\n");
	//TODO only a temp solution
	if(pin->type == TYPE_KV)
		pin =(object*) ((kv_object*)pin)->value;
	if(val->type == TYPE_KV)
	{
		val =(object*) ((kv_object*)val)->value;
		//printf("pt:%c,vt:%c\r\n",pin->type,val->type);
		//printf("val:%d\r\n",((int_object*)val)->value);
	}
	if((pin->type == TYPE_INT) && (val->type == TYPE_INT))
	{
		//printf("dw:\r\n");
		//printf("pin:%d\r\n",((int_object*)pin)->value);
		//printf("val:%d\r\n",((int_object*)val)->value);
		digitalWrite(((int_object*)pin)->value,((int_object*)val)->value);
	}
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *a_analogRead(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin
 	//printf("analogRead\n");
 	object *pin = GetItem((object*)locals,0);
	int r = 0;
	if(pin->type == TYPE_INT)
		r = analogRead(((int_object*)pin)->value);
	int_object *tmp =CreateIntObject(r);
	return ((object*)tmp);	
}

object *a_analogWrite(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin,value
 	//printf("analogWrite\n");
	object *pin = GetItem((object*)locals,0);
	object *value = GetItem((object*)locals,1);
	if(pin->type == TYPE_INT && value->type == TYPE_INT)
		analogWrite(((int_object*)pin)->value,((int_object*)value)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *a_delay(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //ms
 	//printf("delay\n");
 	object *ms = GetItem((object*)locals,0);
	//if(ms->type == TYPE_INT)
	//	printf("delay:%d\r\n",((int_object*)ms)->value);
	//	delay(((int_object*)ms)->value);
	//delay(8);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *a_serialprint(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //message
 	//printf("serial.Println\n");
 	object *message = GetItem((object*)locals,0);
	//if(message->type == TYPE_UNICODE)
	//	Serial.print(((unicode_object*)message)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *a_serialBegin(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //baudrate
 	//printf("serial.Begin\n");
 	object *baudrate = GetItem((object*)locals,0);
	//if(baudrate->type == TYPE_INT)
	//	Serial.begin(((int_object*)baudrate)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

void AddArduinoFunctions(vm *vm)
{
	cfunction *a_pin_mode = CreateCFunction(&a_pinMode, "pinMode");
	vm_AddFunction(vm, a_pin_mode);
	cfunction *a_digital_read = CreateCFunction(&a_digitalRead, "digitalRead");
	vm_AddFunction(vm, a_digital_read);
	cfunction *a_digital_write = CreateCFunction(&a_digitalWrite, "digitalWrite");
	vm_AddFunction(vm, a_digital_write);
	cfunction *a_analog_read = CreateCFunction(&a_analogRead, "analogRead");
	vm_AddFunction(vm, a_analog_read);
	cfunction *a_analog_write = CreateCFunction(&a_analogWrite, "analogWrite");
	vm_AddFunction(vm, a_analog_write);
	cfunction *a_delay_fd = CreateCFunction(&a_delay, "delay");
	vm_AddFunction(vm, a_delay_fd);
	//function_object *a_serial_print = CreateCFunction(&a_serialprint, "Serial.print");
	//vm_AddFunctionObject(vm, a_serial_print);
	//function_object *a_serial_begin = CreateCFunction(&a_serialBegin, "Serial.Begin");
	//vm_AddFunctionObject(vm, a_serial_begin);
	//function_object *a_ = CreateCFunction(&a_, "");
	//vm_AddFunctionObject(vm, a_);
}

void AddArduinoGlobals(vm *vm)
{
	code_object *a_globals = AllocCodeObject();
	a_globals->type = TYPE_CODE;
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
	a_globals->names = (object*)CreateTuple(4);

	int_object *vinput = CreateIntObject(0);
	int_object *voutput = CreateIntObject(1);
	int_object *vlow = CreateIntObject(0);
	int_object *vhigh = CreateIntObject(1);
	unicode_object *kinput = CreateUnicodeObject(str_Copy("INPUT"));
	unicode_object *koutput = CreateUnicodeObject(str_Copy("OUTPUT"));
	unicode_object *klow = CreateUnicodeObject(str_Copy("LOW"));
	unicode_object *khigh = CreateUnicodeObject(str_Copy("HIGH"));
	kv_object *kvinput = CreateKVObject((object*)kinput,(object*)vinput);
	kv_object *kvoutput = CreateKVObject((object*)koutput,(object*)voutput);
	kv_object *kvlow = CreateKVObject((object*)klow,(object*)vlow);
	kv_object *kvhigh = CreateKVObject((object*)khigh,(object*)vhigh);
	//kv_object *kvlow = CreateKVObject((object*)klow,(object*)vinput);
	//kv_object *kvhigh = CreateKVObject((object*)khigh,(object*)voutput);
	SetItem(a_globals->names,0,(object*)kvinput);
	SetItem(a_globals->names,1,(object*)kvoutput);
	SetItem(a_globals->names,2,(object*)kvlow);
	SetItem(a_globals->names,3,(object*)kvhigh);
	
	vm_AddGlobal(vm,a_globals);
}

#endif
