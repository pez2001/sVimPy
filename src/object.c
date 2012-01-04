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


#include "object.h"

object *AllocEmptyObject()
{
	return ((object *)
			mem_malloc(sizeof(empty_object), "AllocEmptyObject() return"));
}

object *AllocKVObject()
{
	return ((object *)
			mem_malloc(sizeof(kv_object), "AllocKVObject() return"));
}

object *AllocObject()
{
	return ((object *) mem_malloc(sizeof(object), "AllocObject() return"));
}

code_object *AllocCodeObject()
{
	return ((code_object *)
			mem_malloc(sizeof(code_object), "AllocCodeObject() return"));
}

/*caller_object *AllocCallerObject()
{
	return ((caller_object *)
			mem_malloc(sizeof(caller_object), "AllocCallerObject() return"));
}
*/
function_object *AllocFunctionObject()
{
	return ((function_object *)
			mem_malloc(sizeof(function_object),
					   "AllocFunctionObject() return"));
}

string_object *AllocStringObject()
{
	return ((string_object *)
			mem_malloc(sizeof(string_object), "AllocStringObject() return"));
}

tuple_object *AllocTupleObject()
{
	return ((tuple_object *)
			mem_malloc(sizeof(tuple_object), "AllocTupleObject() return"));
}

block_object *AllocBlockObject()
{
	return ((block_object *)
			mem_malloc(sizeof(block_object), "AllocBlockObject() return"));
}

unicode_object *AllocUnicodeObject() 
{
   return((unicode_object*)mem_malloc(sizeof(unicode_object), "AllocUnicodeObject() return")); 
}

int_object *AllocIntObject() 
{
   return((int_object*)mem_malloc(sizeof(int_object), "AllocIntObject() return")); 
}

int IsIntObject(object * obj)
{
	return (obj->type == TYPE_INT);
}

int IsStringObject(object * obj)
{
	return (obj->type == TYPE_STRING);
}

int IsUnicodeObject(object * obj)
{
	return (obj->type == TYPE_UNICODE);
}

int IsCodeObject(object * obj)
{
	return (obj->type == TYPE_CODE);
}

int IsTupleObject(object * obj)
{
	return (obj->type == TYPE_TUPLE);
}


string_object *AsStringObject(object * obj)
{
	return ((string_object *) obj);
}

code_object *AsCodeObject(object * obj)
{
	return ((code_object *) obj);
}

/*caller_object *AsCallerObject(object * obj)
{
	return ((caller_object *) obj);
}
*/
function_object *AsFunctionObject(object * obj)
{
	return ((function_object *) obj);
}

tuple_object *AsTupleObject(object * obj)
{
	return ((tuple_object *) obj);
}

unicode_object *AsUnicodeObject(object *obj)
 {
   return((unicode_object*)obj); 
  }

int_object *AsIntObject(object *obj)
 {
   return((int_object*)obj); 
  }
   
object *AllocRefObject()
{
   return((ref_object*)mem_malloc(sizeof(ref_object), "AllocRefObject() return")); 
}
   
ref_object *AsRefObject(object *obj)
{   
   return((ref_object*)obj); 
}

int IsRefObject(object * obj)
{
	return (obj->type == TYPE_REF);
}

object *DissolveRef(object *obj)
{
if(obj->type == TYPE_REF)
	return(((ref_object*)obj)->ref);
return(obj);
}

ref_object *CreateRefObject(object *ref_to,int flags)
{
	ref_object *r = AllocRefObject();
	r->type = TYPE_REF;
	r->flags = flags;
	r->ref_count = 0;
	r->ref = ref_to;
	IncRefCount(ref_to);
	if(debug_level > 3)
		printf("created ref object to :%c\n",ref_to->type);
	return(r);
}

int_object *CreateIntObject(long value,int flags)
{
	int_object *r = AllocIntObject();
	r->type = TYPE_INT;
	r->ref_count = 0;
	r->flags = flags;
	r->value = value;
	return(r);
}

unicode_object *CreateUnicodeObject(char *value,int flags)
{
	unicode_object *r = AllocUnicodeObject();
	r->type = TYPE_UNICODE;
	r->flags = flags;
	r->ref_count = 0;
	r->value = value;
	return(r);
}

tuple_object *CreateTuple(int num,int flags)
{
	tuple_object *r = AllocTupleObject();
	r->type = TYPE_TUPLE;
	r->flags = flags;
	r->ref_count = 0;
	r->list = ptr_CreateList(num,PTR_STATIC_LIST);
	return(r);
}

string_object *CreateStringObject(char *bytes,int len,int flags)
{
	string_object *r = AllocStringObject();
	r->type = TYPE_STRING;
	r->flags = flags;
	r->ref_count = 0;
	r->content = bytes;
	r->len = len;
	return(r);
}

kv_object *CreateKVObject(object *key,object *value,int flags)
{
	kv_object *r = AllocKVObject();
	r->type = TYPE_KV;
	r->flags = flags;
	r->ref_count = 0;
	r->key = key;
	IncRefCount(key);
	r->value = value;
	DecRefCount(value);
	return(r);
}

empty_object *CreateEmptyObject(char type,int flags)
{
	empty_object *r = AllocEmptyObject();
	r->type = type;
	r->flags = flags;
	r->ref_count = 0;
	return(r);
}
   
long objects_num = 0;

long objects_max = 0;

long objects_header_total = 0;

kv_object *ConvertToKVObject(object *obj)
{
	kv_object *kv = AllocKVObject();
	kv->type = TYPE_KV;
	kv->flags = 0;
	kv->ref_count = 0;
	kv->key = obj;
	IncRefCount(obj);
	kv->value = NULL;
	return(kv);
}

kv_object *ConvertToKVObjectValued(object *key,object *value)
{
	kv_object *kv = AllocKVObject();
	kv->type = TYPE_KV;
	kv->flags = 0;
	kv->ref_count = 0;
	kv->key = key;
	IncRefCount(key);
	kv->value = value;
	IncRefCount(value);
	return(kv);
}

void FreeObject(object * obj)
{
	//assert(obj != NULL);
	if (obj == NULL)
		return;
	if(debug_level > 2)
	{
	printf("object to be freed\n");
	 DumpObject(obj,1);
	}
	
	if(obj->ref_count > 1)
	{
		DecRefCount(obj);
		return;
	}
	//DecRefCount(obj);
	
	switch (obj->type)
	{
	case TYPE_BLOCK:
		objects_header_total -= sizeof(block_object);
		break;
	case TYPE_REF:
		if(debug_level > 3)
			printf("Freeing Ref to %x\n",((ref_object*)obj)->ref);
		objects_header_total -= sizeof(ref_object);
		break;
	case TYPE_NULL:
		// printf("freeing NULL object @%x\n",obj);
		objects_header_total -= sizeof(empty_object);
		break;
	case TYPE_NONE:
		// printf("freeing NONE object @%x\n",obj);
		objects_header_total -= sizeof(empty_object);
		break;
	case TYPE_INT:
		// printf("freeing int object @%x\n",obj);
		objects_header_total -= sizeof(empty_object);
		break;
	case TYPE_KV:
		objects_header_total -= sizeof(kv_object);
		FreeObject(((kv_object *) obj)->key);
		FreeObject(((kv_object *) obj)->value);
		break;
	case TYPE_UNICODE:
		//if(obj->flags &OFLAG_ON_STACK)
		//	printf("on stack\n");
		if(debug_level > 3)
			printf("freeing unicode object @%x\n",obj);
		// printf("content:%s\n",((unicode_object*)obj)->value);
		// if(((unicode_object*)obj->ptr)->content != NULL)
		// if(obj->ptr != NULL)
		// mem_free(obj->ptr);
		objects_header_total -= sizeof(unicode_object);
		assert(mem_free(((unicode_object*) obj)->value));
		break;
	case TYPE_STRING:
		// printf("freeing string object @%x\n",obj);
		objects_header_total -= sizeof(string_object);
		assert(mem_free(((string_object *) obj)->content));
		break;
	case TYPE_TUPLE:
		 //printf("freeing tuple object @%x\n",obj);
		objects_header_total -= sizeof(tuple_object);
		if (((tuple_object *)obj)->list != NULL  && ((tuple_object *) obj)->list->num > 0)
		{
			for (int i = 0; i < ((tuple_object *) obj)->list->num; i++)
			{
						//printf("checking sub:%c\n",((object*)((tuple_object *) obj)->list->items[i])->type);
				//if ((((tuple_object *) obj)->list->items[i]) != NULL && !(((object*)((tuple_object *) obj)->list->items[i])->flags & OFLAG_ON_STACK))
				//{
					//printf("freeing sub:%c\n",((object*)((tuple_object *) obj)->list->items[i])->type);
					FreeObject(((tuple_object *) obj)->list->items[i]);
					//if(((object*)((tuple_object *) obj->ptr)->list->items[i])->value_ptr != NULL && !(((object*)((tuple_object *) obj->ptr)->list->items[i])->flags & OFLAG_ON_STACK))
					//	FreeObject(((object*)((tuple_object *) obj->ptr)->list->items[i])->value_ptr);
				//}
				((tuple_object *) obj)->list->items[i] = NULL;
			}
			ptr_CloseList(((tuple_object *) obj)->list);
			
			//assert(mem_free(((tuple_object *) obj->ptr)->list->items));
			((tuple_object *) obj)->list = NULL;
		}
		break;
	case TYPE_CODE:
		 //printf("freeing code object @%x\n",obj);
		objects_header_total -= sizeof(code_object);
		objects_header_total -= strlen(((code_object *) obj)->name) + 1;
		assert(mem_free(((code_object *) obj)->name));
		FreeObject(((code_object *) obj)->code);
		FreeObject(((code_object *) obj)->consts);
		FreeObject(((code_object *) obj)->names);
		FreeObject(((code_object *) obj)->varnames);
		FreeObject(((code_object *) obj)->freevars);
		FreeObject(((code_object *) obj)->cellvars);
		// FreeObject(((code_object*)obj->ptr)->filename); //TO DECREASE
		// MEMORY USAGE
		// FreeObject(((code_object*)obj->ptr)->lnotab);//TO DECREASE MEMORY
		// USAGE
		break;

	}
	// recycle_Remove(obj);

//	if (obj->type != TYPE_INT && obj->type != TYPE_NONE
//		&& obj->type != TYPE_NULL && obj->type != TYPE_TRUE
//		&& obj->type != TYPE_FALSE && obj->ptr != NULL)
//		assert(mem_free(obj->ptr));
	// if(obj->value_ptr != NULL)
	// FreeObject((object*)obj->value_ptr);
	//printf("freed object(%c) @%x\n",obj->type,obj);
	assert(mem_free(obj));
	objects_num--;
}

void PrintObject(object * obj)
{
	if (obj != NULL)
	{
		// printf("tos type:%c\n",obj->type);
		switch (obj->type)
		{
		case TYPE_REF:
			PrintObject(((ref_object*)obj)->ref);
			break;
		case TYPE_INT:
			printf("%d", ((int_object*)obj)->value);
			break;
		case TYPE_KV:
			PrintObject(((kv_object*)obj)->value);
			break;
		case TYPE_UNICODE:
			printf("%s", ((unicode_object *)obj)->value);
			break;
		}

	}
}

/* void DumpObject(object *obj) { DumpObject(obj,0); } */
void DumpObject(object * obj, int level)
{
	if (obj == NULL)
		return;
	// printf("level:%d\n",level);
	for (int i = 0; i < level; i++)
		printf("\t");
	printf("(%d)(%x)",obj->ref_count,obj);
	switch (obj->type)
	{
	case TYPE_BLOCK:
		printf("block object\n");
		break;
	case TYPE_REF:
		printf("ref object\n");
		DumpObject(((ref_object*)obj)->ref,level+1);
		break;
	case TYPE_TRUE:
		printf("true object\n");
		break;
	case TYPE_FALSE:
		printf("false object\n");
		break;
	case TYPE_NULL:
		printf("NULL object\n");
		break;
	case TYPE_NONE:
		printf("NONE object\n");
		break;
	case TYPE_INT:
		printf("int object: %d\n", ((int_object*)obj)->value);
		break;
	case TYPE_KV:
		DumpObject(((kv_object*)obj)->key,level);
		DumpObject(((kv_object*)obj)->value,level + 1);
		break;
	case TYPE_UNICODE:
		printf("unicode object: %s\n", ((unicode_object*)obj)->value);
		break;
	case TYPE_STRING:
		printf("string object: ");
		for (int i = 0; i < ((string_object *) obj)->len; i++)
			printf("%02x ",
				   (unsigned char)((string_object *) obj)->content[i]);
		printf("\n");
		break;
	case TYPE_TUPLE:
		printf("tuple object");
		if (((tuple_object *) obj)->list != NULL && ((tuple_object *) obj)->list->num > 0)
		{
			printf(" contains %d items\n", ((tuple_object *) obj)->list->num);
			for (int i = 0; i < ((tuple_object *) obj)->list->num; i++)
			{
				if (((tuple_object *) obj)->list->items[i] != NULL)
				{

					if ((((object*)((tuple_object *) obj)->list->items[i])->
						 flags & OFLAG_TUPLE_PTR) > 0)
						printf("%d->",i);
					else
						printf("%d  ",i);
					DumpObject(((tuple_object *) obj)->list->items[i],
							   level + 1);
				}
			}
		}
		else
		{
			printf("\n");
			for (int i = 0; i < level; i++)
				printf("\t");
			printf("empty tuple\n");
		}
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("-- tuple object\n");
		break;
	case TYPE_CODE:
		printf("code object\n");
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("name: %s\n", ((code_object *) obj)->name);
		if(debug_level > 2)
		{
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("flags:%d\n",((code_object *) obj)->flags);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("nlocals:%d\n",((code_object *) obj)->nlocals);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("argcount:%d\n",((code_object *) obj)->argcount);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("kwonlyargcount:%d\n",((code_object *) obj)->kwonlyargcount);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("code:\n");
		DumpObject(((code_object *) obj)->code, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("consts:\n");
		DumpObject(((code_object *) obj)->consts, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("names:\n");
		DumpObject(((code_object *) obj)->names, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("varnames:\n");
		DumpObject(((code_object *) obj)->varnames, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("freevars:\n");
		DumpObject(((code_object *) obj)->freevars, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("cellvars:\n");
		DumpObject(((code_object *) obj)->cellvars, level + 1);
		}
		// for(int i=0;i<level;i++)
		// printf("\t");
		// printf("lnotab:\n");
		// DumpObject(((code_object*)obj->ptr)->lnotab,level+1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("-- code object\n");
		break;
	default:
		printf("object type is unknown:%c\n", obj->type);
	}
}

int object_compare(object *a,object *b)
{
	assert(a != NULL && b != NULL);
	if (a == NULL && b == NULL)
		return(0);
	if(a== NULL || b == NULL)
		return (-1);
	
	switch (a->type)
	{
	case TYPE_NULL:
		return(-1);
		break;
	case TYPE_NONE:
		return(-1);
		break;
	case TYPE_KV:
		return(object_compare(((kv_object*)a)->key,((kv_object*)b)->key) && object_compare(((kv_object*)a)->value,((kv_object*)b)->value));
	case TYPE_INT:
		return(((int_object*)a)->value == ((int_object*)b)->value);
		break;
	case TYPE_UNICODE:
		return(strcmp(((unicode_object*)a)->value,((unicode_object*)b)->value));
		break;
	case TYPE_STRING:
		break;
	case TYPE_TUPLE:
		/*if (((tuple_object *) obj->ptr)->list != NULL  && ((tuple_object *) obj->ptr)->list->num > 0)
		{
		ret->
			for (int i = 0; i < ((tuple_object *) obj->ptr)->list->num; i++)
			{
				if ((((tuple_object *) obj->ptr)->list->items[i]) != NULL
					&& !(((object*)((tuple_object *) obj->ptr)->list->items[i])->
						 flags & OFLAG_ON_STACK))
				{
					FreeObject(((tuple_object *) obj->ptr)->list->items[i]);
				}
				((tuple_object *) obj->ptr)->list->items[i] = NULL;
			}
			ptr_CloseList(((tuple_object *) obj->ptr)->list);
			
			//assert(mem_free(((tuple_object *) obj->ptr)->list->items));
			((tuple_object *) obj->ptr)->list = NULL;
		}*/
		break;
	case TYPE_CODE:
		break;

	}
return(-1);
}

object *CopyObject(object *obj)
{
	assert(obj != NULL);
	if (obj == NULL)
		return;

	
	switch (obj->type)
	{
	case TYPE_NULL:
	case TYPE_NONE:
		{	
		empty_object *r = AllocEmptyObject();
		r->type = obj->type;
		r->ref_count  = 0;
		r->flags = obj->flags | OFLAG_ON_STACK;
		return(r);
		}
		break;
	case TYPE_INT:
		{
		int_object *r = AllocIntObject();
		r->type = obj->type;
		r->ref_count  = 0;
		r->flags = obj->flags | OFLAG_ON_STACK;
		r->value = ((int_object*)obj)->value;
		return(r);
		}
		break;
	case TYPE_UNICODE:
	   {
		unicode_object *r = AllocUnicodeObject();
		r->type = obj->type;
		r->ref_count  = 0;
		r->flags = obj->flags | OFLAG_ON_STACK;
		r->value = str_Copy(((unicode_object*)obj)->value);
		return(r);
		}
		break;
	case TYPE_STRING:
		//ret->ptr = mem_malloc(
		break;
	case TYPE_TUPLE:
		/*if (((tuple_object *) obj->ptr)->list != NULL  && ((tuple_object *) obj->ptr)->list->num > 0)
		{
		ret->
			for (int i = 0; i < ((tuple_object *) obj->ptr)->list->num; i++)
			{
				if ((((tuple_object *) obj->ptr)->list->items[i]) != NULL
					&& !(((object*)((tuple_object *) obj->ptr)->list->items[i])->
						 flags & OFLAG_ON_STACK))
				{
					FreeObject(((tuple_object *) obj->ptr)->list->items[i]);
				}
				((tuple_object *) obj->ptr)->list->items[i] = NULL;
			}
			ptr_CloseList(((tuple_object *) obj->ptr)->list);
			
			//assert(mem_free(((tuple_object *) obj->ptr)->list->items));
			((tuple_object *) obj->ptr)->list = NULL;
		}*/
		break;
	case TYPE_CODE:
	//code_object *c = AllocCodeObject();
	//c->name = str_Copy(((code_object*)obj)->name);
	//c-> = str_Copy(((code_object*)obj)->name);
	//add reference object to decrease memory usage for code objects
	//reference objects refs need no freeing
	return(CreateRefObject(obj,obj->flags));
	//return(c);	
	/*
		// printf("freeing code object @%x\n",obj);
		objects_header_total -= sizeof(object);
		objects_header_total -= strlen(((code_object *) obj->ptr)->name) + 1;
		assert(mem_free(((code_object *) obj->ptr)->name));
		FreeObject(((code_object *) obj->ptr)->code);
		FreeObject(((code_object *) obj->ptr)->consts);
		FreeObject(((code_object *) obj->ptr)->names);
		FreeObject(((code_object *) obj->ptr)->varnames);
		FreeObject(((code_object *) obj->ptr)->freevars);
		FreeObject(((code_object *) obj->ptr)->cellvars);
		*/
		break;

	}
  return(NULL);
}

void AppendDictItem(object * tuple,object *key,object *value)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if(((tuple_object *) tuple)->list == NULL)
		((tuple_object *) tuple)->list = ptr_CreateList(0,0);
	object *di = CopyObject(key);//TODO needed??
	kv_object *kv = ConvertToKVObjectValued(di,value);
	if(debug_level > 4)
	{
	printf("appended to \n");
	DumpObject(tuple,0);
	printf("this\n");
	DumpObject(di,0);
	}
	ptr_Push(((tuple_object *) tuple)->list,kv);

}

void SetDictItem(object *tuple,object *key,object *value)
{
	int index = GetDictItemIndex(tuple,key);
	//printf("setting\n");
	//DumpObject(tuple,0);
	//printf("key\n");
	//DumpObject(key,0);
	//printf("value\n");
	//DumpObject(value,0);
	if(index != -1)
		{
			//printf("found index\n");
			//SetItem(tuple,index,value);
			kv_object * k = GetItem(tuple,index);
			if(k->value != NULL)
				FreeObject(k->value);
			k->value = value;
			IncRefCount(value);
		}
	else
	{
	//printf("Appending to dict\n");
	//DumpObject(tuple,0);
	//DumpObject(key,1);
	//DumpObject(value,1);
	 AppendDictItem(tuple,key,value);
	}
}

void IncRefCount(object *obj)
{
	obj->ref_count++;
}
void DecRefCountGC(object *obj,ptr_list *gc)
{
	if(obj->ref_count == 1)
	 {
		obj->ref_count--;
		if(!ptr_Contains(gc,obj))
		{
		if(debug_level > 2)
		{
		printf("object has no refs anymore -> put into gc\n");
		DumpObject(obj,0);
		}
		//FreeObject(obj);
		ptr_Queue(gc,obj);
		}
		return;
	 }
	obj->ref_count--;
}
void DecRefCount(object *obj)
{
	if(obj->ref_count == 1)
	 {
		if(debug_level > 1)
		{
		printf("object has no refs anymore -> freeing\n");
		DumpObject(obj,0);
		}
		FreeObject(obj);
		//ptr_Queue(gc,obj);
		return;
	 }
	obj->ref_count--;
}
int HasNoRefs(object *obj)
{
return(!obj->ref_count);
}
int HasRefs(object *obj)
{
return(obj->ref_count);
}



void SetDictItemByIndex(object *tuple,int index,object *value)
{
	if(index != -1)
		{
			//printf("found index\n");
			//SetItem(tuple,index,value);
			kv_object * k = GetItem(tuple,index);
			if(k->value != NULL)
				FreeObject(k->value);
			//	DecRefCount(k->value);
			k->value = value;
			IncRefCount(value);
		}
}

object *GetDictItem(object *tuple,object *key)
{
	int index = GetDictItemIndex(tuple,key);
	if(index != -1)
		{
			return(((kv_object*)GetItem(tuple,index))->value);
		}
	return(NULL);
}

int GetDictItemIndex(object *tuple,object *key)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL)
		return (-1);
	 //printf("checking in %d tuple items for\n",((tuple_object*)tuple->ptr)->list->num);
	//printf("getdictitemindex\n");
	//DumpObject(key,0);
	//DumpObject(tuple,0);
	for (int i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{
		 //printf("checking tuple:%d\n",i);
		// if(((tuple_object*)tuple->ptr)->items[i]->type == TYPE_UNICODE)
		// printf("checking %s against:
		// %s\n",name,(char*)((tuple_object*)tuple->ptr)->items[i]->ptr);
		if (((tuple_object *) tuple)->list->items[i] == NULL || ((object*)((tuple_object *) tuple)->list->items[i])->type != TYPE_KV)
			continue;
		if ( !object_compare( ((kv_object*)((tuple_object *) tuple)->list->items[i])->key,key))
			return (i);
	}
	return (-1);
}

int GetItemIndexByName(object * tuple, char *name)
{
	//printf("checking for correct type\n");
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL)
		return (-1);
	 //printf("checking in %d tuple items for %s\n",((tuple_object*)tuple)->list->num,name);
	for (int i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{
		//printf("checking tuple:%d\n",i);
		// if(((tuple_object*)tuple->ptr)->items[i]->type == TYPE_UNICODE)
		 //printf("checking %s against: %s\n",name,((unicode_object*)((tuple_object*)tuple)->list->items[i])->value);
		
		if (((tuple_object *) tuple)->list->items[i] == NULL)
			continue;
		object *item = (object*)((tuple_object *) tuple)->list->items[i];
		if(item->type == TYPE_KV)
		item = (object*)((kv_object*)item)->key;
		//DumpObject(item,0);

		if (item->type == TYPE_UNICODE	&& !strcmp(((unicode_object*)item)->value, name)) 
		{
			//printf("found it:%d\n",i );
			return (i);
		}
		//if ((((object*)((tuple_object *) tuple)->list->items[i])->type == TYPE_UNICODE	&& !strcmp(((unicode_object*)((tuple_object *) tuple)->list->items[i])->value, name)) || 
		//   (((object*)((tuple_object *) tuple)->list->items[i])->type == TYPE_KV && (((kv_object*)((tuple_object *) tuple)->list->items[i])->type == TYPE_UNICODE	&& !strcmp(((unicode_object*)((tuple_object *) tuple)->list->items[i])->value, name) ) )  )
		//	return (((tuple_object *) tuple)->list->items[i]);
	}
	return (-1);
}

void ResetIteration(object * tuple)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;

	if ((tuple->flags & OFLAG_TUPLE_RESTART_FLAG))
	{
		tuple->flags ^= OFLAG_TUPLE_RESTART_FLAG;
		return;
	}

	for (int i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{
		 //printf("checking tuple:%d\n",i);
		// if(((tuple_object*)tuple->ptr)->items[i]->type == TYPE_UNICODE)
		// printf("checking %s against:
		// %s\n",name,(char*)((tuple_object*)tuple->ptr)->items[i]->ptr);
		if (((tuple_object *) tuple)->list->items[i] != NULL
			&& (((object*)((tuple_object *) tuple)->list->items[i])->
				flags & OFLAG_TUPLE_PTR) > 0)
		{
			// update tuple ptr
			((object*)((tuple_object *) tuple)->list->items[i])->flags ^= OFLAG_TUPLE_PTR;
		}
	}
	if (((tuple_object *) tuple)->list->items[0] != NULL)
		((object*)((tuple_object *) tuple)->list->items[0])->flags |= OFLAG_TUPLE_PTR;

}

void SetItem(object * tuple, int index, object * obj)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if (index >= ((tuple_object *) tuple)->list->num || index < 0)
		return;
	if(((tuple_object *) tuple)->list->items[index] != NULL)
	{
		FreeObject((object*)((tuple_object *) tuple)->list->items[index]);
	}
	
	((tuple_object *) tuple)->list->items[index] = obj;
	IncRefCount(obj);
	// obj->flags ^= OFLAG_ON_STACK;
}

void DeleteItem(object * tuple, int index)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if (index >= ((tuple_object *) tuple)->list->num || index < 0)
		return;
	if (((tuple_object *) tuple)->list->items[index] == NULL)
		return;
	//if (!(((object*)((tuple_object *) tuple)->list->items[index])->type & OFLAG_ON_STACK))
		FreeObject(((tuple_object *) tuple)->list->items[index]);
	((tuple_object *) tuple)->list->items[index] = NULL;
}

object *GetItem(object * tuple, int index)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return (NULL);
	if (index >= ((tuple_object *) tuple)->list->num || index < 0)
		return (NULL);
	return (((tuple_object *) tuple)->list->items[index]);
}

object *GetNextItem(object * tuple)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return (NULL);

	if ((tuple->flags & OFLAG_TUPLE_RESTART_FLAG))
	{
		tuple->flags ^= OFLAG_TUPLE_RESTART_FLAG;
		return (NULL);
	}


	for (int i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{
		if (((tuple_object *) tuple)->list->items[i] == NULL)
			continue;
		// printf ("checking tuple:%d\n", i);
		// if(((tuple_object*)tuple->ptr)->items[i]->type == TYPE_UNICODE)
		// printf("checking %s against:
		// %s\n",name,(char*)((tuple_object*)tuple->ptr)->items[i]->ptr);
		if ((((object*)((tuple_object *) tuple)->list->items[i])->
			 flags & OFLAG_TUPLE_PTR) > 0)
		{
			// update tuple ptr
			((object*)((tuple_object *) tuple)->list->items[i])->flags ^= OFLAG_TUPLE_PTR;
			if (i + 1 < ((tuple_object *) tuple)->list->num)
				((object*)((tuple_object *) tuple)->list->items[i + 1])->flags |=
					OFLAG_TUPLE_PTR;
			else				// reset
			{
				((object*)((tuple_object *) tuple)->list->items[0])->flags |=
					OFLAG_TUPLE_PTR;
				tuple->flags |= OFLAG_TUPLE_RESTART_FLAG;
				// return(NULL);
			}
			// printf("returning tuple iteration\n");
			return (((tuple_object *) tuple)->list->items[i]);
		}
	}
	return (NULL);
}


long ReadLong(FILE * f)
{
	long r = 0;

	// char *b = (char*)mem_malloc(4);
	int read = fread(&r, 4, 1, f);

	// r = *(long*)b;
	// free(b);
	return (r);
}

char ReadChar(FILE * f)
{
	char r = 0;

	int read = fread(&r, 1, 1, f);

	return (r);
}

object *ReadObject(FILE * f)
{
	objects_num++;
	// printf("increasing objects num to %d\n",objects_num);
	if (objects_num > objects_max)
		objects_max = objects_num;
	char type = ReadChar(f);

		
		if(debug_level > 4)
			printf("reading object with type:%c\n",type);
	// long magic = ReadLong(f);

	switch (type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		{
		// printf("allocating empty object\n");
		empty_object *obj = AllocEmptyObject();
		obj->type = type;
		obj->flags = 0;
		obj->ref_count = 1;
		// printf("allocated empty object @%x\n",obj);
		objects_header_total += sizeof(empty_object);
		return (obj);
		}
	case TYPE_CODE:
       {	
		// printf("reading code chunk\n");
		objects_header_total += sizeof(code_object);
		// printf("allocated object @%x\n",obj);
		code_object *co = AllocCodeObject();
		co->flags = 0;
		co->type = TYPE_CODE;
		co->ref_count = 1;

		// printf("co_ptr=%x\n",(unsigned long)co);
		co->argcount = ReadLong(f);
		co->kwonlyargcount = ReadLong(f);
		co->nlocals = ReadLong(f);
		co->stacksize = ReadLong(f);
		co->co_flags = ReadLong(f);
		// printf("argcount:%d,kwonlyargcount:%d,nlocals:%d,stacksize:%d,flags:%d\n",co->argcount,co->kwonlyargcount,co->nlocals,co->stacksize,co->flags);
		co->code = ReadObject(f);
		co->consts = ReadObject(f);
		co->names = ReadObject(f);
		co->varnames = ReadObject(f);
		co->freevars = ReadObject(f);
		co->cellvars = ReadObject(f);
		// printf("reading filename\n");
		object *tmp_filename = ReadObject(f);

		// co->filename = ReadObject(f);
		// printf("filename:%s\n",(char*)co->filename->ptr);
		// FreeObject(co->filename); //TO DECREASE MEMORY USAGE
		FreeObject(tmp_filename);	// TO DECREASE MEMORY USAGE
		object *tmp_name = ReadObject(f);

		// if(IsUnicodeObject(tmp_name))
		// printf("name:%s\n",(char*)tmp_name->ptr);
		co->name = str_Copy(((unicode_object *)tmp_name)->value);
		objects_header_total += strlen(co->name) + 1;
		FreeObject(tmp_name);
		co->firstlineno = ReadLong(f);
		// co->lnotab = ReadObject(f);
		object *tmp_lnotab = ReadObject(f);

		// printf("read lnotab\n");
		// FreeObject(co->lnotab);//TO DECREASE MEMORY USAGE
		FreeObject(tmp_lnotab);	// TO DECREASE MEMORY USAGE
		// printf("freed lnotab\n");
		// co->name =
		// str_Copy(((string_object*)tmp_name->ptr)->content);//TODO free
		// object tmp_name and string_copy it first

		// printf("string copied");
		// FreeObject(tmp_name);
		// printf ("module name:%s\n", co->name);
		// printf("filename:%s\n",((string_object*)co->filename->ptr)->content);
		// printf("firstlineno:%d\n",co->firstlineno);
		// ReadChunk(f);
		return(co);
		}
		break;
	case TYPE_STRING:
		{
		string_object *obj = AllocStringObject();
		objects_header_total += sizeof(string_object);
		// printf("allocated object @%x\n",obj);
		obj->flags = 0;
		obj->ref_count = 1;

		long n;
		// printf("reading string chunk\n");
		n = ReadLong(f);
		if (n > 0)
		{
			char *string = (char *)mem_malloc(n, "ReadObject() string");

			// printf("str_ptr=%x\n",(unsigned long)string);
			int read = fread(string, n, 1, f);

			// printf("str_so_ptr=%x\n",(unsigned long)so);
			obj->len = n;
			obj->content = string;
			// free(string); 

			// printf("read string:\"%s\"\n",so->content);
			// printf("n:%d\n",so->len);
			obj->type = TYPE_STRING;
		}
		else
		{
			// printf ("read empty string\n");
			obj->content = NULL;
			obj->type = TYPE_NULL;
			obj->len = 0;
		}
		return(obj);
		}
		break;
	case TYPE_TUPLE:
		{
		// printf("reading tuple\n");
		long n = ReadLong(f);
		tuple_object *to = AllocTupleObject();
		to->ref_count = 1;

		// printf("items_num:%d\n",n);
		if (n > 0)
		{
			//to->items =
			//	(object **) mem_malloc(n * sizeof(object *),
			//						   "ReadObject() to->items");
			to->list = ptr_CreateList(n,PTR_STATIC_LIST);
			for (int i = 0; i < n; i++)
			{
				object *tuple = ReadObject(f);

				if (i == 0)
				{
					tuple->flags |= OFLAG_TUPLE_PTR;
					// printf("setting tuple's iterate over ptr\n");
				}
				// printf("object type:%c\n",tuple->type);
				to->list->items[i] = tuple;
			}
		}
		else
		 to->list = NULL;
		//to->num = n;
		to->flags = 0;
		to->type = TYPE_TUPLE;
		return(to);
		}
		break;
	case TYPE_UNICODE:
		{
		// printf("reading unicode\n");
		unicode_object *obj = AllocUnicodeObject();
		obj->ref_count = 1;

		long n = ReadLong(f);
		// printf("len:%d\n",n);
		char *unicode_string =
			(char *)mem_malloc(n + 1, "ReadObject() unicode_string");
		memset(unicode_string, 0, n + 1);
		int uread = fread(unicode_string, n, 1, f);
		//printf("read unicode string:%s\n",unicode_string);
		// uo->len = n;
		obj->flags = 0;
		obj->value = unicode_string;
		obj->type = TYPE_UNICODE;
		return(obj);
		}
		break;
	case TYPE_INT:
		{
		// printf("reading int\n");
		int_object *obj = AllocIntObject();
		long n = ReadLong(f);
		obj->ref_count = 1;

		// unicode_object *uo = AllocUnicodeObject(); 
		// printf("len:%d\n",n);
		// char *unicode_string = (char*)mem_malloc(n+1);
		// memset(unicode_string,0,n+1);
		// int uread = fread(unicode_string,n,1,f);
		// uo->len = n;
		// uo->content = unicode_string;
		// obj->ptr = uo;
		obj->flags = 0;
		obj->value = n;
		obj->type = TYPE_INT;
		return(obj);
		}
		break;
	default:
		{
		empty_object *obj = AllocEmptyObject();
		obj->ref_count = 1;
		obj->type = TYPE_UNKNOWN;
		printf("unknown chunk type:%c\n", type);
		return(obj);
		}
	}
	// printf("read object\n"); 
	return (NULL);
}

