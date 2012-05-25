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

//global non value objects 
object *obj_NULL = NULL;
object *obj_NONE = NULL;
object *obj_TRUE = NULL;
object *obj_FALSE = NULL;
object *obj_ELLIPSIS = NULL;
object *obj_UNKNOWN = NULL;


void obj_Init(void)
{
	obj_NULL = AllocObject();
	obj_NULL->type = TYPE_NULL;
	obj_NULL->ref_count = 1;
	#ifdef USE_LOCKING
	obj_NULL->lock_count = 1;
	#endif
	obj_NONE = AllocObject();
	obj_NONE->type = TYPE_NONE;
	obj_NONE->ref_count = 1;
	#ifdef USE_LOCKING
	obj_NONE->lock_count = 1;
	#endif
	obj_TRUE = AllocObject();
	obj_TRUE->type = TYPE_TRUE;
	obj_TRUE->ref_count = 1;
	#ifdef USE_LOCKING
	obj_TRUE->lock_count = 1;
	#endif
	obj_FALSE = AllocObject();
	obj_FALSE->type = TYPE_FALSE;
	obj_FALSE->ref_count = 1;
	#ifdef USE_LOCKING
	obj_FALSE->lock_count = 1;
	#endif
	obj_ELLIPSIS = AllocObject();
	obj_ELLIPSIS->type = TYPE_ELLIPSIS;
	obj_ELLIPSIS->ref_count = 1;
	#ifdef USE_LOCKING
	obj_ELLIPSIS->lock_count = 1;
	#endif
	obj_UNKNOWN = AllocObject();
	obj_UNKNOWN->type = TYPE_UNKNOWN;
	obj_UNKNOWN->ref_count = 1;
	#ifdef USE_LOCKING
	obj_UNKNOWN->lock_count = 1;
	#endif
}

void obj_Close(void)
{
	#ifdef USE_DEBUGGING
	if(debug_level & DEBUG_VERBOSE_TESTS)
	{
		DumpObject(obj_NULL,0);
		DumpObject(obj_NONE,0);
		DumpObject(obj_TRUE,0);
		DumpObject(obj_FALSE,0);
		DumpObject(obj_ELLIPSIS,0);
		DumpObject(obj_UNKNOWN,0);
	}
	#endif
	gc_DecRefCount(obj_NULL);
	gc_DecRefCount(obj_NONE);
	gc_DecRefCount(obj_TRUE);
	gc_DecRefCount(obj_FALSE);
	gc_DecRefCount(obj_ELLIPSIS);	
	gc_DecRefCount(obj_UNKNOWN);
}

block_object *AllocBlockObject(void)
{	
	#ifdef USE_DEBUGGING
	return((block_object*)mem_malloc(sizeof(block_object), "AllocBlockObject() return"));
	#else
	return((block_object*)malloc(sizeof(block_object)));
	#endif
}

kv_object *AllocKVObject(void)
{
	#ifdef USE_DEBUGGING
	return((kv_object*) mem_malloc(sizeof(kv_object), "AllocKVObject() return"));
	#else
	return((kv_object*) malloc(sizeof(kv_object)));
	#endif
}

object *AllocObject(void)
{
	#ifdef USE_DEBUGGING
	return ((object*) mem_malloc(sizeof(object), "AllocObject() return"));
	#else
	return((object*) malloc(sizeof(object)));
	#endif
}

code_object *AllocCodeObject(void)
{
	#ifdef USE_DEBUGGING
	return((code_object*) mem_malloc(sizeof(code_object), "AllocCodeObject() return"));
	#else
	return((code_object*) malloc(sizeof(code_object)));
	#endif
}

class_object *AllocClassObject(void)
{
	#ifdef USE_DEBUGGING
	return((class_object*) mem_malloc(sizeof(class_object), "AllocClassObject() return"));
	#else
	return((class_object*) malloc(sizeof(class_object)));
	#endif
}

class_instance_object *AllocClassInstanceObject(void)
{
	#ifdef USE_DEBUGGING
	return((class_instance_object*) mem_malloc(sizeof(class_instance_object), "AllocClassInstanceObject() return"));
	#else
	return((class_instance_object*) malloc(sizeof(class_instance_object)));
	#endif
}

function_object *AllocFunctionObject(void)
{
	#ifdef USE_DEBUGGING
	return((function_object*) mem_malloc(sizeof(function_object),"AllocFunctionObject() return"));
	#else
	return((function_object*) malloc(sizeof(function_object)));
	#endif
}

cfunction_object *AllocCFunctionObject(void)
{
	#ifdef USE_DEBUGGING
	return((cfunction_object*) mem_malloc(sizeof(cfunction_object),"AllocCFunctionObject() return"));
	#else
	return((cfunction_object*) malloc(sizeof(cfunction_object)));
	#endif
}

method_object *AllocMethodObject(void)
{
	#ifdef USE_DEBUGGING
	return((method_object*) mem_malloc(sizeof(method_object),"AllocMethodObject() return"));
	#else
	return((method_object*) malloc(sizeof(method_object)));
	#endif
}

string_object *AllocStringObject(void)
{
	#ifdef USE_DEBUGGING
	return((string_object*) mem_malloc(sizeof(string_object), "AllocStringObject() return"));
	#else
	return((string_object*) malloc(sizeof(string_object)));
	#endif
}

tuple_object *AllocTupleObject(void)
{
	#ifdef USE_DEBUGGING
	return((tuple_object*) mem_malloc(sizeof(tuple_object), "AllocTupleObject() return"));
	#else
	return((tuple_object*) malloc(sizeof(tuple_object)));
	#endif
}

unicode_object *AllocUnicodeObject(void) 
{
	#ifdef USE_DEBUGGING
    return((unicode_object*) mem_malloc(sizeof(unicode_object), "AllocUnicodeObject() return")); 
	#else
    return((unicode_object*) malloc(sizeof(unicode_object))); 
	#endif
}

#ifdef USE_LOCKING
ref_object *AllocProxyObject(void)
{
	#ifdef USE_DEBUGGING
    return((proxy_object*)mem_malloc(sizeof(proxy_object), "AllocProxyObject() return")); 
	#else
    return((proxy_object*) malloc(sizeof(proxy_object))); 
	#endif
}
#endif

tag_object *AllocTagObject(void)
{
	#ifdef USE_DEBUGGING
    return((tag_object*)mem_malloc(sizeof(tag_object), "AllocTagObject() return")); 
	#else
    return((tag_object*)malloc(sizeof(tag_object))); 
	#endif
}

int_object *AllocIntObject(void) 
{
	#ifdef USE_DEBUGGING
    return((int_object*) mem_malloc(sizeof(int_object), "AllocIntObject() return")); 
	#else
    return((int_object*) malloc(sizeof(int_object))); 
	#endif
}

float_object *AllocFloatObject(void) 
{
	#ifdef USE_DEBUGGING
    return((float_object*) mem_malloc(sizeof(float_object), "AllocFloatObject() return")); 
	#else
	return((float_object*) malloc(sizeof(float_object))); 
	#endif
}

iter_object *AllocIterObject(void) 
{
	#ifdef USE_DEBUGGING
	return((iter_object*) mem_malloc(sizeof(iter_object), "AllocIterObject() return"));
	#else
	return((iter_object*) malloc(sizeof(iter_object)));
	#endif
}

#ifdef USE_LOCKING
ref_object *CreateProxyObject(void)
{
	proxy_object *r = AllocProxyObject();
	r->type = TYPE_PROXY;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->ref = NULL;
	r->is_cached = NULL;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created proxy object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}
#endif

tag_object *CreateTagObject(void *tag)
{
	tag_object *r = AllocTagObject();
	r->type = TYPE_TAG;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->tag = tag;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created tag object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

int_object *CreateIntObject(long value)
{
	int_object *r = AllocIntObject();
	r->type = TYPE_INT;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->value = value;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

float_object *CreateFloatObject(float value)
{
	float_object *r = AllocFloatObject();
	r->type = TYPE_BINARY_FLOAT;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->value = value;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

unicode_object *CreateUnicodeObject(char *value)
{
	unicode_object *r = AllocUnicodeObject();
	r->type = TYPE_UNICODE;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->value = value;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

tuple_object *CreateTuple(NUM num)
{
	tuple_object *r = AllocTupleObject();
	r->type = TYPE_TUPLE;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->ptr = 0;
	r->list = ptr_CreateList(num,PTR_STATIC_LIST);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

string_object *CreateStringObject(char *bytes,NUM len)
{
	string_object *r = AllocStringObject();
	r->type = TYPE_STRING;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->content = bytes;
	r->len = len;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

kv_object *CreateKVObject(object *key,object *value)
{
	kv_object *r = AllocKVObject();
	r->type = TYPE_KV;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->key = key;
	gc_IncRefCount(key);
	r->value = value;
	gc_IncRefCount(value);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

object *CreateEmptyObject(char type)
{
	//printf("creating empty object reference:%c\n",type);
	switch(type)
	{
		case TYPE_NONE:
			//printf("ref count:%d\n",obj_NONE->ref_count);
			return(obj_NONE);
		case TYPE_NULL:
			//printf("ref count:%d\n",obj_NULL->ref_count);
			return(obj_NULL);
		case TYPE_TRUE:
			//printf("ref count:%d\n",obj_TRUE->ref_count);
			return(obj_TRUE);
		case TYPE_FALSE:
			//printf("ref count:%d\n",obj_FALSE->ref_count);
			return(obj_FALSE);
		case TYPE_ELLIPSIS:
			//printf("ref count:%d\n",obj_ELLIPSIS->ref_count);
			return(obj_ELLIPSIS);
		case TYPE_UNKNOWN:
			//printf("ref count:%d\n",obj_UNKNOWN->ref_count);
			return(obj_UNKNOWN);
	}
	return(obj_UNKNOWN);
}

function_object *CreateFunctionObject(code_object *function_code,tuple_object *defaults,tuple_object *kw_defaults,tuple_object *closure)
{
	function_object *r = AllocFunctionObject();
	r->type = TYPE_FUNCTION;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->func = function_code;
	gc_IncRefCount((object*)function_code);
	
	if(GetTupleLen(function_code->varnames) > 0)
	{
		if(defaults != NULL || kw_defaults != NULL)
			ClearDictValues(function_code->varnames);
		if(defaults!=NULL)
		{
			//printf("loading defaults\n");
			NUM locals_num = function_code->argcount;
			for(NUM i = 0;i < defaults->list->num; i++) //load default values
			{
				if(GetItem(function_code->varnames,(locals_num-1) - i)->type != TYPE_KV)
				{
					SetItem(function_code->varnames,(locals_num-1) - i,(object*)ConvertToKVObjectValued(GetItem(function_code->varnames,(locals_num-1) - i),GetItem((object*)defaults,i)));
				}		
				else
				{
					SetDictItemByIndex(function_code->varnames,(locals_num-1) - i,GetItem((object*)defaults,i));
				}
			}
		}
		if(kw_defaults != NULL)
		{
			//printf("loading kw defaults\n");
			for(NUM i = 0;i < kw_defaults->list->num; i++) //load default keyword values
			{
				kv_object *kv = (kv_object*)GetItem((object*)kw_defaults,i);
				SetDictItem(function_code->varnames,kv->key,kv->value);
			}
		}
	}
	if(closure != NULL)
	{
		//printf("loading closure defaults\n");
		for (NUM i = 0; i < GetTupleLen((object*)closure); i++)
		{
			object *local = GetItem((object*)closure,i);
			SetItem(function_code->freevars,i,local);
		}
	}

	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created function object\n");
		DumpObject((object*)r,0);
	}
	#endif
	//FullDumpObject(r,0);
	return(r);
}

cfunction_object *CreateCFunctionObject(struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals))
{
	cfunction_object *r = AllocCFunctionObject();
	r->type = TYPE_CFUNCTION;
	r->func = func;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created cfunction object\n");
		//DumpObject(r,0);
	}
	#endif
	return(r);
}

method_object *CreateMethodObject(object *func,class_instance_object *instance)
{
	method_object *r = AllocMethodObject();
	r->type = TYPE_METHOD;
	r->func = func;
	gc_IncRefCount((object*)r->func);
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->instance = instance;
	gc_IncRefCount((object*)r->instance);

	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created method object\n");
	}
	#endif
	return(r);
}

class_object *CreateClassObject(code_object *code,object *base_classes)
{
	class_object *r = AllocClassObject();
	r->type = TYPE_CLASS;
	r->code = code;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	r->base_classes = base_classes;
	gc_IncRefCount((object*)code);
	gc_IncRefCount(base_classes);
	code->co_flags |= CO_CLASS_ROOT;
	
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created class object\n");
		//DumpObject(r,0);
	}
	#endif
	return(r);
}

code_object *CreateCodeObject(char *name)
{
	code_object *obj = AllocCodeObject();
	obj->type = TYPE_CODE;
	obj->name = name;
	obj->argcount = 0;
	obj->kwonlyargcount = 0;
	obj->nlocals = 0;
	obj->stacksize = 0;
	obj->co_flags = 0;
	obj->code = NULL;
	obj->consts = NULL;
	obj->varnames = NULL;
	obj->freevars = NULL;
	obj->cellvars = NULL;
	obj->names = NULL;
	obj->ref_count = 0;
	#ifdef USE_LOCKING
	obj->lock_count = 0;
	#endif
	return(obj);
}

class_instance_object *CreateClassInstanceObject(class_object *instance_of)
{
	class_instance_object *r = AllocClassInstanceObject();
	r->type = TYPE_CLASS_INSTANCE;
	r->instance_of = instance_of;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	gc_IncRefCount((object*)instance_of);
	r->methods = (object*)CreateTuple(0);//this is a kv list of cfunctions or functions ->key is an unicode object with the methods name as value
	gc_IncRefCount((object*)r->methods);
	r->vars = (object*)CreateTuple(0);
	gc_IncRefCount((object*)r->vars);
	
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created class instance object\n");
		//DumpObject(r,0);
	}
	#endif
	return(r);
}

iter_object *CreateIterObject(void)
{
	iter_object *r = AllocIterObject();
	r->type = TYPE_ITER;
	r->block_stack = NULL;
	r->tag = NULL;
	r->ref_count = 0;
	#ifdef USE_LOCKING
	r->lock_count = 0;
	#endif
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}
 
#ifdef USE_DEBUGGING
long objects_num = 0;
long objects_max = 0;
long objects_header_total = 0;
#endif

#ifdef USE_LOCKING

//lock objects only if non global member vars are accessed
//check if object is a cached_object
//if it is -> load object from storage into memory
//return loaded object
//increases lock count of object

//this way caching would leave freed pointers arround hmmm
//implement a small proxy object to get arround freed pointers problem

object *LockObject(object *obj)
{
	obj->lock_count++;
	return(obj);
}

//unlock objects after usage of lock and access to member vars is no longer needed
//decreases lock count of object
//if lock count reached zero and object is a non global type check if object should be cached

void UnlockObject(object *obj)
{
	obj->lock_count--;
}

#endif
/*
object *DissolveRef(object *obj)
{
	if(obj == NULL)
		return(NULL);		
	if(obj->type == TYPE_REF)
		return(((ref_object*)obj)->ref);
	return(obj);
}
*/
kv_object *ConvertToKVObject(object *obj)
{
	kv_object *kv = AllocKVObject();
	kv->type = TYPE_KV;
	//kv->flags = 0;
	kv->ref_count = 0;
	kv->key = obj;
	gc_IncRefCount(obj);
	kv->value = NULL;
	return(kv);
}

kv_object *ConvertToKVObjectValued(object *key,object *value)
{
	kv_object *kv = AllocKVObject();
	kv->type = TYPE_KV;
	//kv->flags = 0;
	kv->ref_count = 0;
	kv->key = key;
	gc_IncRefCount(key);
	if(value != NULL)
	{
		kv->value = value;
		gc_IncRefCount(value);
	}
	else
		kv->value = NULL;
	return(kv);
}

#ifndef USE_ARDUINO_FUNCTIONS
void PrintObject(object *obj)
{
	if (obj != NULL)
	{
		switch (obj->type)
		{
		//case TYPE_PROXY:
		//	PrintObject(((proxy_object*)obj)->ref);
		//	break;
		case TYPE_ELLIPSIS:
			printf("Ellipsis");
			break;
		case TYPE_TRUE:
			printf("True");
			break;
		case TYPE_FALSE:
			printf("False");
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
					if(ti != NULL && ti->type != TYPE_NONE)
						PrintObject(ti);
				}
			}
			break;
		}
	}
}
#endif

#ifdef USE_DEBUGGING	
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
		for (char i = 0; i < level; i++)
		debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"stack @%x\n",((block_object*)obj)->stack);
		//for (char i = 0; i < level; i++)
		//debug_printf(DEBUG_ALL,"\t");
		//debug_printf(DEBUG_ALL,"code @%x\n",((block_object*)obj)->code);
		DumpObject((object*)((block_object*)obj)->code,0);
		//debug_printf(DEBUG_ALL,"iter @%x\n",((block_object*)obj)->iter);
		for (char i = 0; i < level; i++)
		debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"start: %d\n",((block_object*)obj)->start);
		for (char i = 0; i < level; i++)
		debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"len: %d\n",((block_object*)obj)->len);
		for (char i = 0; i < level; i++)
		debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"ip: %d\n",((block_object*)obj)->ip);
		//printf("stack @%x\n",((block_object*)obj)->stack);
		//DumpObject(((block_object*)obj)->iter,0);
		for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"-- block object\n");
		break;
	case TYPE_ITER:
			debug_printf(DEBUG_ALL,"iter object\n");
			debug_printf(DEBUG_ALL,"iter tag:\n");
			DumpObject((object*)((iter_object*)obj)->tag,level + 1);
			debug_printf(DEBUG_ALL,"iter block stack:\n");
			stack_Dump(((iter_object*)obj)->block_stack);
			break;
	//case TYPE_REF:
	//	debug_printf(DEBUG_ALL,"ref object\n");
	//	DumpObject(((ref_object*)obj)->ref,level+1);
	//	break;
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
		DumpObject(((kv_object*)obj)->key,level + 1);
		DumpObject(((kv_object*)obj)->value,level + 2);
		break;
	case TYPE_FUNCTION:
		//if(((function_object*)obj)->func_type == FUNC_C || ((function_object*)obj)->func_type == FUNC_C_OBJ)
			//debug_printf(DEBUG_ALL,"function object: %s\n",((function_object*)obj)->name);
			debug_printf(DEBUG_ALL,"function object: %s\n",((function_object*)obj)->func->name);
		//if(((function_object*)obj)->func_type == FUNC_PYTHON)
		//{
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"Python function\n");
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"code:\n");
			DumpObject((object*)((function_object*)obj)->func,level + 1);
			/*for (char i = 0; i < level; i++)
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
			DumpObject((object*)((function_object*)obj)->closure,level + 1);*/
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"-- function object\n");
		//}
		/*else
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
		}*/
		break;
	case TYPE_CFUNCTION:
			debug_printf(DEBUG_ALL,"cfunction object: %x\n",((cfunction_object*)obj)->func);
			//for (char i = 0; i < level; i++)
			//	debug_printf(DEBUG_ALL,"\t");
			//debug_printf(DEBUG_ALL,"C function\n");
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

					if ( i == ((tuple_object*)obj)->ptr)
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
			//for (char i = 0; i < level; i++)
			//	debug_printf(DEBUG_ALL,"\t");
			//debug_printf(DEBUG_ALL,"flags:%d\n",((code_object *) obj)->flags);
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
	case TYPE_CLASS_INSTANCE:
		debug_printf(DEBUG_ALL,"class_instance object:\n");
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"-- class_instance object\n");
		break;
	case TYPE_CLASS:
		debug_printf(DEBUG_ALL,"class object\n");
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"-- class object\n");
		break;
	case TYPE_METHOD:
		debug_printf(DEBUG_ALL,"method object\n");
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"-- method object\n");
		break;
	case TYPE_TAG:
		debug_printf(DEBUG_ALL,"tag object\n");
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"tag: %x\n",((tag_object*)obj)->tag);
		for (char i = 0; i < level; i++)
			debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"-- tag object\n");
		break;
	default:
		debug_printf(DEBUG_ALL,"object type is unknown:%c\n", obj->type);
	}
}
#endif

#ifndef USE_ARDUINO_FUNCTIONS
void FullDumpObject(object * obj, char level)
{
	if (obj == NULL)
		return;
	for (char i = 0; i < level; i++)
		printf("\t");
	printf("(%d)(%x)",obj->ref_count,obj);
	switch (obj->type)
	{
	case TYPE_BLOCK:
		printf("block object\n");
		for (char i = 0; i < level; i++)
		printf("\t");
		printf("stack @%x\n",((block_object*)obj)->stack);
		for (char i = 0; i < level; i++)
		printf("\t");
		printf("code @%x\n",((block_object*)obj)->code);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("start: %d\n",((block_object*)obj)->start);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("len: %d\n",((block_object*)obj)->len);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("ip: %d\n",((block_object*)obj)->ip);
		for (char i = 0; i < level; i++)
				printf("\t");
		printf("-- block object\n");
		break;
	case TYPE_ITER:
			printf("iter object\n");
			printf("iter tag:\n");
			FullDumpObject((object*)((iter_object*)obj)->tag,level + 1);
			//printf("iter block stack:\n");
			//stack_Dump(((iter_object*)obj)->block_stack);
			break;
	//case TYPE_REF:
	//	printf("ref object\n");
	//	FullDumpObject(((ref_object*)obj)->ref,level+1);
	//	break;
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
		FullDumpObject(((kv_object*)obj)->key,level);
		FullDumpObject(((kv_object*)obj)->value,level + 1);
		break;
	case TYPE_FUNCTION:
			printf("function object: %s\n",((function_object*)obj)->func->name);
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("Python function\n");
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("code:\n");
			FullDumpObject((object*)((function_object*)obj)->func,level + 1);
		break;
	case TYPE_CFUNCTION:
			printf("cfunction object: %x\n",((cfunction_object*)obj)->func);
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
		if (((tuple_object *) obj)->list != NULL && ((tuple_object *) obj)->list->num > 0)
		{
			printf(" contains %d items\n", ((tuple_object *) obj)->list->num);
			for (NUM i = 0; i < ((tuple_object *) obj)->list->num; i++)
			{
				if (((tuple_object*) obj)->list->items[i] != NULL)
				{

					if ( i == ((tuple_object*)obj)->ptr)
						printf("%d->",i);
					else
						printf("%d  ",i);
					FullDumpObject((object*)((tuple_object*) obj)->list->items[i], level + 1);
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
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("nlocals:%d\n",((code_object *) obj)->nlocals);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("argcount:%d\n",((code_object *) obj)->argcount);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("co_flags:%d\n",((code_object *) obj)->co_flags);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("kwonlyargcount:%d\n",((code_object *) obj)->kwonlyargcount);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("code:\n");
		FullDumpObject(((code_object *) obj)->code, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("consts:\n");
		FullDumpObject(((code_object *) obj)->consts, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("names:\n");
		FullDumpObject(((code_object *) obj)->names, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("varnames:\n");
		FullDumpObject(((code_object *) obj)->varnames, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("freevars:\n");
		FullDumpObject(((code_object *) obj)->freevars, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("cellvars:\n");
		FullDumpObject(((code_object *) obj)->cellvars, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("-- code object\n");
		break;
	case TYPE_CLASS_INSTANCE:
		printf("class_instance object:\n");
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("-- class_instance object\n");
		break;
	case TYPE_CLASS:
		printf("class object\n");
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("code:\n");
		FullDumpObject((object*)((class_object*)obj)->code, level + 1);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("-- class object\n");
		break;
	case TYPE_METHOD:
		printf("method object\n");
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("-- method object\n");
		break;
	case TYPE_TAG:
		printf("tag object\n");
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("tag: %x\n",((tag_object*)obj)->tag);
		for (char i = 0; i < level; i++)
			printf("\t");
		printf("-- tag object\n");
		break;
	default:
		printf("object type is unknown:%c\n", obj->type);
	}
}
#endif

#ifdef USE_DEBUGGING
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
	//case TYPE_REF:
	//	printf("ref object\n");
	//	DumpObject(((ref_object*)obj)->ref,level+1);
	//	break;
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
		printf("function object: %s\n",((function_object*)obj)->func->name);
		for (char i = 0; i < level; i++)
				printf("\t");
			printf("Python function\n");
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("code:\n");
			DumpObject((object*)((function_object*)obj)->func,level + 1);
			/*for (char i = 0; i < level; i++)
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
			DumpObject((object*)((function_object*)obj)->closure,level + 1);*/
		break;
/*	case TYPE_CFUNCTION:
		printf("cfunction object: %s\n",((cfunction_object*)obj)->name);
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("C function\n");
		break;*/
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

					if ( i == ((tuple_object*)obj)->ptr )
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
		//for (char i = 0; i < level; i++)
		//	printf("\t");
		//printf("flags:%d\n",((code_object *) obj)->flags);
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

BOOL CompareObjects(object *a,object *b)//TODO add tuple compare
{
	//return(CompareOp(a,b,2));
	//printf("comparing objects\n");
	#ifdef USE_DEBUGGING
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
		return(0);
		break;
	case TYPE_NONE:
		return(0);
		break;
	case TYPE_KV:
		return(CompareObjects(((kv_object*)a)->key,((kv_object*)b)->key) && CompareObjects(((kv_object*)a)->value,((kv_object*)b)->value));
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
	#ifdef USE_DEBUGGING
	//assert(obj != NULL);
	#endif
	if (obj == NULL)
		return(NULL);
	//printf("copying object :%x (%c)\n",obj,obj->type);
	
	switch (obj->type)
	{
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_NULL:
		case TYPE_NONE:
			return(CreateEmptyObject(obj->type));
		case TYPE_INT:
			return((object*)CreateIntObject(((int_object*)obj)->value));
		case TYPE_BINARY_FLOAT:
			return((object*)CreateFloatObject(((float_object*)obj)->value));
		case TYPE_UNICODE:
			return((object*)CreateUnicodeObject(str_Copy(((unicode_object*)obj)->value)));
		case TYPE_STRING:
			{
				#ifdef USE_DEBUGGING
				char *tmp = (char*)mem_malloc(((string_object*)obj)->len, "CopyObject(string) return");
				#else
				char *tmp = (char*)malloc(((string_object*)obj)->len);
				#endif
				memset(tmp,0,((string_object*)obj)->len);
				memcpy(tmp, ((string_object*)obj)->content, ((string_object*)obj)->len);
				object *r = (object*)CreateStringObject(tmp,((string_object*)obj)->len);
				return(r);
			}
		case TYPE_KV:
			return((object*)CreateKVObject(CopyObject(((kv_object*)obj)->key),CopyObject(((kv_object*)obj)->value)));
		//case TYPE_REF:
		//	return((object*)CreateRefObject(((ref_object*)obj)->ref));
		case TYPE_TAG:
			return((object*)CreateTagObject(((tag_object*)obj)->tag));
		case TYPE_METHOD:
				return((object*)CreateMethodObject(CopyObject(((method_object*)obj)->func),((method_object*)obj)->instance));
		case TYPE_CFUNCTION:
			//return(obj);
			return((object*)CreateCFunctionObject(((cfunction_object*)obj)->func));
			//return((object*)CreateCFunctionObject(((cfunction_object*)obj)->func),(tuple_object*)CopyObject((object*)((cfunction_object*)obj)->defaults),(tuple_object*)CopyObject((object*)((cfunction_object*)obj)->kw_defaults));
		case TYPE_CLASS:
			//return((object*)CreateClassObject(str_Copy(((class_object*)obj)->name),(code_object*)CopyObject((object*)((class_object*)obj)->code),((class_object*)obj)->base_classes));
			return((object*)CreateClassObject((code_object*)CopyObject((object*)((class_object*)obj)->code),((class_object*)obj)->base_classes));//TODO maybe copy base classes too
		case TYPE_CLASS_INSTANCE:
			return((object*)CreateClassInstanceObject(((class_instance_object*)obj)->instance_of));
		case TYPE_TUPLE:
			{
				tuple_object *t = CreateTuple(GetTupleLen(obj));
				for(INDEX i = 0;i<GetTupleLen(obj);i++)
					SetItem((object*)t,i,CopyObject(GetItem(obj,i)));
				return((object*)t);
			}
			break;
		case TYPE_CODE:
			{
				code_object *c = AllocCodeObject();
				c->type = TYPE_CODE;
				c->ref_count = 0;
				c->name = str_Copy(((code_object*)obj)->name);
				c->argcount = ((code_object*)obj)->argcount;
				c->kwonlyargcount = ((code_object*)obj)->kwonlyargcount;
				c->nlocals = ((code_object*)obj)->nlocals;
				c->stacksize = ((code_object*)obj)->stacksize;	
				c->co_flags = ((code_object*)obj)->co_flags;
				//printf("copied flags:%d\n",c->co_flags);
				//c->code = CopyObject(((code_object*)obj)->code);
				c->code = ((code_object*)obj)->code;
				gc_IncRefCount(c->code);
				//c->consts = CopyObject(((code_object*)obj)->consts);
				c->consts = ((code_object*)obj)->consts;
				gc_IncRefCount(c->consts);
				c->varnames = CopyObject(((code_object*)obj)->varnames);
				gc_IncRefCount(c->varnames);
				//c->freevars = CopyObject(((code_object*)obj)->freevars);
				c->freevars = ((code_object*)obj)->freevars;
				gc_IncRefCount(c->freevars);
				//c->cellvars = CopyObject(((code_object*)obj)->cellvars);
				c->cellvars = ((code_object*)obj)->cellvars;
				gc_IncRefCount(c->cellvars);
				c->names = CopyObject(((code_object*)obj)->names);
				gc_IncRefCount(c->names);
				return((object*)c);
			}
			break;
		case TYPE_FUNCTION:
			{
				function_object *f = CreateFunctionObject((code_object*)CopyObject((object*)((function_object*)obj)->func),NULL,NULL,NULL);
				//f->defaults = (tuple_object*)CopyObject((object*)((function_object*)obj)->defaults);
				//f->kw_defaults = (tuple_object*)CopyObject((object*)((function_object*)obj)->kw_defaults);
				//f->closure = (tuple_object*)CopyObject((object*)((function_object*)obj)->closure);
				//f->defaults = ((function_object*)obj)->defaults;
				//gc_IncRefCount((object*)f->defaults);
				//f->kw_defaults = ((function_object*)obj)->kw_defaults;
				//gc_IncRefCount((object*)f->kw_defaults);
				//f->closure = ((function_object*)obj)->closure;
				//gc_IncRefCount((object*)f->closure);
				return((object*)f);
			}
			break;
		case TYPE_BLOCK:
			{
			}
			break;
		case TYPE_ITER:
			{
			}
			break;
	}
  return(NULL);
}

void AppendDictItem(object * tuple,object *key,object *value)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || key == NULL)
		return;
	if(((tuple_object *) tuple)->list == NULL)
		((tuple_object *) tuple)->list = ptr_CreateList(0,0);
	//object *di = CopyObject(key);//TODO needed??
	//kv_object *kv = ConvertToKVObjectValued(di,value);
	kv_object *kv = ConvertToKVObjectValued(key,value);
	#ifdef USE_DEBUGGING
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
	gc_IncRefCount((object*)kv);

}

void AppendItem(object *tuple,object *value)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if(((tuple_object*)tuple)->list == NULL)
		((tuple_object*)tuple)->list = ptr_CreateList(0,0);
	ptr_Push(((tuple_object*)tuple)->list,value);
	gc_IncRefCount(value);
}

void InsertItem(object *tuple,INDEX index,object *value)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || value == NULL)
		return;
	if(((tuple_object*)tuple)->list == NULL)
		((tuple_object*)tuple)->list = ptr_CreateList(0,0);
	ptr_Insert(((tuple_object*)tuple)->list,index,value);
	gc_IncRefCount(value);
}

void SetDictItem(object *tuple,object *key,object *value)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || key == NULL)
		return;
	INDEX index = GetDictItemIndex(tuple,key);
	#ifdef USE_DEBUGGING
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
				if(value != NULL && value != obj_NULL)
				{
					((kv_object*) k)->value = value;
					gc_IncRefCount(value);
				}
				else
					((kv_object*) k)->value = NULL;
				if(old != NULL)
					gc_DecRefCount(old);
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
					((kv_object*)k)->value = value;
					gc_IncRefCount(value);
				}
				else
					((kv_object*)k)->value = NULL;
				if(old != NULL)
					gc_DecRefCount(old);
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
	for (INDEX i = 0; i < ((tuple_object*) tuple)->list->num; i++)
	{
		if (  ((tuple_object*) tuple)->list->items[i] == NULL || (((tuple_object*) tuple)->list->items[i] != NULL && ((object*)((tuple_object *) tuple)->list->items[i])->type != TYPE_KV)  )
			continue;
		else
		{
			//FullDumpObject(((tuple_object*)tuple)->list->items[i],0);
			kv_object *kv = (kv_object*)((tuple_object*)tuple)->list->items[i];
			((tuple_object*) tuple)->list->items[i] = kv->key;
			gc_IncRefCount(kv->key);
			gc_DecRefCount((object*)kv);
		}
	}
	//printf("cleared dict values\n");
	//DumpObject(tuple,0);
}

object *GetDictItemByIndex(object *tuple,INDEX index)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL || index >= ((tuple_object *) tuple)->list->num || index < 0)
		return(obj_NULL);
	return(((kv_object*)GetItem(tuple,index))->value);
}

NUM GetTupleLen(object *tuple)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object*) tuple)->list == NULL || ((tuple_object*) tuple)->list->num == 0)
		return(0);
	return(((tuple_object*) tuple)->list->num);
}

object *GetDictItem(object *tuple,object *key)
{
	INDEX index = GetDictItemIndex(tuple,key);
	if(index != -1)
		{
			object *kv = GetItem(tuple,index);
			if(kv->type != TYPE_KV)
				return(obj_NULL);
			return(((kv_object*)kv)->value);
		}
	return(obj_NULL);
}

INDEX GetDictItemIndex(object *tuple,object *key)
{
	if(tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object*) tuple)->list == NULL || key == NULL)
		return (-1);
	 //printf("checking in %d tuple items for\n",((tuple_object*)tuple->ptr)->list->num);
	//printf("getdictitemindex\n");
	//printf("key\n");
	//DumpObject(key,0);
	//printf("tuple:%d\n",((tuple_object*)tuple)->list->num);
	//DumpObject(tuple,0);
	for(INDEX i = 0; i < ((tuple_object*)tuple)->list->num; i++)
	{
		 //printf("checking tuple:%d\n",i);
		// if(((tuple_object*)tuple->ptr)->items[i]->type == TYPE_UNICODE)
		 
		 //printf("checking against:\n");
		 //%s\n",name,(char*)((tuple_object*)tuple->ptr)->items[i]->ptr);
		if(((tuple_object*)tuple)->list->items[i] != NULL && ((object*)((tuple_object*)tuple)->list->items[i])->type != TYPE_KV)
		{
			if(!CompareObjects(((tuple_object*)tuple)->list->items[i],key))
				return (i);
		}
		else if(((tuple_object*)tuple)->list->items[i] != NULL)
		{
			if(!CompareObjects(((kv_object*)((tuple_object*)tuple)->list->items[i])->key,key))
				return (i);
		}
	}
	return (-1);
}

INDEX GetItemIndex(object *tuple,object *obj)
{
	if(tuple == NULL || tuple->type != TYPE_TUPLE || ((tuple_object *) tuple)->list == NULL || obj == NULL)
		return(-1);

	for(INDEX i = 0; i < ((tuple_object*) tuple)->list->num; i++)
	{
		if(((tuple_object *) tuple)->list->items[i] != NULL && ((object*)((tuple_object *) tuple)->list->items[i])->type != TYPE_KV)
		{
			if(!CompareObjects(((tuple_object*) tuple)->list->items[i],obj))
				return(i);
		}
		else
		{
			if(!CompareObjects(((kv_object*)((tuple_object*) tuple)->list->items[i])->value,obj))
				return(i);
		}
	}
	return(-1);
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
			gc_IncRefCount(((tuple_object *) tuple)->list->items[i]);
			gc_DecRefCount(((kv_object*)((tuple_object *) tuple)->list->items[i])->key);
		}
		else
			continue;
	}
}

INDEX GetItemIndexByName(object *tuple, char *name)
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
		return (obj_NULL);
	for (INDEX i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{		
		if (((tuple_object*) tuple)->list->items[i] == NULL)
			continue;
		object *item = (object*)((tuple_object *) tuple)->list->items[i];
		if(item->type == TYPE_KV)
		{
			object *key = (object*)((kv_object*)item)->key;
			object *value = (object*)((kv_object*)item)->value;

			if (key->type == TYPE_UNICODE && !strcmp(((unicode_object*)key)->value, name)) 
			{
				return (value);
			}
		}
	}
	return (obj_NULL);
}

object *GetAttribute(object *obj,object *key)
{
	if(obj == NULL || key == NULL)
		return(obj_NULL);
	object *r = NULL;
	if(obj->type == TYPE_CODE)
	{
		r = GetDictItem(((code_object*)obj)->names,key);
	}else if(obj->type == TYPE_CLASS)
	{
		r = GetDictItem(((class_object*)obj)->code->names,key);
	}else if(obj->type == TYPE_CLASS_INSTANCE)
	{
		r = GetClassVar(obj,key);
		if(r == NULL || r == obj_NULL)
		{
			r = GetClassMethod(obj,key);
			if(r != NULL && r != obj_NULL)
			{	
				method_object *mo = CreateMethodObject(r,(class_instance_object*)obj);
				//gc_IncRefCount(r);
				//gc_IncRefCount(obj);
				return((object*)mo);
			}
		}
		if(r == NULL || r == obj_NULL)
			r = GetAttribute((object*)((class_instance_object*)obj)->instance_of,key);
	}
	else
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"object type:%c has no attributes\n",obj->type);
		DumpObject(obj,0);
		#endif
	}
	return(r);
}

/*object *GetAttributeByName(object *obj,char *name)
{


}*/

void SetAttribute(object *obj,object *key,object *value)
{
	if(obj->type == TYPE_CLASS_INSTANCE)
	{
		SetDictItem(((class_instance_object*)obj)->vars,key,value);
	}
//TODO search base_classes before adding a new dict item
//TODO if attribute is a function type add as method

/*
	if(tos->type == TYPE_CODE)
	{
		INDEX index = GetItemIndexByName(((code_object*)tos)->names,((unicode_object*)name)->value);
		if(index != -1)
		{
			SetDictItemByIndex(((code_object*)tos)->names,index,tos1);
		}
		else
			AppendDictItem(((code_object*)tos)->names,name,tos1);
		}else if(tos->type == TYPE_CLASS_INSTANCE)
		{
			INDEX index = GetItemIndexByName(((class_instance_object*)tos)->vars,((unicode_object*)name)->value);
			if(index != -1)
			{
				SetDictItemByIndex(((class_instance_object*)tos)->vars,index,tos1);
			}
			else
				AppendDictItem(((class_instance_object*)tos)->vars,name,tos1);
	}
*/
}

object *GetClassMethod(object *_class,object *key)
{
	object *r = obj_NULL;
	if(_class->type == TYPE_CLASS)
	{
		//DumpObject(((class_object*)class)->code->names,0);
		r = GetDictItem(((class_object*)_class)->code->names,key);
		if(r!= NULL && r != obj_NULL && !(r->type == TYPE_FUNCTION || r->type == TYPE_CFUNCTION))
			r = obj_NULL;
		if(r == NULL || r == obj_NULL)
		{	
			tuple_object *bc = (tuple_object*)((class_object*)_class)->base_classes;
			for(INDEX i = 0;i<GetTupleLen((object*)bc);i++)
			{
				r = GetClassMethod(bc->list->items[i],key);
				if(r != NULL && r != obj_NULL)
					break;
			}
		}
		//DumpObject(r,0);
	}
	else
	if(_class->type == TYPE_CLASS_INSTANCE)
	{
		r = GetDictItem(((class_instance_object*)_class)->methods,key);
		if(r == NULL || r == obj_NULL)
		{
			//DumpObject((object*)((class_instance_object*)class)->instance_of,0);						
			//DumpObject((object*)((class_instance_object*)class)->instance_of->code,0);						
			//DumpObject((object*)((class_instance_object*)class)->instance_of->code->names,0);
			//DumpObject(key,0);
			//r = GetDictItem(((class_instance_object*)class)->instance_of->code->names,key);
			if(r == NULL || obj_NULL)
				r = GetClassMethod((object*)((class_instance_object*)_class)->instance_of,key);
			/*
			tuple_object *bc = (tuple_object*)((class_instance_object*)class)->instance_of->base_classes;
			for(INDEX i = 0;i<GetTupleLen((object*)bc);i++)
			{
				r = GetClassMethod(bc->list->items[i],key);
				if(r != NULL)
					break;
			}
			*/
			//DumpObject(((class_instance_object*)class)->instance_of->base_classes,0);
			//if(r==NULL)
			//	r = GetDictItem(((class_instance_object*)class)->instance_of->code->names,key);
			//if(r==NULL)
			//	printf("didnt find any method\n");
		}
	}
	return(r);
}

object *GetClassVar(object *_class,object *key)
{
	object *r = obj_NULL;
	if(_class->type == TYPE_CLASS_INSTANCE)
	{
		//DumpObject(((class_instance_object*)class)->vars,0);
		r = GetDictItem(((class_instance_object*)_class)->vars,key);
		//if(r == NULL)
		//	r = GetAttribute(((class_instance_object*)class)->instance_of,key);
	}
	return(r);
}

void AddCodeName(object *co,object *key,object *value)
{
	((code_object*)co)->nlocals++;
	if(((code_object*)co)->names == NULL)
	{
		((code_object*)co)->names = (object*)CreateTuple(0);
		gc_IncRefCount(((code_object*)co)->names);
	}
	kv_object *kv = CreateKVObject((object*)key,value);
	AppendItem(((code_object*)co)->names,(object*)kv);
}

void AddCodeFunction(object *co,char *name,object *func)
{
	unicode_object *key = CreateUnicodeObject(str_Copy(name));
	AddCodeName(co,(object*)key,func);
}

void AddCodeCFunction(object *co,char *name,	struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals))
{
	cfunction_object *cfo = CreateCFunctionObject(func);
	AddCodeFunction(co,name,(object*)cfo);
}

void ResetIteration(object *tuple)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
		((tuple_object*)tuple)->ptr = 0;
}

void SetItem(object *tuple, INDEX index, object * obj)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if (index >= ((tuple_object*) tuple)->list->num)
		return;
	if(index < 0)
		index = index + GetTupleLen(tuple);
	object *old = ((tuple_object*) tuple)->list->items[index];	
	if(obj!=NULL && obj != obj_NULL)
	{
		((tuple_object*) tuple)->list->items[index] = obj;
		gc_IncRefCount(obj);
	}
	else
	{
		((tuple_object*) tuple)->list->items[index] = obj_NULL;
		gc_IncRefCount(obj_NULL);
	}
	if(old != NULL)
	{
		gc_DecRefCount(old);
	}		
}

void DeleteItem(object *tuple, INDEX index)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if (index >= ((tuple_object*) tuple)->list->num)
		return;
	if(index < 0)
		index = index + GetTupleLen(tuple);
	if (((tuple_object*) tuple)->list->items[index] == NULL)
		return;
	gc_DecRefCount((object*)(((tuple_object*) tuple)->list->items[index]));
	((tuple_object*) tuple)->list->items[index] = obj_NULL;
	gc_IncRefCount(obj_NULL);
}

void DeleteDictItem(object *tuple,object *key)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE || key == NULL)
		return;
	INDEX index = GetDictItemIndex(tuple,key);
	if(index != -1)
	{
		DeleteItem(tuple,index);
	}
}

object *GetItem(object *tuple, INDEX index)
{
	if(tuple == NULL || tuple->type != TYPE_TUPLE)
		return (obj_NULL);
	if(index >= ((tuple_object *) tuple)->list->num)
		return (obj_NULL);
	if(index < 0)
		index = index + GetTupleLen(tuple);
	return(((tuple_object*)tuple)->list->items[index]);
}

object *GetNextItem(object * tuple) //TODO move to tuples
{
	if(tuple == NULL || tuple->type != TYPE_TUPLE)
		return (obj_NULL);

	//if(((tuple_object*)tuple)->ptr == -1)
	//{
	//	((tuple_object*)tuple)->ptr = 0;
	//	return (NULL);
	//}

	if( ((tuple_object*)tuple)->ptr < ((tuple_object*)tuple)->list->num)
	{
		((tuple_object*)tuple)->ptr++;
		return (((tuple_object *) tuple)->list->items[((tuple_object*)tuple)->ptr-1]);
	}
	else
	{	
		((tuple_object*)tuple)->ptr = 0;
		return (obj_NULL);
	}
}

