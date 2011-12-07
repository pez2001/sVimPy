#include "memory.h"

long mem_chunks_num = 0;
//long chunks_total_size;


void **mem_chunk_items;
long mem_chunks_top = 0;

void mem_Init()
{
mem_chunk_items = (void**)malloc(1000*sizeof(void*));
mem_chunks_top = 0;


}

void mem_Close()
{
//for(int i=0;i<mem_chunks_top;i++)

 free(mem_chunk_items);

}

void mem_Push(void *x)
{
mem_chunk_items[mem_chunks_top] = x;
mem_chunks_top++;
}



void *mem_malloc(size_t size)
{
mem_chunks_num++;
void *tmp = malloc(size);
mem_Push(tmp);
return(tmp);
}
void mem_free(void *ptr)
{
int f = 0;
for(int i=0;i<mem_chunks_top;i++)
{
 if(mem_chunk_items[i] == ptr)
 {
  f =1;
  mem_chunk_items[i] = NULL;
  }
}
if(!f)
 printf("chunk not found @%x\n",ptr);
mem_chunks_num--;
free(ptr);
}
