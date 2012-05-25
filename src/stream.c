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
	#ifdef USE_DEBUGGING
	return((stream*) mem_malloc(sizeof(stream), "AllocStream() return"));
	#else
	return((stream*) malloc(sizeof(stream)));
	#endif
}

streams *AllocStreams(void)
{
	#ifdef USE_DEBUGGING
	return((streams*) mem_malloc(sizeof(streams), "AllocStreams() return"));
	#else
	return((streams*) malloc(sizeof(streams)));
	#endif
}

stream_type *AllocStreamType(void)
{
	#ifdef USE_DEBUGGING
	return((stream_type*) mem_malloc(sizeof(stream_type), "AllocStreamType() return"));
	#else
	return((stream_type*) malloc(sizeof(stream_type)));
	#endif
}

char *AllocStreamBytes(STREAM_NUM len)
{
	#ifdef USE_DEBUGGING
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
	ftype->stream_getpos = &stream_file_getpos;
	ptr_Push(stream_types,ftype);
	#endif
	
	#ifdef USE_ARDUINO_FUNCTIONS
	//arduino flash memory stream
	stream_type *fmtype = AllocStreamType();
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
	ptr_Push(stream_types,fmtype);

	
	//arduino serial stream

	#endif
	//standard io

}

struct _stream_type *streams_GetStreamType(STREAM_TYPE_ID id)
{
	INDEX i = 0;	
	do
	{
		if(((struct _stream_type*)stream_types->items[i])->type == id)
			return(stream_types->items[i]);
		i++;
	}while(i < stream_types->num);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"stream type not defined:%c\n",id);
	#endif
	return(NULL);
}

void streams_Close(void)
{
	INDEX i = 0;	
	do
	{
		#ifdef USE_DEBUGGING
		assert(mem_free(stream_types->items[i]));
		#else
		free(stream_types->items[i]);
		#endif		
		i++;
	}while(i < stream_types->num);
	ptr_CloseList(stream_types);
	#ifdef USE_DEBUGGING
	assert(mem_free(stream_globals));
	#else
	free(stream_globals);
	#endif		
}

#ifndef USE_ARDUINO_FUNCTIONS
stream *stream_CreateFromFile(char *filename,char *flags)
{
	stream *str = AllocStream();
	ptr_list *fopt = ptr_CreateList(0,0);
	char *fname = str_Copy(filename);
	ptr_Push(fopt,fname);
	str->type = streams_GetStreamType(STREAM_TYPE_FILE);
	str->tags = fopt;
	str->flags = str_Copy(flags);
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
	str->type = streams_GetStreamType(STREAM_TYPE_MEMORY);
	str->flags = NULL;
	/*tuple_object *b = CreateTuple(4,0);
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
	//pri
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
	//debug_printf(DEBUG_ALL,"open:%x\n",str->type->stream_open);
	str->tags = mopt;
	str->flags = NULL;
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
	if(stream->type->stream_open == NULL)
		return(0);
	BOOL b = stream->type->stream_open(stream);
	return(b);
}

BOOL stream_Close(struct _stream *stream)
{
	if(stream->type->stream_close == NULL)
		return(0);
	return(stream->type->stream_close(stream));
}

BOOL stream_Free(struct _stream *stream)
{
	if(stream->type->stream_free == NULL)
		return(0);
	return(stream->type->stream_free(stream));
}

BOOL stream_Read(struct _stream *stream,void *ptr,STREAM_NUM len)
{
	if(stream->type->stream_read == NULL)
		return(0);
	return(stream->type->stream_read(stream,ptr,len));
}

BOOL stream_Write(struct _stream *stream,void *ptr ,STREAM_NUM len)
{
	if(stream->type->stream_write == NULL)
		return(0);
	return(stream->type->stream_write(stream,ptr,len));
}

BOOL stream_Seek(struct _stream *stream,STREAM_NUM offset,STREAM_ORIGIN origin)
{
	if(stream->type->stream_seek == NULL)
		return(0);
	return(stream->type->stream_seek(stream,offset,origin));
}

INT stream_GetPos(struct _stream *stream)
{
	if(stream->type->stream_getpos == NULL)
		return(0);
	return(stream->type->stream_getpos(stream));
}
//file stream functions

#ifndef USE_ARDUINO_FUNCTIONS
BOOL stream_file_open(struct _stream *stream)
{
	FILE *f;
	char *filename = (char*)ptr_Get(stream->tags,0);
	#if defined(USE_DEBUGGING)
	debug_printf(DEBUG_VERBOSE_TESTS,"opened file:%s\n",filename);
	#endif
	if(stream->flags != NULL)
	f = fopen(filename, stream->flags);
	else
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
	#ifdef USE_DEBUGGING
	assert(mem_free(filename));
	#else
	free(filename);
	#endif		
	
	ptr_CloseList(stream->tags);
	//if(stream->flags != NULL)
	//	printf("stream flags:[%s]\n",stream->flags);
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

BOOL stream_file_read(struct _stream *stream,void *ptr,STREAM_NUM len)
{
	if(stream->tags->num<=1)
		return(0);
	FILE *f = (FILE*)ptr_Get(stream->tags,1);
	if(fread(ptr, len, 1, f) != 1)
	{
		//printf("eof\n");
		return(0);
	}
	//printf("read data\n");
	return(1);
}

BOOL stream_file_write(struct _stream *stream,void *ptr ,STREAM_NUM len)
{
	if(stream->tags->num<=1)
		return(0);
	FILE *f = (FILE*)ptr_Get(stream->tags,1);
	if(fwrite(ptr, len, 1, f) != 1)
		return(0);
	return(1);
}

BOOL stream_file_seek(struct _stream *stream,STREAM_NUM offset,STREAM_ORIGIN origin)
{ 
	if(stream->tags->num<=1)
		return(0);
	FILE *f = (FILE*)ptr_Get(stream->tags,1);
	int org = 0;
	if(origin == STREAM_ORIGIN_SET)
		org = SEEK_SET;
	else if(origin == STREAM_ORIGIN_CUR)
		org = SEEK_CUR;
	else if(origin == STREAM_ORIGIN_END)
		org = SEEK_END;
	return(fseek(f,offset,org));
}

INT stream_file_getpos(struct _stream *stream)
{ 
	if(stream->tags->num<=1)
		return(0);
	FILE *f = (FILE*)ptr_Get(stream->tags,1);
	if(feof(f))
		return(-1);//triple usage of getpos as feof detector and error detector
	return(ftell(f));
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
BOOL stream_memory_free(struct _stream *stream)
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

BOOL stream_memory_read(struct _stream *stream,void *ptr,STREAM_NUM len)
{
	char *bytes = (char*)ptr_Get(stream->tags,0);
	STREAM_NUM offset = (STREAM_NUM)ptr_Get(stream->tags,2);
	memcpy(ptr,*(&bytes+offset),len);
	ptr_Set(stream->tags,2,(void*)(offset+len));
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
#endif



long stream_ReadLong(struct _stream *f) //TODO move to streams
{
	long r = 0;
	stream_Read(f,&r, 4);
	return (r);
}

FLOAT stream_ReadFloat(struct _stream *f)
{
	float r = 0;
	double t = 0;
	stream_Read(f, &t,8);
	r = (FLOAT)t;
	return (r);
}

char stream_ReadChar(struct _stream *f)
{
	char r = 0;
	stream_Read(f,&r,1);
	return (r);
}

void stream_WriteLong(long l,struct _stream *f)
{
	stream_Write(f,&l,4);
}

void stream_WriteFloat(FLOAT fl,struct _stream *f)
{
	double d = (double)fl;
	stream_Write(f,&d,8);
}

void stream_WriteChar(char c,struct _stream *f)
{
	stream_Write(f,&c,1);
}

struct _object *stream_ReadObject(struct _stream *f)
{
	OBJECT_TYPE type = stream_ReadChar(f);

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
		/*
		// printf("allocating empty object\n");
		object *obj = AllocObject();
		obj->type = type;
		//obj->flags = 0;
		obj->ref_count = 1;
		// printf("allocated empty object @%x\n",obj);
		*/
		object *obj = CreateEmptyObject(type);
		gc_IncRefCount(obj);
		return (obj);
		}
	case TYPE_CODE:
       {	
		code_object *co = AllocCodeObject();
		co->type = TYPE_CODE;
		co->ref_count = 1;
		co->argcount = (NUM)stream_ReadLong(f);
		co->kwonlyargcount = (NUM)stream_ReadLong(f);
		co->nlocals = (NUM)stream_ReadLong(f);
		co->stacksize = (NUM)stream_ReadLong(f);
		co->co_flags = stream_ReadLong(f);
		co->code = stream_ReadObject(f);
		co->consts = stream_ReadObject(f);
		co->names = stream_ReadObject(f);
		co->varnames = stream_ReadObject(f);
		co->freevars = stream_ReadObject(f);
		co->cellvars = stream_ReadObject(f);
		ConvertToDict(co->freevars); //TODO only needed as long deref and closure opcodes are not implemented using references
		ConvertToDict(co->cellvars);
		object *tmp_filename = stream_ReadObject(f);
		gc_DecRefCount(tmp_filename);	// TO DECREASE MEMORY USAGE
		object *tmp_name = stream_ReadObject(f);
		co->name = str_Copy(((unicode_object *)tmp_name)->value);
		gc_DecRefCount(tmp_name);
		stream_ReadLong(f);//firstlineno
		object *tmp_lnotab = stream_ReadObject(f);
		gc_DecRefCount(tmp_lnotab);	// TO DECREASE MEMORY USAGE
		gc_Clear();
		return((object*)co);
		}
		break;
	case TYPE_STRING:
		{
		string_object *obj = AllocStringObject();

		obj->ref_count = 1;

		NUM n;
		// printf("reading string chunk\n");
		n = (NUM)stream_ReadLong(f);
		if (n > 0)
		{
			#ifdef USE_DEBUGGING
			char *string = (char *)mem_malloc(n, "ReadObject() string");
			#else
			char *string = (char *)malloc(n);
			#endif

			// printf("str_ptr=%x\n",(unsigned long)string);
			stream_Read(f,string, n);

			// printf("str_so_ptr=%x\n",(unsigned long)so);
			obj->len = n;
			obj->content = string;
			// free(string); 

			 //printf("read string:\"%s\"\r\n",obj->content);
			//printf("n:%d\n",obj->len);
			obj->type = TYPE_STRING;
		}
		else
		{
			// printf ("read empty string\n");
			obj->content = NULL;
			obj->type = TYPE_NULL;
			obj->len = 0;
		}
		return((object*)obj);
		}
		break;
	case TYPE_TUPLE:
		{
		// printf("reading tuple\n");
		INT n = (INT)stream_ReadLong(f);
		tuple_object *to = AllocTupleObject();
		to->ref_count = 1;
		to->ptr = 0;

		// printf("items_num:%d\n",n);
		if (n > 0)
		{
			//to->items =
			//	(object **) mem_malloc(n * sizeof(object *),
			//						   "ReadObject() to->items");
			to->list = ptr_CreateList(n,PTR_STATIC_LIST);
			for (NUM i = 0; i < n; i++)
			{
				object *tuple = stream_ReadObject(f);

				//if (i == 0)
				//{
				//	to->ptr = tuple;
					// printf("setting tuple's iterate over ptr\n");
				//}
				// printf("object type:%c\n",tuple->type);
				to->list->items[i] = tuple;
			}
		}
		else
		 to->list = NULL;
		//to->num = n;
		//to->flags = 0;
		to->type = TYPE_TUPLE;
		return((object*)to);
		}
		break;
	case TYPE_UNICODE:
		{
		// printf("reading unicode\n");
		unicode_object *obj = AllocUnicodeObject();
		obj->ref_count = 1;

		NUM n = (NUM)stream_ReadLong(f);
		// printf("len:%d\n",n);
		#ifdef USE_DEBUGGING
		char *unicode_string = (char *)mem_malloc(n + 1, "ReadObject() unicode_string");
		#else
		char *unicode_string = (char *)malloc(n + 1);
		#endif
		memset(unicode_string, 0, n + 1);
		stream_Read(f,unicode_string, n);
		//printf("read unicode string:%s\n",unicode_string);
		// uo->len = n;
		//obj->flags = 0;
		obj->value = unicode_string;
		obj->type = TYPE_UNICODE;
		return((object*)obj);
		}
		break;
	case TYPE_BINARY_FLOAT:
		{
		float_object *obj = AllocFloatObject();
		FLOAT n = (FLOAT)stream_ReadFloat(f);
		obj->ref_count = 1;
		//obj->flags = 0;
		obj->value = n;
		//printf("read float:%7g\n",obj->value);
		obj->type = TYPE_BINARY_FLOAT;
		return((object*)obj);
		}
		break;
	case TYPE_INT:
		{
		// printf("reading int\n");
		int_object *obj = AllocIntObject();
		INT n = (INT)stream_ReadLong(f);
		obj->ref_count = 1;
		//obj->flags = 0;
		obj->value = n;
		obj->type = TYPE_INT;
		return((object*)obj);
		}
		break;
	default:
		{
		/*
		object *obj = AllocObject();
		obj->ref_count = 1;
		obj->type = TYPE_UNKNOWN;
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", type);
		#endif
		return(obj);
		*/
		object *obj = CreateEmptyObject(TYPE_UNKNOWN);
		gc_IncRefCount(obj);
		return (obj);
		}
	}
	// printf("read object\r\n"); 
	return (NULL);
}

void stream_WriteObject(struct _object *obj,struct _stream *f)
{
	stream_WriteChar(obj->type,f);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_CREATION,"writing object with type:%c\n",obj->type);
	#endif
	//printf("writing object:\n");
	//DumpObject(obj,0);
	switch(obj->type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_ELLIPSIS:
		{
		return;
		}
	case TYPE_CODE:
       {
		stream_WriteLong(((code_object*)obj)->argcount,f);
		stream_WriteLong(((code_object*)obj)->kwonlyargcount,f);
		stream_WriteLong(((code_object*)obj)->nlocals,f);
		stream_WriteLong(((code_object*)obj)->stacksize,f);
		stream_WriteLong(((code_object*)obj)->co_flags,f);
		stream_WriteObject(((code_object*)obj)->code,f);
		stream_WriteObject(((code_object*)obj)->consts,f);
		stream_WriteObject(((code_object*)obj)->names,f);
		stream_WriteObject(((code_object*)obj)->varnames,f);
		stream_WriteObject(((code_object*)obj)->freevars,f);
		stream_WriteObject(((code_object*)obj)->cellvars,f);
		unicode_object *filename = CreateUnicodeObject("");
		gc_IncRefCount((object*)filename);
		stream_WriteObject((object*)filename,f);
		gc_DecRefCount((object*)filename);
		unicode_object *name = CreateUnicodeObject(((code_object*)obj)->name);
		gc_IncRefCount((object*)name);
		stream_WriteObject((object*)name,f);
		gc_DecRefCount((object*)name);
		stream_WriteLong(0,f);
		object *lnotab = CreateEmptyObject(TYPE_NONE);
		gc_IncRefCount((object*)lnotab);
		stream_WriteObject(lnotab,f);
		gc_DecRefCount(lnotab);
		gc_Clear();
		return;
		}
	case TYPE_STRING:
		{
		NUM n = ((string_object*)obj)->len;
		stream_WriteLong(n,f);
		if (n > 0)
		{
			stream_Write(f,((string_object*)obj)->content, n);
		}
		return;
		}
	case TYPE_TUPLE:
		{
		if(((tuple_object*)obj)->list != NULL)
		{
			INT n = ((tuple_object*)obj)->list->num;
			stream_WriteLong(n,f);
			//printf("tuple items:%d\n",n);
			if (n > 0)
			{
				for (NUM i = 0; i < n; i++)
				{
					stream_WriteObject(((tuple_object*)obj)->list->items[i],f);
				}
			}
		}
		else
			stream_WriteLong(0,f);
		
		//printf("written tuple\n");
		return;
		}
	case TYPE_UNICODE:
		{
		NUM n = strlen(((unicode_object*)obj)->value);
		stream_WriteLong(n,f);
		if(n > 0)
		{
			stream_Write(f,((unicode_object*)obj)->value, n);
		}
		return;
		}
	case TYPE_BINARY_FLOAT:
		{
		stream_WriteFloat(((float_object*)obj)->value,f);
		return;
		}
	case TYPE_INT:
		{
		stream_WriteLong(((int_object*)obj)->value,f);
		return;
		}
	default:
		{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", obj->type);
		#endif
		return;
		}
	}
}

#ifndef USE_ARDUINO_FUNCTIONS
//same as above , but using a reduced code object layout
struct _object *stream_ReadObjectPlus(struct _stream *f)
{
	OBJECT_TYPE type = stream_ReadChar(f);

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
		/*
		object *obj = AllocObject();
		obj->type = type;
		obj->ref_count = 1;
		*/
		object *obj = CreateEmptyObject(type);
		gc_IncRefCount(obj);
		return (obj);
		}
	case TYPE_CODE:
       {	
		code_object *co = AllocCodeObject();
		co->type = TYPE_CODE;
		co->ref_count = 1;
		co->stacksize = 0;

		co->argcount = (NUM)stream_ReadLong(f);
		co->kwonlyargcount = (NUM)stream_ReadLong(f);
		co->nlocals = (NUM)stream_ReadLong(f);
		co->co_flags = stream_ReadLong(f);
		co->code = stream_ReadObjectPlus(f);
		co->consts = stream_ReadObjectPlus(f);
		co->names = stream_ReadObjectPlus(f);
		co->varnames = stream_ReadObjectPlus(f);
		co->freevars = stream_ReadObjectPlus(f);
		co->cellvars = stream_ReadObjectPlus(f);
		ConvertToDict(co->freevars); //TODO only needed as long deref and closure opcodes are not implemented using references
		ConvertToDict(co->cellvars);
		object *tmp_name = stream_ReadObjectPlus(f);
		co->name = str_Copy(((unicode_object *)tmp_name)->value);
		gc_DecRefCount(tmp_name);
		gc_Clear();
		return((object*)co);
		}
		break;
	case TYPE_STRING:
		{
		string_object *obj = AllocStringObject();
		obj->ref_count = 1;
		NUM n;
		n = (NUM)stream_ReadLong(f);
		if (n > 0)
		{
			#ifdef USE_DEBUGGING
			char *string = (char *)mem_malloc(n, "ReadObject() string");
			#else
			char *string = (char *)malloc(n);
			#endif
			stream_Read(f,string, n);
			obj->len = n;
			obj->content = string;
			obj->type = TYPE_STRING;
		}
		else
		{
			obj->content = NULL;
			obj->type = TYPE_NULL;
			obj->len = 0;
		}
		return((object*)obj);
		}
		break;
	case TYPE_TUPLE:
		{
		INT n = (INT)stream_ReadLong(f);
		tuple_object *to = AllocTupleObject();
		to->ref_count = 1;
		to->ptr = 0;
		if (n > 0)
		{
			to->list = ptr_CreateList(n,PTR_STATIC_LIST);
			for (NUM i = 0; i < n; i++)
			{
				object *tuple = stream_ReadObjectPlus(f);
				to->list->items[i] = tuple;
			}
		}
		else
		 to->list = NULL;
		to->type = TYPE_TUPLE;
		return((object*)to);
		}
		break;
	case TYPE_UNICODE:
		{
		unicode_object *obj = AllocUnicodeObject();
		obj->ref_count = 1;

		NUM n = (NUM)stream_ReadLong(f);
		#ifdef USE_DEBUGGING
		char *unicode_string = (char *)mem_malloc(n + 1, "ReadObject() unicode_string");
		#else
		char *unicode_string = (char *)malloc(n + 1);
		#endif
		memset(unicode_string, 0, n + 1);
		stream_Read(f,unicode_string, n);
		obj->value = unicode_string;
		obj->type = TYPE_UNICODE;
		return((object*)obj);
		}
		break;
	case TYPE_BINARY_FLOAT:
		{
		float_object *obj = AllocFloatObject();
		FLOAT n = (FLOAT)stream_ReadFloat(f);
		obj->ref_count = 1;
		obj->value = n;
		obj->type = TYPE_BINARY_FLOAT;
		return((object*)obj);
		}
		break;
	case TYPE_INT:
		{
		int_object *obj = AllocIntObject();
		INT n = (INT)stream_ReadLong(f);
		obj->ref_count = 1;
		obj->value = n;
		obj->type = TYPE_INT;
		return((object*)obj);
		}
		break;
	default:
		{
		/*
		object *obj = AllocObject();
		obj->ref_count = 1;
		obj->type = TYPE_UNKNOWN;
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", type);
		#endif
		return(obj);
		*/
		object *obj = CreateEmptyObject(TYPE_UNKNOWN);
		gc_IncRefCount(obj);
		return (obj);
		}
	}
	return (NULL);
}

void stream_WriteObjectPlus(struct _object *obj,struct _stream *f)
{
	stream_WriteChar(obj->type,f);
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
		return;
		}
	case TYPE_CODE:
       {
		stream_WriteLong(((code_object*)obj)->argcount,f);
		stream_WriteLong(((code_object*)obj)->kwonlyargcount,f);
		stream_WriteLong(((code_object*)obj)->nlocals,f);
		stream_WriteLong(((code_object*)obj)->co_flags,f);
		stream_WriteObject(((code_object*)obj)->code,f);
		stream_WriteObject(((code_object*)obj)->consts,f);
		stream_WriteObject(((code_object*)obj)->names,f);
		stream_WriteObject(((code_object*)obj)->varnames,f);
		stream_WriteObject(((code_object*)obj)->freevars,f);
		stream_WriteObject(((code_object*)obj)->cellvars,f);
		unicode_object *name = CreateUnicodeObject(((code_object*)obj)->name);
		gc_IncRefCount((object*)name);
		stream_WriteObject((object*)name,f);
		gc_DecRefCount((object*)name);
		gc_Clear();
		return;
		}
	case TYPE_STRING:
		{
		NUM n = ((string_object*)obj)->len;
		stream_WriteLong(n,f);
		if (n > 0)
		{
			stream_Write(f,((string_object*)obj)->content, n);
		}
		return;
		}
	case TYPE_TUPLE:
		{
		if(((tuple_object*)obj)->list != NULL)
		{
			INT n = ((tuple_object*)obj)->list->num;
			stream_WriteLong(n,f);
			if (n > 0)
			{
				for (NUM i = 0; i < n; i++)
				{
					stream_WriteObject(((tuple_object*)obj)->list->items[i],f);
				}
			}
		}
		else
			stream_WriteLong(0,f);
		return;
		}
	case TYPE_UNICODE:
		{
		NUM n = strlen(((unicode_object*)obj)->value);
		stream_WriteLong(n,f);
		if(n > 0)
		{
			stream_Write(f,((unicode_object*)obj)->value, n);
		}
		return;
		}
	case TYPE_BINARY_FLOAT:
		{
		stream_WriteFloat(((float_object*)obj)->value,f);
		return;
		}
	case TYPE_INT:
		{
		stream_WriteLong(((int_object*)obj)->value,f);
		return;
		}
	default:
		{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", obj->type);
		#endif
		return;
		}
	}
}

#endif




