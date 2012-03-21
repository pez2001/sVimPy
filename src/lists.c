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

#include "lists.h"


ptr_list *ptr_CreateList(NUM num, unsigned char flags)
{
	#ifdef USE_DEBUGGING
	ptr_list *tmp = (ptr_list*) mem_malloc(sizeof(ptr_list), "ptr_CreateList() return");
	#else
	ptr_list *tmp = (ptr_list*) malloc(sizeof(ptr_list));
	#endif
	if (num)
	{
		#ifdef USE_DEBUGGING
		tmp->items = (void **)mem_malloc(num * sizeof(void *),"ptr_CreateList() items");
		#else
		tmp->items = (void **)malloc(num * sizeof(void *));
		#endif
		memset(tmp->items,0,num*sizeof(void*));
	}
	else
		tmp->items = NULL;
	tmp->num = num;
	tmp->flags = flags;
	return (tmp);
}

/*
ptr_list_with_tag *ptr_CreateTaggedList(NUM num, int flags)
{
	return (NULL);
}
*/

void ptr_CloseList(ptr_list *list)
{
	if (list->num > 0 )//|| list->num != NULL)
	{
		#ifdef USE_DEBUGGING
		//assert(mem_free(list->items));
		mem_free(list->items);
		#else
		free(list->items);
		#endif
	}
	#ifdef USE_DEBUGGING
	//assert(mem_free(list));
	mem_free(list);
	#else
	free(list);
	#endif
}

void ptr_Push(ptr_list *list, void *ptr)
{
	if (!list->num)
	{
		list->num = 1;
		#ifdef USE_DEBUGGING
		list->items = (void**)mem_malloc(list->num * sizeof(void*),"ptr_Push() items");
		#else
		list->items = (void**)malloc(list->num * sizeof(void*));
		#endif
		list->items[0] = ptr;
	}
	else
	{
		#ifdef USE_DEBUGGING
		list->items = (void**)mem_realloc(list->items,(list->num + 1) * sizeof(void*));
		#else
		list->items = (void**)realloc(list->items,(list->num + 1) * sizeof(void*));
		#endif
		list->items[list->num] = ptr;
		list->num++;
	}
}

void *ptr_Pop(ptr_list *list)
{
	if (list->num)
	{
		// printf("ptr pop:%d\n",list->num-1);
		void *tmp = ptr_Remove(list, list->num - 1);
		return (tmp);
	}
	return (NULL);
}

BOOL ptr_Insert(ptr_list *list, INDEX index, void *ptr)
{
	if (index == 0 && !list->num)
	{
		ptr_Push(list, ptr);
	}
	else if (index < list->num - 1)
	{
		list->num++;
		#ifdef USE_DEBUGGING
		list->items = (void **)mem_realloc(list->items, (list->num) * sizeof(void *));
		#else
		list->items = (void **)realloc(list->items, (list->num) * sizeof(void *));
		#endif
		// int len = (list->num-1) - index;
		// if(len)
		// {
		for (NUM i = list->num - 2; i >= index; i--)
		{
			ptr_MoveDown(list, i);
			// memcpy(&list->items[index+1],&list->items[index],len *
			// sizeof(void*));
		}
		list->items[index] = ptr;
		return (1);
	}
	else if (index >= list->num - 1)
		ptr_Push(list, ptr);

	return (0);
}

void ptr_MoveUp(ptr_list *list, INDEX index)
{
	if (!list->num)
		return;
	if (index < list->num && index > 0)
	{
		list->items[index - 1] = list->items[index];
	}
}

void ptr_MoveDown(ptr_list *list, INDEX index)
{
	if (!list->num)
		return;
	if (index < list->num - 1 && index >= 0)
	{
		list->items[index + 1] = list->items[index];
	}
}

void *ptr_Remove(ptr_list *list, INDEX index)
{
	if (!list->num)
		return (NULL);
	if (index < list->num)
	{
		void *tmp = list->items[index];

		// int len = (list->num-1) - index;
		// if(len)
		// {
		for (NUM i = list->num - 1; i > index; i--)
		{
			// printf("move up\n");
			ptr_MoveUp(list, i);
			// memcpy(&list->items[index],&list->items[index+1],len *
			// sizeof(void*));
		}
		if ((list->num - 1 )== 0)
		{
			//printf("freeing empty list\n");
			#ifdef USE_DEBUGGING
			//assert(mem_free(list->items));
			mem_free(list->items);
			#else
			free(list->items);
			#endif
			list->items = NULL;
			list->num = 0;
		}
		else
		{
			#ifdef USE_DEBUGGING
			list->items =	(void**)mem_realloc(list->items, (list->num - 1) * sizeof(void *));
			#else
			list->items =	(void**)realloc(list->items, (list->num - 1) * sizeof(void *));
			#endif
			list->num--;
		}
		// printf("top:%d\n",list->num);
		return (tmp);
	}
	return (NULL);
}

void ptr_Clear(ptr_list *list)
{
	if (list->num)
	{
		#ifdef USE_DEBUGGING
		//assert(mem_free(list->items));
		mem_free(list->items);
		#else
		free(list->items);
		#endif
		list->items = NULL;
	}
	list->num = 0;
}

NUM ptr_GetNum(ptr_list *list)
{
	return (list->num);
}

void *ptr_Get(ptr_list *list, INDEX index)
{
	if (index < list->num)
		return (list->items[index]);
	else
		return (NULL);
}

void ptr_Set(ptr_list *list, INDEX index, void *ptr)
{
	if (index < list->num)
		list->items[index] = ptr;
}

void ptr_Queue(ptr_list *list, void *ptr)
{
	ptr_Insert(list, 0, ptr);
}

void *ptr_Dequeue(ptr_list *list)
{
	if (!list->num)
		return (NULL);
	void *tmp = ptr_Get(list, 0);

	ptr_Remove(list, 0);
	return (tmp);
}

BOOL ptr_Contains(ptr_list *list,void *ptr)
{
	for(int i=0;i<list->num;i++)
	{	
		if(list->items[i] == ptr)
			return(1);
	}
	return(0);
}

INDEX ptr_GetIndex(ptr_list *list, void *ptr)
{
	for(int i=0;i<list->num;i++)
	{	
		if(list->items[i] == ptr)
			return(i);
	}
	return(-1);
}

void ptr_RemoveItem(ptr_list *list, void *ptr)
{
	INDEX index = ptr_GetIndex(list,ptr);
	if(index != -1)
		ptr_Remove(list,index);
}

BOOL ptr_IsEmpty(ptr_list * list)
{
	return (!list->num);
}

