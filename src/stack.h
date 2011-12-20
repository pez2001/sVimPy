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
#include "object.h"

#define STACK_TYPE *object;



typedef struct 
{
object **items;
long top;
long num;
void *recycle_stack;
}stack;

extern stack *recycle;

stack* stack_Init(long items_num,stack *recycle);
object *stack_Pop(stack *stack);
void stack_Push(object *x,stack *stack);
void stack_Close(stack *stack,int free_objects);
int stack_Contains(object *x,stack *stack);
object *stack_Top(stack *stack);
void stack_SetTop(object *x,stack *stack);

object *stack_Second(stack *stack);
void stack_SetSecond(object *x,stack *stack);
object *stack_Third(stack *stack);
void stack_SetThird(object *x,stack *stack);

void stack_Adjust(int by,stack *stack);
void stack_IncreaseSize(int items_num,stack *stack);
void stack_Dump(stack *stack);

#endif
