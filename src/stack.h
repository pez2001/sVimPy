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

#ifndef STACK_H
#define STACK_H

#include "types.h"
#include "lists.h"
#include "object.h"
#include "garbage.h"

#include "debug.h"

#ifdef __cplusplus
extern "C"  {
#endif


#define STACK_TYPE OBJECT_ID

STACK_ID stack_Create(void);/*returns id_list*/

void stack_Close(STACK_ID stack, BOOL free_objects);

OBJECT_ID stack_Pop(STACK_ID stack);/*id_list*/

void stack_Push(STACK_ID stack, OBJECT_ID x);

void stack_Clear(STACK_ID stack, BOOL free_objects);

OBJECT_ID stack_Get(STACK_ID stack,INDEX index);

NUM stack_Pointer(STACK_ID stack);

OBJECT_ID stack_Top(STACK_ID stack);

OBJECT_ID stack_Bottom(STACK_ID stack);

void stack_SetBottom(STACK_ID stack, OBJECT_ID x);

void stack_SetTop(STACK_ID stack, OBJECT_ID x);

OBJECT_ID stack_Second(STACK_ID stack);

void stack_SetSecond(STACK_ID stack, OBJECT_ID x);

OBJECT_ID stack_Third(STACK_ID stack);

void stack_SetThird(STACK_ID stack, OBJECT_ID x);

void stack_Adjust(STACK_ID stack, REL_NUM by);

#ifdef USE_DEBUGGING
void stack_Dump(STACK_ID stack);
#endif

BOOL stack_IsEmpty(STACK_ID stack);

BOOL stack_Contains(STACK_ID stack, OBJECT_ID x);

NUM stack_GetLen(STACK_ID stack);

#ifdef __cplusplus
} 
#endif


#endif
