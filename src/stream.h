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
#include "memory.h"
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
	BOOL (*stream_open)(STREAM_ID stream);/* struct _stream* stream*/
	BOOL (*stream_close)(STREAM_ID stream);
	BOOL (*stream_free)(STREAM_ID stream);	
	BOOL (*stream_read)(STREAM_ID stream,MEM_ID ptr ,STREAM_NUM ptr_offset,STREAM_NUM len);
	BOOL (*stream_write)(STREAM_ID stream,MEM_ID ptr,STREAM_NUM ptr_offset,STREAM_NUM len);
	BOOL (*stream_seek)(STREAM_ID stream,STREAM_NUM offset,STREAM_ORIGIN origin);
	INT (*stream_getpos)(STREAM_ID stream); //returns -1 if eof (eos) reached
} stream_type;

typedef struct _stream
{
	MEM_ID type;/*struct _stream_type*/
	//STREAM_FLAG flags;
	MEM_ID flags;/*char **/
	MEM_ID tag;/*id_list*/
}stream;

typedef struct _streams
{
	MEM_ID output;/*struct _stream **/
	MEM_ID input;/*struct _stream **/
	MEM_ID debug;/*struct _stream **/
}streams;

typedef struct _file_stream_tag
{
	FILE *file;
	STRING_ID filename;
	STREAM_NUM offset;
}file_stream_tag;

typedef struct _byte_stream_tag
{
	STRING_ID bytes;
	STREAM_NUM len;
	STREAM_NUM offset;
}byte_stream_tag;


void streams_Init(void);

void streams_Close(void);

MEM_ID stream_GetType(STREAM_TYPE_ID id);/*returns struct _stream_type **/


STREAM_ID stream_CreateFromFile(BYTES_ID filename,BYTES_ID flags);//(char *filename,char *flags); //will create a copy of filename returns stream*
STREAM_ID stream_CreateFromBytes(BYTES_ID bytes,STREAM_NUM len);//(char *bytes ,STREAM_NUM len); //will use bytes directly - remember to free bytes after stream closing
/*#ifdef USE_ARDUINO_FUNCTIONS
MEM_ID stream_CreateFromFlashBytes(MEM_ID bytes,STREAM_NUM len);//(char *bytes ,STREAM_NUM len);
#endif*/
STREAM_ID stream_CreateStandardOutput(void);
STREAM_ID stream_CreateStandardInput(void);
STREAM_ID stream_CreateDebugOutput(void);

//stream access functions

BOOL stream_Open(STREAM_ID stream);
BOOL stream_Close(STREAM_ID stream);
BOOL stream_Free(STREAM_ID stream);
BOOL stream_Read(STREAM_ID stream,MEM_ID ptr,STREAM_NUM ptr_offset,STREAM_NUM len);
BOOL stream_Write(STREAM_ID stream,MEM_ID ptr ,STREAM_NUM ptr_offset,STREAM_NUM len);
BOOL stream_Seek(STREAM_ID stream,STREAM_NUM offset,STREAM_ORIGIN origin);
INT      stream_GetPos(STREAM_ID stream);

//file stream functions

BOOL stream_file_open(STREAM_ID stream);
BOOL stream_file_close(STREAM_ID stream);
BOOL stream_file_free(STREAM_ID stream);
BOOL stream_file_read(STREAM_ID stream,MEM_ID ptr,STREAM_NUM ptr_offset,STREAM_NUM len);
BOOL stream_file_write(STREAM_ID stream,MEM_ID ptr ,STREAM_NUM ptr_offset,STREAM_NUM len);
BOOL stream_file_seek(STREAM_ID stream,STREAM_NUM offset,STREAM_ORIGIN origin);
INT      stream_file_getpos(STREAM_ID stream);

//memory stream functions

//BOOL stream_memory_open(STREAM_ID stream);
//BOOL stream_memory_close(STREAM_ID stream);
BOOL stream_memory_free(STREAM_ID stream);
BOOL stream_memory_read(STREAM_ID stream,MEM_ID ptr,STREAM_NUM ptr_offset,STREAM_NUM len);
//BOOL stream_memory_write(STREAM_ID stream,char *bytes ,STREAM_NUM len);
//BOOL stream_memory_seek(STREAM_ID stream,STREAM_NUM offset,STREAM_ORIGIN origin);
//INT     stream_memory_getpos(STREAM_ID stream);

/*#ifdef USE_ARDUINO_FUNCTIONS

#include "avr/pgmspace.h"

//flash memory stream functions

//BOOL stream_flash_memory_open(STREAM_ID stream);
//BOOL stream_flash_memory_close(STREAM_ID stream);
BOOL stream_flash_memory_free(STREAM_ID stream);
BOOL stream_flash_memory_read(STREAM_ID stream,MEM_ID ptr,STREAM_NUM len);
//BOOL stream_flash_memory_write(STREAM_ID stream,char *bytes ,STREAM_NUM len);
//BOOL stream_flash_memory_seek(STREAM_ID stream,STREAM_NUM offset,STREAM_ORIGIN origin);
//INT     stream_flash_getpos(STREAM_ID stream);

#endif
*/
//functions used primarily for object reading and writing from/to streams

long stream_ReadLong(STREAM_ID stream);

FLOAT stream_ReadFloat(STREAM_ID stream);

char stream_ReadChar(STREAM_ID stream);

void stream_WriteLong(long l,STREAM_ID stream);//TODO correct parameter order

void stream_WriteFloat(FLOAT fl,STREAM_ID stream);

void stream_WriteChar(char c,STREAM_ID stream);

OBJECT_ID stream_ReadObject(STREAM_ID stream);

void stream_WriteObject(OBJECT_ID obj_id,STREAM_ID stream);

#ifndef USE_ARDUINO_FUNCTIONS
OBJECT_ID stream_ReadObjectPlus(STREAM_ID stream);

void stream_WriteObjectPlus(OBJECT_ID obj_id,STREAM_ID stream);
#endif

#ifdef __cplusplus
} 
#endif


#endif
