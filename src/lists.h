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



void **ptr_CreateList(unsigned int num);
void ptr_CloseList(void **list);
void ptr_Push(void **list,void *ptr);
void *ptr_Pop(void **list);
int ptr_Insert(void **list,int index,void *ptr);
int ptr_Remove(void **list,int index);
int ptr_Clear(void **list);
int ptr_GetNum(void **list);
void *ptr_Get(void **list,int index);
void ptr_Set(void **list,int index,void *ptr);


#endif
