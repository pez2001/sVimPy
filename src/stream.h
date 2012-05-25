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

#ifndef STREAM_H
#define STREAM_H

#include "types.h"
#include "debug.h"
#include "lists.h"
#include "strops.h"
#include "object.h"

#ifdef __cplusplus
extern "C"  {
#endif


#define STREAM_TYPE_FILE 'f'
#define STREAM_TYPE_MEMORY 'm'
#define STREAM_TYPE_FLASH_MEMORY 'F'
#define STREAM_TYPE_DEBUG_OUTPUT 'd'
#define STREAM_TYPE_STANDARD_INPUT 'i'
#define STREAM_TYPE_STANDARD_OUTPUT 'o'

/*#define STREAM_FLAG_NONE 0
#define STREAM_FLAG_READ 1
#define STREAM_FLAG_WRITE 2
#define STREAM_FLAG_APPEND 4
#define STREAM_FLAG_CREATE 8
#define STREAM_FLAG_READWRITE 16
#define STREAM_FLAG_FLUSH 32

#define STREAM_FLAG OBJECT_FLAG
*/
#define STREAM_ORIGIN OBJECT_FLAGS
#define STREAM_ORIGIN_SET 1
#define STREAM_ORIGIN_CUR 2
#define STREAM_ORIGIN_END 3

//TODO add function to return capabilities flag set for stream

struct _stream;

typedef struct _stream_type
{
	STREAM_TYPE_ID type;
	BOOL (*stream_open)(struct _stream *stream);
	BOOL (*stream_close)(struct _stream *stream);
	BOOL (*stream_free)(struct _stream *stream);	
	BOOL (*stream_read)(struct _stream *stream,void *ptr ,STREAM_NUM len);
	BOOL (*stream_write)(struct _stream *stream,void *ptr,STREAM_NUM len);
	BOOL (*stream_seek)(struct _stream *stream,STREAM_NUM offset,STREAM_ORIGIN origin);
	INT (*stream_getpos)(struct _stream *stream); //returns -1 if eof (eos) reached
} stream_type;

typedef struct _stream
{
	struct _stream_type *type;
	//STREAM_FLAG flags;
	char *flags;
	ptr_list *tags;
}stream;

typedef struct _streams
{
	struct _stream *output;
	struct _stream *input;
	struct _stream *debug;
}streams;


void streams_Init(void);

void streams_Close(void);

struct _stream_type *GetStreamType(STREAM_TYPE_ID id);


stream *stream_CreateFromFile(char *filename,char *flags); //will create a copy of filename
stream *stream_CreateFromBytes(char *bytes ,STREAM_NUM len); //will use bytes directly - remember to free bytes after stream closing
#ifdef USE_ARDUINO_FUNCTIONS
stream *stream_CreateFromFlashBytes(char *bytes ,STREAM_NUM len);
#endif
stream *stream_CreateStandardOutput(void);
stream *stream_CreateStandardInput(void);
stream *stream_CreateDebugOutput(void);

//stream access functions

BOOL stream_Open(struct _stream *stream);
BOOL stream_Close(struct _stream *stream);
BOOL stream_Free(struct _stream *stream);
BOOL stream_Read(struct _stream *stream,void *ptr,STREAM_NUM len);
BOOL stream_Write(struct _stream *stream,void *ptr ,STREAM_NUM len);
BOOL stream_Seek(struct _stream *stream,STREAM_NUM offset,STREAM_ORIGIN origin);
INT     stream_GetPos(struct _stream *stream);

//file stream functions

BOOL stream_file_open(struct _stream *stream);
BOOL stream_file_close(struct _stream *stream);
BOOL stream_file_free(struct _stream *stream);
BOOL stream_file_read(struct _stream *stream,void *ptr,STREAM_NUM len);
BOOL stream_file_write(struct _stream *stream,void *ptr ,STREAM_NUM len);
BOOL stream_file_seek(struct _stream *stream,STREAM_NUM offset,STREAM_ORIGIN origin);
INT     stream_file_getpos(struct _stream *stream);

//memory stream functions

//BOOL stream_memory_open(struct _stream *stream);
//BOOL stream_memory_close(struct _stream *stream);
BOOL stream_memory_free(struct _stream *stream);
BOOL stream_memory_read(struct _stream *stream,void *ptr,STREAM_NUM len);
//BOOL stream_memory_write(struct _stream *stream,char *bytes ,STREAM_NUM len);
//BOOL stream_memory_seek(struct _stream *stream,STREAM_NUM offset,STREAM_ORIGIN origin);
//INT     stream_memory_getpos(struct _stream *stream);

#ifdef USE_ARDUINO_FUNCTIONS

#include "avr/pgmspace.h"

//flash memory stream functions

//BOOL stream_flash_memory_open(struct _stream *stream);
//BOOL stream_flash_memory_close(struct _stream *stream);
BOOL stream_flash_memory_free(struct _stream *stream);
BOOL stream_flash_memory_read(struct _stream *stream,void *ptr,STREAM_NUM len);
//BOOL stream_flash_memory_write(struct _stream *stream,char *bytes ,STREAM_NUM len);
//BOOL stream_flash_memory_seek(struct _stream *stream,STREAM_NUM offset,STREAM_ORIGIN origin);
//INT     stream_flash_getpos(struct _stream *stream);

#endif

//functions used primarily for object reading and writing from/to streams

long stream_ReadLong(struct _stream *f);

FLOAT stream_ReadFloat(struct _stream *f);

char stream_ReadChar(struct _stream *f);


void stream_WriteLong(long l,struct _stream *f);

void stream_WriteFloat(FLOAT fl,struct _stream *f);

void stream_WriteChar(char c,struct _stream *f);

struct _object *stream_ReadObject(struct _stream *f);

void stream_WriteObject(struct _object *obj,struct _stream *f);

#ifndef USE_ARDUINO_FUNCTIONS
struct _object *stream_ReadObjectPlus(struct _stream *f);

void stream_WriteObjectPlus(struct _object *obj,struct _stream *f);
#endif

#ifdef __cplusplus
} 
#endif


#endif
