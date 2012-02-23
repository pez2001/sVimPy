#include "memory.h"

#ifdef DEBUGGING

long mem_chunks_num = 0;
long mem_chunks_max_size = 0;
long mem_chunks_actual_size = 0;
mem_chunk **mem_chunk_items;
long mem_chunks_top = 0;

void mem_Init(void)
{
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	mem_chunk_items =	(mem_chunk**) malloc(MAX_MEM_CHUNKS * sizeof(mem_chunk*));
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
				mem_chunk_items[i]->ptr = realloc(ptr, size);
				mem_chunks_actual_size += size - mem_chunk_items[i]->size;
				if (mem_chunks_actual_size > mem_chunks_max_size)
					mem_chunks_max_size = mem_chunks_actual_size;
				mem_chunk_items[i]->size = size;
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
	void *tmp = malloc(size);

	if((debug_level & DEBUG_MEMORY) > 0)
	{
	if (size == 0)
	{
		debug_printf(DEBUG_MEMORY,"allocated zero bytes\n");
		// return(NULL);
	}
	// else
	//debug_printf(DEBUG_ALLOCS,"allocated %d bytes @%x\n",size,tmp);
	if((debug_level & DEBUG_ALLOCS)>0)
		printf("allocated %d bytes @%x\n",size,tmp);
	mem_Push(tmp, size, description);
	}
	return (tmp);
}

int mem_free(void *ptr)
{
	if((debug_level & DEBUG_MEMORY) > 0)
	{
	int f = 0;

	for (int i = 0; i < mem_chunks_top; i++)
	{
		if (mem_chunk_items[i]->ptr == ptr && !mem_chunk_items[i]->is_freed)
		{
			f = 1;
			mem_chunk_items[i]->is_freed = 1;
			break;
		}
	}
	// assert(f);
	if (!f)
	{
		debug_printf(DEBUG_MEMORY,"chunk not found @%x\n", ptr);
		return (0);
	}
	mem_chunks_num--;
	if (mem_chunks_num < 0)
		debug_printf(DEBUG_MEMORY,"more memory freed than allocated\n");
	}
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

#endif


