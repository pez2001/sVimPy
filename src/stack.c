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

STACK_ID stack_Create(void)
{
	STACK_ID stack = list_Create(0,0);
	return(stack);
}

void stack_Close(STACK_ID stack, BOOL free_objects)
{
	if(stack != 0)
	{
		stack_Clear(stack,free_objects);
		list_Close(stack);
	}
}

void stack_Clear(STACK_ID stack, BOOL free_objects)
{
	if(stack != 0)
	{
		if(free_objects)
		{
			id_list *l = (id_list*)mem_lock(stack);
			while(l->num)
			{
					OBJECT_ID fo = list_Pop(stack);
					obj_DecRefCount(fo);
			}
			mem_unlock(stack,0);
		}
		#ifdef USE_DEBUGGING
		else if(list_GetLen(stack) > 0)
			debug_printf(DEBUG_STACK,"%d items left untouched on stack\n", list_GetLen(stack));
		#endif
	}
}

BOOL stack_IsEmpty(STACK_ID stack)
{
	return(list_IsEmpty(stack));
}

void stack_Push(STACK_ID stack, OBJECT_ID x)
{
	if(x == 0)
		return;
	obj_IncRefCount(x);
	list_Push(stack, x);
}

OBJECT_ID stack_Bottom(STACK_ID stack)
{
	if(list_GetLen(stack) < 1)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_Bottom() - stack underflow - no bottom\n");
		#endif
		return(0);
	}
	OBJECT_ID r = list_Get(stack,0);
	return(r);
}

OBJECT_ID stack_Get(STACK_ID stack,INDEX index)
{
	if(index < 0)
		index = list_GetLen(stack) - (-index);
	if(index >= list_GetLen(stack) || index < 0)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_Get() - stack underflow - index out of range\n");
		#endif
		return(0);
	}
	OBJECT_ID r = list_Get(stack,index);
	return(r);
}

NUM stack_Pointer(STACK_ID stack)
{
	return(list_GetLen(stack)-1);
}

OBJECT_ID stack_Top(STACK_ID stack)
{
	if(list_GetLen(stack) < 1)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_Top() - stack underflow - no top\n");
		#endif
		return(0);
	}
	OBJECT_ID r = list_Get(stack,list_GetLen(stack) - 1);
	return(r);
}

OBJECT_ID stack_Second(STACK_ID stack)
{
	if(list_GetLen(stack) < 2)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_Second() - stack underflow - no second\n");
		#endif
		return(0);
	}
	OBJECT_ID r = list_Get(stack,list_GetLen(stack) - 2);
	return(r);
}

OBJECT_ID stack_Third(STACK_ID stack)
{
	if(list_GetLen(stack) < 3)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_Third() - stack underflow - no third\n");
		#endif
		return(0);
	}
	OBJECT_ID r = list_Get(stack,list_GetLen(stack) - 3);
	return (r);
}

void stack_SetBottom(STACK_ID stack, OBJECT_ID x)
{
	if(list_GetLen(stack) < 1)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_SetBottom() - stack underflow - no bottom\n");
		#endif
		return;
	}
	list_Set(stack,0,x);
}

void stack_SetTop(STACK_ID stack, OBJECT_ID x)
{
	if(list_GetLen(stack) < 1)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_SetTop() - stack underflow - no top\n");
		#endif
		return;
	}
	list_Set(stack,list_GetLen(stack) - 1, x);
}

void stack_SetSecond(STACK_ID stack, OBJECT_ID x)
{
	if(list_GetLen(stack) < 2)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_SetSecond() - stack underflow - no second\n");
		#endif
		return;
	}
	list_Set(stack,list_GetLen(stack) - 2,x);
}

void stack_SetThird(STACK_ID stack, OBJECT_ID x)
{
	if(list_GetLen(stack) < 3)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_SetThird() - stack underflow - no third\n");
		#endif
		return;
	}
	list_Set(stack,list_GetLen(stack) - 3,x);
}

void stack_Adjust(STACK_ID stack, REL_NUM by)
{
	if(by>0)
		for(NUM i = 0; i < by; i++)
			list_Push(stack, 0);
	if(by<0)
		for(NUM i = 0; i < -by; i++)
			list_Pop(stack);
}

#ifdef USE_DEBUGGING
void stack_Dump(STACK_ID stack)
{
	if(stack == 0)
	{
		debug_printf(DEBUG_ALL,"non existant stack\n");
		return;
	}
	if(list_GetLen(stack) == 0)
	{
		debug_printf(DEBUG_ALL,"empty stack\n");
		return;
	}
	debug_printf(DEBUG_ALL,"Dumping %d stack items\n", list_GetLen(stack));
	for(NUM i = 0; i < list_GetLen(stack); i++)
	{
		debug_printf(DEBUG_ALL,"item: %d\n", i);
		obj_Dump(list_Get(stack,i), 1,1);
	}
}
#endif

OBJECT_ID stack_Pop(STACK_ID stack)/*id_list*/
{
	if(list_GetLen(stack) < 1)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_STACK,"stack_Pop() - stack underflow\n");
		#endif
		return(0);
	}
	OBJECT_ID r = list_Pop(stack);
	obj_DecRefCount(r);
	return (r);
}

BOOL stack_Contains(STACK_ID stack, OBJECT_ID x)
{
	for (NUM i = 0; i < list_GetLen(stack); i++)
		if(list_Get(stack,i) == x)
			return(1);
	return(0);
}

NUM stack_GetLen(STACK_ID stack)
{
	return(list_GetLen(stack));
}
