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
#ifdef USE_DEBUGGING
#include "assert.h"
#include "memory.h"
#endif

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

#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(push)				/* push current alignment to stack */
#pragma pack(210)					/* set alignment to 1 byte boundary */
#endif


//general object header struct
//normal size = 9
//avr size = 4
typedef struct _object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
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
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	void *tag;
} tag_object;

//integer object struct
//normal size = 13
//avr size = 8
typedef struct _int_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	INT value;
} int_object;

//float object struct
//normal size = 13
//avr size = 8
typedef struct _float_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
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
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	char *value;
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
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	void *value;
	void *key;
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
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	char *name;
	NUM argcount;
	NUM kwonlyargcount;
	NUM nlocals;
	NUM stacksize;
	long co_flags; //TODO minimize storage here
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

//class object struct
//internal representation of python class objects
//normal size = 17
//avr size = 8
typedef struct _class_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	code_object *code;
	object *base_classes;
} class_object;

//class instance object struct
//internal representation of python class instances
//normal size = 21
//avr size = 10
typedef struct _class_instance_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	class_object *instance_of;	
	object *methods;
	object *vars;
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
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	object *func;
	class_instance_object *instance;
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
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	char *content;
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
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	INDEX ptr;
	ptr_list *list;
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
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	struct _code_object *func;
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
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals);
} cfunction_object;

//iter object struct
//iterator state object
//normal size = 21
//avr size = 10
typedef struct _iter_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	object *tag;//used for storage of iter options and index ptr
	object *(*iter_func)(struct _iter_object *iter,struct _vm *vm);
	struct _stack *block_stack;//iters will save blocks on stack when yielding
} iter_object;

//block object struct
//blocks are used to form the execution tree
//normal size = 29
//avr size = 14
typedef struct _block_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	code_object *code;
	INDEX start;
	INDEX ip;
	NUM len;
	struct _stack *stack;
} block_object;

#ifdef USE_LOCKING
/*
typedef struct _cache_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_REF_COUNT lock_count;
	#endif
	//object *object;
	STREAM_NUM stream_pos;
} cache_object;
*/

//proxy object struct
//used for virtual caching objects in streams
//normal size = 9 + 8 
//avr size = 4 + 4
typedef struct _proxy_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	void *ref;//object* or stream_pos/512
	void *is_cached; // == 0 -> not cached use ref as object*  > 0 -> pointer stream where object is cached ,ref is the position(mmc sector address 512 bytes aligned) in the stream 
} proxy_object;
#endif

#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(pop)				/* restore original alignment from stack */
#endif

void obj_Init(void);

void obj_Close(void);

object *AllocObject(void);

block_object *AllocBlockObject(void);

kv_object *AllocKVObject(void);

#ifdef USE_LOCKING
proxy_object *AllocProxyObject(void);
#endif

tag_object *AllocTagObject(void);

string_object *AllocStringObject(void);

tuple_object *AllocTupleObject(void);

unicode_object *AllocUnicodeObject(void);

code_object *AllocCodeObject(void);

class_object *AllocClassObject(void);

class_instance_object *AllocClassInstanceObject(void);

iter_object *AllocIterObject(void);

int_object *AllocIntObject(void);

float_object *AllocFloatObject(void);

function_object *AllocFunctionObject(void);

cfunction_object *AllocCFunctionObject(void);

method_object *AllocMethodObject(void);

#ifdef USE_LOCKING
proxy_object *CreateProxyObject(void);
#endif

tag_object *CreateTagObject(void *tag);

int_object *CreateIntObject(INT value);

float_object *CreateFloatObject(FLOAT value);

unicode_object *CreateUnicodeObject(char *value);

tuple_object *CreateTuple(NUM num);

string_object *CreateStringObject(char *bytes,NUM len);

kv_object *CreateKVObject(object *key,object *value);

object *CreateEmptyObject(char type);

function_object *CreateFunctionObject(code_object *function_code,tuple_object *defaults,tuple_object *kw_defaults,tuple_object *closure);

cfunction_object *CreateCFunctionObject(struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals));//,tuple_object *defaults,tuple_object *kw_defaults);//used for in-python storage of external calls

method_object *CreateMethodObject(object *func,class_instance_object *instance);

class_object *CreateClassObject(code_object *code,object *base_classes);//char *name,

code_object *CreateCodeObject(char *name);

class_instance_object *CreateClassInstanceObject(class_object *instance_of);

iter_object *CreateIterObject(void);

#ifdef USE_LOCKING
object *LockObject(object *obj);

void UnlockObject(object *obj);
#endif

object *CopyObject(object *obj);

BOOL CompareObjects(object *a,object *b);

kv_object *ConvertToKVObject(object *key);

kv_object *ConvertToKVObjectValued(object *key,object *value);

//object *DissolveRef(object *obj);

void PrintObject(object *obj);

#ifndef USE_ARDUINO_FUNCTIONS
void FullDumpObject(object * obj, char level);
#else
void FullDumpObjectArduino(object * obj, char level);
#endif

#ifdef USE_DEBUGGING
void DumpObject(object *obj, char level);

char *DumpObjectXml(object *obj, char level);
#endif

void AddCodeName(object *co,object *key,object *value);

void AddCodeFunction(object *co,char *name,object *func);

void AddCodeCFunction(object *co,char *name,	struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals));


//TODO tuple functions -> move to tuples.c 
void ClearDictValues(object *tuple);

void ConvertToDict(object *tuple);

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

object *GetAttribute(object *obj,object *key);

//object *GetAttributeByName(object *obj,char *name);

void SetAttribute(object *obj,object *key,object *value);

object *GetClassMethod(object *_class,object *key);

object *GetClassVar(object *_class,object *key);

void AppendDictItem(object *tuple,object *key,object *value);

void AppendItem(object *tuple,object *value);

void InsertItem(object *tuple,INDEX index,object *value);

void ClearDictValues(object *tuple);

void DeleteItem(object *tuple, INDEX index);

void DeleteDictItem(object *tuple,object *key);

#ifdef __cplusplus
} 
#endif

#endif


