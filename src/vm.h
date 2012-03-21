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

import
from
nonlocal
as
yield			supported
assert
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

with
try
raise
class
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
#include "garbage.h"
#include "iterators.h"
#include "internal_functions.h"

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

//function types
#define FUNC_PYTHON 'p'
#define FUNC_C 'c'
#define FUNC_C_OBJ 'o'
#define FUNC_NOT_FOUND 'n'


#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */
#endif

typedef struct _vm
{
	stack *blocks;
	ptr_list *functions;
	//ptr_list *garbage;
	//code_object *global;
	ptr_list *globals;
	object *(*interrupt_handler) (struct _vm *vm,stack *stack);
	BOOL interrupt_vm;
	BOOL running;
	object *(*step_handler) (struct _vm *vm);//TODO implement step handler to execute external work functions
} vm;

typedef struct _cfunction
{
	char *name;//used quickly find functions by name
	struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals);
} cfunction;


typedef	union _resolve_function
{
	struct _function_object *fo;
	struct _cfunction *cfo;
} resolve_function;

typedef struct _resolve_container
{
	OBJECT_TYPE func_type;
	union _resolve_function func;
} resolve_container;

#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(pop)				/* restore original alignment from stack */
#endif

//extern long mem_chunks_top;

resolve_container *AllocResolveContainer(void);

void FreeResolveContainer(resolve_container *rc);

cfunction *AllocCFunction(void);

void FreeCFunction(cfunction *cf);

BOOL vm_ObjectExists(vm *vm, object  *obj);

cfunction *CreateCFunction(object *(*func) (vm *vm,tuple_object *locals,tuple_object *kw_locals), char *name);//TODO add keyword parameters

//function_object *CreateCObjFunction(object *(*func) (vm *vm,object *obj),	char *name);

//function_object *CreatePythonFunction(code_object *code);

int vm_AddFunction(vm *vm, cfunction *fo);

void vm_RemoveFunctionByName(vm *vm, char *name);

void vm_RemoveFunction(vm *vm, cfunction *fo);

object *vm_ExecuteCFunctionByName(vm *vm, char *name, tuple_object *locals,tuple_object *kw_locals);

object *vm_ExecuteCFunction(vm *vm, cfunction *cf, tuple_object *locals,tuple_object *kw_locals);

//object *vm_ExecuteCObjFunction(vm *vm, char *name, object *obj);

cfunction *vm_FindFunction(vm *vm, char *name);

vm *vm_Init(code_object *co);//init vm and set global object if given

void vm_Close(vm *vm);//close vm and free all of its used memory

void vm_FreeGlobals(vm *vm);

void vm_AddGlobal(vm *vm, code_object * co);//add a global code object

void vm_RemoveGlobal(vm *vm, code_object *co);//remove a global

void vm_SetInterrupt(vm*vm,object *(*interrupt_func) (struct _vm *vm,stack *stack)); //set interrupt handler function

void vm_Interrupt(vm *vm, object *(*interrupt_func) (struct _vm *vm,stack *stack)); //interrupt vm and call interrupt handler afterwards

void vm_Continue(vm *vm);//continue vm execution

void vm_Exit(vm *vm); //exit vm

void vm_Stop(vm *vm); //pause vm execution

//object *vm_CallFunction(vm *vm,char *name,stack *locals,NUM argc);//call a python function from C

object *vm_CallFunction(vm *vm,char *name, tuple_object *locals);//call a python function from C

object *vm_RunPYC(vm *vm,stream *f,BOOL free_object);

//object *vm_RunObject(vm *vm, object *obj, stack *locals, NUM argc);//run a python code object

//object *vm_InteractiveRunObject(vm *vm, object *obj, stack *locals, NUM argc);

//block_object *vm_StartCodeObject(vm *vm,code_object *co,stack *locals,NUM argc);

object *vm_RunObject(vm *vm, object *obj, tuple_object *locals);//run a python code object

object *vm_InteractiveRunObject(vm *vm, object *obj,  tuple_object *locals);

block_object *vm_StartCodeObject(vm *vm,code_object *co, tuple_object *locals);

//object *vm_StartFunctionObject(vm *vm,function_object *fo,stack *locals,stack *kw_locals,NUM argc,NUM kw_argc);//run a python function object 

//object *vm_StartCFunction(vm *vm,cfunction *cfo,stack *locals,stack *kw_locals,NUM argc,NUM kw_argc);

object *vm_StartFunctionObject(vm *vm,function_object *fo,tuple_object *locals,tuple_object *kw_locals);//run a python function object 

object *vm_StartCFunction(vm *vm,cfunction *cfo,tuple_object *locals,tuple_object *kw_locals);

resolve_container *vm_ResolveFunction(vm *vm,object *to_resolve);//input can be function_objects ,code_objects, unicode_objects -> returns a function_object if any

object *vm_Step(vm *vm);//single step vm //TODO rename to vm_Step

#ifdef USE_DEBUGGING
void vm_DumpCode(vm *vm,BOOL dump_descriptions,BOOL from_start);//dump human readable code of the vm's actual running block 
#endif

#ifdef __cplusplus
} 
#endif


#endif
