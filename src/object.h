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

#include "assert.h"

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


//#define OFLAG_USES_VALUE_PTR 1
//#define OFLAG_IS_NAME 2
//#define OFLAG_IS_CONST 4
//#define OFLAG_IS_VARNAME 8


#define OFLAG_ON_STACK 1
#define OFLAG_UNLOADED 2	//set if object was unloaded -> obj->ptr == seek_pos // or just dump to file and reread if accessed

#define OFLAG_HOLD_IN_MEMORY 4
#define OFLAG_HAS_VALUE_PTR 8
#define OFLAG_TUPLE_PTR 16	//used to iterate over tuples
#define OFLAG_TUPLE_RESTART_FLAG 32	//used to iterate over tuples


//internal types
#define TYPE_FUNCTION 'f'
#define TYPE_CALLER 'C'
#define TYPE_BLOCK 'b'



#pragma pack(push)		/* push current alignment to stack */
#pragma pack(1)			/* set alignment to 1 byte boundary */

typedef struct
{
  char type;
  unsigned char flags;
  void *ptr;
  void *value_ptr;		//TODO remove this member --> TO DECREASE MEMORY USAGE
} object;

typedef struct
{
  char type;
  unsigned char flags;
  void *ptr;
  void *value_ptr;
} valued_object;		//TO OPTIMIZE MEMORY USAGE -> only used in tuples 

typedef struct
{
  char type;
  unsigned char flags;
} empty_object;			//TO OPTIMIZE MEMORY USAGE

typedef struct
{
  long argcount;
  long kwonlyargcount;
  long nlocals;
  long stacksize;
  long flags;
  object *code;
  object *consts;
  object *names;
  object *varnames;
  object *freevars;
  object *cellvars;
//object *filename;
//object *name;
  char *name;
//long codesize;
  long firstlineno;
//object *lnotab;
} code_object;

typedef struct
{
  char *content;
  long len;
} string_object;

typedef struct
{
  object **items;
  long num;
} tuple_object;

/*
typedef struct {
char *content;
//long len;//TO DECREASE MEMORY USAGE
     }unicode_object;
*///TO DECREASE MEMORY USAGE

typedef struct
{
  object *module;
  long pos;
} caller_object;

typedef struct
{
  object *code;
//long pos;
} function_object;


typedef struct
{
  object *code;
  long start;
  long len;
  object *iter;
  long ip;
} block_object;



#pragma pack(pop)		/* restore original alignment from stack */

object *AllocObject ();

object *AllocEmptyObject ();

object *AllocValuedObject ();

string_object *AllocStringObject ();

tuple_object *AllocTupleObject ();

//unicode_object *AllocUnicodeObject();//TO DECREASE MEMORY USAGE
code_object *AllocCodeObject ();

caller_object *AllocCallerObject ();

function_object *AllocFunctionObject ();

block_object *AllocBlockObject ();

object *ReadObject (FILE * f);

string_object *AsStringObject (object * obj);

code_object *AsCodeObject (object * obj);

caller_object *AsCallerObject (object * obj);

function_object *AsFunctionObject (object * obj);

tuple_object *AsTupleObject (object * obj);

//unicode_object *AsUnicodeObject(object *obj);

int IsIntObject (object * obj);

int IsStringObject (object * obj);

int IsUnicodeObject (object * obj);

int IsCodeObject (object * obj);

int IsTupleObject (object * obj);

void FreeObject (object * obj);

void PrintObject (object * obj);

void DumpObject (object * obj, int level);

//void DumpObject(object *obj);
object *GetNextItem (object * tuple);

void ResetIteration (object * tuple);

void SetItem (object * tuple, int index, object * obj);

object *GetItem (object * tuple, int index);

//object *GetTupleItem(tuple_object *tuple,int index);
object *FindUnicodeTupleItem (object * tuple, char *name);

#endif
