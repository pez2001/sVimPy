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

#include "stdio.h"
#include "stdlib.h"

#include "debug.h"



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

alloc function
free function
access function
egress function (for handling memory chunk caching


object chunk header :
unsigned char size
unsigned short id
char[] chunk_data

string chunk header : 
unsigned short size
unsigned short id
char[] string


remember sorting of ids can speed up things



//lock objects only if non global member vars are accessed
//check if object is a cached_object
//if it is -> load object from storage into memory
//return loaded object
//increases lock count of object

//this way caching would leave freed pointers arround hmmm
//implement a small proxy object to get arround freed pointers problem

//unlock objects after usage of lock and access to member vars is no longer needed
//decreases lock count of object
//if lock count reached zero and object is a non global type check if object should be cached




typedef struct _cache_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_REF_COUNT lock_count;
	#endif
	//object *object;
	STREAM_NUM stream_pos;
} cache_object;


//proxy object struct
//used for virtual caching objects in streams
//normal size = 9 + 8 
//avr size = 4 + 4
typedef struct _proxy_object
{
	OBJECT_TYPE type;
	OBJECT_REF_COUNT ref_count;
	#ifdef USE_LOCKING
	OBJECT_LOCK_COUNT lock_count;
	#endif
	void *ref;//object* or stream_pos/512
	void *is_cached; // == 0 -> not cached use ref as object*  > 0 -> pointer stream where object is cached ,ref is the position(mmc sector address 512 bytes aligned) in the stream 
} proxy_object;




*/

#define MEM_SIZE 1024

#define MEM_POOL_CLASS_STATIC 1
#define MEM_POOL_CLASS_DYNAMIC 2
#define MEM_POOL_CLASS_PERMANENT 4
//var string space
#define MEM_POOL_CLASS_VAR 128






#ifdef __cplusplus
extern "C"  {
#endif



typedef struct
{
	void *ptr;
	char *description;
	long size;
	int is_freed;
	long locks;
	//int is_removed;
} mem_chunk;

#define MAX_MEM_CHUNKS 200000000

void mem_Init(void);

BOOL mem_Close(void);

BOOL mem_DebugHeapWalk(BOOL show_leaked,BOOL show_locked);

#ifdef USE_MEMORY_DEBUGGING

	MEM_ID mem_malloc_debug(size_t size,MEM_POOL_CLASS_ID pool_class, char *description);

#else

	MEM_ID mem_malloc(size_t size,MEM_POOL_CLASS_ID pool_class);

#endif

MEM_ID mem_realloc(MEM_ID ptr, size_t size);

MEM_ID mem_copy(MEM_ID src);

MEM_ID mem_create(char *bytes,INT len);

MEM_ID mem_create_buf(INT len);

MEM_ID mem_create_string(char *string);

BOOL mem_compare(MEM_ID a,MEM_ID b);

void mem_set(MEM_ID src,char value,INT len);

BOOL mem_copy_to(MEM_ID dst,char *bytes,INT len);

BOOL mem_copy_from(MEM_ID src,char *bytes,INT len);

BOOL mem_copy_to_offset(MEM_ID dst,NUM dst_offset,char *bytes,INT len);

BOOL mem_copy_from_offset(MEM_ID src,NUM src_offset,char *bytes,INT len);

void mem_free(MEM_ID ptr);

//#ifdef USE_MEMORY_LOCK_DEBUGGING

//	void *mem_lock_debug(MEM_ID ptr,char *key);
	
//	void mem_unlock_debug(MEM_ID ptr,BOOL is_dirty,char *key);

//#else

	void *mem_lock(MEM_ID ptr);

	void mem_unlock(MEM_ID ptr,BOOL is_dirty);

	void *mem_lock_segment(MEM_ID ptr,NUM offset,NUM len); //only lock a portion of the total memory chunk , to enable huge tuples, etc
//#endif

#ifdef __cplusplus
} 
#endif
#endif
