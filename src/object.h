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


#ifndef OBJECT_H
#define OBJECT_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "types.h"
#include "lists.h"
#include "stream.h"
#include "stack.h"
#include "garbage.h"

#include "debug.h"
#include "memory.h"
#include "tuples.h"

#ifdef __cplusplus
extern "C"  {
#endif

#define TYPE_NULL               '0'
#define TYPE_NONE               'N'
#define TYPE_FALSE              'F'
#define TYPE_TRUE               'T'
#define TYPE_STOPITER           'S'
#define TYPE_ELLIPSIS           '.'
#define TYPE_INT                'i'
#define TYPE_INT64              'I'
#define TYPE_FLOAT              'f'
#define TYPE_BINARY_FLOAT       'g'
#define TYPE_COMPLEX            'x'
#define TYPE_BINARY_COMPLEX     'y'
#define TYPE_LONG               'l'
#define TYPE_STRING             's'
#define TYPE_TUPLE              '('
#define TYPE_LIST               '['
#define TYPE_DICT               '{'
#define TYPE_CODE               'c'
#define TYPE_UNICODE            'u'
#define TYPE_UNKNOWN            '?'
#define TYPE_SET                '<'
#define TYPE_FROZENSET          '>'

// internal types
#define TYPE_FUNCTION 'f'
#define TYPE_CFUNCTION 'C'
#define TYPE_BLOCK 'b'
#define TYPE_KV 'k'
#define TYPE_PROXY 'r'
#define TYPE_ITER 'R'
#define TYPE_CLASS_INSTANCE '*'
#define TYPE_CLASS '&'
//#define TYPE_EXCEPTION 'E'
#define TYPE_METHOD 'M'
#define TYPE_TAG 't'

//code flags
#define CO_OPTIMIZED	0x0001
#define CO_NEWLOCALS	0x0002
#define CO_VARARGS	0x0004
#define CO_VARKEYWORDS	0x0008
#define CO_NESTED       0x0010
#define CO_GENERATOR    0x0020
#define CO_NOFREE       0x0040


//internal code flags
#define CO_MODULE_ROOT 0x0080
#define CO_CLASS_ROOT 0x0100
#define CO_SUB_CLASS_ROOT 0x0200

struct _vm;
struct _stack;
struct _object;
struct _tuple_object;
struct _code_object;

//obj_Free forward declaration needed for class deconstructor calling
OBJECT_ID vm_RunMethod(VM_ID vm_id,OBJECT_ID key,CLASS_INSTANCE_ID cio,TUPLE_ID locals_id,TUPLE_ID kw_locals_id);


#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */
#endif


//TODO 
// clearer distinguishing of BYTES_ID & STRING_ID



//general object header struct
//normal size = 9
//avr size = 4
typedef struct _object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
} object;

//tag object struct
//can be used to reference to custom data
// *tag will not be freed by gc
//normal size = 13
//avr size = 6
typedef struct _tag_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	void *tag;
} tag_object;

//integer object struct
//normal size = 13
//avr size = 8
typedef struct _int_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	INT value;
} int_object;

//float object struct
//normal size = 13
//avr size = 8
typedef struct _float_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	FLOAT value;
} float_object;

//unicode object struct
//gc will free value
//normal size = 13
//avr size = 6
typedef struct _unicode_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	BYTES_ID value;/* char* */
}unicode_object;

//kv object struct
//represents a key / value pair
//used in dictionaries for example
//normal size = 17
//avr size = 8
typedef struct _kv_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	OBJECT_ID value;/*object*/
	OBJECT_ID key;/*object*/
} kv_object;				// TO OPTIMIZE MEMORY USAGE -> only used in tuples 

//code object struct
//represents a python code object
//used in python modules,functions,methods,classes
//gc will free name
//normal size = 57
//avr size = 30
typedef struct _code_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	BYTES_ID name; /* char* */
	NUM argcount;
	NUM kwonlyargcount;
	NUM nlocals;
	NUM stacksize;
	long co_flags; //TODO minimize storage here
	STRING_ID code;/*string object*/
	TUPLE_ID consts;/*tuple_object*/
	TUPLE_ID names;/*tuple_object*/
	TUPLE_ID varnames;/*tuple_object*/
	TUPLE_ID freevars;/*tuple_object*/
	TUPLE_ID cellvars;/*tuple_object*/
	// object *filename;
	// object *name;
	// object *lnotab;
	// long codesize;
	// long firstlineno;
} code_object;

//class object struct
//internal representation of python class objects
//normal size = 17
//avr size = 8
typedef struct _class_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	CODE_ID code;/*code_object*/
	TUPLE_ID base_classes;/*tuple_object*/
} class_object;

//class instance object struct
//internal representation of python class instances
//normal size = 21
//avr size = 10
typedef struct _class_instance_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	CLASS_ID instance_of;/*class_object*/
	TUPLE_ID methods;/*tuple_object*/
	TUPLE_ID vars;/*tuple_object*/
} class_instance_object;

//method object struct
//internal representation of python class methods
//methods are bound to a specified class instance
//normal size = 17
//avr size = 8
typedef struct _method_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	CODE_ID func;/*code_object*/
	CLASS_INSTANCE_ID instance;/*class_instance_object*/
} method_object;

//string object struct
//byte arrays 
//at the moment only used to store python bytecode
//gc will free content
//normal size = 17
//avr size = 8
typedef struct _string_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	BYTES_ID content;/*char* */
	NUM len;
} string_object;

//tuple object struct
//normal size = 17
//gc will free list
//avr size = 8
typedef struct _tuple_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	INDEX ptr;
	LIST_ID list; /*id_list*/ //TODO switch ptr_list to MEM_ID
} tuple_object;

//function object struct
//internal type used to push functions on the stack
//or to store functions in modules or methods in classes
//normal size = 13
//avr size = 6
typedef struct _function_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	CODE_ID func;/*code_object*/
} function_object;

//cfunction object struct
//internal type used to push external c-functions on the stack
//or to store c-functions in modules or methods in classes
//normal size = 13
//avr size = 6
typedef struct _cfunction_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	OBJECT_ID (*func) (VM_ID vm_id,TUPLE_ID locals,TUPLE_ID kw_locals);
} cfunction_object;

//iter object struct
//iterator state object
//normal size = 21
//avr size = 10
typedef struct _iter_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	OBJECT_ID tag;//used for storage of iter options and index ptr  => object*
	OBJECT_ID (*iter_func)(OBJECT_ID iter,VM_ID vm_id);
	STACK_ID block_stack;/*stack*/  //iters will save blocks on stack when yielding
} iter_object;

//block object struct
//blocks are used to form the execution tree
//normal size = 29
//avr size = 14
typedef struct _block_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	CODE_ID code; /*code_object*/
	INDEX start;
	INDEX ip;
	NUM len;
	STACK_ID stack;/*stack*/
} block_object;

#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(pop)				/* restore original alignment from stack */
#endif

//object *GetAttributeByName(object *obj,char *name);

void obj_Init(void);

void obj_SetVM(VM_ID vm_id);

void obj_Close(void);

OBJECT_ID obj_Alloc(OBJECT_TYPE type,OBJECT_REF_COUNT ref_count);

OBJECT_ID obj_CreateTag(void *tag);

OBJECT_ID obj_CreateInt(INT value);

OBJECT_ID obj_CreateFloat(FLOAT value);

OBJECT_ID obj_CreateUnicode(MEM_ID value); /*char* */

OBJECT_ID obj_CreateTuple(NUM num);

OBJECT_ID obj_CreateString(MEM_ID bytes,NUM len); /*char* */

OBJECT_ID obj_CreateKV(OBJECT_ID key,OBJECT_ID value);

OBJECT_ID obj_CreateEmpty(char type);

OBJECT_ID obj_CreateFunction(OBJECT_ID function_code,OBJECT_ID defaults,OBJECT_ID kw_defaults,OBJECT_ID closure);

OBJECT_ID obj_CreateCFunction(OBJECT_ID (*func) (VM_ID vm_id,OBJECT_ID locals,OBJECT_ID kw_locals));

OBJECT_ID obj_CreateMethod(OBJECT_ID func,OBJECT_ID class_instance);

OBJECT_ID obj_CreateClass(OBJECT_ID code,OBJECT_ID base_classes);

OBJECT_ID obj_CreateCode(MEM_ID name); /*char* */

OBJECT_ID obj_CreateClassInstance(OBJECT_ID instance_of);

OBJECT_ID obj_CreateIter(void);

OBJECT_ID obj_ConvertToKV(OBJECT_ID obj); /*returns kv_object*/

OBJECT_ID obj_ConvertToValuedKV(OBJECT_ID key,OBJECT_ID value);

void obj_Print(OBJECT_ID obj);

void obj_PrintTabs(char num);

void obj_Dump(OBJECT_ID obj_id, char level,char verbosity);

BOOL obj_Compare(OBJECT_ID a,OBJECT_ID b);//TODO add tuple compare

OBJECT_ID obj_Copy(OBJECT_ID obj_id);

OBJECT_ID obj_GetAttribute(OBJECT_ID obj,OBJECT_ID key);

void obj_SetAttribute(OBJECT_ID obj,OBJECT_ID key,OBJECT_ID value);

OBJECT_ID obj_GetClassMethod(OBJECT_ID _class,OBJECT_ID key);

OBJECT_ID obj_GetClassVar(OBJECT_ID _class,OBJECT_ID key);

void obj_AddCodeName(OBJECT_ID code_object,OBJECT_ID key,OBJECT_ID value);

void obj_AddCodeFunction(OBJECT_ID code_object,BYTES_ID name,OBJECT_ID func);

void obj_AddCodeCFunction(OBJECT_ID code_object,BYTES_ID name, OBJECT_ID (*func) (VM_ID vm_id,OBJECT_ID locals,OBJECT_ID kw_locals));

NUM obj_GetUnicodeLen(OBJECT_ID obj);

void obj_IncRefCount(OBJECT_ID obj_id);

void obj_DecRefCount(OBJECT_ID obj_id);

BOOL obj_HasNoRefs(OBJECT_ID obj);

BOOL obj_HasRefs(OBJECT_ID obj);

void obj_Free(OBJECT_ID obj);

void obj_ClearGC(void);

OBJECT_TYPE obj_GetType(OBJECT_ID obj);

#ifdef __cplusplus
} 
#endif

#endif


