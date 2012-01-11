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

#include "stack.h"

stack *stack_Init()
{
	stack *tmp = (stack *) mem_malloc(sizeof(stack), "stack_Init() return");
	tmp->list = ptr_CreateList(0, 0);
	return (tmp);
}

void stack_Close(stack *stack, int free_objects)
{
	if (stack != NULL)
	{
		stack_Clear(stack,free_objects);
		ptr_CloseList(stack->list);
		assert(mem_free(stack));
	}
}

void stack_Clear(stack *stack, int free_objects)
{
	if (stack != NULL)
	{
		if (free_objects)
		{
			long num = stack->list->num;
			//printf("before stack list num:%d\n",stack->list->num);
			for (int i = 0; i < num; i++)
			{
					FreeObject(ptr_Pop(stack->list));
					//printf("freed %d object\n",i);
			}
			//printf("stack list num:%d\n",stack->list->num);
		}
		else if (stack->list->num > 0 && (debug_level & DEBUG_STACK) > 0)
			printf("%d items left untouched on stack\n", stack->list->num);
	}
}

int stack_IsEmpty(stack *stack)
{
	return(ptr_IsEmpty(stack->list));
}

void stack_Push(stack *stack, object * x)
{
	if (x == NULL)
		return;
	IncRefCount(x);
	ptr_Push(stack->list, x);
}

object *stack_Bottom(stack *stack)
{
	if (stack->list->num < 1)
	{
		if((debug_level & DEBUG_STACK) > 0)
			printf("stack_Bottom() - stack underflow - no top\n");
		return (NULL);
	}
	object *r = stack->list->items[0];

	return (r);
}

object *stack_Top(stack *stack)
{
	if (stack->list->num < 1)
	{
		if((debug_level & DEBUG_STACK) > 0)
			printf("stack_Top() - stack underflow - no top\n");
		return (NULL);
	}
	object *r = stack->list->items[stack->list->num - 1];

	return (r);
}

object *stack_Second(stack *stack)
{
	if (stack->list->num < 2)
	{
		if((debug_level & DEBUG_STACK) > 0)
			printf("stack_Second() - stack underflow - no top\n");
		return (NULL);
	}
	object *r = stack->list->items[stack->list->num - 2];
	return (r);
}

object *stack_Third(stack *stack)
{
	if (stack->list->num < 3)
	{
		if((debug_level & DEBUG_STACK) > 0)
			printf("stack_Third() - stack underflow - no top\n");
		return (NULL);
	}
	object *r = stack->list->items[stack->list->num - 3];

	return (r);
}

void stack_SetBottom(stack *stack, object * x)
{
	if (stack->list->num < 1)
	{
		if((debug_level & DEBUG_STACK) > 0)
			printf("stack_SetBottom() - stack underflow - no top\n");
		return (NULL);
	}
	stack->list->items[0] = x;
	//IncRefCount(x);
}

void stack_SetTop(stack *stack, object * x)
{
	if (stack->list->num < 1)
	{
		if((debug_level & DEBUG_STACK) > 0)
			printf("stack_SetTop() - stack underflow - no top\n");
		return (NULL);
	}
	stack->list->items[stack->list->num - 1] = x;
	//IncRefCount(x);
}

void stack_SetSecond(stack *stack, object * x)
{
	if (stack->list->num < 2)
	{
		if((debug_level & DEBUG_STACK) > 0)
			printf("stack_SetSecond() - stack underflow - no top\n");
		return (NULL);
	}
	stack->list->items[stack->list->num - 2] = x;
	//IncRefCount(x);
}

void stack_SetThird(stack *stack, object * x)
{
	if (stack->list->num < 3)
	{
		if((debug_level & DEBUG_STACK) > 0)
			printf("stack_SetThird() - stack underflow - no top\n");
		return (NULL);
	}
	stack->list->items[stack->list->num - 3] = x;
	//IncRefCount(x);
}

void stack_Adjust(stack *stack, int by)
{
	if(by>0)
	for (int i = 0; i < by; i++)
		ptr_Push(stack->list, NULL);
	if(by<0)
	for (int i = 0; i < -by; i++)
		ptr_Pop(stack->list);
}

void stack_Dump(stack *stack)
{
	if(stack->list == NULL)
	{
		printf("empty stack\n");
		return;
	}
	printf("Dumping %d stack items\n", stack->list->num);
	for (int i = 0; i < stack->list->num; i++)
	{
		printf("item: %d\n", i);
		DumpObject(stack->list->items[i], 1);

	}
}

object *stack_Pop(stack *stack,ptr_list *gc)
{
	if (stack->list->num < 1)
	{
		if((debug_level & DEBUG_STACK) > 0)
			printf("stack_Pop() - stack underflow\n");
		return (NULL);
	}
	object *r = ptr_Pop(stack->list);
	DecRefCountGC(r,gc);
	return (r);
}

int stack_Contains(stack *stack, object * x)
{
	for (int i = 0; i < stack->list->num; i++)
		if (stack->list->items[i] == x)
			return (1);
	return (0);
}

