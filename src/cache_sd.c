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

#include "cache_sd.h"

#ifndef USE_MEMORY_MANAGER_PASS
//sd cache store

file_store_tag *AllocSDStoreTag(void)
{
	return(malloc(sizeof(sd_store_tag)));
}

void cf_OpenBlob(cache_store *store,long i)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;
	FILE *f;
	char *ext = malloc(19);
	ext[0] = '.';
	itoa(i,ext+1,10);
	char *tmp = (char*)malloc(strlen(s_tag->filename) + strlen(ext) + 1);
	
	memset(tmp, 0, strlen(s_tag->filename) + strlen(ext) + 1);
	memcpy(tmp, s_tag->filename, strlen(s_tag->filename));
	memcpy(tmp + strlen(s_tag->filename), ext, strlen(ext));
	//#ifdef USE_DEBUGGING
	//debug_printf(DEBUG_VERBOSE_TESTS,"opened cache file:%s\n",tmp);
	//#endif

	printf("opening: %s\n",tmp);
	f = fopen(tmp, "wb+");
	free(tmp);
	free(ext);
	if(f == NULL)
	{
		return;
	}
	s_tag->blobs[i] = malloc(sizeof(cf_blob));
	s_tag->blobs[i]->file = f;
	s_tag->blobs[i]->entries_num = 0;
	s_tag->blobs[i]->next_entry_offset = CF_BLOB_ENTRIES * sizeof(cf_entry);

}

void cf_CloseBlob(cache_store *store,long i)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;
	fclose(s_tag->blobs[i]->file);	
	s_tag->blobs[i]->file = 0;
}

void cf_FreeBlob(cache_store *store,long i)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;
	free(s_tag->blobs[i]);
}

long cf_GetBlob(MEM_ID mem)
{
	return(mem / CF_BLOB_ENTRIES);
}
long cf_GetBlobEntry(MEM_ID mem)
{
	return(mem % CF_BLOB_ENTRIES);
}


BOOL cf_SetFileCache(char *filename)
{
	cache_store *c = cf_Create(filename);
	return(mem_SetCache(c));
}

BOOL cf_open(cache_store *store)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;

	s_tag->blobs = malloc(sizeof(cf_blob*)*CF_BLOBS);
	for(int i = 0;i<CF_BLOBS;i++)
		cf_OpenBlob(store,i);
	s_tag->blobs_num = CF_BLOBS;
	//printf("cache file opened successfully:%s,%x\n",filename,f);
	return(1);
}

BOOL cf_close(cache_store *store)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;
	if(s_tag->blobs ==	0)
	{
		return(0);
	}
	for(int i = 0;i<s_tag->blobs_num;i++)
		cf_CloseBlob(store,i);
	s_tag->blobs_num = 0;
	return(1);
}

BOOL cf_free(cache_store *store)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;
	if(s_tag->blobs == 0)
	{
		return(0);
	}
	cf_close(store);
	for(int i = 0;i<CF_BLOBS;i++)
		cf_FreeBlob(store,i);

	free(s_tag->blobs);
	free(store);
	return(1);
}


BOOL cf_delete(cache_store *store,MEM_ID mem)
{
	return(1);
}

BOOL cf_clear(cache_store *store)
{
	return(1);
}

BOOL cf_read(cache_store *store,MEM_ID mem)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;
	long b = cf_GetBlob(mem);
	//long be = cf_GetBlobEntry(mem);
	//printf("restoring memory: \"%d\"\n",mem);
	cf_entry *e = cf_GetEntry(store,mem);
	char *tmp = mem_malloc_ptr(e->size,mem);
	//memset(tmp,255,e->size);
	fseek(s_tag->blobs[b]->file,e->start_offset,SEEK_SET);
	fread(tmp,e->size,1,s_tag->blobs[b]->file);
	//mem_set_ptr(mem,tmp);
	//mem_set_size(mem,e->size);
	//if(mem == 86)
	//	mem_Dump(mem);
	free(e);
	return(1);
}

BOOL cf_write(cache_store *store,MEM_ID mem)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;
	long b = cf_GetBlob(mem);
	//printf("caching memory: \"%d\",size:%d\n",mem,mem_get_size(mem));
	fseek(s_tag->blobs[b]->file,s_tag->blobs[b]->next_entry_offset,SEEK_SET);
	fwrite(mem_get_ptr(mem),mem_get_size(mem),1,s_tag->blobs[b]->file);
	cf_SetEntry(store,mem,s_tag->blobs[b]->next_entry_offset);
	s_tag->blobs[b]->next_entry_offset += mem_get_size(mem);
	//if(mem == 86)
	//	mem_Dump(mem);
	return(1);
}

BOOL cf_read_segment(cache_store *store,MEM_ID mem,NUM offset,NUM len)
{
	printf("restoring memory segment: %d (%d,%d)\n",mem,offset,len);

	return(1);
}

BOOL cf_write_segment(cache_store *store,MEM_ID mem,NUM offset,NUM len)
{
	printf("caching memory segment: %d (%d,%d)\n",mem,offset,len);
	return(1);
}

BOOL cf_contains(cache_store *store,MEM_ID mem)
{
	printf("contains memory: %d\n",mem);
	return(1);
}

NUM cf_getsize(cache_store *store,MEM_ID mem)
{
	return(1);
}

NUM cf_getfree(cache_store *store)
{
	return(1);
}

void cf_SetEntry(cache_store *store,MEM_ID mem,unsigned long offset)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;
	long b = cf_GetBlob(mem);
	long be = cf_GetBlobEntry(mem);
	cf_entry *e = malloc(sizeof(cf_entry));
	//e->id = mem;
	e->start_offset = offset;
	e->size = mem_get_size(mem);
	//printf("sizeof cf_entry:%d\n",sizeof(cf_entry));
	fseek(s_tag->blobs[b]->file,0 + sizeof(cf_entry)*be,SEEK_SET);
	fwrite(e,sizeof(cf_entry),1,s_tag->blobs[b]->file);
	free(e);
}

cf_entry *cf_GetEntry(cache_store *store,MEM_ID mem)
{
	file_store_tag *s_tag = (file_store_tag*)store->tag;
	long b = cf_GetBlob(mem);
	long be = cf_GetBlobEntry(mem);
	cf_entry *e = malloc(sizeof(cf_entry));
	fseek(s_tag->blobs[b]->file,0 + sizeof(cf_entry)*be,SEEK_SET);
	fread(e,sizeof(cf_entry),1,s_tag->blobs[b]->file);
	return(e);
}


cache_store *cf_Create(char *filename)
{
	cache_store *s = AllocStore();
	s->type = mem_store_GetType(STORE_TYPE_FILE);
	s->tag = AllocFileStoreTag();
	file_store_tag *fst = (file_store_tag*)s->tag;
	fst->filename = filename;
	fst->blobs_num = 0;
	fst->blobs = NULL;
	//printf("created cache:%d,type:%d\n",s_id,s->type);
	return(s);
}

store_type *cf_CreateStoreType(void)
{
	store_type *ftype = AllocStoreType();
	ftype->type = STORE_TYPE_FILE;
	ftype->store_open = &cf_open;
	ftype->store_close = &cf_close;
	ftype->store_free = &cf_free;
	ftype->store_delete = &cf_delete;
	ftype->store_read = &cf_read;
	ftype->store_write  = &cf_write;
	ftype->store_read_segment = &cf_read_segment;
	ftype->store_write_segment  = &cf_write_segment;
	ftype->store_clear = &cf_clear;
	ftype->store_getsize = &cf_getsize;
	ftype->store_getfree = &cf_getfree;
	ftype->store_contains = &cf_contains;
	return(ftype);
}

#endif
#endif






