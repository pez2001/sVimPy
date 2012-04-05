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

FMOD_SYSTEM *fmod_sys;

void fmod_play_sound(char *filename,int mode)
{
    FMOD_SOUND *sound;
    FMOD_CHANNEL *channel = 0;
    FMOD_RESULT result;


	printf("playing sound:%s\n",filename);
 
    result = FMOD_System_CreateSound(fmod_sys, filename, FMOD_HARDWARE, 0, &sound);
    ERRCHECK(result);

    result = FMOD_Sound_SetMode(sound, FMOD_LOOP_OFF); /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
    ERRCHECK(result);                                   /* so turn it off here.  We could have also just put FMOD_LOOP_OFF in the above CreateSound call. */
    result = FMOD_System_PlaySound(fmod_sys, FMOD_CHANNEL_FREE, sound, 0, &channel);
	/*for(;;)
	{
		printf("playing\n");
		FMOD_System_Update(system);
	}*/
 
    //result = FMOD_Sound_Release(sound1);
    //ERRCHECK(result);
    return 0;
}

object *fmod_playSound(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //filename,mode
	object *filename = GetItem((object*)locals,0);
	object *mode = GetItem((object*)locals,1);
	//printf("playSound\n");
	//TODO only a temp solution
	if(filename->type == TYPE_KV)
		filename = (object*)((kv_object*)filename)->value;
	if(mode->type == TYPE_KV)
		mode = (object*)((kv_object*)mode)->value;
	//if(filename->type == TYPE_UNICODE && mode->type == TYPE_INT)
	//	fmod_play_sound(((unicode_object*)filename)->value,((int_object*)mode)->value);
	if(filename->type == TYPE_UNICODE)
		fmod_play_sound(((unicode_object*)filename)->value,0);
	//printf("playSound: %s ,mode: %d\n",((unicode_object*)filename)->value,((int_object*)mode)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *fmod_Sleep(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	printf("sleep\n");
	object *delay = GetItem((object*)locals,0);
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
void AddFmodFunctions(vm *vm)
{
	cfunction *fmod_play_sound = CreateCFunction(&fmod_playSound, "playSound");
	vm_AddFunction(vm, fmod_play_sound);
	cfunction *fmod_sleep = CreateCFunction(&fmod_Sleep, "Sleep");
	vm_AddFunction(vm, fmod_sleep);
	//cfunction *a_digital_read = CreateCFunction(&a_digitalRead, "digitalRead");
	//vm_AddFunction(vm, a_digital_read);
}

void AddFmodGlobals(vm *vm)
{
/*
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
	
	vm_AddGlobal(vm,a_globals);*/
	//class_object *fmod_global = AllocClassObject();
	//fmod_global->name = str_Copy("fmod");
	//fmod_global->base_classes = ;
	//fmod_global->code = ;
	//fmod_global->names = ;
	//fmod_global->
	//fmod_global->
	//fmod_global->

	code_object *a_globals = AllocCodeObject();
	a_globals->type = TYPE_CODE;
	a_globals->ref_count = 1;
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
	a_globals->names = NULL;
	vm_AddGlobal(vm,a_globals);
}

void fmod_Init(vm *vm)
{
	unsigned int version;
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

	AddFmodFunctions(vm);
}

void fmod_Close(void)
{
	FMOD_RESULT result;
	result = FMOD_System_Close(fmod_sys);
	ERRCHECK(result);
	result = FMOD_System_Release(fmod_sys);
	ERRCHECK(result);
}

#endif
