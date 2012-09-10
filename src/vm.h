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
#include "memory.h"
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
	STACK_ID blocks; //execution frame stack
	TUPLE_ID globals;//dictionary of globals
	OBJECT_ID(*interrupt_handler) (VM_ID vm_id,STACK_ID stack);
	OBJECT_ID(*import_module_handler) (VM_ID vm_id,BYTES_ID module_name);
	OBJECT_ID(*exception_handler) (VM_ID vm_id,OBJECT_ID exception);
	OBJECT_ID(*step_handler) (VM_ID vm_id);//TODO implement step handler to execute external work functions
	//#ifndef USE_ARDUINO_FUNCTIONS
	NUM error_code;
	BYTES_ID error_message;
	//#endif
	BOOL interrupt_vm;
	BOOL running;
} vm;

#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(pop)				/* restore original alignment from stack */
#endif

//BOOL vm_ObjectExists(vm *vm, object  *obj);

//#ifdef USE_DEBUGGING
//void vm_DumpStackTree(VM_ID vm_id);
//#endif

OBJECT_ID vm_ExecuteCFunctionByName(VM_ID vm_id, BYTES_ID name, TUPLE_ID locals_id,TUPLE_ID kw_locals_id);

VM_ID vm_Init(CODE_ID co_id);//init vm and set global object if given

NUM vm_Close(VM_ID vm_id);//close vm and free all of its used memory

void vm_SetImportModuleHandler(VM_ID vm_id, OBJECT_ID(*import_module_handler) (VM_ID vm_id,BYTES_ID module_name));

void vm_SetExceptionHandler(VM_ID vm_id, OBJECT_ID(*exception_handler) (VM_ID vm_id,OBJECT_ID exception));

void vm_SetInterruptHandler(VM_ID vm_id, OBJECT_ID(*interrupt_handler) (VM_ID vm_id,STACK_ID stack));

void vm_SetStepHandler(VM_ID vm_id, OBJECT_ID(*step_handler) (VM_ID vm_id));

void vm_FreeGlobals(VM_ID vm_id);

void vm_AddGlobal(VM_ID vm_id,OBJECT_ID key,OBJECT_ID global);//add a global object

void vm_RemoveGlobal(VM_ID vm_id, OBJECT_ID key);//remove a global object

OBJECT_ID vm_GetGlobal(VM_ID vm_id, OBJECT_ID key);//retrieve a global object

OBJECT_ID vm_GetGlobalByIndex(VM_ID vm_id, INDEX i);

void vm_Interrupt(VM_ID vm_id, OBJECT_ID(*interrupt_func) (VM_ID vm_id,STACK_ID stack)); //interrupt vm and call interrupt handler afterwards

void vm_Continue(VM_ID vm_id);//continue vm execution

void vm_Exit(VM_ID vm_id,BYTES_ID message,NUM code); //exit vm

void vm_Stop(VM_ID vm_id); //pause vm execution

//OBJECT_ID vm_Step(VM_ID vm_id);//single step vm

NUM vm_GetBlockLevel(VM_ID vm_id);

OBJECT_ID vm_Step(VM_ID vm_id,BOOL exit_after_reaching_block_level);//single step vm - if 1 exit on return if block level matches the parameter 

OBJECT_ID vm_RunPYC(VM_ID vm_id,STREAM_ID f,BOOL free_object);

OBJECT_ID vm_RunRPYC(VM_ID vm_id,STREAM_ID f ,BOOL free_object);

#ifndef USE_ARDUINO_FUNCTIONS
OBJECT_ID vm_RunRPYCPlus(VM_ID vm_id,STREAM_ID f ,BOOL free_object);
#endif
OBJECT_ID vm_RunFunction(VM_ID vm_id,BYTES_ID name_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id);//call a python function from C

OBJECT_ID vm_RunObject(VM_ID vm_id, OBJECT_ID obj_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id);//run a python object if possible

OBJECT_ID vm_RunFunctionObject(VM_ID vm_id, FUNCTION_ID func_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id);//run a python function object
//#ifdef USE_DEBUGGING
//OBJECT_ID vm_InteractiveRunObject(VM_ID vm_id, OBJECT_ID obj,  TUPLE_ID locals_id,TUPLE_ID kw_locals_id);
//#endif

OBJECT_ID vm_StartMethod(VM_ID vm_id,OBJECT_ID key,CLASS_INSTANCE_ID cio_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id);

OBJECT_ID vm_RunMethod(VM_ID vm_id,OBJECT_ID key,CLASS_INSTANCE_ID cio,TUPLE_ID locals_id,TUPLE_ID kw_locals_id);

OBJECT_ID vm_StartObject(VM_ID vm_id,OBJECT_ID obj_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id);

OBJECT_ID vm_StartObjectCopy(VM_ID vm_id,OBJECT_ID obj_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id);

BLOCK_ID vm_StartCodeObject(VM_ID vm_id,CODE_ID co_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id);

BLOCK_ID vm_StartClassObject(VM_ID vm_id,CLASS_ID class_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id);

OBJECT_ID vm_StartFunctionObject(VM_ID vm_id,FUNCTION_ID fo_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id);//run a python function object 

OBJECT_ID vm_StartCFunctionObject(VM_ID vm_id,CFUNCTION_ID cfo_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id);//run a c function object 

OBJECT_ID vm_StartMethodObject(VM_ID vm_id,METHOD_ID mo_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id);

//#ifdef USE_DEBUGGING
//void vm_DumpCode(VM_ID vm_id,BOOL dump_descriptions,BOOL from_start);//dump human readable code of the vm's actual running block 
//#endif

#ifdef __cplusplus
} 
#endif
#endif
