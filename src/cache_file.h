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


//basic file cache store


#ifndef CACHE_FILE_H
#define CACHE_FILE_H

#include "features.h"
#include "types.h"
#include "memory_store.h"

#include <stdlib.h>
#include <stdio.h>
//#include <unistd.h>


#define CF_BLOB_ENTRIES 100000
#define CF_BLOBS 20

//#define CF_


/*
cache uses static 'blobs'
a blob has a constant number of entries in sequential order
size may vary depending on data


*/

#define STORE_TYPE_FILE 'f'



//#define CF_BLOCK_SIZE 128

/*typedef struct _cf_file_header
{
	INT entries_num;//CF_BLOB_ENTRIES
	INT entries_used;
	INT next_entry_offset;
}cf_file_header;
*/
/*
typedef struct _cf_block_header
{
	INT prev;
	INT next;
	//short used;
}cf_block_header;
*/
typedef struct _cf_entry
{
	//MEM_ID id;
	unsigned long start_offset;
	STREAM_NUM size;
}cf_entry;

typedef struct _cf_blob
{
	long entries_num;
	unsigned long next_entry_offset;
	FILE *file;
}cf_blob;


typedef struct _file_store_tag
{
	cf_blob **blobs;
	long blobs_num;
	char *filename;
	//STREAM_NUM entries;
}file_store_tag;




#ifdef __cplusplus
extern "C"  {
#endif

struct _store_type *cf_CreateStoreType(void);
BOOL cf_SetFileCache(char *filename);
struct _cache_store *cf_Create(char *filename);

void cf_SetEntry(struct _cache_store *store,MEM_ID mem,unsigned long offset);
cf_entry *cf_GetEntry(struct _cache_store *store,MEM_ID mem);


BOOL cf_open(struct _cache_store *store);
BOOL cf_close(struct _cache_store *store);
BOOL cf_free(struct _cache_store *store);
BOOL cf_delete(struct _cache_store *store,MEM_ID mem);
BOOL cf_clear(struct _cache_store *store);
BOOL cf_read(struct _cache_store *store,MEM_ID mem);
BOOL cf_write(struct _cache_store *store,MEM_ID mem);
BOOL cf_read_segment(struct _cache_store *store,MEM_ID mem,NUM offset,NUM len);
BOOL cf_write_segment(struct _cache_store *store,MEM_ID mem,NUM offset,NUM len);
BOOL cf_contains(struct _cache_store *store,MEM_ID mem);
NUM cf_getsize(struct _cache_store *store,MEM_ID mem);
NUM cf_getfree(struct _cache_store *store);

#ifdef __cplusplus
} 
#endif


#endif
