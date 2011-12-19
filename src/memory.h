#ifndef MEMORY_H
#define MEMORY_H

#include "stdio.h"
#include "stdlib.h"

#include "assert.h"

typedef struct {
void *ptr;
char *description;
long size;
int is_freed;
}mem_chunk;

void mem_Init();

void *mem_malloc(size_t size,char *description);
void *mem_realloc(void *ptr,size_t size);
int mem_free(void *ptr);


#endif
