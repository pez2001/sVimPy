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

#include "memory.h"
#include "assert.h"
#include "debug.h"
extern int debug_level;


typedef struct
{
	void **items;
	unsigned int num;
	int flags;
} ptr_list;
typedef struct
{
	void **items;
	unsigned int num;
	int flags;
	void *tag;
} ptr_list_with_tag;

#define PTR_STATIC_LIST 1		// TODO add support for static lists , so indices wont change
#define PTR_LIST_HAS_TAG 2

ptr_list *ptr_CreateList(unsigned int num, int flags);

ptr_list_with_tag *ptr_CreateTaggedList(unsigned int num, int flags);

void ptr_CloseList(ptr_list * list);

void ptr_Push(ptr_list * list, void *ptr);

void *ptr_Pop(ptr_list * list);

int ptr_Insert(ptr_list * list, int index, void *ptr);

void *ptr_Remove(ptr_list * list, int index);

int ptr_Clear(ptr_list * list);

int ptr_GetNum(ptr_list * list);

void *ptr_Get(ptr_list * list, int index);

void ptr_Set(ptr_list * list, int index, void *ptr);

void ptr_Queue(ptr_list * list, void *ptr);

void *ptr_Dequeue(ptr_list * list);

int ptr_IsEmpty(ptr_list * list);

int ptr_Contains(ptr_list *list,void *ptr);

#endif
