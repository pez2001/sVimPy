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

#define STACK_ITEM_SIZE 4
//#define STACK_TYPE *void
#define STACK_TYPE *object;

#define STACK_MIN_ITEMS 100
#define CALLSTACK_MIN_ITEMS 20
#define STACK_SIZE_INCREASE 200


typedef struct 
{
object **items;
long top;
long num;
}stack;

extern stack *recycle;

stack* stack_Init(long items_num);
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

//object *callstack_Pop();
//void callstack_Push(object *x);

//object *recycle_Pop();
//void recycle_Push(object *x);


#endif
