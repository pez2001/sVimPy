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
extern int debug_level;


typedef struct
{
	//stack *recycle;
	stack *blocks;
	ptr_list *functions;
	ptr_list *garbage;
	code_object *global;
	// code_object *co;//global code object //Bottom of blocks stack is the
	// global module
	// int ip;//instruction pointer moved to block object
} vm;

#define FUNC_PYTHON 1
#define FUNC_C 2
#define FUNC_C_OBJ 4

typedef struct
{
	unsigned int type;
	union func_def
	{
		object *(*func) (vm *vm,stack * stack);
		object *(*func_obj) (vm *vm,object * object);
		code_object *code;
	} func;
	char *name;
} function_definition;



function_definition *CreateCFunction(object *(*func) (vm *vm,stack *stack), char *name);

function_definition *CreateCObjFunction(object *(*func) (vm *vm,object *obj),	char *name);

function_definition *CreatePythonFunction(object *code, char *name);

int vm_AddFunctionDefinition(vm *vm, function_definition *fd);

void vm_RemoveFunction(vm *vm, char *name);

void vm_RemoveFunctionDefinition(vm *vm, function_definition *fd);

object *vm_ExecuteCFunction(vm *vm, char *name, stack *stack);

object *vm_ExecuteCObjFunction(vm *vm, char *name, object *obj);

function_definition *vm_FindFunction(vm *vm, char *name);

vm *vm_Init(code_object *co);//init vm and set global object if given

void vm_Close(vm *vm);//close vm and free all of its used memory

void vm_SetGlobal(vm *vm, code_object * co);//set global code object

object *vm_RunObject(vm *vm, object *obj, stack *locals, int argc);//run a python code object

block_object *vm_StartObject(vm *vm,object *obj,stack *locals,int argc);//run a python code object

block_object *vm_StartFunctionObject(vm *vm,object *obj,stack *locals,int argc);//run a python function object

object *vm_CallFunction(vm *vm,char *name,stack *locals);//call a python function from C

object *vm_StepObject(vm *vm);//single step vm

void vm_DumpCode(vm *vm,int dump_descriptions,int from_start);//dump human readable code of the vm's actual running block 

#endif
