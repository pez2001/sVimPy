#ifndef OBJECT_H
#define OBJECT_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "memory.h"

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


#define OFLAG_USES_VALUE_PTR 1
#define OFLAG_IS_NAME 2
#define OFLAG_IS_CONST 4
#define OFLAG_IS_VARNAME 8

//internal types
#define TYPE_FUNCTION 'f'
#define TYPE_CALLER 'C'




typedef struct {
char type;
char *name;
void *ptr;
void *value_ptr;
unsigned int flags;
}object;

typedef struct {
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
object *filename;
//object *name;
//char *name;
//long codesize;
long firstlineno;
object *lnotab;
}code_object;

typedef struct {
char *content;
long len;
}string_object;

typedef struct {
object **items;
long num;
}tuple_object;

typedef struct {
char *content;
long len;
}unicode_object;

typedef struct {
object *module;
long pos;
}caller_object;

typedef struct {
object *code;
//long pos;
}function_object;



object *AllocObject();
string_object *AllocStringObject();
tuple_object *AllocTupleObject();
unicode_object *AllocUnicodeObject();
code_object *AllocCodeObject();
caller_object *AllocCallerObject();
function_object *AllocFunctionObject();
object *ReadObject(FILE *f);

string_object *AsStringObject(object *obj);
code_object *AsCodeObject(object *obj);
caller_object *AsCallerObject(object *obj);
function_object *AsFunctionObject(object *obj);
tuple_object *AsTupleObject(object *obj);
unicode_object *AsUnicodeObject(object *obj);

void FreeObject(object *obj);


#endif