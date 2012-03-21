/* 
 * sVimPy - small Virtual interpreting machine for Python
 * (c) 2012 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */

#include "memory.h"

#ifdef USE_DEBUGGING

long mem_chunks_num = 0;
long mem_chunks_max_size = 0;
long mem_chunks_actual_size = 0;
mem_chunk **mem_chunk_items;
long mem_chunks_top = 0;

#define BOUNDS_LEN 1


void mem_Init(void)
{
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	mem_chunk_items =	(mem_chunk**) malloc(1 * sizeof(mem_chunk*));
	mem_chunks_top = 0;
	}
}

void mem_Close(void)
{
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	printf("%d memory allocs recorded\n",mem_chunks_top);
	for (int i = 0; i < mem_chunks_top; i++)
	{
		//debug_printf(DEBUG_MEMORY,"chunk:%d\n",i);
		if (!mem_chunk_items[i]->is_freed)
		{
			printf("found leaked chunk\n");
			printf("leaked chunk: %s (%d) @%x : ", mem_chunk_items[i]->description, mem_chunk_items[i]->size, mem_chunk_items[i]->ptr);
			for (int ix = 0; ix < mem_chunk_items[i]->size; ix++)
			{
				printf("%x ", *((unsigned char *)(mem_chunk_items[i]->ptr + ix)));
			}
			printf("  ");
			for (int ix = 0; ix < mem_chunk_items[i]->size; ix++)
			{
				printf("%c  ",*((unsigned char *)(mem_chunk_items[i]->ptr + ix)));
			}
			printf("\n");

		}
		free(mem_chunk_items[i]);
	}
	debug_printf(DEBUG_MEMORY,"leaked chunks test done\n");
	free(mem_chunk_items);
	mem_chunks_top = 0;
	printf("not freed heap bytes num:%d\n", mem_chunks_actual_size);
	printf("MAX HEAP USAGE:%d\n", mem_chunks_max_size);
	}
}

void mem_Push(void *x, long size, char *description)
{
	mem_chunk_items[mem_chunks_top] = (mem_chunk*)malloc(sizeof(mem_chunk));
	mem_chunk_items[mem_chunks_top]->ptr = x;
	mem_chunk_items[mem_chunks_top]->size = size;
	mem_chunk_items[mem_chunks_top]->description = description;
	mem_chunk_items[mem_chunks_top]->is_freed = 0;
	mem_chunks_top++;
	mem_chunk_items = (mem_chunk**) realloc(mem_chunk_items,(mem_chunks_top +1) * sizeof(mem_chunk*));
	
	long tmp_mem_chunks_max_size = 0;

	for (int i = 0; i < mem_chunks_top; i++)
	{
		if (!mem_chunk_items[i]->is_freed)
			tmp_mem_chunks_max_size += mem_chunk_items[i]->size;
	}
	if (tmp_mem_chunks_max_size > mem_chunks_max_size)
		mem_chunks_max_size = tmp_mem_chunks_max_size;
	mem_chunks_actual_size = tmp_mem_chunks_max_size;
	// printf("actual heap usage:%d\n",mem_chunks_actual_size);
}

void *mem_realloc_cpy(void *ptr,size_t size,size_t old_size)
{
	void *tmp = (void*)malloc(size);
	memcpy(tmp,ptr,size);
	//printf("rea:%x,%d,%d\n",ptr,size,old_size);
	memset(ptr,254,old_size);
	//printf("clr:%x\n",tmp);
	free(ptr);
	return(tmp);
}
void *mem_realloc(void *ptr, size_t size)
{
	if((debug_level & DEBUG_MEMORY) > 0)
	{
		//printf("reallocated memory\n");
		if(size == 0)
			printf("reallocated zero bytes\n");
		for (int i = 0; i < mem_chunks_top; i++)
		{
			if (mem_chunk_items[i]->ptr == ptr && !mem_chunk_items[i]->is_freed)
			{	
				void *s =ptr;
				//s-=BOUNDS_LEN;
				//printf("realloc:%x,%d,%d\n",s,size,mem_chunk_items[i]->size);
				//mem_chunk_items[i]->ptr = mem_realloc_cpy(s, size+(BOUNDS_LEN*2),mem_chunk_items[i]->size+(BOUNDS_LEN*2));
				mem_chunk_items[i]->ptr = mem_realloc_cpy(s, size,mem_chunk_items[i]->size);
				//printf("ret from cpy\n");
				//char *e = mem_chunk_items[i]->ptr;
				//e+=size+BOUNDS_LEN;
				//printf("setting:%x,%d\n",e,BOUNDS_LEN);
				//memset(e,253,BOUNDS_LEN);
				//printf("set:%x,%d\n",e,BOUNDS_LEN);
				//memset(mem_chunk_items[i]->ptr,252,BOUNDS_LEN);
				mem_chunks_actual_size += size - mem_chunk_items[i]->size;
				if (mem_chunks_actual_size > mem_chunks_max_size)
					mem_chunks_max_size = mem_chunks_actual_size;
				mem_chunk_items[i]->size = size;
				//mem_chunk_items[i]->ptr+=BOUNDS_LEN;
				//printf("rea thru:%x\n",mem_chunk_items[i]->ptr);
				return (mem_chunk_items[i]->ptr);
			}
		}
		printf("realloc chunk not found @%x\n", ptr);
		return (NULL);
	}
	else
	{
		return(realloc(ptr, size));
	}
}



void *mem_malloc(size_t size, char *description)
{
	if((debug_level & DEBUG_MEMORY) > 0)
		mem_chunks_num++;
	//void *tmp = malloc(size+(BOUNDS_LEN*2));
	//tmp = *(&tmp + size*3);
	//tmp += size*3;
	//memset(tmp,255,size+(BOUNDS_LEN*2));
	void *tmp = malloc(size);
	//tmp = *(&tmp + size*3);
	//tmp += size*3;
	memset(tmp,255,size);
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	if (size == 0)
	{
		debug_printf(DEBUG_MEMORY,"allocated zero bytes\n");
		// return(NULL);
	}
	//tmp = (void*) *(((char*)tmp)+BOUNDS_LEN);
	// else
	//debug_printf(DEBUG_ALLOCS,"allocated %d bytes @%x\n",size,tmp);
	if((debug_level & DEBUG_ALLOCS)>0)
		printf("allocated %d bytes @%x\n",size,tmp);
	//tmp += BOUNDS_LEN;
	mem_Push(tmp, size, description);
	}
	
	return (tmp);
}

int mem_free(void *ptr)
{
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	int f = 0;
	//ptr = (void*) *(((char*)ptr)-BOUNDS_LEN);
	void *p = ptr;
	//p-=BOUNDS_LEN;
	for (int i = 0; i < mem_chunks_top; i++)
	{
		if (mem_chunk_items[i]->ptr == p && !mem_chunk_items[i]->is_freed)
		{
			f = 1;
			mem_chunk_items[i]->is_freed = 1;
			//memset(ptr,255,mem_chunk_items[i]->size+(BOUNDS_LEN*2));
			memset(ptr,255,mem_chunk_items[i]->size);
			/*unsigned char *bound = (unsigned char*)mem_chunk_items[i]->ptr;
			bound += mem_chunk_items[i]->size+1;
			unsigned char *bound2 = (unsigned char*)mem_chunk_items[i]->ptr;
			bound2 -= 1;
			if(*bound != 255)
			{
				printf("buffer overun detected in alloc: %s (%d) @%x : ", mem_chunk_items[i]->description, mem_chunk_items[i]->size, mem_chunk_items[i]->ptr);
				//printf("buffer overun detected in alloc @ %x\n",ptr);
				for (int ix = mem_chunk_items[i]->size; ix < mem_chunk_items[i]->size+BOUNDS_LEN; ix++)
				{
					printf("%x ", *((unsigned char*)(mem_chunk_items[i]->ptr + ix)));
				}
				printf("  ");
				for (int ix = mem_chunk_items[i]->size; ix < mem_chunk_items[i]->size+BOUNDS_LEN; ix++)
				{
					printf("%c  ",*((unsigned char*)(mem_chunk_items[i]->ptr + ix)));
				}
				printf("\n");
	
			}
			if(*bound2 != 255)
			{
				printf("buffer underrun detected in alloc: %s (%d) @%x : ", mem_chunk_items[i]->description, mem_chunk_items[i]->size, mem_chunk_items[i]->ptr);
				//printf("buffer overun detected in alloc @ %x\n",ptr);
				for (int ix = 0; ix < BOUNDS_LEN; ix++)
				{
					printf("%x ", *((unsigned char*)(mem_chunk_items[i]->ptr + ix)));
				}
				printf("  ");
				for (int ix = 0; ix < BOUNDS_LEN; ix++)
				{
					printf("%c  ",*((unsigned char*)(mem_chunk_items[i]->ptr + ix)));
				}
				printf("\n");
	
			}
			*/
			break;
		}
	}
	// assert(f);
	if (!f)
	{
		debug_printf(DEBUG_MEMORY,"chunk not found @%x\n", p);
		mem_cnf();
		return (0);
	}
	mem_chunks_num--;
	if (mem_chunks_num < 0)
		debug_printf(DEBUG_MEMORY,"more memory freed than allocated\n");
	free(p);
	}
	else
		free(ptr);
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	long tmp_mem_chunks_max_size = 0;

	for (int i = 0; i < mem_chunks_top; i++)
	{
		if (!mem_chunk_items[i]->is_freed)
			tmp_mem_chunks_max_size += mem_chunk_items[i]->size;
	}
	mem_chunks_actual_size = tmp_mem_chunks_max_size;
	// printf("actual heap usage:%d\n",mem_chunks_actual_size);
	// printf("freed bytes @%x\n",ptr);
	}
	return (1);
}

void mem_cnf(void)
{

}

#endif


