#ifndef MEMORY_H
#define MEMORY_H

#include "stdio.h"
#include "stdlib.h"

void mem_Init();

void *mem_malloc(size_t size);
void mem_free(void *ptr);


#endif
