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

stack *stack_Init(stack * recycle)
{
	stack *tmp = (stack *) mem_malloc(sizeof(stack), "stack_Init() return");
	tmp->list = ptr_CreateList(0, 0);
	tmp->recycle_stack = recycle;
	return (tmp);
}

/* 
   void stack_IncreaseSize (int items_num, stack * stack) { if (stack == NULL)
   return; stack->items = (object **) mem_realloc (stack->items, stack->num +
   items_num * sizeof (object *)); stack->num += items_num; printf ("increasing 
   stack size by:%d\n", items_num); } */

void stack_Close(stack * stack, int free_objects)
{
	if (stack != NULL)
	{
		if (free_objects)
		{
			for (int i = 0; i < stack->list->num; i++)
				if ((((object *) stack->list->items[i])->
					 flags & OFLAG_ON_STACK) > 0)
				{
					FreeObject(stack->list->items[i]);
				}
		}
		else if (stack->list->num > 0)
			printf("%d items left untouched on stack\n", stack->list->num);
		ptr_CloseList(stack->list);
		assert(mem_free(stack));
	}
}



void stack_Push(stack * stack, object * x)
{
	if (x == NULL)
		return;
	ptr_Push(stack->list, x);
}

object *stack_Bottom(stack * stack)
{
	if (stack->list->num < 1)
	{
		printf("stack_Bottom() - stack underflow - no top\n");
		return (NULL);
	}
	object *r = stack->list->items[0];

	return (r);
}

object *stack_Top(stack * stack)
{
	if (stack->list->num < 1)
	{
		printf("stack_Top() - stack underflow - no top\n");
		return (NULL);
	}
	object *r = stack->list->items[stack->list->num - 1];

	return (r);
}

object *stack_Second(stack * stack)
{
	if (stack->list->num < 2)
	{
		printf("stack_Second() - stack underflow - no top\n");
		return (NULL);
	}
	object *r = stack->list->items[stack->list->num - 2];

	return (r);
}

object *stack_Third(stack * stack)
{
	if (stack->list->num < 3)
	{
		printf("stack_Third() - stack underflow - no top\n");
		return (NULL);
	}
	object *r = stack->list->items[stack->list->num - 3];

	return (r);
}

void stack_SetBottom(stack * stack, object * x)
{
	if (stack->list->num < 1)
	{
		printf("stack_SetBottom() - stack underflow - no top\n");
		return (NULL);
	}
	stack->list->items[0] = x;
}

void stack_SetTop(stack * stack, object * x)
{
	if (stack->list->num < 1)
	{
		printf("stack_SetTop() - stack underflow - no top\n");
		return (NULL);
	}
	stack->list->items[stack->list->num - 1] = x;
}

void stack_SetSecond(stack * stack, object * x)
{
	if (stack->list->num < 2)
	{
		printf("stack_SetSecond() - stack underflow - no top\n");
		return (NULL);
	}
	stack->list->items[stack->list->num - 2] = x;

}

void stack_SetThird(stack * stack, object * x)
{
	if (stack->list->num < 3)
	{
		printf("stack_SetThird() - stack underflow - no top\n");
		return (NULL);
	}
	stack->list->items[stack->list->num - 3] = x;

}

void stack_Adjust(stack * stack, int by)
{
	// if ((stack->top + by) > stack->num)
	// stack_IncreaseSize (((stack->top + by) - stack->num), stack);

	// stack->top += by;
	for (int i = 0; i < by; i++)
		ptr_Push(stack->list, NULL);
}


void stack_Dump(stack * stack)
{
	printf("Dumping %d stack items\n", stack->list->num);
	for (int i = 0; i < stack->list->num; i++)
	{
		printf("item: %d\n", i);
		DumpObject(stack->list->items[i], 1);

	}
}


object *stack_Pop(stack * _stack)
{
	if (_stack->list->num < 1)
	{
		printf("stack_Pop() - stack underflow\n");
		return (NULL);
	}
	object *r = ptr_Pop(_stack->list);

	if ((r->flags & OFLAG_ON_STACK))
	{
		// printf("removed stack only item\n");
		if (_stack->recycle_stack != NULL)
		{
			if (!stack_Contains((stack *) _stack->recycle_stack, r))
			{
			//	 printf("put object on recycle stack @:%d\n",((stack*)_stack->recycle_stack)->list->num);
				stack_Push((stack *) _stack->recycle_stack, r);
			}
		}
	}
	return (r);
}

int stack_Contains(stack * stack, object * x)
{
	for (int i = 0; i < stack->list->num; i++)
		if (stack->list->items[i] == x)
			return (1);
	return (0);
}

/* 
   void recycle_Remove(object *x) { for(int i =0;i<recycle->top;i++) { if(x == 
   recycle->items[i]) printf("found object @%x\n",x);

   } } */
