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

 
ptr_list *ptr_CreateList(unsigned int num)
{
ptr_list *tmp = (ptr_list*)mem_malloc(sizeof(ptr_list),"ptr_CreateList() return");
 if(num)
	tmp->items = (void**)mem_malloc(num*sizeof(void*),"ptr_CreateList() items");
 else
	tmp->items = NULL;
tmp->num = num;
return(tmp);
}
void ptr_CloseList(ptr_list *list)
{
if(list->num)
{
	assert(mem_free(list->items));
}
assert(mem_free(list));
}
void ptr_Push(ptr_list *list,void *ptr)
{
if(!list->num)
{
	list->num = 1;
	list->items = (void**)mem_malloc(list->num*sizeof(void*),"ptr_Push() items");
	list->items[0] = ptr;
}
else
{
	list->items = (void**)mem_realloc(list->items,(list->num+1)*sizeof(void*));
	list->items[list->num] = ptr;
	list->num++;
}
}
void *ptr_Pop(ptr_list *list)
{
if(list->num)
{
	void *tmp = list->items[list->num-1];
	ptr_Remove(list,list->num-1);
	return(tmp);
}
return(NULL);
}
int ptr_Insert(ptr_list *list,int index,void *ptr)
{
if(index < list->num)
{
	list->items = (void**)mem_realloc(list->items,(list->num+1)*sizeof(void*));
	int len = (list->num-1) - index;
	if(len)
	  memcpy(list->items+ (index+1) * sizeof(void*),list->items + index * sizeof(void*),len * sizeof(void*));
	list->items[index] = ptr;
	list->num++;
 return(1);
}
return(0);
}
void *ptr_Remove(ptr_list *list,int index)
{
 if(!list->num)
  return(NULL);
 if(index < list->num)
 {
	void *tmp = list->items[index];
	int len = (list->num-1) - index;
	if(len)
	  memcpy(list->items+ index * sizeof(void*),list->items + (index+1) * sizeof(void*),len * sizeof(void*));
	list->items = (void**)mem_realloc(list->items,(list->num-1)*sizeof(void*));
	list->num--;
	return(tmp);
 }
return(NULL);
}
int ptr_Clear(ptr_list *list)
{
if(list->num)
{
	assert(mem_free(list->items));
}
list->num = 0;
}
int ptr_GetNum(ptr_list *list)
{
return(list->num);
}
void *ptr_Get(ptr_list *list,int index)
{
if(index < list->num)
 return(list->items[index]);
else
 return(NULL);
}
void ptr_Set(ptr_list *list,int index,void *ptr)
{
if(index < list->num)
 list->items[index] = ptr;
} 
void ptr_Queue(ptr_list *list,void *ptr)
{
ptr_Insert(list,0,ptr);
}
void *ptr_Dequeue(ptr_list *list)
{
if(!list->num)
 return(NULL);
void *tmp = ptr_Get(list,0);
ptr_Remove(list,0);
return(tmp);
}
int ptr_IsEmpty(ptr_list *list)
{
 return(!list->num);
}

