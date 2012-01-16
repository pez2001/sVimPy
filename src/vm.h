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
is
from
nonlocal
as
yield
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

//classes and exceptions

with
try
raise
class
finally
except

*/

#define DEBUG
#include "object.h"
#include "opcodes.h"
#include "stack.h"
#include "numops.h"
#include "lists.h"
#include "stream.h"
#include "garbage.h"

#include "debug.h"
#include "assert.h"

extern const opcode opcodes[];

#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */

typedef struct _vm
{
	//stack *recycle;
	stack *blocks;
	ptr_list *functions;
	ptr_list *garbage;
	code_object *global;
	// code_object *co;//global code object //Bottom of blocks stack is the
	// global module
	// int ip;//instruction pointer moved to block object
	object *(*interrupt_handler) (struct _vm *vm,stack *stack);
	unsigned char interrupt_vm;
	unsigned char running;
} vm;

#pragma pack(pop)				/* restore original alignment from stack */


function_object *CreateCFunction(object *(*func) (vm *vm,stack *stack), char *name);

function_object *CreateCObjFunction(object *(*func) (vm *vm,object *obj),	char *name);

function_object *CreatePythonFunction(code_object *code);

int vm_AddFunctionObject(vm *vm, function_object *fo);

void vm_RemoveFunction(vm *vm, char *name);

void vm_RemoveFunctionObject(vm *vm, function_object *fo);

object *vm_ExecuteCFunction(vm *vm, char *name, stack *stack);

object *vm_ExecuteCObjFunction(vm *vm, char *name, object *obj);

function_object *vm_FindFunction(vm *vm, char *name);

vm *vm_Init(code_object *co);//init vm and set global object if given

void vm_Close(vm *vm);//close vm and free all of its used memory

void vm_SetGlobal(vm *vm, code_object * co);//set global code object

void vm_SetInterrupt(vm*vm,object *(*interrupt_func) (struct _vm *vm,stack *stack)); //set interrupt handler function

void vm_Interrupt(vm *vm, object *(*interrupt_func) (struct _vm *vm,stack *stack)); //interrupt vm and call interrupt handler afterwards

void vm_Continue(vm *vm);//continue vm execution

void vm_Exit(vm *vm); //exit vm

void vm_Stop(vm *vm); //pause vm execution

object *vm_CallFunction(vm *vm,char *name,stack *locals);//call a python function from C

object *vm_RunObject(vm *vm, object *obj, stack *locals, int argc);//run a python code object

block_object *vm_StartObject(vm *vm,object *obj,stack *locals,int argc);//run a python code object

object *vm_StartFunctionObject(vm *vm,function_object *fo,stack *locals,int argc);//run a python function object

function_object *vm_ResolveFunction(vm *vm,object *to_resolve);//input can be function_objects ,code_objects, unicode_objects -> returns a function_object if any

object *vm_StepObject(vm *vm);//single step vm

void vm_DumpCode(vm *vm,int dump_descriptions,int from_start);//dump human readable code of the vm's actual running block 

#endif
