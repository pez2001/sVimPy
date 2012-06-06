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

#include "stdio.h"
#include "stdlib.h"

#include "assert.h"
#include "debug.h"


/*
memory layout : 

chunks are accessed with ids 
the first 32768 are pool ids
the rest is reserved for string ids


max alloc size 256 except ptr lists which are a special case and are handled like strings

pools for fixed size objects
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



*/




#ifdef USE_DEBUGGING

#ifdef __cplusplus
extern "C"  {
#endif



typedef struct
{
	void *ptr;
	char *description;
	long size;
	int is_freed;
} mem_chunk;

#define MAX_MEM_CHUNKS 200000000

void mem_Init(void);

void mem_Close(void);

void *mem_malloc(size_t size, char *description);

void *mem_realloc(void *ptr, size_t size);

int mem_free(void *ptr);

#ifdef __cplusplus
} 
#endif



#endif

#endif
