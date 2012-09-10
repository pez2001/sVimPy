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


LIST_ID list_Create(NUM num, unsigned char flags)
{
	#ifdef USE_MEMORY_DEBUGGING
	MEM_ID tmp = mem_malloc_debug(sizeof(id_list),MEM_POOL_CLASS_STATIC, "list_Create() return");
	#else
	MEM_ID tmp = mem_malloc(sizeof(id_list),MEM_POOL_CLASS_STATIC);
	#endif
	id_list *l = (id_list*)mem_lock(tmp);
	if(num)
	{
		#ifdef USE_MEMORY_DEBUGGING
		l->items = mem_malloc_debug(num * sizeof(OBJECT_ID),MEM_POOL_CLASS_DYNAMIC,"list_Create() items");
		#else
		l->items = mem_malloc(num * sizeof(OBJECT_ID),MEM_POOL_CLASS_DYNAMIC);
		#endif
		OBJECT_ID *i = (OBJECT_ID*)mem_lock(l->items);
		memset(i,0,num*sizeof(OBJECT_ID));
		mem_unlock(l->items,1);
	}
	else
		l->items = 0;
	l->num = num;
	l->flags = flags;
	mem_unlock(tmp,1);
	return(tmp);
}

void list_Close(LIST_ID list)
{
	id_list *l = (id_list*)mem_lock(list);
	if(l->num > 0)
		mem_free(l->items);
	mem_unlock(list,0);
	mem_free(list);
}

void list_Push(LIST_ID list, OBJECT_ID obj)
{
	id_list *l = (id_list*)mem_lock(list);
	//printf("locked list:%d\r\n",l->num);
	if(!l->num)
	{
		l->num = 1;
		//printf("allocating items\r\n");
		#ifdef USE_MEMORY_DEBUGGING
		l->items = mem_malloc_debug(l->num * sizeof(OBJECT_ID),MEM_POOL_CLASS_DYNAMIC,"list_Push() items");
		#else
		l->items = mem_malloc(l->num * sizeof(OBJECT_ID),MEM_POOL_CLASS_DYNAMIC);
		#endif
		//printf("alloced items\r\n");
		OBJECT_ID *i = (OBJECT_ID*)mem_lock(l->items);	
		//printf("locked items\r\n");
		i[0] = obj;
		mem_unlock(l->items,1);
		//printf("unlocked items\r\n");
	}
	else
	{
		l->items = mem_realloc(l->items,(l->num + 1) * sizeof(OBJECT_ID));
		OBJECT_ID *i = (OBJECT_ID*)mem_lock(l->items);	
		i[l->num] = obj;
		mem_unlock(l->items,1);
		l->num++;
	}
	mem_unlock(list,1);
}

OBJECT_ID list_Pop(LIST_ID list)
{
	id_list *l = (id_list*)mem_lock(list);
	if(l->num)
	{
		OBJECT_ID tmp = list_Remove(list, l->num - 1);
		mem_unlock(list,0);
		return (tmp);
	}
	mem_unlock(list,0);
	return (0);
}

BOOL list_Insert(LIST_ID list, INDEX index, OBJECT_ID obj)
{
	id_list *l = (id_list*)mem_lock(list);
	if(index == 0 && !l->num)
	{
		list_Push(list, obj);
	}
	else if(index <= l->num - 1)
	{
		l->num++;
		l->items = mem_realloc(l->items, (l->num) * sizeof(OBJECT_ID));
		for(NUM i = l->num - 2; i >= index; i--)
		{
			list_MoveDown(list, i);
		}
		OBJECT_ID *i = (OBJECT_ID*)mem_lock(l->items);	
		i[index] = obj;
		mem_unlock(l->items,1);
		mem_unlock(list,1);
		return (1);
	}
	else if(index > l->num - 1)
		list_Push(list, obj);
	mem_unlock(list,1);
	return(0);
}

void list_MoveUp(LIST_ID list, INDEX index)
{
	id_list *l = (id_list*)mem_lock(list);
	if(!l->num)
	{
		mem_unlock(list,0);
		return;
	}
	if(index < l->num && index > 0)
	{
		OBJECT_ID *i = (OBJECT_ID*)mem_lock(l->items);	
		i[index - 1] = i[index];
		mem_unlock(l->items,1);
	}
	mem_unlock(list,0);
}

void list_MoveDown(LIST_ID list, INDEX index)
{
	id_list *l = (id_list*)mem_lock(list);
	if(!l->num)
	{
		mem_unlock(list,0);
		return;
	}
	if(index < l->num - 1 && index >= 0)
	{
		OBJECT_ID *i = (OBJECT_ID*)mem_lock(l->items);	
		i[index + 1] = i[index];
		mem_unlock(l->items,1);
	}
	mem_unlock(list,0);
}

OBJECT_ID list_Remove(LIST_ID list, INDEX index)
{
	id_list *l = (id_list*)mem_lock(list);
	if(!l->num)
	{
		mem_unlock(list,0);
		return(0);
	}
	if(index < l->num)
	{
		OBJECT_ID *i = (OBJECT_ID*)mem_lock(l->items);	
		OBJECT_ID tmp = i[index];
		for(NUM i = l->num - 1; i > index; i--)
		{
			list_MoveUp(list, i);
		}
		if((l->num - 1) == 0)
		{
			mem_unlock(l->items,0);
			mem_free(l->items);
			l->items = 0;
			l->num = 0;
		}
		else
		{
			mem_unlock(l->items,1);
			l->items =	mem_realloc(l->items, (l->num - 1) * sizeof(OBJECT_ID));
			l->num--;
		}
		mem_unlock(list,1);
		return(tmp);
	}
	mem_unlock(list,0);
	return(0);
}

void list_Clear(LIST_ID list)
{
	id_list *l = (id_list*)mem_lock(list);
	if(l->num)
	{
		mem_free(l->items);
		l->items = 0;
	}
	l->num = 0;
	mem_unlock(list,1);
}

NUM list_GetLen(LIST_ID list)
{
	id_list *l = (id_list*)mem_lock(list);
	NUM r = l->num;
	mem_unlock(list,0);
	return(r);
}

OBJECT_ID list_Get(LIST_ID list, INDEX index)
{
	OBJECT_ID r = 0;
	id_list *l = (id_list*)mem_lock(list);
	if(index < l->num)
	{
		OBJECT_ID *i = (OBJECT_ID*)mem_lock(l->items);	
		r = i[index];
		mem_unlock(l->items,0);
	}
	mem_unlock(list,0);
	return(r);
}

void list_Set(LIST_ID list, INDEX index, OBJECT_ID obj)
{
	id_list *l = (id_list*)mem_lock(list);
	//printf("%d items in tuple\n",l->num);
	if(index < l->num)
	{
		OBJECT_ID *i = (OBJECT_ID*)mem_lock(l->items);	
		i[index] = obj;
		//printf("set %d item in tuple to :%d\n",index,obj);
		mem_unlock(l->items,1);
	}
	mem_unlock(list,0);
}

void list_Queue(LIST_ID list, OBJECT_ID obj)
{
	list_Insert(list, 0, obj);
}

OBJECT_ID list_Dequeue(LIST_ID list)
{
	/*id_list *l = (id_list*)mem_lock(list);
	if(!l->num)
	{
		mem_unlock(list,0);
		return(0);
	}
	mem_unlock(list,0);
	OBJECT_ID tmp = list_Get(list, 0);
	list_Remove(list, 0);
	return(tmp);
	*/
	return(list_Remove(list, 0));
}

BOOL list_Contains(LIST_ID list,OBJECT_ID obj)
{
	id_list *l = (id_list*)mem_lock(list);
	OBJECT_ID *ic = (OBJECT_ID*)mem_lock(l->items);	
	for(int i=0;i<l->num;i++)
	{	
		if(ic[i] == obj)
		{
			mem_unlock(l->items,0);
			mem_unlock(list,0);
			return(1);
		}
	}
	mem_unlock(l->items,0);
	mem_unlock(list,0);
	return(0);
}

INDEX list_GetIndex(LIST_ID list, OBJECT_ID obj)
{
	id_list *l = (id_list*)mem_lock(list);
	OBJECT_ID *ic = (OBJECT_ID*)mem_lock(l->items);	
	for(int i=0;i<l->num;i++)
	{	
		if(ic[i] == obj)
		{
			mem_unlock(l->items,0);
			mem_unlock(list,0);
			return(i);
		}
	}
	mem_unlock(l->items,0);
	mem_unlock(list,0);
	return(-1);
}

void list_RemoveItem(LIST_ID list, OBJECT_ID obj)
{
	INDEX index = list_GetIndex(list,obj);
	if(index != -1)
		list_Remove(list,index);
}

BOOL list_IsEmpty(LIST_ID list)
{
	id_list *l = (id_list*)mem_lock(list);
	BOOL r = !l->num;
	mem_unlock(list,0);
	return(r);
}

