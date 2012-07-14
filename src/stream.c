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

#include "stream.h"

MEM_ID stream_types;/*id_list*/
MEM_ID stream_globals;

STREAM_ID AllocStream(void)
{
	#ifdef USE_MEMORY_DEBUGGING
	return(mem_malloc_debug(sizeof(stream),MEM_POOL_CLASS_STATIC, "AllocStream() return"));
	#else
	return(mem_malloc(sizeof(stream),MEM_POOL_CLASS_STATIC));
	#endif
}

MEM_ID AllocStreams(void)
{
	#ifdef USE_MEMORY_DEBUGGING
	return(mem_malloc_debug(sizeof(streams),MEM_POOL_CLASS_STATIC, "AllocStreams() return"));
	#else
	return(mem_malloc(sizeof(streams),MEM_POOL_CLASS_STATIC));
	#endif
}

MEM_ID AllocStreamType(void)
{
	#ifdef USE_MEMORY_DEBUGGING
	return(mem_malloc_debug(sizeof(stream_type),MEM_POOL_CLASS_STATIC, "AllocStreamType() return"));
	#else
	return(mem_malloc(sizeof(stream_type),MEM_POOL_CLASS_STATIC));
	#endif
}

MEM_ID AllocStreamBytes(STREAM_NUM len)
{
	#ifdef USE_MEMORY_DEBUGGING
	return(mem_malloc_debug(len,MEM_POOL_CLASS_DYNAMIC, "AllocStreamBytes() return"));
	#else
	return(mem_malloc(len),MEM_POOL_CLASS_DYNAMIC);
	#endif
}

MEM_ID AllocFileStreamTag(void)
{
	#ifdef USE_MEMORY_DEBUGGING
	return(mem_malloc_debug(sizeof(file_stream_tag),MEM_POOL_CLASS_STATIC, "AllocFileStreamTag() return"));
	#else
	return(mem_malloc(sizeof(file_stream_tag),MEM_POOL_CLASS_STATIC));
	#endif
}

MEM_ID AllocByteStreamTag(void)
{
	#ifdef USE_MEMORY_DEBUGGING
	return(mem_malloc_debug(sizeof(byte_stream_tag),MEM_POOL_CLASS_STATIC, "AllocByteStreamTag() return"));
	#else
	return(mem_malloc(sizeof(byte_stream_tag),MEM_POOL_CLASS_STATIC));
	#endif
}

void streams_Init(void)
{
	stream_types = list_Create(0,0);
	stream_globals = AllocStreams();
	
	//memory stream
	MEM_ID mtype_id = AllocStreamType();
	stream_type *mtype = (stream_type*)mem_lock(mtype_id);
	mtype->type = STREAM_TYPE_MEMORY;
	mtype->stream_open = NULL;
	mtype->stream_close = NULL;
	//mtype->stream_open = &stream_memory_open;
	//mtype->stream_close = &stream_memory_close;
	mtype->stream_free = &stream_memory_free;
	mtype->stream_read = &stream_memory_read;
	mtype->stream_write  = NULL;
	mtype->stream_seek = NULL;
	mtype->stream_getpos = NULL;
	//mtype->stream_write = &stream_memory_write;
	//mtype->stream_seek = &stream_memory_seek;
	//mtype->stream_getpos = &stream_memory_getpos;
	mem_unlock(mtype_id,1);
	list_Push(stream_types,mtype_id);
	//file stream
	#ifndef USE_ARDUINO_FUNCTIONS
	MEM_ID ftype_id = AllocStreamType();
	stream_type *ftype = (stream_type*)mem_lock(ftype_id);
	ftype->type = STREAM_TYPE_FILE;
	ftype->stream_open = &stream_file_open;
	ftype->stream_close = &stream_file_close;
	ftype->stream_free = &stream_file_free;
	ftype->stream_read = &stream_file_read;
	ftype->stream_write = &stream_file_write;
	ftype->stream_seek = &stream_file_seek;
	ftype->stream_getpos = &stream_file_getpos;
	mem_unlock(ftype_id,1);
	list_Push(stream_types,ftype_id);
	#endif
	
	#ifdef USE_ARDUINO_FUNCTIONS
	//arduino flash memory stream
	MEM_ID fmtype_id = AllocStreamType();
	stream_type *fmtype = (stream_type*)mem_lock(fmtype_id);
	fmtype->type = STREAM_TYPE_FLASH_MEMORY;
	fmtype->stream_open = NULL;
	fmtype->stream_close = NULL;
	//fmtype->stream_open = &stream_flash_memory_open;
	//fmtype->stream_close = &stream_flash_memory_close;
	fmtype->stream_free = &stream_flash_memory_free;
	fmtype->stream_read = &stream_flash_memory_read;
	fmtype->stream_write = NULL;
	fmtype->stream_seek = NULL;
	fmtype->stream_getpos  = NULL;
	//fmtype->stream_write = &stream_flash_memory_write;
	//fmtype->stream_seek = &stream_flash_memory_seek;
	//fmtype->stream_getpos = &stream_flash_memory_getpos;
	mem_unlock(fmtype_id,1);
	list_Push(stream_types,fmtype_id);
	//arduino serial stream

	#endif
	//standard io

}

MEM_ID stream_GetType(STREAM_TYPE_ID id)/*returns struct _stream_type **/
{
	INDEX i = 0;	
	do
	{
		MEM_ID ti = list_Get(stream_types,i);
		stream_type *t = (stream_type*)mem_lock(ti);
		
		if(t->type == id)
		{
			mem_unlock(ti,0);
			return(ti);
		}
		mem_unlock(ti,0);
		i++;
	}while(i < list_GetLen(stream_types));
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"stream type not defined:%c\n",id);
	#endif
	return(0);
}

void streams_Close(void)
{
	INDEX i = 0;	
	do
	{
		mem_free(list_Get(stream_types,i));
		i++;
	}while(i < list_GetLen(stream_types));
	list_Close(stream_types);
	mem_free(stream_globals);
}

#ifndef USE_ARDUINO_FUNCTIONS
STREAM_ID stream_CreateFromFile(BYTES_ID filename,BYTES_ID flags)//(char *filename,char *flags); //will create a copy of filename returns stream*
{
	MEM_ID str_id = AllocStream();
	stream *str = (stream*)mem_lock(str_id);
	//MEM_ID fname = mem_copy(filename);
	str->type = stream_GetType(STREAM_TYPE_FILE);
	str->tag = AllocFileStreamTag();
	file_stream_tag *fst = (file_stream_tag*)mem_lock(str->tag);
	//fst->filename = fname;
	fst->filename = filename;
	mem_unlock(str->tag,1);
	str->flags = flags;
	mem_unlock(str_id,1);
	return(str_id);
}
#endif

STREAM_ID stream_CreateFromBytes(BYTES_ID bytes,STREAM_NUM len)//(char *bytes ,STREAM_NUM len); //will use bytes directly - remember to free bytes after stream closing
{
	MEM_ID str_id = AllocStream();
	stream *str = (stream*)mem_lock(str_id);
	str->tag = AllocByteStreamTag();
	byte_stream_tag *bst = (byte_stream_tag*)mem_lock(str->tag);
	bst->bytes = bytes;
	//printf("bytes:%x\n",bytes);
	bst->len = len;
	bst->offset = 0;
	mem_unlock(str->tag,1);
	str->type = stream_GetType(STREAM_TYPE_MEMORY);
	str->flags = 0;
	mem_unlock(str_id,1);
	return(str_id);
}
/*
#ifdef USE_ARDUINO_FUNCTIONS
MEM_ID stream_CreateFromFlashBytes(MEM_ID bytes,STREAM_NUM len)//(char *bytes ,STREAM_NUM len);
{
	stream *str = AllocStream();
	ptr_list *mopt = ptr_CreateList(0,0);
	ptr_Push(mopt,bytes);
	ptr_Push(mopt,(void*)len);
	ptr_Push(mopt,0);
	//printf("getting stream type\n");
	str->type = streams_GetStreamType(STREAM_TYPE_FLASH_MEMORY);
	//printf("got stream type\n");
	//debug_printf(DEBUG_ALL,"open:%x\n",str->type->stream_open);
	str->tags = mopt;
	str->flags = NULL;
	return(str);
}
#endif
*/
STREAM_ID stream_CreateStandardOutput(void)
{
	STREAM_ID str_id = AllocStream();
	stream *str = (stream*)mem_lock(str_id);
	str->type = stream_GetType(STREAM_TYPE_STANDARD_OUTPUT);
	str->tag = 0;
	mem_unlock(str_id,1);
	return(str_id);
}

STREAM_ID stream_CreateStandardInput(void)
{
	STREAM_ID str_id = AllocStream();
	stream *str = (stream*)mem_lock(str_id);
	str->type = stream_GetType(STREAM_TYPE_STANDARD_INPUT);
	str->tag = 0;
	mem_unlock(str_id,1);
	return(str_id);
}

STREAM_ID stream_CreateDebugOutput(void)
{
	STREAM_ID str_id = AllocStream();
	stream *str = (stream*)mem_lock(str_id);
	str->type = stream_GetType(STREAM_TYPE_DEBUG_OUTPUT);
	str->tag = 0;
	mem_unlock(str_id,1);
	return(str_id);
}

//stream access functions

BOOL stream_Open(STREAM_ID stream)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	stream_type *str_type = (stream_type*)mem_lock(str->type);
	if(str_type->stream_open == NULL)
	{
		mem_unlock(str->type,0);
		mem_unlock(stream,0);
		return(1);
	}
	BOOL b = str_type->stream_open(stream);
	mem_unlock(str->type,0);
	mem_unlock(stream,0);
	return(b);
}

BOOL stream_Close(STREAM_ID stream)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	stream_type *str_type = (stream_type*)mem_lock(str->type);
	if(str_type->stream_close == NULL)
	{
		mem_unlock(str->type,0);
		mem_unlock(stream,0);
		return(1);
	}
	BOOL b = str_type->stream_close(stream);
	mem_unlock(str->type,0);
	mem_unlock(stream,0);
	return(b);
}

BOOL stream_Free(STREAM_ID stream)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	stream_type *str_type = (stream_type*)mem_lock(str->type);
	if(str_type->stream_free == NULL)
	{
		mem_unlock(str->type,0);
		mem_unlock(stream,0);
		return(1);
	}
	BOOL (*stream_free)(STREAM_ID stream) = str_type->stream_free;
	mem_unlock(str->type,0);
	mem_unlock(stream,0);
	BOOL b = stream_free(stream);
	//mem_DebugHeapWalk(0,1);
	return(b);
}

BOOL stream_Read(STREAM_ID stream,MEM_ID ptr,STREAM_NUM ptr_offset,STREAM_NUM len)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	stream_type *str_type = (stream_type*)mem_lock(str->type);
	if(str_type->stream_read == NULL)
	{
		mem_unlock(str->type,0);
		mem_unlock(stream,0);
		return(1);
	}
	BOOL b = str_type->stream_read(stream,ptr,ptr_offset,len);
	mem_unlock(str->type,0);
	mem_unlock(stream,0);
	return(b);
}

BOOL stream_Write(STREAM_ID stream,MEM_ID ptr ,STREAM_NUM ptr_offset,STREAM_NUM len)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	stream_type *str_type = (stream_type*)mem_lock(str->type);
	if(str_type->stream_write == NULL)
	{
		mem_unlock(str->type,0);
		mem_unlock(stream,0);
		return(1);
	}
	BOOL b = str_type->stream_write(stream,ptr,ptr_offset,len);
	mem_unlock(str->type,0);
	mem_unlock(stream,0);
	return(b);
}

BOOL stream_Seek(STREAM_ID stream,STREAM_NUM offset,STREAM_ORIGIN origin)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	stream_type *str_type = (stream_type*)mem_lock(str->type);
	if(str_type->stream_seek == NULL)
	{
		mem_unlock(str->type,0);
		mem_unlock(stream,0);
		return(1);
	}
	BOOL b = str_type->stream_seek(stream,offset,origin);
	mem_unlock(str->type,0);
	mem_unlock(stream,0);
	return(b);
}

INT      stream_GetPos(STREAM_ID stream)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	stream_type *str_type = (stream_type*)mem_lock(str->type);
	if(str_type->stream_getpos == NULL)
	{
		mem_unlock(str->type,0);
		mem_unlock(stream,0);
		return(-1);
	}
	INT b = str_type->stream_getpos(stream);
	mem_unlock(str->type,0);
	mem_unlock(stream,0);
	return(b);
}
//file stream functions

#ifndef USE_ARDUINO_FUNCTIONS
BOOL stream_file_open(STREAM_ID stream)
{
	FILE *f;
	struct _stream *str = (struct _stream*)mem_lock(stream);
	file_stream_tag *str_tag = (file_stream_tag*)mem_lock(str->tag);
	char *filename = (char*)mem_lock(str_tag->filename);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"opened file:%s\n",filename);
	#endif
	//printf("opening: %s\n",filename);
	if(str->flags != 0)
	{
		char *flags = (char*)mem_lock(str->flags);
		f = fopen(filename, flags);
		mem_unlock(str->flags,0);
	}
	else
		f = fopen(filename, "rb");
	mem_unlock(str_tag->filename,0);
	if(f == NULL)
	{
		mem_unlock(str->tag,0);
		mem_unlock(stream,0);
		return(0);
	}
	str_tag->file = f;
	mem_unlock(str->tag,1);
	mem_unlock(stream,0);
	//printf("file opened successfully:%x\n",f);
	return(1);
}

BOOL stream_file_close(STREAM_ID stream)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	file_stream_tag *str_tag = (file_stream_tag*)mem_lock(str->tag);
	if(str_tag->file == 0)
	{
		mem_unlock(str->tag,0);
		mem_unlock(stream,0);
		return(0);
	}
	FILE *f = (FILE*)str_tag->file;
	fclose(f);
	str_tag->file = 0;
	mem_unlock(str->tag,1);
	mem_unlock(stream,0);
	//mem_free
	return(1);
}

BOOL stream_file_free(STREAM_ID stream)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	file_stream_tag *str_tag = (file_stream_tag*)mem_lock(str->tag);
	if(str_tag->file != 0)
	{
		FILE *f = (FILE*)str_tag->file;
		fclose(f);
	}
	if(str_tag->filename != 0)
		mem_free(str_tag->filename);
	mem_unlock(str->tag,0);
	if(str->flags != 0)
		mem_free(str->flags);
	mem_free(str->tag);
	mem_unlock(stream,0);
	mem_free(stream);
	return(1);
}

BOOL stream_file_read(STREAM_ID stream,MEM_ID ptr,STREAM_NUM ptr_offset,STREAM_NUM len)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	file_stream_tag *str_tag = (file_stream_tag*)mem_lock(str->tag);
	//printf("tag f:%x\n",str_tag->file);
	if(str_tag->file == 0)
	{
		mem_unlock(str->tag,0);
		mem_unlock(stream,0);
		return(0);
	}
	FILE *f = (FILE*)str_tag->file;
	char *bytes = (char*)mem_lock(ptr);
	//printf("bytes:%x,f:%x,str_tag:%x,str:%x,len:%d,ptr_offset:%d\n",bytes,f,str_tag,str,len,ptr_offset);
	if(fread(bytes+ptr_offset, len, 1, f) != 1)
	{
		mem_unlock(ptr,0);
		mem_unlock(str->tag,0);
		mem_unlock(stream,0);
		return(0);
	}
	mem_unlock(ptr,1);
	mem_unlock(str->tag,0);
	mem_unlock(stream,0);
	return(1);
}

BOOL stream_file_write(STREAM_ID stream,MEM_ID ptr ,STREAM_NUM ptr_offset,STREAM_NUM len)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	file_stream_tag *str_tag = (file_stream_tag*)mem_lock(str->tag);
	if(str_tag->file != 0)
	{
		mem_unlock(str->tag,0);
		mem_unlock(stream,0);
		return(0);
	}
	FILE *f = (FILE*)str_tag->file;
	char *bytes = (char*)mem_lock(ptr);
	if(fwrite(bytes+ptr_offset, len, 1, f) != 1)
	{
		mem_unlock(ptr,0);
		mem_unlock(str->tag,0);
		mem_unlock(stream,0);
		return(0);
	}
	mem_unlock(ptr,0);
	mem_unlock(str->tag,0);
	mem_unlock(stream,0);
	return(1);
}

BOOL stream_file_seek(STREAM_ID stream,STREAM_NUM offset,STREAM_ORIGIN origin)
{ 
	struct _stream *str = (struct _stream*)mem_lock(stream);
	file_stream_tag *str_tag = (file_stream_tag*)mem_lock(str->tag);
	if(str_tag->file != 0)
	{
		mem_unlock(str->tag,0);
		mem_unlock(stream,0);
		return(0);
	}
	FILE *f = (FILE*)str_tag->file;
	int org = 0;
	if(origin == STREAM_ORIGIN_SET)
		org = SEEK_SET;
	else if(origin == STREAM_ORIGIN_CUR)
		org = SEEK_CUR;
	else if(origin == STREAM_ORIGIN_END)
		org = SEEK_END;
	BOOL r = fseek(f,offset,org);
	mem_unlock(str->tag,0);
	mem_unlock(stream,0);
	return(r);
}

INT stream_file_getpos(STREAM_ID stream)
{ 
	struct _stream *str = (struct _stream*)mem_lock(stream);
	file_stream_tag *str_tag = (file_stream_tag*)mem_lock(str->tag);
	if(str_tag->file != 0)
	{
		mem_unlock(str->tag,0);
		mem_unlock(stream,0);
		return(0);
	}
	FILE *f = (FILE*)str_tag->file;
	if(feof(f))
		return(-1);//triple usage of getpos as feof detector and error detector
	INT r = ftell(f);
	mem_unlock(str->tag,0);
	mem_unlock(stream,0);
	return(r);
}
#endif
/*
BOOL stream_memory_open(struct _stream *stream)
{
	return(1);
}

BOOL stream_memory_close(struct _stream *stream)
{
	return(1);
}
*/
BOOL stream_memory_free(STREAM_ID stream)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	if(str->flags != 0)
		mem_free(str->flags);
	mem_free(str->tag);
	mem_unlock(stream,0);
	mem_free(stream);
	return(1);
}

BOOL stream_memory_read(STREAM_ID stream,MEM_ID ptr,STREAM_NUM ptr_offset,STREAM_NUM len)
{
	struct _stream *str = (struct _stream*)mem_lock(stream);
	byte_stream_tag *str_tag = (byte_stream_tag*)mem_lock(str->tag);
	char *bytes = (char*)mem_lock(str_tag->bytes);
	//printf("read from bytes:%x,tag offset:%d,len:%d,tag len:%d\n",bytes,str_tag->offset,len,str_tag->len);
	if((str_tag->offset + len) > str_tag->len)
		return(0);
	//printf("reading\n");
	mem_copy_to_offset(ptr,ptr_offset,(bytes+str_tag->offset),len);
	str_tag->offset = (str_tag->offset+len);
	mem_unlock(str_tag->bytes,0);
	mem_unlock(str->tag,1);
	mem_unlock(stream,0);
	return(1);
}
/*
BOOL stream_memory_write(struct _stream *stream,void *ptr ,STREAM_NUM len)
{
	return(0);
}

BOOL stream_memory_seek(struct _stream *stream,STREAM_NUM offset,STREAM_ORIGIN origin)
{ 
	return(0);
}
INT stream_memory_getpos(struct _stream *stream)
{ 
	return(0);
}
*/
/*
#ifdef USE_ARDUINO_FUNCTIONS
/*
BOOL stream_flash_memory_open(struct _stream *stream)
{
	return(1);
}

BOOL stream_flash_memory_close(struct _stream *stream)
{
	return(1);
}
*/
/*
BOOL stream_flash_memory_free(struct _stream *stream)
{
	ptr_CloseList(stream->tags);
	#ifdef USE_DEBUGGING
	if(stream->flags != NULL)
		assert(mem_free(stream->flags));
	assert(mem_free(stream));
	#else
	if(stream->flags != NULL)
		free(stream->flags);
	free(stream);
	#endif		
	return(1);
}

BOOL stream_flash_memory_read(struct _stream *stream,void *ptr,STREAM_NUM len)
{
	char *bytes = (char*)ptr_Get(stream->tags,0);
	STREAM_NUM offset = (STREAM_NUM)ptr_Get(stream->tags,2);
	memcpy_P(ptr,(bytes+offset),len);
	ptr_Set(stream->tags,2,(void*)(offset+len));
	return(1);
}
/*
BOOL stream_flash_memory_write(struct _stream *stream,void *ptr ,STREAM_NUM len)
{
	return(0);
}

BOOL stream_flash_memory_seek(struct _stream *stream,STREAM_NUM offset,STREAM_ORIGIN origin)
{ 
	return(0);
}
INT stream_flash_memory_getpos(struct _stream *stream)
{ 
	return(0);
}
*/
/*
#endif
*/


long stream_ReadLong(STREAM_ID stream)
{
	long r = 0;
	BYTES_ID l = mem_create_buf(4);
	stream_Read(stream,l, 0,4);
	//printf("b:%d\n",b);
	mem_copy_from(l,(char*)&r,4);
	mem_free(l);
	return(r);
}

FLOAT stream_ReadFloat(STREAM_ID stream)
{
	float r = 0;
	double t = 0;
	BYTES_ID l = mem_create_buf(8);
	stream_Read(stream, l,0,8);
	mem_copy_from(l,(char*)&t,8);
	mem_free(l);
	r = (FLOAT)t;
	return(r);
}

char stream_ReadChar(STREAM_ID stream)
{
	char r = 0;
	BYTES_ID l = mem_create_buf(8);
	stream_Read(stream,l,0,1);
	mem_copy_from(l,(char*)&r,1);
	mem_free(l);
	return(r);
}

void stream_WriteLong(long l,STREAM_ID stream)
{
	BYTES_ID lb = mem_create_buf(4);
	mem_copy_to(lb,(char*)&l,4);
	stream_Write(stream,lb,0,4);
	mem_free(lb);
}

void stream_WriteFloat(FLOAT fl,STREAM_ID stream)
{
	BYTES_ID lb = mem_create_buf(4);
	double d = (double)fl;
	mem_copy_to(lb,(char*)&d,8);
	stream_Write(stream,lb,0,8);
	mem_free(lb);
}

void stream_WriteChar(char c,STREAM_ID stream)
{
	BYTES_ID lb = mem_create_buf(1);
	mem_copy_to(lb,(char*)&c,1);
	stream_Write(stream,lb,0,1);
	mem_free(lb);
}

OBJECT_ID stream_ReadObject(STREAM_ID stream)
{
	OBJECT_TYPE type = stream_ReadChar(stream);

	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_CREATION,"reading object with type:%c\n",type);
	#endif
	switch (type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_ELLIPSIS:
		{
			OBJECT_ID obj_id = obj_CreateEmpty(type);
			obj_IncRefCount(obj_id);
			return (obj_id);
		}
		case TYPE_CODE:
        {	
			OBJECT_ID co_id = obj_Alloc(TYPE_CODE,1);
			code_object *co = (code_object*)mem_lock(co_id);
			co->argcount = (NUM)stream_ReadLong(stream);
			co->kwonlyargcount = (NUM)stream_ReadLong(stream);
			co->nlocals = (NUM)stream_ReadLong(stream);
			co->stacksize = (NUM)stream_ReadLong(stream);
			co->co_flags = stream_ReadLong(stream);
			co->code = stream_ReadObject(stream);
			co->consts = stream_ReadObject(stream);
			co->names = stream_ReadObject(stream);
			co->varnames = stream_ReadObject(stream);
			co->freevars = stream_ReadObject(stream);
			co->cellvars = stream_ReadObject(stream);
			tuple_ConvertToDict(co->freevars); //TODO only needed as long deref and closure opcodes are not implemented using references
			tuple_ConvertToDict(co->cellvars);
			OBJECT_ID tmp_filename = stream_ReadObject(stream);
			obj_DecRefCount(tmp_filename);	// TO DECREASE MEMORY USAGE
			OBJECT_ID tmp_name_id = stream_ReadObject(stream);
			unicode_object *tmp_name = (unicode_object*)mem_lock(tmp_name_id);
			co->name = mem_copy(tmp_name->value);
			mem_unlock(tmp_name_id,0);
			obj_DecRefCount(tmp_name_id);
			stream_ReadLong(stream);//firstlineno
			OBJECT_ID tmp_lnotab = stream_ReadObject(stream);
			obj_DecRefCount(tmp_lnotab);	// TO DECREASE MEMORY USAGE
			obj_ClearGC();
			mem_unlock(co_id,1);
			//obj_Dump(co_id,1,1);
			return(co_id);
		}
		break;
		case TYPE_STRING:
		{
			OBJECT_ID obj_id = obj_Alloc(TYPE_STRING,1);
			string_object *obj = (string_object*)mem_lock(obj_id);
			NUM n;
			n = (NUM)stream_ReadLong(stream);
			if(n > 0)
			{
				#ifdef USE_MEMORY_DEBUGGING
				MEM_ID string = mem_malloc_debug(n,MEM_POOL_CLASS_DYNAMIC, "ReadObject() string");
				#else
				MEM_ID string = mem_malloc(n,MEM_POOL_CLASS_DYNAMIC);
				#endif
				stream_Read(stream,string,0, n);
				obj->len = n;
				obj->content = string;
			}
			else
			{
				obj->content = 0;
				//obj->type = TYPE_NULL;
				obj->len = 0;
			}
			mem_unlock(obj_id,1);
			return(obj_id);
		}
		break;
		case TYPE_TUPLE:
		{
			INT n = (INT)stream_ReadLong(stream);
			//printf("reading %d tuple items\n",n);
			OBJECT_ID to_id = obj_Alloc(TYPE_TUPLE,1);
			tuple_object *to = (tuple_object*)mem_lock(to_id);
			to->ptr = 0;
			if(n > 0)
			{
				to->list = list_Create(n,LIST_STATIC);
				//printf("created tuple list:%d\n",to->list);
				for (NUM i = 0; i < n; i++)
				{
					OBJECT_ID tuple = stream_ReadObject(stream);
					//obj_Dump(tuple,1,1);
					list_Set(to->list,i,tuple);
				}
				//obj_Dump(to_id,1,1);
			}
			else
				to->list = 0;
			mem_unlock(to_id,1);
			//obj_Dump(to_id,1,1);
			return(to_id);
		}
		break;
		case TYPE_UNICODE:
		{
			OBJECT_ID obj_id = obj_Alloc(TYPE_UNICODE,1);
			unicode_object *obj= (unicode_object*)mem_lock(obj_id);
			NUM n = (NUM)stream_ReadLong(stream);
			#ifdef USE_MEMORY_DEBUGGING
			MEM_ID unicode_string = mem_malloc_debug(n + 1,MEM_POOL_CLASS_DYNAMIC, "ReadObject() unicode_string");
			#else
			MEM_ID unicode_string = mem_malloc(n + 1,MEM_POOL_CLASS_DYNAMIC);
			#endif
			mem_set(unicode_string, 0, n + 1);
			stream_Read(stream,unicode_string, 0,n);
			obj->value = unicode_string;
			mem_unlock(obj_id,1);
			return(obj_id);
		}
		break;
		case TYPE_BINARY_FLOAT:
		{
			OBJECT_ID obj_id = obj_Alloc(TYPE_BINARY_FLOAT,1);
			float_object *obj= (float_object*)mem_lock(obj_id);
			FLOAT n = (FLOAT)stream_ReadFloat(stream);
			obj->value = n;
			mem_unlock(obj_id,1);
			return(obj_id);
		}
		break;
		case TYPE_INT:
		{
			OBJECT_ID obj_id = obj_Alloc(TYPE_INT,1);
			int_object *obj= (int_object*)mem_lock(obj_id);
			INT n = (INT)stream_ReadLong(stream);
			obj->value = n;
			mem_unlock(obj_id,1);
			return(obj_id);
		}
		break;
		default:
		{
			OBJECT_ID obj_id = obj_CreateEmpty(TYPE_UNKNOWN);
			obj_IncRefCount(obj_id);
			return(obj_id);
		}
	}
	return(0);
}

void stream_WriteObject(OBJECT_ID obj_id,STREAM_ID stream)
{
	object *obj = (object*)mem_lock(obj_id);
	stream_WriteChar(obj->type,stream);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_CREATION,"writing object with type:%c\n",obj->type);
	#endif
	switch(obj->type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_ELLIPSIS:
		{
			mem_unlock(obj_id,0);
			return;
		}
	case TYPE_CODE:
       {
			stream_WriteLong(((code_object*)obj)->argcount,stream);
			stream_WriteLong(((code_object*)obj)->kwonlyargcount,stream);
			stream_WriteLong(((code_object*)obj)->nlocals,stream);
			stream_WriteLong(((code_object*)obj)->stacksize,stream);
			stream_WriteLong(((code_object*)obj)->co_flags,stream);
			stream_WriteObject(((code_object*)obj)->code,stream);
			stream_WriteObject(((code_object*)obj)->consts,stream);
			stream_WriteObject(((code_object*)obj)->names,stream);
			stream_WriteObject(((code_object*)obj)->varnames,stream);
			stream_WriteObject(((code_object*)obj)->freevars,stream);
			stream_WriteObject(((code_object*)obj)->cellvars,stream);
			OBJECT_ID filename = obj_CreateUnicode(mem_create("",1));
			obj_IncRefCount(filename);
			stream_WriteObject(filename,stream);
			obj_DecRefCount(filename);
			OBJECT_ID name = obj_CreateUnicode(((code_object*)obj)->name);
			obj_IncRefCount(name);
			stream_WriteObject(name,stream);
			obj_DecRefCount(name);
			stream_WriteLong(0,stream);
			OBJECT_ID lnotab = obj_CreateEmpty(TYPE_NONE);
			obj_IncRefCount(lnotab);
			stream_WriteObject(lnotab,stream);
			obj_DecRefCount(lnotab);
			obj_ClearGC();
			mem_unlock(obj_id,0);
			return;
		}
	case TYPE_STRING:
		{
			NUM n = ((string_object*)obj)->len;
			stream_WriteLong(n,stream);
			if(n > 0)
			{
				stream_Write(stream,((string_object*)obj)->content,0, n);
			}
			mem_unlock(obj_id,0);
			return;
		}
	case TYPE_TUPLE:
		{
			INT n = tuple_GetLen(obj_id);
			stream_WriteLong(n,stream);
			if(n > 0)
			{
				for(NUM i = 0; i < n; i++)
				{
					stream_WriteObject(tuple_GetItem(obj_id,i),stream);
				}
			}
			else
				stream_WriteLong(0,stream);
			mem_unlock(obj_id,0);
			return;
		}
	case TYPE_UNICODE:
		{
			NUM n = obj_GetUnicodeLen(obj_id);
			stream_WriteLong(n,stream);
			if(n > 0)
			{
				stream_Write(stream,((unicode_object*)obj)->value, 0,n);
			}
			mem_unlock(obj_id,0);
			return;
		}
	case TYPE_BINARY_FLOAT:
		{
			stream_WriteFloat(((float_object*)obj)->value,stream);
			mem_unlock(obj_id,0);
			return;
		}
	case TYPE_INT:
		{
			stream_WriteLong(((int_object*)obj)->value,stream);
			mem_unlock(obj_id,0);
			return;
		}
	default:
		{
			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", obj->type);
			#endif
			mem_unlock(obj_id,0);
			return;
		}
	}
}

#ifndef USE_ARDUINO_FUNCTIONS
//same as above , but using a reduced code object layout
OBJECT_ID stream_ReadObjectPlus(STREAM_ID stream)
{
	OBJECT_TYPE type = stream_ReadChar(stream);

	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_CREATION,"reading object with type:%c\n",type);
	#endif
	switch (type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_ELLIPSIS:
		{
			OBJECT_ID obj_id = obj_CreateEmpty(type);
			obj_IncRefCount(obj_id);
			return (obj_id);
		}
		case TYPE_CODE:
       {	
			OBJECT_ID co_id = obj_Alloc(TYPE_CODE,1);
			code_object *co = (code_object*)mem_lock(co_id);
			co->stacksize = 0;
			co->argcount = (NUM)stream_ReadLong(stream);
			co->kwonlyargcount = (NUM)stream_ReadLong(stream);
			co->nlocals = (NUM)stream_ReadLong(stream);
			co->co_flags = stream_ReadLong(stream);
			co->code = stream_ReadObjectPlus(stream);
			co->consts = stream_ReadObjectPlus(stream);
			co->names = stream_ReadObjectPlus(stream);
			co->varnames = stream_ReadObjectPlus(stream);
			co->freevars = stream_ReadObjectPlus(stream);
			co->cellvars = stream_ReadObjectPlus(stream);
			tuple_ConvertToDict(co->freevars); //TODO only needed as long deref and closure opcodes are not implemented using references
			tuple_ConvertToDict(co->cellvars);
			OBJECT_ID tmp_name_id = stream_ReadObjectPlus(stream);
			unicode_object *tmp_name = (unicode_object*)mem_lock(tmp_name_id);
			co->name = mem_copy(tmp_name->value);
			mem_unlock(tmp_name_id,0);
			obj_DecRefCount(tmp_name_id);
			obj_ClearGC();
			mem_unlock(co_id,1);
			return(co_id);
		}
		break;
		case TYPE_STRING:
		{
			OBJECT_ID obj_id = obj_Alloc(TYPE_STRING,1);
			string_object *obj = (string_object*)mem_lock(obj_id);
			NUM n;
			n = (NUM)stream_ReadLong(stream);
			if (n > 0)
			{
				#ifdef USE_MEMORY_DEBUGGING
				MEM_ID string = mem_malloc_debug(n,MEM_POOL_CLASS_DYNAMIC, "ReadObject() string");
				#else
				MEM_ID string = mem_malloc(n,MEM_POOL_CLASS_DYNAMIC);
				#endif
				stream_Read(stream,string, 0,n);
				obj->len = n;
				obj->content = string;
			}
			else
			{
				obj->content = 0;
				//obj->type = TYPE_NULL;
				obj->len = 0;
			}
			mem_unlock(obj_id,1);
			return(obj_id);
		}
		break;
		case TYPE_TUPLE:
		{
			INT n = (INT)stream_ReadLong(stream);
			OBJECT_ID to_id = obj_Alloc(TYPE_TUPLE,1);
			tuple_object *to = (tuple_object*)mem_lock(to_id);
			to->ptr = 0;
			if (n > 0)
			{
				to->list = list_Create(n,LIST_STATIC);
				for (NUM i = 0; i < n; i++)
				{
					OBJECT_ID tuple = stream_ReadObject(stream);
					list_Set(to->list,i,tuple);
				}
			}
			else
				to->list = 0;
			mem_unlock(to_id,1);
			return(to_id);
		}
		break;
		case TYPE_UNICODE:
		{
			OBJECT_ID obj_id = obj_Alloc(TYPE_UNICODE,1);
			unicode_object *obj= (unicode_object*)mem_lock(obj_id);
			NUM n = (NUM)stream_ReadLong(stream);
			#ifdef USE_MEMORY_DEBUGGING
			MEM_ID unicode_string = mem_malloc_debug(n + 1,MEM_POOL_CLASS_DYNAMIC, "ReadObject() unicode_string");
			#else
			MEM_ID unicode_string = mem_malloc(n + 1,MEM_POOL_CLASS_DYNAMIC);
			#endif
			mem_set(unicode_string, 0, n + 1);
			stream_Read(stream,unicode_string, 0,n);
			obj->value = unicode_string;
			mem_unlock(obj_id,1);
			return(obj_id);
		}
		break;
		case TYPE_BINARY_FLOAT:
		{
			OBJECT_ID obj_id = obj_Alloc(TYPE_BINARY_FLOAT,1);
			float_object *obj= (float_object*)mem_lock(obj_id);
			FLOAT n = (FLOAT)stream_ReadFloat(stream);
			obj->value = n;
			mem_unlock(obj_id,1);
			return(obj_id);
		}
		break;
		case TYPE_INT:
		{
			OBJECT_ID obj_id = obj_Alloc(TYPE_INT,1);
			int_object *obj= (int_object*)mem_lock(obj_id);
			INT n = (INT)stream_ReadLong(stream);
			obj->value = n;
			mem_unlock(obj_id,1);
			return(obj_id);
		}
		break;
	default:
		{
			OBJECT_ID obj_id = obj_CreateEmpty(TYPE_UNKNOWN);
			obj_IncRefCount(obj_id);
			return(obj_id);
		}
	}
	return(0);
}

void stream_WriteObjectPlus(OBJECT_ID obj_id,STREAM_ID stream)
{
	object *obj = (object*)mem_lock(obj_id);
	stream_WriteChar(obj->type,stream);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_CREATION,"writing object with type:%c\n",obj->type);
	#endif

	switch(obj->type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_ELLIPSIS:
		{
			mem_unlock(obj_id,0);
			return;
		}
		case TYPE_CODE:
       {
			stream_WriteLong(((code_object*)obj)->argcount,stream);
			stream_WriteLong(((code_object*)obj)->kwonlyargcount,stream);
			stream_WriteLong(((code_object*)obj)->nlocals,stream);
			stream_WriteLong(((code_object*)obj)->co_flags,stream);
			stream_WriteObject(((code_object*)obj)->code,stream);
			stream_WriteObject(((code_object*)obj)->consts,stream);
			stream_WriteObject(((code_object*)obj)->names,stream);
			stream_WriteObject(((code_object*)obj)->varnames,stream);
			stream_WriteObject(((code_object*)obj)->freevars,stream);
			stream_WriteObject(((code_object*)obj)->cellvars,stream);
			OBJECT_ID name = obj_CreateUnicode(((code_object*)obj)->name);
			obj_IncRefCount(name);
			stream_WriteObject(name,stream);
			obj_DecRefCount(name);
			obj_ClearGC();
			mem_unlock(obj_id,0);
			return;
		}
		case TYPE_STRING:
		{
			NUM n = ((string_object*)obj)->len;
			stream_WriteLong(n,stream);
			if(n > 0)
			{
				stream_Write(stream,((string_object*)obj)->content, 0,n);
			}	
			mem_unlock(obj_id,0);
			return;
		}
		case TYPE_TUPLE:
		{
			INT n = tuple_GetLen(obj_id);
			stream_WriteLong(n,stream);
			if(n > 0)
			{
				for(NUM i = 0; i < n; i++)
				{
					stream_WriteObject(tuple_GetItem(obj_id,i),stream);
				}
			}
			else
				stream_WriteLong(0,stream);
			mem_unlock(obj_id,0);
			return;
		}
		case TYPE_UNICODE:
		{
			NUM n = obj_GetUnicodeLen(obj_id);
			stream_WriteLong(n,stream);
			if(n > 0)
			{
				stream_Write(stream,((unicode_object*)obj)->value, 0,n);
			}
			mem_unlock(obj_id,0);
			return;
		}
		case TYPE_BINARY_FLOAT:
		{
			stream_WriteFloat(((float_object*)obj)->value,stream);
			mem_unlock(obj_id,0);
			return;
		}
		case TYPE_INT:
		{
			stream_WriteLong(((int_object*)obj)->value,stream);
			mem_unlock(obj_id,0);
			return;
		}
		default:
		{
			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", obj->type);
			#endif
			mem_unlock(obj_id,0);
			return;
		}
	}
}

#endif




