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
OBJECT_ID obj_NULL = 0;
OBJECT_ID obj_NONE = 0;
OBJECT_ID obj_TRUE = 0;
OBJECT_ID obj_FALSE = 0;
OBJECT_ID obj_ELLIPSIS = 0;
OBJECT_ID obj_UNKNOWN = 0;

//garbage collection globals
LIST_ID garbage;/*id_list*/
VM_ID garbage_vm;


void obj_Init(void)
{
	obj_NULL = obj_Alloc(TYPE_NULL,1); //NEEDS TO BE THE FIRST ALLOC TO GET THE ZERO ID
	obj_NONE = obj_Alloc(TYPE_NONE,1);
	obj_TRUE = obj_Alloc(TYPE_TRUE,1);
	obj_FALSE = obj_Alloc(TYPE_FALSE,1);
	obj_ELLIPSIS = obj_Alloc(TYPE_ELLIPSIS,1);
	obj_UNKNOWN = obj_Alloc(TYPE_UNKNOWN,1);
	garbage = list_Create(0,0);
	garbage_vm = 0;
}

void obj_SetVM(VM_ID vm_id) //only needed to call class destructors from obj_Free
{
	garbage_vm = vm_id;
}

void obj_Close(void)
{
	#ifdef USE_DEBUGGING
	if(debug_level & DEBUG_VERBOSE_TESTS)
	{
		//obj_Dump(obj_NULL,0,1);
		//obj_Dump(obj_NONE,0,1);
		//obj_Dump(obj_TRUE,0,1);
		//obj_Dump(obj_FALSE,0,1);
		//obj_Dump(obj_ELLIPSIS,0,1);
		//obj_Dump(obj_UNKNOWN,0,1);
	}
	#endif
	//obj_DecRefCount(obj_NULL);
	obj_DecRefCount(obj_NONE);
	obj_DecRefCount(obj_TRUE);
	obj_DecRefCount(obj_FALSE);
	obj_DecRefCount(obj_ELLIPSIS);	
	obj_DecRefCount(obj_UNKNOWN);

	obj_ClearGC();
	list_Close(garbage);
	mem_free(obj_NULL);
	garbage_vm = 0;
}

OBJECT_ID obj_Alloc(OBJECT_TYPE type,OBJECT_REF_COUNT ref_count)
{
	OBJECT_ID obj = 0;
	switch(type)
	{
		case TYPE_NULL:
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_NONE:
		case TYPE_ELLIPSIS:
		case TYPE_UNKNOWN:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(object),MEM_POOL_CLASS_PERMANENT, "AllocObject() return");
				#else
				obj = mem_malloc(sizeof(object),MEM_POOL_CLASS_PERMANENT);
				#endif
			}
			break;
		case TYPE_KV:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(kv_object),MEM_POOL_CLASS_STATIC, "AllocKVObject() return");
				#else
				obj = mem_malloc(sizeof(kv_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_CODE:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(code_object),MEM_POOL_CLASS_STATIC, "AllocCodeObject() return");
				#else
				obj = mem_malloc(sizeof(code_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_CLASS:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(class_object),MEM_POOL_CLASS_STATIC, "AllocClassObject() return");
				#else
				obj = mem_malloc(sizeof(class_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_CLASS_INSTANCE:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(class_instance_object),MEM_POOL_CLASS_STATIC, "AllocClassInstanceObject() return");
				#else
				obj = mem_malloc(sizeof(class_instance_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_FUNCTION:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(function_object),MEM_POOL_CLASS_STATIC, "AllocFunctionObject() return");
				#else
				obj = mem_malloc(sizeof(function_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_CFUNCTION:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(cfunction_object),MEM_POOL_CLASS_STATIC, "AllocCFunctionObject() return");
				#else
				obj = mem_malloc(sizeof(cfunction_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_METHOD:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(method_object),MEM_POOL_CLASS_STATIC, "AllocMethodObject() return");
				#else
				obj = mem_malloc(sizeof(method_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_STRING:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(string_object),MEM_POOL_CLASS_STATIC, "AllocStringObject() return");
				#else
				obj = mem_malloc(sizeof(string_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_TUPLE:
		case TYPE_DICT:
		case TYPE_LIST:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(tuple_object),MEM_POOL_CLASS_STATIC, "AllocTupleObject() return");
				#else
				obj = mem_malloc(sizeof(tuple_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_UNICODE:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(unicode_object),MEM_POOL_CLASS_STATIC, "AllocUnicodeObject() return");
				#else
				obj = mem_malloc(sizeof(unicode_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_TAG:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(tag_object),MEM_POOL_CLASS_STATIC, "AllocTagObject() return");
				#else
				obj = mem_malloc(sizeof(tag_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_INT:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(int_object),MEM_POOL_CLASS_STATIC, "AllocIntObject() return");
				#else
				obj = mem_malloc(sizeof(int_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_BINARY_FLOAT:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(float_object),MEM_POOL_CLASS_STATIC, "AllocFloatObject() return");
				#else
				obj = mem_malloc(sizeof(float_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_ITER:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(iter_object),MEM_POOL_CLASS_STATIC, "AllocIterObject() return");
				#else
				obj = mem_malloc(sizeof(iter_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
		case TYPE_BLOCK:
			{
				#ifdef USE_MEMORY_DEBUGGING
				obj = mem_malloc_debug(sizeof(block_object),MEM_POOL_CLASS_STATIC, "AllocBlockObject() return");
				#else
				obj = mem_malloc(sizeof(block_object),MEM_POOL_CLASS_STATIC);
				#endif
			}
			break;
			
	}
	object *o = (object*)mem_lock(obj);
	o->type = type;
	o->ref_count = ref_count;
	mem_unlock(obj,1);
	return(obj);
}

OBJECT_ID obj_CreateTag(void *tag)
{
	OBJECT_ID r = obj_Alloc(TYPE_TAG,0);
	tag_object *l = (tag_object*)mem_lock(r);
	l->tag = tag;
	mem_unlock(r,1);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created tag object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateInt(INT value)
{
	OBJECT_ID r = obj_Alloc(TYPE_INT,0);
	int_object *l = (int_object*)mem_lock(r);
	l->value = value;
	mem_unlock(r,1);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created int object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateFloat(FLOAT value)
{
	OBJECT_ID r = obj_Alloc(TYPE_BINARY_FLOAT,0);
	float_object *l = (float_object*)mem_lock(r);
	l->value = value;
	mem_unlock(r,1);
	
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created float object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateUnicode(MEM_ID value) /*char* */
{
	OBJECT_ID r = obj_Alloc(TYPE_UNICODE,0);
	unicode_object *l = (unicode_object*)mem_lock(r);
	l->value = value;
	mem_unlock(r,1);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created unicode object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateTuple(NUM num)
{
	OBJECT_ID r = obj_Alloc(TYPE_TUPLE,0);
	tuple_object *l = (tuple_object*)mem_lock(r);
	l->ptr = 0;
	l->list = list_Create(num,LIST_STATIC);
	mem_unlock(r,1);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateString(MEM_ID bytes,NUM len) /*char* */
{
	OBJECT_ID r = obj_Alloc(TYPE_STRING,0);
	string_object *l = (string_object*)mem_lock(r);
	l->content = bytes;
	l->len = len;
	mem_unlock(r,1);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created string object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateKV(OBJECT_ID key,OBJECT_ID value)
{
	OBJECT_ID r = obj_Alloc(TYPE_KV,0);
	kv_object *l = (kv_object*)mem_lock(r);
	l->key = key;
	obj_IncRefCount(key);
	l->value = value;
	obj_IncRefCount(value);
	mem_unlock(r,1);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created kv object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateEmpty(char type)
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

OBJECT_ID obj_CreateFunction(OBJECT_ID function_code,OBJECT_ID defaults,OBJECT_ID kw_defaults,OBJECT_ID closure)
{
	OBJECT_ID r = obj_Alloc(TYPE_FUNCTION,0);
	function_object *l = (function_object*)mem_lock(r);
	l->func = function_code;
	obj_IncRefCount(function_code);
	mem_unlock(r,1);
	
	code_object *fc = (code_object*)mem_lock(function_code);
	
	if(tuple_GetLen(fc->varnames) > 0)
	{
		if(defaults != 0 || kw_defaults != 0)
			tuple_ClearDictValues(fc->varnames);
		if(defaults != 0)
		{
			//printf("loading defaults\n");
			NUM locals_num = fc->argcount;
			//tuple_object *d = (tuple_object*)mem_lock(defaults);		
			for(NUM i = 0;i < tuple_GetLen(defaults); i++) //load default values
			{
				OBJECT_ID item = tuple_GetItem(fc->varnames,(locals_num-1) - i);
				object *oi = (object*)mem_lock(item);
				if(oi->type != TYPE_KV)
				{
					tuple_SetItem(fc->varnames,(locals_num-1) - i,obj_ConvertToValuedKV(tuple_GetItem(fc->varnames,(locals_num-1) - i),tuple_GetItem(defaults,i)));
				}		
				else
				{
					tuple_SetDictItemByIndex(fc->varnames,(locals_num-1) - i,tuple_GetItem(defaults,i));
				}
				mem_unlock(item,0);
			}
			//mem_unlock(defaults,0);
		}
		
		if(kw_defaults != 0)
		{
			//tuple_object *kw = (tuple_object*)mem_lock(kw_defaults);		
			//printf("loading kw defaults\n");
			for(NUM i = 0;i < tuple_GetLen(kw_defaults); i++) //load default keyword values
			{
				OBJECT_ID kv = tuple_GetItem(kw_defaults,i);
				kv_object *k = (kv_object*)mem_lock(kv);		
				tuple_SetDictItem(fc->varnames,k->key,k->value);
				mem_unlock(kv,0);
			}
			//mem_unlock(kw_defaults,0);
		}
	}
	
	if(closure != 0)
	{
		
		//printf("loading closure defaults\n");
		for (NUM i = 0; i < tuple_GetLen(closure); i++)
		{
			OBJECT_ID local = tuple_GetItem(closure,i);
			tuple_SetItem(fc->freevars,i,local);
		}
	}
	mem_unlock(function_code,0);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created function object\n");
		obj_Dump(r,0,1);
	}
	#endif
	//FullDumpObject(r,0);
	return(r);
}

OBJECT_ID obj_CreateCFunction(OBJECT_ID (*func) (VM_ID vm_id,OBJECT_ID locals,OBJECT_ID kw_locals))
{
	OBJECT_ID r = obj_Alloc(TYPE_CFUNCTION,0);
	cfunction_object *l = (cfunction_object*)mem_lock(r);
	l->func = func;
	mem_unlock(r,1);

	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created cfunction object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateMethod(OBJECT_ID func,OBJECT_ID class_instance)
{
	OBJECT_ID r = obj_Alloc(TYPE_METHOD,0);
	method_object *l = (method_object*)mem_lock(r);
	l->func = func;
	obj_IncRefCount(l->func);
	l->instance = class_instance;
	obj_IncRefCount(l->instance);
	mem_unlock(r,1);

	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created method object\n");
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateClass(OBJECT_ID code,OBJECT_ID base_classes)
{
	OBJECT_ID r = obj_Alloc(TYPE_CLASS,0);
	class_object *l = (class_object*)mem_lock(r);
	l->code = code;
	l->base_classes = base_classes;
	obj_IncRefCount(code);
	obj_IncRefCount(base_classes);
	mem_unlock(r,1);
	code_object *c = (code_object*)mem_lock(code);
	c->co_flags |= CO_CLASS_ROOT;
	mem_unlock(code,1);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created class object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateCode(MEM_ID name) /*char* */
{
	OBJECT_ID r = obj_Alloc(TYPE_CODE,0);
	code_object *l = (code_object*)mem_lock(r);
	l->name = name;
	l->argcount = 0;
	l->kwonlyargcount = 0;
	l->nlocals = 0;
	l->stacksize = 0;
	l->co_flags = 0;
	l->code = 0;
	l->consts = 0;
	l->varnames = 0;
	l->freevars = 0;
	l->cellvars = 0;
	l->names = 0;
	mem_unlock(r,1);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created code object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateClassInstance(OBJECT_ID instance_of)
{
	OBJECT_ID r = obj_Alloc(TYPE_CLASS_INSTANCE,0);
	class_instance_object *l = (class_instance_object*)mem_lock(r);
	l->instance_of = instance_of;
	obj_IncRefCount(instance_of);
	l->methods = obj_CreateTuple(0);//this is a kv list of cfunctions or functions ->key is an unicode object with the methods name as value
	obj_IncRefCount(l->methods);
	l->vars = obj_CreateTuple(0);
	obj_IncRefCount(l->vars);
	mem_unlock(r,1);
	
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created class instance object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}

OBJECT_ID obj_CreateIter(void)
{
	OBJECT_ID r = obj_Alloc(TYPE_ITER,0);
	iter_object *l = (iter_object*)mem_lock(r);
	l->block_stack = 0;
	l->tag = 0;
	mem_unlock(r,1);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_CREATION) > 0)
	{
		debug_printf(DEBUG_CREATION,"created iter object\n");
		obj_Dump(r,0,1);
	}
	#endif
	return(r);
}
 
OBJECT_ID obj_ConvertToKV(OBJECT_ID obj) /*returns kv_object*/
{
	return(obj_CreateKV(obj,0));
}

OBJECT_ID obj_ConvertToValuedKV(OBJECT_ID key,OBJECT_ID value)
{
	return(obj_CreateKV(key,value));
}

#ifndef USE_ARDUINO_FUNCTIONS
void obj_Print(OBJECT_ID obj)
{
	if(obj != 0)
	{
		object *o = (object*)mem_lock(obj);
		switch(o->type)
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
			printf("%llu", ((int_object*)o)->value);
			break;
		case TYPE_BINARY_FLOAT:
			printf("%7g", ((float_object*)o)->value);
			break;
		case TYPE_KV:
			obj_Print(((kv_object*)o)->value);
			break;
		case TYPE_UNICODE:
			{
				char *string = (char*)mem_lock(((unicode_object*)o)->value);
				printf("%s", string);
				mem_unlock(((unicode_object*)o)->value,0);
			}
			break;
		case TYPE_TUPLE:
			{
				for(NUM i = 0;i<tuple_GetLen(obj);i++)
				{
					if(i != 0)
						printf(", ");
					OBJECT_ID ti = tuple_GetItem(obj,i);
					if(ti != obj_NULL && ti != obj_NONE)
						obj_Print(ti);
				}
			}
			break;
		}
		mem_unlock(obj,0);
	}
}
#endif

#ifdef USE_DEBUGGING	

void obj_PrintTabs(char num)
{
	for(char i = 0; i < num; i++)
		debug_printf(DEBUG_ALL,"\t");
}

void obj_Dump(OBJECT_ID obj_id, char level,char verbosity)
{
	//printf("obj_Dump id:%d\n",obj);
	if(obj_id == 0)
		return;
	object *obj = (object*)mem_lock(obj_id);
	obj_PrintTabs(level);
	debug_printf(DEBUG_ALL,"<object id=\"%d\" ref_count=\"%d\">\n",obj_id,obj->ref_count);
	switch(obj->type)
	{
	case TYPE_BLOCK:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>block object</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<stack id=\"%d\"></stack>\n",((block_object*)obj)->stack);
			obj_Dump(((block_object*)obj)->code,level +1 ,verbosity);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<start>%d</start>\n",((block_object*)obj)->start);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<len>%d</len>\n",((block_object*)obj)->len);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<ip>%d</ip>\n",((block_object*)obj)->ip);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_ITER:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>iter</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<iter tag>\n");
			obj_Dump(((iter_object*)obj)->tag,level + 1,verbosity);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</iter tag>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<iter block stack>\n");
			stack_Dump(((iter_object*)obj)->block_stack);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</iter block stack>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_ELLIPSIS:
		{
			for(char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"<type>ellipsis</type>\n");
			for(char i = 0; i < level; i++)
				debug_printf(DEBUG_ALL,"\t");
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_TRUE:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>true</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_FALSE:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>false</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
			}
		break;
	case TYPE_NULL:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>NULL</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_NONE:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>NONE</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_BINARY_FLOAT:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>float</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<value>%7g</value>\n", ((float_object*)obj)->value);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_INT:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>int</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<value>%d</value>\n", ((int_object*)obj)->value);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_KV:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>kv</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<key>\n");
			obj_Dump(((kv_object*)obj)->key,level + 1,verbosity);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</key>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<value>\n");
			obj_Dump(((kv_object*)obj)->value,level + 1,verbosity);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</value>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_FUNCTION:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>function</type\n");
			obj_PrintTabs(level);
			code_object *fo = (code_object*)mem_lock(((function_object*)obj)->func);
			char *name = (char*)mem_lock(fo->name);
			debug_printf(DEBUG_ALL,"<name>%s</name>\n",name);
			mem_unlock(fo->name,0);
			mem_unlock(((function_object*)obj)->func,0);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<code>\n");
			obj_Dump(((function_object*)obj)->func,level + 1,verbosity);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</code>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_CFUNCTION:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>cfunction</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<address>%x</address>\n", ((cfunction_object*)obj)->func);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_UNICODE:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>unicode</type>\n");
			obj_PrintTabs(level);
			char *value = (char*)mem_lock(((unicode_object*)obj)->value);
			debug_printf(DEBUG_ALL,"<value>%s</value>\n", value);
			mem_unlock(((unicode_object*)obj)->value,0);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_STRING:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>string</type>\n");
			obj_PrintTabs(level);
			char *content = (char*)mem_lock(((string_object*)obj)->content);
			debug_printf(DEBUG_ALL,"<content>\n");
			obj_PrintTabs(level + 1);
			for (NUM i = 0; i < ((string_object*)obj)->len; i++)
				debug_printf(DEBUG_ALL,"%02x ", (unsigned char)content[i]);
			mem_unlock(((string_object*)obj)->content,0);
			debug_printf(DEBUG_ALL,"\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</content>\n");
			obj_PrintTabs(level);			
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_TUPLE:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>tuple</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<items_num>%d</items_num>\n", tuple_GetLen(obj_id));
			if(tuple_GetLen(obj_id) > 0)
			{
				for (NUM i = 0; i < tuple_GetLen(obj_id); i++)
				{
					OBJECT_ID it = tuple_GetItem(obj_id,i);
					if(it != 0)
					{
						obj_PrintTabs(level);
						if(i == ((tuple_object*)obj)->ptr)
							debug_printf(DEBUG_ALL,"<item index=\"%d\" ptr=\"true\">\n",i);
						else
							debug_printf(DEBUG_ALL,"<item index=\"%d\">\n",i);
						obj_Dump(it, level + 1,verbosity);
						obj_PrintTabs(level);
						debug_printf(DEBUG_ALL,"</item>\n");
					}
				}
			}
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_CODE:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>code</type>\n");
			obj_PrintTabs(level);
			char *name = (char*)mem_lock(((code_object*)obj)->name);
			debug_printf(DEBUG_ALL,"<name>%s</name>\n", name);
			mem_unlock(((code_object*)obj)->name,0);
			if(verbosity)
			{
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<flags>%d</flags>\n",((code_object*)obj)->co_flags);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<nlocals>%d</nlocals>\n",((code_object*)obj)->nlocals);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<argcount>%d</argcount>\n",((code_object*)obj)->argcount);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<kwonlyargument>%d</kwonlyargument>\n",((code_object*)obj)->kwonlyargcount);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<code>\n");
				obj_Dump(((code_object*)obj)->code, level + 1,verbosity);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"</code>\n");
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<consts>\n");
				obj_Dump(((code_object*)obj)->consts, level + 1,verbosity);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"</consts>\n");
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<names>\n");
				obj_Dump(((code_object*)obj)->names, level + 1,verbosity);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"</names>\n");
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<varnames>\n");
				obj_Dump(((code_object*)obj)->varnames, level + 1,verbosity);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"</varnames>\n");
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<freevars>\n");
				obj_Dump(((code_object*)obj)->freevars, level + 1,verbosity);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"</freevars>\n");
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"<cellvars>\n");
				obj_Dump(((code_object*)obj)->cellvars, level + 1,verbosity);
				obj_PrintTabs(level);
				debug_printf(DEBUG_ALL,"</cellvars>\n");
			}
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_CLASS_INSTANCE:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>class_instance</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_CLASS:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>class</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_METHOD:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>method</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	case TYPE_TAG:
		{
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<type>tag</type>\n");
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"<tag>%x</tag>\n",((tag_object*)obj)->tag);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
		break;
	default:
		{
			debug_printf(DEBUG_ALL,"<type id=\"%c\">unknown</type>\n", obj->type);
			obj_PrintTabs(level);
			debug_printf(DEBUG_ALL,"</object>\n");
		}
	}
	mem_unlock(obj_id,0);
}
#endif

BOOL obj_Compare(OBJECT_ID a,OBJECT_ID b)//TODO add tuple compare
{
	if(a == 0 && b == 0)
		return(0);
	if(a== 0 || b == 0)
		return (1);
	if(a == b)
		return(0);
	object *oa = (object*)mem_lock(a);
	object *ob = (object*)mem_lock(b);
	if(oa->type != ob->type)
	{
		mem_unlock(b,0);
		mem_unlock(a,0);
		return(1);
	}
	switch(oa->type)
	{
	case TYPE_NULL:
		{
			mem_unlock(b,0);
			mem_unlock(a,0);
			return(0);
		}
	case TYPE_NONE:
		{
			mem_unlock(b,0);
			mem_unlock(a,0);
			return(0);
		}
	case TYPE_KV:
		 {
			BOOL r = obj_Compare(((kv_object*)oa)->key,((kv_object*)ob)->key) && obj_Compare(((kv_object*)oa)->value,((kv_object*)ob)->value);
			mem_unlock(b,0);
			mem_unlock(a,0);
			return(r);
		}
	case TYPE_INT:
		{
			BOOL r = !(((int_object*)oa)->value == ((int_object*)ob)->value);
			mem_unlock(b,0);
			mem_unlock(a,0);
			return(r);
		}
	case TYPE_BINARY_FLOAT:
		{
			BOOL r = !(((float_object*)oa)->value == ((float_object*)ob)->value);
			mem_unlock(b,0);
			mem_unlock(a,0);
			return(r);
		}
	case TYPE_UNICODE:
		{
			char *sa = (char*)mem_lock(((unicode_object*)oa)->value);
			char *sb = (char*)mem_lock(((unicode_object*)ob)->value);
			BOOL r = strcmp(sa,sb);
			mem_unlock(((unicode_object*)ob)->value,0);
			mem_unlock(((unicode_object*)oa)->value,0);
			mem_unlock(b,0);
			mem_unlock(a,0);
			return(r);
		}
	case TYPE_STRING:
		break;
	case TYPE_TUPLE:
		break;
	case TYPE_CODE:
		break;
	}
	mem_unlock(b,0);
	mem_unlock(a,0);
	return(-1);
}

OBJECT_ID obj_Copy(OBJECT_ID obj_id)
{	
	object *obj = (object*)mem_lock(obj_id);
	switch(obj->type)
	{
		case TYPE_TRUE:
		case TYPE_FALSE:
		case TYPE_NULL:
		case TYPE_NONE:
			{
				OBJECT_ID r = obj_CreateEmpty(obj->type); 
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_INT:
			{
				OBJECT_ID r = obj_CreateInt(((int_object*)obj)->value);
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_BINARY_FLOAT:
			{
				OBJECT_ID r = obj_CreateFloat(((float_object*)obj)->value);
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_UNICODE:
			{
				//char *value = (char*)mem_lock(((unicode_object*)o)->value);
				//OBJECT_ID r = obj_CreateUnicode(str_Copy(value));
				OBJECT_ID r = obj_CreateUnicode(mem_copy(((unicode_object*)obj)->value));
				//mem_unlock(((unicode_object*)o)->value,0);
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_STRING:
			{
				OBJECT_ID r = obj_CreateString(mem_copy(((string_object*)obj)->content),((string_object*)obj)->len);
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_KV:
			{
				OBJECT_ID r = obj_CreateKV(obj_Copy(((kv_object*)obj)->key),obj_Copy(((kv_object*)obj)->value));
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_TAG:
			{
				OBJECT_ID r = obj_CreateTag(((tag_object*)obj)->tag);
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_METHOD:
			{
				OBJECT_ID r = obj_CreateMethod(obj_Copy(((method_object*)obj)->func),((method_object*)obj)->instance);
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_CFUNCTION:
			{
				OBJECT_ID r = obj_CreateCFunction(((cfunction_object*)obj)->func);
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_CLASS:
			{
				OBJECT_ID r = obj_CreateClass(obj_Copy(((class_object*)obj)->code),((class_object*)obj)->base_classes);//TODO maybe copy base classes too
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_CLASS_INSTANCE:
			{
				OBJECT_ID r = obj_CreateClassInstance(((class_instance_object*)obj)->instance_of);
				mem_unlock(obj_id,0);
				return(r);
			}
		case TYPE_TUPLE:
			{
				OBJECT_ID t = obj_CreateTuple(tuple_GetLen(obj_id));
				for(INDEX i = 0;i<tuple_GetLen(obj_id);i++)
					tuple_SetItem(t,i,obj_Copy(tuple_GetItem(obj_id,i)));
				mem_unlock(obj_id,0);
				return(t);
			}
		case TYPE_CODE:
			{
				OBJECT_ID c_id = obj_Alloc(TYPE_CODE,0);
				code_object *c = (code_object*)mem_lock(c_id);
				c->name = mem_copy(((code_object*)obj)->name);
				c->argcount = ((code_object*)obj)->argcount;
				c->kwonlyargcount = ((code_object*)obj)->kwonlyargcount;
				c->nlocals = ((code_object*)obj)->nlocals;
				c->stacksize = ((code_object*)obj)->stacksize;	
				c->co_flags = ((code_object*)obj)->co_flags;
				c->code = ((code_object*)obj)->code;
				obj_IncRefCount(c->code);
				c->consts = ((code_object*)obj)->consts;
				obj_IncRefCount(c->consts);
				c->varnames = obj_Copy(((code_object*)obj)->varnames);
				obj_IncRefCount(c->varnames);
				c->freevars = ((code_object*)obj)->freevars;
				obj_IncRefCount(c->freevars);
				c->cellvars = ((code_object*)obj)->cellvars;
				obj_IncRefCount(c->cellvars);
				c->names = obj_Copy(((code_object*)obj)->names);
				obj_IncRefCount(c->names);
				mem_unlock(c_id,1);
				mem_unlock(obj_id,0);
				return(c_id);
			}
		case TYPE_FUNCTION:
			{
				OBJECT_ID f = obj_CreateFunction(obj_Copy(((function_object*)obj)->func),0,0,0);
				mem_unlock(obj_id,0);
				return(f);
			}
		case TYPE_BLOCK:
			{
			}
			break;
		case TYPE_ITER:
			{
			}
			break;
	}
	return(0);
}

OBJECT_ID obj_GetAttribute(OBJECT_ID obj,OBJECT_ID key)
{
	if(obj == 0 || key == 0)
		return(0);
	OBJECT_ID r = 0;
	object *o = (object*)mem_lock(obj);
	if(o->type == TYPE_CODE)
	{
		r = tuple_GetDictItem(((code_object*)o)->names,key);
	}else if(o->type == TYPE_CLASS)
	{
		code_object *cco = (code_object*)mem_lock(((class_object*)o)->code);
		r = tuple_GetDictItem(cco->names,key);
		mem_unlock(((class_object*)o)->code,0);
	}else if(o->type == TYPE_CLASS_INSTANCE)
	{
		r = obj_GetClassVar(obj,key);
		if(r == 0)
		{
			r = obj_GetClassMethod(obj,key);
			if(r != 0)
			{	
				mem_unlock(obj,0);
				OBJECT_ID mo = obj_CreateMethod(r,obj);
				return(mo);
			}
		}
		if(r == 0)
			r = obj_GetAttribute(((class_instance_object*)o)->instance_of,key);
	}
	else
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"object type:%c has no attributes\n",o->type);
		obj_Dump(obj,0,1);
		#endif
	}
	mem_unlock(obj,0);
	return(r);
}

/*object *GetAttributeByName(object *obj,char *name)
{


}*/

void obj_SetAttribute(OBJECT_ID obj,OBJECT_ID key,OBJECT_ID value)
{
	object *o = (object*)mem_lock(obj);
	if(o->type == TYPE_CLASS_INSTANCE)
	{
		tuple_SetDictItem(((class_instance_object*)o)->vars,key,value);
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
	mem_unlock(obj,0);
}

OBJECT_ID obj_GetClassMethod(OBJECT_ID _class,OBJECT_ID key)
{
	OBJECT_ID r = 0;
	object *c = (object*)mem_lock(_class);
	if(c->type == TYPE_CLASS)
	{
		code_object *cco = (code_object*)mem_lock(((class_object*)c)->code);
		r = tuple_GetDictItem(cco->names,key);
		mem_unlock(((class_object*)c)->code,0);
		if(!(obj_GetType(r) == TYPE_FUNCTION || obj_GetType(r) == TYPE_CFUNCTION))
			r = 0;
		if(r == 0)//TODO check if this still works
		{	
			OBJECT_ID bc = ((class_object*)c)->base_classes;
			//object *obc = (object*)mem_lock(((class_object*)c)->base_classes);
			for(INDEX i = 0;i<tuple_GetLen(bc);i++)
			{
				r = obj_GetClassMethod(tuple_GetItem(bc,i),key);
				if(r != 0)
					break;
			}
			//mem_unlock(
		}
	}
	else
	if(c->type == TYPE_CLASS_INSTANCE)
	{
		r = tuple_GetDictItem(((class_instance_object*)c)->methods,key);
		if(r == 0)
		{
			if(r == 0)
				r = obj_GetClassMethod(((class_instance_object*)c)->instance_of,key);
		}
	}
	mem_unlock(_class,0);
	return(r);
}

OBJECT_ID obj_GetClassVar(OBJECT_ID _class,OBJECT_ID key)
{
	OBJECT_ID r = 0;
	object *c = (object*)mem_lock(_class);
	if(c->type == TYPE_CLASS_INSTANCE)
	{
		r = tuple_GetDictItem(((class_instance_object*)c)->vars,key);
	}
	mem_unlock(_class,0);
	return(r);
}

void obj_AddCodeName(OBJECT_ID code_object,OBJECT_ID key,OBJECT_ID value)
{
	struct _code_object *c = (struct _code_object*)mem_lock(code_object);
	c->nlocals++;
	if(c->names == 0)
	{
		c->names = obj_CreateTuple(0);
		obj_IncRefCount(c->names);
	}
	OBJECT_ID kv = obj_CreateKV(key,value);
	tuple_AppendItem(c->names,kv);
	mem_unlock(code_object,1);
}

void obj_AddCodeFunction(OBJECT_ID code_object,BYTES_ID name,OBJECT_ID func)
{
	//OBJECT_ID key = obj_CreateUnicode(mem_copy(name));
	OBJECT_ID key = obj_CreateUnicode(name);
	obj_AddCodeName(code_object,key,func);
}

void obj_AddCodeCFunction(OBJECT_ID code_object,BYTES_ID name, OBJECT_ID (*func) (VM_ID vm_id,OBJECT_ID locals,OBJECT_ID kw_locals))
{
	OBJECT_ID cfo = obj_CreateCFunction(func);
	obj_AddCodeFunction(code_object,name,cfo);
}

NUM obj_GetUnicodeLen(OBJECT_ID obj)
{
	unicode_object *u = (unicode_object*)mem_lock(obj);
	char *string = (char*)mem_lock(u->value);
	NUM r = strlen(string);
	mem_unlock(u->value,0);
	mem_unlock(obj,0);
	return(r);
}
	
void obj_IncRefCount(OBJECT_ID obj_id)
{
	if(obj_id == 0)
		return;
	object *obj = mem_lock(obj_id);
	//if(obj->type == TYPE_NONE)
	//	printf("global NONE object ref_count:%d about to be incremented\n",obj->ref_count);
	obj->ref_count++;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_GC) > 0)
	{
		debug_printf(DEBUG_GC,"%d : %d refs (incremented:%c)\n",obj_id,obj->ref_count,obj->type);
	}
	#endif
	mem_unlock(obj_id,1);
}

void obj_DecRefCount(OBJECT_ID obj_id)
{
	if(obj_id == 0)
		return;
	object *obj = (object*)mem_lock(obj_id);
	#ifdef USE_DEBUGGING
	if(obj->type == TYPE_NONE && obj->ref_count == 1)
		debug_printf(DEBUG_VERBOSE_TESTS,"WARNING global object ref_count reached 1\n");
	#endif
	//if(obj->type == TYPE_NONE)
	//	printf("global NONE object ref_count:%d about to be decremented\n",obj->ref_count);
	if(obj->ref_count == 1)
	 {
		obj->ref_count--;
		if(!list_Contains(garbage,obj_id))
		{
			#ifdef USE_DEBUGGING
			if((debug_level & DEBUG_GC) > 0)
			{
				//debug_printf(DEBUG_GC,"object has no refs anymore -> put into gc\n");
				debug_printf(DEBUG_GC,"%d : 0 refs (decremented[GC]:%c - pushed to GC)\n",obj_id,obj->type);
				//DumpObject(obj,0);
			}
			#endif
			//FreeObject(obj);
			list_Queue(garbage,obj_id);
		}
		mem_unlock(obj_id,1);
		return;
	 }
	if(obj->ref_count > 1)
		obj->ref_count--;
	else
	{
			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_ALL,"possibly freed object is still be referenced by objects: %d\n",obj_id);
			obj_Dump(obj_id,0,0);
			if((debug_level & DEBUG_GC) > 0)
			{
				debug_printf(DEBUG_GC,"possibly freed object is still be referenced by objects: %d\n",obj_id);
			}
			#endif
	}
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_GC,"%d : %d refs (decremented[GC]:%c)\n",obj_id,obj->ref_count,obj->type);
	#endif
	mem_unlock(obj_id,1);
}

BOOL obj_HasNoRefs(OBJECT_ID obj)
{
	if(obj == 0)
		return(1);
	object *o = (object*)mem_lock(obj);
	BOOL r = !o->ref_count;
	mem_unlock(obj,0);
	return(r);
}

BOOL obj_HasRefs(OBJECT_ID obj)
{
	if(obj == 0)
		return(0);
	object *o = (object*)mem_lock(obj);
	BOOL r = o->ref_count;
	mem_unlock(obj,0);
	return(r);
}

void obj_Free(OBJECT_ID obj_id)
{
	if(obj_id == 0)
		return;
	object *obj = (object*)mem_lock(obj_id);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_FREEING) > 0)
	{
		debug_printf(DEBUG_FREEING,"object to be freed\n");
		obj_Dump(obj_id,1,1);
	}
	if(obj->ref_count < 0)
		debug_printf(DEBUG_ALL,"ref skew detected:%d\n",obj_id);
	#endif
	
	if(obj->ref_count > 1)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_GC) > 0)
		{
			debug_printf(DEBUG_GC,"%d : %d (decreasing due FreeObject)\n",obj_id,obj->ref_count);
		}
		debug_printf(DEBUG_ALL,"Impossible\n");
		#endif
		mem_unlock(obj_id,0);
		return;
	}

	switch(obj->type)
	{
		case TYPE_BLOCK:
			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_VERBOSE_FREEING,"Freeing block object %d\n",obj_id);
			#endif
			obj_DecRefCount(((block_object*)obj)->code);
			if(((block_object*)obj)->stack != 0)
				stack_Close(((block_object*)obj)->stack,1);
			break;
		case TYPE_ITER:
			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_VERBOSE_FREEING,"Freeing Iter %d\n",obj_id);
			#endif
			if(((iter_object*)obj)->block_stack != 0)
			{	
				stack_Close(((iter_object*)obj)->block_stack,1);
			}
			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_VERBOSE_FREEING,"Freeing Iter tag %d\n",((iter_object*)obj)->tag);
			#endif
			obj_DecRefCount(((iter_object*)obj)->tag);
			break;
		case TYPE_NULL:
			break;
		case TYPE_NONE:
			break;
		case TYPE_INT:
			break;
		case TYPE_BINARY_FLOAT:
			break;
		case TYPE_KV:
			obj_DecRefCount(((kv_object*)obj)->key);
			obj_DecRefCount(((kv_object*)obj)->value);
			break;
		case TYPE_FUNCTION:
			obj_DecRefCount(((function_object*)obj)->func);
			break;
		case TYPE_CFUNCTION:
			break;
		case TYPE_UNICODE:
			if(((unicode_object*)obj)->value != 0)
			{
				#ifdef USE_DEBUGGING
				debug_printf(DEBUG_VERBOSE_FREEING,"freeing unicode object %d\n",obj_id);
				#endif
				mem_free(((unicode_object*)obj)->value);
			}
			break;
		case TYPE_STRING:
			if(((string_object*)obj)->content != 0)
				mem_free(((string_object*)obj)->content);
			break;
		case TYPE_TUPLE:
			if(tuple_GetLen(obj_id) > 0)
			{
				for(NUM i = 0; i < tuple_GetLen(obj_id); i++)
				{
					obj_DecRefCount(tuple_GetItem(obj_id,i));
				}
			}
			if(((tuple_object*)obj)->list != 0)
				list_Close(((tuple_object*)obj)->list);
			break;
		case TYPE_CODE:
			if(((code_object*)obj)->name != 0)
			{
				mem_free(((code_object*)obj)->name);
			}
			obj_DecRefCount(((code_object*)obj)->code);
			obj_DecRefCount(((code_object*)obj)->consts);
			obj_DecRefCount(((code_object*)obj)->names);
			obj_DecRefCount(((code_object*)obj)->varnames);
			obj_DecRefCount(((code_object*)obj)->freevars);
			obj_DecRefCount(((code_object*)obj)->cellvars);
			break;
		case TYPE_CLASS:
			obj_DecRefCount(((class_object*)obj)->code);
			obj_DecRefCount(((class_object*)obj)->base_classes);
			break;
		case TYPE_CLASS_INSTANCE:
			{
				if(garbage_vm != 0)
				{	
					//printf("calling class instance destructor\n");
					OBJECT_ID method_name = obj_CreateUnicode(mem_create_string("__del__"));
					OBJECT_ID locals = obj_CreateTuple(0);
					obj_IncRefCount(locals);
					list_Push(garbage,obj_id);
					OBJECT_ID rmr = vm_RunMethod(garbage_vm,method_name,obj_id,locals,0);
					if(tuple_GetLen(locals)>0)
					{
						//tuple_SetItem(locals,0,obj_CreateEmpty(TYPE_NULL));
						tuple_SetItem(locals,0,0);//removing loop reference of class instance
					}
					list_RemoveItem(garbage,obj_id);
					//printf("destruction completed\n");
					obj_DecRefCount(locals);
					obj_IncRefCount(rmr);
					obj_DecRefCount(rmr);
					obj_IncRefCount(method_name);
					obj_DecRefCount(method_name);
				}
			obj_DecRefCount(((class_instance_object*)obj)->instance_of);
			obj_DecRefCount(((class_instance_object*)obj)->methods);
			obj_DecRefCount(((class_instance_object*)obj)->vars);
		}
		break;
		case TYPE_METHOD:
			obj_DecRefCount(((method_object*)obj)->func);
			obj_DecRefCount(((method_object*)obj)->instance);
			break;
		case TYPE_TAG:
			break;
	}
	mem_unlock(obj_id,0);
	mem_free(obj_id);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_FREEING,"freed object:%x\n",obj_id);
	#endif
}

void obj_ClearGC(void)
{
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_GC,"obj_ClearGC(%d);\n",list_GetLen(garbage));
	#endif
	while(list_GetLen(garbage))
	{
		OBJECT_ID g = list_Pop(garbage);
		if(obj_HasNoRefs(g))
		{
			#ifdef USE_DEBUGGING
			if((debug_level & DEBUG_GC) > 0)
			{
				object *go = (object*)mem_lock(g);
				debug_printf(DEBUG_GC,"%d : %d (killed:%c)\n",g,go->ref_count,go->type);
				mem_unlock(g,0);
			}
			#endif
			obj_Free(g);
		}
		else
		{
			#ifdef USE_DEBUGGING
			if((debug_level & DEBUG_GC) > 0)
			{
				object *go = (object*)mem_lock(g);
				debug_printf(DEBUG_GC,"%d : %d (survived:%c)\n",g,go->ref_count,go->type);
				mem_unlock(g,0);
			}
			#endif
		}
	}
}

OBJECT_TYPE obj_GetType(OBJECT_ID obj)
{
	if(obj == 0)
		return(0);
	object *o = (object*)mem_lock(obj);
	OBJECT_TYPE r = o->type;
	mem_unlock(obj,0);
	return(r);
}






