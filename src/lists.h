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


#ifndef LISTS_H
#define LISTS_H
#include "features.h"

#include "types.h"
#include "debug.h"

#include "memory.h"


#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"
#include "string.h"


#ifdef __cplusplus
extern "C"  {
#endif


#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */
#endif
typedef struct _id_list
{
	MEM_ID items; /*list of OBJECT_IDs*/
	NUM num;
	unsigned char flags;
} id_list;


#ifndef USE_ARDUINO_FUNCTIONS
#pragma pack(pop)				/* restore original alignment from stack */
#endif

#define LIST_STATIC 1		// TODO add support for static lists , so indices wont change

LIST_ID list_Create(NUM num, unsigned char flags); /* returns id_list*/

void list_Close(LIST_ID list);

void list_Push(LIST_ID list, OBJECT_ID obj);

OBJECT_ID list_Pop(LIST_ID list);

BOOL list_Insert(LIST_ID list, INDEX index, OBJECT_ID obj);

OBJECT_ID list_Remove(LIST_ID list, INDEX index);

void list_RemoveItem(LIST_ID list, OBJECT_ID obj);

INDEX list_GetIndex(LIST_ID list, OBJECT_ID obj);

void list_Clear(LIST_ID list);

NUM list_GetLen(LIST_ID list);

OBJECT_ID list_Get(LIST_ID list, INDEX index);

void list_Set(LIST_ID list, INDEX index, OBJECT_ID obj);

void list_Queue(LIST_ID list, OBJECT_ID obj);

OBJECT_ID list_Dequeue(LIST_ID list);

BOOL list_IsEmpty(LIST_ID list);

BOOL list_Contains(LIST_ID list,OBJECT_ID obj);

void list_MoveUp(LIST_ID list, INDEX index);

void list_MoveDown(LIST_ID list, INDEX index);


#ifdef __cplusplus
} 
#endif


#endif
