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

char * memory = NULL;

long mem_chunks_num = 0;
long mem_locks_num = 0;
long mem_unlocks_num = 0;
long mem_max_locked = 0;
long mem_max_locked_size = 0;
mem_chunk **mem_chunk_items;

MEM_ID *mem_locks;
long mem_locks_top = 0;

void mem_Init(void)
{
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	}
	#endif
	mem_chunk_items =	(mem_chunk**) malloc(1 * sizeof(mem_chunk*));
	mem_chunks_num = 0;
	mem_locks_num = 0;
	mem_unlocks_num = 0;
	mem_locks_top = 0;
	mem_max_locked = 0;
	mem_max_locked_size = 0;
	mem_locks = (MEM_ID*) malloc(1 * sizeof(MEM_ID));
}

MEM_ID mem_AddChunk(void *ptr,char *description,long size)
{
	mem_chunk *chunk = (mem_chunk*)malloc(sizeof(mem_chunk));
	chunk->ptr = ptr;
	chunk->description = description;
	chunk->size = size;
	chunk->is_freed = 0;
	//chunk->is_removed = 0;
	chunk->locks = 0;
	mem_chunk_items = (mem_chunk**) realloc(mem_chunk_items,(mem_chunks_num+1)*sizeof(mem_chunk*));
	mem_chunk_items[mem_chunks_num] = chunk;
	mem_chunks_num++;
	return(mem_chunks_num-1);
}

void mem_PushLock(MEM_ID ptr)
{
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	mem_locks[mem_locks_top] = ptr;
	mem_locks_top++;
	//printf("locked %d\n",ptr);
	if(mem_locks_top > mem_max_locked)
	{
		mem_max_locked = mem_locks_top;
		//printf("locks peaked @ %d\n",mem_locks_top);
	}
	long heap_size = 0;
	for(long i = 0;i<mem_locks_top;i++)
	{
		heap_size += mem_chunk_items[mem_locks[i]]->size;
	}
	if(heap_size > mem_max_locked_size)
	{
		mem_max_locked_size = heap_size;
		//printf("heap peaked @ %d\n",heap_size);
	}
}

void mem_PopLock(MEM_ID ptr)
{
	mem_locks_top--;
	if(mem_locks_top<0)
	{
		printf("Error no locks left to pop\n");
		mem_locks_top = 0;
	}
	else
	{
		if(mem_locks[mem_locks_top] != ptr)
		{
			printf("Error wrong unlock sequence:last lock was %d but ptr is %d (open locks:%d)\n",mem_locks[mem_locks_top],ptr,mem_locks_top);
		//force segfault for gdb debugging  to get the exact location 
			mem_locks = 1;
			if(mem_locks[0] == 'c')
				printf("crash\n");
		}	
		//printf("unlocked %d\n",ptr);
	}
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
}


/*
MEM_ID mem_RemoveChunk(MEM_ID chunk)
{
	
}
*/

BOOL mem_DebugHeapWalk(BOOL show_leaked,BOOL show_locked)
{
	long locked = 0;
	long leaked = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(show_leaked && !mem_chunk_items[i]->is_freed)
		{
			printf("leaked chunk id:%d\ndescription:%s\nsize:%d\nhex:\n",i,mem_chunk_items[i]->description,mem_chunk_items[i]->size);
			for(int b = 0;b<mem_chunk_items[i]->size;b++)
			{
				printf("%2x ",((unsigned char*)mem_chunk_items[i]->ptr)[b]);
			}
			printf("\nraw:\n");
			for(int b = 0;b<mem_chunk_items[i]->size;b++)
			{
				printf("%c",((unsigned char*)mem_chunk_items[i]->ptr)[b]);
			}
			printf("\n");
			leaked++;
		}
		if(show_locked && mem_chunk_items[i]->locks > 0)
		{
			printf("still locked chunk id:%d\ndescription:%s\nsize:%d\nlocks num:%d\n",i,mem_chunk_items[i]->description,mem_chunk_items[i]->size,mem_chunk_items[i]->locks);
			locked++;			
		}
	}
	if(show_locked)
		printf("still locked chunks:%d\n",locked);
	if(show_leaked)
		printf("leaked chunks:%d\n",leaked);
	if(locked || leaked)
		return(1);
	else 
		return(0);
}

BOOL mem_Close(void)
{
	printf("mem chunks allocated:%d\n",mem_chunks_num);
	printf("mem chunks locked:%d\n",mem_locks_num);
	printf("mem chunks unlocked:%d\n",mem_unlocks_num);
	printf("locks peaked @ %d\n",mem_max_locked);
	printf("heap peaked @ %d\n",mem_max_locked_size);
	BOOL r = mem_DebugHeapWalk(1,1);
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(!mem_chunk_items[i]->is_freed)
		{
			free(mem_chunk_items[i]->ptr);
		}
		if(mem_chunk_items[i]->description != NULL)
			free(mem_chunk_items[i]->description);
		free(mem_chunk_items[i]);
	}
	free(mem_chunk_items);
	return(r);
}

MEM_ID mem_malloc(size_t size,MEM_POOL_CLASS_ID pool_class)
{
	void *ptr = malloc(size);
	return(mem_AddChunk(ptr,NULL,size));
}

MEM_ID mem_malloc_debug(size_t size,MEM_POOL_CLASS_ID pool_class, char *description)
{
	void *ptr = malloc(size);
	return(mem_AddChunk(ptr,description,size));
}

MEM_ID mem_realloc(MEM_ID ptr, size_t size)
{
	void *tmp = (void*)malloc(size);
	memcpy(tmp,mem_chunk_items[ptr]->ptr,size);
	free(mem_chunk_items[ptr]->ptr);
	mem_chunk_items[ptr]->ptr = tmp;
	return(ptr);
}

void mem_free(MEM_ID ptr)
{	
	free(mem_chunk_items[ptr]->ptr);
	mem_chunk_items[ptr]->is_freed = 1;
	mem_chunk_items[ptr]->ptr = NULL;
}

void *mem_lock(MEM_ID ptr)
{	
	mem_locks_num++;
	mem_chunk_items[ptr]->locks++;
	mem_PushLock(ptr);
	return(mem_chunk_items[ptr]->ptr);
}


void mem_unlock(MEM_ID ptr,BOOL is_dirty)
{
	mem_unlocks_num++;
	mem_chunk_items[ptr]->locks--;	
	mem_PopLock(ptr);
}

MEM_ID mem_copy(MEM_ID src)
{
	void *tmp = (void*)malloc(mem_chunk_items[src]->size);
	memcpy(tmp,mem_chunk_items[src]->ptr,mem_chunk_items[src]->size);
	return(mem_AddChunk(tmp,"memory copy",mem_chunk_items[src]->size));
}

MEM_ID mem_create(char *bytes,INT len)
{
	void *tmp = (void*)malloc(len);
	memcpy(tmp,(void*)bytes,len);
	return(mem_AddChunk(tmp,"memory create",len));
}

MEM_ID mem_create_buf(INT len)
{
	void *ptr = malloc(len);
	memset((void*)ptr,0,len);
	return(mem_AddChunk(ptr,"memory create buffer",len));

}

MEM_ID mem_create_string(char *string)
{
	void *tmp = (void*)malloc(strlen(string) + 1);
	memcpy(tmp,(void*)string,strlen(string) + 1);
	return(mem_AddChunk(tmp,"memory create string",strlen(string) + 1));
}

BOOL mem_compare(MEM_ID a,MEM_ID b)
{
	long len = mem_chunk_items[a]->size;
	long len2 = mem_chunk_items[b]->size;
	if(len != len2)
		return(0);
	return(!memcmp((void*)mem_chunk_items[a]->ptr,(void*)mem_chunk_items[b]->ptr,len));
}

void mem_set(MEM_ID src,char value,INT len)
{
	memset(mem_chunk_items[src]->ptr,value,len);
}

BOOL mem_copy_to(MEM_ID dst,char *bytes,INT len)
{
	memcpy(mem_chunk_items[dst]->ptr,(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from(MEM_ID src,char *bytes,INT len)
{
	memcpy(bytes,mem_chunk_items[src]->ptr,len);
	return(1);
}

BOOL mem_copy_to_offset(MEM_ID dst,NUM dst_offset,char *bytes,INT len)
{
	memcpy((void*)(((char*)mem_chunk_items[dst]->ptr)+dst_offset),(void*)bytes,len);
	return(1);
}


BOOL mem_copy_from_offset(MEM_ID src,NUM src_offset,char *bytes,INT len)
{
	memcpy((void*)bytes,(void*)(((char*)mem_chunk_items[src]->ptr)+src_offset),len);
	return(1);
}

/*

#define BOUNDS_LEN 1

void mem_Init(void)
{
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
				printf("%x ", *((unsigned char *)((unsigned char*)mem_chunk_items[i]->ptr + ix)));
			}
			printf("  ");
			for (int ix = 0; ix < mem_chunk_items[i]->size; ix++)
			{
				printf("%c  ",*((unsigned char *)((unsigned char*)mem_chunk_items[i]->ptr + ix)));
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
	//printf("memory pushed:%d(%d)\n",mem_chunks_top,size);
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
				//printf("memory changed:%d(%d)\n",i,size);
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
			//printf("memory freed:%d(%d)\n",i,mem_chunk_items[i]->size);
			mem_chunk_items[i]->is_freed = 1;
			//memset(ptr,255,mem_chunk_items[i]->size+(BOUNDS_LEN*2));
			memset(ptr,255,mem_chunk_items[i]->size);
			
			//
			unsigned char *bound = (unsigned char*)mem_chunk_items[i]->ptr;
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
			//
			break;
		}
	}
	// assert(f);
	if (!f)
	{
		debug_printf(DEBUG_MEMORY,"chunk not found @%x\n", p);
		//mem_cnf();
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
*/


