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
#include "fmod_wrapper.h"

#ifdef USE_FMOD_FUNCTIONS

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}



void fmod_play_sound(FMOD_SYSTEM *fmod_sys,char *filename,int mode)
{
    FMOD_SOUND *sound;
    FMOD_CHANNEL *channel = 0;
    FMOD_RESULT result;


	printf("playing sound:%s\n",filename);
 
    result = FMOD_System_CreateSound(fmod_sys, filename, FMOD_HARDWARE, 0, &sound);
    ERRCHECK(result);

    result = FMOD_Sound_SetMode(sound, FMOD_LOOP_OFF);
    ERRCHECK(result);                             
    result = FMOD_System_PlaySound(fmod_sys, FMOD_CHANNEL_FREE, sound, 0, &channel);
    //result = FMOD_Sound_Release(sound1);
    //ERRCHECK(result);
    return;
}

OBJECT_ID fmod_playSound(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	object *self = GetItem((object*)locals,0);
	object *filename = GetItem((object*)locals,1);
	object *mode = GetItem((object*)locals,2);
	unicode_object *sys_name = CreateUnicodeObject(str_Copy("__fmod_sys__"));
	object *sys = GetAttribute(self,(object*)sys_name);
	gc_IncRefCount((object*)sys_name);
	gc_DecRefCount((object*)sys_name);
	if(filename->type == TYPE_KV)
		filename = (object*)((kv_object*)filename)->value;
	if(mode->type == TYPE_KV)
		mode = (object*)((kv_object*)mode)->value;
	if(filename->type == TYPE_UNICODE)
		fmod_play_sound(((tag_object*)sys)->tag,((unicode_object*)filename)->value,0);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

OBJECT_ID mod_Sleep(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	object *delay = GetItem((object*)locals,1);
	if(delay->type == TYPE_KV)
		delay = (object*)((kv_object*)delay)->value;
	if(delay->type == TYPE_INT)
	{
		Sleep(((int_object*)delay)->value);
		//Sleep(5000);
	}
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

void AddFmodGlobals(VM_ID vm)
{
	code_object *a_globals = CreateCodeObject(str_Copy("fmod_code"));
	AddCodeCFunction((object*)a_globals,"Sleep",&fmod_Sleep);
	AddCodeCFunction((object*)a_globals,"playSound",&fmod_playSound);
	AddCodeCFunction((object*)a_globals,"__init__",&fmod_Init);
	AddCodeCFunction((object*)a_globals,"__del__",&fmod_Close);

	class_object *fmod_global = CreateClassObject(a_globals,NULL);
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("fmod")),fmod_global);
}

OBJECT_ID fmod_Init(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	object *self = GetItem((object*)locals,0);
	unsigned int version;
    FMOD_SYSTEM *fmod_sys;
	FMOD_RESULT result;
 	result = FMOD_System_Create(&fmod_sys);
	ERRCHECK(result);

	result = FMOD_System_GetVersion(fmod_sys, &version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return 0;
	}

	result = FMOD_System_Init(fmod_sys, 32, FMOD_INIT_NORMAL, NULL);
	ERRCHECK(result);

	unicode_object *sys = CreateUnicodeObject(str_Copy("__fmod_sys__"));
	tag_object *tag = CreateTagObject(fmod_sys);
	SetAttribute(self,sys,tag);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

OBJECT_ID fmod_Close(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	object *self = GetItem((object*)locals,0);
	unicode_object *sys_name = CreateUnicodeObject(str_Copy("__fmod_sys__"));
	object *sys = GetAttribute(self,sys_name);
	gc_IncRefCount(sys_name);
	gc_DecRefCount(sys_name);
	FMOD_RESULT result;
	result = FMOD_System_Close(((tag_object*)sys)->tag);
	ERRCHECK(result);
	result = FMOD_System_Release(((tag_object*)sys)->tag);
	ERRCHECK(result);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

#endif
