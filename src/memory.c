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


//using defines for speeding up the lowest level functions

#ifdef USE_MEMORY_MANAGER_PASS

void mem_Init(void)
{	
}

void mem_Dump(MEM_ID mem)
{
			printf("chunk id:%d\nsize:%d\nhex:\n",mem,mem_get_size(mem));
			for(int b = 0;b<mem_get_size(mem);b++)
			{
				printf("%2x ",((unsigned char*)mem)[b]);
			}
			printf("\nraw:\n");
			for(int b = 0;b<mem_get_size(mem);b++)
			{
				printf("%c",((unsigned char*)mem)[b]);
			}
			printf("\n");

}

BOOL mem_Close(void)
{
	return(1);
}

MEM_ID mem_malloc(MEM_NUM size,MEM_POOL_CLASS_ID pool_class)
{
	void *tmp = malloc(size+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = size;
	return((char*)tmp+sizeof(MEM_NUM));
}

MEM_ID mem_realloc(MEM_ID ptr, MEM_NUM size)
{
	void *tmp = realloc((char*)ptr-sizeof(MEM_NUM),size+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = size;
	return((char*)tmp+sizeof(MEM_NUM));
}

void mem_free(MEM_ID ptr)
{	
	free((char*)ptr-sizeof(MEM_NUM));
}

MEM_NUM mem_get_size(MEM_ID ptr)
{
	MEM_NUM size = (MEM_NUM*)((char*)ptr-sizeof(MEM_NUM))[0];
	//printf("memory size:%d\n",size);
	//(void*) *(((char*)ptr)-4);	
	return(size);
} 

void *mem_get_ptr(MEM_ID ptr)
{
	return(ptr);
}

void *mem_lock(MEM_ID ptr)
{	
	return(ptr);
}

void mem_unlock(MEM_ID ptr,BOOL is_dirty)
{
	return;
}

MEM_ID mem_copy(MEM_ID src)
{
	void *tmp = (void*)malloc(mem_get_size(src)+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = mem_get_size(src);
	memcpy((char*)tmp+sizeof(MEM_NUM),src,mem_get_size(src));
	return((char*)tmp+sizeof(MEM_NUM));
}

MEM_ID mem_create(char *bytes,MEM_NUM len)
{
	void *tmp = (void*)malloc(len+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = len;
	memcpy((char*)tmp+sizeof(MEM_NUM),(void*)bytes,len);
	return((char*)tmp+sizeof(MEM_NUM));
}

MEM_ID mem_create_buf(MEM_NUM len)
{
	void *tmp = malloc(len+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = len;
	memset((char*)tmp+sizeof(MEM_NUM),0,len);
	return((char*)tmp+sizeof(MEM_NUM));
}

MEM_ID mem_create_string(char *string)
{
	void *tmp = (void*)malloc(strlen(string) + 1 + sizeof(MEM_NUM));
	//((MEM_NUM*)tmp)[0] = strlen(string) + 1 + sizeof(MEM_NUM);
	((MEM_NUM*)tmp)[0] = strlen(string) + 1;
	memcpy((char*)tmp+sizeof(MEM_NUM),(void*)string,strlen(string) + 1);
	return((char*)tmp+sizeof(MEM_NUM));
}

BOOL mem_compare(MEM_ID a,MEM_ID b)
{
	MEM_NUM len = mem_get_size(a);
	MEM_NUM len2 = mem_get_size(b);
	if(len != len2)
		return(0);
	return(!memcmp(a,b,len));
}

void mem_set(MEM_ID src,char value,MEM_NUM len)
{
	memset(src,value,len);
}

BOOL mem_copy_to(MEM_ID dst,char *bytes,MEM_NUM len)
{
	memcpy(dst,(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from(MEM_ID src,char *bytes,MEM_NUM len)
{
	memcpy(bytes,src,len);
	return(1);
}

BOOL mem_copy_to_offset(MEM_ID dst,MEM_NUM dst_offset,char *bytes,MEM_NUM len)
{
	memcpy((void*)(((char*)dst)+dst_offset),(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from_offset(MEM_ID src,MEM_NUM src_offset,char *bytes,MEM_NUM len)
{
	memcpy((void*)bytes,(void*)(((char*)src)+src_offset),len);
	return(1);
}

#endif

#ifdef USE_MEMORY_MANAGER_CACHE_DEBUG
long mem_chunks_num = 0;
long mem_locks_num = 0;
long mem_unlocks_num = 0;
long mem_max_locked = 0;
long mem_max_chunk_size = 0;
long mem_max_locked_size = 0;
mem_chunk **mem_chunk_items;

MEM_ID *mem_locks;
long mem_locks_top = 0;

cache_store *cache = NULL;


BOOL mem_SetCache(cache_store *store)
{
	cache = store;
	return(mem_store_Open(cache));
}

void mem_Init(void)
{
	mem_chunk_items =	(mem_chunk**) malloc(1 * sizeof(mem_chunk*));
	mem_chunks_num = 0;
	mem_locks_num = 0;
	mem_unlocks_num = 0;
	mem_locks_top = 0;
	mem_max_locked = 0;
	mem_max_chunk_size = 0;
	mem_max_locked_size = 0;
	mem_locks = (MEM_ID*) malloc(1 * sizeof(MEM_ID));
	mem_store_InitCacheTypes();
}

void mem_set_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class)
{
	mem_chunk_items[mem]->pool_class |= pool_class;

}

void mem_unset_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class)
{
	mem_chunk_items[mem]->pool_class &= ~pool_class; //TODO ?
}

MEM_ID mem_AddChunk(void *ptr,char *description,MEM_NUM size,unsigned char pool_class)
{
	mem_chunk *chunk = (mem_chunk*)malloc(sizeof(mem_chunk));
	chunk->ptr = ptr;
	chunk->description = description;
	chunk->size = size;
	chunk->is_freed = 0;
	chunk->pool_class = pool_class;
	//chunk->is_removed = 0;
	chunk->locks = 0;
	//printf("allocated chunk @ %x\r\n",chunk);
	mem_chunk_items = (mem_chunk**) realloc(mem_chunk_items,(mem_chunks_num+1)*sizeof(mem_chunk*));
	//printf("increased chunks list for chunk: %d\r\n",mem_chunks_num);
	mem_chunk_items[mem_chunks_num] = chunk;
	mem_chunks_num++;
	return(mem_chunks_num-1);
}

void mem_PushLock(MEM_ID ptr)
{
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	mem_locks[mem_locks_top] = ptr;
	mem_locks_top++;
	if(ptr == 0)
	{
				//printf("WARNING locking NULL\n");
				//mem_locks = (void*)1;
				//if(mem_locks[0] == 'c')
				//	printf("crash\n");	
	}
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
			mem_locks = (void*)1;
			if(mem_locks[0] == 'c')
				printf("crash\n");
		}	
		//printf("unlocked %d\n",ptr);
	}
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	//check if ptr in locks
}

BOOL mem_HasLocks(MEM_ID ptr)
{
	BOOL r = 0;
	for(long i = 0;i<mem_locks_top;i++)
	{
		if(ptr == mem_locks[i])
			r = 1;
	}
	return(r);
}

void mem_Dump(MEM_ID mem)
{
			printf("chunk id:%d\ndescription:%s\nsize:%d\nhex:\n",mem,mem_chunk_items[mem]->description,mem_chunk_items[mem]->size);
			for(int b = 0;b<mem_chunk_items[mem]->size;b++)
			{
				printf("%2x ",((unsigned char*)mem_chunk_items[mem]->ptr)[b]);
			}
			printf("\nraw:\n");
			for(int b = 0;b<mem_chunk_items[mem]->size;b++)
			{
				printf("%c",((unsigned char*)mem_chunk_items[mem]->ptr)[b]);
			}
			printf("\n");

}

BOOL mem_DebugHeapWalk(BOOL show_leaked,BOOL show_locked)
{
	long locked = 0;
	long leaked = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(show_leaked && !mem_chunk_items[i]->is_freed )
		{
			printf("leaked ");
			mem_Dump(i);
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
		return(0);
	else 
		return(1);
}

INT mem_DebugHeapWalkSize(void)
{
	long locked = 0;
	INT locked_size = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(mem_chunk_items[i]->locks > 0)
		{
			//printf("lock size:%d\n",mem_chunk_items[i]->size);
			locked_size = locked_size + mem_chunk_items[i]->size;
			locked++;			
		}
	}
	//printf("locked:%d,size:%d\n",locked,locked_size);
	return(locked_size);
}

BOOL mem_Close(void)
{
	if(cache != NULL)
	{
		printf("closing memory store\n");
		mem_store_Close(cache);
		printf("freeing memory store\n");
		mem_store_Free(cache);
	}
	printf("freeing cache types\n");
	mem_store_FreeCacheTypes();
	printf("mem chunks allocated:%d\n",mem_chunks_num);
	printf("mem max chunk size:%d\n",mem_max_chunk_size);
	printf("mem chunks locked:%d\n",mem_locks_num);
	printf("mem chunks unlocked:%d\n",mem_unlocks_num);
	printf("locks peaked @ %d\n",mem_max_locked);
	printf("heap peaked @ %d\n",mem_max_locked_size);
	BOOL r = mem_DebugHeapWalk(1,1);
	//BOOL r = 0;
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
	free(mem_locks);
	return(r);
}

MEM_ID mem_malloc(MEM_NUM size,MEM_POOL_CLASS_ID pool_class)
{
	void *ptr = malloc(size);
	//printf("allocated @ %x\r\n",ptr);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	return(mem_AddChunk(ptr,NULL,size,pool_class));
}

MEM_ID mem_malloc_debug(MEM_NUM size,MEM_POOL_CLASS_ID pool_class, char *description)
{
	void *ptr = malloc(size);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	return(mem_AddChunk(ptr,description,size,pool_class));
}

char *mem_malloc_ptr(MEM_NUM size,MEM_ID ptr)
{
	char *tmp = (char*)malloc(size);
	mem_set_ptr(ptr,tmp);
	mem_set_size(ptr,size);
	return(tmp);
}

MEM_ID mem_realloc(MEM_ID ptr, MEM_NUM size)
{
	if(cache != NULL && mem_chunk_items[ptr]->is_freed && mem_chunk_items[ptr]->locks == 0 && (mem_chunk_items[ptr]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,ptr);
	}	
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	void *tmp = (void*)malloc(size);
	size_t msize = mem_chunk_items[ptr]->size;
	if(msize > size) //make memory move fitting the new chunk size
		msize = size;
		
	//printf("realloc:ptr:%d, @%d,%d to %d\n",ptr,mem_chunk_items[ptr]->ptr,mem_chunk_items[ptr]->size,tmp);
	memcpy(tmp,mem_chunk_items[ptr]->ptr,msize);
	free(mem_chunk_items[ptr]->ptr);
	mem_chunk_items[ptr]->ptr = tmp;
	mem_chunk_items[ptr]->size = size;	
	return(ptr);
}

void mem_free(MEM_ID ptr)
{	
	if(!mem_chunk_items[ptr]->is_freed)
	{
		free(mem_chunk_items[ptr]->ptr);
		mem_chunk_items[ptr]->is_freed = 1;
		mem_chunk_items[ptr]->ptr = NULL;
	}
}

MEM_NUM mem_get_size(MEM_ID ptr)
{
	return(mem_chunk_items[ptr]->size);
} 

void *mem_get_ptr(MEM_ID ptr)
{
	return(mem_chunk_items[ptr]->ptr);
} 

void mem_set_ptr(MEM_ID ptr,void *dst)
{
	mem_chunk_items[ptr]->ptr = dst;
	mem_chunk_items[ptr]->is_freed = 0;
}

void mem_set_size(MEM_ID ptr,MEM_NUM size)
{
	mem_chunk_items[ptr]->size = size;
}

void *mem_lock(MEM_ID ptr)
{	
	mem_locks_num++;
	if(cache != NULL && mem_chunk_items[ptr]->is_freed && mem_chunk_items[ptr]->locks == 0 && (mem_chunk_items[ptr]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,ptr);
	}	

	mem_chunk_items[ptr]->locks++;
	mem_PushLock(ptr);
	//mem_DebugHeapWalkSize();
	return(mem_chunk_items[ptr]->ptr);
}

void mem_unlock(MEM_ID ptr,BOOL is_dirty)
{
	mem_unlocks_num++;
	mem_chunk_items[ptr]->locks--;	
	mem_PopLock(ptr);
	if(is_dirty && cache != NULL && mem_chunk_items[ptr]->locks == 0 && (mem_chunk_items[ptr]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{	
		mem_store_Write(cache,ptr);
		mem_free(ptr);
	}
}

MEM_ID mem_copy(MEM_ID src)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	void *tmp = (void*)malloc(mem_chunk_items[src]->size);
	memcpy(tmp,mem_chunk_items[src]->ptr,mem_chunk_items[src]->size);
	return(mem_AddChunk(tmp,"memory copy",mem_chunk_items[src]->size,mem_chunk_items[src]->pool_class));
}

MEM_ID mem_create(char *bytes,MEM_NUM len)
{
	void *tmp = (void*)malloc(len);
	memcpy(tmp,(void*)bytes,len);
	return(mem_AddChunk(tmp,"memory create",len,MEM_POOL_CLASS_DYNAMIC));
}

MEM_ID mem_add(char *bytes,MEM_NUM len)
{
	return(mem_AddChunk(bytes,"memory add",len,MEM_POOL_CLASS_STATIC));
}

MEM_ID mem_create_buf(MEM_NUM len)
{
	void *ptr = malloc(len);
	memset((void*)ptr,0,len);
	return(mem_AddChunk(ptr,"memory create buffer",len,MEM_POOL_CLASS_BUFFER));

}

MEM_ID mem_create_string(char *string)
{
	void *tmp = (void*)malloc(strlen(string) + 1);
	memcpy(tmp,(void*)string,strlen(string) + 1);
	return(mem_AddChunk(tmp,"memory create string",strlen(string) + 1,MEM_POOL_CLASS_VAR));
}

BOOL mem_compare(MEM_ID a,MEM_ID b)
{
	if(cache != NULL && mem_chunk_items[a]->is_freed && mem_chunk_items[a]->locks == 0 && (mem_chunk_items[a]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,a);
	}	
	if(cache != NULL && mem_chunk_items[b]->is_freed && mem_chunk_items[b]->locks == 0 && (mem_chunk_items[b]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,b);
	}	
	long len = mem_chunk_items[a]->size;
	long len2 = mem_chunk_items[b]->size;
	if(len != len2)
		return(0);
	return(!memcmp((void*)mem_chunk_items[a]->ptr,(void*)mem_chunk_items[b]->ptr,len));
}

void mem_set(MEM_ID src,char value,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	memset(mem_chunk_items[src]->ptr,value,len);
}

BOOL mem_copy_to(MEM_ID dst,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[dst]->is_freed && mem_chunk_items[dst]->locks == 0 && (mem_chunk_items[dst]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,dst);
	}	
	memcpy(mem_chunk_items[dst]->ptr,(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from(MEM_ID src,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	memcpy(bytes,mem_chunk_items[src]->ptr,len);
	return(1);
}

BOOL mem_copy_to_offset(MEM_ID dst,MEM_NUM dst_offset,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[dst]->is_freed && mem_chunk_items[dst]->locks == 0 && (mem_chunk_items[dst]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,dst);
	}	
	memcpy((void*)(((char*)mem_chunk_items[dst]->ptr)+dst_offset),(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from_offset(MEM_ID src,MEM_NUM src_offset,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	memcpy((void*)bytes,(void*)(((char*)mem_chunk_items[src]->ptr)+src_offset),len);
	return(1);
}

#endif

#ifdef USE_MEMORY_MANAGER_DEBUG
long mem_chunks_num = 0;
long mem_locks_num = 0;
long mem_unlocks_num = 0;
long mem_max_locked = 0;
long mem_max_chunk_size = 0;
long mem_max_locked_size = 0;
mem_chunk **mem_chunk_items;

MEM_ID *mem_locks;
long mem_locks_top = 0;

void mem_Init(void)
{
	mem_chunk_items =	(mem_chunk**) malloc(1 * sizeof(mem_chunk*));
	mem_chunks_num = 0;
	mem_locks_num = 0;
	mem_unlocks_num = 0;
	mem_locks_top = 0;
	mem_max_locked = 0;
	mem_max_chunk_size = 0;
	mem_max_locked_size = 0;
	mem_locks = (MEM_ID*) malloc(1 * sizeof(MEM_ID));
}

void mem_set_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class)
{
	mem_chunk_items[mem]->pool_class |= pool_class;

}

void mem_unset_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class)
{
	mem_chunk_items[mem]->pool_class &= ~pool_class; //TODO ?
}

MEM_ID mem_AddChunk(void *ptr,char *description,MEM_NUM size,unsigned char pool_class)
{
	mem_chunk *chunk = (mem_chunk*)malloc(sizeof(mem_chunk));
	chunk->ptr = ptr;
	chunk->description = description;
	chunk->size = size;
	chunk->is_freed = 0;
	chunk->pool_class = pool_class;
	//chunk->is_removed = 0;
	chunk->locks = 0;
	//printf("allocated chunk @ %x\r\n",chunk);
	mem_chunk_items = (mem_chunk**) realloc(mem_chunk_items,(mem_chunks_num+1)*sizeof(mem_chunk*));
	//printf("increased chunks list for chunk: %d\r\n",mem_chunks_num);
	mem_chunk_items[mem_chunks_num] = chunk;
	mem_chunks_num++;
	return(mem_chunks_num-1);
}

void mem_PushLock(MEM_ID ptr)
{
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	mem_locks[mem_locks_top] = ptr;
	mem_locks_top++;
	if(ptr == 0)
	{
				//printf("WARNING locking NULL\n");
				//mem_locks = (void*)1;
				//if(mem_locks[0] == 'c')
				//	printf("crash\n");	
	}
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
			mem_locks = (void*)1;
			if(mem_locks[0] == 'c')
				printf("crash\n");
		}	
		//printf("unlocked %d\n",ptr);
	}
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	//check if ptr in locks
}

BOOL mem_HasLocks(MEM_ID ptr)
{
	//BOOL r = 0;
	if(!mem_locks_top)
		return(0);
	for(long i = 0;i<mem_locks_top;i++)
	{
		if(ptr == mem_locks[i])
			//r = 1;
			return(1);
	}
	return(0);
}

void mem_Dump(MEM_ID mem)
{
			printf("chunk id:%d\ndescription:%s\nsize:%d\nhex:\n",mem,mem_chunk_items[mem]->description,mem_chunk_items[mem]->size);
			for(int b = 0;b<mem_chunk_items[mem]->size;b++)
			{
				printf("%2x ",((unsigned char*)mem_chunk_items[mem]->ptr)[b]);
			}
			printf("\nraw:\n");
			for(int b = 0;b<mem_chunk_items[mem]->size;b++)
			{
				printf("%c",((unsigned char*)mem_chunk_items[mem]->ptr)[b]);
			}
			printf("\n");

}

BOOL mem_DebugHeapWalk(BOOL show_leaked,BOOL show_locked)
{
	long locked = 0;
	long leaked = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(show_leaked && !mem_chunk_items[i]->is_freed )
		{
			printf("leaked ");
			mem_Dump(i);
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
		return(0);
	else 
		return(1);
}

INT mem_DebugHeapWalkSize(void)
{
	long locked = 0;
	INT locked_size = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(mem_chunk_items[i]->locks > 0)
		{
			//printf("lock size:%d\n",mem_chunk_items[i]->size);
			locked_size = locked_size + mem_chunk_items[i]->size;
			locked++;			
		}
	}
	//printf("locked:%d,size:%d\n",locked,locked_size);
	return(locked_size);
}

BOOL mem_Close(void)
{
	printf("mem chunks allocated:%d\n",mem_chunks_num);
	printf("mem max chunk size:%d\n",mem_max_chunk_size);
	printf("mem chunks locked:%d\n",mem_locks_num);
	printf("mem chunks unlocked:%d\n",mem_unlocks_num);
	printf("locks peaked @ %d\n",mem_max_locked);
	printf("heap peaked @ %d\n",mem_max_locked_size);
	BOOL r = mem_DebugHeapWalk(1,1);
	//BOOL r = 0;
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

MEM_ID mem_malloc(MEM_NUM size,MEM_POOL_CLASS_ID pool_class)
{
	void *ptr = malloc(size);
	//printf("allocated @ %x\r\n",ptr);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	return(mem_AddChunk(ptr,NULL,size,pool_class));
}

MEM_ID mem_malloc_debug(MEM_NUM size,MEM_POOL_CLASS_ID pool_class, char *description)
{
	void *ptr = malloc(size);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	return(mem_AddChunk(ptr,description,size,pool_class));
}

MEM_ID mem_realloc(MEM_ID ptr, MEM_NUM size)
{
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	void *tmp = (void*)malloc(size);
	size_t msize = mem_chunk_items[ptr]->size;
	if(msize > size) //make memory move fitting the new chunk size
		msize = size;
		
	//printf("realloc:ptr:%d, @%d,%d to %d\n",ptr,mem_chunk_items[ptr]->ptr,mem_chunk_items[ptr]->size,tmp);
	memcpy(tmp,mem_chunk_items[ptr]->ptr,msize);
	free(mem_chunk_items[ptr]->ptr);
	mem_chunk_items[ptr]->ptr = tmp;
	mem_chunk_items[ptr]->size = size;	
	return(ptr);
}

void mem_free(MEM_ID ptr)
{	
	if(!mem_chunk_items[ptr]->is_freed)
	{
		free(mem_chunk_items[ptr]->ptr);
		mem_chunk_items[ptr]->is_freed = 1;
		mem_chunk_items[ptr]->ptr = NULL;
	}
}

MEM_NUM mem_get_size(MEM_ID ptr)
{
	return(mem_chunk_items[ptr]->size);
} 

void *mem_get_ptr(MEM_ID ptr)
{
	return(mem_chunk_items[ptr]->ptr);
} 

void mem_set_ptr(MEM_ID ptr,void *dst)
{
	mem_chunk_items[ptr]->ptr = dst;
	mem_chunk_items[ptr]->is_freed = 0;
}

void mem_set_size(MEM_ID ptr,MEM_NUM size)
{
	mem_chunk_items[ptr]->size = size;
}

void *mem_lock(MEM_ID ptr)
{		
	mem_locks_num++;
	mem_chunk_items[ptr]->locks++;
	mem_PushLock(ptr);
	//mem_DebugHeapWalkSize();
	//printf("locking :%d @ %d\n",ptr,mem_chunk_items[ptr]->ptr);
	return(mem_chunk_items[ptr]->ptr);
}

void mem_unlock(MEM_ID ptr,BOOL is_dirty)
{
	mem_unlocks_num++;
	mem_chunk_items[ptr]->locks--;	
	mem_PopLock(ptr);
	//printf("unlocking :%d @ %d\n",ptr,mem_chunk_items[ptr]->ptr);
}

MEM_ID mem_copy(MEM_ID src)
{
	void *tmp = (void*)malloc(mem_chunk_items[src]->size);
	memcpy(tmp,mem_chunk_items[src]->ptr,mem_chunk_items[src]->size);
	return(mem_AddChunk(tmp,"memory copy",mem_chunk_items[src]->size,mem_chunk_items[src]->pool_class));
}

MEM_ID mem_create(char *bytes,MEM_NUM len)
{
	void *tmp = (void*)malloc(len);
	memcpy(tmp,(void*)bytes,len);
	return(mem_AddChunk(tmp,"memory create",len,MEM_POOL_CLASS_DYNAMIC));
}

MEM_ID mem_add(char *bytes,MEM_NUM len)
{
	return(mem_AddChunk(bytes,"memory add",len,MEM_POOL_CLASS_STATIC));
}

MEM_ID mem_create_buf(MEM_NUM len)
{
	void *ptr = malloc(len);
	memset((void*)ptr,0,len);
	return(mem_AddChunk(ptr,"memory create buffer",len,MEM_POOL_CLASS_BUFFER));

}

MEM_ID mem_create_string(char *string)
{
	void *tmp = (void*)malloc(strlen(string) + 1);
	memcpy(tmp,(void*)string,strlen(string) + 1);
	return(mem_AddChunk(tmp,"memory create string",strlen(string) + 1,MEM_POOL_CLASS_VAR));
}

BOOL mem_compare(MEM_ID a,MEM_ID b)
{
	long len = mem_chunk_items[a]->size;
	long len2 = mem_chunk_items[b]->size;
	if(len != len2)
		return(0);
	return(!memcmp((void*)mem_chunk_items[a]->ptr,(void*)mem_chunk_items[b]->ptr,len));
}

void mem_set(MEM_ID src,char value,MEM_NUM len)
{
	memset(mem_chunk_items[src]->ptr,value,len);
}

BOOL mem_copy_to(MEM_ID dst,char *bytes,MEM_NUM len)
{
	memcpy(mem_chunk_items[dst]->ptr,(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from(MEM_ID src,char *bytes,MEM_NUM len)
{
	memcpy(bytes,mem_chunk_items[src]->ptr,len);
	return(1);
}

BOOL mem_copy_to_offset(MEM_ID dst,MEM_NUM dst_offset,char *bytes,MEM_NUM len)
{
	memcpy((void*)(((char*)mem_chunk_items[dst]->ptr)+dst_offset),(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from_offset(MEM_ID src,MEM_NUM src_offset,char *bytes,MEM_NUM len)
{
	memcpy((void*)bytes,(void*)(((char*)mem_chunk_items[src]->ptr)+src_offset),len);
	return(1);
}

#endif

#ifdef USE_MEMORY_MANAGER_CACHE

MEM_ID *mem_locks;
long mem_locks_top = 0;
char **mem_chunks;
long mem_chunks_top = 0;
long mem_chunks_free_chain_start = -1;//each char* is casted to an long containing the next free chunk id
cache_store *cache = NULL;


BOOL mem_SetCache(cache_store *store)
{
	cache = store;
	return(mem_store_Open(cache));
}

void mem_Init(void)
{
	mem_locks_top = 0;
	//mem_locks = (MEM_ID*) malloc(1 * sizeof(MEM_ID));
	mem_chunks_top = 0;
	//mem_chunks = (char**)malloc(1 * sizeof(char*));
	mem_chunks = NULL;
	mem_locks = NULL;
	mem_store_InitCacheTypes();
}

MEM_ID mem_AddChunk(void *ptr)
{
	if(mem_chunks_top>0)
		mem_chunks = (char**) realloc(mem_chunks,(mem_chunks_top+1)*sizeof(char*));
	else
		mem_chunks = (char**)malloc(1 * sizeof(char*));
	mem_chunks[mem_chunks_top] = ptr;
	mem_chunks_top++;
	return(mem_chunks_top-1);
}

void mem_PushLock(MEM_ID ptr)
{
	if(mem_locks_top>0)
		mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	else
		mem_locks = (MEM_ID*) malloc(1 * sizeof(MEM_ID));
	mem_locks[mem_locks_top] = ptr;
	mem_locks_top++;
	if(ptr == 0)
	{
				//printf("WARNING locking NULL\n");
				//mem_locks = (void*)1;
				//if(mem_locks[0] == 'c')
				//	printf("crash\n");	
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
			mem_locks = (void*)1;
			if(mem_locks[0] == 'c')
				printf("crash\n");
		}	
		//printf("unlocked %d\n",ptr);
	}
	if(mem_locks_top == 0)
	{
		free(mem_locks);
		mem_locks = NULL;
	}
	else
		mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top)*sizeof(MEM_ID));
	//check if ptr in locks
}

BOOL mem_HasLocks(MEM_ID ptr)
{
	if(!mem_locks_top)
		return(0);
	BOOL r = 0;
	for(long i = 0;i<mem_locks_top;i++)
	{
		if(ptr == mem_locks[i])
			return(1);
			//r = 1;
	}
	return(0);
}

void mem_Dump(MEM_ID mem)
{
			printf("chunk id:%d\nsize:%d\nhex:\n",mem,mem_get_size(mem));
			for(int b = 0;b<mem_get_size(mem);b++)
			{
				printf("%2x ",((unsigned char*)mem)[b]);
			}
			printf("\nraw:\n");
			for(int b = 0;b<mem_get_size(mem);b++)
			{
				printf("%c",((unsigned char*)mem)[b]);
			}
			printf("\n");

}

BOOL mem_Close(void)
{
	if(cache != NULL)
	{
		printf("closing memory store\n");
		mem_store_Close(cache);
		printf("freeing memory store\n");
		mem_store_Free(cache);
	}
	printf("freeing cache types\n");
	mem_store_FreeCacheTypes();
	if(mem_locks != NULL)
		free(mem_locks);
	if(mem_chunks != NULL)
		free(mem_chunks);
	return(1);
}

MEM_ID mem_malloc(MEM_NUM size,MEM_POOL_CLASS_ID pool_class)
{
	char *tmp = (char*)malloc(size+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = size;
	return(mem_AddChunk(tmp+sizeof(MEM_NUM)));
}

char *mem_malloc_ptr(MEM_NUM size,MEM_ID ptr)
{
	char *tmp = (char*)malloc(size+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = size;
	mem_set_ptr(ptr,tmp+sizeof(MEM_NUM));
	return(tmp+sizeof(MEM_NUM));
}

MEM_ID mem_realloc(MEM_ID ptr, MEM_NUM size)
{	
	if(cache != NULL && mem_chunks[ptr] == 0)
	{
		mem_store_Read(cache,ptr);
	}	
	char *tmp = (char*)malloc(size+sizeof(MEM_NUM));
	size_t msize = mem_get_size(ptr);
	if(msize > size) //make memory move fitting the new chunk size
		msize = size;
		
	((MEM_NUM*)tmp)[0] = size;
	memcpy(tmp+sizeof(MEM_NUM),mem_chunks[ptr],msize);
	free(mem_chunks[ptr]-sizeof(MEM_NUM));
	mem_chunks[ptr] = tmp+sizeof(MEM_NUM);
	//printf("realloced :%d\n",ptr);
	return(ptr);
}

void mem_free(MEM_ID ptr)
{	
	if(ptr < mem_chunks_top && mem_chunks[ptr] != 0)
	{
		//printf("ptr:%d,locks:%d\n",ptr,mem_HasLocks(ptr));
		//if(mem_HasLocks(ptr))
		//{
		//	char *tmp = 0;
		//	tmp[0] = 'c';
		//}
		free(mem_chunks[ptr]-sizeof(MEM_NUM));
		//free(mem_chunks[ptr]);
		mem_chunks[ptr] = 0;
		//printf("freed:%d\n",ptr);
	}
	//else 
	//	printf("invalid free call:%d\n",ptr);
}

MEM_NUM mem_get_size(MEM_ID ptr)
{
	//mem_Dump(ptr);
	MEM_NUM size = ((MEM_NUM*) (mem_chunks[ptr]-sizeof(MEM_NUM)) )[0];
	//printf("memory size:%d\n",size);
	return(size);
} 

void *mem_get_ptr(MEM_ID ptr)
{
	return(mem_chunks[ptr]);
} 

void mem_set_ptr(MEM_ID ptr,void *dst)
{
	mem_chunks[ptr] = dst;
}

void mem_set_size(MEM_ID ptr,MEM_NUM size)
{
	((MEM_NUM*)(mem_chunks[ptr]-sizeof(MEM_NUM)))[0] = size;
}

void *mem_lock(MEM_ID ptr)
{	
	if(cache != NULL && mem_chunks[ptr] == 0)
	{
		//printf("reading from store:%d\n",ptr);
		mem_store_Read(cache,ptr);
	}	
	//printf("locking :%d @ %d\n",ptr,mem_chunks[ptr]);
	mem_PushLock(ptr);
	return(mem_chunks[ptr]);
}

void mem_unlock(MEM_ID ptr,BOOL is_dirty)
{
	//printf("(before pop)unlocking:%d from %d(locks=%d)\n",ptr,mem_chunks[ptr],mem_HasLocks(ptr));
	mem_PopLock(ptr);
	/*if(is_dirty && ptr < 7 && locks_t > 6)
	{
		printf("trying to modify global object:%d\n",ptr);
		char *crash = 0;
		crash[0] = 1;


	}else if(ptr< 7)
	{
		printf("trying to modify global object:%d\n",ptr);
		locks_t++;
	}*/	
	//printf("unlocking:%d from %d(locks=%d)\n",ptr,mem_chunks[ptr],mem_HasLocks(ptr));
	if(is_dirty && cache != NULL && !mem_HasLocks(ptr))
	{	
		//if(mem_chunks[ptr] == 0)
		//	printf("unlocking zero ptr:%d\n",ptr);
		mem_store_Write(cache,ptr);
		//mem_free(ptr);
	}
}

MEM_ID mem_copy(MEM_ID src)
{
	if(cache != NULL && mem_chunks[src] == 0)
	{
		mem_store_Read(cache,src);
	}	
	char *tmp = (char*)malloc(mem_get_size(src)+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = mem_get_size(src);
	memcpy((tmp+sizeof(MEM_NUM)),mem_chunks[src],mem_get_size(src));
	return(mem_AddChunk(tmp + sizeof(MEM_NUM)));
}

MEM_ID mem_create(char *bytes,MEM_NUM len)
{
	char *tmp = (char*)malloc(len+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = len;
	memcpy(tmp+sizeof(MEM_NUM),(void*)bytes,len);
	return(mem_AddChunk(tmp + sizeof(MEM_NUM)));
}
/*
MEM_ID mem_add(char *bytes,MEM_NUM len)
{
	return(mem_AddChunk(bytes));
}
*/

MEM_ID mem_create_buf(MEM_NUM len)
{
	char *tmp = (char*)malloc(len+sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = len;
	memset(tmp+sizeof(MEM_NUM),0,len);
	return(mem_AddChunk(tmp + sizeof(MEM_NUM)));
}

MEM_ID mem_create_string(char *string)
{
	char *tmp = (char*)malloc(strlen(string) + 1 + sizeof(MEM_NUM));
	((MEM_NUM*)tmp)[0] = strlen(string) + 1;
	memcpy(tmp + sizeof(MEM_NUM),(void*)string,strlen(string) + 1);
	return(mem_AddChunk(tmp + sizeof(MEM_NUM)));
}

BOOL mem_compare(MEM_ID a,MEM_ID b)
{
	if(cache != NULL && mem_chunks[a] == 0)
	{
		mem_store_Read(cache,a);
	}	
	if(cache != NULL && mem_chunks[b] == 0)
	{
		mem_store_Read(cache,b);
	}	
	long len = mem_get_size(a);
	long len2 = mem_get_size(b);
	if(len != len2)
		return(0);
	return(!memcmp((void*)mem_chunks[a],(void*)mem_chunks[b],len));
}

void mem_set(MEM_ID src,char value,MEM_NUM len)
{
	if(cache != NULL && mem_chunks[src] == 0)
	{
		mem_store_Read(cache,src);
	}	
	memset(mem_chunks[src],value,len);
}

BOOL mem_copy_to(MEM_ID dst,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunks[dst] == 0)
	{
		mem_store_Read(cache,dst);
	}	
	memcpy((void*)mem_chunks[dst],(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from(MEM_ID src,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunks[src] == 0)
	{
		mem_store_Read(cache,src);
	}	
	memcpy((void*)bytes,(void*)mem_chunks[src],len);
	return(1);
}

BOOL mem_copy_to_offset(MEM_ID dst,MEM_NUM dst_offset,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunks[dst] == 0)
	{
		mem_store_Read(cache,dst);
	}	
	memcpy((void*)(mem_chunks[dst]+dst_offset),(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from_offset(MEM_ID src,MEM_NUM src_offset,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunks[src] == 0)
	{
		mem_store_Read(cache,src);
	}	
	memcpy((void*)bytes,(void*)(mem_chunks[src]+src_offset),len);
	return(1);
}

#endif

#ifdef USE_MEMORY_MANAGER_POOL
long mem_chunks_num = 0;
long mem_locks_num = 0;
long mem_unlocks_num = 0;
long mem_max_locked = 0;
long mem_max_chunk_size = 0;
long mem_max_locked_size = 0;
mem_chunk **mem_chunk_items;

MEM_ID *mem_locks;
long mem_locks_top = 0;

cache_store *cache = NULL;


BOOL mem_SetCache(cache_store *store)
{
	cache = store;
	return(mem_store_Open(cache));
}

void mem_Init(void)
{
/*	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	}
	#endif*/
	mem_chunk_items =	(mem_chunk**) malloc(1 * sizeof(mem_chunk*));
	mem_chunks_num = 0;
	mem_locks_num = 0;
	mem_unlocks_num = 0;
	mem_locks_top = 0;
	mem_max_locked = 0;
	mem_max_chunk_size = 0;
	mem_max_locked_size = 0;
	mem_locks = (MEM_ID*) malloc(1 * sizeof(MEM_ID));
	mem_store_InitCacheTypes();
}

void mem_set_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class)
{
	mem_chunk_items[mem]->pool_class |= pool_class;

}

void mem_unset_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class)
{
	mem_chunk_items[mem]->pool_class &= ~pool_class; //TODO ?
}

MEM_ID mem_AddChunk(void *ptr,char *description,MEM_NUM size,unsigned char pool_class)
{
	return(ptr);
	
	mem_chunk *chunk = (mem_chunk*)malloc(sizeof(mem_chunk));
	chunk->ptr = ptr;
	chunk->description = description;
	chunk->size = size;
	chunk->is_freed = 0;
	chunk->pool_class = pool_class;
	//chunk->is_removed = 0;
	chunk->locks = 0;
	printf("allocated chunk @ %x\r\n",chunk);
	mem_chunk_items = (mem_chunk**) realloc(mem_chunk_items,(mem_chunks_num+1)*sizeof(mem_chunk*));
	printf("increased chunks list for chunk: %d\r\n",mem_chunks_num);
	mem_chunk_items[mem_chunks_num] = chunk;
	mem_chunks_num++;
	return(mem_chunks_num-1);
}

void mem_PushLock(MEM_ID ptr)
{
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	mem_locks[mem_locks_top] = ptr;
	mem_locks_top++;
	if(ptr == 0)
	{
				//printf("WARNING locking NULL\n");
				//mem_locks = (void*)1;
				//if(mem_locks[0] == 'c')
				//	printf("crash\n");	
	}
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
			mem_locks = (void*)1;
			if(mem_locks[0] == 'c')
				printf("crash\n");
		}	
		//printf("unlocked %d\n",ptr);
	}
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	//check if ptr in locks
}

BOOL mem_HasLocks(MEM_ID ptr)
{
	BOOL r = 0;
	for(long i = 0;i<mem_locks_top;i++)
	{
		if(ptr == mem_locks[i])
			r = 1;
	}
	return(r);
}

void mem_Dump(MEM_ID mem)
{
			printf("chunk id:%d\ndescription:%s\nsize:%d\nhex:\n",mem,mem_chunk_items[mem]->description,mem_chunk_items[mem]->size);
			for(int b = 0;b<mem_chunk_items[mem]->size;b++)
			{
				printf("%2x ",((unsigned char*)mem_chunk_items[mem]->ptr)[b]);
			}
			printf("\nraw:\n");
			for(int b = 0;b<mem_chunk_items[mem]->size;b++)
			{
				printf("%c",((unsigned char*)mem_chunk_items[mem]->ptr)[b]);
			}
			printf("\n");

}

BOOL mem_DebugHeapWalk(BOOL show_leaked,BOOL show_locked)
{
	long locked = 0;
	long leaked = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(show_leaked && !mem_chunk_items[i]->is_freed )
		{
			printf("leaked ");
			mem_Dump(i);
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

INT mem_DebugHeapWalkSize(void)
{
	long locked = 0;
	INT locked_size = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(mem_chunk_items[i]->locks > 0)
		{
			//printf("lock size:%d\n",mem_chunk_items[i]->size);
			locked_size = locked_size + mem_chunk_items[i]->size;
			locked++;			
		}
	}
	//printf("locked:%d,size:%d\n",locked,locked_size);
	return(locked_size);
}

BOOL mem_Close(void)
{
	if(cache != NULL)
	{
		printf("closing memory store\n");
		mem_store_Close(cache);
		printf("freeing memory store\n");
		mem_store_Free(cache);
	}
	printf("freeing cache types\n");
	mem_store_FreeCacheTypes();
	printf("mem chunks allocated:%d\n",mem_chunks_num);
	printf("mem max chunk size:%d\n",mem_max_chunk_size);
	printf("mem chunks locked:%d\n",mem_locks_num);
	printf("mem chunks unlocked:%d\n",mem_unlocks_num);
	printf("locks peaked @ %d\n",mem_max_locked);
	printf("heap peaked @ %d\n",mem_max_locked_size);
	BOOL r = mem_DebugHeapWalk(1,1);
	//BOOL r = 0;
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

MEM_ID mem_malloc(MEM_NUM size,MEM_POOL_CLASS_ID pool_class)
{
	void *ptr = malloc(size);
	//printf("allocated @ %x\r\n",ptr);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	return(mem_AddChunk(ptr,NULL,size,pool_class));
}

MEM_ID mem_malloc_debug(MEM_NUM size,MEM_POOL_CLASS_ID pool_class, char *description)
{
	void *ptr = malloc(size);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	return(mem_AddChunk(ptr,description,size,pool_class));
}

MEM_ID mem_realloc(MEM_ID ptr, MEM_NUM size)
{
	return(realloc(ptr,size));
	
	if(cache != NULL && mem_chunk_items[ptr]->is_freed && mem_chunk_items[ptr]->locks == 0 && (mem_chunk_items[ptr]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,ptr);
	}	
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	void *tmp = (void*)malloc(size);
	size_t msize = mem_chunk_items[ptr]->size;
	if(msize > size) //make memory move fitting the new chunk size
		msize = size;
		
	//printf("realloc:ptr:%d, @%d,%d to %d\n",ptr,mem_chunk_items[ptr]->ptr,mem_chunk_items[ptr]->size,tmp);
	memcpy(tmp,mem_chunk_items[ptr]->ptr,msize);
	free(mem_chunk_items[ptr]->ptr);
	mem_chunk_items[ptr]->ptr = tmp;
	mem_chunk_items[ptr]->size = size;	
	return(ptr);
}
//int k1 = 0;
void mem_free(MEM_ID ptr)
{	
	free(ptr);
	return;
	if(!mem_chunk_items[ptr]->is_freed)
	{
		free(mem_chunk_items[ptr]->ptr);
		mem_chunk_items[ptr]->is_freed = 1;
		mem_chunk_items[ptr]->ptr = NULL;
	}
	//if(ptr == 109)
	//	k1++;
	//if(k1 == 8)
	//{
	//	printf("109 freed:%d,%d,%s\n",mem_chunk_items[ptr]->size,mem_chunk_items[ptr]->locks,mem_chunk_items[ptr]->description);
	//	char *m = 0;
	//	m[0] = 'x';
	//}
}

MEM_NUM mem_get_size(MEM_ID ptr)
{
	return(mem_chunk_items[ptr]->size);
} 

void *mem_get_ptr(MEM_ID ptr)
{
	return(mem_chunk_items[ptr]->ptr);
} 

void mem_set_ptr(MEM_ID ptr,void *dst)
{
	mem_chunk_items[ptr]->ptr = dst;
	mem_chunk_items[ptr]->is_freed = 0;
}

void mem_set_size(MEM_ID ptr,MEM_NUM size)
{
	mem_chunk_items[ptr]->size = size;
}

void *mem_lock(MEM_ID ptr)
{	
	return(ptr);
	
	mem_locks_num++;
	if(cache != NULL && mem_chunk_items[ptr]->is_freed && mem_chunk_items[ptr]->locks == 0 && (mem_chunk_items[ptr]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,ptr);
	}	

	mem_chunk_items[ptr]->locks++;
	mem_PushLock(ptr);
	//mem_DebugHeapWalkSize();
	return(mem_chunk_items[ptr]->ptr);
}

void mem_unlock(MEM_ID ptr,BOOL is_dirty)
{
	return;
	mem_unlocks_num++;
	mem_chunk_items[ptr]->locks--;	
	mem_PopLock(ptr);
	if(is_dirty && cache != NULL && mem_chunk_items[ptr]->locks == 0 && (mem_chunk_items[ptr]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{	
		mem_store_Write(cache,ptr);
		mem_free(ptr);
	}
}

MEM_ID mem_copy(MEM_ID src)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	void *tmp = (void*)malloc(mem_chunk_items[src]->size);
	memcpy(tmp,mem_chunk_items[src]->ptr,mem_chunk_items[src]->size);
	return(mem_AddChunk(tmp,"memory copy",mem_chunk_items[src]->size,mem_chunk_items[src]->pool_class));
}

MEM_ID mem_create(char *bytes,MEM_NUM len)
{
	void *tmp = (void*)malloc(len);
	memcpy(tmp,(void*)bytes,len);
	return(mem_AddChunk(tmp,"memory create",len,MEM_POOL_CLASS_DYNAMIC));
}

MEM_ID mem_add(char *bytes,MEM_NUM len)
{
	return(mem_AddChunk(bytes,"memory add",len,MEM_POOL_CLASS_STATIC));
}

MEM_ID mem_create_buf(MEM_NUM len)
{
	void *ptr = malloc(len);
	memset((void*)ptr,0,len);
	return(mem_AddChunk(ptr,"memory create buffer",len,MEM_POOL_CLASS_BUFFER));

}

MEM_ID mem_create_string(char *string)
{
	void *tmp = (void*)malloc(strlen(string) + 1);
	memcpy(tmp,(void*)string,strlen(string) + 1);
	return(mem_AddChunk(tmp,"memory create string",strlen(string) + 1,MEM_POOL_CLASS_VAR));
}

BOOL mem_compare(MEM_ID a,MEM_ID b)
{
	if(cache != NULL && mem_chunk_items[a]->is_freed && mem_chunk_items[a]->locks == 0 && (mem_chunk_items[a]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,a);
	}	
	if(cache != NULL && mem_chunk_items[b]->is_freed && mem_chunk_items[b]->locks == 0 && (mem_chunk_items[b]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,b);
	}	
	long len = mem_chunk_items[a]->size;
	long len2 = mem_chunk_items[b]->size;
	if(len != len2)
		return(0);
	return(!memcmp((void*)mem_chunk_items[a]->ptr,(void*)mem_chunk_items[b]->ptr,len));
}

void mem_set(MEM_ID src,char value,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	memset(mem_chunk_items[src]->ptr,value,len);
}

BOOL mem_copy_to(MEM_ID dst,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[dst]->is_freed && mem_chunk_items[dst]->locks == 0 && (mem_chunk_items[dst]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,dst);
	}	
	memcpy(mem_chunk_items[dst]->ptr,(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from(MEM_ID src,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	memcpy(bytes,mem_chunk_items[src]->ptr,len);
	return(1);
}

BOOL mem_copy_to_offset(MEM_ID dst,MEM_NUM dst_offset,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[dst]->is_freed && mem_chunk_items[dst]->locks == 0 && (mem_chunk_items[dst]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,dst);
	}	
	memcpy((void*)(((char*)mem_chunk_items[dst]->ptr)+dst_offset),(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from_offset(MEM_ID src,MEM_NUM src_offset,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	memcpy((void*)bytes,(void*)(((char*)mem_chunk_items[src]->ptr)+src_offset),len);
	return(1);
}

#endif

#ifdef USE_MEMORY_MANAGER_POOL_DEBUG
long mem_chunks_num = 0;
long mem_locks_num = 0;
long mem_unlocks_num = 0;
long mem_max_locked = 0;
long mem_max_chunk_size = 0;
long mem_max_locked_size = 0;
mem_chunk **mem_chunk_items;

MEM_ID *mem_locks;
long mem_locks_top = 0;

cache_store *cache = NULL;


BOOL mem_SetCache(cache_store *store)
{
	cache = store;
	return(mem_store_Open(cache));
}

void mem_Init(void)
{
/*	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	}
	#endif*/
	mem_chunk_items =	(mem_chunk**) malloc(1 * sizeof(mem_chunk*));
	mem_chunks_num = 0;
	mem_locks_num = 0;
	mem_unlocks_num = 0;
	mem_locks_top = 0;
	mem_max_locked = 0;
	mem_max_chunk_size = 0;
	mem_max_locked_size = 0;
	mem_locks = (MEM_ID*) malloc(1 * sizeof(MEM_ID));
	mem_store_InitCacheTypes();
}

void mem_set_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class)
{
	mem_chunk_items[mem]->pool_class |= pool_class;

}

void mem_unset_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class)
{
	mem_chunk_items[mem]->pool_class &= ~pool_class; //TODO ?
}

MEM_ID mem_AddChunk(void *ptr,char *description,MEM_NUM size,unsigned char pool_class)
{
	return(ptr);
	
	mem_chunk *chunk = (mem_chunk*)malloc(sizeof(mem_chunk));
	chunk->ptr = ptr;
	chunk->description = description;
	chunk->size = size;
	chunk->is_freed = 0;
	chunk->pool_class = pool_class;
	//chunk->is_removed = 0;
	chunk->locks = 0;
	printf("allocated chunk @ %x\r\n",chunk);
	mem_chunk_items = (mem_chunk**) realloc(mem_chunk_items,(mem_chunks_num+1)*sizeof(mem_chunk*));
	printf("increased chunks list for chunk: %d\r\n",mem_chunks_num);
	mem_chunk_items[mem_chunks_num] = chunk;
	mem_chunks_num++;
	return(mem_chunks_num-1);
}

void mem_PushLock(MEM_ID ptr)
{
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	mem_locks[mem_locks_top] = ptr;
	mem_locks_top++;
	if(ptr == 0)
	{
				//printf("WARNING locking NULL\n");
				//mem_locks = (void*)1;
				//if(mem_locks[0] == 'c')
				//	printf("crash\n");	
	}
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
			mem_locks = (void*)1;
			if(mem_locks[0] == 'c')
				printf("crash\n");
		}	
		//printf("unlocked %d\n",ptr);
	}
	mem_locks = (MEM_ID*) realloc(mem_locks,(mem_locks_top+1)*sizeof(MEM_ID));
	//check if ptr in locks
}

BOOL mem_HasLocks(MEM_ID ptr)
{
	BOOL r = 0;
	for(long i = 0;i<mem_locks_top;i++)
	{
		if(ptr == mem_locks[i])
			r = 1;
	}
	return(r);
}

void mem_Dump(MEM_ID mem)
{
			printf("chunk id:%d\ndescription:%s\nsize:%d\nhex:\n",mem,mem_chunk_items[mem]->description,mem_chunk_items[mem]->size);
			for(int b = 0;b<mem_chunk_items[mem]->size;b++)
			{
				printf("%2x ",((unsigned char*)mem_chunk_items[mem]->ptr)[b]);
			}
			printf("\nraw:\n");
			for(int b = 0;b<mem_chunk_items[mem]->size;b++)
			{
				printf("%c",((unsigned char*)mem_chunk_items[mem]->ptr)[b]);
			}
			printf("\n");

}

BOOL mem_DebugHeapWalk(BOOL show_leaked,BOOL show_locked)
{
	long locked = 0;
	long leaked = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(show_leaked && !mem_chunk_items[i]->is_freed )
		{
			printf("leaked ");
			mem_Dump(i);
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

INT mem_DebugHeapWalkSize(void)
{
	long locked = 0;
	INT locked_size = 0;
	for(long i = 0;i<mem_chunks_num;i++)
	{
		if(mem_chunk_items[i]->locks > 0)
		{
			//printf("lock size:%d\n",mem_chunk_items[i]->size);
			locked_size = locked_size + mem_chunk_items[i]->size;
			locked++;			
		}
	}
	//printf("locked:%d,size:%d\n",locked,locked_size);
	return(locked_size);
}

BOOL mem_Close(void)
{
	if(cache != NULL)
	{
		printf("closing memory store\n");
		mem_store_Close(cache);
		printf("freeing memory store\n");
		mem_store_Free(cache);
	}
	printf("freeing cache types\n");
	mem_store_FreeCacheTypes();
	printf("mem chunks allocated:%d\n",mem_chunks_num);
	printf("mem max chunk size:%d\n",mem_max_chunk_size);
	printf("mem chunks locked:%d\n",mem_locks_num);
	printf("mem chunks unlocked:%d\n",mem_unlocks_num);
	printf("locks peaked @ %d\n",mem_max_locked);
	printf("heap peaked @ %d\n",mem_max_locked_size);
	BOOL r = mem_DebugHeapWalk(1,1);
	//BOOL r = 0;
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

MEM_ID mem_malloc(MEM_NUM size,MEM_POOL_CLASS_ID pool_class)
{
	void *ptr = malloc(size);
	//printf("allocated @ %x\r\n",ptr);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	return(mem_AddChunk(ptr,NULL,size,pool_class));
}

MEM_ID mem_malloc_debug(MEM_NUM size,MEM_POOL_CLASS_ID pool_class, char *description)
{
	void *ptr = malloc(size);
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	return(mem_AddChunk(ptr,description,size,pool_class));
}

MEM_ID mem_realloc(MEM_ID ptr, MEM_NUM size)
{
	return(realloc(ptr,size));
	
	if(cache != NULL && mem_chunk_items[ptr]->is_freed && mem_chunk_items[ptr]->locks == 0 && (mem_chunk_items[ptr]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,ptr);
	}	
	if(size > mem_max_chunk_size)
	{
		mem_max_chunk_size = size;
		//printf("chunk size peaked @: %d\n",mem_max_chunk_size);
	}		
	void *tmp = (void*)malloc(size);
	size_t msize = mem_chunk_items[ptr]->size;
	if(msize > size) //make memory move fitting the new chunk size
		msize = size;
		
	//printf("realloc:ptr:%d, @%d,%d to %d\n",ptr,mem_chunk_items[ptr]->ptr,mem_chunk_items[ptr]->size,tmp);
	memcpy(tmp,mem_chunk_items[ptr]->ptr,msize);
	free(mem_chunk_items[ptr]->ptr);
	mem_chunk_items[ptr]->ptr = tmp;
	mem_chunk_items[ptr]->size = size;	
	return(ptr);
}
//int k1 = 0;
void mem_free(MEM_ID ptr)
{	
	free(ptr);
	return;
	if(!mem_chunk_items[ptr]->is_freed)
	{
		free(mem_chunk_items[ptr]->ptr);
		mem_chunk_items[ptr]->is_freed = 1;
		mem_chunk_items[ptr]->ptr = NULL;
	}
	//if(ptr == 109)
	//	k1++;
	//if(k1 == 8)
	//{
	//	printf("109 freed:%d,%d,%s\n",mem_chunk_items[ptr]->size,mem_chunk_items[ptr]->locks,mem_chunk_items[ptr]->description);
	//	char *m = 0;
	//	m[0] = 'x';
	//}
}

MEM_NUM mem_get_size(MEM_ID ptr)
{
	return(mem_chunk_items[ptr]->size);
} 

void *mem_get_ptr(MEM_ID ptr)
{
	return(mem_chunk_items[ptr]->ptr);
} 

void mem_set_ptr(MEM_ID ptr,void *dst)
{
	mem_chunk_items[ptr]->ptr = dst;
	mem_chunk_items[ptr]->is_freed = 0;
}

void mem_set_size(MEM_ID ptr,MEM_NUM size)
{
	mem_chunk_items[ptr]->size = size;
}

void *mem_lock(MEM_ID ptr)
{	
	return(ptr);
	
	mem_locks_num++;
	if(cache != NULL && mem_chunk_items[ptr]->is_freed && mem_chunk_items[ptr]->locks == 0 && (mem_chunk_items[ptr]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,ptr);
	}	

	mem_chunk_items[ptr]->locks++;
	mem_PushLock(ptr);
	//mem_DebugHeapWalkSize();
	return(mem_chunk_items[ptr]->ptr);
}

void mem_unlock(MEM_ID ptr,BOOL is_dirty)
{
	return;
	mem_unlocks_num++;
	mem_chunk_items[ptr]->locks--;	
	mem_PopLock(ptr);
	if(is_dirty && cache != NULL && mem_chunk_items[ptr]->locks == 0 && (mem_chunk_items[ptr]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{	
		mem_store_Write(cache,ptr);
		mem_free(ptr);
	}
}

MEM_ID mem_copy(MEM_ID src)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	void *tmp = (void*)malloc(mem_chunk_items[src]->size);
	memcpy(tmp,mem_chunk_items[src]->ptr,mem_chunk_items[src]->size);
	return(mem_AddChunk(tmp,"memory copy",mem_chunk_items[src]->size,mem_chunk_items[src]->pool_class));
}

MEM_ID mem_create(char *bytes,MEM_NUM len)
{
	void *tmp = (void*)malloc(len);
	memcpy(tmp,(void*)bytes,len);
	return(mem_AddChunk(tmp,"memory create",len,MEM_POOL_CLASS_DYNAMIC));
}

MEM_ID mem_add(char *bytes,MEM_NUM len)
{
	return(mem_AddChunk(bytes,"memory add",len,MEM_POOL_CLASS_STATIC));
}

MEM_ID mem_create_buf(MEM_NUM len)
{
	void *ptr = malloc(len);
	memset((void*)ptr,0,len);
	return(mem_AddChunk(ptr,"memory create buffer",len,MEM_POOL_CLASS_BUFFER));

}

MEM_ID mem_create_string(char *string)
{
	void *tmp = (void*)malloc(strlen(string) + 1);
	memcpy(tmp,(void*)string,strlen(string) + 1);
	return(mem_AddChunk(tmp,"memory create string",strlen(string) + 1,MEM_POOL_CLASS_VAR));
}

BOOL mem_compare(MEM_ID a,MEM_ID b)
{
	if(cache != NULL && mem_chunk_items[a]->is_freed && mem_chunk_items[a]->locks == 0 && (mem_chunk_items[a]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,a);
	}	
	if(cache != NULL && mem_chunk_items[b]->is_freed && mem_chunk_items[b]->locks == 0 && (mem_chunk_items[b]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,b);
	}	
	long len = mem_chunk_items[a]->size;
	long len2 = mem_chunk_items[b]->size;
	if(len != len2)
		return(0);
	return(!memcmp((void*)mem_chunk_items[a]->ptr,(void*)mem_chunk_items[b]->ptr,len));
}

void mem_set(MEM_ID src,char value,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	memset(mem_chunk_items[src]->ptr,value,len);
}

BOOL mem_copy_to(MEM_ID dst,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[dst]->is_freed && mem_chunk_items[dst]->locks == 0 && (mem_chunk_items[dst]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,dst);
	}	
	memcpy(mem_chunk_items[dst]->ptr,(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from(MEM_ID src,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	memcpy(bytes,mem_chunk_items[src]->ptr,len);
	return(1);
}

BOOL mem_copy_to_offset(MEM_ID dst,MEM_NUM dst_offset,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[dst]->is_freed && mem_chunk_items[dst]->locks == 0 && (mem_chunk_items[dst]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,dst);
	}	
	memcpy((void*)(((char*)mem_chunk_items[dst]->ptr)+dst_offset),(void*)bytes,len);
	return(1);
}

BOOL mem_copy_from_offset(MEM_ID src,MEM_NUM src_offset,char *bytes,MEM_NUM len)
{
	if(cache != NULL && mem_chunk_items[src]->is_freed && mem_chunk_items[src]->locks == 0 && (mem_chunk_items[src]->pool_class & MEM_POOL_CLASS_PERMANENT) == 0 )
	{
		mem_store_Read(cache,src);
	}	
	memcpy((void*)bytes,(void*)(((char*)mem_chunk_items[src]->ptr)+src_offset),len);
	return(1);
}

#endif



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
