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

#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"
#include "string.h"

#ifdef DEBUGGING
#include "memory.h"
#include "assert.h"
#endif

#pragma pack(push)				/* push current alignment to stack */
#pragma pack(1)					/* set alignment to 1 byte boundary */

typedef struct
{
	void **items;
	NUM num;
	unsigned char flags;
} ptr_list;

/*
typedef struct
{
	void **items;
	NUM num;
	unsigned char flags;
	void *tag;
} ptr_list_with_tag;
*/
#pragma pack(pop)				/* restore original alignment from stack */

#define PTR_STATIC_LIST 1		// TODO add support for static lists , so indices wont change
#define PTR_LIST_HAS_TAG 2

ptr_list *ptr_CreateList(NUM num, unsigned char flags);

//ptr_list_with_tag *ptr_CreateTaggedList(NUM num, int flags);

void ptr_CloseList(ptr_list * list);

void ptr_Push(ptr_list * list, void *ptr);

void *ptr_Pop(ptr_list * list);

BOOL ptr_Insert(ptr_list * list, INDEX index, void *ptr);

void *ptr_Remove(ptr_list * list, INDEX index);

void ptr_RemoveItem(ptr_list * list, void *ptr);

INDEX ptr_GetIndex(ptr_list *list, void *ptr);

void ptr_Clear(ptr_list * list);

NUM ptr_GetNum(ptr_list * list);

void *ptr_Get(ptr_list * list, INDEX index);

void ptr_Set(ptr_list * list, INDEX index, void *ptr);

void ptr_Queue(ptr_list * list, void *ptr);

void *ptr_Dequeue(ptr_list * list);

BOOL ptr_IsEmpty(ptr_list * list);

BOOL ptr_Contains(ptr_list *list,void *ptr);

void ptr_MoveUp(ptr_list * list, INDEX index);

void ptr_MoveDown(ptr_list * list, INDEX index);

#endif
