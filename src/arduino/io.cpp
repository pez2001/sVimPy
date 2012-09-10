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

OBJECT_ID a_pinMode(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
 //pin,mode
	if(tuple_GetLen(locals) < 2)
	{
		OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
		return(tmp);	
	}
	OBJECT_ID pin = tuple_GetItem(locals,0);
	OBJECT_ID mode = tuple_GetItem(locals,1);
	if(obj_GetType(pin) == TYPE_KV)
	{
		kv_object *kpin = (kv_object*)mem_lock(pin);
		OBJECT_ID old = pin;
		pin = kpin->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(mode) == TYPE_KV)
	{
		kv_object *kmode = (kv_object*)mem_lock(mode);
		OBJECT_ID old = mode;
		mode = kmode->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(pin) == TYPE_INT && obj_GetType(mode) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		int_object *imode = (int_object*)mem_lock(mode);
		printf("pinMode: %d to %d\n",ipin->value,imode->value);
		pinMode(ipin->value,imode->value);
		mem_unlock(mode,0);
		mem_unlock(pin,0);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);	
}

OBJECT_ID a_digitalRead(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID pin = tuple_GetItem(locals,0);
	int r = 0;
	if(obj_GetType(pin) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		printf("digitalRead: %d\n",ipin->value);
		r = digitalRead(ipin->value);
		mem_unlock(pin,0);
	}
	INT_ID tmp = obj_CreateInt(r);
	return(tmp);	
}

OBJECT_ID a_digitalWrite(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
 	if(tuple_GetLen(locals) < 2)
	{
		OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
		return(tmp);	
	}
	OBJECT_ID pin = tuple_GetItem(locals,0);
	OBJECT_ID val = tuple_GetItem(locals,1);
	if(obj_GetType(pin) == TYPE_KV)
	{
		kv_object *kpin = (kv_object*)mem_lock(pin);
		OBJECT_ID old = pin;
		pin = kpin->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(val) == TYPE_KV)
	{
		kv_object *kval = (kv_object*)mem_lock(val);
		OBJECT_ID old = val;
		val = kval->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(pin) == TYPE_INT && obj_GetType(val) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		int_object *ival = (int_object*)mem_lock(val);
		printf("digitalWrite pin: %d val: %d\n",ipin->value,ival->value);
		digitalWrite(ipin->value,ival->value);
		mem_unlock(val,0);
		mem_unlock(pin,0);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);	
}

OBJECT_ID a_analogRead(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
 	OBJECT_ID pin = tuple_GetItem(locals,0);
	int r = 0;
	if(obj_GetType(pin) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		printf("analogRead: %d\n",ipin->value);
		r = analogRead(ipin->value);
		mem_unlock(pin,0);
	}
	INT_ID tmp = obj_CreateInt(r);
	return(tmp);	
}

OBJECT_ID a_analogWrite(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID pin = tuple_GetItem(locals,0);
	OBJECT_ID val = tuple_GetItem(locals,1);
	if(obj_GetType(pin) == TYPE_KV)
	{
		kv_object *kpin = (kv_object*)mem_lock(pin);
		OBJECT_ID old = pin;
		pin = kpin->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(val) == TYPE_KV)
	{
		kv_object *kval = (kv_object*)mem_lock(val);
		OBJECT_ID old = val;
		val = kval->value;
		mem_unlock(old,0);
		//printf("kv'd value\n");
	}
	if(obj_GetType(pin) == TYPE_INT && obj_GetType(val) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		int_object *ival = (int_object*)mem_lock(val);
		//obj_Dump(val,1,1);
		//obj_Print(val);
		printf("analogWrite pin: %d, value: %d\n",ipin->value,ival->value);
		//printf("value: %d\n",ival->value);
		analogWrite(ipin->value,ival->value);
		mem_unlock(val,0);
		mem_unlock(pin,0);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);	
}

OBJECT_ID a_delay(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
 	OBJECT_ID ms_id = tuple_GetItem(locals,0);
	int_object *ms = (int_object*)mem_lock(ms_id);
	printf("delay: %d\n",ms->value);
	//TODO add delay here
	delay(ms->value);
	mem_unlock(ms_id,0);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);	
}
/*
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
*/
void AddArduinoGlobal(VM_ID vm)
{
	CODE_ID a_globals = obj_CreateCode(mem_create_string("arduino"));
	//printf("created a_global\r\n");
	obj_AddCodeCFunction(a_globals,mem_create_string("pinMode"),&a_pinMode);
	//printf("added pinMode\r\n");
	obj_AddCodeCFunction(a_globals,mem_create_string("digitalRead"),&a_digitalRead);
	obj_AddCodeCFunction(a_globals,mem_create_string("digitalWrite"),&a_digitalWrite);
	obj_AddCodeCFunction(a_globals,mem_create_string("analogRead"),&a_analogRead);
	obj_AddCodeCFunction(a_globals,mem_create_string("analogWrite"),&a_analogWrite);
	obj_AddCodeCFunction(a_globals,mem_create_string("delay"),&a_delay);
	//obj_AddCodeCFunction(a_globals,mem_create_string("Serial.print"),&a_serialprint);
	//obj_AddCodeCFunction(a_globals,mem_create_string("Serial.Begin"),&a_serialBegin);
	obj_AddCodeName(a_globals,obj_CreateUnicode(mem_create_string("INPUT")),obj_CreateInt(0));
	obj_AddCodeName(a_globals,obj_CreateUnicode(mem_create_string("OUTPUT")),obj_CreateInt(1));
	obj_AddCodeName(a_globals,obj_CreateUnicode(mem_create_string("LOW")),obj_CreateInt(0));
	obj_AddCodeName(a_globals,obj_CreateUnicode(mem_create_string("HIGH")),obj_CreateInt(1));
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("arduino")),a_globals);
}


#endif
