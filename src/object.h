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

//#define TAG "cpython-32"
//#define CACHEDIR "__pycache__"
/* Current magic word and string tag as globals. */

//static const char *pyc_tag = TAG;

//static short pyc_magic_short = 3180;
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
#define TYPE_REF 'r'
#define TYPE_ITER 'R'
#define TYPE_CLASS_INSTANCE '*'
#define TYPE_CLASS '&'
#define TYPE_EXCEPTION 'E'
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
/*
typedef	union _object_func
{
	struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals);
	struct _object* (*func_obj) (struct _vm *vm,struct _object *object);
	struct _code_object *code;
} object_func;
*/

#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */
#endif

typedef struct _object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
} object;

typedef struct _ref_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	object *ref;
} ref_object;

typedef struct _tag_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	void *tag;
} tag_object;


typedef struct _int_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	INT value;
} int_object;

typedef struct _float_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	FLOAT value;
} float_object;

typedef struct _unicode_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	char *value;
}unicode_object;


typedef struct _kv_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	void *value;
	void *key;
} kv_object;				// TO OPTIMIZE MEMORY USAGE -> only used in tuples 

typedef struct _code_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
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

typedef struct _class_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	//char *name;//TODO remove
	code_object *code;
	object *base_classes;
} class_object;

typedef struct _class_instance_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	class_object *instance_of;	
	object *methods;
	object *vars;
} class_instance_object;

typedef struct _method_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	//char *name;
	object *func;
	class_instance_object *instance;
} method_object;


typedef struct _string_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	char *content;
	NUM len;
} string_object;


typedef struct _tuple_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	//object *ptr;
	INDEX ptr;
	ptr_list *list;
} tuple_object;


//struct _vm;
//struct _stack;

typedef struct _function_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	/*tuple_object *defaults;//set to default values in MAKE_FUNCTION opcode //TODO REMOVE THESE ... redundant with new function calling style(each function gets its own copy before execution > move this into the make_function opcode)
	tuple_object *kw_defaults;//set to default keyword values in MAKE_FUNCTION opcode
	tuple_object *closure;*/
	struct _code_object *func;
} function_object;

typedef struct _cfunction_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	//tuple_object *defaults;//set to default values in MAKE_FUNCTION opcode //TODO check if really needed
	//tuple_object *kw_defaults;//set to default keyword values in MAKE_FUNCTION opcode
	struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals);
} cfunction_object;


//TODO create struct for generator storage
typedef struct _iter_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	object *tag;//used for storage of iter options and index ptr
	object *(*iter_func)(struct _iter_object *iter,struct _vm *vm);
	struct _stack *block_stack;//iters will save blocks on stack when yielding
} iter_object;

typedef struct _block_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	code_object *code;
	INDEX start;
	INDEX ip;
	NUM len;
	struct _stack *stack;
} block_object;

typedef struct _exception_object
{
	OBJECT_TYPE type;
	//OBJECT_FLAGS flags;
	OBJECT_REF_COUNT ref_count;
	char *name;
	char *description;
	//struct _block_object *block;//block in which the exception occured 
	struct _stack *traceback;
	struct _object *exception;//
} exception_object;




#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(pop)				/* restore original alignment from stack */
#endif

void obj_Init(void);

void obj_Close(void);


kv_object *ConvertToKVObject(object *key);

kv_object *ConvertToKVObjectValued(object *key,object *value);

object *AllocObject(void);

exception_object *AllocExceptionObject(void);

block_object *AllocBlockObject(void);

kv_object *AllocKVObject(void);

ref_object *AllocRefObject(void);

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

object *LockObject(object *obj);

void UnlockObject(object *obj);


long ReadLong(stream *f);

FLOAT ReadFloat(stream *f);

char ReadChar(stream *f);

object *ReadObject(stream *f);

object *ReadObjectPlus(stream *f);

void WriteLong(long l,stream *f);

void WriteFloat(FLOAT fl,stream *f);

void WriteChar(char c,stream *f);

void WriteObject(object *obj,stream *f);

void WriteObjectPlus(object *obj,stream *f);

object *DissolveRef(object *obj);

ref_object *CreateRefObject(object *ref_to);

tag_object *CreateTagObject(void *tag);

int_object *CreateIntObject(INT value);

float_object *CreateFloatObject(FLOAT value);

unicode_object *CreateUnicodeObject(char *value);

tuple_object *CreateTuple(NUM num);

string_object *CreateStringObject(char *bytes,NUM len);

kv_object *CreateKVObject(object *key,object *value);

object *CreateEmptyObject(char type);

function_object *CreateFunctionObject_MAKE_FUNCTION(code_object *function_code,tuple_object *defaults,tuple_object *kw_defaults);

function_object *CreateFunctionObject_MAKE_CLOSURE(code_object *function_code,tuple_object *closure,tuple_object *defaults,tuple_object *kw_defaults);

function_object *CreateFunctionObject(code_object *co);

cfunction_object *CreateCFunctionObject(struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals));//,tuple_object *defaults,tuple_object *kw_defaults);//used for in-python storage of external calls

method_object *CreateMethodObject(object *func,class_instance_object *instance);

class_object *CreateClassObject(code_object *code,object *base_classes);//char *name,

code_object *CreateCodeObject(char *name);

class_instance_object *CreateClassInstanceObject(class_object *instance_of);

exception_object *CreateExceptionObject(char *name,char *description,struct _stack *blocks,struct _object *exception);

iter_object *CreateIterObject(void);

void PrintObject(object *obj);

void FullDumpObject(object * obj, char level);

#ifdef USE_DEBUGGING
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

object *GetAttribute(object *obj,object *key);

//object *GetAttributeByName(object *obj,char *name);

void SetAttribute(object *obj,object *key,object *value);

object *GetClassMethod(object *class,object *key);

object *GetClassVar(object *class,object *key);

object *CopyObject(object *obj);

BOOL CompareObjects(object *a,object *b);

void AppendDictItem(object *tuple,object *key,object *value);

void AppendItem(object *tuple,object *value);

void InsertItem(object *tuple,INDEX index,object *value);

void ClearDictValues(object *tuple);

void DeleteItem(object *tuple, INDEX index);

void DeleteDictItem(object *tuple,object *key);

void AddCodeFunction(object *co,char *name,object *func);

void AddCodeCFunction(object *co,char *name,	struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals));


#ifdef __cplusplus
} 
#endif

#endif


