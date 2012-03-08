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


#define STACK_TYPE *object;

//TODO remove redundant stack struct
// #define stack ptr_list

typedef struct _stack
{
	ptr_list *list;
} stack;

struct _object;

stack *stack_Init(void); //TODO rename to Create

struct _object *stack_Pop(stack *stack);

void stack_Push(stack *stack, struct _object * x);

void stack_Close(stack *stack, BOOL free_objects);

void stack_Clear(stack *stack, BOOL free_objects);

struct _object *stack_Get(stack *stack,INDEX index);

NUM stack_Pointer(stack *stack);

struct _object *stack_Top(stack *stack);

struct _object *stack_Bottom(stack *stack);

void stack_SetBottom(stack *stack, struct _object * x);

void stack_SetTop(stack *stack, struct _object * x);

struct _object *stack_Second(stack *stack);

void stack_SetSecond(stack *stack, struct _object * x);

struct _object *stack_Third(stack *stack);

void stack_SetThird(stack *stack, struct _object * x);

void stack_Adjust(stack *stack, REL_NUM by);

#ifdef DEBUGGING
void stack_Dump(stack *stack);
#endif

BOOL stack_IsEmpty(stack *stack);

BOOL stack_Contains(stack *stack, struct _object * x);

#ifdef __cplusplus
} 
#endif


#endif
