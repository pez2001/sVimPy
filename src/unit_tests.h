#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include "vm.h"
#include "object.h"
#include "memory.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern long mem_chunks_num;
extern long mem_chunks_actual_size;
extern long mem_chunks_max_size;

extern long objects_num;
extern long objects_max;


#define MAGIC (3180 | ((long)'\r'<<16) | ((long)'\n'<<24))
#define TAG "cpython-32"
#define CACHEDIR "__pycache__"
/* Current magic word and string tag as globals. */
static long pyc_magic = MAGIC;
static const char *pyc_tag = TAG;
static short pyc_magic_short = 3180;




#endif
