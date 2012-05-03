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

object *fmod_playSound(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //filename,mode
	object *self = GetItem((object*)locals,0);
	object *filename = GetItem((object*)locals,1);
	object *mode = GetItem((object*)locals,2);
	unicode_object *sys_name = CreateUnicodeObject(str_Copy("__fmod_sys__"));
	object *sys = GetAttribute(self,sys_name);
	gc_IncRefCount(sys_name);
	gc_DecRefCount(sys_name);
	//printf("playSound\n");
	//TODO only a temp solution
	if(filename->type == TYPE_KV)
		filename = (object*)((kv_object*)filename)->value;
	if(mode->type == TYPE_KV)
		mode = (object*)((kv_object*)mode)->value;
	//if(filename->type == TYPE_UNICODE && mode->type == TYPE_INT)
	//	fmod_play_sound(((unicode_object*)filename)->value,((int_object*)mode)->value);
	if(filename->type == TYPE_UNICODE)
		fmod_play_sound(((tag_object*)sys)->tag,((unicode_object*)filename)->value,0);
	//printf("playSound: %s ,mode: %d\n",((unicode_object*)filename)->value,((int_object*)mode)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *fmod_Sleep(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	//printf("sleep\n");
	object *self = GetItem((object*)locals,0);
	object *delay = GetItem((object*)locals,1);
	if(delay->type == TYPE_KV)
		delay = (object*)((kv_object*)delay)->value;
	if(delay->type == TYPE_INT)
	{
		printf("sleeping:%d \n",((int_object*)delay)->value);
		Sleep(((int_object*)delay)->value);
	}
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

void AddFmodGlobals(vm *vm)
{
	code_object *a_globals = AllocCodeObject();
	a_globals->type = TYPE_CODE;
	a_globals->ref_count = 0;
	a_globals->name = str_Copy("fmod");
	a_globals->argcount = 0;
	a_globals->kwonlyargcount = 0;
	a_globals->nlocals = 0;
	a_globals->stacksize = 0;
	a_globals->co_flags = 0;
	a_globals->code = NULL;
	a_globals->consts = NULL;
	a_globals->varnames = NULL;
	a_globals->freevars = NULL;
	a_globals->cellvars = NULL;

	a_globals->names = (object*)CreateTuple(4);
	gc_IncRefCount(a_globals->names);


	cfunction_object *sleep_cfo = CreateCFunctionObject(&fmod_Sleep,NULL,NULL);
	unicode_object *sleep = CreateUnicodeObject(str_Copy("Sleep"));
	kv_object *kvsleep = CreateKVObject((object*)sleep,(object*) sleep_cfo);
	SetItem(a_globals->names,0,(object*)kvsleep);

	cfunction_object *play_cfo = CreateCFunctionObject(&fmod_playSound,NULL,NULL);
	unicode_object *play = CreateUnicodeObject(str_Copy("playSound"));
	kv_object *kvplay = CreateKVObject((object*)play,(object*) play_cfo);
	SetItem(a_globals->names,1,(object*)kvplay);

	cfunction_object *init_cfo = CreateCFunctionObject(&fmod_Init,NULL,NULL);
	unicode_object *init = CreateUnicodeObject(str_Copy("__init__"));
	kv_object *kvinit = CreateKVObject((object*)init,(object*) init_cfo);
	SetItem(a_globals->names,2,(object*)kvinit);

	cfunction_object *del_cfo = CreateCFunctionObject(&fmod_Close,NULL,NULL);
	unicode_object *del = CreateUnicodeObject(str_Copy("__del__"));
	kv_object *kvdel = CreateKVObject((object*)del,(object*) del_cfo);
	SetItem(a_globals->names,3,(object*)kvdel);

	class_object *fmod_global = AllocClassObject();
	fmod_global->type = TYPE_CLASS;
	fmod_global->name = str_Copy("fmod");
	fmod_global->base_classes = NULL;
	fmod_global->code = a_globals;
	gc_IncRefCount(a_globals);
	fmod_global->code->co_flags ^= CO_CLASS_ROOT;
	fmod_global->ref_count = 0;
	vm_AddClass(vm,fmod_global);
}

void fmod_Init(vm *vm,tuple_object *locals,tuple_object *kw_locals)
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


void fmod_Close(vm *vm,tuple_object *locals,tuple_object *kw_locals)
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
