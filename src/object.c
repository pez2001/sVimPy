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

block_object *AllocBlockObject(void)
{	
	#ifdef DEBUGGING
	return((block_object *)mem_malloc(sizeof(block_object), "AllocBlockObject() return"));
	#else
	return((block_object *)malloc(sizeof(block_object)));
	#endif
}
/*
loop_block_object *AllocLoopBlockObject()
{	
	#ifdef DEBUGGING
	return((loop_block_object *)mem_malloc(sizeof(loop_block_object), "AllocLoopBlockObject() return"));
	#else
	return((loop_block_object *)malloc(sizeof(loop_block_object)));
	#endif
}*/

kv_object *AllocKVObject(void)
{
	#ifdef DEBUGGING
	return((kv_object *) mem_malloc(sizeof(kv_object), "AllocKVObject() return"));
	#else
	return((kv_object *) malloc(sizeof(kv_object)));
	#endif
}

object *AllocObject(void)
{
	#ifdef DEBUGGING
	return ((object *) mem_malloc(sizeof(object), "AllocObject() return"));
	#else
	return((object *) malloc(sizeof(object)));
	#endif
}

code_object *AllocCodeObject(void)
{
	#ifdef DEBUGGING
	return((code_object *) mem_malloc(sizeof(code_object), "AllocCodeObject() return"));
	#else
	return((code_object *) malloc(sizeof(code_object)));
	#endif
}

function_object *AllocFunctionObject(void)
{
	#ifdef DEBUGGING
	return((function_object *) mem_malloc(sizeof(function_object),"AllocFunctionObject() return"));
	#else
	return((function_object *) malloc(sizeof(function_object)));
	#endif
}

string_object *AllocStringObject(void)
{
	#ifdef DEBUGGING
	return((string_object *) mem_malloc(sizeof(string_object), "AllocStringObject() return"));
	#else
	return((string_object *) malloc(sizeof(string_object)));
	#endif
}

tuple_object *AllocTupleObject(void)
{
	#ifdef DEBUGGING
	return((tuple_object *) mem_malloc(sizeof(tuple_object), "AllocTupleObject() return"));
	#else
	return((tuple_object *) malloc(sizeof(tuple_object)));
	#endif
}

unicode_object *AllocUnicodeObject(void) 
{
	#ifdef DEBUGGING
    return((unicode_object*) mem_malloc(sizeof(unicode_object), "AllocUnicodeObject() return")); 
	#else
    return((unicode_object*) malloc(sizeof(unicode_object))); 
	#endif
}

ref_object *AllocRefObject(void)
{
	#ifdef DEBUGGING
    return((ref_object*) mem_malloc(sizeof(ref_object), "AllocRefObject() return")); 
	#else
    return((ref_object*) malloc(sizeof(ref_object))); 
	#endif
}

int_object *AllocIntObject(void) 
{
	#ifdef DEBUGGING
    return((int_object*) mem_malloc(sizeof(int_object), "AllocIntObject() return")); 
	#else
    return((int_object*) malloc(sizeof(int_object))); 
	#endif
}

float_object *AllocFloatObject(void) 
{
	#ifdef DEBUGGING
    return((float_object*) mem_malloc(sizeof(float_object), "AllocFloatObject() return")); 
	#else
	return((float_object*) malloc(sizeof(float_object))); 
	#endif
}

iter_object *AllocIterObject(void) 
{
	#ifdef DEBUGGING
	return((iter_object*) mem_malloc(sizeof(iter_object), "AllocIterObject() return"));
	#else
	return((iter_object*) malloc(sizeof(iter_object)));
	#endif
}

/*int IsIntObject(object * obj)
{
	return (obj->type == TYPE_INT);
}

int IsFloatObject(object * obj)
{
	return (obj->type == TYPE_BINARY_FLOAT);
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

int IsRefObject(object * obj)
{
	return (obj->type == TYPE_REF);
}

int IsIterObject(object * obj)
{
	return (obj->type == TYPE_ITER);
}

string_object *AsStringObject(object * obj)
{
	return ((string_object *) obj);
}

code_object *AsCodeObject(object * obj)
{
	return ((code_object *) obj);
}

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

float_object *AsFloatObject(object *obj)
 {
   return((float_object*)obj); 
  }   
  
ref_object *AsRefObject(object *obj)
{   
   return((ref_object*)obj); 
}

iter_object *AsIterObject(object *obj)
{   
   return((iter_object*)obj); 
}
*/

object *DissolveRef(object *obj)
{
if(obj->type == TYPE_REF)
	return(((ref_object*)obj)->ref);
return(obj);
}

ref_object *CreateRefObject(object *ref_to,OBJECT_FLAGS flags)
{
	ref_object *r = AllocRefObject();
	r->type = TYPE_REF;
	r->flags = flags;
	r->ref_count = 0;
	r->ref = ref_to;
	IncRefCount(ref_to);
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

int_object *CreateIntObject(long value,OBJECT_FLAGS flags)
{
	int_object *r = AllocIntObject();
	r->type = TYPE_INT;
	r->ref_count = 0;
	r->flags = flags;
	r->value = value;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

float_object *CreateFloatObject(float value,OBJECT_FLAGS flags)
{
	float_object *r = AllocFloatObject();
	r->type = TYPE_BINARY_FLOAT;
	r->ref_count = 0;
	r->flags = flags;
	r->value = value;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

unicode_object *CreateUnicodeObject(char *value,OBJECT_FLAGS flags)
{
	unicode_object *r = AllocUnicodeObject();
	r->type = TYPE_UNICODE;
	r->flags = flags;
	r->ref_count = 0;
	r->value = value;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

tuple_object *CreateTuple(NUM num,OBJECT_FLAGS flags)
{
	tuple_object *r = AllocTupleObject();
	r->type = TYPE_TUPLE;
	r->flags = flags;
	r->ref_count = 0;
	r->list = ptr_CreateList(num,PTR_STATIC_LIST);
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

string_object *CreateStringObject(char *bytes,NUM len,OBJECT_FLAGS flags)
{
	string_object *r = AllocStringObject();
	r->type = TYPE_STRING;
	r->flags = flags;
	r->ref_count = 0;
	r->content = bytes;
	r->len = len;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

kv_object *CreateKVObject(object *key,object *value,OBJECT_FLAGS flags)
{
	kv_object *r = AllocKVObject();
	r->type = TYPE_KV;
	r->flags = flags;
	r->ref_count = 0;
	r->key = key;
	IncRefCount(key);
	r->value = value;
	//DecRefCount(value);// ?? why did this not produce any memory leaks ?? strange
	IncRefCount(value);
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

object *CreateEmptyObject(char type,OBJECT_FLAGS flags)
{
	object *r = AllocObject();
	r->type = type;
	r->flags = flags;
	r->ref_count = 0;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

function_object *CreateFunctionObject_MAKE_FUNCTION(code_object *function_code,tuple_object *defaults,tuple_object *kw_defaults,OBJECT_FLAGS flags)
{
	function_object *r = AllocFunctionObject();
	r->type = TYPE_FUNCTION;
	r->flags = flags;
	r->ref_count = 0;
	r->func.code = function_code;
	r->func_type = FUNC_PYTHON;
	r->closure = NULL;
	r->name = function_code->name;
	IncRefCount((object*)function_code);
	if(defaults != NULL)
	{
		r->defaults = defaults;
		IncRefCount((object*)defaults);
	}
	else
		r->defaults = NULL;
	if(kw_defaults != NULL)
	{
		r->kw_defaults = kw_defaults;
		IncRefCount((object*)kw_defaults);
	}
	else
		r->kw_defaults = NULL;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

function_object *CreateFunctionObject_MAKE_CLOSURE(code_object *function_code,tuple_object *closure,tuple_object *defaults,tuple_object *kw_defaults,OBJECT_FLAGS flags)
{
	function_object *r = AllocFunctionObject();
	r->type = TYPE_FUNCTION;
	r->flags = flags;
	r->ref_count = 0;
	r->func.code = function_code;
	r->closure = closure;
	IncRefCount((object*)closure);
	r->func_type = FUNC_PYTHON;
	r->name = function_code->name;
	IncRefCount((object*)function_code);
	if(defaults != NULL)
	{
		r->defaults = defaults;
		IncRefCount((object*)defaults);
	}
	else
		r->defaults = NULL;
	if(kw_defaults != NULL)
	{
		r->kw_defaults = kw_defaults;
		IncRefCount((object*)kw_defaults);
	}
	else
		r->kw_defaults = NULL;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

function_object *CreateFunctionObject(unsigned char func_type,OBJECT_FLAGS flags)
{
	function_object *r = AllocFunctionObject();
	r->type = TYPE_FUNCTION;
	r->flags = flags;
	r->defaults = NULL;
	r->kw_defaults = NULL;
	r->closure = NULL;
	r->func.func = NULL;
	r->name = NULL;
	r->ref_count = 0;
	r->func_type = func_type;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		//DumpObject(r,0);
	}
	#endif
	return(r);
}

iter_object *CreateIterObject(OBJECT_FLAGS flags)
{
	iter_object *r = AllocIterObject();
	r->type = TYPE_ITER;
	r->flags = flags;
	//r->iter_func = iter_func;
	//r->tag = tag;
	r->ref_count = 0;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}
 
#ifdef DEBUGGING
long objects_num = 0;
long objects_max = 0;
long objects_header_total = 0;
#endif

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

void FreeBlockObject(object *obj)
{
		//if(((block_object*)obj)->iter != NULL)
		//	FreeObject(((block_object*)obj)->iter);
		//printf("closing block stack\n");
		if(((block_object*)obj)->stack != NULL)
			stack_Close(((block_object*)obj)->stack,0);
		//printf("closed block\n");
}

void FreeObject(object * obj)
{
	//assert(obj != NULL);
	if (obj == NULL)
		return;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_FREEING) > 0)
	{
		debug_printf(DEBUG_FREEING,"object to be freed\n");
		DumpObject(obj,1);
	}
	#endif
	
	if(obj->ref_count > 1)
	{
		DecRefCount(obj);
		#ifdef DEBUGGING
		if((debug_level & DEBUG_GC) > 0)
		{
			debug_printf(DEBUG_GC,"decreased object ref_count\n");
			DumpObject(obj,0);
		}
		#endif
		return;
	}
	//DecRefCount(obj);
	
	switch (obj->type)
	{
	case TYPE_BLOCK:
		#ifdef DEBUGGING
		debug_printf(DEBUG_VERBOSE_FREEING,"Freeing block object %x\n",obj);
		#endif
		//objects_header_total -= sizeof(block_object);
		FreeBlockObject(obj);
		break;
	case TYPE_REF:
		#ifdef DEBUGGING
		debug_printf(DEBUG_VERBOSE_FREEING,"Freeing Ref to %x\n",((ref_object*)obj)->ref);
		objects_header_total -= sizeof(ref_object);
		#endif
		break;
	case TYPE_ITER:
		#ifdef DEBUGGING
		debug_printf(DEBUG_VERBOSE_FREEING,"Freeing Iter %x\n",obj);
		objects_header_total -= sizeof(ref_object);
		#endif
		if(((iter_object*)obj)->block_stack != NULL)
		{	
			//printf("closing iter stack\n");
			stack_Close(((iter_object*)obj)->block_stack,0);
		}
		FreeObject((object*)((iter_object*)obj)->tag);
		break;
	case TYPE_NULL:
		// printf("freeing NULL object @%x\n",obj);
		#ifdef DEBUGGING
		objects_header_total -= sizeof(object);
		#endif
		break;
	case TYPE_NONE:
		// printf("freeing NONE object @%x\n",obj);
		#ifdef DEBUGGING
		objects_header_total -= sizeof(object);
		#endif
		break;
	case TYPE_INT:
		// printf("freeing int object @%x\n",obj);
		#ifdef DEBUGGING
		objects_header_total -= sizeof(int_object);
		#endif
		break;
	case TYPE_BINARY_FLOAT:
		// printf("freeing float object @%x\n",obj);
		#ifdef DEBUGGING
		objects_header_total -= sizeof(float_object);
		#endif
		break;
	case TYPE_KV:
		#ifdef DEBUGGING
		objects_header_total -= sizeof(kv_object);
		#endif

		FreeObject((object*)((kv_object*) obj)->key);
		FreeObject((object*)((kv_object*) obj)->value);
		break;
	case TYPE_FUNCTION:
		#ifdef DEBUGGING
		objects_header_total -= sizeof(function_object);
		#endif

		if(((function_object*) obj)->func_type == FUNC_PYTHON)
		{
			FreeObject((object*)((function_object*) obj)->func.code);
			FreeObject((object*)((function_object*) obj)->defaults);
			FreeObject((object*)((function_object*) obj)->kw_defaults);
			FreeObject((object*)((function_object*) obj)->closure);
		}else 
		if(((function_object*) obj)->func_type == FUNC_C || ((function_object*) obj)->func_type == FUNC_C_OBJ)
		{
			#ifdef DEBUGGING
			assert(mem_free(((function_object*) obj)->name));
			#else
			free(((function_object*)obj)->name);
			#endif
		}
		break;
	case TYPE_UNICODE:
		//if(obj->flags &OFLAG_ON_STACK)
		//	printf("on stack\n");
		#ifdef DEBUGGING
		debug_printf(DEBUG_VERBOSE_FREEING,"freeing unicode object @%x\n",obj);
		objects_header_total -= sizeof(unicode_object);
		#endif
		// printf("content:%s\n",((unicode_object*)obj)->value);
		// if(((unicode_object*)obj->ptr)->content != NULL)
		// if(obj->ptr != NULL)
		// mem_free(obj->ptr);
		#ifdef DEBUGGING
		assert(mem_free(((unicode_object*) obj)->value));
		#else
		free(((unicode_object*) obj)->value);
		#endif
		break;
	case TYPE_STRING:
		// printf("freeing string object @%x\n",obj);
		#ifdef DEBUGGING
		objects_header_total -= sizeof(string_object);
		#endif

		#ifdef DEBUGGING
		assert(mem_free(((string_object *) obj)->content));
		#else
		free(((string_object*) obj)->content);
		#endif
		break;
	case TYPE_TUPLE:
		 //printf("freeing tuple object @%x\n",obj);
		#ifdef DEBUGGING
		objects_header_total -= sizeof(tuple_object);
		#endif

		if (((tuple_object *)obj)->list != NULL)
		{
			if(((tuple_object *) obj)->list->num > 0)
			{
				for (NUM i = 0; i < ((tuple_object *) obj)->list->num; i++)
				{
					//printf("checking sub:%c\n",((object*)((tuple_object *) obj)->list->items[i])->type);
					//if ((((tuple_object *) obj)->list->items[i]) != NULL && !(((object*)((tuple_object *) obj)->list->items[i])->flags & OFLAG_ON_STACK))
					//{
						//printf("freeing sub:%c\n",((object*)((tuple_object *) obj)->list->items[i])->type);
						FreeObject((object*)((tuple_object *) obj)->list->items[i]);
						//if(((object*)((tuple_object *) obj->ptr)->list->items[i])->value_ptr != NULL && !(((object*)((tuple_object *) obj->ptr)->list->items[i])->flags & OFLAG_ON_STACK))
						//	FreeObject(((object*)((tuple_object *) obj->ptr)->list->items[i])->value_ptr);
					//}
					((tuple_object *) obj)->list->items[i] = NULL;
				}
			}
			ptr_CloseList(((tuple_object *) obj)->list);
		}
			//assert(mem_free(((tuple_object *) obj->ptr)->list->items));
		((tuple_object *) obj)->list = NULL;
		break;
	case TYPE_CODE:
		 //printf("freeing code object @%x\n",obj);
		#ifdef DEBUGGING
		objects_header_total -= sizeof(code_object);
		objects_header_total -= strlen(((code_object *) obj)->name) + 1;
		#endif

		#ifdef DEBUGGING
		assert(mem_free(((code_object *) obj)->name));
		#else
		free(((code_object*) obj)->name);
		#endif
		FreeObject((object*)((code_object *) obj)->code);
		FreeObject((object*)((code_object *) obj)->consts);
		FreeObject((object*)((code_object *) obj)->names);
		FreeObject((object*)((code_object *) obj)->varnames);
		FreeObject((object*)((code_object *) obj)->freevars);
		FreeObject((object*)((code_object *) obj)->cellvars);
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
	#ifdef DEBUGGING
	assert(mem_free(obj));
	objects_num--;
	#else
	free(obj);
	#endif
}

void PrintObject(object * obj)
{
	if (obj != NULL)
	{
		switch (obj->type)
		{
		case TYPE_REF:
			PrintObject(((ref_object*)obj)->ref);
			break;
		case TYPE_ELLIPSIS:
			printf("Ellipsis");
			break;
		case TYPE_NONE:
			printf("None");
			break;
		case TYPE_INT:
			printf("%d", ((int_object*)obj)->value);
			break;
		case TYPE_BINARY_FLOAT:
			printf("%7g", ((float_object*)obj)->value);
			break;
		case TYPE_KV:
			PrintObject(((kv_object*)obj)->value);
			break;
		case TYPE_UNICODE:
			printf("%s", ((unicode_object *)obj)->value);
			break;
		case TYPE_TUPLE:
			{
				tuple_object *to = (tuple_object*)obj;
				for(NUM i = 0;i<to->list->num;i++)
				{
					if(i != 0)
						printf(", ");
					object *ti = to->list->items[i];
					if(ti != NULL)
						PrintObject(ti);
				}
			}
			break;
		}
	}
}

#ifdef DEBUGGING	
void DumpObject(object * obj, char level)
{
	if (obj == NULL)
		return;
	// printf("level:%d\n",level);
	for (char i = 0; i < level; i++)
		debug_printf(DEBUG_ALL,"\t");
	debug_printf(DEBUG_ALL,"(%d)(%x)",obj->ref_count,obj);
	switch (obj->type)
	{
	case TYPE_BLOCK:
		debug_printf(DEBUG_ALL,"block object\n");
		debug_printf(DEBUG_ALL,"stack @%x\n",((block_object*)obj)->stack);
		//debug_printf(DEBUG_ALL,"iter @%x\n",((block_object*)obj)->iter);
		debug_printf(DEBUG_ALL,"start: %d\n",((block_object*)obj)->start);
		debug_printf(DEBUG_ALL,"len: %d\n",((block_object*)obj)->len);
		debug_printf(DEBUG_ALL,"ip: %d\n",((block_object*)obj)->ip);
		//printf("stack @%x\n",((block_object*)obj)->stack);
		//DumpObject(((block_object*)obj)->iter,0);
		for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"-- block object\n");
		break;
	case TYPE_ITER:
			debug_printf(DEBUG_ALL,"iter object\n");
			break;
	case TYPE_REF:
		debug_printf(DEBUG_ALL,"ref object\n");
		DumpObject(((ref_object*)obj)->ref,level+1);
		break;
	case TYPE_ELLIPSIS:
		debug_printf(DEBUG_ALL,"ellipsis	 object\n");
		break;
	case TYPE_TRUE:
		debug_printf(DEBUG_ALL,"true object\n");
		break;
	case TYPE_FALSE:
		debug_printf(DEBUG_ALL,"false object\n");
		break;
	case TYPE_NULL:
		debug_printf(DEBUG_ALL,"NULL object\n");
		break;
	case TYPE_NONE:
		debug_printf(DEBUG_ALL,"NONE object\n");
		break;
	case TYPE_BINARY_FLOAT:
		debug_printf(DEBUG_ALL,"float object: %7g\n", ((float_object*)obj)->value);
		break;
	case TYPE_INT:
		debug_printf(DEBUG_ALL,"int object: %d\n", ((int_object*)obj)->value);
		break;
	case TYPE_KV:
		debug_printf(DEBUG_ALL,"kv object\n");
		DumpObject(((kv_object*)obj)->key,level);
		DumpObject(((kv_object*)obj)->value,level + 1);
		break;
	case TYPE_FUNCTION:
		if(((function_object*)obj)->func_type == FUNC_C || ((function_object*)obj)->func_type == FUNC_C_OBJ)
			debug_printf(DEBUG_ALL,"function object: %s\n",((function_object*)obj)->name);
		if(((function_object*)obj)->func_type == FUNC_PYTHON)
		{
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"Python function\n");
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"code:\n");
			DumpObject((object*)((function_object*)obj)->func.code,level + 1);
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"defaults:\n");
			DumpObject((object*)((function_object*)obj)->defaults,level + 1);
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"kw_defaults:\n");
			DumpObject((object*)((function_object*)obj)->kw_defaults,level + 1);
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"closure:\n");
			DumpObject((object*)((function_object*)obj)->closure,level + 1);
		}else
		if(((function_object*)obj)->func_type == FUNC_C)
		{
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"C function\n");
		}else
		if(((function_object*)obj)->func_type == FUNC_C_OBJ)
		{
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"C Object function\n");
		}
		break;
	case TYPE_UNICODE:
		debug_printf(DEBUG_ALL,"unicode object: %s\n", ((unicode_object*)obj)->value);
		break;
	case TYPE_STRING:
		debug_printf(DEBUG_ALL,"string object: ");
		for (NUM i = 0; i < ((string_object *) obj)->len; i++)
			debug_printf(DEBUG_ALL,"%02x ", (unsigned char)((string_object *) obj)->content[i]);
		debug_printf(DEBUG_ALL,"\n");
		break;
	case TYPE_TUPLE:
		debug_printf(DEBUG_ALL,"tuple object");
		if (((tuple_object *) obj)->list != NULL && ((tuple_object *) obj)->list->num > 0)
		{
			debug_printf(DEBUG_ALL," contains %d items\n", ((tuple_object *) obj)->list->num);
			for (NUM i = 0; i < ((tuple_object *) obj)->list->num; i++)
			{
				if (((tuple_object*) obj)->list->items[i] != NULL)
				{

					if ((((object*)((tuple_object*) obj)->list->items[i])->flags & OFLAG_TUPLE_PTR) > 0)
						debug_printf(DEBUG_ALL,"%d->",i);
					else
						debug_printf(DEBUG_ALL,"%d  ",i);
					DumpObject((object*)((tuple_object*) obj)->list->items[i], level + 1);
				}
			}
		}
		else
		{
			debug_printf(DEBUG_ALL,"\n");
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"empty tuple\n");
		}
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"-- tuple object\n");
		break;
	case TYPE_CODE:
		debug_printf(DEBUG_ALL,"code object\n");
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"name: %s\n", ((code_object *) obj)->name);
	if((debug_level & DEBUG_FULL_DUMP) > 0)
		{
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"flags:%d\n",((code_object *) obj)->flags);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"nlocals:%d\n",((code_object *) obj)->nlocals);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"argcount:%d\n",((code_object *) obj)->argcount);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"kwonlyargcount:%d\n",((code_object *) obj)->kwonlyargcount);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"code:\n");
		DumpObject(((code_object *) obj)->code, level + 1);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"consts:\n");
		DumpObject(((code_object *) obj)->consts, level + 1);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"names:\n");
		DumpObject(((code_object *) obj)->names, level + 1);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"varnames:\n");
		DumpObject(((code_object *) obj)->varnames, level + 1);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"freevars:\n");
		DumpObject(((code_object *) obj)->freevars, level + 1);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"cellvars:\n");
		DumpObject(((code_object *) obj)->cellvars, level + 1);
		}
		// for(int i=0;i<level;i++)
		// printf("\t");
		// printf("lnotab:\n");
		// DumpObject(((code_object*)obj->ptr)->lnotab,level+1);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"-- code object\n");
		break;
	default:
		debug_printf(DEBUG_ALL,"object type is unknown:%c\n", obj->type);
	}
}
#endif

#ifdef DEBUGGING
char *DumpObjectXml(object * obj, char level)
{
	if (obj == NULL)
		return(NULL);
	// printf("level:%d\n",level);
	for (char i = 0; i < level; i++)
		printf("\t");
	printf("(%d)(%x)",obj->ref_count,obj);
	switch (obj->type)
	{
	case TYPE_BLOCK:
		printf("block object\n");
		printf("stack @%x\n",((block_object*)obj)->stack);
		//printf("iter @%x\n",((block_object*)obj)->iter);
		printf("start: %d\n",((block_object*)obj)->start);
		printf("len: %d\n",((block_object*)obj)->len);
		printf("ip: %d\n",((block_object*)obj)->ip);
		//printf("stack @%x\n",((block_object*)obj)->stack);
		
		for (char i = 0; i < level; i++)
				printf("\t");
		printf("-- block object\n");
		
		break;
	case TYPE_REF:
		printf("ref object\n");
		DumpObject(((ref_object*)obj)->ref,level+1);
		break;
	case TYPE_ELLIPSIS:
		printf("ellipsis	 object\n");
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
	case TYPE_BINARY_FLOAT:
		printf("float object: %7g\n", ((float_object*)obj)->value);
		break;
	case TYPE_INT:
		printf("int object: %d\n", ((int_object*)obj)->value);
		break;
	case TYPE_KV:
		printf("kv object\n");
		DumpObject(((kv_object*)obj)->key,level);
		DumpObject(((kv_object*)obj)->value,level + 1);
		break;
	case TYPE_FUNCTION:
		printf("function object: %s\n",((function_object*)obj)->name);
		if(((function_object*)obj)->func_type == FUNC_PYTHON)
		{
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("Python function\n");
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("code:\n");
			DumpObject((object*)((function_object*)obj)->func.code,level + 1);
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("defaults:\n");
			DumpObject((object*)((function_object*)obj)->defaults,level + 1);
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("kw_defaults:\n");
			DumpObject((object*)((function_object*)obj)->kw_defaults,level + 1);
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("closure:\n");
			DumpObject((object*)((function_object*)obj)->closure,level + 1);
		}else
		if(((function_object*)obj)->func_type == FUNC_C)
		{
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("C function\n");
		}else
		if(((function_object*)obj)->func_type == FUNC_C_OBJ)
		{
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("C Object function\n");
		}
		
		break;
	case TYPE_UNICODE:
		printf("unicode object: %s\n", ((unicode_object*)obj)->value);
		break;
	case TYPE_STRING:
		printf("string object: ");
		for (NUM i = 0; i < ((string_object *) obj)->len; i++)
			printf("%02x ", (unsigned char)((string_object *) obj)->content[i]);
		printf("\n");
		break;
	case TYPE_TUPLE:
		printf("tuple object");
		if (((tuple_object*) obj)->list != NULL && ((tuple_object *) obj)->list->num > 0)
		{
			printf(" contains %d items\n", ((tuple_object *) obj)->list->num);
			for (NUM i = 0; i < ((tuple_object *) obj)->list->num; i++)
			{
				if (((tuple_object*) obj)->list->items[i] != NULL)
				{

					if ((((object*)((tuple_object*) obj)->list->items[i])->flags & OFLAG_TUPLE_PTR) > 0)
						printf("%d->",i);
					else
						printf("%d  ",i);
					DumpObject((object*)((tuple_object*) obj)->list->items[i], level + 1);
				}
			}
		}
		else
		{
			printf("\n");
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("empty tuple\n");
		}
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("-- tuple object\n");
		break;
	case TYPE_CODE:
		printf("code object\n");
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("name: %s\n", ((code_object *) obj)->name);
	if((debug_level & DEBUG_FULL_DUMP) > 0)
		{
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("flags:%d\n",((code_object *) obj)->flags);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("nlocals:%d\n",((code_object *) obj)->nlocals);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("argcount:%d\n",((code_object *) obj)->argcount);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("kwonlyargcount:%d\n",((code_object *) obj)->kwonlyargcount);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("code:\n");
		DumpObject(((code_object *) obj)->code, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("consts:\n");
		DumpObject(((code_object *) obj)->consts, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("names:\n");
		DumpObject(((code_object *) obj)->names, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("varnames:\n");
		DumpObject(((code_object *) obj)->varnames, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("freevars:\n");
		DumpObject(((code_object *) obj)->freevars, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("cellvars:\n");
		DumpObject(((code_object *) obj)->cellvars, level + 1);
		}
		// for(int i=0;i<level;i++)
		// printf("\t");
		// printf("lnotab:\n");
		// DumpObject(((code_object*)obj->ptr)->lnotab,level+1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("-- code object\n");
		break;
	default:
		printf("object type is unknown:%c\n", obj->type);
	}
	return(NULL);
}
#endif

BOOL object_compare(object *a,object *b)
{
	//return(CompareOp(a,b,2));
	//printf("comparing objects\n");
	#ifdef DEBUGGING
	assert(a != NULL && b != NULL);
	#endif
	if (a == NULL && b == NULL)
		return(0);
	if(a== NULL || b == NULL)
		return (1);
	if(a->type != b->type)
		return(1);
	switch (a->type)
	{
	case TYPE_NULL:
		return(1);
		break;
	case TYPE_NONE:
		return(1);
		break;
	case TYPE_KV:
		return(object_compare(((kv_object*)a)->key,((kv_object*)b)->key) && object_compare(((kv_object*)a)->value,((kv_object*)b)->value));
	case TYPE_INT:
		return( !(((int_object*)a)->value == ((int_object*)b)->value));
		break;
	case TYPE_BINARY_FLOAT:
		return( !(((float_object*)a)->value == ((float_object*)b)->value));
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
	#ifdef DEBUGGING
	assert(obj != NULL);
	#endif
	if (obj == NULL)
		return(NULL);

	
	switch (obj->type)
	{
	case TYPE_NULL:
	case TYPE_NONE:
		{	
		object *r = AllocObject();
		r->type = obj->type;
		r->ref_count  = 0;
		r->flags = obj->flags;
		return(r);
		}
		break;
	case TYPE_INT:
		{
		int_object *r = AllocIntObject();
		r->type = obj->type;
		r->ref_count  = 0;
		r->flags = obj->flags;
		r->value = ((int_object*)obj)->value;
		return((object*)r);
		}
		break;
	case TYPE_BINARY_FLOAT:
		{
		float_object *r = AllocFloatObject();
		r->type = obj->type;
		r->ref_count  = 0;
		r->flags = obj->flags;
		r->value = ((float_object*)obj)->value;
		return((object*)r);
		}
		break;
	case TYPE_UNICODE:
	   {
		unicode_object *r = AllocUnicodeObject();
		r->type = obj->type;
		r->ref_count  = 0;
		r->flags = obj->flags;
		r->value = str_Copy(((unicode_object*)obj)->value);
		return((object*)r);
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
	return((object*)CreateRefObject(obj,obj->flags));

	case TYPE_FUNCTION:
	//code_object *c = AllocCodeObject();
	//c->name = str_Copy(((code_object*)obj)->name);
	//c-> = str_Copy(((code_object*)obj)->name);
	//add reference object to decrease memory usage for code objects
	//reference objects refs need no freeing
	return((object*)CreateRefObject(obj,obj->flags));

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

void IncRefCount(object *obj)
{
	obj->ref_count++;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_GC) > 0)
	{
		debug_printf(DEBUG_GC,"object has gained a ref \n");
		DumpObject(obj,0);
	}
	#endif
}

void DecRefCountGC(object *obj,ptr_list *gc)
{
	if(obj->ref_count == 1)
	 {
		obj->ref_count--;
		if(!ptr_Contains(gc,obj))
		{
			#ifdef DEBUGGING
			if((debug_level & DEBUG_GC) > 0)
			{
				debug_printf(DEBUG_GC,"object has no refs anymore -> put into gc\n");
				DumpObject(obj,0);
			}
			#endif
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
		//obj->ref_count--;
		#ifdef DEBUGGING
		if((debug_level & DEBUG_GC) > 0)
		{
			debug_printf(DEBUG_GC,"object has no refs anymore -> freeing\n");
			DumpObject(obj,0);
		}
		#endif
		FreeObject(obj);
		//ptr_Queue(gc,obj);
		return;
	 }
	obj->ref_count--;
}

BOOL HasNoRefs(object *obj)
{
return(!obj->ref_count);
}

BOOL HasRefs(object *obj)
{
return(obj->ref_count);
}

void AppendDictItem(object * tuple,object *key,object *value)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if(((tuple_object *) tuple)->list == NULL)
		((tuple_object *) tuple)->list = ptr_CreateList(0,0);
	//object *di = CopyObject(key);//TODO needed??
	//kv_object *kv = ConvertToKVObjectValued(di,value);
	kv_object *kv = ConvertToKVObjectValued(key,value);
	#ifdef DEBUGGING
	if((debug_level & DEBUG_LISTS) > 0)
	{
	debug_printf(DEBUG_LISTS,"appended to \n");
	DumpObject(tuple,0);
	debug_printf(DEBUG_LISTS,"this\n");
	//DumpObject(di,0);
	DumpObject(key,0);
	}
	#endif
	ptr_Push(((tuple_object*) tuple)->list,kv);
	IncRefCount((object*)kv);

}

void AppendItem(object *tuple,object *value)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if(((tuple_object *) tuple)->list == NULL)
		((tuple_object *) tuple)->list = ptr_CreateList(0,0);
	ptr_Push(((tuple_object *) tuple)->list,value);
	IncRefCount(value);
}

void SetDictItem(object *tuple,object *key,object *value)
{
	INDEX index = GetDictItemIndex(tuple,key);
	#ifdef DEBUGGING
	if((debug_level & DEBUG_LISTS) > 0)
	{
		debug_printf(DEBUG_LISTS,"setting\n");
		DumpObject(tuple,0);
		debug_printf(DEBUG_LISTS,"key\n");
		DumpObject(key,0);
		debug_printf(DEBUG_LISTS,"value\n");
		DumpObject(value,0);
	}
	#endif
	
	if(index != -1)
		{
			//printf("found index\n");
			//SetItem(tuple,index,value);
			object * k = GetItem(tuple,index);
			if(k->type == TYPE_KV)
			{
				object *old =((kv_object*) k)->value;
				if(value != NULL)
				{
					((kv_object*) k)->value = value;
					IncRefCount(value);
				}
				else
					((kv_object*) k)->value = NULL;
				if(old != NULL)
					FreeObject(old);
			}
			else 
			{
					kv_object *kv = ConvertToKVObjectValued(k,value);
					//FreeObject(k);
					SetItem(tuple,index,(object*)kv);
					//IncRefCount(kv);
			}
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

void SetDictItemByIndex(object *tuple,INDEX index,object *value)
{
	if(index != -1)
		{
			//printf("found index\n");
			//SetItem(tuple,index,value);
			object * k = GetItem(tuple,index);
			if(k->type == TYPE_KV)
			{
				object *old = ((kv_object*) k)->value;
				if(value != NULL)
				{
					((kv_object*) k)->value = value;
					IncRefCount(value);
				}
				else
					((kv_object*) k)->value = NULL;
				if(old != NULL)
					FreeObject(old);
			}
			else 
			{
					kv_object *kv = ConvertToKVObjectValued(k,value);
					//FreeObject(k);
					SetItem(tuple,index,(object*)kv);
					//IncRefCount(kv);
			}
		}

}

void ClearDictValues(object *tuple)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL)
		return;
	//DumpObject(tuple,0);
	//printf("clear dict values\n");
	for (NUM i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{
		if (((tuple_object *) tuple)->list->items[i] != NULL && ((object*)((tuple_object *) tuple)->list->items[i])->type != TYPE_KV)
			continue;
		else
		{
			kv_object *kv = (kv_object*)((tuple_object *) tuple)->list->items[i];
			((tuple_object *) tuple)->list->items[i] = kv->key;
			IncRefCount(kv->key);
			FreeObject((object*)kv);
		}
	}
	//printf("cleared dict values\n");
	//DumpObject(tuple,0);
}

object *GetDictItemByIndex(object *tuple,INDEX index)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL || index >= ((tuple_object *) tuple)->list->num || index < 0)
		return(NULL);
	return(((kv_object*)GetItem(tuple,index))->value);
}

NUM GetTupleLen(object *tuple)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL || ((tuple_object *) tuple)->list->num == 0)
		return(0);
	return(((tuple_object *) tuple)->list->num);
}

object *GetDictItem(object *tuple,object *key)
{
	INDEX index = GetDictItemIndex(tuple,key);
	if(index != -1)
		{
			return(((kv_object*)GetItem(tuple,index))->value);
		}
	return(NULL);
}

INDEX GetDictItemIndex(object *tuple,object *key)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL)
		return (-1);
	 //printf("checking in %d tuple items for\n",((tuple_object*)tuple->ptr)->list->num);
	//printf("getdictitemindex\n");
	//DumpObject(key,0);
	//DumpObject(tuple,0);
	for (NUM i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{
		 //printf("checking tuple:%d\n",i);
		// if(((tuple_object*)tuple->ptr)->items[i]->type == TYPE_UNICODE)
		// printf("checking %s against:
		// %s\n",name,(char*)((tuple_object*)tuple->ptr)->items[i]->ptr);
		if (((tuple_object *) tuple)->list->items[i] != NULL && ((object*)((tuple_object *) tuple)->list->items[i])->type != TYPE_KV)
		{
			if ( !object_compare( ((tuple_object *) tuple)->list->items[i],key))
				return (i);
		}
		else
		{
			if ( !object_compare( ((kv_object*)((tuple_object *) tuple)->list->items[i])->key,key))
				return (i);
		}
	}
	return (-1);
}

INDEX GetItemIndex(object *tuple,object *obj)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL)
		return (-1);

	for (NUM i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{

		if (((tuple_object *) tuple)->list->items[i] != NULL && ((object*)((tuple_object *) tuple)->list->items[i])->type != TYPE_KV)
		{
			if ( !object_compare( ((tuple_object *) tuple)->list->items[i],obj))
				return (i);
		}
		else
		{
			if ( !object_compare( ((kv_object*)((tuple_object *) tuple)->list->items[i])->value,obj))
				return (i);
		}
	}
	return (-1);
}

void ConvertToDict(object *tuple)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL || ((tuple_object *) tuple)->list->num == 0)
		return;

	for (NUM i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{

		if (((tuple_object *) tuple)->list->items[i] != NULL && ((object*)((tuple_object *) tuple)->list->items[i])->type != TYPE_KV)
		{
			((tuple_object *) tuple)->list->items[i] = ConvertToKVObject(((tuple_object *) tuple)->list->items[i]);
			IncRefCount(((tuple_object *) tuple)->list->items[i]);
			DecRefCount(((kv_object*)((tuple_object *) tuple)->list->items[i])->key);
		}
		else
			continue;
	}
}

INDEX GetItemIndexByName(object * tuple, char *name)
{
	//printf("checking for correct type\n");
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL)
		return (-1);
	 //printf("checking in %d tuple items for %s\n",((tuple_object*)tuple)->list->num,name);
	for (NUM i = 0; i < ((tuple_object *) tuple)->list->num; i++)
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

object *GetDictItemByName(object *tuple,char *name)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL)
		return (NULL);
	for (NUM i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{		
		if (((tuple_object *) tuple)->list->items[i] == NULL)
			continue;
		object *item = (object*)((tuple_object *) tuple)->list->items[i];
		if(item->type == TYPE_KV)
		{
			object *key = (object*)((kv_object*)item)->key;
			object *value = (object*)((kv_object*)item)->value;

			if (key->type == TYPE_UNICODE	&& !strcmp(((unicode_object*)key)->value, name)) 
			{
				return (value);
			}
		}
	}
	return (NULL);
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

	for (NUM i = 0; i < ((tuple_object *) tuple)->list->num; i++)
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

void SetItem(object * tuple, INDEX index, object * obj)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if (index >= ((tuple_object *) tuple)->list->num || index < 0)
		return;
	object *old = ((tuple_object *) tuple)->list->items[index];	
	if(obj!=NULL)
	{
		((tuple_object *) tuple)->list->items[index] = obj;
		IncRefCount(obj);
	// obj->flags ^= OFLAG_ON_STACK;
	}
	else
		((tuple_object *) tuple)->list->items[index] = NULL;

	if(old != NULL)
	{
		FreeObject(old);
	}		
}

void DeleteItem(object * tuple, INDEX index)
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

object *GetItem(object * tuple, INDEX index)
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


	for (NUM i = 0; i < ((tuple_object *) tuple)->list->num; i++)
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

long ReadLong(stream *f)
{

	long r = 0;

	// char *b = (char*)mem_malloc(4);
	stream_Read(f,&r, 4);
	//r = (long) *stream_Read(f, 4);
	//printf("stream read:%
	// r = *(long*)b;
	//free(b);
	return (r);
}

/*
#define CONV_PREC 4
float ConvertDouble(double d)
{
long g[CONV_PREC];

g[0] = 0;
int e = (int)d;
float ef = (float)e;
float r = (float)((int)d);
printf("d:%f,e:%d,r:%f\n",d,e,r);

for(int i = 1;i<CONV_PREC;i++)
{
	double x = (double)long_pow(10,i);
	printf("x:%f,d-e:%f\n",x,(d-ef));
	float gf = x*(d-ef);
	g[i] = (long)gf;
}
for(int i = 1;i<CONV_PREC;i++)
{
printf("g[%d]:%d\n",i,g[i]);

}
for(int i = CONV_PREC-1;i>0;i--)
{
long d = (g[i-1]*long_pow(10,i-2));
printf("d:%d\n",d);

g[i] = g[i] - d;
printf("g[%d]:%d\n",i,g[i]);
}
for(int i = 1;i<CONV_PREC;i++)
{
float a = (float) g[i] /(float) long_pow(10,i);
printf("a:%f\n",a);
r+= a;

}


return(r);
}
*/

FLOAT ReadFloat(stream *f)
{
	float r = 0;
	double t = 0;
	//int g = 0;
	stream_Read(f, &t,8);
	//int read = fread(&t, 4, 1, f);
	//read = fread(&r, 4, 1, f);
	//r = ConvertDouble(t);
	r = (FLOAT)t;
	//printf("r:%f\n",r);
	//printf("r:%f,t:%f\n",r,t);
	return (r);
}

char ReadChar(stream *f)
{
	char r = 0;
	stream_Read(f,&r,1);
	return (r);
}

object *ReadObject(stream *f)
{
	#ifdef DEBUGGING
	objects_num++;
	// printf("increasing objects num to %d\n",objects_num);
	if (objects_num > objects_max)
		objects_max = objects_num;
	#endif
	OBJECT_TYPE type = ReadChar(f);

	#ifdef DEBUGGING
	debug_printf(DEBUG_CREATION,"reading object with type:%c\n",type);
	#endif
	// long magic = ReadLong(f);

	switch (type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_ELLIPSIS:
		{
		// printf("allocating empty object\n");
		object *obj = AllocObject();
		obj->type = type;
		obj->flags = 0;
		obj->ref_count = 1;
		// printf("allocated empty object @%x\n",obj);
		#ifdef DEBUGGING
		objects_header_total += sizeof(object);
		#endif
		return (obj);
		}
	case TYPE_CODE:
       {	
		// printf("reading code chunk\n");
		#ifdef DEBUGGING
		objects_header_total += sizeof(code_object);
		#endif
		// printf("allocated object @%x\n",obj);
		code_object *co = AllocCodeObject();
		co->flags = 0;
		co->type = TYPE_CODE;
		co->ref_count = 1;

		// printf("co_ptr=%x\n",(unsigned long)co);
		co->argcount = (NUM)ReadLong(f);
		co->kwonlyargcount = (NUM)ReadLong(f);
		co->nlocals = (NUM)ReadLong(f);
		co->stacksize = (NUM)ReadLong(f);
		co->co_flags = ReadLong(f);
		// printf("argcount:%d,kwonlyargcount:%d,nlocals:%d,stacksize:%d,flags:%d\n",co->argcount,co->kwonlyargcount,co->nlocals,co->stacksize,co->flags);
		co->code = ReadObject(f);
		co->consts = ReadObject(f);
		co->names = ReadObject(f);
		co->varnames = ReadObject(f);
		co->freevars = ReadObject(f);
		co->cellvars = ReadObject(f);
		ConvertToDict(co->freevars); //TODO only needed as long deref and closure opcodes are not implemented using references
		ConvertToDict(co->cellvars);
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
		#ifdef DEBUGGING
		objects_header_total += strlen(co->name) + 1;
		#endif
		FreeObject(tmp_name);
		
		//long firstlineno = ReadLong(f);
		ReadLong(f);//firstlineno
		//co->firstlineno = ReadLong(f);
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
		return((object*)co);
		}
		break;
	case TYPE_STRING:
		{
		string_object *obj = AllocStringObject();
		#ifdef DEBUGGING
		objects_header_total += sizeof(string_object);
		#endif
		// printf("allocated object @%x\n",obj);
		obj->flags = 0;
		obj->ref_count = 1;

		NUM n;
		// printf("reading string chunk\n");
		n = (NUM)ReadLong(f);
		if (n > 0)
		{
			#ifdef DEBUGGING
			char *string = (char *)mem_malloc(n, "ReadObject() string");
			#else
			char *string = (char *)malloc(n);
			#endif

			// printf("str_ptr=%x\n",(unsigned long)string);
			stream_Read(f,string, n);

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
		return((object*)obj);
		}
		break;
	case TYPE_TUPLE:
		{
		// printf("reading tuple\n");
		INT n = (INT)ReadLong(f);
		tuple_object *to = AllocTupleObject();
		to->ref_count = 1;

		// printf("items_num:%d\n",n);
		if (n > 0)
		{
			//to->items =
			//	(object **) mem_malloc(n * sizeof(object *),
			//						   "ReadObject() to->items");
			to->list = ptr_CreateList(n,PTR_STATIC_LIST);
			for (NUM i = 0; i < n; i++)
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
		return((object*)to);
		}
		break;
	case TYPE_UNICODE:
		{
		// printf("reading unicode\n");
		unicode_object *obj = AllocUnicodeObject();
		obj->ref_count = 1;

		NUM n = (NUM)ReadLong(f);
		// printf("len:%d\n",n);
		#ifdef DEBUGGING
		char *unicode_string = (char *)mem_malloc(n + 1, "ReadObject() unicode_string");
		#else
		char *unicode_string = (char *)malloc(n + 1);
		#endif
		memset(unicode_string, 0, n + 1);
		stream_Read(f,unicode_string, n);
		//printf("read unicode string:%s\n",unicode_string);
		// uo->len = n;
		obj->flags = 0;
		obj->value = unicode_string;
		obj->type = TYPE_UNICODE;
		return((object*)obj);
		}
		break;
	case TYPE_BINARY_FLOAT:
		{
		float_object *obj = AllocFloatObject();
		FLOAT n = (FLOAT)ReadFloat(f);
		obj->ref_count = 1;
		obj->flags = 0;
		obj->value = n;
		//printf("read float:%7g\n",obj->value);
		obj->type = TYPE_BINARY_FLOAT;
		return((object*)obj);
		}
		break;
	case TYPE_INT:
		{
		// printf("reading int\n");
		int_object *obj = AllocIntObject();
		INT n = (INT)ReadLong(f);
		obj->ref_count = 1;
		obj->flags = 0;
		obj->value = n;
		obj->type = TYPE_INT;
		return((object*)obj);
		}
		break;
	default:
		{
		object *obj = AllocObject();
		obj->ref_count = 1;
		obj->type = TYPE_UNKNOWN;
		#ifdef DEBUGGING
		debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", type);
		#endif
		return(obj);
		}
	}
	// printf("read object\n"); 
	return (NULL);
}



