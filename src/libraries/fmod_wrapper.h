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

#ifndef FMOD_WRAPPER_H
#define FMOD_WRAPPER_H

#include "../features.h"

#ifdef USE_FMOD_FUNCTIONS

//#include "fmod/fmod_codec.h"
//#include "fmod/fmod_dsp.h"
//#include "fmod/fmod_memoryinfo.h"
//#include "fmod/fmod_output.h"
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"
//#include <windows.h>
void __stdcall Sleep(unsigned long);
//#include <unistd.h>
//int usleep(unsigned long useconds) __THROW;
//unsigned int sleep(unsigned int seconds) __THROW;
//void WINAPI Sleep( __in  long dwMilliseconds);

#include "../types.h"
#include "../object.h"
#include "../stack.h"
#include "../vm.h"
#include "../debug.h"


object *fmod_playSound(vm *vm,tuple_object *locals,tuple_object *kw_locals); //(filename,mode); - returns id object used in other functions
object *fmod_Sleep(vm *vm,tuple_object *locals,tuple_object *kw_locals);
void fmod_Init(vm *vm,tuple_object *locals,tuple_object *kw_locals);
void fmod_Close(vm *vm,tuple_object *locals,tuple_object *kw_locals);

void AddFmodGlobals(vm *vm);



#endif
#endif
