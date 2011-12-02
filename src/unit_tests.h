#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H
#include "vm.h"


#include "stdio.h"
#include "stdlib.h"
#include "string.h"


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



#endif
