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


#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

#include "features.h"

#define TYPE char

#define MEM_NUM unsigned long

#define OBJECT_TYPE char
#define OBJECT_FLAGS unsigned char
//#define OBJECT_REF_COUNT unsigned short

//stream 
#ifdef USE_ARDUINO_FUNCTIONS
	#define STREAM_NUM unsigned short
	#define OBJECT_REF_COUNT short
	#define OBJECT_LOCK_COUNT unsigned char
	// used in INDEXING situations
	#define INDEX short 
	//used in enum over array and total numbers
	//#define NUM unsigned short
	#define NUM short
	//used in relative calcs
	#define REL_NUM short
#else
	#define STREAM_NUM long 
	#define OBJECT_REF_COUNT long
	#define OBJECT_LOCK_COUNT long
	// used in INDEXING situations
	#define INDEX long 
	//used in enum over array and total numbers
	//#define NUM unsigned short
	#define NUM long
	//used in relative calcs
	#define REL_NUM long
#endif


//standard int size

#ifdef USE_ARDUINO_FUNCTIONS
	#define INT long
#else
	#define INT long long
#endif
//standard floating number size
#define FLOAT float


//used for boolean comparisons
#define BOOL unsigned char

#ifdef USE_ARDUINO_FUNCTIONS
	#define MEM_ID unsigned short
#else
	#ifdef USE_MEMORY_MANAGER_PASS
		#define MEM_ID void*
	#else
		#define MEM_ID unsigned long
	#endif
#endif

#define MEM_POOL_CLASS_ID char

#define STREAM_TYPE_ID MEM_ID

#define BYTES_ID MEM_ID
#define VM_ID MEM_ID
#define STREAM_ID MEM_ID
#define STACK_ID MEM_ID
#define LIST_ID MEM_ID
#define CACHE_STORE_ID MEM_ID
#define CACHE_STORE_TYPE_ID MEM_ID

//python type ids
#define OBJECT_ID MEM_ID
#define TUPLE_ID OBJECT_ID
#define CODE_ID OBJECT_ID
#define BLOCK_ID OBJECT_ID
#define CLASS_ID OBJECT_ID
#define METHOD_ID OBJECT_ID
#define FUNCTION_ID OBJECT_ID
#define CFUNCTION_ID OBJECT_ID
#define TAG_ID OBJECT_ID
#define CLASS_INSTANCE_ID OBJECT_ID
#define ITER_ID OBJECT_ID
#define FLOAT_ID OBJECT_ID
#define INT_ID OBJECT_ID
#define UNICODE_ID OBJECT_ID
#define KV_ID OBJECT_ID
#define STRING_ID OBJECT_ID

#ifdef USE_DEBUGGING

void DumpTypes(void);

#endif

#endif
