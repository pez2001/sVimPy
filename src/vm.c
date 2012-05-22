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

#include "vm.h"

#ifdef USE_DEBUGGING

const char *helpmsg = "Commands:\n\
dbs :\n\
	dump block stack\n\
\n\
dca :\n\
	dump all code\n\
\n\
nd :\n\
	show next opcode with description\n\
\n\
dg :\n\
	dump garbage\n\
\n\
ds :\n\
	dump stack\n\
\n\
cs :\n\
	clearing stack\n\
\n\
di :\n\
	dump actual iter object\n\
\n\
dr :\n\
	dump actual running object\n\
\n\
dc :\n\
	dump code\n\
\n\
q :\n\
	quit vm\n\
\n\
h :\n\
	show this help message\n\
\n\
d :\n\
	dump main object\n\
\n\
n :\n\
	show next opcode\n\
\n\
s : [STEPS]\n\
	execute step , if STEPS is given execute STEPS in a row\n\
\n\
j : OFFSET\n\
	set instruction pointer to OFFSET\n\
\n\
r :\n\
	run remaining code till the end or until vm interruption was caught\n\
\n";

#endif
/*
BOOL vm_SearchObject(object *obj,object *needle)
{
	if (obj != NULL)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"checking obj(%c:%d):%x against %x\n",obj->type,obj->ref_count,obj,needle);
		#endif
		//DumpObject(obj,0);
		if(obj == needle)
			return(1);

		switch (obj->type)
		{
		case TYPE_BLOCK:
			if(((block_object*)obj)->stack != NULL && ((block_object*)obj)->stack->list != NULL && ((block_object*)obj)->stack->list->num)
			{
				for(NUM i = 0;i<((block_object*)obj)->stack->list->num;i++)
				{
					if(vm_SearchObject(((block_object*)obj)->stack->list->items[i],needle))
						return(1);
				}
			}				
			return(vm_SearchObject((object*)((block_object*)obj)->code,needle));
			break;
		case TYPE_ITER:
			if(vm_SearchObject(((iter_object*)obj)->tag,needle))
				return(1);
			if(((iter_object*)obj)->block_stack != NULL && ((iter_object*)obj)->block_stack->list != NULL && ((iter_object*)obj)->block_stack->list->num)
			{
				for(NUM i = 0;i<((iter_object*)obj)->block_stack->list->num;i++)
				{
					if(vm_SearchObject(((iter_object*)obj)->block_stack->list->items[i],needle))
						return(1);
				}
			}			
			break;
		case TYPE_FUNCTION:
				#ifdef USE_DEBUGGING
				debug_printf(DEBUG_ALL,"checking function defaults\n");
				#endif
				if(vm_SearchObject((object*)((function_object*)obj)->defaults,needle))
					return(1);
				#ifdef USE_DEBUGGING
				debug_printf(DEBUG_ALL,"checking function kw_defaults\n");
				#endif
				if(vm_SearchObject((object*)((function_object*)obj)->kw_defaults,needle))
					return(1);
				#ifdef USE_DEBUGGING
				debug_printf(DEBUG_ALL,"checking function closure\n");
				#endif
				if(vm_SearchObject((object*)((function_object*)obj)->closure,needle))
				 	return(1);
				#ifdef USE_DEBUGGING
				debug_printf(DEBUG_ALL,"checking code\n");
				#endif
				if(vm_SearchObject((object*)((function_object*)obj)->func,needle))
					return(1);
				#ifdef USE_DEBUGGING
				debug_printf(DEBUG_ALL,"checked function\n",obj->type,obj,needle);
				#endif
			break;
		case TYPE_REF:
			return(vm_SearchObject(((ref_object*)obj)->ref,needle));
			break;
		case TYPE_CODE:
			if(vm_SearchObject(((code_object *)obj)->code,needle))
				return(1);
			if(vm_SearchObject(((code_object *)obj)->consts,needle))
				return(1);
			if(vm_SearchObject(((code_object *)obj)->names,needle))
				return(1);
			if(vm_SearchObject(((code_object *)obj)->varnames,needle))
				return(1);
			if(vm_SearchObject(((code_object *)obj)->freevars,needle))
				return(1);
			return(vm_SearchObject(((code_object*)obj)->cellvars,needle));
		case TYPE_KV:
			if(vm_SearchObject(((kv_object*)obj)->key,needle))
				return(1);
			return(vm_SearchObject(((kv_object*)obj)->value,needle));
			break;
		case TYPE_TUPLE:
			{
				tuple_object *to = (tuple_object*)obj;
				//DumpObject(to,0);
				if(to->list != NULL && to->list->num)
				{
					for(NUM i = 0;i<to->list->num;i++)
					{
						//debug_printf(DEBUG_ALL,"checking entry:%d\n",i);
						object *ti = to->list->items[i];
						if(ti != NULL)
							if(vm_SearchObject(ti,needle))
								return(1);
						//debug_printf(DEBUG_ALL,"checked entry:%d\n",i);
					}
				}
			}
			break;
		}
	}
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"checked obj(%c):%x against %x\n",obj->type,obj,needle);	
	#endif
	return(0);
}

BOOL vm_ObjectExists(vm *vm, object  *obj)
{
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"checking blocks\n");
	#endif
	for(NUM i = 0;i<vm->blocks->list->num;i++)
	{
		if(vm_SearchObject(vm->blocks->list->items[i],obj))
			return(1);
	}	
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"checking globals\n");
	#endif
	for(NUM i = 0;i<vm->globals->num;i++)
	{
		if(vm_SearchObject(vm->globals->items[i],obj))
			return(1);
	}	
	return(0);
}
*/
#ifdef USE_DEBUGGING
void vm_DumpStackTree(vm *vm)
{
	for(INDEX i = 0;i<vm->blocks->list->num;i++)
	{
		printf("Dumping stack of block:%x\n",vm->blocks->list->items[i]);
		stack_Dump(((block_object*)vm->blocks->list->items[i])->stack);
	}
}
#endif

vm *vm_Init(code_object *co)
{
	#ifdef USE_DEBUGGING
	mem_Init();
	vm *tmp = (vm *) mem_malloc(sizeof(vm), "vm_Init() return");
	#else
	vm *tmp = (vm *)malloc(sizeof(vm));
	#endif

	tmp->blocks = stack_Init();
	tmp->globals = CreateTuple(0);
	gc_IncRefCount((object*)tmp->globals);
	tmp->running = 0;
	tmp->interrupt_vm = 0;
	tmp->interrupt_handler = NULL;
	tmp->exception_handler = NULL;
	tmp->step_handler = NULL;
	tmp->import_module_handler = NULL;
	
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"gc_Init\n");
	#endif
	gc_Init(tmp);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"obj_Init\n");
	#endif	
	obj_Init();
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"streams_Init\n");
	#endif	
	streams_Init();

	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"AddInternalFunctions\n");
	#endif
	AddInternalFunctions(tmp);
	AddInternalClasses(tmp);
	//#ifdef USE_ARDUINO_FUNCTIONS
	//AddArduinoFunctions(vm);
	//AddArduinoGlobals(vm);
	//#endif

	//AddFmodGlobals(vm);
	
	if (co != NULL)
	{
		vm_AddGlobal(tmp,(object*)CreateUnicodeObject(str_Copy("__main__")),(object*)co);
	}
	
	return (tmp);
}

void vm_Close(vm *vm)
{
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing vm\n");
	#endif
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"cleaning gc\n");
	#endif
	gc_Clear();

	stack_Close(vm->blocks, 1);
	gc_DecRefCount((object*)vm->globals);
	#ifdef USE_DEBUGGING
	assert(mem_free(vm));
	#else
	free(vm);
	#endif
	
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing streams\n");
	#endif
	streams_Close();	
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing obj\n");
	#endif
	obj_Close();
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing gc\n");
	#endif
	gc_Close();

	#ifdef USE_DEBUGGING
	mem_Close();
	#endif
}

void vm_FreeGlobals(vm *vm)
{
	for(INDEX i=0;i<vm->globals->list->num;i++)
	{	
			gc_DecRefCount((object*)vm->globals->list->items[i]);
	}
}

void vm_AddGlobal(vm *vm,object *key,object *global)//add a global object
{
	AppendDictItem((object*)vm->globals,key,global);
}

void vm_RemoveGlobal(vm *vm, object *key)//remove a global object
{
	DeleteDictItem((object*)vm->globals,key);
}

object *vm_GetGlobal(vm *vm, object *key)//retrieve a global object
{
	return(GetDictItem((object*)vm->globals,key));
}

void vm_Interrupt(vm *vm,object *(*interrupt) (struct _vm *vm,stack *stack))
{
	vm->interrupt_handler = interrupt;
	vm->interrupt_vm = 1;
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"forcing interrupt\n");
	#endif
}

void vm_SetInterrupt(vm*vm ,object *(*interrupt) (struct _vm *vm,stack *stack))
{
	vm->interrupt_handler = interrupt;
}

void vm_Continue(vm *vm)
{
	vm->interrupt_vm = 0;
}

void vm_Exit(vm *vm)//just a placeholder for things i didnt consider right now
{
	vm->interrupt_vm = 0;
	vm->running = 0;
	//vm_Close(vm);
	stack_Clear(vm->blocks,1);
}

void vm_Stop(vm *vm)
{
	vm->interrupt_vm = 0;
	vm->running = 0;
}

object *vm_StartObjectCopy(vm *vm,object *obj, tuple_object *locals,tuple_object *kw_locals)
{
	if(obj == NULL)
		return(NULL);

	if(obj->type == TYPE_UNICODE)
	{
		object *f = vm_GetGlobal(vm,obj);
		if(f != obj_NULL)
		{
			object *uret = vm_StartObjectCopy(vm,f, locals,kw_locals);
			return(uret);
		}
		/*
		else
		{
			object *caller_func = NULL;
			for(int i=0;i<vm->globals->num;i++)
			{	
				INDEX index = GetItemIndexByName(((code_object*)vm->globals->items[i])->names,((unicode_object*)obj)->value);
				if(index != -1)
					caller_func = GetItem(((code_object*)vm->globals->items[i])->names,index);
				if(caller_func != NULL && caller_func->type == TYPE_KV)
					caller_func = ((kv_object*)caller_func)->value;
				if(caller_func != NULL)
					break;
			}
			if (caller_func != NULL)
			{
				object *c = CopyObject(caller_func);
				//printf("copied caller func Object\n");
				gc_IncRefCount(c);
				object *cr = vm_StartObject(vm,c,locals,kw_locals);
				//object *cr = vm_StartObject(vm,obj,locals,kw_locals);
				gc_DecRefCount(c);
				return(cr);
			}
		}
		*/
	}else if(obj->type == TYPE_CFUNCTION)
	{
		object *r = vm_StartObject(vm,obj,locals,kw_locals);
		return(r);
	}/*else if(obj->type == TYPE_METHOD)
	{
		object *c = CopyObject(((method_object*)obj)->func);
		//gc_IncRefCount(c);
		object *mc = CreateMethodObject(c,((method_object*)obj)->instance);
		gc_IncRefCount(mc);
		object *r = vm_StartObject(vm,mc,locals,kw_locals);
		gc_DecRefCount(mc);
		return(r);
	}*//*else if(obj->type == TYPE_CLASS)
	{
		//printf("starting class object: %x, code: %x\n",obj,((class_object*)obj)->code);
		object *r = vm_StartObject(vm,obj,locals,kw_locals);
		return(r);
	}*/else
	{
		object *c = CopyObject(obj);
		gc_IncRefCount(c);
		object *r = vm_StartObject(vm,c,locals,kw_locals);
		gc_DecRefCount(c);
		return(r);
	}
	return(NULL);
}

object *vm_StartObject(vm *vm,object *obj, tuple_object *locals,tuple_object *kw_locals)
{
	if(obj == NULL)
		return(NULL);

	//printf("StartObject(%x:%c)\n",obj,obj->type);
	switch(obj->type)
	{
		case TYPE_METHOD:
			{
				#ifdef USE_DEBUGGING
				debug_printf(DEBUG_VERBOSE_STEP,"executing method\n");
				#endif
				object *rmo = vm_StartMethodObject(vm,(method_object*)obj,locals,kw_locals);
				return(rmo);
			}
		case TYPE_CLASS:
			{
				#ifdef USE_DEBUGGING 
				debug_printf(DEBUG_VERBOSE_STEP,"executing class initializer -> returning finalized class instance\n");
				#endif
				//printf("start object class co_flags:%d\n",((class_object*)obj)->code->co_flags);
				vm_StartClassObject(vm,(class_object*)obj,locals,kw_locals);
				return(NULL);
			}
		case TYPE_CODE:
			{
				vm_StartCodeObject(vm,(code_object*)obj,locals,kw_locals);
				return(NULL);
			}
		case TYPE_CFUNCTION:
			{
				object *pret = vm_StartCFunctionObject(vm,(cfunction_object*)obj, locals,kw_locals);
				return(pret);
			}
		case TYPE_UNICODE:
			{
				object *f = vm_GetGlobal(vm,(object*) ((unicode_object*)obj)->value);
				if(f != NULL)
				{
					object *pret = vm_StartObject(vm,f, locals,kw_locals);
					return(pret);
				}
				
				/*
				cfunction *f = vm_FindFunction(vm, ((unicode_object*)obj)->value);
				if(f != NULL)
				{
					//printf("calling c function:%s\n",((unicode_object*)obj)->value);
					object *uret = vm_StartCFunction(vm,f, locals,kw_locals);
					return(uret);
				}
				else
				{
					object *caller_func = NULL;
					for(int i=0;i<vm->globals->num;i++)
					{	
						INDEX index = GetItemIndexByName(((code_object*)vm->globals->items[i])->names,((unicode_object*)obj)->value);
						if(index != -1)
							caller_func = GetItem(((code_object*)vm->globals->items[i])->names,index);
						if(caller_func != NULL && caller_func->type == TYPE_KV)
							caller_func = ((kv_object*)caller_func)->value;
						if(caller_func != NULL)
							break;
					}
					if (caller_func != NULL)
					{
						object *cr = vm_StartObject(vm,caller_func,locals,kw_locals);
						return(cr);
					}
				}
				*/
			}
		case TYPE_FUNCTION:
			{
				//gc_IncRefCount((object*)obj);
				//printf("starting function object: %x\n",obj);
				object *pret = vm_StartFunctionObject(vm,(function_object*)obj, locals,kw_locals);
				//gc_DecRefCount((object*)obj);
				return(pret);
			}
	}
	return(NULL);
}

block_object *vm_StartCodeObject(vm *vm,code_object *co,tuple_object *locals,tuple_object *kw_locals) //TODO support kw_locals
{
		//code_object *co = (code_object*)obj;
		string_object *bytecodes = (string_object*) co->code;
		block_object *bo = AllocBlockObject();
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"sco Creating new execution block: %x\n",bo);
		#endif
		bo->type = TYPE_BLOCK;
		bo->code = co;
		gc_IncRefCount((object*)bo->code);
		bo->ip = 0;
		//bo->iter = NULL;
		bo->start = 0;
		bo->len = bytecodes->len;
		bo->ref_count = 0;
		bo->stack = stack_Init();		// co->stacksize
		//bo->initiated_locals = 1;
		stack_Push(vm->blocks,(object*)bo);
		if (locals != NULL)
		{
			for (NUM i = 0; i < locals->list->num; i++)
			{
				// printf("adding local %d\n",i);
				object *local = GetItem((object*)locals,i);

				if(GetItem(co->varnames,i)->type != TYPE_KV)
					{
						//printf("converting\n");
						SetItem(co->varnames,i,(object*)ConvertToKVObjectValued(GetItem(co->varnames,i),local));
					}
				else
				{
					SetDictItemByIndex(co->varnames,i,local);
				}
			}
		}
		return(bo);
}

block_object *vm_StartClassObject(vm *vm,class_object *co,tuple_object *locals,tuple_object *kw_locals) //TODO support kw_locals
{
	block_object *bo = NULL;
	if(co->code->code != NULL)
	{
		string_object *bytecodes = (string_object*) co->code->code;
		bo = AllocBlockObject();
		//printf("sclo Creating new execution block: %x for code: %x\n",bo,co->code);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"sclo Creating new execution block: %x\n",bo);
		#endif
		bo->type = TYPE_BLOCK;
		bo->code = co->code;
		gc_IncRefCount((object*)bo->code);
		//printf("sclo cof:%d\n",bo->code->co_flags);
		bo->ip = 0;
		bo->start = 0;
		bo->len = bytecodes->len;
		bo->ref_count = 0;
		bo->stack = stack_Init();		// co->stacksize
		stack_Push(vm->blocks,(object*)bo);
		stack_Push(bo->stack,(object*)co);//push class to be retrieved later by OPCODE_RETURN_VALUE
		//stack_Dump(bo->stack);
		if (locals != NULL)
		{
			SetDictItemByIndex((object*)co->code->varnames,0,(object*)locals);
			//DumpObject((object*)co->code->varnames,0);
		}
	}
	else
	{
		//no code defined
		//push class instance
		class_instance_object *cio = CreateClassInstanceObject(co);
		if(!stack_IsEmpty(vm->blocks))
		{
			block_object *actual = (block_object*)stack_Top(vm->blocks);
			stack_Push(actual->stack,(object*)cio);
		}
		unicode_object *method_name = CreateUnicodeObject(str_Copy("__init__"));
		object *rmr = vm_RunMethod(vm,(object*)method_name,cio,(tuple_object*)locals,NULL);
		gc_IncRefCount(rmr);
		gc_DecRefCount(rmr);
		gc_IncRefCount((object*)method_name);
		gc_DecRefCount((object*)method_name);
		gc_Clear();
	}
	tuple_object *bc = (tuple_object*)co->base_classes;
	for(INDEX i = 0;i<GetTupleLen((object*)bc);i++)
	{
		((class_object*)bc->list->items[i])->code->co_flags |= CO_SUB_CLASS_ROOT;
		((class_object*)bc->list->items[i])->code->co_flags &= ~CO_CLASS_ROOT;
		vm_StartClassObject(vm,bc->list->items[i],locals,kw_locals);//TODO check if this makes actually sense (var passing to base classes)
	}
	return(bo);
}

object *vm_StartFunctionObject(vm *vm,function_object *fo,tuple_object *locals,tuple_object *kw_locals)
{	
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"executing python function: %s\n",  fo->func->name);
		#endif
		block_object *bo = AllocBlockObject();
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"sfo Creating new execution block: %x\n",bo);
		#endif
		bo->type = TYPE_BLOCK;
		bo->ip = 0;
		bo->start = 0;
		bo->ref_count = 0;
		bo->stack = stack_Init();		// co->stacksize
		string_object *bytecodes = (string_object *) fo->func->code;
		bo->code = fo->func;
		gc_IncRefCount((object*)bo->code);
		bo->len = bytecodes->len;
			
		//printf("loading arguments\n");
		if (locals != NULL)
		{
			NUM oarg = locals->list->num;
			if(oarg > fo->func->argcount)
				oarg = fo->func->argcount;
			for (NUM i = 0; i < oarg; i++)
			{
				object *local = GetItem((object*)locals,i);
				if(GetItem(fo->func->varnames,i)->type != TYPE_KV)
				{
					SetItem(fo->func->varnames,i,(object*)ConvertToKVObjectValued(GetItem(fo->func->varnames,i),local));
				}
				else
				{
					SetDictItemByIndex(fo->func->varnames,i,local);
				}
			}
			//printf("loading arguments 2\n");
			if(locals->list->num > fo->func->argcount)
			{
				NUM narg = locals->list->num - fo->func->argcount;
				tuple_object *nt = CreateTuple(0); //creating tuple from the rest of the arguments
				for (NUM i = 0; i < narg; i++)
				{
					object *t = GetItem((object*)locals,oarg+i);
					t = DissolveRef(t);
					if(t->type == TYPE_KV)
						t = ((kv_object*)t)->value;
					AppendItem((object*)nt,t);
				}
				SetDictItemByIndex(fo->func->varnames,fo->func->argcount+fo->func->kwonlyargcount,(object*)nt);
			}		
		}
		
		if(kw_locals != NULL)
		{
			//printf("loading keyword arguments\n");
			for(NUM i = 0;i<kw_locals->list->num;i++)
			{
				kv_object *kw_local = (kv_object*)GetItem((object*)kw_locals,i);
				SetDictItem(fo->func->varnames,kw_local->key,kw_local->value);
			}
		}

		if((fo->func->co_flags & CO_GENERATOR)>0 )
		{
			iter_object *iter = CreateIterObject();
			iter_InitGenerator(iter,vm,bo);
			return((object*)iter);
		}
		//normal functions get pushed on the block stack
		stack_Push(vm->blocks, (object*)bo);
		return(NULL);//got nothing to return now
}

object *vm_StartCFunctionObject(vm *vm,cfunction_object *cfo,tuple_object *locals,tuple_object *kw_locals)
{
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"executing C function object\n");
	#endif
	return((*cfo->func) (vm,locals,kw_locals));
}

object *vm_RunMethod(vm *vm,object *key,class_instance_object *cio,tuple_object *locals,tuple_object *kw_locals)
{
	tuple_object *l = locals;
	//printf("method instance:%x\n",cio);
	//FullDumpObject(cio,0);
	//FullDumpObject(cio->instance_of->code,0);
	//printf("method locals start:%x\n",l);
	//DumpObject(l,0);
	//FullDumpObject(key,0);
	object *m = GetClassMethod((object*)cio,key);
	if(m == NULL || m == obj_NULL)
	//if(m == NULL )
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"method not found\n");
		if((debug_level & DEBUG_VERBOSE_STEP) > 0)
			DumpObject(key,0);
		#endif
		return(NULL);
	}
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"running method\n");
	#endif
	if(l == NULL || (object*)l == obj_NULL)
	{
		l = CreateTuple(0);
		gc_IncRefCount((object*)l);
	}
	InsertItem((object*)l,0,(object*)cio);
	//AppendItem(locals,cio);
	//AppendItem(locals,CreateUnicodeObject(str_Copy("hi")));
	//printf("sm mo:\n");
	//DumpObject((object*)mo,0);
	//printf("sm locals:\n");
	//DumpObject(locals,0);
	//gc_IncRefCount(locals);
	NUM s = vm->blocks->list->num;
	NUM n = 0;
	//vm_StartFunctionObject(vm,(function_object*)m,locals,kw_locals);
	object *ret = NULL;
	if(m->type == TYPE_FUNCTION)
	{
		ret = vm_StartFunctionObject(vm,(function_object*)m,l,kw_locals);
		while(ret == NULL && s != n) //TODO replace this construct with a more streamlined version without the need to call step in a loop
		{
			ret = vm_Step(vm);
			n = vm->blocks->list->num;
		}
		ret = stack_Pop(((block_object*)stack_Top(vm->blocks))->stack);//needed because method returns to value to the parent block stack
		SetItem(((function_object*)m)->func->varnames,0,NULL);//remove loop reference
		
    }else if(m->type == TYPE_CFUNCTION)
	{
		ret = vm_StartCFunctionObject(vm,(cfunction_object*)m,l,kw_locals);//gc_DecRefCount(locals);
	}

	//printf("method instance:%x\n",cio);
	//FullDumpObject(cio,0);
	//FullDumpObject(cio->instance_of->code,0);

	if(locals == NULL || (object*)locals == obj_NULL)
	{
		gc_DecRefCount((object*)l);
	}
	
	//printf("method locals:%x\n",l);
	//DumpObject(l,0);
	//if(pop_ret 
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"method thru\n");
	#endif
	return(ret);
}

object *vm_StartMethod(vm *vm,object *key,class_instance_object *cio,tuple_object *locals,tuple_object *kw_locals)
{
	tuple_object *l = locals;
	object *m = GetClassMethod((object*)cio,key);
	if(m == NULL)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"method not found\n");
		DumpObject(key,0);
		#endif
		return(NULL);
	}
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"starting method\n");
	#endif
	if(l == NULL)
	{
		l = CreateTuple(0);
		gc_IncRefCount((object*)l);
	}
	InsertItem((object*)l,0,(object*)cio);
	//AppendItem(locals,cio);
	//AppendItem(locals,CreateUnicodeObject(str_Copy("hi")));
	//printf("sm mo:\n");
	//DumpObject((object*)mo,0);
	//printf("sm locals:\n");
	//DumpObject(locals,0);
	//gc_IncRefCount(locals);
	//object *r = vm_StartFunctionObject(vm,(function_object*)m,locals,kw_locals);
	//gc_DecRefCount(locals);
	object *r = NULL;
	if(m->type == TYPE_FUNCTION)
		r = vm_StartFunctionObject(vm,(function_object*)m,l,kw_locals);
	else if(m->type == TYPE_CFUNCTION)
		r = vm_StartCFunctionObject(vm,(cfunction_object*)m,l,kw_locals);
	if(locals == NULL)
		gc_DecRefCount((object*)l);

	return(r);
}

object *vm_StartMethodObject(vm *vm,method_object *mo,tuple_object *locals,tuple_object *kw_locals)
{
	tuple_object *l = locals;
	if(l == NULL)
	{
		l = CreateTuple(0);
		gc_IncRefCount((object*)l);
	}
	InsertItem((object*)l,0,(object*)mo->instance);
	//printf("smo locals:\n");
	//DumpObject(locals,0);
	object *r = NULL;
	if(mo->func->type == TYPE_FUNCTION)
		r = vm_StartFunctionObject(vm,(function_object*)mo->func,l,kw_locals);
	else if(mo->func->type == TYPE_CFUNCTION)
		r = vm_StartCFunctionObject(vm,(cfunction_object*)mo->func,l,kw_locals);
	if(locals == NULL)
		gc_DecRefCount((object*)l);
	//if(r != NULL && r->type )
	return(r);
}

#ifdef USE_DEBUGGING
void vm_DumpCode(vm *vm,BOOL dump_descriptions, BOOL from_start)
{
				block_object *bo = (block_object *) stack_Top(vm->blocks);
				object *obj = (object*)bo->code;
				code_object *co = (code_object *) obj;
				string_object *bytecodes = (string_object *) co->code;
				char *string = bytecodes->content;
				int ip = bo->ip;
				int s = 1;
				if(from_start)
					ip = bo->start;
				while(ip < bo->len)
				{
					unsigned char op = (unsigned char)string[ip++];	// get op and increment code pointer
					int index = GetOpcodeIndex(op);
					if (index >= 0)
					{
						if(dump_descriptions)
						{
							if (opcodes[index].argcount != 0)
							{
								printf("[%03d,%03d,%02xh] [ %s ] (%d)\n%s\n", s, ip-1, opcodes[index].opcode, opcodes[index].name,num_short(string[ip], string[ip+1]),opcodes[index].description);
								ip+=2;
							}
							else
								printf("[%03d,%03d,%02xh] [ %s ]\n%s\n", s, ip-1,opcodes[index].opcode, opcodes[index].name,opcodes[index].description);
						}else
						{
							if (opcodes[index].argcount != 0)
							{
								printf("[%03d,%03d,%02xh] [ %s ] (%d)\n", s, ip-1, opcodes[index].opcode, opcodes[index].name,num_short(string[ip], string[ip+1]));
									ip+=2;
						}
							else
								printf("[%03d,%03d,%02xh] [ %s ]\n", s,ip-1,opcodes[index].opcode, opcodes[index].name);
						}
					}
					else
					{
						printf("unknown opcode:%x at %d\n", (char)string[ip],ip);
					}
					s++;
				}
}
#endif

#ifdef USE_DEBUGGING
object *vm_InteractiveRunObject(vm *vm, object *obj, tuple_object *locals,tuple_object *kw_locals)
{
		if(obj->type != TYPE_CODE)
			return(CreateEmptyObject(TYPE_NONE));
		vm_StartCodeObject(vm,(code_object*)obj,locals,kw_locals);
		object *ret = NULL;
		int steps = 0;
		while(ret == NULL)
		{
			if(vm->interrupt_vm)
			{
				//printf("catched an interrupt\n");
				block_object *b = (block_object*)stack_Top(vm->blocks);
				if(vm->interrupt_handler != NULL)
					vm->interrupt_handler(vm,b->stack);
				vm->running = 0;
				vm->interrupt_vm = 0;
			}
			else
			if(!vm->running && steps == 0)
			{
			printf("$");
			char *cmd = mem_malloc(80,"vm_InteractiveRunObject() cmd string\n");
			scanf("%s",cmd);
			//printf("%s\n",cmd);
			if(strlen(cmd)>= 3 && cmd[0] == 'd' && cmd[1] == 'b' && cmd[2] == 's')
			{
				printf("dumping block stack\n");
				stack_Dump(vm->blocks);
				printf("dumped\n");
			}else
			if(strlen(cmd)>= 3 && cmd[0] == 'd' && cmd[1] == 'c' && cmd[2] == 'a')
			{
				printf("dumping all bytecodes\n");
				vm_DumpCode(vm,0,1);
				printf("dumped\n");
			}else
			if(strlen(cmd)>= 2 && cmd[0] == 'n' && cmd[1] == 'd')
			{
				printf("next opcode\n");
				block_object *bo = (block_object *) stack_Top(vm->blocks);
				object *obj = (object*)bo->code;
				code_object *co = (code_object *) obj;
				string_object *bytecodes = (string_object *) co->code;
				char *string = bytecodes->content;
				if(bo->ip < bo->len)
				{
					unsigned char op = (unsigned char)string[bo->ip];	// get op and increment code pointer
					int index = GetOpcodeIndex(op);
					if (index >= 0)
					{
						if (opcodes[index].argcount != 0)
							printf("[%d,%xh] opcode: [ %s ] (%d)\n%s\n", bo->ip, opcodes[index].opcode, opcodes[index].name,num_short(string[bo->ip+ 1], string[bo->ip + 2]),opcodes[index].description);
						else
							printf("[%d,%xh] opcode: [ %s ]\n%s\n", bo->ip,opcodes[index].opcode, opcodes[index].name,opcodes[index].description);
					}
					else
					{
						printf("unknown opcode:%x at %d\n", (char)string[bo->ip],bo->ip);
					}
				}
				printf("dumped\n");
			}else
			/*if(strlen(cmd)>= 2 && cmd[0] == 'd' && cmd[1] == 'g')
			{
				printf("dumping garbage\n");
				//stack_Dump(vm->garbage);
				printf("dumped\n");
			}else*/
			if(strlen(cmd)>= 2 && cmd[0] == 'd' && cmd[1] == 's')
			{
				printf("dumping stack\n");
				block_object *b = (block_object*)stack_Top(vm->blocks);
				if(b->stack != NULL)
					stack_Dump(b->stack);
				else
					printf("block has no stack\n");
				printf("dumped\n");
			}else 
			if(strlen(cmd)>= 2 && cmd[0] == 'c' && cmd[1] == 's')
			{
				printf("clearing stack\n");
				block_object *b = (block_object*)stack_Top(vm->blocks);
				if(b->stack != NULL)
					stack_Clear(b->stack,1);
				else
					printf("block has no stack\n");
				printf("cleared\n");
			}else 
			/*if(strlen(cmd)>= 2 && cmd[0] == 'd' && cmd[1] == 'i')
			{
				printf("dumping iter\n");
				block_object *b = (block_object*)stack_Top(vm->blocks);
				if(b->iter != NULL)
					DumpObject(b->iter,0);
				else
					printf("block has no iter\n");
				printf("dumped\n");
			}else*/ 
			if(strlen(cmd)>= 2 && cmd[0] == 'd' && cmd[1] == 'r')
			{
				printf("dumping actual block code\n");
				block_object *b = (block_object*)stack_Top(vm->blocks);
				if(b->code != NULL)
					DumpObject((object*)b->code,0);
				else
					printf("block has no code\n");
				printf("dumped\n");
			}else
			if(strlen(cmd)>= 2 && cmd[0] == 'd' && cmd[1] == 'c')
			{
				printf("dumping actual bytecodes\n");
				vm_DumpCode(vm,0,0);
				printf("dumped\n");
			}else
			if(strlen(cmd)>= 1 && cmd[0] == 'q')
			{
				#ifdef USE_DEBUGGING
				assert(mem_free(cmd));
				#else
				free(cmd)
				#endif
				stack_Clear(vm->blocks,1);
				return(NULL);
			}else
			if(strlen(cmd)>= 1 && cmd[0] == 'h')
			{
				printf(helpmsg);
			}else
			if(strlen(cmd)>= 1 && cmd[0] == 'd')
			{
				printf("dumping run object\n");
				DumpObject(obj,0);
				printf("dumped\n");
			}else
			if(strlen(cmd)>= 1 && cmd[0] == 'n')
			{
				printf("next opcode\n");
				block_object *bo = (block_object *) stack_Top(vm->blocks);
				object *obj = (object*)bo->code;
				code_object *co = (code_object *) obj;
				string_object *bytecodes = (string_object *) co->code;
				char *string = bytecodes->content;
				if(bo->ip < bo->len)
				{
					unsigned char op = (unsigned char)string[bo->ip];	// get op and increment code pointer
					int index = GetOpcodeIndex(op);
					if (index >= 0)
					{
						if (opcodes[index].argcount != 0)
							printf("[%d,%xh] opcode: [ %s ] (%d)\n", bo->ip, opcodes[index].opcode, opcodes[index].name,num_short(string[bo->ip+ 1], string[bo->ip + 2]));
						else
							printf("[%d,%xh] opcode: [ %s ]\n", bo->ip,opcodes[index].opcode, opcodes[index].name);
					}
					else
					{
						printf("unknown opcode:%x at %d\n", (char)string[bo->ip],bo->ip);
					}
				}
				printf("dumped\n");
			}else 			
			if(strlen(cmd)>= 1 && cmd[0] == 's')
			{
				if(strlen(cmd)>1)
				{
					steps = atoi(cmd+1);
					printf("%d steps in a row\n",steps);
				}
				else
					ret = vm_Step(vm);
			}else
			if(strlen(cmd)>= 1 && cmd[0] == 'j')
			{
				if(strlen(cmd)>1)
				{
					long pos = atoi(cmd+1);
					printf("jumping to %d\n",pos);
					block_object *bo = (block_object *) stack_Top(vm->blocks);
					bo->ip = pos;
				}
				else
					ret = vm_Step(vm);
			}else
			if(strlen(cmd)>= 1 && cmd[0] == 'r')
			{
				vm->running = 1;
				vm->interrupt_vm = 0;
			}
			#ifdef USE_DEBUGGING
			assert(mem_free(cmd));
			#else
			free(cmd);
			#endif
			}
			else //run or steps != 0
			{
				//printf("running\n");
				if(vm->running)
					ret = vm_Step(vm);
				else
				{
					for(int i=0;i<steps;i++)
					{
						printf("step %d\n",i+1);
						ret = vm_Step(vm);
						if(ret != NULL)
							return(ret);
					}
					steps = 0;
				}
			}
		}
		vm->running = 0;
		vm->interrupt_vm = 0;
		return(ret);
}
#endif

object *vm_RunFunction(vm *vm,char *name,tuple_object *locals,tuple_object *kw_locals)
{
	object *f = NULL;
	for(int i=0;i<vm->globals->list->num;i++)
	{	
			if(((object*)vm->globals->list->items[i])->type == TYPE_CODE)
				f = GetDictItemByName(((code_object*)vm->globals->list->items[i])->names,name);
			if(f != NULL)
				break;
	}

	//DumpObject(f,0);
	//printf("found func\n");
	if(f->type == TYPE_FUNCTION)
	{
		object *ret = vm_StartFunctionObject(vm,(function_object*)f,locals,kw_locals);
		ret = NULL;
		//printf("pushed func\n");
			while(ret == NULL)
				ret = vm_Step(vm);
		return(ret);
	}
	return(NULL);
}

object *vm_RunPYC(vm *vm,stream *f ,BOOL free_object)
{
	long pyc_magic = MAGIC;
	BOOL r = stream_Open(f);
	if (!r)
		return(NULL);//TODO maybe return an empty object for simplification
	long magic = ReadLong(f);
	if (magic != pyc_magic)
		return(NULL);
	//long time = ReadLong(f);
	ReadLong(f);//read time
	object *obj = ReadObject(f);
	object *global_key = (object*)CreateUnicodeObject(str_Copy("__main__"));
	vm_AddGlobal(vm, global_key,obj);
	object *ret = vm_RunObject(vm, obj, NULL,NULL);
	if (ret != NULL)
	{
		gc_DecRefCount(ret);
	}
	if(free_object)
	{
		gc_DecRefCount(obj);
		vm_RemoveGlobal(vm,global_key);
		stream_Free(f);
		return(NULL);
	}
	stream_Free(f);
	return(obj);
}

object *vm_RunObject(vm *vm, object *obj, tuple_object *locals,tuple_object *kw_locals)
{
		if(obj->type != TYPE_CODE)
			return(CreateEmptyObject(TYPE_NONE));
		unicode_object *var_name = CreateUnicodeObject(str_Copy("__name__"));
		gc_IncRefCount((object*)var_name);
		unicode_object *module_name = CreateUnicodeObject(str_Copy("__main__"));
		SetDictItem(((code_object*)obj)->names,(object*)var_name,(object*)module_name);
		gc_DecRefCount((object*)var_name);
		vm_StartObject(vm,obj,locals,kw_locals);
		object *ret = NULL;
		while(ret == NULL)
			ret = vm_Step(vm);
		return(ret);
}

object *vm_Step(vm *vm)
{
	block_object *bo = (block_object*) stack_Top(vm->blocks);
	object *obj = (object*)bo->code;
	if(obj->type != TYPE_CODE)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"stepping non code:%c(%x)\n",obj->type,obj);
		#endif
		return(CreateEmptyObject(TYPE_NONE));
	}
	if (obj->type == TYPE_CODE)
	{
		code_object *co = (code_object*) obj;
		string_object *bytecodes = (string_object *) co->code;
		char *string = bytecodes->content;

	
		if(bo->ip < bo->len)
		{
			unsigned char op = (unsigned char)string[bo->ip++];	// get op and increment code pointer
			BOOL has_extended_arg = 0;
			short extended_arg = 0;
			INT arg = 0;		// holds the argument if present
			BOOL op_thru = 0;	// used to skip unneccessary switches

			object *tos = NULL; //top of the stack objects
			object *tos1 = NULL;
			object *tos2 = NULL;

			tuple_object *var_list = NULL;//used for simpler function calling
			tuple_object *kw_list = NULL;//used for simpler function calling
			
			#ifdef USE_DEBUGGING
			if((debug_level & DEBUG_SHOW_OPCODES) > 0)
			{
				INDEX index = GetOpcodeIndex(op);
				if (index >= 0)
				{
					if (opcodes[index].argcount != 0)
						debug_printf(DEBUG_ALL,"[%03d,%02xh] [ %s ] (%d)\n", bo->ip-1,opcodes[index].opcode, opcodes[index].name,num_short(string[bo->ip], string[bo->ip + 1]));
					else
						debug_printf(DEBUG_ALL,"[%03d,%02xh] [ %s ]\n", bo->ip-1,opcodes[index].opcode, opcodes[index].name);
				}
				else
				{
					//FullDumpObject(bo->code,0);
					debug_printf(DEBUG_ALL,"unknown opcode:%x at %d in %x\n", (char)string[bo->ip - 1],bo->ip - 1,bo->code);
				}
			}
			#ifdef USE_ARDUINO_OPCODE_DEBUGGING
			INDEX index = GetOpcodeIndex(op);
			if (index >= 0)
			{
				opcode *op = malloc(sizeof(opcode));
				memcpy_P(op,&opcodes[index],sizeof(opcode));
				char *op_name = malloc(strlen_P(&op->name)+1);
				memset(op_name,0,strlen_P(&op->name)+1);
				memcpy_P(op_name,&op->name,strlen_P(&op->name)):
				
				if (op->argcount != 0)
					debug_printf(DEBUG_ALL,"[%03d,%02xh] [ %s ] (%d)\n", bo->ip-1,op->opcode, op_name ,num_short(string[bo->ip], string[bo->ip + 1]));
				else
					debug_printf(DEBUG_ALL,"[%03d,%02xh] [ %s ]\n", bo->ip-1,op->opcode, op_name);
				
				free(op_name);
				free(op);
				}
			else
			{
				debug_printf(DEBUG_ALL,"unknown opcode:%x at %d\n", (char)string[bo->ip - 1],bo->ip - 1);
			}
			#endif

			#endif // -- FOR DEBUGGING
			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_VERBOSE_STEP,"preparing opcode\n");
			#endif
			// prepare opcode argument,increment codepointer and intepret
			// small opcodes not using an arg etc
			switch (op)
			{
			case OPCODE_NOP:
				{
					op_thru = 1;
				}
				break;
				
			case OPCODE_YIELD_VALUE:
				{
					bo->ip++;
					object *ret = stack_Pop(bo->stack);
					gc_IncRefCount(ret);
					gc_Clear();
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
						debug_printf(DEBUG_VERBOSE_STEP,"yield object @:%x\n",ret);
						DumpObject(ret,0);
					}
					#endif			
					return(ret);
				}
				break;
				
			case OPCODE_RETURN_VALUE:
				{
					object *ret = stack_Pop(bo->stack);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
						debug_printf(DEBUG_VERBOSE_STEP,"ret object @:%x\n",ret);
						DumpObject(ret,0);
					}
					#endif
					if(((bo->code->co_flags & CO_SUB_CLASS_ROOT) > 0) )// || (bo->code->co_flags & CO_CLASS_CONSTRUCTOR) > 0))
					{
						stack_Pop(vm->blocks);
						bo->code->co_flags |= CO_CLASS_ROOT;
						bo->code->co_flags &= ~CO_SUB_CLASS_ROOT;
						gc_Clear();
						return (NULL);
					}else	if((bo->code->co_flags & CO_CLASS_ROOT) > 0)
					{
						//printf("return in class root\n");
						//printf("actual block:%x\n",stack_Top(vm->blocks));
						//stack_Dump(bo->stack);
						stack_Pop(vm->blocks);
						class_object *c = (class_object*)stack_Pop(bo->stack);//this was initially pushed on stack by vm_StartClassObject();
						//vm_AddGlobal(vm,c);
						//FullDumpObject(c,0);
						class_instance_object *cio = CreateClassInstanceObject(c);
						unicode_object *locals_name = CreateUnicodeObject(str_Copy("__locals__"));
						gc_IncRefCount((object*)locals_name);
						object *locals = GetDictItem(c->code->varnames,(object*)locals_name);
						SetDictItem(c->code->varnames,(object*)locals_name,NULL);
						gc_DecRefCount((object*)locals_name);
						gc_IncRefCount(locals);
						if(!stack_IsEmpty(vm->blocks))
						{
							block_object *parent = (block_object*)stack_Top(vm->blocks);
							stack_Push(parent->stack,(object*)cio);
						}
						
						unicode_object *method_name = CreateUnicodeObject(str_Copy("__init__"));
						gc_IncRefCount((object*)method_name);
						object *rmr = vm_RunMethod(vm,(object*)method_name,cio,(tuple_object*)locals,NULL);
						gc_IncRefCount(rmr);
						gc_DecRefCount(rmr);
						gc_DecRefCount((object*)method_name);
						gc_DecRefCount(locals);
						gc_Clear();
						return(NULL);
					} else if((bo->code->co_flags & CO_MODULE_ROOT) > 0)
					{
						stack_Pop(vm->blocks);
						unicode_object *mkey = CreateUnicodeObject(str_Copy(co->name));
						vm_RemoveGlobal(vm,(object*)mkey);
						gc_IncRefCount((object*)mkey);
						gc_DecRefCount((object*)mkey);
						
					} else
					{
						//block_object *actual_block = (block_object*)stack_Pop(vm->blocks);
						stack_Pop(vm->blocks);
						if(!stack_IsEmpty(vm->blocks))
						{
							block_object *parent = (block_object*)stack_Top(vm->blocks);
							#ifdef USE_DEBUGGING
							if((debug_level & DEBUG_VERBOSE_STEP) > 0)
							{
								stack_Dump(parent->stack);
							}
							#endif
							stack_Push(parent->stack,ret);
							#ifdef USE_DEBUGGING
							debug_printf(DEBUG_VERBOSE_STEP,"still blocks on the stack\n");
							#endif
						}
						else 
						{
							gc_IncRefCount(ret);
							gc_Clear();
							return(ret);
						}
					}					
					gc_Clear();
					return (NULL);
				}

			case OPCODE_STOP_CODE:
				{
					bo->ip = bo->len;
					op_thru = 1;
				}
				break;

			case OPCODE_POP_TOP:
				{
					stack_Pop(bo->stack);
					op_thru = 1;
				}
				break;

			case OPCODE_POP_BLOCK:
				{
					stack_Pop(vm->blocks);
					op_thru = 1;
				}
				break;

			case OPCODE_BREAK_LOOP:
				{
					#ifdef USE_DEBUGGING
					block_object *bbo = (block_object *) stack_Pop(vm->blocks);
					debug_printf(DEBUG_VERBOSE_STEP,"break to: %d ,start: %d ,len:%d\n",bo->ip,bbo->start,bbo->len);
					#else
					stack_Pop(vm->blocks);
					#endif
					op_thru = 1;
				}
				break;

			case OPCODE_ROT_TWO:
				{
					tos = stack_Top(bo->stack);
					tos1 = stack_Second(bo->stack);
					stack_SetTop(bo->stack, tos1);
					stack_SetSecond(bo->stack, tos);
					op_thru = 1;
				}
				break;

			case OPCODE_ROT_THREE:
				{
					tos = stack_Top(bo->stack);
					tos1 = stack_Second(bo->stack);
					tos2 = stack_Third(bo->stack);
					stack_SetTop(bo->stack, tos1);
					stack_SetSecond(bo->stack, tos2);
					stack_SetThird(bo->stack, tos);
					op_thru = 1;
				}
				break;

			case OPCODE_DUP_TOP:
				{
					tos = stack_Top(bo->stack);
					stack_Push(bo->stack, tos);
					op_thru = 1;
				}
				break;

			case OPCODE_DUP_TOP_TWO:
				{
					tos = stack_Top(bo->stack);
					tos1 = stack_Second(bo->stack);
					stack_Adjust(bo->stack, 2);
					stack_SetTop(bo->stack, tos);
					gc_IncRefCount(tos);
					stack_SetSecond(bo->stack, tos1);
					gc_IncRefCount(tos1);
					op_thru = 1;
				}
				break;

			case OPCODE_GET_ITER:
				{
					object *iter = stack_Pop(bo->stack);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
					 debug_printf(DEBUG_VERBOSE_STEP,"iter:\n");
					 DumpObject(iter,0);
					}
					#endif
					iter_object *it = iter_CreateIter(iter,vm);//,vm);
					stack_Push(bo->stack,(object*)it);
					op_thru = 1;
				}
				break;

			case OPCODE_EXTENDED_ARG:
				{
					extended_arg = num_short(string[bo->ip + 1], string[bo->ip + 2]);
					//printf("found extended arg\n");
					bo->ip += 2;
					op_thru = 1;
					has_extended_arg = 1;
				}
				break;
				
			case OPCODE_MAP_ADD:				
			case OPCODE_SET_ADD:
			case OPCODE_LOAD_DEREF:
			case OPCODE_STORE_DEREF:
			case OPCODE_DELETE_DEREF:
			case OPCODE_LOAD_GLOBAL:
			case OPCODE_STORE_GLOBAL:
			case OPCODE_DELETE_GLOBAL:
			case OPCODE_LOAD_CONST:
			case OPCODE_LOAD_NAME:
			case OPCODE_STORE_NAME:
			case OPCODE_DELETE_NAME:
			case OPCODE_LOAD_FAST:
			case OPCODE_STORE_FAST:
			case OPCODE_DELETE_FAST:
			case OPCODE_BUILD_TUPLE:
			case OPCODE_BUILD_LIST:
			case OPCODE_BUILD_MAP:
			case OPCODE_BUILD_SET:
			case OPCODE_FOR_ITER:
			case OPCODE_UNPACK_SEQUENCE:
			case OPCODE_SETUP_LOOP:
			case OPCODE_CALL_FUNCTION:
			case OPCODE_CALL_FUNCTION_VAR:
			case OPCODE_CALL_FUNCTION_KW:
			case OPCODE_CALL_FUNCTION_VAR_KW:
			case OPCODE_JUMP_FORWARD:
			case OPCODE_JUMP_IF_FALSE:
			case OPCODE_JUMP_IF_TRUE:
			case OPCODE_JUMP_ABSOLUTE:
			case OPCODE_POP_JUMP_IF_FALSE:
			case OPCODE_POP_JUMP_IF_TRUE:
			case OPCODE_BUILD_SLICE:
			case OPCODE_MAKE_CLOSURE:
			case OPCODE_LOAD_CLOSURE:
			case OPCODE_PRINT_EXPR:
			case OPCODE_COMPARE_OP:
			case OPCODE_IMPORT_FROM:
			case OPCODE_IMPORT_NAME:
			case OPCODE_SETUP_EXCEPT:
			case OPCODE_SETUP_FINALLY:
			case OPCODE_MAKE_FUNCTION:
			case OPCODE_RAISE_VARARGS:
			case OPCODE_SETUP_WITH:
			case OPCODE_LOAD_ATTR:
			case OPCODE_STORE_ATTR:
				{
					if (has_extended_arg)
					{
						arg = num_long((extended_arg >> 8) & 0xFF, extended_arg & 0xFF, string[bo->ip], string[bo->ip + 1]);
						has_extended_arg = 0;
						extended_arg = 0;
						//printf("found extended arg\n");

					}
					else
						arg = (long)num_short(string[bo->ip], string[bo->ip + 1]);
					bo->ip += 2;
				}
				break;
			}

			if (op_thru)
			{
				gc_Clear();
				return(NULL);
			}

			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_VERBOSE_STEP,"fetching tos items\n");
			#endif
			// fetch tos items 
			switch (op)
			{
			case OPCODE_UNARY_INVERT:
			case OPCODE_UNARY_NOT:
			case OPCODE_UNARY_NEGATIVE:
			case OPCODE_UNARY_POSITIVE:
			case OPCODE_UNPACK_SEQUENCE:
			case OPCODE_POP_JUMP_IF_FALSE:
			case OPCODE_POP_JUMP_IF_TRUE:
			case OPCODE_JUMP_IF_FALSE:
			case OPCODE_JUMP_IF_TRUE:
			case OPCODE_MAKE_FUNCTION:
			case OPCODE_MAKE_CLOSURE:
			case OPCODE_STORE_FAST:
			case OPCODE_STORE_NAME:
			case OPCODE_STORE_GLOBAL:
			case OPCODE_STORE_DEREF:
			case OPCODE_LIST_APPEND:
			case OPCODE_SET_ADD:
			case OPCODE_IMPORT_FROM:
			case OPCODE_IMPORT_STAR:
			case OPCODE_SETUP_WITH:
			case OPCODE_LOAD_ATTR:
			case OPCODE_STORE_LOCALS:
				{
					tos = stack_Pop(bo->stack);
					tos = DissolveRef(tos);
					if(tos->type == TYPE_KV)
						tos = ((kv_object*)tos)->value;
					#ifdef USE_DEBUGGING
					assert(tos != NULL);
					#endif
				}
				break;
			
			case OPCODE_INPLACE_FLOOR_DIVIDE:
			case OPCODE_INPLACE_TRUE_DIVIDE:
			case OPCODE_INPLACE_ADD:
			case OPCODE_INPLACE_SUBTRACT:
			case OPCODE_INPLACE_MULTIPLY:
			case OPCODE_INPLACE_DIVIDE:
			case OPCODE_INPLACE_MODULO:
			case OPCODE_INPLACE_POWER:
			case OPCODE_INPLACE_LSHIFT:
			case OPCODE_INPLACE_RSHIFT:
			case OPCODE_INPLACE_AND:
			case OPCODE_INPLACE_XOR:
			case OPCODE_INPLACE_OR:
			case OPCODE_COMPARE_OP:
			case OPCODE_DELETE_SUBSCR:
			case OPCODE_BINARY_ADD:
			case OPCODE_BINARY_SUBTRACT:
			case OPCODE_BINARY_TRUE_DIVIDE:
			case OPCODE_BINARY_FLOOR_DIVIDE:
			case OPCODE_BINARY_MODULO:
			case OPCODE_BINARY_RSHIFT:
			case OPCODE_BINARY_POWER:
			case OPCODE_BINARY_MULTIPLY:
			case OPCODE_BINARY_LSHIFT:
			case OPCODE_BINARY_DIVIDE:
			case OPCODE_BINARY_AND:
			case OPCODE_BINARY_XOR:
			case OPCODE_BINARY_OR:
			case OPCODE_BINARY_SUBSCR:
			case OPCODE_MAP_ADD:
			case OPCODE_IMPORT_NAME:
			case OPCODE_STORE_ATTR:
				{
					tos = stack_Pop(bo->stack);
					tos1 = stack_Pop(bo->stack);
					tos = DissolveRef(tos);
					tos1 = DissolveRef(tos1);
					if(tos->type == TYPE_KV)
						tos = ((kv_object*)tos)->value;
					if(tos1->type == TYPE_KV)
						tos1 = ((kv_object*)tos1)->value;
					#ifdef USE_DEBUGGING
					assert(tos != NULL);
					assert(tos1 != NULL);
					#endif
				}
				break;

			case OPCODE_STORE_SUBSCR:
			case OPCODE_STORE_MAP:
			//case OPCODE_RAISE_VARARGS:
				{
					tos = stack_Pop(bo->stack);
					tos1 = stack_Pop(bo->stack);
					tos2 = stack_Pop(bo->stack);
					tos = DissolveRef(tos);
					tos1 = DissolveRef(tos1);
					tos2 = DissolveRef(tos2);
					if(tos->type == TYPE_KV)
						tos = ((kv_object*)tos)->value;
					if(tos1->type == TYPE_KV)
						tos1 = ((kv_object*)tos1)->value;
					if(tos2->type == TYPE_KV)
						tos2 = ((kv_object*)tos2)->value;
				}
				break;
			}

			#ifdef USE_DEBUGGING
			debug_printf(DEBUG_VERBOSE_STEP,"executing main ops\n");
			#endif
			// execute remaining ops here
			switch (op)
			{
			
			case OPCODE_RAISE_VARARGS:
				{
					//printf("RAISE_VARARGS\n");
					//stack_Dump(bo->stack);
					tos = stack_Pop(bo->stack);
					tos = DissolveRef(tos);
					if(tos->type == TYPE_KV)
						tos = ((kv_object*)tos)->value;
					if(arg > 1) //parameter
					{
						tos1 = stack_Pop(bo->stack);
						tos1 = DissolveRef(tos1);
						if(tos1->type == TYPE_KV)
							tos1 = ((kv_object*)tos1)->value;
					}
					if(arg > 2) //traceback
					{
						tos2 = stack_Pop(bo->stack);
						tos2 = DissolveRef(tos2);
						if(tos2->type == TYPE_KV)
							tos2 = ((kv_object*)tos2)->value;
					}
					if(vm->exception_handler != NULL)
						vm->exception_handler(vm,tos);
					//gc_Clear();
					object *o = CreateEmptyObject(TYPE_NONE);
					//o->ref_count++;
					gc_IncRefCount(o);
					return(CreateEmptyObject(TYPE_NONE));
				}
				break;
				
			case OPCODE_STORE_LOCALS:
				{
					//Pops TOS from the stack and stores it as the current frame’s f_locals. This is used in class construction.
					//not needed cause im handling locals a bit different then cpython
				}
				break;		
			
			case OPCODE_END_FINALLY:
				{
		
				}
				break;

			case OPCODE_DELETE_DEREF:
				{
					//if(
					//SetDictItemByIndex(co_freevars,arg,NULL);
				}
				break;

			case OPCODE_DELETE_ATTR:
				{
					//if(
					//SetDictItemByIndex(co_freevars,arg,NULL);
				}
				break;

			case OPCODE_LOAD_BUILD_CLASS:
				{
					cfunction_object *cfo = CreateCFunctionObject(&if_build_class);//TODO switch to get global
					stack_Push(bo->stack,(object*)cfo);
				}
				break;

			case OPCODE_FOR_ITER:
				{
					object *next =  NULL;
					if(stack_Top(bo->stack)->type == TYPE_ITER)
						next = iter_NextNow((iter_object*)stack_Top(bo->stack),vm);
					if (next->type == TYPE_NONE)
					{
						stack_Pop(bo->stack);
						#ifdef USE_DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"for iter thru \n");
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
							DumpObject(next,1);
						#endif
						bo->ip = bo->ip + arg;
					}
					else
					{
						#ifdef USE_DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"for iter next \n");
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
							DumpObject(next,1);
						#endif
						stack_Push(bo->stack, next);
					}
				}
				break;
				
			case OPCODE_LOAD_ATTR:
				{
					object *name = GetItem(co->names,arg);
					if(name->type == TYPE_KV)
						name = ((kv_object*)name)->value;
					//stack_Push(bo->stack, name);
					//printf("importing from: %s\n",((unicode_object*)name)->value);
					#ifdef USE_DEBUGGING
					if(name->type == TYPE_UNICODE)
						debug_printf(DEBUG_VERBOSE_STEP,"loading attribute: %s\n",((unicode_object*)name)->value);
					//if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					//	DumpObject(name,1);
					#endif
					object *attr = GetAttribute(tos,name);
					if(attr != NULL)
						stack_Push(bo->stack,attr);
					#ifdef USE_DEBUGGING
					else
						debug_printf(DEBUG_ALL,"loading attribute failed\n");
					#endif
				}
				break;
				
			case OPCODE_STORE_ATTR:
				{
					//"Implements TOS.name = TOS1, where /namei/ is the index of name in co_names."
					object *name = GetItem(co->names,arg);
					if(name->type == TYPE_KV)
						name = ((kv_object*)name)->value;
					#ifdef USE_DEBUGGING
					if(name->type == TYPE_UNICODE)
						debug_printf(DEBUG_VERBOSE_STEP,"storing attribute: %s\n",((unicode_object*)name)->value);
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(tos1,1);
					#endif
					SetAttribute(tos,name,tos1);
				}	
				break;

				case OPCODE_IMPORT_NAME:
				{
					object *name = GetItem(co->names,arg);
					if(name->type == TYPE_KV)
						name = ((kv_object*)name)->value;
					//stack_Push(bo->stack, name);
					#ifdef USE_DEBUGGING
					if(name->type == TYPE_UNICODE)
						debug_printf(DEBUG_VERBOSE_STEP,"importing: %s\n",((unicode_object*)name)->value);
					//if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					//	DumpObject(name,1);
					#endif
					//printf("importing: %s\n",((unicode_object*)name)->value);
					//stack_Dump(bo->stack);
					object *mc = vm_GetGlobal(vm,name);
					if(mc != obj_NULL)
					{
						stack_Push(bo->stack,mc);
						break;
					}
					/*
					BOOL found = 0;
					for(int i=0;i<vm->globals->num;i++)
					{	
						if(!strcmp(((code_object*)vm->globals->items[i])->name,((unicode_object*)name)->value))
						{
							stack_Push(bo->stack,vm->globals->items[i]);
							//vm_AddGlobal(vm, (code_object*)vm->globals->items[i]);
							//gc_DecRefCount(vm->globals->items[i]);
							found = 1;
						}
					}
					if(found)
						break;
					for(int i=0;i<vm->classes->num;i++)
					{	
						if(!strcmp(((class_object*)vm->classes->items[i])->name,((unicode_object*)name)->value))
						{
							stack_Push(bo->stack,vm->classes->items[i]);
							//vm_AddGlobal(vm, (code_object*)vm->globals->items[i]);
							//gc_DecRefCount(vm->globals->items[i]);
							found = 1;
						}
					}
					if(found)
						break;
					*/
					if(vm->import_module_handler != NULL)
					{
						object *module = vm->import_module_handler(vm,((unicode_object*)name)->value);
						//gc_DecRefCount(module);
						if(module != NULL) // found the module push it on stack
						{
							stack_Push(bo->stack,module);
							vm_AddGlobal(vm, name,module);
							//FullDumpObject(vm->globals,0);
							vm_StartCodeObject(vm,(code_object*)module,NULL,NULL);
							//vm_RemoveGlobal(vm, (code_object*)module);
							gc_DecRefCount(module);//TODO why leaks if removed ????
						}
						else
						{
							#ifdef USE_DEBUGGING
							debug_printf(DEBUG_ALL,"failed to import: %s\n",((unicode_object*)name)->value);
							#endif
						}
					}
					
				}
				break;
				
			case OPCODE_IMPORT_FROM:
				{
					object *name = GetItem(co->names,arg);
					if(name->type == TYPE_KV)
						name = ((kv_object*)name)->value;
					//stack_Push(bo->stack, name);
					//printf("importing from: %s\n",((unicode_object*)name)->value);
					#ifdef USE_DEBUGGING
					if(name->type == TYPE_UNICODE)
						debug_printf(DEBUG_VERBOSE_STEP,"importing from: %s\n",((unicode_object*)name)->value);
					//if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					//	DumpObject(name,1);
					#endif
					object *lgo = NULL;
					//stack_Dump(bo->stack);
					//DumpObject(tos,0);
					if(tos->type == TYPE_CODE)
					{
						INDEX index = GetItemIndexByName(((code_object*)tos)->names,((unicode_object*)name)->value);
						if(index != -1)
						{
							lgo = GetItem(((code_object*)tos)->names,index);
						}
						if(lgo != NULL) //push the imported function on stack
							stack_Push(bo->stack, lgo);
						else
						{
							#ifdef USE_DEBUGGING
							debug_printf(DEBUG_ALL,"warning import function was not found\n");
							#endif
						}
					}else if(tos->type == TYPE_CLASS)
						stack_Push(bo->stack, tos);
					//DumpObject(lgo,0);
				}
				break;
				
			case OPCODE_IMPORT_STAR:
				{
					code_object *co = (code_object*)tos;
					vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy(co->name)), tos);
				}
				break;
				
			case OPCODE_MAKE_FUNCTION:
				{
					NUM narg = (NUM) (arg & 255);
					NUM nkey = (NUM) ((arg >> 8) & 255);
					//NUM n = (NUM) (narg + (nkey*2));
					tuple_object *defaults = NULL;
					tuple_object *kw_defaults = NULL;
					
					if(narg > 0)
					{
						defaults = CreateTuple(narg); //creating defaults tuple
						for (NUM i = 0; i < narg; i++)
						{
							object *t = stack_Pop(bo->stack);
							t = DissolveRef(t);
							if(t->type == TYPE_KV)
								t = ((kv_object*)t)->value;
							SetItem((object*)defaults,i,t);
						}
					}
					if(nkey >0)
					{
						kw_defaults = CreateTuple(0); //creating kw defaults tuple
						for (NUM i = 0; i < nkey; i++)//keyword arguments
						{
							object *value = stack_Pop(bo->stack);
							value = DissolveRef(value);
							object *key = stack_Pop(bo->stack);
							key = DissolveRef(key);
							SetDictItem((object*)kw_defaults,key,value);
						}
					}
					function_object *fo = CreateFunctionObject_MAKE_FUNCTION((code_object*)tos,defaults,kw_defaults);
					stack_Push(bo->stack,(object*)fo);
					gc_IncRefCount((object*)defaults);
					gc_IncRefCount((object*)kw_defaults);
					gc_DecRefCount((object*)defaults);
					gc_DecRefCount((object*)kw_defaults);
					//printf("created function object: %x (%x)\n",fo,tos);
					//DumpObject(fo,0);
					//printf("code\n");
					//DumpObject(tos,0);
				}
				break;
				
			case OPCODE_MAKE_CLOSURE:
				{
					NUM narg = (NUM) (arg & 255);
					NUM nkey = (NUM) ((arg >> 8) & 255);
					//NUM n = (NUM) (narg + (nkey*2));
					tuple_object *defaults = NULL;
					tuple_object *kw_defaults = NULL;
					tos1 = stack_Pop(bo->stack);
					tos1 = DissolveRef(tos1);
					if(narg > 0)
					{
						defaults = CreateTuple(narg); //creating defaults tuple
						for (NUM i = 0; i < narg; i++)
						{
							object *t = stack_Pop(bo->stack);
							t = DissolveRef(t);
							if(t->type == TYPE_KV)
								t = ((kv_object*)t)->value;
							SetItem((object*)defaults,i,t);
						}
					}
					if(nkey >0)
					{
						kw_defaults = CreateTuple(0); //creating kw defaults tuple
						for (NUM i = 0; i < nkey; i++)//keyword arguments
						{
							object *value = stack_Pop(bo->stack);
							object *key = stack_Pop(bo->stack);
							SetDictItem((object*)kw_defaults,key,value);
						}
					}
					//DumpObject(tos1,0);
					function_object *fo = CreateFunctionObject_MAKE_CLOSURE((code_object*)tos,(tuple_object*)tos1,defaults,kw_defaults);
					stack_Push(bo->stack,(object*)fo);
					gc_IncRefCount((object*)defaults);
					gc_IncRefCount((object*)kw_defaults);
					gc_DecRefCount((object*)defaults);
					gc_DecRefCount((object*)kw_defaults);
				}
				break;
				
			case OPCODE_POP_JUMP_IF_FALSE:
				{
					if (tos->type == TYPE_FALSE)
					{
						bo->ip = arg;
					}
				}
				break;
				
			case OPCODE_POP_JUMP_IF_TRUE:
				{
					//DumpObject(tos,0);
					if (tos->type == TYPE_TRUE)
					{
						bo->ip = arg;
					}
				}
				break;
				
			case OPCODE_JUMP_FORWARD:
				{
					bo->ip = bo->ip + arg;
				}
				break;
				
			case OPCODE_JUMP_ABSOLUTE:
				{
					bo->ip = arg;
				}
				break;
				
			case OPCODE_JUMP_IF_FALSE:
				{
					//printf("before jump\n");
					//stack_Dump(bo->stack);
					//DumpObject(tos,0);
					if (tos->type == TYPE_FALSE)
					{
						stack_Push(bo->stack,tos);
						bo->ip = arg;
					}
					//else
					
					//printf("after jump cmp\n");
					//stack_Dump(bo->stack);
				}
				break;
				
			case OPCODE_JUMP_IF_TRUE:
				{
					if (tos->type == TYPE_TRUE)
					{
						bo->ip = arg;
						stack_Push(bo->stack,tos);
					}
					//else

				}
				break;

			case OPCODE_WITH_CLEANUP:
				{
					stack_Clear(bo->stack,1);
					gc_Clear();
				}
				break;
			
			case OPCODE_SETUP_WITH:
				{
					//printf("setup with:%d,%d\n",bo->ip,arg);
					//DumpObject(tos,0);
					block_object *block = AllocBlockObject();
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"setup with Creating new execution block: %x\n",block);
					#endif

					block->start = bo->ip;
					block->code = co;
					gc_IncRefCount((object*)co);
					block->ip = bo->ip;
					block->len = bo->ip + arg;
					block->ref_count = 0;
					block->type = TYPE_BLOCK;
					block->stack = stack_Init();
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject((object*)block,0);
					#endif
					stack_Push(vm->blocks, (object*)block);
					bo->ip = block->len;
					//stack_Push(bo->stack,tos);
					stack_Push(block->stack,tos);
				}
				break;
			
			case OPCODE_SETUP_LOOP:
				{
					block_object *block = AllocBlockObject();
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"setup loop Creating new execution block: %x\n",block);
					#endif
					block->start = bo->ip;
					block->code = co;
					gc_IncRefCount((object*)co);
					block->ip = bo->ip;
					block->len = bo->ip + arg;
					block->ref_count = 0;
					block->type = TYPE_BLOCK;
					block->stack = stack_Init();
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject((object*)bo,0);
					#endif
					stack_Push(vm->blocks, (object*)block);
					bo->ip = block->len;
				}
				break;
				
			case OPCODE_BUILD_SET:
				{
					tuple_object *to =CreateTuple(0);
					stack_Push(bo->stack,(object*)to);
				}
				break;
				
			case OPCODE_BUILD_MAP:
				{
					tuple_object *to =CreateTuple(0);
					stack_Push(bo->stack,(object*)to);
				}
				break;
				
			case OPCODE_BUILD_TUPLE:
			case OPCODE_BUILD_LIST:
				{
					tuple_object *blcall = NULL;
					blcall = CreateTuple(0);	// arg
					for (NUM i = 0; i < arg; i++)
					{
						//object *t = stack_Pop(bo->stack);
						object *t = stack_Get(bo->stack,stack_Pointer(bo->stack) - (arg-1-i));
						if(t->type == TYPE_ITER)
							iter_ExpandTuple((iter_object*)t,vm,blcall);
						else
							AppendItem((object*)blcall, t);
					}
					for (NUM i = 0; i < arg; i++)
					{
						stack_Pop(bo->stack);
					}
					stack_Push(bo->stack,(object*)blcall);
					#ifdef USE_DEBUGGING
					if(debug_level & DEBUG_VERBOSE_STEP)
						FullDumpObject((object*)blcall,0);
					#endif
				}
				break;
				
			case OPCODE_LOAD_GLOBAL:
				{
					tuple_object *co_names = (tuple_object *) co->names;
					object *name = obj_NULL;
					if(((object*)co_names->list->items[arg])->type == TYPE_KV)
						name = ((kv_object*)co_names->list->items[arg])->key;
					else
						name = co_names->list->items[arg];
					object *lgo = obj_NULL;
					lgo = vm_GetGlobal(vm,(object*)name);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"searching for global: ");
					if(debug_level & DEBUG_VERBOSE_STEP)
						PrintObject(name);
					debug_printf(DEBUG_VERBOSE_STEP,"\n");
					#endif
					//printf("\n");
					if(lgo == obj_NULL)
					{
						//printf("searching code objects\n");
						for(INDEX i=0;i<GetTupleLen((object*)vm->globals);i++)
						{	
							//printf("i:%d\n",i);
							code_object *co = (code_object*)GetDictItemByIndex((object*)vm->globals,i);
							if(co->type == TYPE_CODE)
							{
								//FullDumpObject(co,0);
								INDEX index = GetItemIndexByName(co->names,((unicode_object*)name)->value);
								if(index != -1)
								{
									lgo = GetItem(co->names,index);
								}
								if(lgo != obj_NULL)
									break;
								index = GetItemIndexByName(co->varnames,((unicode_object*)name)->value);
								if(index != -1)
								{
									lgo = GetItem(co->varnames,index);
								}
								if(lgo != obj_NULL)
									break;
							}
						}
					}
					stack_Push(bo->stack, lgo);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(lgo,1);
					#endif
				}
				break;

			case OPCODE_STORE_GLOBAL:
				{
					tuple_object *co_names = (tuple_object *) co->names;
					object *name = obj_NULL;
					if(((object*)co_names->list->items[arg])->type == TYPE_KV)
						name = ((kv_object*)co_names->list->items[arg])->key;
					else
						name = co_names->list->items[arg];
					for(INDEX i=0;i<GetTupleLen((object*)vm->globals);i++)
					{	
						code_object *co = (code_object*)GetDictItemByIndex((object*)vm->globals,i);
						if(co->type == TYPE_CODE)
						{
							INDEX index = GetItemIndexByName(co->names,((unicode_object*)name)->value);
							if(index != -1)
							{
								SetDictItemByIndex(co->names,index,tos);
								break;		
							}
							index = GetItemIndexByName(co->varnames,((unicode_object*)name)->value);
							if(index != -1)
							{
								SetDictItemByIndex(co->varnames,index,tos);
								break;
							}
						}
					}
				}
				break;

			case OPCODE_DELETE_GLOBAL:
				{
					//TODO needs to be TYPE_KV aware and has to remove unneeded objects
					tuple_object *co_names = (tuple_object *) co->names;
					object *name = obj_NULL;
					if(((object*)co_names->list->items[arg])->type == TYPE_KV)
						name = ((kv_object*)co_names->list->items[arg])->key;
					else
						name = co_names->list->items[arg];

					for(INDEX i=0;i<GetTupleLen((object*)vm->globals);i++)
					{	
						code_object *co = (code_object*)GetDictItemByIndex((object*)vm->globals,i);
						if(co->type == TYPE_CODE)
						{
							INDEX index = GetItemIndexByName(co->names,((unicode_object*)name)->value);
							if(index != -1)
							{
								SetDictItemByIndex(co->names,index,NULL);
								break;
							}
							index = GetItemIndexByName(co->varnames,((unicode_object*)name)->value);
							if(index != -1)
							{
								SetDictItemByIndex(co->varnames,index,NULL);
							}
						}
					}
				}
				break;

			case OPCODE_LOAD_NAME:
				{
					object *name = GetItem(co->names,arg);
					if(name->type == TYPE_KV)
						name = ((kv_object*)name)->value;
					stack_Push(bo->stack, name);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(name,1);
					#endif
				}
				break;

			case OPCODE_LOAD_CONST:
				{
					object *lconst = GetItem(co->consts,arg);
					stack_Push(bo->stack, lconst);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(lconst,1);
					#endif
				}
				break;

			case OPCODE_LOAD_FAST:
				{
					object *fast = GetItem(co->varnames,arg);
					if(fast->type == TYPE_KV)
						fast = ((kv_object*)fast)->value;
					stack_Push(bo->stack, fast);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(fast,1);
					#endif
				}
				break;
	
			case OPCODE_STORE_DEREF:
				{
					if(arg < GetTupleLen(co->cellvars))
					{
						if(GetItem(co->cellvars,arg)->type != TYPE_KV)
						{
							SetItem(co->cellvars,arg,(object*)ConvertToKVObjectValued(GetItem(co->cellvars,arg),tos));
						}
						else
						{
							SetDictItemByIndex(co->cellvars,arg,tos);
						}
					}
					else
					{
						if(GetItem(co->freevars,arg - GetTupleLen(co->cellvars))->type != TYPE_KV)
						{
							SetItem(co->freevars,arg - GetTupleLen(co->cellvars),(object*)ConvertToKVObjectValued(GetItem(co->freevars,arg - GetTupleLen(co->cellvars)),tos));
						}
						else
						{
							SetDictItemByIndex(co->freevars,arg - GetTupleLen(co->cellvars),tos);
						}
					}
					//printf("cellvars:\n");
					//FullDumpObject(co->cellvars,0);
					//printf("freevars:\n");
					//FullDumpObject(co->freevars,0);
					
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(tos,1);
					#endif
				}
				break;

			case OPCODE_LOAD_DEREF:
				{
					//printf("cellvars:\n");
					//FullDumpObject(co->cellvars,0);
					//printf("freevars:\n");
					//FullDumpObject(co->freevars,0);
					object *cell =  NULL;
					if(arg < GetTupleLen(co->cellvars))
						cell = GetItem(co->cellvars,arg);
					else
						cell = GetItem(co->freevars,arg - GetTupleLen(co->cellvars));
						if(cell->type == TYPE_KV)
							cell = ((kv_object*)cell)->value;
					stack_Push(bo->stack, cell);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(cell,1);
					#endif
				}
				break;

			case OPCODE_LOAD_CLOSURE:
				{
					object *fast = NULL;
					if(arg < GetTupleLen(co->cellvars))
						fast = GetItem(co->cellvars,arg);
					else
						fast = GetItem(co->freevars,arg - GetTupleLen(co->cellvars));
					stack_Push(bo->stack, fast);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(fast,1);
					#endif
				}
				break;

			case OPCODE_STORE_NAME:
				{
					if(GetItem(co->names,arg)->type != TYPE_KV)
					{
						SetItem(co->names,arg,(object*)ConvertToKVObjectValued(GetItem(co->names,arg),tos));
					}
					else
					{
						SetDictItemByIndex(co->names,arg,tos);
					}
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(tos,1);
					#endif
					//printf("stored name\n");
				}
				break;

			case OPCODE_STORE_MAP:
				{
					SetDictItem(tos2,tos,tos1);
					stack_Push(bo->stack,tos2);
				}
				break;

			case OPCODE_MAP_ADD:
				{
					tos2 = stack_Get(bo->stack,-arg);
					tos2 = DissolveRef(tos2);
					if(tos2->type == TYPE_KV)
						tos2 = ((kv_object*)tos2)->value;
					#ifdef USE_DEBUGGING
					assert(tos2 != NULL);
					DumpObject(tos2,0);
					printf("append dict item:\n");
					DumpObject(tos,0);
					printf("value:\n");
					DumpObject(tos1,0);
					#endif
					SetDictItem(tos2,tos,tos1);
				}
				break;

			case OPCODE_LIST_APPEND:
				{
					tos1 = stack_Get(bo->stack,-arg);
					tos1 = DissolveRef(tos1);
					if(tos1->type == TYPE_KV)
						tos1 = ((kv_object*)tos1)->value;
					#ifdef USE_DEBUGGING
					assert(tos1 != NULL);
					#endif
					AppendItem(tos1,tos);
				}
				break;

			case OPCODE_SET_ADD:
				{
					tos1 = stack_Get(bo->stack,-arg);
					tos1 = DissolveRef(tos1);
					if(tos1->type == TYPE_KV)
						tos1 = ((kv_object*)tos1)->value;
					AppendItem(tos1,tos);
				}
				break;
				
			case OPCODE_STORE_FAST:
				{
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(tos,1);
					#endif
					if(GetItem(co->varnames,arg)->type != TYPE_KV)
					{
						#ifdef USE_DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"converting\n");
						#endif
						SetItem(co->varnames,arg,(object*)ConvertToKVObjectValued(GetItem(co->varnames,arg),tos));
					}
				else
				{
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"updating\n");
					#endif
					SetDictItemByIndex(co->varnames,arg,tos);
				}
				#ifdef USE_DEBUGGING
				if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					FullDumpObject(tos,1);
				#endif
				}
				break;

			case OPCODE_DELETE_NAME:
				{
				DeleteItem(co->names, arg);
				}
				break;

			case OPCODE_DELETE_FAST:
				{
				DeleteItem(co->varnames, arg);
				}
				break;

			case OPCODE_DELETE_SUBSCR:
				{
					NUM dsa = 0;

					if (tos->type == TYPE_INT)
					{
						dsa = (NUM) ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_TUPLE)
					{
						DeleteItem(tos1, dsa);
					}
				}
				break;

			case OPCODE_STORE_SUBSCR:
				{
					NUM ssa = 0;
					 
					if (tos->type == TYPE_INT)
					{
						ssa = (NUM) ((int_object*)tos)->value;
						#ifdef USE_DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"ssa: %d\n",ssa);
						#endif
					}
					if (tos1->type == TYPE_TUPLE)
					{
					//if(ssa<0)
					//	printf("ssa:%d\n",ssa);
					if (tos->type == TYPE_INT)
						SetItem(tos1, ssa, tos2);
					else
						SetDictItem(tos1,tos,tos2);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					 {
						debug_printf(DEBUG_VERBOSE_STEP,"tos:%c\n",tos->type);
						DumpObject(tos,0);
						debug_printf(DEBUG_VERBOSE_STEP,"tos1:%c\n",tos1->type);
						DumpObject(tos1,0);
						debug_printf(DEBUG_VERBOSE_STEP,"tos2:%c\n",tos2->type);
						DumpObject(tos2,0);
					}
					#endif
					}
				}
				break;

			case OPCODE_BINARY_SUBSCR:
				{
					NUM bsa = 0;
					if (tos->type == TYPE_INT)
					{
						bsa = (NUM) ((int_object*)tos)->value;
						
					}
				
					if (tos1->type == TYPE_TUPLE)
					{
						//if(bsa<0)
						//	printf("bsa:%d\n",bsa);
						#ifdef USE_DEBUGGING
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						{
							DumpObject(tos1, 0);
							DumpObject(tos,0);
							debug_printf(DEBUG_VERBOSE_STEP,"bsa:%d\n", bsa);
						}
						#endif
						object *bst= NULL;
						if (tos->type == TYPE_INT)
							bst = GetItem(tos1, bsa);
						else
							bst = GetDictItem(tos1,tos);
						#ifdef USE_DEBUGGING
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
							DumpObject(bst, 0);
						#endif
						stack_Push(bo->stack, bst);
					}
				}
				break;

			case OPCODE_UNPACK_SEQUENCE: //TODO does not support tuples/iters for unpacking
				{
					for (NUM i = arg-1; i >= 0; i--)
					{
						switch (tos->type)
						{
						case TYPE_UNICODE:
							{
								unicode_object *uso = CreateUnicodeObject(str_FromChar(((unicode_object*)tos)->value[i]));
								#ifdef USE_DEBUGGING
								if((debug_level & DEBUG_VERBOSE_STEP) > 0)
									DumpObject((object*)uso,0);
								#endif
								stack_Push(bo->stack, (object*)uso);
							}
						}
					}
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
						stack_Dump(bo->stack);
					}
					#endif
				}
				break;

			case OPCODE_UNARY_POSITIVE:
				{
					if (tos->type == TYPE_INT)
					{
						int_object *up = CreateIntObject(+((int_object*)tos)->value);
						stack_Push(bo->stack, (object*)up);
					}
				}
				break;

			case OPCODE_UNARY_NEGATIVE:
				{
					if (tos->type == TYPE_INT)
					{
						int_object *up = CreateIntObject(-((int_object*)tos)->value);
						stack_Push(bo->stack, (object*)up);
					}
				}
				break;

			case OPCODE_UNARY_NOT:
				{
					//printf("unary not :%c\n",tos->type);
					if (tos->type == TYPE_INT)
					{
						//printf("int:%d\n",((int_object*)tos)->value);
						//object *up = CreateEmptyObject(((int_object*)tos)->value > 0 ? TYPE_TRUE : TYPE_FALSE);
						object *up = CreateEmptyObject(((int_object*)tos)->value == 0 ? TYPE_TRUE : TYPE_FALSE);
						stack_Push(bo->stack, up);
						//DumpObject(up,0);
					}
					else if (tos->type == TYPE_TRUE || tos->type == TYPE_FALSE)
					{
						object *up = CreateEmptyObject(tos->type == TYPE_TRUE ? TYPE_FALSE : TYPE_TRUE);
						stack_Push(bo->stack, up);
					}
					else if (tos->type == TYPE_NONE || tos->type == TYPE_NULL)
					{
						object *up = CreateEmptyObject( TYPE_TRUE);//TYPE_FALSE
						stack_Push(bo->stack, up);
					}
				}
				break;

			case OPCODE_UNARY_INVERT:
				{
					if (tos->type == TYPE_INT)
					{
						int_object *up = CreateIntObject(-((((int_object*)tos)->value) + 1));
						stack_Push(bo->stack, (object*)up);
					}
				}
				break;

			case OPCODE_INPLACE_POWER:
			case OPCODE_INPLACE_FLOOR_DIVIDE:
			case OPCODE_INPLACE_TRUE_DIVIDE:
			case OPCODE_INPLACE_MODULO:
			case OPCODE_INPLACE_SUBTRACT:
			case OPCODE_INPLACE_MULTIPLY:
			case OPCODE_INPLACE_ADD:
			case OPCODE_INPLACE_OR:
			case OPCODE_INPLACE_LSHIFT:
			case OPCODE_INPLACE_XOR:
			case OPCODE_INPLACE_RSHIFT:
			case OPCODE_INPLACE_AND:
			case OPCODE_BINARY_OR:
			case OPCODE_BINARY_XOR:
			case OPCODE_BINARY_AND:
			case OPCODE_BINARY_LSHIFT:
			case OPCODE_BINARY_RSHIFT:
			case OPCODE_BINARY_MODULO:
			case OPCODE_BINARY_FLOOR_DIVIDE:
			case OPCODE_BINARY_TRUE_DIVIDE:
			case OPCODE_BINARY_SUBTRACT:
			case OPCODE_BINARY_POWER:
			case OPCODE_BINARY_MULTIPLY:
			case OPCODE_BINARY_ADD:
				{
						object *new_tos = BinaryOp(tos,tos1,op);
						stack_Push(bo->stack, new_tos);
				}
				break;

			case OPCODE_COMPARE_OP:
				{
					object *new_tos = CompareOp(tos,tos1,arg);
					stack_Push(bo->stack, new_tos);
				}
				break;
				   
			case OPCODE_CALL_FUNCTION_VAR_KW:
			case OPCODE_CALL_FUNCTION_KW:
				{
						kw_list = (tuple_object*)stack_Pop(bo->stack);
				}
			case OPCODE_CALL_FUNCTION_VAR:
				{
					if(op == OPCODE_CALL_FUNCTION_VAR_KW || op == OPCODE_CALL_FUNCTION_VAR)
						var_list = (tuple_object*)stack_Pop(bo->stack);
				}
			case OPCODE_CALL_FUNCTION:
				{
					tuple_object *call = NULL;
					tuple_object *kw_call = NULL;
					NUM narg = (NUM) (arg & 255);
					NUM nkey = (NUM) ((arg >> 8) & 255);
					//NUM n = (NUM) (narg + (nkey*2));
					//fetch function object from tos and Resolve to function object
					//object *function = stack_Get(bo->stack,stack_Pointer(bo->stack) - n);
										
					if (narg > 0 || var_list != NULL)
					{
						call = CreateTuple(0);	// arg, 
						gc_IncRefCount((object*)call);
					}
					if (nkey > 0 || kw_list != NULL)
					{					
						kw_call = CreateTuple(0);
						gc_IncRefCount((object*)kw_call);
					}
					if(kw_list != NULL) //variable keyword arguments
					{
						for (NUM i = 0;i< kw_list->list->num; i++)
						{
							object *v = GetItem((object*)kw_list,i);
							AppendItem((object*)kw_call,v);
						}	
						
					}
					if(var_list != NULL) //variable list arguments
					{
						for (NUM i = 0;i< var_list->list->num; i++)
						{
							object *v = GetItem((object*)var_list,i);
							AppendItem((object*)call,v);
						}	
					}
					for (NUM i = 0; i < nkey; i++)//keyword arguments
					{
						object *value = stack_Pop(bo->stack);
						object *key = stack_Pop(bo->stack);
						kv_object *kv = CreateKVObject(key,value);
						AppendItem((object*)kw_call,(object*)kv);
					}
					for (NUM i = 0; i < narg; i++)//positional arguments
					{
						object *p = stack_Get(bo->stack,stack_Pointer(bo->stack) - (narg-1-i));
						AppendItem((object*)call,p);
					}
					for (NUM i = 0; i < narg; i++)//pop positional arguments
					{
					stack_Pop(bo->stack);
					}

					object *rf = stack_Pop(bo->stack);//finally pop the function 
					if(rf->type == TYPE_KV)
						rf = ((kv_object*)rf)->value;
					rf = DissolveRef(rf);
					object *ret = vm_StartObjectCopy(vm,rf,call,kw_call);
					if (ret != NULL)
					{
						stack_Push(bo->stack, ret);
					}
					gc_DecRefCount((object*)call);
					gc_DecRefCount((object*)kw_call);
				}
				break;

			default:
				#ifdef USE_DEBUGGING
				if((debug_level & DEBUG_DUMP_UNSUPPORTED) > 0)
				{
					int index = GetOpcodeIndex(op);
					debug_printf(DEBUG_DUMP_UNSUPPORTED,"[%02xh] [ %s ] not supported !!\n",opcodes[index].opcode, opcodes[index].name);
				}
				#endif
				break;
			}
			gc_Clear();
			if(vm->step_handler != NULL)
			{
				vm->step_handler(vm);
			}
			return(NULL);
		} //END OF STEP
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"code thru.\n");
		#endif
		object *ret = NULL;
		if(stack_IsEmpty(bo->stack))
		{
			stack_Pop(vm->blocks);
			gc_Clear();
			return(NULL);
		}
		else
		{
			stack_Pop(vm->blocks);
			ret = stack_Pop(bo->stack);
			gc_IncRefCount(ret);
			gc_Clear();
			return (ret);
		}
	}
	return (NULL);
}

#ifdef CREATELIB

int main(int argc, char *argv[])
{

}

#endif

