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

#define STREAM_TYPE_FILE 'f'
#define STREAM_TYPE_MEMORY 'm'
#define STREAM_TYPE_FLASH_MEMORY 'F'
#define STREAM_TYPE_DEBUG_OUTPUT 'd'
#define STREAM_TYPE_STANDARD_INPUT 'i'
#define STREAM_TYPE_STANDARD_OUTPUT 'o'

struct _stream;

typedef struct _stream_type
{
	STREAM_TYPE_ID type;
	BOOL (*stream_open)(struct _stream *stream);
	BOOL (*stream_close)(struct _stream *stream);
	BOOL (*stream_free)(struct _stream *stream);	
	BOOL (*stream_read)(struct _stream *stream,void *ptr ,STREAM_NUM len);
	BOOL (*stream_write)(struct _stream *stream,char *bytes,STREAM_NUM len);
	BOOL (*stream_seek)(struct _stream *stream,STREAM_NUM offset);
} stream_type;

typedef struct _stream
{
	stream_type *type;
	ptr_list *tags;
}stream;

typedef struct _streams
{
	stream *output;
	stream *input;
	stream *debug;
}streams;


void streams_Init(void);

stream_type GetStreamType(STREAM_TYPE_ID id);

void streams_Close(void);

stream *stream_CreateFromFile(char *filename); //will create a copy of filename
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
BOOL stream_Write(struct _stream *stream,char *bytes ,STREAM_NUM len);
BOOL stream_Seek(struct _stream *stream,STREAM_NUM offset);

//file stream functions

BOOL stream_file_open(struct _stream *stream);
BOOL stream_file_close(struct _stream *stream);
BOOL stream_file_free(struct _stream *stream);
BOOL stream_file_read(struct _stream *stream,void *ptr,STREAM_NUM len);
BOOL stream_file_write(struct _stream *stream,char *bytes ,STREAM_NUM len);
BOOL stream_file_seek(struct _stream *stream,STREAM_NUM offset);

//memory stream functions

BOOL stream_memory_open(struct _stream *stream);
BOOL stream_memory_close(struct _stream *stream);
BOOL stream_memory_free(struct _stream *stream);
BOOL stream_memory_read(struct _stream *stream,void *ptr,STREAM_NUM len);
BOOL stream_memory_write(struct _stream *stream,char *bytes ,STREAM_NUM len);
BOOL stream_memory_seek(struct _stream *stream,STREAM_NUM offset);

#ifdef USE_ARDUINO_FUNCTIONS

#include "avr/pgmspace.h"

//flash memory stream functions

BOOL stream_flash_memory_open(struct _stream *stream);
BOOL stream_flash_memory_close(struct _stream *stream);
BOOL stream_flash_memory_free(struct _stream *stream);
BOOL stream_flash_memory_read(struct _stream *stream,void *ptr,STREAM_NUM len);
BOOL stream_flash_memory_write(struct _stream *stream,char *bytes ,STREAM_NUM len);
BOOL stream_flash_memory_seek(struct _stream *stream,STREAM_NUM offset);

#endif



#endif
