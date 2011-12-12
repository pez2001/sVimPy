#include "memory.h"

long mem_chunks_num = 0;
long mem_chunks_max_size = 0;
long mem_chunks_actual_size = 0;


mem_chunk **mem_chunk_items;
long mem_chunks_top = 0;

void mem_Init()
{
mem_chunk_items = (mem_chunk**)malloc(2000*sizeof(mem_chunk*));
mem_chunks_top = 0;


}

void mem_Close()
{
for(int i=0;i<mem_chunks_top;i++)
{
 if(!mem_chunk_items[i]->is_freed)
 {
  printf("leaked chunk(%d) @%x : ",mem_chunk_items[i]->size,mem_chunk_items[i]->ptr);
   for(int ix=0;ix<mem_chunk_items[i]->size;ix++)
 {
  printf("%x ",*((unsigned char*)(mem_chunk_items[i]->ptr+ix)));
 }
 printf("\n");

  }
 free(mem_chunk_items[i]);
 
 }
free(mem_chunk_items);

printf("not freed heap bytes num:%d\n",mem_chunks_actual_size);
printf("MAX HEAP USAGE:%d\n",mem_chunks_max_size);

}

void mem_Push(void *x,long size)
{
mem_chunk_items[mem_chunks_top] = (mem_chunk*)malloc(sizeof(mem_chunk));
mem_chunk_items[mem_chunks_top]->ptr = x;
mem_chunk_items[mem_chunks_top]->size = size;
mem_chunk_items[mem_chunks_top]->is_freed = 0;
mem_chunks_top++;

long tmp_mem_chunks_max_size = 0;
for(int i=0;i<mem_chunks_top;i++)
{
if(!mem_chunk_items[i]->is_freed)
 tmp_mem_chunks_max_size += mem_chunk_items[i]->size;
}
if(tmp_mem_chunks_max_size > mem_chunks_max_size)
 mem_chunks_max_size = tmp_mem_chunks_max_size;
mem_chunks_actual_size = tmp_mem_chunks_max_size;
//printf("actual heap usage:%d\n",mem_chunks_actual_size);
}


void *mem_realloc(void *ptr,size_t size)
{
for(int i=0;i<mem_chunks_top;i++)
{
 if(mem_chunk_items[i]->ptr == ptr && !mem_chunk_items[i]->is_freed)
 {
  ptr = realloc(ptr,size);
  mem_chunk_items[i]->ptr = ptr;
  mem_chunks_actual_size +=  size  - mem_chunk_items[i]->size;
  if(mem_chunks_actual_size > mem_chunks_max_size)
   mem_chunks_max_size = mem_chunks_actual_size;
  mem_chunk_items[i]->size = size;
  return(ptr);
  }
}
 printf("chunk not found @%x\n",ptr);
 return(NULL);
}

void *mem_malloc(size_t size)
{
mem_chunks_num++;
void *tmp = malloc(size);
 if(size == 0)
{
 printf("allocated zero bytes\n");
 //return(NULL);
 }
 //else
 //printf("allocated %d bytes @%x\n",size,tmp);

mem_Push(tmp,size);
return(tmp);
}
void mem_free(void *ptr)
{
int f = 0;
for(int i=0;i<mem_chunks_top;i++)
{
 if(mem_chunk_items[i]->ptr == ptr && !mem_chunk_items[i]->is_freed)
 {
  f =1;
  mem_chunk_items[i]->is_freed = 1;
  }
}
if(!f)
 printf("chunk not found @%x\n",ptr);
mem_chunks_num--;
if(mem_chunks_num<0)
 printf("more memory freed than allocated\n");
free(ptr);

long tmp_mem_chunks_max_size = 0;
for(int i=0;i<mem_chunks_top;i++)
{
if(!mem_chunk_items[i]->is_freed)
 tmp_mem_chunks_max_size += mem_chunk_items[i]->size;
}
mem_chunks_actual_size = tmp_mem_chunks_max_size;
//printf("actual heap usage:%d\n",mem_chunks_actual_size);
//printf("freed bytes @%x\n",ptr);
}
