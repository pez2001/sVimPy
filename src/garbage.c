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

#include "garbage.h"

ptr_list *garbage;

void gc_Init(void)
{
	garbage = ptr_CreateList(0,0);
}

void gc_Close(void)
{
	gc_Clear();
	ptr_CloseList(garbage);
}
	
void gc_IncRefCount(object *obj)
{
	if(obj == NULL)
		return;
	obj->ref_count++;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_GC) > 0)
	{
		debug_printf(DEBUG_GC,"%x : %d refs (incremented:%c)\n",obj,obj->ref_count,obj->type);
	}
	#endif
}

void gc_DecRefCount(object *obj)
{
	if(obj == NULL)
		return;
	if(obj->ref_count == 1)
	 {
		obj->ref_count--;
		if(!ptr_Contains(garbage,obj))
		{
			#ifdef USE_DEBUGGING
			if((debug_level & DEBUG_GC) > 0)
			{
				//debug_printf(DEBUG_GC,"object has no refs anymore -> put into gc\n");
				debug_printf(DEBUG_GC,"%x : 0 refs (decremented[GC]:%c - pushed to GC)\n",obj,obj->type);
				//DumpObject(obj,0);
			}
			#endif
			//FreeObject(obj);
			ptr_Queue(garbage,obj);
		}
		return;
	 }
	if(obj->ref_count > 1)
		obj->ref_count--;
	else
	{
			#ifdef USE_DEBUGGING
			if((debug_level & DEBUG_GC) > 0)
			{
				debug_printf(DEBUG_GC,"possibly freed object is still be referenced by objects: %x\n",obj);
			}
			#endif
	}
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_GC,"%x : %d refs (decremented[GC]:%c)\n",obj,obj->ref_count,obj->type);
	#endif
}

BOOL gc_HasNoRefs(object *obj)
{
	if(obj == NULL)
		return(1);
	return(!obj->ref_count);
}

BOOL gc_HasRefs(object *obj)
{
	if(obj == NULL)
		return(0);
	return(obj->ref_count);
}

void gc_FreeObject(object *obj)
{
	if (obj == NULL)
		return;
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_FREEING) > 0)
	{
		debug_printf(DEBUG_FREEING,"object to be freed\n");
		DumpObject(obj,1);
	}
	if(obj->ref_count < 0)
		debug_printf(DEBUG_ALL,"ref skew detected:%x\n",obj);
	#endif
	
	if(obj->ref_count > 1)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_GC) > 0)
		{
			debug_printf(DEBUG_GC,"%x : %d (decreasing due FreeObject)\n",obj,obj->ref_count);
		}
		#endif
		//gc_DecRefCount(obj);
		printf("Impossible\n");
		return;
	}

	switch(obj->type)
	{
	case TYPE_BLOCK:
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_FREEING,"Freeing block object %x\n",obj);
		#endif
		gc_DecRefCount((object*)((block_object*)obj)->code);
		if(((block_object*)obj)->stack != NULL)
			stack_Close(((block_object*)obj)->stack,1);
		break;
	case TYPE_REF:
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_FREEING,"Freeing Ref to %x\n",((ref_object*)obj)->ref);
		#endif
		break;
	case TYPE_ITER:
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_FREEING,"Freeing Iter %x\n",obj);
		#endif
		if(((iter_object*)obj)->block_stack != NULL)
		{	
			stack_Close(((iter_object*)obj)->block_stack,1);
		}
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_FREEING,"Freeing Iter tag %x\n",((iter_object*)obj)->tag);
		#endif
		gc_DecRefCount(((iter_object*)obj)->tag);
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
		gc_DecRefCount(((kv_object*)obj)->key);
		gc_DecRefCount(((kv_object*)obj)->value);
		break;
	case TYPE_FUNCTION:
		gc_DecRefCount((object*)((function_object*)obj)->defaults);
		gc_DecRefCount((object*)((function_object*)obj)->kw_defaults);
		gc_DecRefCount((object*)((function_object*)obj)->closure);
		gc_DecRefCount((object*)((function_object*)obj)->func);
		break;
	case TYPE_CFUNCTION:
		gc_DecRefCount((object*)((function_object*)obj)->defaults);
		gc_DecRefCount((object*)((function_object*)obj)->kw_defaults);
		break;
	case TYPE_UNICODE:
	   if(((unicode_object*)obj)->value != NULL)
		{
			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_VERBOSE_FREEING,"freeing unicode object @%x\n",obj);
			//assert(mem_free(((unicode_object*)obj)->value));
			mem_free(((unicode_object*)obj)->value);
			#else
			free(((unicode_object*)obj)->value);
			#endif
		}
		break;
	case TYPE_STRING:
		if(((string_object *)obj)->content != NULL)
		{
			#ifdef USE_DEBUGGING
			//assert(mem_free(((string_object *)obj)->content));
			mem_free(((string_object *)obj)->content);
			#else
			free(((string_object*)obj)->content);
			#endif
		}
		break;
	case TYPE_TUPLE:
		if (((tuple_object*)obj)->list != NULL)
		{
			if(((tuple_object*)obj)->list->num > 0)
			{
				for (NUM i = 0; i < ((tuple_object*)obj)->list->num; i++)
				{
					gc_DecRefCount((object*)((tuple_object*)obj)->list->items[i]);
				}
			}
			ptr_CloseList(((tuple_object*)obj)->list);
		}
		break;
	case TYPE_CODE:
		if(((code_object*)obj)->name != NULL)
		{
			#ifdef USE_DEBUGGING
			assert(mem_free(((code_object*)obj)->name));
			#else
			free(((code_object*)obj)->name);
			#endif
		}
		gc_DecRefCount(((code_object*)obj)->code);
		gc_DecRefCount(((code_object*)obj)->consts);
		gc_DecRefCount(((code_object*)obj)->names);
		gc_DecRefCount(((code_object*)obj)->varnames);
		gc_DecRefCount(((code_object*)obj)->freevars);
		gc_DecRefCount(((code_object*)obj)->cellvars);
		break;
	case TYPE_CLASS:
		if(((class_object*)obj)->name != NULL)
		{
			#ifdef USE_DEBUGGING
			assert(mem_free(((class_object*)obj)->name));
			#else
			free(((class_object*)obj)->name);
			#endif
		}
		gc_DecRefCount(((class_object*)obj)->code);
		gc_DecRefCount(((class_object*)obj)->base_classes);
		break;
	case TYPE_CLASS_INSTANCE:
		gc_DecRefCount(((class_instance_object*)obj)->instance_of);
		gc_DecRefCount(((class_instance_object*)obj)->methods);
		gc_DecRefCount(((class_instance_object*)obj)->vars);
		break;
	case TYPE_METHOD:
		gc_DecRefCount(((method_object*)obj)->func);
		gc_DecRefCount(((method_object*)obj)->instance);
		break;
	}

	#ifdef USE_DEBUGGING
	//assert(mem_free(obj));
	mem_free(obj);
	debug_printf(DEBUG_FREEING,"freed object:%x\n",obj);
	#else
	free(obj);
	#endif
}

void gc_Clear(void)
{
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_GC,"gc_Clear();\n");
	#endif
	while(garbage->num)
	{
		object *g = ptr_Pop(garbage);
		if(gc_HasNoRefs(g))
		{
			//FreeObject(g);
			#ifdef USE_DEBUGGING
			if((debug_level & DEBUG_GC) > 0)
			{
				debug_printf(DEBUG_GC,"%x : %d (killed:%c)\n",g,g->ref_count,g->type);
				//if(vm_ObjectExists(vm,g))
				//{
				//	debug_printf(DEBUG_GC,"%x : %d (Warning still in use and just got killed)\n",g,g->ref_count);
				//}				
			}
			#endif
			gc_FreeObject(g);
		}
		else
		{
			#ifdef USE_DEBUGGING
			if((debug_level & DEBUG_GC) > 0)
			{
				debug_printf(DEBUG_GC,"%x : %d (survived:%c)\n",g,g->ref_count,g->type);
				//if(!vm_ObjectExists(vm,g))
				//{
				//	debug_printf(DEBUG_GC,"%x : %d (Warning no reference in object tree was found)\n",g,g->ref_count);
				//}				
			}
			#endif
		}
	}
}

