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
		val = (object*) ((kv_object*)val)->value;
		//printf("pt:%c,vt:%c\r\n",pin->type,val->type);
		//printf("val:%d\r\n",((int_object*)val)->value);
	}
	if((pin->type == TYPE_INT) && (val->type == TYPE_INT))
	{
		//printf("dw:\r\n");
		//printf("pin:%d\r\n",((int_object*)pin)->value);
		//printf("val:%d\r\n",((int_object*)val)->value);
		digitalWrite(((int_object*)pin)->value,((int_object*)val)->value);
		//pinMode(9,OUTPUT);
		//digitalWrite(9,((int_object*)val)->value);
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
	if(ms->type == TYPE_KV)
	{
		ms = (object*) ((kv_object*)ms)->value;
		//printf("pt:%c,vt:%c\r\n",pin->type,val->type);
		//printf("val:%d\r\n",((int_object*)val)->value);
	}
	if(ms->type == TYPE_INT)
	{
		printf("delay:%d\r\n",((int_object*)ms)->value);
		delay(((int_object*)ms)->value);
	}
	//delay(8);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
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
void AddArduinoGlobal(vm *vm)
{
	code_object *a_globals = CreateCodeObject(str_Copy("arduino"));
	AddCodeCFunction((object*)a_globals,"pinMode",&a_pinMode);
	AddCodeCFunction((object*)a_globals,"digitalRead",&a_digitalRead);
	AddCodeCFunction((object*)a_globals,"digitalWrite",&a_digitalWrite);
	AddCodeCFunction((object*)a_globals,"analogRead",&a_analogRead);
	AddCodeCFunction((object*)a_globals,"analogWrite",&a_analogWrite);
	AddCodeCFunction((object*)a_globals,"delay",&a_delay);
	//AddCodeCFunction((object*)a_globals,"Serial.print",&a_serialprint);
	//AddCodeCFunction((object*)a_globals,"Serial.Begin",&a_serialBegin);
	AddCodeName((object*)a_globals,(object*)CreateUnicodeObject(str_Copy("INPUT")),(object*)CreateIntObject(0));
	AddCodeName((object*)a_globals,(object*)CreateUnicodeObject(str_Copy("OUTPUT")),(object*)CreateIntObject(1));
	AddCodeName((object*)a_globals,(object*)CreateUnicodeObject(str_Copy("LOW")),(object*)CreateIntObject(0));
	AddCodeName((object*)a_globals,(object*)CreateUnicodeObject(str_Copy("HIGH")),(object*)CreateIntObject(1));
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("arduino")),(object*)a_globals);
}

#endif
