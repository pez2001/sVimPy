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
/*
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
*/
void vm_SetImportModuleHandler(VM_ID vm_id, OBJECT_ID(*import_module_handler) (VM_ID vm_id,BYTES_ID module_name))
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	vm->import_module_handler = import_module_handler;
	mem_unlock(vm_id,1);
}

void vm_SetExceptionHandler(VM_ID vm_id, OBJECT_ID(*exception_handler) (VM_ID vm_id,OBJECT_ID exception))
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	vm->exception_handler = exception_handler;
	mem_unlock(vm_id,1);
}

void vm_SetInterruptHandler(VM_ID vm_id, OBJECT_ID(*interrupt_handler) (VM_ID vm_id,STACK_ID stack))
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	vm->interrupt_handler = interrupt_handler;
	mem_unlock(vm_id,1);
}

void vm_SetStepHandler(VM_ID vm_id, OBJECT_ID(*step_handler) (VM_ID vm_id))
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	vm->step_handler = step_handler;
	mem_unlock(vm_id,1);
}

VM_ID vm_Init(CODE_ID co_id)//init vm and set global object if given
{
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"mem_Init\n");
	#endif	
	mem_Init();
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"obj_Init\n");
	#endif	
	obj_Init();
	#ifdef USE_MEMORY_DEBUGGING
	VM_ID tmp_id = mem_malloc_debug(sizeof(vm),MEM_POOL_CLASS_PERMANENT,  "vm_Init() return");
	#else
	VM_ID tmp_id = mem_malloc(sizeof(vm),MEM_POOL_CLASS_PERMANENT);
	#endif
	obj_SetVM(tmp_id);
	struct _vm *tmp = (struct _vm*)mem_lock(tmp_id);
	tmp->error_code = 0;
	tmp->error_message = 0;
	tmp->blocks = stack_Create();
	tmp->globals = obj_CreateTuple(0);
	obj_IncRefCount(tmp->globals);
	tmp->running = 0;
	tmp->interrupt_vm = 0;
	tmp->interrupt_handler = NULL;
	tmp->exception_handler = NULL;
	tmp->step_handler = NULL;
	tmp->import_module_handler = NULL;
	mem_unlock(tmp_id,1);

	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"streams_Init\n");
	#endif	
	streams_Init();

	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"AddInternalFunctions\n");
	#endif
	#ifdef USE_INTERNAL_FUNCTIONS
	AddInternalFunctions(tmp_id);
	#endif
	#ifdef USE_INTERNAL_CLASSES
	AddInternalClasses(tmp_id);
	#endif
	#ifndef USE_ARDUINO_FUNCTIONS
	AddArduinoGlobal(tmp_id);
	#endif
	//AddArduinoFunctions(vm);
	//AddArduinoGlobals(vm);
	//#endif
	
	//AddFmodGlobals(vm);
	if(co_id != 0)
	{
		vm_AddGlobal(tmp_id,obj_CreateUnicode(mem_create_string("__main__")),co_id);
	}
	return(tmp_id);
}

NUM vm_Close(VM_ID vm_id)//close vm and free all of its used memory
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	NUM code = vm->error_code;
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing vm\n");
	#endif
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"cleaning gc\n");
	#endif

	stack_Clear(vm->blocks, 1);
	obj_DecRefCount(vm->globals);
	obj_ClearGC();
	//if(vm->error_message != 0) TODO ++
	//	mem_free(vm->error_message));
	stack_Close(vm->blocks,0);
	mem_unlock(vm_id,0);
	mem_free(vm_id);
 
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing streams\n");
	#endif
	streams_Close();	
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing obj\n");
	#endif
	obj_Close();
	#ifdef USE_DEBUGGING
	if(mem_Close())
		if(code == 0)
			code = 101;
	#endif
	return(code);
}

void vm_FreeGlobals(VM_ID vm_id)
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	for(INDEX i=0;i<tuple_GetLen(vm->globals);i++)
	{	
			obj_DecRefCount(tuple_GetItem(vm->globals,i));
	}
	mem_unlock(vm_id,0);
}

void vm_AddGlobal(VM_ID vm_id,OBJECT_ID key,OBJECT_ID global)//add a global object
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	tuple_AppendDictItem(vm->globals,key,global);
	mem_unlock(vm_id,0);
}

void vm_RemoveGlobal(VM_ID vm_id, OBJECT_ID key)//remove a global object
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	//obj_Dump(vm->globals,0,0);
	//printf("removing obj by key:%d\n",key);
	//obj_Dump(key,1,1);
	tuple_DeleteDictItem(vm->globals,key);
	mem_unlock(vm_id,0);
}

OBJECT_ID vm_GetGlobal(VM_ID vm_id, OBJECT_ID key)//retrieve a global object
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	OBJECT_ID r = tuple_GetDictItem(vm->globals,key);
	mem_unlock(vm_id,0);
	return(r);
}

OBJECT_ID vm_GetGlobalByIndex(VM_ID vm_id, INDEX i)
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	OBJECT_ID r = tuple_GetDictItemByIndex(vm->globals,i);
	mem_unlock(vm_id,0);
	return(r);
}

void vm_Interrupt(VM_ID vm_id, OBJECT_ID(*interrupt_func) (VM_ID vm_id,STACK_ID stack)) //interrupt vm and call interrupt handler afterwards
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	vm->interrupt_handler = interrupt_func;
	vm->interrupt_vm = 1;
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"forcing interrupt\n");
	#endif
	mem_unlock(vm_id,1);
}

void vm_Continue(VM_ID vm_id)//continue vm execution
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	vm->interrupt_vm = 0;
	mem_unlock(vm_id,1);
}

void vm_Exit(VM_ID vm_id,BYTES_ID message,NUM code) //exit vm
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	vm->interrupt_vm = 0;
	vm->running = 0;
	vm->error_message = message;
	vm->error_code = code;
	//vm_Close(vm);
	stack_Clear(vm->blocks,1);
	mem_unlock(vm_id,1);
}

void vm_Stop(VM_ID vm_id)//pause vm execution
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	vm->interrupt_vm = 0;
	vm->running = 0;
	mem_unlock(vm_id,1);
}

OBJECT_ID vm_StartObjectCopy(VM_ID vm_id,OBJECT_ID obj_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id)
{
	if(obj_id == 0)
		return(0);
	object *obj = (object*)mem_lock(obj_id);
		
	if(obj->type == TYPE_UNICODE)
	{
		OBJECT_ID f = vm_GetGlobal(vm_id,obj_id);
		if(f != 0)
		{
			mem_unlock(obj_id,0);
			OBJECT_ID uret = vm_StartObjectCopy(vm_id,f, locals_id,kw_locals_id);
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
		mem_unlock(obj_id,0);
		OBJECT_ID r = vm_StartObject(vm_id,obj_id,locals_id,kw_locals_id);
		return(r);
	}
	/*else if(obj->type == TYPE_METHOD)
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
	}*/
	else
	{
		mem_unlock(obj_id,0);
		OBJECT_ID c = obj_Copy(obj_id);
		obj_IncRefCount(c);
		OBJECT_ID r = vm_StartObject(vm_id,c,locals_id,kw_locals_id);
		obj_DecRefCount(c);
		return(r);
	}
	mem_unlock(obj_id,0);
	return(0);
}

OBJECT_ID vm_StartObject(VM_ID vm_id,OBJECT_ID obj_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id)
{
	if(obj_id == 0)
		return(0);
	object *obj = (object*)mem_lock(obj_id);

	switch(obj->type)
	{
		case TYPE_METHOD:
			{
				#ifdef USE_DEBUGGING
				debug_printf(DEBUG_VERBOSE_STEP,"executing method\n");
				#endif
				mem_unlock(obj_id,0);
				OBJECT_ID rmo = vm_StartMethodObject(vm_id,obj_id,locals_id,kw_locals_id);
				return(rmo);
			}
		case TYPE_CLASS:
			{
				#ifdef USE_DEBUGGING 
				debug_printf(DEBUG_VERBOSE_STEP,"executing class initializer -> returning finalized class instance\n");
				#endif
				//printf("start object class co_flags:%d\n",((class_object*)obj)->code->co_flags);
				mem_unlock(obj_id,0);
				vm_StartClassObject(vm_id,obj_id,locals_id,kw_locals_id);
				return(0);
			}
		case TYPE_CODE:
			{
				mem_unlock(obj_id,0);
				vm_StartCodeObject(vm_id,obj_id,locals_id,kw_locals_id);
				return(0);
			}
		case TYPE_CFUNCTION:
			{
				mem_unlock(obj_id,0);
				OBJECT_ID pret = vm_StartCFunctionObject(vm_id,obj_id, locals_id,kw_locals_id);
				return(pret);
			}
		case TYPE_UNICODE:
			{
				mem_unlock(obj_id,0);
				OBJECT_ID f = vm_GetGlobal(vm_id,obj_id);
				if(f != 0)
				{
					OBJECT_ID pret = vm_StartObject(vm_id,f, locals_id,kw_locals_id);
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
				mem_unlock(obj_id,0);
				OBJECT_ID pret = vm_StartFunctionObject(vm_id,obj_id, locals_id,kw_locals_id);
				//gc_DecRefCount((object*)obj);
				return(pret);
			}
	}
	mem_unlock(obj_id,0);
	return(0);
}

BLOCK_ID vm_StartCodeObject(VM_ID vm_id,CODE_ID co_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id)  //TODO support kw_locals
{
	code_object *co = (code_object*)mem_lock(co_id);
	//code_object *co = (code_object*)obj;
	STRING_ID bytecodes_id = co->code;
	mem_unlock(co_id,0);
	BLOCK_ID bo_id = obj_Alloc(TYPE_BLOCK,0);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"sco Creating new execution block: %d\n",bo_id);
	#endif
	block_object *bo =(block_object*)mem_lock(bo_id);
	bo->code = co_id;
	obj_IncRefCount(bo->code);
	bo->ip = 0;
	bo->start = 0;
	string_object *bytecodes = (string_object*)mem_lock(bytecodes_id);
	bo->len = bytecodes->len;
	mem_unlock(bytecodes_id,0);
	bo->stack = stack_Create();
	mem_unlock(bo_id,1);
	// co->stacksize
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	stack_Push(vm->blocks,bo_id);
	mem_unlock(vm_id,0);
	if(locals_id != 0)
	{
		for(NUM i = 0; i < tuple_GetLen(locals_id); i++)
		{
			OBJECT_ID local = tuple_GetItem(locals_id,i);
			if(obj_GetType(tuple_GetItem(co->varnames,i)) != TYPE_KV)
			{
				tuple_SetItem(co->varnames,i,obj_ConvertToValuedKV(tuple_GetItem(co->varnames,i),local));
			}
			else
			{
				tuple_SetDictItemByIndex(co->varnames,i,local);
			}
		}
	}
	return(bo_id);
}

BLOCK_ID vm_StartClassObject(VM_ID vm_id,CLASS_ID class_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id) //TODO support kw_locals
{
	class_object *_class = (class_object*)mem_lock(class_id);
	CODE_ID ccode = _class->code;
	mem_unlock(class_id,0);
	code_object *code = (code_object*)mem_lock(ccode);
	BLOCK_ID bo_id = 0;
	if(code->code != 0)
	{
		STRING_ID bytecodes_id = code->code;
		mem_unlock(ccode,0);
		bo_id = obj_Alloc(TYPE_BLOCK,0);
		block_object *bo =(block_object*)mem_lock(bo_id);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"sclo Creating new execution block: %d\n",bo_id);
		#endif
		bo->code = ccode;
		obj_IncRefCount(bo->code);
		bo->ip = 0;
		bo->start = 0;
		string_object *bytecodes = (string_object*)mem_lock(bytecodes_id);
		bo->len = bytecodes->len;
		mem_unlock(bytecodes_id,0);
		bo->stack = stack_Create();		// co->stacksize
		mem_unlock(bo_id,1);
		struct _vm *vm = (struct _vm*)mem_lock(vm_id);
		stack_Push(vm->blocks,bo_id);
		mem_unlock(vm_id,0);
		stack_Push(bo->stack,class_id);//push class to be retrieved later by OPCODE_RETURN_VALUE
		if(locals_id != 0)
		{
			tuple_SetDictItemByIndex(code->varnames,0,locals_id);
		}
	}
	else
	{
		mem_unlock(ccode,0);
		//no code defined
		//push class instance
		struct _vm *vm = (struct _vm*)mem_lock(vm_id);
		CLASS_INSTANCE_ID cio = obj_CreateClassInstance(class_id);
		if(!stack_IsEmpty(vm->blocks))
		{
			BLOCK_ID actual_id = stack_Top(vm->blocks);
			block_object *actual = (block_object*)mem_lock(actual_id);
			stack_Push(actual->stack,cio);
			mem_unlock(actual_id,0);
		}
		mem_unlock(vm_id,0);
		UNICODE_ID method_name = obj_CreateUnicode(mem_create_string("__init__"));
		//OBJECT_ID rmr = vm_RunMethod(vm_id,method_name,cio,locals_id,kw_locals_id);
		OBJECT_ID rmr = vm_RunMethod(vm_id,method_name,cio,locals_id,0);
		obj_IncRefCount(rmr);
		obj_DecRefCount(rmr);
		obj_IncRefCount(method_name);
		obj_DecRefCount(method_name);
		obj_ClearGC();
	}
	
	_class = (class_object*)mem_lock(class_id);
	TUPLE_ID bc = _class->base_classes;
	mem_unlock(class_id,0);
	for(INDEX i = 0;i<tuple_GetLen(bc);i++)
	{
		CLASS_ID bci_id = tuple_GetItem(bc,i);
		class_object *bci = (class_object*)mem_lock(bci_id);
		code_object *bci_code = (code_object*)mem_lock(bci->code);
		
		bci_code->co_flags |= CO_SUB_CLASS_ROOT;
		bci_code->co_flags &= ~CO_CLASS_ROOT;
		mem_unlock(bci->code,1);
		mem_unlock(bci_id,0);
		vm_StartClassObject(vm_id,bci_id,locals_id,kw_locals_id);//TODO check if this makes actually sense (var passing to base classes)
	}
	return(bo_id);
}

OBJECT_ID vm_StartFunctionObject(VM_ID vm_id,FUNCTION_ID fo_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id)//run a python function object 
{	
	function_object *fo = (function_object*)mem_lock(fo_id);
	//#ifdef USE_DEBUGGING
	//debug_printf(DEBUG_VERBOSE_STEP,"executing python function: %s\n",  fo->func->name);
	//#endif
	CODE_ID fc_id = fo->func;
	mem_unlock(fo_id,0);
	BLOCK_ID bo_id = obj_Alloc(TYPE_BLOCK,0);
	block_object *bo =(block_object*)mem_lock(bo_id);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"sfo Creating new execution block: %d\n",bo_id);
	#endif
	bo->ip = 0;
	bo->start = 0;
	bo->stack = stack_Create();		// co->stacksize
	code_object *fc = (code_object*)mem_lock(fc_id);
	STRING_ID bytecodes_id = fc->code;
	bo->code = fc_id;
	obj_IncRefCount(bo->code);
	string_object *bytecodes = (string_object*)mem_lock(bytecodes_id);
	bo->len = bytecodes->len;
	mem_unlock(bytecodes_id,0);
	mem_unlock(fc_id,0);
	mem_unlock(bo_id,1);
	fc = (code_object*)mem_lock(fc_id);
	if(locals_id != 0)
	{
		NUM oarg = tuple_GetLen(locals_id);
		if(oarg > fc->argcount)
			oarg = fc->argcount;
		for (NUM i = 0; i < oarg; i++)
		{
			OBJECT_ID local = tuple_GetItem(locals_id,i);
			if(obj_GetType(tuple_GetItem(fc->varnames,i)) != TYPE_KV)
			{
				tuple_SetItem(fc->varnames,i,obj_ConvertToValuedKV(tuple_GetItem(fc->varnames,i),local));
			}
			else
			{
				tuple_SetDictItemByIndex(fc->varnames,i,local);
			}
		}
		//printf("loading arguments 2\n");
		if(tuple_GetLen(locals_id) > fc->argcount)
		{
			NUM narg = tuple_GetLen(locals_id) - fc->argcount;
			TUPLE_ID nt = obj_CreateTuple(0); //creating tuple from the rest of the arguments
			for (NUM i = 0; i < narg; i++)
			{
				OBJECT_ID t_id = tuple_GetItem(locals_id,oarg+i);
				if(obj_GetType(t_id) == TYPE_KV)
				{
					kv_object *t = (kv_object*)mem_lock(t_id);
					t_id = t->value;
					mem_unlock(t_id,0);
				}
				tuple_AppendItem(nt,t_id);
			}
			tuple_SetDictItemByIndex(fc->varnames,fc->argcount+fc->kwonlyargcount,nt);
		}		
	}
	if(kw_locals_id != 0)
	{
		//printf("loading keyword arguments\n");
		for(NUM i = 0;i<tuple_GetLen(kw_locals_id);i++)
		{
			KV_ID kw_local_id = tuple_GetItem(kw_locals_id,i);
			kv_object *kw_local = (kv_object*)mem_lock(kw_local_id);
			tuple_SetDictItem(fc->varnames,kw_local->key,kw_local->value);
			mem_unlock(kw_local_id,0);
		}
	}
	if((fc->co_flags & CO_GENERATOR)>0 )
	{
		ITER_ID iter = obj_CreateIter();
		iter_InitGenerator(iter,vm_id,bo_id);
		mem_unlock(fc_id,0);
		return(iter);
	}
	mem_unlock(fc_id,0);
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	//normal functions get pushed on the block stack
	stack_Push(vm->blocks, bo_id);
	mem_unlock(vm_id,0);
	return(0);//got nothing to return now
}

OBJECT_ID vm_StartCFunctionObject(VM_ID vm_id,CFUNCTION_ID cfo_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id)//run a c function object 
{
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"executing C function object\n");
	#endif
	cfunction_object *cfo =(cfunction_object*)mem_lock(cfo_id);
	OBJECT_ID r = (*cfo->func) (vm_id,locals_id,kw_locals_id);
	mem_unlock(cfo_id,0);
	return(r);
}

OBJECT_ID vm_RunMethod(VM_ID vm_id,OBJECT_ID key,CLASS_INSTANCE_ID cio,TUPLE_ID locals_id,TUPLE_ID kw_locals_id)
{
	TUPLE_ID l = locals_id;
	OBJECT_ID m = obj_GetClassMethod(cio,key);
	if(m == 0)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"method not found\n");
		if((debug_level & DEBUG_VERBOSE_STEP) > 0)
			obj_Dump(key,0,1);
		#endif
		return(0);
	}
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"running method\n");
	#endif
	if(l == 0)
	{
		l = obj_CreateTuple(0);
		obj_IncRefCount(l);
	}
	tuple_InsertItem(l,0,cio);
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	NUM s = stack_GetLen(vm->blocks);
	mem_unlock(vm_id,0);
	NUM n = 0;
	OBJECT_ID ret = 0;
	if(obj_GetType(m) == TYPE_FUNCTION)
	{
		ret = vm_StartFunctionObject(vm_id,m,l,kw_locals_id);
		while(ret == 0 && s != n) //TODO replace this construct with a more streamlined version without the need to call step in a loop
		{
			ret = vm_Step(vm_id);
			struct _vm *vm = (struct _vm*)mem_lock(vm_id);
			n = stack_GetLen(vm->blocks);
			mem_unlock(vm_id,0);
		}
		struct _vm *vm = (struct _vm*)mem_lock(vm_id);
		BLOCK_ID top_id = stack_Top(vm->blocks);
		mem_unlock(vm_id,0);
		block_object *top = (block_object*)mem_lock(top_id);
		ret = stack_Pop(top->stack);
		mem_unlock(top_id,0);
		function_object *mo = (function_object*)mem_lock(m);
		code_object *moc = (code_object*)mem_lock(mo->func);
		tuple_SetItem(moc->varnames,0,0);//remove loop reference
		mem_unlock(mo->func,0);
		mem_unlock(m,0);
    }else if(obj_GetType(m) == TYPE_CFUNCTION)
	{
		ret = vm_StartCFunctionObject(vm_id,m,l,kw_locals_id);//gc_DecRefCount(locals);
	}

	if(locals_id == 0)
	{
		obj_DecRefCount(l);
	}
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"method thru\n");
	#endif
	return(ret);
}

OBJECT_ID vm_StartMethod(VM_ID vm_id,OBJECT_ID key,CLASS_INSTANCE_ID cio_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id)
{
	TUPLE_ID l = locals_id;
	OBJECT_ID m = obj_GetClassMethod(cio_id,key);
	if(m == 0)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"method not found\n");
		obj_Dump(key,0,1);
		#endif
		return(0);
	}
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"starting method\n");
	#endif
	if(l == 0)
	{
		l = obj_CreateTuple(0);
		obj_IncRefCount(l);
	}
	tuple_InsertItem(l,0,cio_id);
	OBJECT_ID r = 0;
	if(obj_GetType(m) == TYPE_FUNCTION)
		r = vm_StartFunctionObject(vm_id,m,l,kw_locals_id);
	else if(obj_GetType(m) == TYPE_CFUNCTION)
		r = vm_StartCFunctionObject(vm_id,m,l,kw_locals_id);
	if(locals_id == 0)
		obj_DecRefCount(l);
	return(r);
}

OBJECT_ID vm_StartMethodObject(VM_ID vm_id,METHOD_ID mo_id,TUPLE_ID locals_id,TUPLE_ID kw_locals_id)
{
	TUPLE_ID l = locals_id;
	if(l == 0)
	{
		l = obj_CreateTuple(0);
		obj_IncRefCount(l);
	}
	method_object *mo = (method_object*)mem_lock(mo_id);
	tuple_InsertItem(l,0,mo->instance);
	OBJECT_ID r = 0;
	if(obj_GetType(mo->func) == TYPE_FUNCTION)
		r = vm_StartFunctionObject(vm_id,mo->func,l,kw_locals_id);
	else if(obj_GetType(mo->func)== TYPE_CFUNCTION)
		r = vm_StartCFunctionObject(vm_id,mo->func,l,kw_locals_id);
	if(locals_id == 0)
		obj_DecRefCount(l);
	mem_unlock(mo_id,0);
	return(r);
}
/*
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
			}
			else
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
*/
/*
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
			if(strlen(cmd)>= 2 && cmd[0] == 'd' && cmd[1] == 'i')
			{
				printf("dumping iter\n");
				block_object *b = (block_object*)stack_Top(vm->blocks);
				if(b->iter != NULL)
					DumpObject(b->iter,0);
				else
					printf("block has no iter\n");
				printf("dumped\n");
			}else
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
*/

OBJECT_ID vm_RunFunction(VM_ID vm_id,BYTES_ID name_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id)//call a python function from C
{
	OBJECT_ID f = 0;
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	
	for(int i=0;i<tuple_GetLen(vm->globals);i++)
	{	
			if(obj_GetType(vm_GetGlobalByIndex(vm_id,i)) == TYPE_CODE)
			{
				OBJECT_ID fcode_id = vm_GetGlobalByIndex(vm_id,i);
				code_object *fcode = (code_object*)mem_lock(fcode_id);
				f = tuple_GetDictItemByName(fcode->names,name_id);
				mem_unlock(fcode_id,0);
			}
			if(f != 0)
				break;
	}
	mem_unlock(vm_id,0);
	if(obj_GetType(f) == TYPE_FUNCTION)
	{
		OBJECT_ID ret = vm_StartFunctionObject(vm_id,f,locals_id,kw_locals_id);
		ret = 0;
			while(ret == 0)
				ret = vm_Step(vm_id);
		return(ret);
	}
	return(0);
}

OBJECT_ID vm_RunPYC(VM_ID vm_id,STREAM_ID f,BOOL free_object)
{
	long pyc_magic = MAGIC;
	BOOL r = stream_Open(f);
	if(!r)
		return(0);//TODO maybe return an empty object for simplification
	long magic = stream_ReadLong(f);
	if(magic != pyc_magic)
		return(0);
	stream_ReadLong(f);//read time
	OBJECT_ID obj = stream_ReadObject(f);
	OBJECT_ID global_key = obj_CreateUnicode(mem_create_string("__main__"));
	vm_AddGlobal(vm_id, global_key,obj);
	OBJECT_ID ret = vm_RunObject(vm_id, obj, 0,0);
	if (ret != 0)
	{
		obj_DecRefCount(ret);
	}
	if(free_object)
	{
		obj_DecRefCount(obj);
		vm_RemoveGlobal(vm_id,global_key);
		stream_Free(f);
		return(0);
	}
	stream_Free(f);
	return(obj);
}

OBJECT_ID vm_RunRPYC(VM_ID vm_id,STREAM_ID f ,BOOL free_object)
{
	BOOL r = stream_Open(f);
	if(!r)
		return(0);//TODO maybe return an empty object for simplification
	OBJECT_ID obj = stream_ReadObject(f);
	OBJECT_ID global_key = obj_CreateUnicode(mem_create_string("__main__"));
	vm_AddGlobal(vm_id, global_key,obj);
	OBJECT_ID ret = vm_RunObject(vm_id, obj,0,0);
	
	if(ret != 0)
	{
		obj_DecRefCount(ret);
	}
	if(free_object)
	{
		obj_DecRefCount(obj);
		vm_RemoveGlobal(vm_id,global_key);
		stream_Free(f);
		return(0);
	}
	stream_Free(f);
	return(obj);
}

OBJECT_ID vm_RunRPYCPlus(VM_ID vm_id,STREAM_ID f ,BOOL free_object)
{
	BOOL r = stream_Open(f);
	if(!r)
		return(0);//TODO maybe return an empty object for simplification
	OBJECT_ID obj = stream_ReadObjectPlus(f);
	OBJECT_ID global_key = obj_CreateUnicode(mem_create_string("__main__"));
	vm_AddGlobal(vm_id, global_key,obj);
	OBJECT_ID ret = vm_RunObject(vm_id, obj,0,0);
	if(ret != 0)
	{
		obj_DecRefCount(ret);
	}
	if(free_object)
	{
		obj_DecRefCount(obj);
		vm_RemoveGlobal(vm_id,global_key);
		stream_Free(f);
		return(0);
	}
	stream_Free(f);
	return(obj);
}

OBJECT_ID vm_RunObject(VM_ID vm_id, OBJECT_ID obj_id, TUPLE_ID locals_id,TUPLE_ID kw_locals_id)//run a python object if possible
{
	if(obj_GetType(obj_id) != TYPE_CODE)
		return(obj_CreateEmpty(TYPE_NONE));
	UNICODE_ID var_name = obj_CreateUnicode(mem_create_string("__name__"));
	obj_IncRefCount(var_name);
	UNICODE_ID module_name = obj_CreateUnicode(mem_create_string("__main__"));
	code_object *code = (code_object*)mem_lock(obj_id);
	tuple_SetDictItem(code->names,var_name,module_name);
	mem_unlock(obj_id,0);
	obj_DecRefCount(var_name);
	vm_StartObject(vm_id,obj_id,locals_id,kw_locals_id);
	OBJECT_ID ret = 0;
	while(ret == 0)
		ret = vm_Step(vm_id);
	return(ret);
}

OBJECT_ID vm_Step(VM_ID vm_id)
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	BLOCK_ID bo_id = stack_Top(vm->blocks);
	block_object *bo = (block_object*)mem_lock(bo_id);
	OBJECT_ID obj_id = bo->code;
	object *obj = (object*)mem_lock(obj_id);
	if(obj->type != TYPE_CODE)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"stepping non code:%c(%d)\n",obj->type,obj_id);
		#endif
		mem_unlock(obj_id,0);
		mem_unlock(bo_id,0);
		mem_unlock(vm_id,0);
		return(obj_CreateEmpty(TYPE_NONE));
	}
	else
	{
		code_object *co = (code_object*)obj;
		STRING_ID bytecodes_id = co->code;
		string_object *bytecodes = (string_object*)mem_lock(bytecodes_id);
		BYTES_ID string_id = bytecodes->content;
		char *string =(char*)mem_lock(string_id);
	
		if(bo->ip < bo->len)
		{
			//printf("step\r\n");
			unsigned char op = (unsigned char)string[bo->ip++];	// get op and increment code pointer
			BOOL has_extended_arg = 0;
			short extended_arg = 0;
			INT arg = 0;		// holds the argument if present
			BOOL op_thru = 0;	// used to skip unneccessary switches

			OBJECT_ID tos_id = 0; //top of the stack objects
			OBJECT_ID tos1_id = 0;
			OBJECT_ID tos2_id = 0;
			object *tos = NULL;
			object *tos1 = NULL;
			object *tos2 = NULL;
				
			TUPLE_ID var_list = 0;//used for simpler function calling
			TUPLE_ID kw_list = 0;//used for simpler function calling
			
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
			/*
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
			*/
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
					OBJECT_ID ret = stack_Pop(bo->stack);
					obj_IncRefCount(ret);
					obj_ClearGC();
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
						debug_printf(DEBUG_VERBOSE_STEP,"yield object @:%d\n",ret);
						obj_Dump(ret,0,1);
					}
					#endif			
					mem_unlock(string_id,0);
					mem_unlock(bytecodes_id,0);
					mem_unlock(obj_id,1);
					mem_unlock(bo_id,0);
					mem_unlock(vm_id,0);
					return(ret);
				}
				break;
				
			case OPCODE_RETURN_VALUE:
				{
					OBJECT_ID ret = stack_Pop(bo->stack);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
						debug_printf(DEBUG_VERBOSE_STEP,"ret object @:%d\n",ret);
						obj_Dump(ret,0,1);
					}
					#endif
					if(((co->co_flags & CO_SUB_CLASS_ROOT) > 0) )// || (bo->code->co_flags & CO_CLASS_CONSTRUCTOR) > 0))
					{
						stack_Pop(vm->blocks);
						co->co_flags |= CO_CLASS_ROOT;
						co->co_flags &= ~CO_SUB_CLASS_ROOT;
						mem_unlock(string_id,0);
						mem_unlock(bytecodes_id,0);
						mem_unlock(obj_id,1);
						mem_unlock(bo_id,0);
						mem_unlock(vm_id,0);
						obj_ClearGC();
						return(0);
					}
					else if((co->co_flags & CO_CLASS_ROOT) > 0)
					{
						//printf("return in class root\n");
						stack_Pop(vm->blocks);
						CLASS_ID c_id = stack_Pop(bo->stack);//this was initially pushed on stack by vm_StartClassObject();
						CLASS_INSTANCE_ID cio_id = obj_CreateClassInstance(c_id);
						UNICODE_ID locals_name = obj_CreateUnicode(mem_create_string("__locals__"));
						obj_IncRefCount(locals_name);
						class_object *c = (class_object*)mem_lock(c_id);
						code_object *cc = (code_object*)mem_lock(c->code);
						OBJECT_ID locals = tuple_GetDictItem(cc->varnames,locals_name);
						tuple_SetDictItem(cc->varnames,locals_name,0);
						obj_DecRefCount(locals_name);
						obj_IncRefCount(locals);
						if(!stack_IsEmpty(vm->blocks))
						{
							BLOCK_ID parent_id = stack_Top(vm->blocks);
							block_object *parent = (block_object*)mem_lock(parent_id);
							stack_Push(parent->stack,cio_id);
							mem_unlock(parent_id,0);
						}
						
						UNICODE_ID method_name = obj_CreateUnicode(mem_create_string("__init__"));
						obj_IncRefCount(method_name);
						OBJECT_ID rmr = vm_RunMethod(vm_id,method_name,cio_id,locals,0);
						obj_IncRefCount(rmr);
						obj_DecRefCount(rmr);
						obj_DecRefCount(method_name);
						obj_DecRefCount(locals);
						obj_ClearGC();
						mem_unlock(c->code,0);
						mem_unlock(c_id,0);
						mem_unlock(string_id,0);
						mem_unlock(bytecodes_id,0);
						mem_unlock(obj_id,1);
						mem_unlock(bo_id,0);
						mem_unlock(vm_id,0);
						return(0);
					} 
					else if((co->co_flags & CO_MODULE_ROOT) > 0)//TODO simplify and move this to import & maybe leave the stack pop
					{
						stack_Pop(vm->blocks);
						UNICODE_ID mkey = obj_CreateUnicode(mem_copy(co->name));
						vm_RemoveGlobal(vm_id,mkey);
						obj_IncRefCount(mkey);
						obj_DecRefCount(mkey);
						
					} else
					{
						stack_Pop(vm->blocks);
						if(!stack_IsEmpty(vm->blocks))
						{
							BLOCK_ID parent_id = stack_Top(vm->blocks);
							block_object *parent = (block_object*)mem_lock(parent_id);
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
							mem_unlock(parent_id,0);
						}
						else 
						{
							obj_IncRefCount(ret);
							mem_unlock(string_id,0);
							mem_unlock(bytecodes_id,0);
							mem_unlock(obj_id,1);
							mem_unlock(bo_id,0);
							mem_unlock(vm_id,0);
							obj_ClearGC();
							return(ret);
						}
					}
					//obj_Dump(326,1,1);
					mem_unlock(string_id,0);
					mem_unlock(bytecodes_id,0);
					mem_unlock(obj_id,1);
					mem_unlock(bo_id,0);
					mem_unlock(vm_id,0);
					//printf("step thru\n");
					obj_ClearGC();
					//printf("gc thru\n"); 
					return(0);
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
					//#ifdef USE_DEBUGGING
					//block_object *bbo = (block_object *) stack_Pop(vm->blocks);
					//debug_printf(DEBUG_VERBOSE_STEP,"break to: %d ,start: %d ,len:%d\n",bo->ip,bbo->start,bbo->len);
					//#else
					stack_Pop(vm->blocks);
					//#endif
					op_thru = 1;
				}
				break;

			case OPCODE_ROT_TWO:
				{
					tos_id = stack_Top(bo->stack);
					tos1_id = stack_Second(bo->stack);
					stack_SetTop(bo->stack, tos1_id);
					stack_SetSecond(bo->stack, tos_id);
					op_thru = 1;
				}
				break;

			case OPCODE_ROT_THREE:
				{
					tos_id = stack_Top(bo->stack);
					tos1_id = stack_Second(bo->stack);
					tos2_id = stack_Third(bo->stack);
					stack_SetTop(bo->stack, tos1_id);
					stack_SetSecond(bo->stack, tos2_id);
					stack_SetThird(bo->stack, tos_id);
					op_thru = 1;
				}
				break;

			case OPCODE_DUP_TOP:
				{
					tos_id = stack_Top(bo->stack);
					stack_Push(bo->stack, tos_id);
					op_thru = 1;
				}
				break;

			case OPCODE_DUP_TOP_TWO:
				{
					tos_id = stack_Top(bo->stack);
					tos1_id = stack_Second(bo->stack);
					stack_Adjust(bo->stack, 2);
					stack_SetTop(bo->stack, tos_id);
					obj_IncRefCount(tos_id);
					stack_SetSecond(bo->stack, tos1_id);
					obj_IncRefCount(tos1_id);
					op_thru = 1;
				}
				break;

			case OPCODE_GET_ITER:
				{
					OBJECT_ID iter = stack_Pop(bo->stack);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
					 debug_printf(DEBUG_VERBOSE_STEP,"iter:\n");
					 obj_Dump(iter,0,1);
					}
					#endif
					ITER_ID it = iter_CreateIter(iter,vm_id);//,vm);
					stack_Push(bo->stack,it);
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
				obj_ClearGC();
				mem_unlock(string_id,0);
				mem_unlock(bytecodes_id,0);
				mem_unlock(obj_id,1);
				mem_unlock(bo_id,0);
				mem_unlock(vm_id,0);
				return(0);
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
					tos_id = stack_Pop(bo->stack);
					if(obj_GetType(tos_id) == TYPE_KV)
					{
						kv_object *tk = (kv_object*)mem_lock(tos_id);
						OBJECT_ID old = tos_id;
						tos_id = tk->value;
						mem_unlock(old,0);
					}
					tos = (object*)mem_lock(tos_id);
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
					tos_id = stack_Pop(bo->stack);
					if(obj_GetType(tos_id) == TYPE_KV)
					{
						kv_object *tk = (kv_object*)mem_lock(tos_id);
						OBJECT_ID old = tos_id;
						tos_id = tk->value;
						mem_unlock(old,0);
					}
					tos = (object*)mem_lock(tos_id);

					tos1_id = stack_Pop(bo->stack);
					if(obj_GetType(tos1_id) == TYPE_KV)
					{
						kv_object *tk1 = (kv_object*)mem_lock(tos1_id);
						OBJECT_ID old1 = tos1_id;
						tos1_id = tk1->value;
						mem_unlock(old1,0);
					}
					tos1 = (object*)mem_lock(tos1_id);
					}
				break;

			case OPCODE_STORE_SUBSCR:
			case OPCODE_STORE_MAP:
			//case OPCODE_RAISE_VARARGS:
				{
					tos_id = stack_Pop(bo->stack);
					if(obj_GetType(tos_id) == TYPE_KV)
					{
						kv_object *tk = (kv_object*)mem_lock(tos_id);
						OBJECT_ID old = tos_id;
						tos_id = tk->value;
						mem_unlock(old,0);
					}
					tos = (object*)mem_lock(tos_id);
					
					tos1_id = stack_Pop(bo->stack);
					if(obj_GetType(tos1_id) == TYPE_KV)
					{
						kv_object *tk1 = (kv_object*)mem_lock(tos1_id);
						OBJECT_ID old1 = tos1_id;
						tos1_id = tk1->value;
						mem_unlock(old1,0);
					}
					tos1 = (object*)mem_lock(tos1_id);
					
					tos2_id = stack_Pop(bo->stack);
					if(obj_GetType(tos2_id) == TYPE_KV)
					{
						kv_object *tk2 = (kv_object*)mem_lock(tos2_id);
						OBJECT_ID old2 = tos2_id;
						tos2_id = tk2->value;
						mem_unlock(old2,0);
					}
					tos2 = (object*)mem_lock(tos2_id);
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
					tos_id = stack_Pop(bo->stack);
					if(obj_GetType(tos_id) == TYPE_KV)
					{
						kv_object *tk = (kv_object*)mem_lock(tos_id);
						OBJECT_ID old = tos_id;
						tos_id = tk->value;
						mem_unlock(old,0);
					}
					//object *tos = (object*)mem_lock(tos_id);
					if(arg > 1) //parameter
					{
						tos1_id = stack_Pop(bo->stack);
						if(obj_GetType(tos1_id) == TYPE_KV)
						{
							kv_object *tk1 = (kv_object*)mem_lock(tos1_id);
							OBJECT_ID old1 = tos1_id;
							tos1_id = tk1->value;
							mem_unlock(old1,0);
						}
						//object *tos1 = (object*)mem_lock(tos1_id);
					}
					if(arg > 2) //traceback
					{
						tos2_id = stack_Pop(bo->stack);
						if(obj_GetType(tos2_id) == TYPE_KV)
						{
							kv_object *tk2 = (kv_object*)mem_lock(tos2_id);
							OBJECT_ID old2 = tos2_id;
							tos2_id = tk2->value;
							mem_unlock(old2,0);
						}
						//object *tos2 = (object*)mem_lock(tos2_id);
					}
					if(vm->exception_handler != NULL)
						vm->exception_handler(vm_id,tos_id);
					OBJECT_ID o = obj_CreateEmpty(TYPE_NONE);
					obj_IncRefCount(o);
					mem_unlock(string_id,0);
					mem_unlock(bytecodes_id,0);
					mem_unlock(obj_id,0);
					mem_unlock(bo_id,1);
					mem_unlock(vm_id,0);
					return(obj_CreateEmpty(TYPE_NONE));
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
					CFUNCTION_ID cfo = obj_CreateCFunction(&if_build_class);//TODO switch to get global
					stack_Push(bo->stack,cfo);
				}
				break;

			case OPCODE_FOR_ITER:
				{
					OBJECT_ID next =  0;
					if(obj_GetType(stack_Top(bo->stack)) == TYPE_ITER)
						next = iter_NextNow(stack_Top(bo->stack),vm_id);
					if(obj_GetType(next) == TYPE_NONE)
					{
						stack_Pop(bo->stack);
						#ifdef USE_DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"for iter thru \n");
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
							obj_Dump(next,1,1);
						#endif
						bo->ip = bo->ip + arg;
					}
					else
					{
						#ifdef USE_DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"for iter next \n");
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
							obj_Dump(next,1,1);
						#endif
						stack_Push(bo->stack, next);
					}
				}
				break;
				
			case OPCODE_LOAD_ATTR:
				{
					OBJECT_ID name = tuple_GetItem(co->names,arg);
					if(obj_GetType(name) == TYPE_KV)
					{
						kv_object *kname = (kv_object*)mem_lock(name);
						OBJECT_ID old = name;
						name = kname->value;
						mem_unlock(old,0);
					}
					//stack_Push(bo->stack, name);
					//printf("importing from: %s\n",((unicode_object*)name)->value);
					#ifdef USE_DEBUGGING
					//if(obj_GetType(name) == TYPE_UNICODE)
					//	debug_printf(DEBUG_VERBOSE_STEP,"loading attribute: %s\n",((unicode_object*)name)->value);
					//if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					//	DumpObject(name,1);
					#endif
					//obj_Dump(tos_id,0,0);
					//obj_Dump(name,0,0);
					OBJECT_ID attr = obj_GetAttribute(tos_id,name);
					if(attr != 0)
						stack_Push(bo->stack,attr);
					#ifdef USE_DEBUGGING
					else
						debug_printf(DEBUG_ALL,"loading attribute failed\n");
					#endif
					//obj_Dump(attr,0,0);
				}
				break;
				
			case OPCODE_STORE_ATTR:
				{
					//"Implements TOS.name = TOS1, where /namei/ is the index of name in co_names."
					OBJECT_ID name = tuple_GetItem(co->names,arg);
					if(obj_GetType(name) == TYPE_KV)
					{
						kv_object *kn = (kv_object*)mem_lock(name);
						OBJECT_ID old = name;
						name = kn->value;
						mem_unlock(old,0);
					}
					#ifdef USE_DEBUGGING
					//if(obj_GetType(name) == TYPE_UNICODE)
					//	debug_printf(DEBUG_VERBOSE_STEP,"storing attribute: %s\n",((unicode_object*)name)->value);
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(tos1_id,1,1);
					#endif
					obj_SetAttribute(tos_id,name,tos1_id);
				}	
				break;

				case OPCODE_IMPORT_NAME:
				{
					OBJECT_ID name = tuple_GetItem(co->names,arg);
					if(obj_GetType(name) == TYPE_KV)
					{
						kv_object *kn = (kv_object*)mem_lock(name);
						OBJECT_ID old = name;
						name = kn->value;
						mem_unlock(old,0);
					}
					#ifdef USE_DEBUGGING
					//if(name->type == TYPE_UNICODE)
					//	debug_printf(DEBUG_VERBOSE_STEP,"importing: %s\n",((unicode_object*)name)->value);
					//if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					//	DumpObject(name,1);
					#endif
					OBJECT_ID mc = vm_GetGlobal(vm_id,name);
					if(mc != 0)
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
						unicode_object *nn = (unicode_object*)mem_lock(name);
						OBJECT_ID module = vm->import_module_handler(vm_id,nn->value);
						mem_unlock(name,0);
						if(module != 0) // found the module push it on stack
						{
							stack_Push(bo->stack,module);
							vm_AddGlobal(vm_id, name,module);
							vm_StartCodeObject(vm_id,module,0,0);
							obj_DecRefCount(module);
						}
						else
						{
							//#ifdef USE_DEBUGGING
							//debug_printf(DEBUG_ALL,"failed to import: %s\n",((unicode_object*)name)->value);
							//#endif
						}
					}
					
				}
				break;
				
			case OPCODE_IMPORT_FROM:
				{
					OBJECT_ID name = tuple_GetItem(co->names,arg);
					if(obj_GetType(name) == TYPE_KV)
					{
						kv_object *kn = (kv_object*)mem_lock(name);
						OBJECT_ID old = name;
						name = kn->value;
						mem_unlock(old,0);
					}
					#ifdef USE_DEBUGGING
					//if(name->type == TYPE_UNICODE)
					//	debug_printf(DEBUG_VERBOSE_STEP,"importing from: %s\n",((unicode_object*)name)->value);
					//if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					//	DumpObject(name,1);
					#endif
					OBJECT_ID lgo = 0;
					if(tos->type == TYPE_CODE)
					{
						unicode_object *nn = (unicode_object*)mem_lock(name);
						INDEX index = tuple_GetItemIndexByName(((code_object*)tos)->names,nn->value);
						mem_unlock(name,0);
						if(index != -1)
						{
							lgo = tuple_GetItem(((code_object*)tos)->names,index);
						}
						if(lgo != 0) //push the imported function on stack
							stack_Push(bo->stack, lgo);
						else
						{
							#ifdef USE_DEBUGGING
							debug_printf(DEBUG_ALL,"warning import function was not found\n");
							#endif
						}
					}else if(tos->type == TYPE_CLASS)
						stack_Push(bo->stack, tos_id);
					//DumpObject(lgo,0);
				}
				break;
				
			case OPCODE_IMPORT_STAR:
				{
					code_object *co = (code_object*)tos;
					vm_AddGlobal(vm_id,obj_CreateUnicode(mem_copy(co->name)), tos_id);
				}
				break;
				
			case OPCODE_MAKE_FUNCTION:
				{
					NUM narg = (NUM) (arg & 255);
					NUM nkey = (NUM) ((arg >> 8) & 255);
					//NUM n = (NUM) (narg + (nkey*2));
					TUPLE_ID defaults = 0;
					TUPLE_ID kw_defaults = 0;
					
					if(narg > 0)
					{
						defaults = obj_CreateTuple(narg); //creating defaults tuple
						for (NUM i = 0; i < narg; i++)
						{
							OBJECT_ID t = stack_Pop(bo->stack);
							//obj_Dump(t,1,1);
							if(obj_GetType(t) == TYPE_KV)
							{
								kv_object *kt = (kv_object*)mem_lock(t);
								OBJECT_ID old = t;
								t = kt->value;
								mem_unlock(old,0);
							}
							tuple_SetItem(defaults,i,t);
						}
					}
					if(nkey >0)
					{
						kw_defaults = obj_CreateTuple(0); //creating kw defaults tuple
						for (NUM i = 0; i < nkey; i++)//keyword arguments
						{
							OBJECT_ID value = stack_Pop(bo->stack);
							OBJECT_ID key = stack_Pop(bo->stack);
							tuple_SetDictItem(kw_defaults,key,value);
						}
					}
					FUNCTION_ID fo = obj_CreateFunction(tos_id,defaults,kw_defaults,0);
					stack_Push(bo->stack,fo);
					obj_IncRefCount(defaults);
					obj_IncRefCount(kw_defaults);
					obj_DecRefCount(defaults);
					obj_DecRefCount(kw_defaults);
				}
				break;
				
			case OPCODE_MAKE_CLOSURE:
				{
					NUM narg = (NUM) (arg & 255);
					NUM nkey = (NUM) ((arg >> 8) & 255);
					//NUM n = (NUM) (narg + (nkey*2));
					TUPLE_ID defaults = 0;
					TUPLE_ID kw_defaults = 0;
					tos1_id = stack_Pop(bo->stack);
					//tos1 = DissolveRef(tos1);
					if(narg > 0)
					{
						defaults = obj_CreateTuple(narg); //creating defaults tuple
						for (NUM i = 0; i < narg; i++)
						{
							OBJECT_ID t = stack_Pop(bo->stack);
							if(obj_GetType(t) == TYPE_KV)
							{
								kv_object *kt = (kv_object*)mem_lock(t);
								OBJECT_ID old = t;
								t = kt->value;
								mem_unlock(old,0);
							}
							tuple_SetItem(defaults,i,t);
						}
					}
					if(nkey >0)
					{
						kw_defaults = obj_CreateTuple(0); //creating kw defaults tuple
						for (NUM i = 0; i < nkey; i++)//keyword arguments
						{
							OBJECT_ID value = stack_Pop(bo->stack);
							OBJECT_ID key = stack_Pop(bo->stack);
							tuple_SetDictItem(kw_defaults,key,value);
						}
					}
					FUNCTION_ID fo = obj_CreateFunction(tos_id,defaults,kw_defaults,tos1_id);
					stack_Push(bo->stack,fo);
					obj_IncRefCount(defaults);
					obj_IncRefCount(kw_defaults);
					obj_DecRefCount(defaults);
					obj_DecRefCount(kw_defaults);
				}
				break;
				
			case OPCODE_POP_JUMP_IF_FALSE:
				{
					if(tos->type == TYPE_FALSE)
					{
						bo->ip = arg;
					}
				}
				break;
				
			case OPCODE_POP_JUMP_IF_TRUE:
				{
					if(tos->type == TYPE_TRUE)
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
					if(tos->type == TYPE_FALSE)
					{
						stack_Push(bo->stack,tos_id);
						bo->ip = arg;
					}
				}
				break;
				
			case OPCODE_JUMP_IF_TRUE:
				{
					if(tos->type == TYPE_TRUE)
					{
						bo->ip = arg;
						stack_Push(bo->stack,tos_id);
					}
				}
				break;

			case OPCODE_WITH_CLEANUP:
				{
					stack_Clear(bo->stack,1);
					obj_ClearGC();
				}
				break;
			
			case OPCODE_SETUP_WITH:
				{
					//printf("setup with:%d,%d\n",bo->ip,arg);
					//DumpObject(tos,0);
					BLOCK_ID block_id = obj_Alloc(TYPE_BLOCK,0);
					block_object *block = (block_object*)mem_lock(block_id);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"setup with Creating new execution block: %x\n",block);
					#endif

					block->start = bo->ip;
					block->code = obj_id;
					obj_IncRefCount(obj_id);
					block->ip = bo->ip;
					block->len = bo->ip + arg;
					block->stack = stack_Create();
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(block_id,0,1);
					#endif
					stack_Push(vm->blocks, block_id);
					bo->ip = block->len;
					//stack_Push(bo->stack,tos);
					mem_unlock(block_id,1);
					stack_Push(block->stack,tos_id);
				}
				break;
			
			case OPCODE_SETUP_LOOP:
				{
					BLOCK_ID block_id = obj_Alloc(TYPE_BLOCK,0);
					block_object *block = (block_object*)mem_lock(block_id);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"setup loop Creating new execution block: %x\n",block);
					#endif
					block->start = bo->ip;
					block->code = obj_id;
					obj_IncRefCount(obj_id);
					block->ip = bo->ip;
					block->len = bo->ip + arg;
					block->stack = stack_Create();
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(block_id,0,1);
					#endif
					bo->ip = block->len;
					mem_unlock(block_id,1);
					stack_Push(vm->blocks, block_id);
				}
				break;
				
			case OPCODE_BUILD_SET:
				{
					TUPLE_ID to = obj_CreateTuple(0);
					stack_Push(bo->stack,to);
				}
				break;
				
			case OPCODE_BUILD_MAP:
				{
					TUPLE_ID to = obj_CreateTuple(0);
					stack_Push(bo->stack,to);
				}
				break;
				
			case OPCODE_BUILD_TUPLE:
			case OPCODE_BUILD_LIST:
				{
					TUPLE_ID blcall = obj_CreateTuple(0);	// arg
					for (NUM i = 0; i < arg; i++)
					{
						//object *t = stack_Pop(bo->stack);
						OBJECT_ID t = stack_Get(bo->stack,stack_Pointer(bo->stack) - (arg-1-i));
						if(obj_GetType(t) == TYPE_ITER)
							iter_ExpandTuple(t,vm_id,blcall);
						else
							tuple_AppendItem(blcall, t);
					}
					for (NUM i = 0; i < arg; i++)
					{
						stack_Pop(bo->stack);
					}
					stack_Push(bo->stack,blcall);
					#ifdef USE_DEBUGGING
					if(debug_level & DEBUG_VERBOSE_STEP)
						obj_Dump(blcall,0,1);
					#endif
				}
				break;
				
			case OPCODE_LOAD_GLOBAL:
				{
					TUPLE_ID co_names = co->names;
					OBJECT_ID name = 0;
					if(obj_GetType(tuple_GetItem(co_names,arg)) == TYPE_KV)
					{
						OBJECT_ID it = tuple_GetItem(co_names,arg);
						kv_object *kit = (kv_object*)mem_lock(it);
						name = kit->key;
						mem_unlock(it,0);
					}
					else
						name = tuple_GetItem(co_names,arg);
					OBJECT_ID lgo = 0;
					lgo = vm_GetGlobal(vm_id,name);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"searching for global: ");
					if(debug_level & DEBUG_VERBOSE_STEP)
						obj_Print(name);
					debug_printf(DEBUG_VERBOSE_STEP,"\n");
					#endif
					if(lgo == 0)
					{
						for(INDEX i=0;i<tuple_GetLen(vm->globals);i++)
						{	
							CODE_ID lco_id = tuple_GetDictItemByIndex(vm->globals,i);
							if(obj_GetType(lco_id) == TYPE_CODE)
							{
								code_object *lco = (code_object*)mem_lock(lco_id);
								unicode_object *nn = (unicode_object*)mem_lock(name);
								INDEX index = tuple_GetItemIndexByName(lco->names,nn->value);
								if(index != -1)
								{
									lgo = tuple_GetItem(lco->names,index);
								}
								if(lgo != 0)
								{
									mem_unlock(name,0);
									mem_unlock(lco_id,0);
									break;
								}
								index = tuple_GetItemIndexByName(lco->varnames,nn->value);
								if(index != -1)
								{
									lgo = tuple_GetItem(lco->varnames,index);
								}
								mem_unlock(name,0);
								mem_unlock(lco_id,0);
								if(lgo != 0)
									break;
							}
						}
					}
					if(lgo == 0)
					{
						printf("global:");
						obj_Print(name);
						printf(" not found!\n");
					}
					stack_Push(bo->stack, lgo);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(lgo,1,1);
					#endif
				}
				break;

			case OPCODE_STORE_GLOBAL:
				{
					TUPLE_ID co_names = co->names;
					OBJECT_ID name = 0;
					if(obj_GetType(tuple_GetItem(co_names,arg)) == TYPE_KV)
					{
						OBJECT_ID it = tuple_GetItem(co_names,arg);
						kv_object *kit = (kv_object*)mem_lock(it);
						name = kit->key;
						mem_unlock(it,0);
					}
					else
						name = tuple_GetItem(co_names,arg);
					for(INDEX i=0;i<tuple_GetLen(vm->globals);i++)
					{	
						CODE_ID sco_id = tuple_GetDictItemByIndex(vm->globals,i);
						if(obj_GetType(sco_id) == TYPE_CODE)
						{
							code_object *sco = (code_object*)mem_lock(sco_id);
							unicode_object *nn = (unicode_object*)mem_lock(name);
							INDEX index = tuple_GetItemIndexByName(sco->names,nn->value);
							if(index != -1)
							{
								tuple_SetDictItemByIndex(sco->names,index,tos_id);
								mem_unlock(name,0);
								mem_unlock(sco_id,0);
								break;		
							}
							index = tuple_GetItemIndexByName(sco->varnames,nn->value);
							if(index != -1)
							{
								tuple_SetDictItemByIndex(sco->varnames,index,tos_id);
								mem_unlock(name,0);
								mem_unlock(sco_id,0);
								break;
							}
							mem_unlock(name,0);
							mem_unlock(sco_id,0);
						}
					}
				}
				break;

			case OPCODE_DELETE_GLOBAL:
				{
					//TODO needs to be TYPE_KV aware and has to remove unneeded objects
					TUPLE_ID co_names = co->names;
					OBJECT_ID name = 0;
					if(obj_GetType(tuple_GetItem(co_names,arg)) == TYPE_KV)
					{
						OBJECT_ID it = tuple_GetItem(co_names,arg);
						kv_object *kit = (kv_object*)mem_lock(it);
						name = kit->key;
						mem_unlock(it,0);
					}
					else
						name = tuple_GetItem(co_names,arg);
					printf("got global name\n");
					for(INDEX i=0;i<tuple_GetLen(vm->globals);i++)
					{	
						printf("checking global code module %d of %d\n",i,tuple_GetLen(vm->globals));
						OBJECT_ID dco_id = tuple_GetDictItemByIndex(vm->globals,i);
						if(obj_GetType(dco_id) == TYPE_CODE)
						{
							code_object *dco = (code_object*)mem_lock(dco_id);
							unicode_object *nn = (unicode_object*)mem_lock(name);
							INDEX index = tuple_GetItemIndexByName(dco->names,nn->value);
							if(index != -1)
							{
								printf("setting name:%d\n",index);
								tuple_SetDictItemByIndex(dco->names,index,0);
								mem_unlock(name,0);
								mem_unlock(dco_id,0);
								break;
							}
							index = tuple_GetItemIndexByName(dco->varnames,nn->value);
							if(index != -1)
							{
								printf("setting varname:%d\n",index);
								tuple_SetDictItemByIndex(dco->varnames,index,0);
							}
							mem_unlock(name,0);
							mem_unlock(dco_id,0);
						}
					}
				}
				break;

			case OPCODE_LOAD_NAME:
				{
					OBJECT_ID name_id = tuple_GetItem(co->names,arg);
					if(obj_GetType(name_id) == TYPE_KV)
					{
						//TODO add obj_GetValue(KV_ID obj_id)
						//TODO add obj_GetKey(KV_ID obj_id)
						kv_object *name = (kv_object*)mem_lock(name_id);
						OBJECT_ID old = name_id;
						name_id = name->value;
						mem_unlock(old,0);
					}
					stack_Push(bo->stack, name_id);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(name_id,1,1);
					#endif
				}
				break;

			case OPCODE_LOAD_CONST:
				{
					//obj_Dump(obj,1,1);
					OBJECT_ID lconst = tuple_GetItem(co->consts,arg);
					stack_Push(bo->stack, lconst);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(lconst,1,1);
					#endif
				}
				break;

			case OPCODE_LOAD_FAST:
				{
					//obj_Dump(co->varnames,0,0);
					OBJECT_ID fast_id = tuple_GetItem(co->varnames,arg);
					if(obj_GetType(fast_id) == TYPE_KV)
					{
						kv_object *fast = (kv_object*)mem_lock(fast_id);
						OBJECT_ID old = fast_id;
						fast_id = fast->value;
						mem_unlock(old,0);
					}
					stack_Push(bo->stack, fast_id);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(fast_id,1,1);
					#endif
				}
				break;
	
			case OPCODE_STORE_DEREF:
				{
					if(arg < tuple_GetLen(co->cellvars))
					{
						if(obj_GetType(tuple_GetItem(co->cellvars,arg)) != TYPE_KV)
						{
							tuple_SetItem(co->cellvars,arg,obj_ConvertToValuedKV(tuple_GetItem(co->cellvars,arg),tos_id));
						}
						else
						{
							tuple_SetDictItemByIndex(co->cellvars,arg,tos_id);
						}
					}
					else
					{
						if(obj_GetType(tuple_GetItem(co->freevars,arg - tuple_GetLen(co->cellvars))) != TYPE_KV)
						{
							tuple_SetItem(co->freevars,arg - tuple_GetLen(co->cellvars),obj_ConvertToValuedKV(tuple_GetItem(co->freevars,arg - tuple_GetLen(co->cellvars)),tos_id));
						}
						else
						{
							tuple_SetDictItemByIndex(co->freevars,arg - tuple_GetLen(co->cellvars),tos_id);
						}
					}
				
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(tos_id,1,1);
					#endif
				}
				break;

			case OPCODE_LOAD_DEREF:
				{
					OBJECT_ID cell_id =  0;
					if(arg < tuple_GetLen(co->cellvars))
						cell_id = tuple_GetItem(co->cellvars,arg);
					else
						cell_id = tuple_GetItem(co->freevars,arg - tuple_GetLen(co->cellvars));
						if(obj_GetType(cell_id) == TYPE_KV)
						{
							kv_object *cell = (kv_object*)mem_lock(cell_id);
							OBJECT_ID old = cell_id;
							cell_id = cell->value;
							mem_unlock(old,0);
						}
					stack_Push(bo->stack, cell_id);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(cell_id,1,1);
					#endif
				}
				break;

			case OPCODE_LOAD_CLOSURE:
				{
					OBJECT_ID fast = 0;
					if(arg < tuple_GetLen(co->cellvars))
						fast = tuple_GetItem(co->cellvars,arg);
					else
						fast = tuple_GetItem(co->freevars,arg - tuple_GetLen(co->cellvars));
					stack_Push(bo->stack, fast);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(fast,1,1);
					#endif
				}
				break;

			case OPCODE_STORE_NAME:
				{
					if(obj_GetType(tuple_GetItem(co->names,arg)) != TYPE_KV)
					{
						tuple_SetItem(co->names,arg,obj_ConvertToValuedKV(tuple_GetItem(co->names,arg),tos_id));
					}
					else
					{
						tuple_SetDictItemByIndex(co->names,arg,tos_id);
					}
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(tos_id,1,1);
					#endif
					//printf("stored name\n");
				}
				break;

			case OPCODE_STORE_MAP:
				{
					tuple_SetDictItem(tos2_id,tos_id,tos1_id);
					stack_Push(bo->stack,tos2_id);
				}
				break;

			case OPCODE_MAP_ADD:
				{
					tos2_id = stack_Get(bo->stack,-arg);
					if(obj_GetType(tos2_id) == TYPE_KV)
					{
						kv_object *kt = (kv_object*)mem_lock(tos2_id);
						OBJECT_ID old = tos2_id;
						tos2_id = kt->key;
						mem_unlock(old,0);
					}
					tuple_SetDictItem(tos2_id,tos_id,tos1_id);
				}
				break;

			case OPCODE_LIST_APPEND:
				{
					tos1_id = stack_Get(bo->stack,-arg);
					if(obj_GetType(tos1_id) == TYPE_KV)
					{
						kv_object *kt = (kv_object*)mem_lock(tos1_id);
						OBJECT_ID old = tos1_id;
						tos1_id = kt->key;
						mem_unlock(old,0);
					}
					tuple_AppendItem(tos1_id,tos_id);
				}
				break;

			case OPCODE_SET_ADD:
				{
					tos1_id = stack_Get(bo->stack,-arg);
					if(obj_GetType(tos1_id) == TYPE_KV)
					{
						kv_object *kt = (kv_object*)mem_lock(tos1_id);
						OBJECT_ID old = tos1_id;
						tos1_id = kt->key;
						mem_unlock(old,0);
					}
					tuple_AppendItem(tos1_id,tos_id);
				}
				break;
				
			case OPCODE_STORE_FAST:
				{
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						obj_Dump(tos_id,1,1);
					#endif
					if(obj_GetType(tuple_GetItem(co->varnames,arg)) != TYPE_KV)
					{
						#ifdef USE_DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"converting\n");
						#endif
						tuple_SetItem(co->varnames,arg,obj_ConvertToValuedKV(tuple_GetItem(co->varnames,arg),tos_id));
					}
				else
				{
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"updating\n");
					#endif
					tuple_SetDictItemByIndex(co->varnames,arg,tos_id);
				}
				#ifdef USE_DEBUGGING
				if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					obj_Dump(tos_id,1,1);
				#endif
				}
				break;

			case OPCODE_DELETE_NAME:
				{
					tuple_DeleteItem(co->names, arg);
				}
				break;

			case OPCODE_DELETE_FAST:
				{
					tuple_DeleteItem(co->varnames, arg);
				}
				break;

			case OPCODE_DELETE_SUBSCR:
				{
					NUM dsa = 0;
					if(tos->type == TYPE_INT)
					{
						dsa = (NUM) ((int_object*)tos)->value;
					}
					if(tos1->type == TYPE_TUPLE)
					{
						tuple_DeleteItem(tos1_id, dsa);
					}
				}
				break;

			case OPCODE_STORE_SUBSCR:
				{
					NUM ssa = 0;
					 
					if(tos->type == TYPE_INT)
					{
						ssa = (NUM) ((int_object*)tos)->value;
						#ifdef USE_DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"ssa: %d\n",ssa);
						#endif
					}
					if(tos1->type == TYPE_TUPLE)
					{
					if(tos->type == TYPE_INT)
						tuple_SetItem(tos1_id, ssa, tos2_id);
					else
						tuple_SetDictItem(tos1_id,tos_id,tos2_id);
					#ifdef USE_DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					 {
						debug_printf(DEBUG_VERBOSE_STEP,"tos:%c\n",tos->type);
						obj_Dump(tos_id,0,1);
						debug_printf(DEBUG_VERBOSE_STEP,"tos1:%c\n",tos1->type);
						obj_Dump(tos1_id,0,1);
						debug_printf(DEBUG_VERBOSE_STEP,"tos2:%c\n",tos2->type);
						obj_Dump(tos2_id,0,1);
					}
					#endif
					}
				}
				break;

			case OPCODE_BINARY_SUBSCR:
				{
					NUM bsa = 0;
					if(tos->type == TYPE_INT)
					{
						bsa = (NUM) ((int_object*)tos)->value;		
					}
				
					if(tos1->type == TYPE_TUPLE)
					{
						#ifdef USE_DEBUGGING
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						{
							obj_Dump(tos1_id, 0,1);
							obj_Dump(tos_id,0,1);
							debug_printf(DEBUG_VERBOSE_STEP,"bsa:%d\n", bsa);
						}
						#endif
						OBJECT_ID bst= 0;
						if(tos->type == TYPE_INT)
							bst = tuple_GetItem(tos1_id, bsa);
						else
							bst = tuple_GetDictItem(tos1_id,tos_id);
						#ifdef USE_DEBUGGING
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
							obj_Dump(bst, 0,1);
						#endif
						stack_Push(bo->stack, bst);
					}
				}
				break;

			case OPCODE_UNPACK_SEQUENCE: //TODO does not support tuples/iters for unpacking
				{
					for (NUM i = arg-1; i >= 0; i--)
					{
						switch(tos->type)
						{
						case TYPE_UNICODE:
							{
								BYTES_ID seq_id = ((unicode_object*)tos)->value;
								char *seq = (char*)mem_lock(seq_id);
								UNICODE_ID uso = obj_CreateUnicode(str_FromChar(seq[i]));//TODO check for leak
								mem_unlock(seq_id,0);
								#ifdef USE_DEBUGGING
								if((debug_level & DEBUG_VERBOSE_STEP) > 0)
									obj_Dump(uso,0,1);
								#endif
								stack_Push(bo->stack, uso);
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
					if(tos->type == TYPE_INT)
					{
						INT_ID up = obj_CreateInt(+((int_object*)tos)->value);
						stack_Push(bo->stack, up);
					}
				}
				break;

			case OPCODE_UNARY_NEGATIVE:
				{
					if(tos->type == TYPE_INT)
					{
						INT_ID up = obj_CreateInt(-((int_object*)tos)->value);
						stack_Push(bo->stack, up);
					}
				}
				break;

			case OPCODE_UNARY_NOT:
				{
					if(tos->type == TYPE_INT)
					{
						OBJECT_ID up = obj_CreateEmpty(((int_object*)tos)->value == 0 ? TYPE_TRUE : TYPE_FALSE);
						stack_Push(bo->stack, up);
					}
					else if(tos->type == TYPE_TRUE || tos->type == TYPE_FALSE)
					{
						OBJECT_ID up = obj_CreateEmpty(tos->type == TYPE_TRUE ? TYPE_FALSE : TYPE_TRUE);
						stack_Push(bo->stack, up);
					}
					else if(tos->type == TYPE_NONE || tos->type == TYPE_NULL)
					{
						OBJECT_ID up = obj_CreateEmpty(TYPE_TRUE);//TYPE_FALSE
						stack_Push(bo->stack, up);
					}
				}
				break;

			case OPCODE_UNARY_INVERT:
				{
					if(tos->type == TYPE_INT)
					{
						INT_ID up = obj_CreateInt(-((((int_object*)tos)->value) + 1));
						stack_Push(bo->stack, up);
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
						OBJECT_ID new_tos = BinaryOp(tos_id,tos1_id,op);
						stack_Push(bo->stack, new_tos);
				}
				break;

			case OPCODE_COMPARE_OP:
				{
					OBJECT_ID new_tos = CompareOp(tos_id,tos1_id,arg);
					stack_Push(bo->stack, new_tos);
				}
				break;
				   
			case OPCODE_CALL_FUNCTION_VAR_KW:
			case OPCODE_CALL_FUNCTION_KW:
				{
						kw_list = stack_Pop(bo->stack);
				}
			case OPCODE_CALL_FUNCTION_VAR:
				{
					if(op == OPCODE_CALL_FUNCTION_VAR_KW || op == OPCODE_CALL_FUNCTION_VAR)
						var_list = stack_Pop(bo->stack);
				}
			case OPCODE_CALL_FUNCTION:
				{
					TUPLE_ID call = 0;
					TUPLE_ID kw_call = 0;
					NUM narg = (NUM) (arg & 255);
					NUM nkey = (NUM) ((arg >> 8) & 255);
					//NUM n = (NUM) (narg + (nkey*2));
					//fetch function object from tos and Resolve to function object
					//object *function = stack_Get(bo->stack,stack_Pointer(bo->stack) - n);
					//stack_Dump(bo->stack);			
					if(narg > 0 || var_list != 0)
					{
						call = obj_CreateTuple(0);	// arg, 
						obj_IncRefCount(call);
					}
					if(nkey > 0 || kw_list != 0)
					{					
						kw_call = obj_CreateTuple(0);
						obj_IncRefCount(kw_call);
					}
					if(kw_list != 0) //variable keyword arguments
					{
						for(NUM i = 0;i< tuple_GetLen(kw_list); i++)
						{
							OBJECT_ID v = tuple_GetItem(kw_list,i);
							tuple_AppendItem(kw_call,v);
						}	
					}
					if(var_list != 0) //variable list arguments
					{
						for (NUM i = 0;i< tuple_GetLen(var_list); i++)
						{
							OBJECT_ID v = tuple_GetItem(var_list,i);
							tuple_AppendItem(call,v);
						}	
					}
					for(NUM i = 0; i < nkey; i++)//keyword arguments
					{
						OBJECT_ID value = stack_Pop(bo->stack);
						OBJECT_ID key = stack_Pop(bo->stack);
						KV_ID kv = obj_CreateKV(key,value);
						tuple_AppendItem(kw_call,kv);
					}
					for(NUM i = 0; i < narg; i++)//retrieve positional arguments
					{
						OBJECT_ID p = stack_Get(bo->stack,stack_Pointer(bo->stack) - (narg-1-i));
						tuple_AppendItem(call,p);
					}
					for(NUM i = 0; i < narg; i++)//pop positional arguments
					{
						stack_Pop(bo->stack);
					}

					OBJECT_ID rf = stack_Pop(bo->stack);//finally pop the function 
					if(obj_GetType(rf) == TYPE_KV)
					{
						kv_object *krf = (kv_object*)mem_lock(rf);
						OBJECT_ID old = rf;
						rf = krf->value;
						mem_unlock(old,0);
					}
					//obj_Dump(rf,1,1);
					OBJECT_ID ret = vm_StartObjectCopy(vm_id,rf,call,kw_call);
					if(ret != 0)
					{
						stack_Push(bo->stack, ret);
					}
					obj_DecRefCount(call);
					obj_DecRefCount(kw_call);
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

		#ifdef USE_DEBUGGING
			debug_printf(DEBUG_VERBOSE_STEP,"unlocking tos items\n");
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
					mem_unlock(tos_id,0);
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
					mem_unlock(tos1_id,0);
					mem_unlock(tos_id,0);
				}
				break;

			case OPCODE_STORE_SUBSCR:
			case OPCODE_STORE_MAP:
			//case OPCODE_RAISE_VARARGS:
				{
					mem_unlock(tos2_id,0);
					mem_unlock(tos1_id,0);
					mem_unlock(tos_id,0);
				}
				break;
			}


			//step thru -> clear gc and call handler if present
			obj_ClearGC();
			if(vm->step_handler != NULL)
			{
				vm->step_handler(vm_id);
			}
			mem_unlock(string_id,0);
			mem_unlock(bytecodes_id,0);
			mem_unlock(obj_id,0);
			mem_unlock(bo_id,1);
			mem_unlock(vm_id,0);
			return(0);
		} //END OF STEP
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"code thru.\n");
		#endif
		OBJECT_ID ret = 0;
		if(stack_IsEmpty(bo->stack))
		{
			stack_Pop(vm->blocks);
			obj_ClearGC();
			mem_unlock(string_id,0);
			mem_unlock(bytecodes_id,0);
			mem_unlock(obj_id,0);
			mem_unlock(bo_id,1);
			mem_unlock(vm_id,0);
			return(0);
		}
		else
		{
			stack_Pop(vm->blocks);
			ret = stack_Pop(bo->stack);
			mem_unlock(string_id,0);
			mem_unlock(bytecodes_id,0);
			mem_unlock(obj_id,0);
			mem_unlock(bo_id,1);
			mem_unlock(vm_id,0);
			obj_IncRefCount(ret);
			obj_ClearGC();
			return(ret);
		}
	}
	mem_unlock(obj_id,0);
	mem_unlock(bo_id,0);
	mem_unlock(vm_id,0);
	return(0);
}

#ifdef CREATELIB

int main(int argc, char *argv[])
{

}

#endif

