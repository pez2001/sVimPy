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

#include "debug.h"
#ifdef DEBUGGING
#include "assert.h"
#include "memory.h"
#endif

//#define TAG "cpython-32"
//#define CACHEDIR "__pycache__"
/* Current magic word and string tag as globals. */

//static const char *pyc_tag = TAG;

//static short pyc_magic_short = 3180;


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

struct _vm;
struct _stack;
struct _object;
struct _code_object;

typedef	union _object_func
{
	struct _object* (*func) (struct _vm *vm,struct _stack *stack);
	struct _object* (*func_obj) (struct _vm *vm,struct _object *object);
	struct _code_object *code;
} object_func;


#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */

typedef struct _object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
} object;

typedef struct _ref_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	object *ref;
} ref_object;


typedef struct _int_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	INT value;
} int_object;

typedef struct _float_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	FLOAT value;
} float_object;

typedef struct _unicode_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	char *value;
}unicode_object;


typedef struct _kv_object
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

typedef struct _code_object
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

typedef struct _string_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	char *content;
	NUM len;
} string_object;


typedef struct _tuple_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	ptr_list *list;
} tuple_object;


//struct _vm;
//struct _stack;

typedef struct _function_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	char *name;//used quickly find functions by name
	tuple_object *defaults;//set to default values in MAKE_FUNCTION opcode
	tuple_object *kw_defaults;//set to default keyword values in MAKE_FUNCTION opcode
	tuple_object *closure;
	//code_object *code;
	unsigned char func_type;
	union _object_func func;
} function_object;

//TODO create struct for generator storage
typedef struct _iter_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	object *tag;//used for storage of iter options and actual ptr
	object *(*iter_func)(struct _iter_object *iter);
	struct _stack *block_stack;//iters will save blocks on stack when yielding
} iter_object;
/*
typedef struct _loop_block_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	code_object *code;
	INDEX start;
	NUM len;
	INDEX ip;
	//struct _stack *stack;
} loop_block_object;
*/
typedef struct _block_object
{
	OBJECT_TYPE type;
	OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	code_object *code;
	INDEX start;
	//INDEX start;
	//NUM len;
	//iter_object *iter;  //TODO rename to tag ,and use it for yielding too
	//struct _block_object *parent; //used for chaining
	INDEX ip;
	NUM len;
	//char initiated_locals;//TODO REMOVE NOT NEEDED
	struct _stack *stack;
} block_object;

#pragma pack(pop)				/* restore original alignment from stack */

void IncRefCount(object *obj);

void DecRefCountGC(object *obj,ptr_list *gc);

void DecRefCount(object *obj);

BOOL HasNoRefs(object *obj);

BOOL HasRefs(object *obj);

kv_object *ConvertToKVObject(object *key);

kv_object *ConvertToKVObjectValued(object *key,object *value);

object *AllocObject(void);

block_object *AllocBlockObject(void);

//loop_block_object *AllocLoopBlockObject();

kv_object *AllocKVObject(void);

ref_object *AllocRefObject(void);

string_object *AllocStringObject(void);

tuple_object *AllocTupleObject(void);

unicode_object *AllocUnicodeObject(void);

code_object *AllocCodeObject(void);

iter_object *AllocIterObject(void);

int_object *AllocIntObject(void);

float_object *AllocFloatObject(void);

function_object *AllocFunctionObject(void);

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

long ReadLong(stream *f);

FLOAT ReadFloat(stream *f);

char ReadChar(stream *f);

object *ReadObject(stream *f);

object *DissolveRef(object *obj);

ref_object *CreateRefObject(object *ref_to,OBJECT_FLAGS flags);

int_object *CreateIntObject(INT value,OBJECT_FLAGS flags);

float_object *CreateFloatObject(FLOAT value,OBJECT_FLAGS flags);

unicode_object *CreateUnicodeObject(char *value,OBJECT_FLAGS flags);

tuple_object *CreateTuple(NUM num,OBJECT_FLAGS flags);

string_object *CreateStringObject(char *bytes,NUM len,OBJECT_FLAGS flags);

kv_object *CreateKVObject(object *key,object *value,OBJECT_FLAGS flags);

object *CreateEmptyObject(char type,OBJECT_FLAGS flags);

function_object *CreateFunctionObject_MAKE_FUNCTION(code_object *function_code,tuple_object *defaults,tuple_object *kw_defaults,OBJECT_FLAGS flags);

function_object *CreateFunctionObject_MAKE_CLOSURE(code_object *function_code,tuple_object *closure,tuple_object *defaults,tuple_object *kw_defaults,OBJECT_FLAGS flags);

function_object *CreateFunctionObject(unsigned char func_type,OBJECT_FLAGS flags);

//iter_object *CreateIterObject(object *(*iter_func)(struct _iter_object *iter),object *tag,int flags);

iter_object *CreateIterObject(OBJECT_FLAGS flags);

void FreeBlockObject(object *obj);

void FreeObject(object *obj);

void PrintObject(object *obj);

#ifdef DEBUGGING
void DumpObject(object *obj, char level);

char *DumpObjectXml(object *obj, char level);
#endif

object *GetNextItem(object *tuple);

void ResetIteration(object *tuple);

void SetItem(object *tuple, INDEX index, object *obj);

object *GetItem(object *tuple, INDEX index);

INDEX GetItemIndexByName(object *tuple, char *name);

void SetDictItem(object *tuple,object *key,object *value);

void SetDictItemByIndex(object *tuple,INDEX index,object *value);

object *GetDictItem(object *tuple,object *key);

object *GetDictItemByIndex(object *tuple,INDEX index);

object *GetDictItemByName(object *tuple,char *name);

NUM GetTupleLen(object *tuple);

INDEX GetDictItemIndex(object *tuple,object *key);

INDEX GetItemIndex(object *tuple,object *obj);

object *CopyObject(object *obj);

BOOL object_compare(object *a,object *b);

void AppendDictItem(object *tuple,object *key,object *value);

void AppendItem(object *tuple,object *value);

void ClearDictValues(object *tuple);

void DeleteItem(object *tuple, INDEX index);

#endif
