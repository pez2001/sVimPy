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
	#ifdef USE_DEBUGGING
	return((block_object *)mem_malloc(sizeof(block_object), "AllocBlockObject() return"));
	#else
	return((block_object *)malloc(sizeof(block_object)));
	#endif
}
/*
loop_block_object *AllocLoopBlockObject()
{	
	#ifdef USE_DEBUGGING
	return((loop_block_object *)mem_malloc(sizeof(loop_block_object), "AllocLoopBlockObject() return"));
	#else
	return((loop_block_object *)malloc(sizeof(loop_block_object)));
	#endif
}*/

kv_object *AllocKVObject(void)
{
	#ifdef USE_DEBUGGING
	return((kv_object *) mem_malloc(sizeof(kv_object), "AllocKVObject() return"));
	#else
	return((kv_object *) malloc(sizeof(kv_object)));
	#endif
}

object *AllocObject(void)
{
	#ifdef USE_DEBUGGING
	return ((object *) mem_malloc(sizeof(object), "AllocObject() return"));
	#else
	return((object *) malloc(sizeof(object)));
	#endif
}

code_object *AllocCodeObject(void)
{
	#ifdef USE_DEBUGGING
	return((code_object *) mem_malloc(sizeof(code_object), "AllocCodeObject() return"));
	#else
	return((code_object *) malloc(sizeof(code_object)));
	#endif
}

function_object *AllocFunctionObject(void)
{
	#ifdef USE_DEBUGGING
	return((function_object *) mem_malloc(sizeof(function_object),"AllocFunctionObject() return"));
	#else
	return((function_object *) malloc(sizeof(function_object)));
	#endif
}
/*
cfunction_object *AllocCFunctionObject(void)
{
	#ifdef USE_DEBUGGING
	return((cfunction_object *) mem_malloc(sizeof(cfunction_object),"AllocCFunctionObject() return"));
	#else
	return((cfunction_object *) malloc(sizeof(cfunction_object)));
	#endif
}
*/
string_object *AllocStringObject(void)
{
	#ifdef USE_DEBUGGING
	return((string_object *) mem_malloc(sizeof(string_object), "AllocStringObject() return"));
	#else
	return((string_object *) malloc(sizeof(string_object)));
	#endif
}

tuple_object *AllocTupleObject(void)
{
	#ifdef USE_DEBUGGING
	return((tuple_object *) mem_malloc(sizeof(tuple_object), "AllocTupleObject() return"));
	#else
	return((tuple_object *) malloc(sizeof(tuple_object)));
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

ref_object *AllocRefObject(void)
{
	#ifdef USE_DEBUGGING
    return((ref_object*) mem_malloc(sizeof(ref_object), "AllocRefObject() return")); 
	#else
    return((ref_object*) malloc(sizeof(ref_object))); 
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

ref_object *CreateRefObject(object *ref_to)//,OBJECT_FLAGS flags)
{
	ref_object *r = AllocRefObject();
	r->type = TYPE_REF;
	//r->flags = flags;
	r->ref_count = 0;
	r->ref = ref_to;
	gc_IncRefCount(ref_to);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

int_object *CreateIntObject(long value)//,OBJECT_FLAGS flags)
{
	int_object *r = AllocIntObject();
	r->type = TYPE_INT;
	r->ref_count = 0;
	//r->flags = flags;
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

float_object *CreateFloatObject(float value)//,OBJECT_FLAGS flags)
{
	float_object *r = AllocFloatObject();
	r->type = TYPE_BINARY_FLOAT;
	r->ref_count = 0;
	//r->flags = flags;
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

unicode_object *CreateUnicodeObject(char *value)//,OBJECT_FLAGS flags)
{
	unicode_object *r = AllocUnicodeObject();
	r->type = TYPE_UNICODE;
	//r->flags = flags;
	r->ref_count = 0;
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

tuple_object *CreateTuple(NUM num)//,OBJECT_FLAGS flags)
{
	tuple_object *r = AllocTupleObject();
	r->type = TYPE_TUPLE;
	//r->flags = flags;
	r->ref_count = 0;
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

string_object *CreateStringObject(char *bytes,NUM len)//,OBJECT_FLAGS flags)
{
	string_object *r = AllocStringObject();
	r->type = TYPE_STRING;
	//r->flags = flags;
	r->ref_count = 0;
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

kv_object *CreateKVObject(object *key,object *value)//,OBJECT_FLAGS flags)
{
	kv_object *r = AllocKVObject();
	r->type = TYPE_KV;
	//r->flags = flags;
	r->ref_count = 0;
	r->key = key;
	gc_IncRefCount(key);
	r->value = value;
	//DecRefCount(value);// ?? why did this not produce any memory leaks ?? strange
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

object *CreateEmptyObject(char type)//,OBJECT_FLAGS flags)
{
	object *r = AllocObject();
	r->type = type;
	//r->flags = flags;
	r->ref_count = 0;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

function_object *CreateFunctionObject_MAKE_FUNCTION(code_object *function_code,tuple_object *defaults,tuple_object *kw_defaults)//,OBJECT_FLAGS flags)
{
	function_object *r = AllocFunctionObject();
	r->type = TYPE_FUNCTION;
	//r->flags = flags;
	r->ref_count = 0;
	//r->func.code = function_code;
	r->func = function_code;
	//r->func_type = FUNC_PYTHON;
	r->closure = NULL;
	//r->name = function_code->name;
	gc_IncRefCount((object*)function_code);
	if(defaults != NULL)
	{
		r->defaults = defaults;
		gc_IncRefCount((object*)defaults);
	}
	else
		r->defaults = NULL;
	if(kw_defaults != NULL)
	{
		r->kw_defaults = kw_defaults;
		gc_IncRefCount((object*)kw_defaults);
	}
	else
		r->kw_defaults = NULL;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

function_object *CreateFunctionObject_MAKE_CLOSURE(code_object *function_code,tuple_object *closure,tuple_object *defaults,tuple_object *kw_defaults)//,OBJECT_FLAGS flags)
{
	function_object *r = AllocFunctionObject();
	r->type = TYPE_FUNCTION;
	//r->flags = flags;
	r->ref_count = 0;
	//r->func.code = function_code;
	r->func = function_code;
	r->closure = closure;
	gc_IncRefCount((object*)closure);
	//r->func_type = FUNC_PYTHON;
	//r->name = function_code->name;
	gc_IncRefCount((object*)function_code);
	if(defaults != NULL)
	{
		r->defaults = defaults;
		gc_IncRefCount((object*)defaults);
	}
	else
		r->defaults = NULL;
	if(kw_defaults != NULL)
	{
		r->kw_defaults = kw_defaults;
		gc_IncRefCount((object*)kw_defaults);
	}
	else
		r->kw_defaults = NULL;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		DumpObject((object*)r,0);
	}
	#endif
	return(r);
}

function_object *CreateFunctionObject(code_object *co)//unsigned char func_type);//,OBJECT_FLAGS flags)
{
	function_object *r = AllocFunctionObject();
	r->type = TYPE_FUNCTION;
	//r->flags = flags;
	r->defaults = NULL;
	r->kw_defaults = NULL;
	r->closure = NULL;
	//r->func.func = NULL;
	r->func = co;
	//r->name = NULL;
	r->ref_count = 0;
	//r->func_type = func_type;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created function object\n");
		//DumpObject(r,0);
	}
	#endif
	return(r);
}
/*
cfunction_object *CreateCFunctionObject(char *name,struct _object* (*func) (struct _vm *vm,struct _tuple_object *locals,struct _tuple_object *kw_locals))//,OBJECT_FLAGS flags)
{
	cfunction_object *r = AllocCFunctionObject();
	r->type = TYPE_CFUNCTION;
	//r->flags = flags;
	//r->func.func = NULL;
	r->func = func;
	r->name = NULL;
	r->ref_count = 0;
	//r->func_type = func_type;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created cfunction object\n");
		//DumpObject(r,0);
	}
	#endif
	return(r);
}
*/
iter_object *CreateIterObject(void)//OBJECT_FLAGS flags)
{
	iter_object *r = AllocIterObject();
	r->type = TYPE_ITER;
	//r->flags = flags;
	//r->iter_func = iter_func;
	//r->tag = tag;
	r->ref_count = 0;
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
	kv->value = value;
	gc_IncRefCount(value);
	return(kv);
}

/*void FreeBlockObject(object *obj)
{
		//if(((block_object*)obj)->iter != NULL)
		//	FreeObject(((block_object*)obj)->iter);
		//printf("closing block stack\n");
		debug_printf(DEBUG_ALL,"block object to be freed:%x\n",obj);
		debug_printf(DEBUG_ALL,"block object code to be freed:%x\n",((block_object*)obj)->code);
		FreeObject((object*)((block_object*)obj)->code);
		if(((block_object*)obj)->stack != NULL)
			stack_Close(((block_object*)obj)->stack,0);
		//printf("closed block\n");
}
*/
void PrintObject(object *obj)
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
					if(ti != NULL && ti->type != TYPE_NONE)
						PrintObject(ti);
				}
			}
			break;
		}
	}
}

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
		for (char i = 0; i < level; i++)
		debug_printf(DEBUG_ALL,"\t");
		debug_printf(DEBUG_ALL,"code @%x\n",((block_object*)obj)->code);
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
/*	case TYPE_CFUNCTION:
			debug_printf(DEBUG_ALL,"cfunction object: %s\n",((cfunction_object*)obj)->name);
			for (char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"C function\n");
		break;*/
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
	default:
		debug_printf(DEBUG_ALL,"object type is unknown:%c\n", obj->type);
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
		printf("function object: %s\n",((function_object*)obj)->func->name);
		for (char i = 0; i < level; i++)
				printf("\t");
			printf("Python function\n");
			for (char i = 0; i < level; i++)
				printf("\t");
			printf("code:\n");
			DumpObject((object*)((function_object*)obj)->func,level + 1);
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

BOOL object_compare(object *a,object *b)
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
	#ifdef USE_DEBUGGING
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
		//r->flags = obj->flags;
		return(r);
		}
		break;
	case TYPE_INT:
		{
		int_object *r = AllocIntObject();
		r->type = obj->type;
		r->ref_count  = 0;
		//r->flags = obj->flags;
		r->value = ((int_object*)obj)->value;
		return((object*)r);
		}
		break;
	case TYPE_BINARY_FLOAT:
		{
		float_object *r = AllocFloatObject();
		r->type = obj->type;
		r->ref_count  = 0;
		//r->flags = obj->flags;
		r->value = ((float_object*)obj)->value;
		return((object*)r);
		}
		break;
	case TYPE_UNICODE:
	   {
		unicode_object *r = AllocUnicodeObject();
		r->type = obj->type;
		r->ref_count  = 0;
		//r->flags = obj->flags;
		r->value = str_Copy(((unicode_object*)obj)->value);
		return((object*)r);
		}
		break;
	case TYPE_STRING:
		//ret->ptr = mem_malloc(
		break;
	case TYPE_TUPLE:
		break;
	case TYPE_CODE:
		//add reference object to decrease memory usage for code objects
		//reference objects refs need no freeing
		return((object*)CreateRefObject(obj));//,obj->flags));
		break;

	case TYPE_FUNCTION:
		//add reference object to decrease memory usage for code objects
		//reference objects refs need no freeing
		return((object*)CreateRefObject(obj));//,obj->flags));
		break;
/*	case TYPE_CFUNCTION:
		//add reference object to decrease memory usage for code objects
		//reference objects refs need no freeing
		return((object*)CreateRefObject(obj));//,obj->flags));
		break;*/
	}
  return(NULL);
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
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if(((tuple_object*)tuple)->list == NULL)
		((tuple_object*)tuple)->list = ptr_CreateList(0,0);
	ptr_Insert(((tuple_object*)tuple)->list,index,value);
	gc_IncRefCount(value);
}

void SetDictItem(object *tuple,object *key,object *value)
{
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
				if(value != NULL)
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
	for (NUM i = 0; i < ((tuple_object *) tuple)->list->num; i++)
	{
		if (((tuple_object *) tuple)->list->items[i] != NULL && ((object*)((tuple_object *) tuple)->list->items[i])->type != TYPE_KV)
			continue;
		else
		{
			kv_object *kv = (kv_object*)((tuple_object *) tuple)->list->items[i];
			((tuple_object *) tuple)->list->items[i] = kv->key;
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
	if (index >= ((tuple_object*) tuple)->list->num || index < 0)
		return;
	object *old = ((tuple_object*) tuple)->list->items[index];	
	if(obj!=NULL)
	{
		((tuple_object*) tuple)->list->items[index] = obj;
		gc_IncRefCount(obj);
	// obj->flags ^= OFLAG_ON_STACK;
	}
	else
		((tuple_object*) tuple)->list->items[index] = NULL;

	if(old != NULL)
	{
		gc_DecRefCount(old);
	}		
}

void DeleteItem(object *tuple, INDEX index)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if (index >= ((tuple_object*) tuple)->list->num || index < 0)
		return;
	if (((tuple_object*) tuple)->list->items[index] == NULL)
		return;
	//if (!(((object*)((tuple_object *) tuple)->list->items[index])->type & OFLAG_ON_STACK))
		gc_DecRefCount((object*)(((tuple_object*) tuple)->list->items[index]));
	((tuple_object*) tuple)->list->items[index] = NULL;
}

object *GetItem(object *tuple, INDEX index)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return (NULL);
	if (index >= ((tuple_object *) tuple)->list->num || index < 0)
		return (NULL);
	return (((tuple_object *) tuple)->list->items[index]);
}

object *GetNextItem(object * tuple) //TODO move to tuples
{
	if(tuple == NULL || tuple->type != TYPE_TUPLE)
		return (NULL);

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
		return (NULL);
	}
}

long ReadLong(stream *f) //TODO move to streams
{
	long r = 0;
	stream_Read(f,&r, 4);
	return (r);
}

FLOAT ReadFloat(stream *f)
{
	float r = 0;
	double t = 0;
	stream_Read(f, &t,8);
	r = (FLOAT)t;
	return (r);
}

char ReadChar(stream *f)
{
	char r = 0;
	stream_Read(f,&r,1);
	return (r);
}

void WriteLong(long l,stream *f)
{
	stream_Write(f,&l,4);
}

void WriteFloat(FLOAT fl,stream *f)
{
	double d = (double)fl;
	stream_Write(f,&d,8);
}

void WriteChar(char c,stream *f)
{
	stream_Write(f,&c,1);
}

object *ReadObject(stream *f)
{
	OBJECT_TYPE type = ReadChar(f);

	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_CREATION,"reading object with type:%c\n",type);
	#endif
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
		//obj->flags = 0;
		obj->ref_count = 1;
		// printf("allocated empty object @%x\n",obj);
		return (obj);
		}
	case TYPE_CODE:
       {	
		code_object *co = AllocCodeObject();
		co->type = TYPE_CODE;
		co->ref_count = 1;
		co->argcount = (NUM)ReadLong(f);
		co->kwonlyargcount = (NUM)ReadLong(f);
		co->nlocals = (NUM)ReadLong(f);
		co->stacksize = (NUM)ReadLong(f);
		co->co_flags = ReadLong(f);
		co->code = ReadObject(f);
		co->consts = ReadObject(f);
		co->names = ReadObject(f);
		co->varnames = ReadObject(f);
		co->freevars = ReadObject(f);
		co->cellvars = ReadObject(f);
		ConvertToDict(co->freevars); //TODO only needed as long deref and closure opcodes are not implemented using references
		ConvertToDict(co->cellvars);
		object *tmp_filename = ReadObject(f);
		gc_DecRefCount(tmp_filename);	// TO DECREASE MEMORY USAGE
		object *tmp_name = ReadObject(f);
		co->name = str_Copy(((unicode_object *)tmp_name)->value);
		gc_DecRefCount(tmp_name);
		ReadLong(f);//firstlineno
		object *tmp_lnotab = ReadObject(f);
		gc_DecRefCount(tmp_lnotab);	// TO DECREASE MEMORY USAGE
		gc_Clear();
		return((object*)co);
		}
		break;
	case TYPE_STRING:
		{
		string_object *obj = AllocStringObject();

		obj->ref_count = 1;

		NUM n;
		// printf("reading string chunk\n");
		n = (NUM)ReadLong(f);
		if (n > 0)
		{
			#ifdef USE_DEBUGGING
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

			 //printf("read string:\"%s\"\r\n",obj->content);
			//printf("n:%d\n",obj->len);
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
		to->ptr = 0;

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

				//if (i == 0)
				//{
				//	to->ptr = tuple;
					// printf("setting tuple's iterate over ptr\n");
				//}
				// printf("object type:%c\n",tuple->type);
				to->list->items[i] = tuple;
			}
		}
		else
		 to->list = NULL;
		//to->num = n;
		//to->flags = 0;
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
		#ifdef USE_DEBUGGING
		char *unicode_string = (char *)mem_malloc(n + 1, "ReadObject() unicode_string");
		#else
		char *unicode_string = (char *)malloc(n + 1);
		#endif
		memset(unicode_string, 0, n + 1);
		stream_Read(f,unicode_string, n);
		//printf("read unicode string:%s\n",unicode_string);
		// uo->len = n;
		//obj->flags = 0;
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
		//obj->flags = 0;
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
		//obj->flags = 0;
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
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", type);
		#endif
		return(obj);
		}
	}
	// printf("read object\r\n"); 
	return (NULL);
}

void WriteObject(object *obj,stream *f)
{
	WriteChar(obj->type,f);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_CREATION,"writing object with type:%c\n",obj->type);
	#endif
	//printf("writing object:\n");
	//DumpObject(obj,0);
	switch(obj->type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_ELLIPSIS:
		{
		return;
		}
	case TYPE_CODE:
       {
		WriteLong(((code_object*)obj)->argcount,f);
		WriteLong(((code_object*)obj)->kwonlyargcount,f);
		WriteLong(((code_object*)obj)->nlocals,f);
		WriteLong(((code_object*)obj)->stacksize,f);
		WriteLong(((code_object*)obj)->co_flags,f);
		WriteObject(((code_object*)obj)->code,f);
		WriteObject(((code_object*)obj)->consts,f);
		WriteObject(((code_object*)obj)->names,f);
		WriteObject(((code_object*)obj)->varnames,f);
		WriteObject(((code_object*)obj)->freevars,f);
		WriteObject(((code_object*)obj)->cellvars,f);
		unicode_object *filename = CreateUnicodeObject("");
		gc_IncRefCount((object*)filename);
		WriteObject((object*)filename,f);
		gc_DecRefCount((object*)filename);
		unicode_object *name = CreateUnicodeObject(((code_object*)obj)->name);
		gc_IncRefCount((object*)name);
		WriteObject((object*)name,f);
		gc_DecRefCount((object*)name);
		WriteLong(0,f);
		object *lnotab = CreateEmptyObject(TYPE_NONE);
		gc_IncRefCount((object*)lnotab);
		WriteObject(lnotab,f);
		gc_DecRefCount(lnotab);
		gc_Clear();
		return;
		}
	case TYPE_STRING:
		{
		NUM n = ((string_object*)obj)->len;
		WriteLong(n,f);
		if (n > 0)
		{
			stream_Write(f,((string_object*)obj)->content, n);
		}
		return;
		}
	case TYPE_TUPLE:
		{
		if(((tuple_object*)obj)->list != NULL)
		{
			INT n = ((tuple_object*)obj)->list->num;
			WriteLong(n,f);
			//printf("tuple items:%d\n",n);
			if (n > 0)
			{
				for (NUM i = 0; i < n; i++)
				{
					WriteObject(((tuple_object*)obj)->list->items[i],f);
				}
			}
		}
		else
			WriteLong(0,f);
		
		//printf("written tuple\n");
		return;
		}
	case TYPE_UNICODE:
		{
		NUM n = strlen(((unicode_object*)obj)->value);
		WriteLong(n,f);
		if(n > 0)
		{
			stream_Write(f,((unicode_object*)obj)->value, n);
		}
		return;
		}
	case TYPE_BINARY_FLOAT:
		{
		WriteFloat(((float_object*)obj)->value,f);
		return;
		}
	case TYPE_INT:
		{
		WriteLong(((int_object*)obj)->value,f);
		return;
		}
	default:
		{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", obj->type);
		#endif
		return;
		}
	}
}

//same as above , but using a reduced code object layout
object *ReadObjectPlus(stream *f)
{
	OBJECT_TYPE type = ReadChar(f);

	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_CREATION,"reading object with type:%c\n",type);
	#endif
	switch (type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_ELLIPSIS:
		{
		object *obj = AllocObject();
		obj->type = type;
		obj->ref_count = 1;
		return (obj);
		}
	case TYPE_CODE:
       {	
		code_object *co = AllocCodeObject();
		co->type = TYPE_CODE;
		co->ref_count = 1;
		co->stacksize = 0;

		co->argcount = (NUM)ReadLong(f);
		co->kwonlyargcount = (NUM)ReadLong(f);
		co->nlocals = (NUM)ReadLong(f);
		co->co_flags = ReadLong(f);
		co->code = ReadObjectPlus(f);
		co->consts = ReadObjectPlus(f);
		co->names = ReadObjectPlus(f);
		co->varnames = ReadObjectPlus(f);
		co->freevars = ReadObjectPlus(f);
		co->cellvars = ReadObjectPlus(f);
		ConvertToDict(co->freevars); //TODO only needed as long deref and closure opcodes are not implemented using references
		ConvertToDict(co->cellvars);
		object *tmp_name = ReadObjectPlus(f);
		co->name = str_Copy(((unicode_object *)tmp_name)->value);
		gc_DecRefCount(tmp_name);
		gc_Clear();
		return((object*)co);
		}
		break;
	case TYPE_STRING:
		{
		string_object *obj = AllocStringObject();
		obj->ref_count = 1;
		NUM n;
		n = (NUM)ReadLong(f);
		if (n > 0)
		{
			#ifdef USE_DEBUGGING
			char *string = (char *)mem_malloc(n, "ReadObject() string");
			#else
			char *string = (char *)malloc(n);
			#endif
			stream_Read(f,string, n);
			obj->len = n;
			obj->content = string;
			obj->type = TYPE_STRING;
		}
		else
		{
			obj->content = NULL;
			obj->type = TYPE_NULL;
			obj->len = 0;
		}
		return((object*)obj);
		}
		break;
	case TYPE_TUPLE:
		{
		INT n = (INT)ReadLong(f);
		tuple_object *to = AllocTupleObject();
		to->ref_count = 1;
		to->ptr = 0;
		if (n > 0)
		{
			to->list = ptr_CreateList(n,PTR_STATIC_LIST);
			for (NUM i = 0; i < n; i++)
			{
				object *tuple = ReadObjectPlus(f);
				to->list->items[i] = tuple;
			}
		}
		else
		 to->list = NULL;
		to->type = TYPE_TUPLE;
		return((object*)to);
		}
		break;
	case TYPE_UNICODE:
		{
		unicode_object *obj = AllocUnicodeObject();
		obj->ref_count = 1;

		NUM n = (NUM)ReadLong(f);
		#ifdef USE_DEBUGGING
		char *unicode_string = (char *)mem_malloc(n + 1, "ReadObject() unicode_string");
		#else
		char *unicode_string = (char *)malloc(n + 1);
		#endif
		memset(unicode_string, 0, n + 1);
		stream_Read(f,unicode_string, n);
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
		obj->value = n;
		obj->type = TYPE_BINARY_FLOAT;
		return((object*)obj);
		}
		break;
	case TYPE_INT:
		{
		int_object *obj = AllocIntObject();
		INT n = (INT)ReadLong(f);
		obj->ref_count = 1;
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
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", type);
		#endif
		return(obj);
		}
	}
	return (NULL);
}

void WriteObjectPlus(object *obj,stream *f)
{
	WriteChar(obj->type,f);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_CREATION,"writing object with type:%c\n",obj->type);
	#endif

	switch(obj->type)
	{
		case TYPE_NONE:
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_ELLIPSIS:
		{
		return;
		}
	case TYPE_CODE:
       {
		WriteLong(((code_object*)obj)->argcount,f);
		WriteLong(((code_object*)obj)->kwonlyargcount,f);
		WriteLong(((code_object*)obj)->nlocals,f);
		WriteLong(((code_object*)obj)->co_flags,f);
		WriteObject(((code_object*)obj)->code,f);
		WriteObject(((code_object*)obj)->consts,f);
		WriteObject(((code_object*)obj)->names,f);
		WriteObject(((code_object*)obj)->varnames,f);
		WriteObject(((code_object*)obj)->freevars,f);
		WriteObject(((code_object*)obj)->cellvars,f);
		unicode_object *name = CreateUnicodeObject(((code_object*)obj)->name);
		gc_IncRefCount((object*)name);
		WriteObject((object*)name,f);
		gc_DecRefCount((object*)name);
		gc_Clear();
		return;
		}
	case TYPE_STRING:
		{
		NUM n = ((string_object*)obj)->len;
		WriteLong(n,f);
		if (n > 0)
		{
			stream_Write(f,((string_object*)obj)->content, n);
		}
		return;
		}
	case TYPE_TUPLE:
		{
		if(((tuple_object*)obj)->list != NULL)
		{
			INT n = ((tuple_object*)obj)->list->num;
			WriteLong(n,f);
			if (n > 0)
			{
				for (NUM i = 0; i < n; i++)
				{
					WriteObject(((tuple_object*)obj)->list->items[i],f);
				}
			}
		}
		else
			WriteLong(0,f);
		return;
		}
	case TYPE_UNICODE:
		{
		NUM n = strlen(((unicode_object*)obj)->value);
		WriteLong(n,f);
		if(n > 0)
		{
			stream_Write(f,((unicode_object*)obj)->value, n);
		}
		return;
		}
	case TYPE_BINARY_FLOAT:
		{
		WriteFloat(((float_object*)obj)->value,f);
		return;
		}
	case TYPE_INT:
		{
		WriteLong(((int_object*)obj)->value,f);
		return;
		}
	default:
		{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"unknown chunk type:%c\n", obj->type);
		#endif
		return;
		}
	}
}
