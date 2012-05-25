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

#ifndef VM_H
#define VM_H

/* keywords

import		supported
from			supported
nonlocal
as
yield			supported
assert		supported
lambda		not tested
elif			not tested
False		supported
return		supported
None		supported
continue	supported
for			supported
True			supported
def			supported
while			supported
and			supported
del			supported
global		supported
not			supported
if				supported
or				supported
else			supported
pass			supported
break		supported
in				supported
is				supported

//classes and exceptions

with		supported
try
raise
class	supported
finally
except

*/
#include "features.h"
#include "types.h"
#include "object.h"
#include "opcodes.h"
#include "stack.h"
#include "numops.h"
#include "lists.h"
#include "stream.h"
#include "iterators.h"
#include "internal_functions.h"
#include "internal_classes.h"

#include "garbage.h"

#include "debug.h"

#ifdef USE_DEBUGGING
#include "assert.h"
#endif

#if defined(USE_DEBUGGING) || defined(USE_ARDUINO_OPCODE_DEBUGGING)
extern const opcode opcodes[];
#endif 

#ifdef USE_ARDUINO_DEBUGGING
#include "avr/pgmspace.h"
#endif

#ifdef __cplusplus
extern "C"  {
#endif

#define MAGIC (3180 | ((long)'\r'<<16) | ((long)'\n'<<24))


#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */
#endif

typedef struct _vm
{
	stack *blocks; //execution frame stack
	tuple_object *globals;//dictionary of globals
	object *(*interrupt_handler) (struct _vm *vm,stack *stack);
	object *(*import_module_handler) (struct _vm *vm,char *module_name);
	object *(*exception_handler) (struct _vm *vm,struct _object *exception);
	object *(*step_handler) (struct _vm *vm);//TODO implement step handler to execute external work functions
	//#ifndef USE_ARDUINO_FUNCTIONS
	NUM error_code;
	char *error_message;
	//#endif
	BOOL interrupt_vm;
	BOOL running;
} vm;

#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(pop)				/* restore original alignment from stack */
#endif

//BOOL vm_ObjectExists(vm *vm, object  *obj);

#ifdef USE_DEBUGGING
void vm_DumpStackTree(vm *vm);
#endif

object *vm_ExecuteCFunctionByName(vm *vm, char *name, tuple_object *locals,tuple_object *kw_locals);

vm *vm_Init(code_object *co);//init vm and set global object if given

NUM vm_Close(vm *vm);//close vm and free all of its used memory

void vm_FreeGlobals(vm *vm);

void vm_AddGlobal(vm *vm,object *key,object *global);//add a global object

void vm_RemoveGlobal(vm *vm, object *key);//remove a global object

object *vm_GetGlobal(vm *vm, object *key);//retrieve a global object

object *vm_GetGlobalByIndex(vm *vm, INDEX i);

void vm_SetInterrupt(vm*vm,object *(*interrupt_func) (struct _vm *vm,stack *stack)); //set interrupt handler function

void vm_Interrupt(vm *vm, object *(*interrupt_func) (struct _vm *vm,stack *stack)); //interrupt vm and call interrupt handler afterwards

void vm_Continue(vm *vm);//continue vm execution

void vm_Exit(vm *vm,char *message,NUM code); //exit vm

void vm_Stop(vm *vm); //pause vm execution

object *vm_Step(vm *vm);//single step vm //TODO rename to vm_Step

object *vm_RunPYC(vm *vm,stream *f,BOOL free_object);

object *vm_RunRPYC(vm *vm,stream *f ,BOOL free_object);

object *vm_RunRPYCPlus(vm *vm,stream *f ,BOOL free_object);

object *vm_RunFunction(vm *vm,char *name, tuple_object *locals,tuple_object *kw_locals);//call a python function from C

object *vm_RunObject(vm *vm, object *obj, tuple_object *locals,tuple_object *kw_locals);//run a python object if possible

#ifdef USE_DEBUGGING
object *vm_InteractiveRunObject(vm *vm, object *obj,  tuple_object *locals,tuple_object *kw_locals);
#endif

object *vm_StartMethod(vm *vm,object *key,class_instance_object *cio,tuple_object *locals,tuple_object *kw_locals);

object *vm_RunMethod(vm *vm,object *key,class_instance_object *cio,tuple_object *locals,tuple_object *kw_locals);

object *vm_StartObject(vm *vm,object *obj, tuple_object *locals,tuple_object *kw_locals);

object *vm_StartObjectCopy(vm *vm,object *o, tuple_object *locals,tuple_object *kw_locals);

block_object *vm_StartCodeObject(vm *vm,code_object *co, tuple_object *locals,tuple_object *kw_locals);

block_object *vm_StartClassObject(vm *vm,class_object *co,tuple_object *locals,tuple_object *kw_locals);

object *vm_StartFunctionObject(vm *vm,function_object *fo,tuple_object *locals,tuple_object *kw_locals);//run a python function object 

object *vm_StartCFunctionObject(vm *vm,cfunction_object *cfo,tuple_object *locals,tuple_object *kw_locals);//run a c function object 

object *vm_StartMethodObject(vm *vm,method_object *mo,tuple_object *locals,tuple_object *kw_locals);

#ifdef USE_DEBUGGING
void vm_DumpCode(vm *vm,BOOL dump_descriptions,BOOL from_start);//dump human readable code of the vm's actual running block 
#endif

#ifdef __cplusplus
} 
#endif
#endif
