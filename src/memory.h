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
#ifndef MEMORY_H
#define MEMORY_H

#include "features.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#include "debug.h"
#include "memory_store.h"

#ifndef USE_ARDUINO_FUNCTIONS
#ifndef USE_MEMORY_MANAGER_PASS
#ifndef USE_MEMORY_MANAGER_DEBUG
#include "cache_file.h"
#endif
#endif
#endif



/*
memory layout : 

chunks are accessed with ids 
the first 32768 are pool ids
the rest is reserved for string ids


max alloc size 256 except ptr lists which are a special case and are handled like strings

pools for fixed size objects, const object will be handled seperately
string pool which will automatically fill freed space by moving strings afterwards
list pool for ptr lists etc


cache for memory chunks addressable by id stored in streams



*/

//#define MEM_SIZE 1024

#define MEM_POOL_CLASS_STATIC 1
#define MEM_POOL_CLASS_DYNAMIC 2
#define MEM_POOL_CLASS_PERMANENT 4
#define MEM_POOL_CLASS_BUFFER 8
//var string space
#define MEM_POOL_CLASS_VAR 128



#ifdef USE_MEMORY_MANAGER_POOL_DEBUG

typedef struct _mem_chunk_header
{
	char locks;//locks num , if set to -1 the chunk is free
	MEM_ID id
	MEM_NUM size;
	//unsigned char pool_class;
	
} mem_chunk_header;

typedef struct _mem_free_chunk_header
{
	char magic;//if set to -1 its a free chunk
	MEM_ID next_chunk
	MEM_NUM size;
} mem_free_chunk_header;

#endif





typedef struct
{
	void *ptr;
	char *description;
	unsigned char pool_class;
	long size;
	int is_freed;
	long locks;
	//int is_removed;
} mem_chunk;

#ifdef __cplusplus
extern "C"  {
#endif


void mem_Init(void);

BOOL mem_SetCache(struct _cache_store *store);

BOOL mem_Close(void);

void mem_Dump(MEM_ID mem);

BOOL mem_DebugHeapWalk(BOOL show_leaked,BOOL show_locked);

#ifdef USE_MEMORY_DEBUGGING

	MEM_ID mem_malloc_debug(MEM_NUM size,MEM_POOL_CLASS_ID pool_class, char *description);

#else

	MEM_ID mem_malloc(MEM_NUM size,MEM_POOL_CLASS_ID pool_class);

#endif

char *mem_malloc_ptr(MEM_NUM size,MEM_ID ptr);

void mem_set_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class);

void mem_unset_pool_class(MEM_ID mem, MEM_POOL_CLASS_ID pool_class);

MEM_ID mem_realloc(MEM_ID ptr, MEM_NUM size);

MEM_ID mem_copy(MEM_ID src);

MEM_ID mem_create(char *bytes,MEM_NUM len);

MEM_ID mem_add(char *bytes,MEM_NUM len);

MEM_ID mem_create_buf(MEM_NUM len);

MEM_ID mem_create_string(char *string);

BOOL mem_compare(MEM_ID a,MEM_ID b);

void mem_set(MEM_ID src,char value,MEM_NUM len);

BOOL mem_copy_to(MEM_ID dst,char *bytes,MEM_NUM len);

BOOL mem_copy_from(MEM_ID src,char *bytes,MEM_NUM len);

BOOL mem_copy_to_offset(MEM_ID dst,MEM_NUM dst_offset,char *bytes,MEM_NUM len);

BOOL mem_copy_from_offset(MEM_ID src,MEM_NUM src_offset,char *bytes,MEM_NUM len);

MEM_NUM mem_get_size(MEM_ID ptr);

void *mem_get_ptr(MEM_ID ptr);

void mem_set_ptr(MEM_ID ptr,void *dst);

void mem_set_size(MEM_ID ptr,MEM_NUM size);

void mem_free(MEM_ID ptr);

//#ifdef USE_MEMORY_LOCK_DEBUGGING

//	void *mem_lock_debug(MEM_ID ptr,char *key);
	
//	void mem_unlock_debug(MEM_ID ptr,BOOL is_dirty,char *key);

//#else

void *mem_lock(MEM_ID ptr);

void mem_unlock(MEM_ID ptr,BOOL is_dirty);

void *mem_lock_segment(MEM_ID ptr,MEM_NUM offset,MEM_NUM len); //only lock a portion of the total memory chunk , to enable huge tuples, etc
//#endif

#ifdef __cplusplus
} 
#endif
#endif
