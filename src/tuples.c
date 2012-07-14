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

#include "tuples.h"


void tuple_AppendDictItem(TUPLE_ID tuple,OBJECT_ID key,OBJECT_ID value)
{
	if(tuple == 0 || key == 0)
		return;
	tuple_object *tp = (tuple_object*)mem_lock(tuple);
	if(tp->type != TYPE_TUPLE)
	{
		mem_unlock(tuple,0);
		return;
	}
	if(tp->list == 0)
		tp->list = list_Create(0,0);
	//object *di = CopyObject(key);//TODO needed??
	//kv_object *kv = ConvertToKVObjectValued(di,value);
	OBJECT_ID kv = obj_ConvertToValuedKV(key,value);
	list_Push(tp->list,kv);
	mem_unlock(tuple,1);
	obj_IncRefCount(kv);
}

void tuple_AppendItem(TUPLE_ID tuple,OBJECT_ID value)
{
	if(tuple == 0)
		return;
	tuple_object *tp = (tuple_object*)mem_lock(tuple);
	if(tp->type != TYPE_TUPLE)
	{
		mem_unlock(tuple,0);
		return;
	}
	if(tp->list == 0)
		tp->list = list_Create(0,0);
	list_Push(tp->list,value);
	mem_unlock(tuple,1);
	obj_IncRefCount(value);
}

void tuple_InsertItem(TUPLE_ID tuple,INDEX index,OBJECT_ID value)
{
	if(tuple == 0)
		return;
	tuple_object *tp = (tuple_object*)mem_lock(tuple);
	if(tp->type != TYPE_TUPLE)
	{
		mem_unlock(tuple,0);
		return;
	}
	if(tp->list == 0)
		tp->list = list_Create(0,0);
	mem_unlock(tuple,1);
	list_Insert(tp->list,index,value);
	obj_IncRefCount(value);
}

BOOL tuple_IsTuple(TUPLE_ID tuple)
{
	tuple_object *tp = (tuple_object*)mem_lock(tuple);
	if(tp->type != TYPE_TUPLE)
	{
		mem_unlock(tuple,0);
		return(0);
	}
	mem_unlock(tuple,0);
	return(1);
}

void tuple_SetDictItem(TUPLE_ID tuple,OBJECT_ID key,OBJECT_ID value)
{
	if(tuple == 0 || key == 0 || !tuple_IsTuple(tuple))
		return;
	INDEX index = tuple_GetDictItemIndex(tuple,key);	
	if(index != -1)
	{
		OBJECT_ID k_id = tuple_GetItem(tuple,index);
		object *k = (object*)mem_lock(k_id);
		if(k->type == TYPE_KV)
		{
			OBJECT_ID old =((kv_object*)k)->value;
			if(value != 0)
			{
				((kv_object*)k)->value = value;
				obj_IncRefCount(value);
			}
			else
				((kv_object*)k)->value = 0;
			if(old != 0)
				obj_DecRefCount(old);
		}
		else 
		{
			OBJECT_ID kv = obj_ConvertToValuedKV(k_id,value);
			tuple_SetItem(tuple,index,kv);
		}
		mem_unlock(k_id,1);
	}
	else
	{
		tuple_AppendDictItem(tuple,key,value);
	}
}

void tuple_SetDictItemByIndex(TUPLE_ID tuple,INDEX index,OBJECT_ID value)
{
	if(tuple == 0 || !tuple_IsTuple(tuple))
		return;
	if(index != -1)
	{
		OBJECT_ID k_id = tuple_GetItem(tuple,index);
		object *k = (object*)mem_lock(k_id);
		if(k->type == TYPE_KV)
		{
			OBJECT_ID old = ((kv_object*)k)->value;
			if(value != 0)
			{
				((kv_object*)k)->value = value;
				obj_IncRefCount(value);
			}
			else
				((kv_object*)k)->value = 0;
			if(old != 0)
				obj_DecRefCount(old);
		}
		else 
		{
			OBJECT_ID kv = obj_ConvertToValuedKV(k_id,value);
			tuple_SetItem(tuple,index,kv);
		}
		mem_unlock(k_id,1);
	}
}

void tuple_ClearDictValues(TUPLE_ID tuple)
{
	if(tuple == 0 || !tuple_IsTuple(tuple))
		return;
	for(INDEX i = 0; i < tuple_GetLen(tuple); i++)
	{
		OBJECT_ID it_id = tuple_GetItem(tuple,i);
		if(obj_GetType(it_id) != TYPE_KV)
		{
			continue;
		}
		else
		{
			kv_object *kv = (kv_object*)mem_lock(it_id);
			tuple_SetItem(tuple,i,kv->key);
			mem_unlock(it_id,0);
			obj_DecRefCount(it_id);
		}
	}
}

OBJECT_ID tuple_GetDictItemByIndex(TUPLE_ID tuple,INDEX index)
{
	if(tuple == 0 || !tuple_IsTuple(tuple))
		return(0);
	if(index >= tuple_GetLen(tuple) || index < 0)
		return(0);
	OBJECT_ID kv_id = tuple_GetItem(tuple,index);
	object *kv = (object*)mem_lock(kv_id);
	if(kv_id == 0 || kv->type != TYPE_KV)
	{
		mem_unlock(kv_id,0);
		return(0);
	}
	OBJECT_ID value = ((kv_object*)kv)->value;
	mem_unlock(kv_id,0);
	return(value);
}

NUM tuple_GetLen(TUPLE_ID tuple)
{
	if(tuple == 0)
		return(0);
	tuple_object *tp = (tuple_object*)mem_lock(tuple); 
	if(tp->type != TYPE_TUPLE)
	{
		mem_unlock(tuple,0);
		return(0);
	}

	NUM len = 0;
	if(tp->list!= 0)
		len=list_GetLen(tp->list);
	mem_unlock(tuple,0);
	return(len);
}

OBJECT_ID tuple_GetDictItem(TUPLE_ID tuple,OBJECT_ID key)
{
	if(tuple == 0 || tuple_GetLen(tuple) == 0)
		return(0);
	INDEX index = tuple_GetDictItemIndex(tuple,key);
	if(index != -1)
	{
		OBJECT_ID kv_id = tuple_GetItem(tuple,index);
		object *kv = (object*)mem_lock(kv_id);
		if(kv_id == 0 || kv->type != TYPE_KV)
		{
			mem_unlock(kv_id,0);
			return(0);
		}
		OBJECT_ID value = ((kv_object*)kv)->value;
		mem_unlock(kv_id,0);
		return(value);
	}
	return(0);
}

INDEX tuple_GetDictItemIndex(TUPLE_ID tuple,OBJECT_ID key)
{
	if(tuple == 0 || key == 0 || tuple_GetLen(tuple) == 0)
		return(-1);
	for(INDEX i = 0; i < tuple_GetLen(tuple); i++)
	{
		OBJECT_ID it_id = tuple_GetItem(tuple,i);
		object *it = (object*)mem_lock(it_id);
		if(it_id != 0 && it->type != TYPE_KV)
		{
			if(!obj_Compare(it_id,key))
			{
				mem_unlock(it_id,0);
				return(i);
			}
		}
		else if(it_id != 0)
		{
			if(!obj_Compare(((kv_object*)it)->key,key))
			{
				mem_unlock(it_id,0);
				return (i);
			}
		}
		mem_unlock(it_id,0);
	}
	return (-1);
}

INDEX tuple_GetItemIndex(TUPLE_ID tuple,OBJECT_ID obj)
{
	if(tuple == 0 || obj == 0 || tuple_GetLen(tuple) == 0)
		return(-1);

	for(INDEX i = 0; i < tuple_GetLen(tuple); i++)
	{
		OBJECT_ID it_id = tuple_GetItem(tuple,i);
		if(obj_GetType(it_id) != TYPE_KV)
		{
			if(!obj_Compare(it_id,obj))
			{
				return(i);
			}
		}
		else
		{
			kv_object *kv = (kv_object*)mem_lock(it_id);
			if(!obj_Compare(kv->value,obj))
			{
				mem_unlock(it_id,0);
				return(i);
			}
			mem_unlock(it_id,0);
		}
	}
	return(-1);
}

void tuple_ConvertToDict(TUPLE_ID tuple)
{
	if(tuple == 0 || tuple_GetLen(tuple) == 0)
		return;

	for (NUM i = 0; i < tuple_GetLen(tuple); i++)
	{
		OBJECT_ID it_id = tuple_GetItem(tuple,i);
		//object *it = (object*)mem_lock(it_id);
		if(obj_GetType(it_id) != TYPE_KV)
		{
			tuple_SetItem(tuple,i, obj_ConvertToKV(it_id));
			//gc_IncRefCount(((tuple_object *) tuple)->list->items[i]);
			//gc_DecRefCount(((kv_object*)((tuple_object *) tuple)->list->items[i])->key);
		}
		else
			continue;
		//mem_unlock(it_id,0);
	}
}

INDEX tuple_GetItemIndexByName(TUPLE_ID tuple, BYTES_ID name)
{
	if(tuple == 0 || name == 0 || tuple_GetLen(tuple) == 0)
		return(-1);
	for(NUM i = 0; i < tuple_GetLen(tuple); i++)
	{
		OBJECT_ID it_id = tuple_GetItem(tuple,i);
		if(it_id == 0)
			continue;
		object *it = (object*)mem_lock(it_id);
		if(it->type == TYPE_KV)
		{
			OBJECT_ID old = it_id;
			it_id = ((kv_object*)it)->key;
			mem_unlock(old,0);
			it = (object*)mem_lock(it_id);
		}
		if(it->type == TYPE_UNICODE && mem_compare(((unicode_object*)it)->value, name)) 
		{
			mem_unlock(it_id,0);
			return(i);
		}
		mem_unlock(it_id,0);
	}
	return(-1);
}

OBJECT_ID tuple_GetDictItemByName(TUPLE_ID tuple,BYTES_ID name)
{
	if(tuple == 0 || name == 0 || tuple_GetLen(tuple) == 0)
		return(0);
	for (INDEX i = 0; i < tuple_GetLen(tuple); i++)
	{		
		OBJECT_ID it_id = tuple_GetItem(tuple,i);
		object *it = (object*)mem_lock(it_id);
		if(it_id == 0)
			continue;
		if(it->type == TYPE_KV)
		{
			OBJECT_ID key_id = ((kv_object*)it)->key;
			OBJECT_ID value_id = ((kv_object*)it)->value;
			object *key = (object*)mem_lock(key_id);
			if(key->type == TYPE_UNICODE && mem_compare(((unicode_object*)key)->value, name)) 
			{
				mem_unlock(key_id,0);
				mem_unlock(it_id,0);
				return(value_id);
			}
			mem_unlock(key_id,0);
		}
		mem_unlock(it_id,0);
	}
	return(0);
}

void tuple_ResetIteration(TUPLE_ID tuple)
{
	if(tuple == 0) 
		return;
	tuple_object *tp = (tuple_object*)mem_lock(tuple);
	if(tp->type != TYPE_TUPLE || list_GetLen(tp->list) == 0)
	{
		mem_unlock(tuple,0);
		return;
	}
	tp->ptr = 0;
	mem_unlock(tuple,0);
}

void tuple_SetItem(TUPLE_ID tuple, INDEX index, OBJECT_ID obj)
{
	if(tuple == 0 || !tuple_IsTuple(tuple)) 
		return;
	if(index >= tuple_GetLen(tuple))
		return;
	tuple_object *tp = (tuple_object*)mem_lock(tuple);
	if(index < 0)
		index = index + tuple_GetLen(tuple);
	OBJECT_ID old = list_Get(tp->list,index);	
	if(obj != 0)
	{
		list_Set(tp->list,index,obj);
		obj_IncRefCount(obj);
	}
	else
	{
		list_Set(tp->list,index,0);
		obj_IncRefCount(0);//obj_NULL
	}
	if(old != 0)
	{
		obj_DecRefCount(old);
	}
	mem_unlock(tuple,0);
}

OBJECT_ID tuple_GetItem(TUPLE_ID tuple, INDEX index)
{
	if(tuple == 0 || tuple_GetLen(tuple) == 0) 
		return(0);
	if(index >= tuple_GetLen(tuple))
		return(0);
	tuple_object *tp = (tuple_object*)mem_lock(tuple);
	if(index < 0)
		index = index + tuple_GetLen(tuple);
	OBJECT_ID r = list_Get(tp->list,index);
	mem_unlock(tuple,0);
	return(r);
}

void tuple_DeleteItem(TUPLE_ID tuple, INDEX index)
{
	if(tuple == 0 || tuple_GetLen(tuple) == 0) 
		return;
	if(index >= tuple_GetLen(tuple))
		return;
	if(index < 0)
		index = index + tuple_GetLen(tuple);

	OBJECT_ID item_id = tuple_GetItem(tuple,index);	
	//printf("removing item:%d,%d\n",index,item_id);
	if(item_id == 0)
		return;
	//obj_DecRefCount(item_id);
	tuple_SetItem(tuple,index,0);
}

void tuple_DeleteDictItem(TUPLE_ID tuple,OBJECT_ID key)
{
	if(tuple == 0 || key == 0 || tuple_GetLen(tuple) == 0) 
		return;
	INDEX index = tuple_GetDictItemIndex(tuple,key);
	if(index != -1)
	{
		tuple_DeleteItem(tuple,index);
	}
}

OBJECT_ID tuple_GetNextItem(TUPLE_ID tuple)
{
	if(tuple == 0|| tuple_GetLen(tuple) == 0)
		return(0);
	tuple_object *tp = (tuple_object*)mem_lock(tuple);
	if(tp->type != TYPE_TUPLE || list_GetLen(tp->list) == 0)
	{
		mem_unlock(tuple,0);
		return(0);
	}
	if(tp->ptr < list_GetLen(tp->list))
	{
		tp->ptr++;
		mem_unlock(tuple,0);
		return(tuple_GetItem(tuple,tp->ptr-1));
	}
	else
	{	
		tp->ptr = 0;
		mem_unlock(tuple,0);
		return(0);
	}
}



