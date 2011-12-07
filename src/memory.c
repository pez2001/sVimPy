#include "memory.h"

long mem_chunks_num = 0;
//long chunks_total_size;


void *mem_malloc(size_t size)
{
mem_chunks_num++;
return(malloc(size));
}
void mem_free(void *ptr)
{
mem_chunks_num--;
free(ptr);
}
