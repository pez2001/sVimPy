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


#endif
