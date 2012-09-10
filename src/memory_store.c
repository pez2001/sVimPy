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

#include "memory_store.h"

store_type **store_types;
int store_types_num;

cache_store *AllocStore(void)
{
	return(malloc(sizeof(cache_store)));
}

store_type *AllocStoreType(void)
{
	return(malloc(sizeof(store_type)));
}

void mem_store_InitCacheTypes(void)
{

	store_types = (store_type**)malloc(sizeof(store_type*)*1);
	#ifndef USE_ARDUINO_FUNCTIONS
	store_type *ftype = cf_CreateStoreType();
	store_types[0] = ftype;
	#endif
	store_types_num = 0;
}

void mem_store_FreeCacheTypes(void)
{
	INDEX i = 0;	
	do
	{
		free(store_types[i]);
		i++;
	}while(i < store_types_num);
	free(store_types);
}

store_type *mem_store_GetType(TYPE id)
{
	INDEX i = 0;	
	do
	{
		store_type *t = (store_type*)store_types[i];
		
		if(t->type == id)
		{
			return(t);
		}
		i++;
	}while(i < store_types_num);
	/*#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"store type not defined:%c\n",id);
	#endif*/
	return(NULL);
}

//store access functions

BOOL mem_store_Open(cache_store *store)
{
	store_type *type = (store_type*)store->type;
	if(type->store_open == NULL)
	{
		return(1);
	}
	BOOL b = type->store_open(store);
	return(b);
}

BOOL mem_store_Close(cache_store *store)
{
	store_type *type = (store_type*)store->type;
	if(type->store_close == NULL)
	{
		return(1);
	}
	BOOL b = type->store_close(store);
	return(b);
}

BOOL mem_store_Free(struct _cache_store *store)
{
	store_type *type = (store_type*)store->type;
	if(type->store_close == NULL)
	{
		return(1);
	}
	BOOL b = type->store_free(store);
	return(b);
}

BOOL mem_store_Delete(cache_store *store,MEM_ID mem)
{
	store_type *type = (store_type*)store->type;
	if(type->store_delete == NULL)
	{
		return(1);
	}
	BOOL b = type->store_delete(store,mem);
	return(b);
}

BOOL mem_store_Clear(cache_store *store)
{
	store_type *type = (store_type*)store->type;
	if(type->store_clear == NULL)
	{
		return(1);
	}
	BOOL b = type->store_clear(store);
	return(b);
}

BOOL mem_store_Read(cache_store *store,MEM_ID mem)
{
	store_type *type = (store_type*)store->type;
	if(type->store_read == NULL)
	{
		return(1);
	}
	BOOL b = type->store_read(store,mem);
	return(b);
}

BOOL mem_store_Write(cache_store *store,MEM_ID mem)
{
	store_type *type = (store_type*)store->type;
	if(type->store_write == NULL)
	{
		return(1);
	}
	BOOL b = type->store_write(store,mem);
	return(b);
}

BOOL mem_store_ReadSegment(cache_store *store,MEM_ID mem,NUM offset,NUM len)
{
	store_type *type = (store_type*)store->type;
	if(type->store_read == NULL)
	{
		return(1);
	}
	BOOL b = type->store_read_segment(store,mem,offset,len);
	return(b);
}

BOOL mem_store_WriteSegment(cache_store *store,MEM_ID mem,NUM offset,NUM len)
{
	store_type *type = (store_type*)store->type;
	if(type->store_write == NULL)
	{
		return(1);
	}
	BOOL b = type->store_write_segment(store,mem,offset,len);
	return(b);
}

BOOL mem_store_Contains(cache_store *store,MEM_ID mem)
{
	store_type *type = (store_type*)store->type;
	if(type->store_contains == NULL)
	{
		return(1);
	}
	BOOL b = type->store_contains(store,mem);
	return(b);
}

NUM mem_store_GetSize(cache_store *store,MEM_ID mem)
{
	store_type *type = (store_type*)store->type;
	if(type->store_getsize == NULL)
	{
		return(1);
	}
	NUM l = type->store_getsize(store,mem);
	return(l);
}

NUM mem_store_GetFree(cache_store *store)
{
	store_type *type = (store_type*)store->type;
	if(type->store_getfree == NULL)
	{
		return(1);
	}
	NUM si = type->store_getfree(store);
	return(si);
}



