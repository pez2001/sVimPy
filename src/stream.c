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

ptr_list *stream_types;
streams *stream_globals;

stream *AllocStream(void)
{
	#ifdef DEBUGGING
	return((stream*) mem_malloc(sizeof(stream), "AllocStream() return"));
	#else
	return((stream*) malloc(sizeof(stream)));
	#endif
}

streams *AllocStreams(void)
{
	#ifdef DEBUGGING
	return((streams*) mem_malloc(sizeof(streams), "AllocStreams() return"));
	#else
	return((streams*) malloc(sizeof(streams)));
	#endif
}

stream_type *AllocStreamType(void)
{
	#ifdef DEBUGGING
	return((stream_type*) mem_malloc(sizeof(stream_type), "AllocStreamType() return"));
	#else
	return((stream_type*) malloc(sizeof(stream_type)));
	#endif
}

char *AllocStreamBytes(STREAM_NUM len)
{
	#ifdef DEBUGGING
	return((char*) mem_malloc(len, "AllocStreamBytes() return"));
	#else
	return((char*) malloc(len));
	#endif
}


void streams_Init(void)
{
	stream_types = ptr_CreateList(0,0);
	stream_globals = AllocStreams();
	//memory stream
	stream_type *mtype = AllocStreamType();
	mtype->type = STREAM_TYPE_MEMORY;
	mtype->stream_open = &stream_memory_open;
	mtype->stream_close = &stream_memory_close;
	mtype->stream_free = &stream_memory_free;
	mtype->stream_read = &stream_memory_read;
	mtype->stream_write = &stream_memory_write;
	mtype->stream_seek = &stream_memory_seek;
	ptr_Push(stream_types,mtype);
	//file stream
	#ifndef USE_ARDUINO_FUNCTIONS
	stream_type *ftype = AllocStreamType();
	ftype->type = STREAM_TYPE_FILE;
	ftype->stream_open = &stream_file_open;
	ftype->stream_close = &stream_file_close;
	ftype->stream_free = &stream_file_free;
	ftype->stream_read = &stream_file_read;
	ftype->stream_write = &stream_file_write;
	ftype->stream_seek = &stream_file_seek;
	ptr_Push(stream_types,ftype);
	#endif
	
	#ifdef USE_ARDUINO_FUNCTIONS
	//arduino flash memory stream
	stream_type *fmtype = AllocStreamType();
	fmtype->type = STREAM_TYPE_FLASH_MEMORY;
	fmtype->stream_open = &stream_flash_memory_open;
	fmtype->stream_close = &stream_flash_memory_close;
	fmtype->stream_free = &stream_flash_memory_free;
	fmtype->stream_read = &stream_flash_memory_read;
	fmtype->stream_write = &stream_flash_memory_write;
	fmtype->stream_seek = &stream_flash_memory_seek;
	ptr_Push(stream_types,fmtype);


	//arduino serial stream

	#endif
	//standard io

}

stream_type* streams_GetStreamType(STREAM_TYPE_ID id)
{
	INDEX i = 0;	
	do
	{
		if(((stream_type*)stream_types->items[i])->type == id)
			return(stream_types->items[i]);
		i++;
	}while(i < stream_types->num);
	//printf("stream type not defined:%c\n",id);
	return(NULL);
}

void streams_Close(void)
{
	INDEX i = 0;	
	do
	{
		#ifdef DEBUGGING
		assert(mem_free(stream_types->items[i]));
		#else
		free(stream_types->items[i]);
		#endif		
		i++;
	}while(i < stream_types->num);
	ptr_CloseList(stream_types);
	#ifdef DEBUGGING
	assert(mem_free(stream_globals));
	#else
	free(stream_globals);
	#endif		
}

#ifndef USE_ARDUINO_FUNCTIONS
stream *stream_CreateFromFile(char *filename)
{
	stream *str = AllocStream();
	ptr_list *fopt = ptr_CreateList(0,0);
	char *fname = str_Copy(filename);
	ptr_Push(fopt,fname);
	str->type = streams_GetStreamType(STREAM_TYPE_FILE);
	str->tags = fopt;
	return(str);
}
#endif

stream *stream_CreateFromBytes(char *bytes ,STREAM_NUM len)
{
	stream *str = AllocStream();
	ptr_list *mopt = ptr_CreateList(0,0);
	ptr_Push(mopt,bytes);
	ptr_Push(mopt,(void*)len);
	STREAM_NUM offset = 0;
	ptr_Push(mopt,(void*)offset);
	//printf("getting stream type\n");
	/*str->type = streams_GetStreamType(STREAM_TYPE_MEMORY);

	tuple_object *b = CreateTuple(4,0);
	string_object s* = CreateStringObject(bytes,len,0);
	SetItem((object*)b,0,(object*)s);
	int_object *ioffset = CreateIntObject(offset,0);
	SetItem((object*)seq,1,(object*)istep);
	int_object *ipos = CreateIntObject(start,0);
	SetItem((object*)seq,2,(object*)ipos);
	int_object *istart = CreateIntObject(start,0);
	SetItem((object*)seq,3,(object*)istart);
	iter->tag = (object*)seq;
    */
	//printf("got stream type\n");
	//printf("open:%x\n",str->type->stream_open);
	str->tags = mopt;
	return(str);
}

#ifdef USE_ARDUINO_FUNCTIONS
stream *stream_CreateFromFlashBytes(char *bytes ,STREAM_NUM len)
{
	stream *str = AllocStream();
	ptr_list *mopt = ptr_CreateList(0,0);
	ptr_Push(mopt,bytes);
	ptr_Push(mopt,(void*)len);
	ptr_Push(mopt,0);
	//printf("getting stream type\n");
	str->type = streams_GetStreamType(STREAM_TYPE_FLASH_MEMORY);
	//printf("got stream type\n");
	//printf("open:%x\n",str->type->stream_open);
	str->tags = mopt;
	return(str);
}
#endif

stream *stream_CreateStandardOutput(void)
{
	stream * str = AllocStream();
	str->type = streams_GetStreamType(STREAM_TYPE_STANDARD_OUTPUT);
	str->tags = NULL;
	return(str);
}

stream *stream_CreateStandardInput(void)
{
	stream * str = AllocStream();
	str->type = streams_GetStreamType(STREAM_TYPE_STANDARD_INPUT);
	str->tags = NULL;
	return(str);
}

stream *stream_CreateDebugOutput(void)
{
	stream * str = AllocStream();
	str->type = streams_GetStreamType(STREAM_TYPE_DEBUG_OUTPUT);
	str->tags = NULL;
	return(str);
}

//stream access functions

BOOL stream_Open(struct _stream *stream)
{
	//printf("stream opening:%x\n",stream->type->stream_open);
	//BOOL b = stream->type->stream_open(stream);
	//printf("stream opened\n");
	//return(b);
	return(stream->type->stream_open(stream));
}

BOOL stream_Close(struct _stream *stream)
{
	return(stream->type->stream_close(stream));
}

BOOL stream_Free(struct _stream *stream)
{
	return(stream->type->stream_free(stream));
}

BOOL stream_Read(struct _stream *stream,void *ptr,STREAM_NUM len)
{
	return(stream->type->stream_read(stream,ptr,len));
}

BOOL stream_Write(struct _stream *stream,char *bytes ,STREAM_NUM len)
{
	return(stream->type->stream_write(stream,bytes,len));
}

BOOL stream_Seek(struct _stream *stream,STREAM_NUM offset)
{
	return(stream->type->stream_seek(stream,offset));
}

//file stream functions

#ifndef USE_ARDUINO_FUNCTIONS
BOOL stream_file_open(struct _stream *stream)
{
	FILE *f;
	//printf("opening file stream\n");
	char *filename = (char*)ptr_Get(stream->tags,0);
	#if defined(USE_DEBUGGING)
	debug_printf(DEBUG_VERBOSE_TESTS,"opened file:%s\n",filename);
	#endif
	
	//printf("filename:%s\n",filename);
	f = fopen(filename, "rb");
	if (f == NULL)
		return(0);
	ptr_Push(stream->tags,f);
	return(1);
}

BOOL stream_file_close(struct _stream *stream)
{
	if(stream->tags->num<=1)
		return(0);
	FILE *f = (FILE*)ptr_Pop(stream->tags);
	fclose(f);
	return(1);
}

BOOL stream_file_free(struct _stream *stream)
{
	if(stream->tags->num> 1)
		stream_file_close(stream);
	char *filename = ptr_Pop(stream->tags);
	#ifdef DEBUGGING
	assert(mem_free(filename));
	#else
	free(filename);
	#endif		
	
	ptr_CloseList(stream->tags);
	#ifdef DEBUGGING
	assert(mem_free(stream));
	#else
	free(stream);
	#endif		

	return(1);
}

BOOL stream_file_read(struct _stream *stream,void *ptr,STREAM_NUM len)
{
	if(stream->tags->num<=1)
		return(0);
	FILE *f = (FILE*)ptr_Get(stream->tags,1);
	//char *bytes = AllocStreamBytes(len);
	if(fread(ptr, len, 1, f) != len)
		return(0);
	return(1);
}

BOOL stream_file_write(struct _stream *stream,char *bytes ,STREAM_NUM len)
{
	if(stream->tags->num<=1)
		return(0);
	//FILE *f = (FILE*)ptr_Get(stream->tags,1);
	return(0);
}

BOOL stream_file_seek(struct _stream *stream,STREAM_NUM offset)
{ 
	if(stream->tags->num<=1)
		return(0);
	//FILE *f = (FILE*)ptr_Get(stream->tags,1);
	return(0);
}
#endif

BOOL stream_memory_open(struct _stream *stream)
{
	//printf("memory opened\n");
	return(1);
}

BOOL stream_memory_close(struct _stream *stream)
{
	return(1);
}

BOOL stream_memory_free(struct _stream *stream)
{
/*	char *bytes = (char*)ptr_Get(stream->tags,0);
	STREAM_NUM = (STREAM_NUM)ptr_Get(stream->tags,1);
	#ifdef DEBUGGING
	assert(mem_free(filename));
	#else
	free(filename);
	#endif		
	*/
	ptr_CloseList(stream->tags);
	#ifdef DEBUGGING
	assert(mem_free(stream));
	#else
	free(stream);
	#endif		
	return(1);
}

BOOL stream_memory_read(struct _stream *stream,void *ptr,STREAM_NUM len)
{
	//printf("memory read\n");
	char *bytes = (char*)ptr_Get(stream->tags,0);
	//STREAM_NUM blen = (STREAM_NUM)ptr_Get(stream->tags,1);
	STREAM_NUM offset = (STREAM_NUM)ptr_Get(stream->tags,2);
	//printf("copying bytes :%d,%d\n",len,offset);
	//printf("bytes@%x,b+o:%x\n",bytes,(bytes+offset));
	memcpy(ptr,bytes+offset,len);
	//printf("updating offset\n");
	ptr_Set(stream->tags,2,(void*)(offset+len));
	return(1);
}

BOOL stream_memory_write(struct _stream *stream,char *bytes ,STREAM_NUM len)
{
	return(0);
}

BOOL stream_memory_seek(struct _stream *stream,STREAM_NUM offset)
{ 
	return(0);
}

#ifdef USE_ARDUINO_FUNCTIONS

BOOL stream_flash_memory_open(struct _stream *stream)
{
	//printf("memory opened\n");
	return(1);
}

BOOL stream_flash_memory_close(struct _stream *stream)
{
	return(1);
}

BOOL stream_flash_memory_free(struct _stream *stream)
{
	ptr_CloseList(stream->tags);
	#ifdef DEBUGGING
	assert(mem_free(stream));
	#else
	free(stream);
	#endif		
	return(1);
}

BOOL stream_flash_memory_read(struct _stream *stream,void *ptr,STREAM_NUM len)
{
	char *bytes = (char*)ptr_Get(stream->tags,0);
	//STREAM_NUM blen = (STREAM_NUM)ptr_Get(stream->tags,1);
	#if defined(USE_DEBUGGING)
	debug_printf(DEBUG_ALL," reading stream\r\n");
 	#endif
	STREAM_NUM offset = (STREAM_NUM)ptr_Get(stream->tags,2);
	memcpy_P(ptr,bytes+offset,len);
	ptr_Set(stream->tags,2,(void*)(offset+len));
	return(1);
}

BOOL stream_flash_memory_write(struct _stream *stream,char *bytes ,STREAM_NUM len)
{
	return(0);
}

BOOL stream_flash_memory_seek(struct _stream *stream,STREAM_NUM offset)
{ 
	return(0);
}

#endif




