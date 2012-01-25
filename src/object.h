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

#include "debug.h"
#ifdef DEBUGGING
#include "assert.h"
#include "memory.h"
#endif

#define MAGIC (3180 | ((long)'\r'<<16) | ((long)'\n'<<24))
#define TAG "cpython-32"
#define CACHEDIR "__pycache__"
/* Current magic word and string tag as globals. */
static long pyc_magic = MAGIC;

static const char *pyc_tag = TAG;

static short pyc_magic_short = 3180;


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


// #define OFLAG_USES_VALUE_PTR 1
// #define OFLAG_IS_NAME 2
// #define OFLAG_IS_CONST 4
// #define OFLAG_IS_VARNAME 8


//#define OFLAG_ON_STACK 1
#define OFLAG_UNLOADED 2		// set if object was unloaded -> obj->ptr ==
								// seek_pos // or just dump to file and reread 
								// if accessed

#define OFLAG_HOLD_IN_MEMORY 4
//#define OFLAG_HAS_VALUE_PTR 8
#define OFLAG_TUPLE_PTR 16		// used to iterate over tuples
#define OFLAG_TUPLE_RESTART_FLAG 32	// used to iterate over tuples
#define OFLAG_IS_DICT 64 //used distinguish between tuples and dicts because both use the same structs
//#define OFLAG_LEFT_NAMESPACE 128

//code flags
#define CO_OPTIMIZED	0x0001
#define CO_NEWLOCALS	0x0002
#define CO_VARARGS	0x0004
#define CO_VARKEYWORDS	0x0008
#define CO_NESTED       0x0010
#define CO_GENERATOR    0x0020
#define CO_NOFREE       0x0040



// internal types
#define TYPE_FUNCTION 'f'
#define TYPE_BLOCK 'b'
#define TYPE_KV 'k'
#define TYPE_REF 'r'
#define TYPE_ITER 'R'


//function types
#define FUNC_PYTHON 1
#define FUNC_C 2
#define FUNC_C_OBJ 4


#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */

typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
} object;

typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	object *ref;
} ref_object;


typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	INT value;
} int_object;

typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	FLOAT value;
} float_object;

typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	char *value;
}unicode_object;


typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	void *value;
	void *key;
} kv_object;				// TO OPTIMIZE MEMORY USAGE -> only used in tuples 

/*								
typedef struct
{
	char type;
	unsigned char flags;
	unsigned short  ref_count;
} empty_object;					// TO OPTIMIZE MEMORY USAGE
*/

typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	char *name;
	NUM argcount;
	NUM kwonlyargcount;
	NUM nlocals;
	NUM stacksize;
	long co_flags;
	object *code;
	object *consts;
	object *names;
	object *varnames;
	object *freevars;
	object *cellvars;
	// object *filename;
	// object *name;
	// object *lnotab;
	// long codesize;
	// long firstlineno;
} code_object;

typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	char *content;
	NUM len;
} string_object;


typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	ptr_list *list;
} tuple_object;


struct _vm;
struct _stack;

typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	tuple_object *defaults;//set to default values in MAKE_FUNCTION opcode
	tuple_object *kw_defaults;//set to default keyword values in MAKE_FUNCTION opcode
	//code_object *code;
	char *name;//used quickly find functions by name
	unsigned char func_type;
	tuple_object *closure;
	union func_def
	{
		object *(*func) (struct _vm *vm,struct _stack *stack);
		object *(*func_obj) (struct _vm *vm,object *object);
		code_object *code;
	} func;
} function_object;

//TODO create struct for generator storage
typedef struct _iter_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	object *tag;//used for storage of iter options and actual ptr
	object *(*iter_func)(struct _iter_object *iter);
} iter_object;

typedef struct
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	code_object *code;
	INDEX start;
	NUM len;
	iter_object *iter;  //TODO rename to tag ,and use it for yielding too
	INDEX ip;
	char initiated_locals;//TODO REMOVE NOT NEEDED
	struct _stack *stack;
} block_object;

#pragma pack(pop)				/* restore original alignment from stack */

block_object *AllocBlockObject();

void IncRefCount(object *obj);

void DecRefCountGC(object *obj,ptr_list *gc);

void DecRefCount(object *obj);

int HasNoRefs(object *obj);

int HasRefs(object *obj);

kv_object *ConvertToKVObject(object *key);

kv_object *ConvertToKVObjectValued(object *key,object *value);

object *AllocObject();

//object *AllocEmptyObject();

object *AllocKVObject();

object *AllocRefObject();

string_object *AllocStringObject();

tuple_object *AllocTupleObject();

unicode_object *AllocUnicodeObject();

code_object *AllocCodeObject();

iter_object *AllocIterObject();

int_object *AllocIntObject();

float_object *AllocFloatObject();

function_object *AllocFunctionObject();

/*object *AsObject(void *ptr);

string_object *AsStringObject(object * obj);

code_object *AsCodeObject(object * obj);

ref_object *AsRefObject(object *obj);

iter_object *AsIterObject(object * obj);

int_object *AsIntObject(object * obj);

float_object *AsFloatObject(object * obj);

function_object *AsFunctionObject(object * obj);

tuple_object *AsTupleObject(object * obj);

unicode_object *AsUnicodeObject(object *obj);

int IsIntObject(object * obj);

int IsFloatObject(object * obj);

int IsStringObject(object * obj);

int IsUnicodeObject(object * obj);

int IsCodeObject(object * obj);

int IsTupleObject(object * obj);

int IsRefObject(object * obj);

int IsIterObject(object * obj);
*/

long ReadLong(FILE * f);

float ReadFloat(FILE * f);

char ReadChar(FILE * f);

object *ReadObject(FILE * f);

object *DissolveRef(object *obj);

ref_object *CreateRefObject(object *ref_to,int flags);

int_object *CreateIntObject(long value,int flags);

float_object *CreateFloatObject(float value,int flags);

unicode_object *CreateUnicodeObject(char *value,int flags);

tuple_object *CreateTuple(int num,int flags);

string_object *CreateStringObject(char *bytes,int len,int flags);

kv_object *CreateKVObject(object *key,object *value,int flags);

object *CreateEmptyObject(char type,int flags);

function_object *CreateFunctionObject_MAKE_FUNCTION(code_object *function_code,tuple_object *defaults,tuple_object *kw_defaults,int flags);

function_object *CreateFunctionObject_MAKE_CLOSURE(code_object *function_code,object *closure,tuple_object *defaults,tuple_object *kw_defaults,int flags);

function_object *CreateFunctionObject(unsigned char func_type,int flags);

//iter_object *CreateIterObject(object *(*iter_func)(struct _iter_object *iter),object *tag,int flags);

iter_object *CreateIterObject(int flags);

void FreeBlockObject(object *obj);

void FreeObject(object * obj);

void PrintObject(object * obj);

#ifdef DEBUGGING
void DumpObject(object * obj, int level);

char *DumpObjectXml(object * obj, int level);
#endif

object *GetNextItem(object * tuple);

void ResetIteration(object * tuple);

void SetItem(object * tuple, int index, object * obj);

object *GetItem(object * tuple, int index);

int GetItemIndexByName(object * tuple, char *name);

void SetDictItem(object *tuple,object *key,object *value);

void SetDictItemByIndex(object *tuple,int index,object *value);

object *GetDictItem(object *tuple,object *key);

object *GetDictItemByIndex(object *tuple,int index);

int GetTupleLen(object *tuple);

int GetDictItemIndex(object *tuple,object *key);

int GetItemIndex(object *tuple,object *obj);

object *CopyObject(object *obj);

int object_compare(object *a,object *b);

void AppendDictItem(object * tuple,object *key,object *value);

void AppendItem(object *tuple,object *value);

void ClearDictValues(object *tuple);

#endif
