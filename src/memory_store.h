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


#ifndef MEMORY_STORE_H
#define MEMORY_STORE_H

#include "features.h"
#include "types.h"

struct _store_type;
struct _cache_store;
#include "memory.h"

#ifndef USE_ARDUINO_FUNCTIONS
#include "cache_file.h"
#endif

typedef struct _store_type
{
	TYPE type;
	BOOL (*store_open)(struct _cache_store *store);
	BOOL (*store_close)(struct _cache_store *store);
	BOOL (*store_free)(struct _cache_store *store);
	BOOL (*store_delete)(struct _cache_store *store,MEM_ID mem);	
	BOOL (*store_clear)(struct _cache_store *store);	
	BOOL (*store_read)(struct _cache_store *store,MEM_ID mem);
	BOOL (*store_write)(struct _cache_store *store,MEM_ID mem);
	BOOL (*store_read_segment)(struct _cache_store *store,MEM_ID mem ,NUM offset,NUM len);
	BOOL (*store_write_segment)(struct _cache_store *store,MEM_ID mem,NUM offset,NUM len);
	BOOL (*store_contains)(struct _cache_store *store,MEM_ID mem);
	NUM (*store_getsize)(struct _cache_store *store,MEM_ID mem);
	NUM (*store_getfree)(struct _cache_store *store); 
} store_type;

typedef struct _cache_store
{
	store_type *type;
	void *tag;
}cache_store;




#ifdef __cplusplus
extern "C"  {
#endif


void mem_store_InitCacheTypes(void);
void mem_store_FreeCacheTypes(void);

cache_store *AllocStore(void);
store_type *AllocStoreType(void);
store_type *mem_store_GetType(TYPE id);

//store access functions

BOOL mem_store_Open(struct _cache_store *store);
BOOL mem_store_Close(struct _cache_store *store);
BOOL mem_store_Free(struct _cache_store *store);
BOOL mem_store_Delete(struct _cache_store *store,MEM_ID mem);
BOOL mem_store_Clear(struct _cache_store *store);
BOOL mem_store_Read(struct _cache_store *store,MEM_ID mem);
BOOL mem_store_Write(struct _cache_store *store,MEM_ID mem);
BOOL mem_store_ReadSegment(struct _cache_store *store,MEM_ID mem,NUM offset,NUM len);
BOOL mem_store_WriteSegment(struct _cache_store *store,MEM_ID mem,NUM offset,NUM len);
BOOL mem_store_Contains(struct _cache_store *store,MEM_ID mem);
NUM mem_store_GetSize(struct _cache_store *store,MEM_ID mem);
NUM mem_store_GetFree(struct _cache_store *store);



#ifdef __cplusplus
} 
#endif


#endif
