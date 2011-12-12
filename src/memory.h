#ifndef MEMORY_H
#define MEMORY_H

#include "stdio.h"
#include "stdlib.h"

typedef struct {
void *ptr;
long size;
int is_freed;
}mem_chunk;

void mem_Init();

void *mem_malloc(size_t size);
void *mem_realloc(void *ptr,size_t size);
void mem_free(void *ptr);


#endif
