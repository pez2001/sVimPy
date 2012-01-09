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
#include "memory.h"
#include "lists.h"
#include "assert.h"

extern int debug_level;
#include "debug.h"



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
//#define TYPE_FUNCTION 'f'
//#define TYPE_CALLER 'C'
#define TYPE_BLOCK 'b'
#define TYPE_KV 'k'
#define TYPE_REF 'r'



#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */

typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	void *value;
	//void *value_ptr;			// TODO remove this member --> TO DECREASE
								// MEMORY USAGE
} object;

typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	object *ref;
} ref_object;


typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	long value;
} int_object;

typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	float value;
} float_object;

typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	char *value;
}unicode_object;


typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	void *value;
	void *key;
} kv_object;				// TO OPTIMIZE MEMORY USAGE -> only used in
								// tuples 

typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
} empty_object;					// TO OPTIMIZE MEMORY USAGE

typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	long argcount;
	long kwonlyargcount;
	long nlocals;
	long stacksize;
	long co_flags;
	object *code;
	object *consts;
	object *names;
	object *varnames;
	object *freevars;
	object *cellvars;
	// object *filename;
	// object *name;
	char *name;
	// long codesize;
	long firstlineno;
	// object *lnotab;
} code_object;

typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	char *content;
	long len;
} string_object;


typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	ptr_list *list
} tuple_object;


//TODO repurpose for generator storage
typedef struct
{
	code_object *code;
	// long pos;
} function_object;


struct _stack;

typedef struct
{
	char type;
	unsigned char flags;
	short ref_count;
	code_object *code;
	long start;
	long len;
	object *iter;
	long ip;
	char initiated_locals;
	struct _stack *stack;
} block_object;

#pragma pack(pop)				/* restore original alignment from stack */


block_object *AllocBlockObject();

void FreeBlockObject(object *obj);

void IncRefCount(object *obj);

void DecRefCountGC(object *obj,ptr_list *gc);

void DecRefCount(object *obj);

int HasNoRefs(object *obj);

int HasRefs(object *obj);

kv_object *ConvertToKVObject(object *key);

kv_object *ConvertToKVObjectValued(object *key,object *value);

object *AllocObject();

object *AllocEmptyObject();

object *AllocKVObject();

object *AllocRefObject();

string_object *AllocStringObject();

tuple_object *AllocTupleObject();

unicode_object *AllocUnicodeObject();

code_object *AllocCodeObject();

//caller_object *AllocCallerObject();

int_object *AllocIntObject();

function_object *AllocFunctionObject();

long ReadLong(FILE * f);

char ReadChar(FILE * f);

object *ReadObject(FILE * f);

object *AsObject(void *ptr);

string_object *AsStringObject(object * obj);

code_object *AsCodeObject(object * obj);

ref_object *AsRefObject(object *obj);

//caller_object *AsCallerObject(object * obj);
int_object *AsIntObject(object * obj);

function_object *AsFunctionObject(object * obj);

tuple_object *AsTupleObject(object * obj);

unicode_object *AsUnicodeObject(object *obj);

int IsIntObject(object * obj);

int IsStringObject(object * obj);

int IsUnicodeObject(object * obj);

int IsCodeObject(object * obj);

int IsTupleObject(object * obj);

int IsRefObject(object * obj);

object *DissolveRef(object *obj);

ref_object *CreateRefObject(object *ref_to,int flags);

int_object *CreateIntObject(long value,int flags);

unicode_object *CreateUnicodeObject(char *value,int flags);

tuple_object *CreateTuple(int num,int flags);

string_object *CreateStringObject(char *bytes,int len,int flags);

kv_object *CreateKVObject(object *key,object *value,int flags);

empty_object *CreateEmptyObject(char type,int flags);

void FreeObject(object * obj);

void PrintObject(object * obj);

void DumpObject(object * obj, int level);

object *GetNextItem(object * tuple);

void ResetIteration(object * tuple);

void SetItem(object * tuple, int index, object * obj);

object *GetItem(object * tuple, int index);

int GetItemIndexByName(object * tuple, char *name);

void SetDictItem(object *tuple,object *key,object *value);

void SetDictItemByIndex(object *tuple,int index,object *value);

object *GetDictItem(object *tuple,object *key);

int GetDictItemIndex(object *tuple,object *key);

object *CopyObject(object *obj);

int object_compare(object *a,object *b);

void AppendDictItem(object * tuple,object *key,object *value);

#endif
