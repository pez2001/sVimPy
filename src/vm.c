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

#ifdef DEBUGGING

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


function_object *CreateCFunction(object *(*func) (vm *vm,stack *stack), char *name)
{
	function_object *fo = CreateFunctionObject(FUNC_C,0);
	fo->func.func = func;
	fo->name = str_Copy(name);
	return (fo);
}

function_object *CreateCObjFunction(object *(*func) (vm *vm,object *obj),	char *name)
{
	function_object *fo = CreateFunctionObject(FUNC_C_OBJ,0);
	fo->func.func_obj = (*func);
	fo->name = str_Copy(name);
	return (fo);
}

function_object *CreatePythonFunction(code_object *code)
{
	function_object *fo = CreateFunctionObject(FUNC_PYTHON,0);
	fo->func.code = code;
	fo->name = code->name;
	return (fo);
}

int vm_AddFunctionObject(vm *vm, function_object *fo)
{
	ptr_Push(vm->functions, fo);
	IncRefCount(fo);
	return (vm->functions->num - 1);
}

void vm_RemoveFunction(vm *vm, char *name)
{
	for (NUM i = 0; i < vm->functions->num; i++)
	{
		if (!strcmp(((function_object*) vm->functions->items[i])->name, name))
		{
			FreeObject(vm->functions->items[i]);
			ptr_Remove(vm->functions, i);
			return;
		}
	}
}

void vm_RemoveFunctionObject(vm *vm, function_object *fo)
{
	for (NUM i = 0; i < vm->functions->num; i++)
	{
		if (vm->functions->items[i] == fo)
		{
			FreeObject(fo);
			ptr_Remove(vm->functions, i);
			return;
		}
	}
}

object *vm_ExecuteCFunction(vm *vm, char *name, stack *stack)
{
	function_object *fo = vm_FindFunction(vm, name);

	if (fo != NULL && fo->func_type == FUNC_C)
	{
		// printf("fd found:%s\n",fd->name);
		
		return ((*fo->func.func) (vm,stack));
	}
	return(NULL);
}

object *vm_ExecuteCObjFunction(vm *vm, char *name, object *obj)
{
	function_object *fo = vm_FindFunction(vm, name);

	if (fo != NULL && fo->func_type == FUNC_C_OBJ)
	{
		return ((*fo->func.func_obj) (vm,obj));
	}
	return(NULL);
}

function_object *vm_FindFunction(vm *vm, char *name)
{
	for (NUM i = 0; i < vm->functions->num; i++)
	{
		if (!strcmp(((function_object*) vm->functions->items[i])->name, name))
			return ((function_object*) (vm->functions->items[i]));
	}
	return (NULL);
}

vm *vm_Init(code_object *co)
{
	#ifdef DEBUGGING
	vm *tmp = (vm *) mem_malloc(sizeof(vm), "vm_Init() return");
	#else
	vm *tmp = (vm *)malloc(sizeof(vm));
	#endif

	tmp->functions = ptr_CreateList(0, 0);
	tmp->blocks = stack_Init();
	tmp->garbage = ptr_CreateList(0,0);
	tmp->running = 0;
	tmp->interrupt_vm = 0;
	tmp->interrupt_handler = NULL;
	
	if (co != NULL)
	{
		tmp->global = co;
	}
	return (tmp);
}

void vm_Close(vm *vm)
{
	if (vm->functions->num)
	{
		for (NUM i = 0; i < vm->functions->num; i++)
			FreeObject(vm->functions->items[i]);
	}
	ptr_CloseList(vm->functions);
	gc_Clear(vm->garbage);
	ptr_CloseList(vm->garbage);
	for(NUM i = 0;i<vm->blocks->list->num;i++)
			FreeObject(vm->blocks->list->items[i]);
	stack_Close(vm->blocks, 0);
	#ifdef DEBUGGING
	assert(mem_free(vm));
	#else
	free(vm);
	#endif
}

void vm_SetGlobal(vm *vm, code_object *co)
{
	vm->global = co;
}

void vm_Interrupt(vm *vm,object *(*interrupt) (struct _vm *vm,stack *stack))
{
	vm->interrupt_handler = interrupt;
	vm->interrupt_vm = 1;
	#ifdef DEBUGGING
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
	vm_Close(vm);
}

void vm_Stop(vm *vm)
{
	vm->interrupt_vm = 0;
	vm->running = 0;
}

block_object *vm_StartObject(vm *vm,object *obj,stack *locals,NUM argc)
{
		code_object *co = (code_object *) obj;
		string_object *bytecodes = (string_object *) co->code;
		block_object *bo = AllocBlockObject();
		bo->type = TYPE_BLOCK;
		bo->code = co;
		bo->ip = 0;
		//bo->iter = NULL;
		bo->start = 0;
		bo->len = bytecodes->len;
		bo->ref_count = 0;
		bo->stack = stack_Init();		// co->stacksize
		//bo->initiated_locals = 1;
		stack_Push(vm->blocks, bo);
		if (argc > 0 && locals != NULL)
		{
			for (NUM i = 0; i < argc; i++)
			{
				// printf("adding local %d\n",i);
				object *local = stack_Pop(locals,vm->garbage);

				tuple_object *co_varnames = (tuple_object *) co->varnames;
				
				if(GetItem(co_varnames,i)->type != TYPE_KV)
					{
						//printf("converting\n");
						SetItem(co_varnames,i,ConvertToKVObjectValued(GetItem(co_varnames,i),local));
					}
				else
				{
					SetDictItemByIndex(co_varnames,i,local);
				}
			}
		}
		return(bo);
}

object *vm_StartFunctionObject(vm *vm,function_object *fo,stack *locals,stack *kw_locals,NUM argc,NUM kw_argc)
{
		if(fo->func_type == FUNC_C)
		{
			#ifdef DEBUGGING
			debug_printf(DEBUG_VERBOSE_STEP,"executing C function: %s\n",  fo->name);
			#endif
			object *tmp = vm_ExecuteCFunction(vm, fo->name,locals);
			return(tmp);
		}
		if(fo->func_type == FUNC_PYTHON)
		{
			#ifdef DEBUGGING
			debug_printf(DEBUG_VERBOSE_STEP,"executing python function: %s\n",  fo->name);
			#endif
		}
	
		//code_object *co = (code_object*)fo->code;
		//tuple_object *defaults = fo->defaults;
		block_object *bo = AllocBlockObject();
		bo->type = TYPE_BLOCK;
		bo->ip = 0;
		//bo->iter = NULL;
		bo->start = 0;
		bo->ref_count = 0;
		bo->stack = stack_Init();		// co->stacksize
		//bo->initiated_locals = 1;
		if(fo->func_type == FUNC_PYTHON)
		{
			//printf("preparing python function\n");
			string_object *bytecodes = (string_object *) fo->func.code->code;
			bo->code = fo->func.code;
			bo->len = bytecodes->len;
			if(((tuple_object*)fo->func.code->varnames)->list != NULL)
			{
				ClearDictValues(fo->func.code->varnames);
				//int locals_num = ((tuple_object*)fo->func.code->varnames)->list->num;
				if(fo->defaults!=NULL)
				{
					NUM locals_num = fo->defaults->list->num;
					//printf("locals num:%d\n",locals_num);
					//printf("loading default values\n");
					for(NUM i = 0;i < fo->defaults->list->num; i++) //load default values
					{
						if(GetItem(fo->func.code->varnames,(locals_num-1) - i)->type != TYPE_KV)
						{
							SetItem(fo->func.code->varnames,(locals_num-1) - i,ConvertToKVObjectValued(GetItem(fo->func.code->varnames,(locals_num-1) - i),GetItem(fo->defaults,i)));
						}		
						else
						{
							SetDictItemByIndex(fo->func.code->varnames,(locals_num-1) - i,GetItem(fo->defaults,i));
						}
					}
				}
				if(fo->kw_defaults != NULL)
					{
					//printf("loading kw defaults\n");
					for(NUM i = 0;i < fo->kw_defaults->list->num; i++) //load default keyword values
					{
						kv_object *kv = GetItem(fo->kw_defaults,i);
						SetDictItem(fo->func.code->varnames,kv->key,kv->value);
					}
				}
			}
			//printf("loading arguments\n");
			if (argc > 0 && locals != NULL)
			{
				NUM oarg = argc;
				if(oarg > fo->func.code->argcount)
					oarg = fo->func.code->argcount;
				//printf("oarg:%d,locals:%d\n",oarg,stack_Pointer(locals));
				for (NUM i = 0; i < oarg; i++)
				{
					object *local = stack_Pop(locals,vm->garbage);
					if(GetItem(fo->func.code->varnames,i)->type != TYPE_KV)
					{
						SetItem(fo->func.code->varnames,i,ConvertToKVObjectValued(GetItem(fo->func.code->varnames,i),local));
					}
					else
					{
						SetDictItemByIndex(fo->func.code->varnames,i,local);
					}
				}
			
				//printf("loading arguments 2\n");
				if(argc > fo->func.code->argcount)
				{
					NUM narg = argc - fo->func.code->argcount;
					//printf("more locals than argcount:%d\n",narg);
					tuple_object *nt = CreateTuple(0,0); //creating tuple from the rest of the arguments
					for (NUM i = 0; i < narg; i++)
					{
						object *t = stack_Pop(locals,vm->garbage);
						t = DissolveRef(t);
						if(t->type == TYPE_KV)
							t = ((kv_object*)t)->value;
						//DumpObject(t,0);
						AppendItem(nt,t);
					}
					//DumpObject(nt,0);
					SetDictItemByIndex(fo->func.code->varnames,fo->func.code->argcount+fo->func.code->kwonlyargcount,nt);
				}		
			}
			//printf("loading keyword arguments\n");
			if(kw_argc > 0 && kw_locals != NULL)
			{
				for(NUM i = 0;i< kw_argc;i++)
				{
					kv_object *kw_local = stack_Pop(kw_locals,vm->garbage);
					SetDictItem(fo->func.code->varnames,kw_local->key,kw_local->value);
				}
			}
			
			if (fo->closure != NULL)
			{
				for (NUM i = 0; i < GetTupleLen(fo->closure); i++)
				{
					object *local = GetItem(fo->closure,i);
					if(local->type == TYPE_KV)
						local = ((kv_object*)local)->value;
					if(GetItem(fo->func.code->freevars,i)->type != TYPE_KV)
					{
						SetItem(fo->func.code->freevars,i,ConvertToKVObjectValued(GetItem(fo->func.code->freevars,i),local));
					}
					else
					{
						SetDictItemByIndex(fo->func.code->freevars,i,local);
					}
				}
			}
		}
		if(fo->func_type == FUNC_PYTHON && ((fo->func.code->co_flags & CO_NESTED)>0 || (fo->func.code->co_flags & CO_GENERATOR)>0) )
		{
			//if((fo->func.code->co_flags & CO_GENERATOR) >0)
			//	printf("function is a generator\n");
			//else
			//	printf("function is nested\n");
			iter_object *iter = CreateIterObject(0);
			iter_InitGenerator(iter,bo);
			return(iter);
		}
		//normal functions get pushed on the block stack
		stack_Push(vm->blocks, bo);
		//printf("pushed block:%x\n",bo);
		return(NULL);//got nothing to return now
}

function_object *vm_ResolveFunction(vm *vm,object *to_resolve)
{
	if(to_resolve->type == TYPE_KV)
		to_resolve = ((kv_object*)to_resolve)->value;
	to_resolve = DissolveRef(to_resolve);

	if (to_resolve != NULL && to_resolve->type == TYPE_FUNCTION)
	{
		return(to_resolve); //nothing to done really here
	}
	else	if (to_resolve != NULL && to_resolve->type == TYPE_CODE)
	{
		#ifdef DEBUGGING
		debug_printf(DEBUG_ALL,"WARNING: cant resolve from code object\n");
		#endif
		return(NULL);
	}
	else if (to_resolve != NULL && to_resolve->type == TYPE_UNICODE)
	{
		object *f = vm_FindFunction(vm, ((unicode_object*)to_resolve)->value);
		if(f != NULL)
			return(f);
		else
		{
			object *caller_func = NULL;
			INDEX index = GetItemIndexByName(vm->global->names,((unicode_object*)to_resolve)->value);
			if(index != -1)
				caller_func = GetItem(vm->global->names,index);
			if (caller_func != NULL && ((object *) caller_func)->type == TYPE_CODE)
			{
				#ifdef DEBUGGING
				debug_printf(DEBUG_ALL,"WARNING: cant resolve from code object\n");
				#endif
				return(NULL);
			}
			else if (caller_func != NULL && ((object *) caller_func)->type == TYPE_FUNCTION)
			{
				return(caller_func);
			}
		}
	}
	return(NULL);
}

#ifdef DEBUGGING
void vm_DumpCode(vm *vm,BOOL dump_descriptions, BOOL from_start)
{
				block_object *bo = (block_object *) stack_Top(vm->blocks);
				object *obj = bo->code;
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

#ifdef DEBUGGING
object *vm_InteractiveRunObject(vm *vm, object *obj, stack *locals, NUM argc)
{
		block_object *bo = vm_StartObject(vm,obj,locals,argc);
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
			int r = scanf("%s",cmd);
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
				object *obj = bo->code;
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
			if(strlen(cmd)>= 2 && cmd[0] == 'd' && cmd[1] == 'g')
			{
				printf("dumping garbage\n");
				stack_Dump(vm->garbage);
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
					DumpObject(b->code,0);
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
				#ifdef DEBUGGING
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
				object *obj = bo->code;
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
					ret = vm_StepObject(vm);
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
					ret = vm_StepObject(vm);
			}else
			if(strlen(cmd)>= 1 && cmd[0] == 'r')
			{
				vm->running = 1;
				vm->interrupt_vm = 0;
			}
			#ifdef DEBUGGING
			assert(mem_free(cmd));
			#else
			free(cmd);
			#endif
			}
			else //run or steps != 0
			{
				//printf("running\n");
				if(vm->running)
					ret = vm_StepObject(vm);
				else
				{
					for(int i=0;i<steps;i++)
					{
						printf("step %d\n",i+1);
						ret = vm_StepObject(vm);
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

object *vm_RunObject(vm *vm, object *obj, stack *locals, NUM argc)
{
		block_object *bo = vm_StartObject(vm,obj,locals,argc);
		object *ret = NULL;
		while(ret == NULL)
			ret = vm_StepObject(vm);
		return(ret);
}

object *vm_StepObject(vm *vm)
{
	block_object *bo = (block_object *) stack_Top(vm->blocks);
	object *obj = bo->code;
	
	if (obj->type == TYPE_CODE)
	{
		code_object *co = (code_object *) obj;
		string_object *bytecodes = (string_object *) co->code;
		char *string = bytecodes->content;
		code_object *global = vm->global;

		BOOL has_extended_arg = 0;
		short extended_arg = 0;

		if(bo->ip < bo->len)
		{
			unsigned char op = (unsigned char)string[bo->ip++];	// get op and increment code pointer

			INT arg = 0;		// holds the argument if present
			BOOL op_thru = 0;	// used to skip unneccessary switches

			object *tos = NULL; //top of the stack objects
			object *tos1 = NULL;
			object *tos2 = NULL;

			tuple_object *var_list = NULL;//used for simpler function calling
			tuple_object *kw_list = NULL;//used for simpler function calling
			
			#ifdef DEBUGGING// FOR DEBUGGING
			if((debug_level & DEBUG_SHOW_OPCODES) > 0)
			{
				int old_i = bo->ip;
				int index = GetOpcodeIndex(op);
				if (index >= 0)
				{
					if (opcodes[index].argcount != 0)
						debug_printf(DEBUG_ALL,"[%03d,%02xh] [ %s ] (%d)\n", bo->ip-1,opcodes[index].opcode, opcodes[index].name,num_short(string[bo->ip], string[bo->ip + 1]));
					else
						debug_printf(DEBUG_ALL,"[%03d,%02xh] [ %s ]\n", bo->ip-1,opcodes[index].opcode, opcodes[index].name);
				}
				else
				{
					debug_printf(DEBUG_ALL,"unknown opcode:%x at %d\n", (char)string[bo->ip - 1],bo->ip - 1);
				}
			}

			switch (op)
			{
			case OPCODE_UNPACK_EX	:
			case OPCODE_BUILD_SLICE:
				debug_printf(DEBUG_ALL,"not supported\n");
				vm_Interrupt(vm,NULL);
				//bo->ip--;
				op_thru = 1;
				break;

			}
			if (op_thru)
			{
				gc_Clear(vm->garbage);
				return(NULL);
			}
			#endif // -- FOR DEBUGGING
		
			
			#ifdef DEBUGGING
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
				
			case OPCODE_IMPORT_STAR:
				{
				op_thru = 1;
				}
				break;
				
			case OPCODE_YIELD_VALUE:
				{
					bo->ip++;
					object *ret = stack_Pop(bo->stack,vm->garbage);
					IncRefCount(ret);
					//printf("yielding\n");
					gc_Clear(vm->garbage);
					
					return(ret);
				}
				break;
			case OPCODE_RETURN_VALUE:
				{
					object *ret = stack_Pop(bo->stack,vm->garbage);
					/*IncRefCount(ret);
					//ret->flags |= OFLAG_LEFT_NAMESPACE;
					//ret = DissolveRef(ret);
					//	if(ret->type == TYPE_KV)
					//	{
					//		ret = ((kv_object*)ret)->value;
					//		((kv_object*)ret)->value = NULL;
					//	}
					//assert(ret != NULL);
					//ret = stack_Pop(bo->stack,vm->garbage);
					//object *t = stack_Pop(bo->stack);*/

					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
						debug_printf(DEBUG_VERBOSE_STEP,"ret object @:%x\n",ret);
						DumpObject(ret,0);
					}
					#endif
					object *pbo = stack_Pop(vm->blocks,vm->garbage);
					/*ret = CopyObject(ret);
					//ret = CreateRefObject(ret,0);
					//printf("popped block:%x\n",pbo);
					//DumpObject(pbo,0);
					//stack_Dump(bo->stack);
					//if(!stack_IsEmpty(vm->blocks))
					//{
					//if(debug_level > 5)
					//	printf("pushing ret\n");*/

					if(!stack_IsEmpty(vm->blocks))
					{
						block_object *parent = (block_object*)stack_Top(vm->blocks);
						#ifdef DEBUGGING
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						{
							DumpObject(parent,0);
							stack_Dump(parent->stack);
						}
						#endif
						stack_Push(parent->stack,ret);
						#ifdef DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"still blocks on the stack\n");
						#endif
						gc_Clear(vm->garbage);
					}
					else 
					{
						IncRefCount(ret);
						gc_Clear(vm->garbage);
						/*object *tmp = CreateEmptyObject(TYPE_NONE,0);
						//IncRefCount(tmp);
						//printf("no return object on stack , returning NONE object\n");
						//return (tmp);
						//return (NULL);*/
						return(ret);
					}					
					gc_Clear(vm->garbage);
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
					object *pt = stack_Pop(bo->stack,vm->garbage);
					//FreeObject(pt);
					op_thru = 1;
				}
				break;

			case OPCODE_POP_BLOCK:
				{
					block_object *rob = stack_Pop(vm->blocks,vm->garbage);
					//if(rob->iter != NULL)
						//FreeObject(rob->iter);
					//	DecRefCountGC(rob->iter,vm->garbage);
					//DecRefCountGC(rob,vm->garbage);
					//assert(mem_free(rob));
					op_thru = 1;
				}
				break;

			case OPCODE_BREAK_LOOP:
				{
					block_object *bbo = (block_object *) stack_Pop(vm->blocks,vm->garbage);
					//bo->ip = bbo->start + bbo->len - 1;
					#ifdef DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"break to: %d ,start: %d ,len:%d\n",bo->ip,bbo->start,bbo->len);
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
					IncRefCount(tos);
					stack_SetSecond(bo->stack, tos1);
					IncRefCount(tos1);
					op_thru = 1;
				}
				break;

			case OPCODE_GET_ITER:
				{
					object *iter = stack_Pop(bo->stack,vm->garbage);
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
					 debug_printf(DEBUG_VERBOSE_STEP,"iter:\n");
					 DumpObject(iter,0);
					//printf("bo_stack:\n");
					//stack_Dump(vm->blocks);
					//printf("bo:\n");
					//DumpObject(stack_Top(vm->blocks),0);
					}
					#endif
					//block_object *abo = (block_object *) stack_Top(vm->blocks);
					//abo->iter = iter;
					//IncRefCount(iter);
					//ResetIteration(iter);
					//stack_Push(bo->stack,iter);
					/*if(abo->iter != NULL)
					{
						#ifdef DEBUGGING
						debug_printf(DEBUG_ALL,"block iter already used\n");
						#endif
						DecRefCountGC(abo->iter,vm->garbage);
					}*/
					iter_object *it = iter_CreateIter(iter);//,vm);
					stack_Push(bo->stack,it);
					//abo->iter = it;
					//IncRefCount(it);	
					op_thru = 1;
				}
				break;

			case OPCODE_EXTENDED_ARG:
				{
					extended_arg = num_short(string[bo->ip + 1], string[bo->ip + 2]);
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
			case OPCODE_STORE_LOCALS:
			case OPCODE_PRINT_EXPR:
			case OPCODE_LOAD_BUILD_CLASS:
			case OPCODE_WITH_CLEANUP:
			case OPCODE_END_FINALLY:
			case OPCODE_BUILD_CLASS:
			case OPCODE_COMPARE_OP:
			case OPCODE_IMPORT_FROM:
			case OPCODE_IMPORT_NAME:
			case OPCODE_SETUP_EXCEPT:
			case OPCODE_SETUP_FINALLY:
			case OPCODE_MAKE_FUNCTION:
			case OPCODE_RAISE_VARARGS:
				{
					if (has_extended_arg)
					{
						arg = num_long((extended_arg >> 8) & 0xFF, extended_arg & 0xFF, string[bo->ip], string[bo->ip + 1]);
						has_extended_arg = 0;
						extended_arg = 0;
					}
					else
						arg = (long)num_short(string[bo->ip], string[bo->ip + 1]);
					bo->ip += 2;
				}
				break;
			}

			if (op_thru)
			{
				gc_Clear(vm->garbage);
				return(NULL);
			}

			#ifdef DEBUGGING
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
				{
					tos = stack_Pop(bo->stack,vm->garbage);
					tos = DissolveRef(tos);
					if(tos->type == TYPE_KV)
						tos = ((kv_object*)tos)->value;
					#ifdef DEBUGGING
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
				{
					tos = stack_Pop(bo->stack,vm->garbage);
					tos1 = stack_Pop(bo->stack,vm->garbage);
					tos = DissolveRef(tos);
					tos1 = DissolveRef(tos1);
					if(tos->type == TYPE_KV)
						tos = ((kv_object*)tos)->value;
					if(tos1->type == TYPE_KV)
						tos1 = ((kv_object*)tos1)->value;
					#ifdef DEBUGGING
					assert(tos != NULL);
					assert(tos1 != NULL);
					#endif
				}
				break;

			case OPCODE_STORE_SUBSCR:
			case OPCODE_STORE_MAP:
				{
					tos = stack_Pop(bo->stack,vm->garbage);
					tos1 = stack_Pop(bo->stack,vm->garbage);
					tos2 = stack_Pop(bo->stack,vm->garbage);
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

			#ifdef DEBUGGING
			debug_printf(DEBUG_VERBOSE_STEP,"executing main ops\n");
			#endif
			// execute remaining ops here
			switch (op)
			{
			case OPCODE_FOR_ITER:
				{
					//block_object *fabo = (block_object *) stack_Top(vm->blocks);
					//object *next = iter_Next(fabo->iter);
					//DumpObject(fabo,0);
					//printf("iter:\n");
					//DumpObject(fabo->iter,0);
					//printf("stack\n");
					//stack_Dump(bo->stack);
					object *next =  NULL;
					//if(fabo->iter != NULL)
					//	next = iter_Next(fabo->iter);
					//else 
					if(stack_Top(bo->stack)->type == TYPE_ITER)
						next = iter_NextNow(stack_Top(bo->stack),vm);
					/*if(next != NULL && next->type == TYPE_BLOCK)
					{
						stack_Push(vm->blocks, next);
					}
					else*/
					if (next != NULL && next->type != TYPE_NONE)
					{
						stack_Push(bo->stack, next);
					}
					else
					{
						if(next != NULL)
							DecRefCountGC(next,vm->garbage);
							//FreeObject(next);
						//stack_Pop(bo->stack,vm->garbage);
						//if(fabo->iter != NULL)
						//	DecRefCountGC(fabo->iter,vm->garbage);
						//fabo->iter = NULL;
						bo->ip = bo->ip + arg;
					}
					//printf("stack after\n");
					//stack_Dump(bo->stack);
				}
				break;
				
			case OPCODE_IMPORT_NAME:
				{
				
				}
				break;
				
			case OPCODE_IMPORT_FROM:
				{
				
				}
				break;
				
			case OPCODE_IMPORT_STAR:
				{
				
				}
				break;
				
			case OPCODE_MAKE_FUNCTION:
				{
					NUM narg = (NUM) (arg & 255);
					NUM nkey = (NUM) ((arg >> 8) & 255);
					NUM n = (NUM) (narg + (nkey*2));
					tuple_object *defaults = NULL;
					tuple_object *kw_defaults = NULL;
					
					if(narg > 0)
					{
						defaults = CreateTuple(narg,0); //creating defaults tuple
						for (NUM i = 0; i < narg; i++)
						{
							object *t = stack_Pop(bo->stack,vm->garbage);
							t = DissolveRef(t);
							if(t->type == TYPE_KV)
								t = ((kv_object*)t)->value;
							SetItem(defaults,i,t);
						}
					}
					if(nkey >0)
					{
						kw_defaults = CreateTuple(0,0); //creating kw defaults tuple
						for (NUM i = 0; i < nkey; i++)//keyword arguments
						{
							object *value = stack_Pop(bo->stack,vm->garbage);
							object *key = stack_Pop(bo->stack,vm->garbage);
							SetDictItem(kw_defaults,key,value);
						}
					}
					function_object *fo = CreateFunctionObject_MAKE_FUNCTION(tos,defaults,kw_defaults,0);
					stack_Push(bo->stack,fo);
				}
				break;
				
			case OPCODE_MAKE_CLOSURE:
				{
					NUM narg = (NUM) (arg & 255);
					NUM nkey = (NUM) ((arg >> 8) & 255);
					NUM n = (NUM) (narg + (nkey*2));
					tuple_object *defaults = NULL;
					tuple_object *kw_defaults = NULL;
					tos1 = stack_Pop(bo->stack,vm->garbage);
					tos1 = DissolveRef(tos1);
					if(narg > 0)
					{
						defaults = CreateTuple(narg,0); //creating defaults tuple
						for (NUM i = 0; i < narg; i++)
						{
							object *t = stack_Pop(bo->stack,vm->garbage);
							t = DissolveRef(t);
							if(t->type == TYPE_KV)
								t = ((kv_object*)t)->value;
							SetItem(defaults,i,t);
						}
					}
					if(nkey >0)
					{
						kw_defaults = CreateTuple(0,0); //creating kw defaults tuple
						for (NUM i = 0; i < nkey; i++)//keyword arguments
						{
							object *value = stack_Pop(bo->stack,vm->garbage);
							object *key = stack_Pop(bo->stack,vm->garbage);
							SetDictItem(kw_defaults,key,value);
						}
					}
					//DumpObject(tos1,0);
					function_object *fo = CreateFunctionObject_MAKE_CLOSURE(tos,tos1,defaults,kw_defaults,0);
					stack_Push(bo->stack,fo);
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
					if (tos->type == TYPE_FALSE)
					{
						bo->ip = arg;
					}
				}
				break;
				
			case OPCODE_JUMP_IF_TRUE:
				{
					if (tos->type == TYPE_TRUE)
					{
						bo->ip = arg;
					}
				}
				break;
				
			case OPCODE_SETUP_LOOP:
				{
					block_object *block = AllocBlockObject();
					block->start = bo->ip;
					block->code = co;
					block->ip = bo->ip;
					block->len = bo->ip + arg;
					block->ref_count = 0;
					block->type = TYPE_BLOCK;
					block->stack = stack_Init();
					//printf("block - start: %d, len: %d\n",block->start,block->len);
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(bo,0);
					#endif
					stack_Push(vm->blocks, block);
					bo->ip = block->len;
				}
				break;
				
			case OPCODE_BUILD_SET:
				{
					tuple_object *to =CreateTuple(0,0);
					stack_Push(bo->stack,to);
				}
				break;
				
			case OPCODE_BUILD_MAP:
				{
					tuple_object *to =CreateTuple(0, OFLAG_IS_DICT);
					stack_Push(bo->stack,to);
				}
				break;
				
			case OPCODE_BUILD_TUPLE:
			case OPCODE_BUILD_LIST:
				{
					// stack_Push(BuildList(bo->stack,arg),bo->stack);
					stack *blcall = NULL;
					blcall = stack_Init();	// arg
					tos = NULL;
					for (NUM i = 0; i < arg; i++)
					{
						stack_Push(blcall, stack_Pop(bo->stack,vm->garbage));
					}
					object *tmp = vm_ExecuteCFunction(vm, "list", blcall);
					if (tmp != NULL)
					{
						stack_Push(bo->stack, tmp);
						//DecRefCount(tmp);
					}
					stack_Close(blcall, 0);
				}
				break;
				
			case OPCODE_LOAD_GLOBAL:
				{
					tuple_object *co_names = (tuple_object *) co->names;
					char *name = NULL;
					if(((object*)co_names->list->items[arg])->type == TYPE_KV)
						name = ((unicode_object*)((kv_object*)co_names->list->items[arg])->key)->value;
					else
						name = ((unicode_object*)co_names->list->items[arg])->value;
					#ifdef DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"searching for:%s\n", name);
					#endif
					INDEX index = GetItemIndexByName(global->names,name);
					object *lgo = NULL;
					if(index != -1)
					{
						lgo = GetItem(global->names,index);
					}
					index = GetItemIndexByName(global->varnames,name);
					if(index != -1)
					{
						lgo = GetItem(global->varnames,index);
					}
					if (lgo == NULL)
					{
						lgo = vm_FindFunction(vm, name);
						if (lgo  == NULL)
						{
							#ifdef DEBUGGING
							debug_printf(DEBUG_VERBOSE_STEP,"global not found\n");
							#endif
							break;
						}
					}
					stack_Push(bo->stack, lgo);
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(lgo,1);
					#endif
					// stack_Dump(bo->stack);
				}
				break;

			case OPCODE_STORE_GLOBAL:
				{
					tuple_object *co_names = (tuple_object *) co->names;
					char *name = NULL;
					if(((object*)co_names->list->items[arg])->type == TYPE_KV)
						name = ((unicode_object*)((kv_object*)co_names->list->items[arg])->key)->value;
					else
						name = ((unicode_object*)co_names->list->items[arg])->value;
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					{
						debug_printf(DEBUG_VERBOSE_STEP,"name:%s\n",name);
						DumpObject(tos,0);
					}
					#endif
					
					INDEX index = GetItemIndexByName(global->names,name);
					if(index != -1)
					{
					if(((object*)((tuple_object*)global->names)->list->items[index])->type != TYPE_KV)
					{
						((tuple_object*)global->names)->list->items[index] = ConvertToKVObjectValued(((tuple_object*)global->names)->list->items[index],tos);
						DecRefCountGC(((kv_object*)((tuple_object*)global->names)->list->items[index])->key,vm->garbage);
						IncRefCount(((tuple_object*)global->names)->list->items[index]);
					}
					else	
					{
							if(((kv_object*)((tuple_object*)global->names)->list->items[index])->value != NULL)
								DecRefCountGC(((kv_object*)((tuple_object*)global->names)->list->items[index])->value,vm->garbage);
							((kv_object*)((tuple_object*)global->names)->list->items[index])->value = tos;
							IncRefCount(tos);
					}
					break;		
					}
					index = GetItemIndexByName(global->varnames,name);
					if(index != -1)
					{
					if(((object*)((tuple_object*)global->varnames)->list->items[index])->type != TYPE_KV)
					{
						((tuple_object*)global->varnames)->list->items[index] = ConvertToKVObjectValued(((tuple_object*)global->varnames)->list->items[index],tos);
						DecRefCountGC(((kv_object*)((tuple_object*)global->names)->list->items[index])->key,vm->garbage);
						IncRefCount(((tuple_object*)global->varnames)->list->items[index]);
					}
					else
					{
							((kv_object*)((tuple_object*)global->varnames)->list->items[index])->value = tos;
							if(((kv_object*)((tuple_object*)global->varnames)->list->items[index])->value != NULL)
								DecRefCountGC(((kv_object*)((tuple_object*)global->varnames)->list->items[index])->value,vm->garbage);
							IncRefCount(tos);
					}
					break;
					}		
				#ifdef DEBUGGING
				debug_printf(DEBUG_VERBOSE_STEP,"global not found\n");
				#endif
				}
				break;

			case OPCODE_DELETE_GLOBAL:
				{
					//TODO needs to be TYPE_KV aware and has to remove unneeded objects
					tuple_object *co_names = (tuple_object *) co->names;
					char *name = NULL;
					if(((object*)co_names->list->items[arg])->type == TYPE_KV)
						name = ((unicode_object*)((kv_object*)co_names->list->items[arg])->key)->value;
					else
						name = ((unicode_object*)co_names->list->items[arg])->value;

					INDEX index = GetItemIndexByName(global->names,name);
					if(index != -1)
					{
						SetDictItemByIndex(global->names,index,NULL);
						//DeleteItem(global->names, index);
						break;
					}
					index = GetItemIndexByName(global->varnames,name);
					if(index != -1)
					{
						SetDictItemByIndex(global->varnames,index,NULL);
						//DeleteItem(global->varnames, index);
					}
				}
				break;

			case OPCODE_LOAD_NAME:
				{
					object *name = GetItem(co->names,arg);
					if(name->type == TYPE_KV)
						name = ((kv_object*)name)->value;
					stack_Push(bo->stack, name);
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(name,1);
					#endif
				}
				break;

			case OPCODE_LOAD_CONST:
				{
					object *lconst = GetItem(co->consts,arg);
					stack_Push(bo->stack, lconst);
					#ifdef DEBUGGING
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
					#ifdef DEBUGGING
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
							SetItem(co->cellvars,arg,ConvertToKVObjectValued(GetItem(co->cellvars,arg),tos));
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
							SetItem(co->freevars,arg - GetTupleLen(co->cellvars),ConvertToKVObjectValued(GetItem(co->freevars,arg - GetTupleLen(co->cellvars)),tos));
						}
						else
						{
							SetDictItemByIndex(co->freevars,arg - GetTupleLen(co->cellvars),tos);
						}
					}
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(tos,1);
					#endif
				}
				break;

			case OPCODE_LOAD_DEREF:
				{
					object *cell =  NULL;
					if(arg < GetTupleLen(co->cellvars))
						cell = GetItem(co->cellvars,arg);
					else
						cell = GetItem(co->freevars,arg - GetTupleLen(co->cellvars));
						if(cell->type == TYPE_KV)
							cell = ((kv_object*)cell)->value;
					stack_Push(bo->stack, cell);
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(cell,1);
					#endif
				}
				break;

			case OPCODE_LOAD_CLOSURE:
				{
					object *fast = NULL;
					if(arg < GetTupleLen(co->cellvars))
						//GetDictItemByIndex(co->cellvars,arg);
						fast = GetItem(co->cellvars,arg);
						//fast = GetItem(co->freevars,arg);
					//if(fast == NULL)
					else
						//fast = GetDictItemByIndex(co->freevars,arg - GetTupleLen(co->cellvars));
						fast = GetItem(co->freevars,arg - GetTupleLen(co->cellvars));
					//printf("closure:\n");
					//DumpObject(fast,0);
					//printf("-- closure\n");
					stack_Push(bo->stack, fast);
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(fast,1);
					#endif
				}
				break;

			case OPCODE_DELETE_DEREF:
				{
					//if(
					//SetDictItemByIndex(co_freevars,arg,NULL);
				}
				break;

			case OPCODE_STORE_NAME:
				{
					//tuple_object *co_names = (tuple_object *) co->names;
					//if(tos->type == TYPE_CODE)
						//tos = CreateRefObject(tos,0);
					//else
						//tos = CopyObject(tos);
					if(GetItem(co->names,arg)->type != TYPE_KV)
					{
						SetItem(co->names,arg,ConvertToKVObjectValued(GetItem(co->names,arg),tos));
					}
					else
					{
						SetDictItemByIndex(co->names,arg,tos);
					}
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(tos,1);
					#endif
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
					#ifdef DEBUGGING
					assert(tos2 != NULL);
					DumpObject(tos2,0);
					printf("append dict item:\n");
					DumpObject(tos,0);
					printf("value:\n");
					DumpObject(tos1,0);
					#endif
					SetDictItem(tos2,tos,tos1);
					//stack_Push(bo->stack,);
				}
				break;

			case OPCODE_LIST_APPEND:
				{
					tos1 = stack_Get(bo->stack,-arg);
					tos1 = DissolveRef(tos1);
					if(tos1->type == TYPE_KV)
						tos1 = ((kv_object*)tos1)->value;
					#ifdef DEBUGGING
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
					#ifdef DEBUGGING
					assert(tos1 != NULL);
					DumpObject(tos1,0);
					printf("append item:\n");
					DumpObject(tos,0);
					#endif
					AppendItem(tos1,tos);
				}
				break;
				
			case OPCODE_STORE_FAST:
				{
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						DumpObject(tos,1);
					#endif
					tuple_object *co_varnames = (tuple_object*) co->varnames;
					if(GetItem(co_varnames,arg)->type != TYPE_KV)
					{
						#ifdef DEBUGGING
						debug_printf(DEBUG_VERBOSE_STEP,"converting\n");
						#endif
						SetItem(co_varnames,arg,ConvertToKVObjectValued(GetItem(co_varnames,arg),tos));
					}
				else
				{
					#ifdef DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"updating\n");
					#endif
					SetDictItemByIndex(co_varnames,arg,tos);
				}
				#ifdef DEBUGGING
				if((debug_level & DEBUG_VERBOSE_STEP) > 0)
					DumpObject(tos,1);
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
						// DumpObject(tos2,0);
						// printf("ssa: %d\n",ssa);
						DeleteItem(tos1, dsa);
						// DumpObject (tos1, 0);
					}
				}
				break;

			case OPCODE_STORE_SUBSCR:
				{
					NUM ssa = 0;
					//tos2 = CopyObject(tos2);
					//IncRefCount(tos2);
					#ifdef DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"tos:%c\n",tos->type);
					debug_printf(DEBUG_VERBOSE_STEP,"tos1:%c\n",tos1->type);
					#endif
					 
					if (tos->type == TYPE_INT)
					{
						ssa = (NUM) ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_TUPLE)
					{
						// DumpObject(tos2,0);
					#ifdef DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"ssa: %d\n",ssa);
					#endif
					if (tos->type == TYPE_INT)
						SetItem(tos1, ssa, tos2);
					else
						SetDictItem(tos1,tos,tos2);
						// DumpObject(tos1,0);
					#ifdef DEBUGGING
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						 DumpObject(tos1,0);
					#endif
					}
				}
				break;

			case OPCODE_BINARY_SUBSCR:
				{
					NUM bsa = 0;

					// printf("binary subscribe");
					if (tos->type == TYPE_INT)
					{
						bsa = (NUM) ((int_object*)tos)->value;
					}
				
					if (tos1->type == TYPE_TUPLE)
					{
						#ifdef DEBUGGING
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
						#ifdef DEBUGGING
						if((debug_level & DEBUG_VERBOSE_STEP) > 0)
							DumpObject(bst, 0);
						#endif
						stack_Push(bo->stack, bst);
					}
				}
				break;

			case OPCODE_UNPACK_SEQUENCE:
				{
					for (NUM i = arg-1; i >= 0; i--)
					{
						switch (tos->type)
						{
						case TYPE_UNICODE:
							{
								unicode_object *uso = CreateUnicodeObject(str_FromChar(((unicode_object*)tos)->value[i]),0);
								#ifdef DEBUGGING
								if((debug_level & DEBUG_VERBOSE_STEP) > 0)
									DumpObject(uso,0);
								#endif
								stack_Push(bo->stack, uso);
							}
						}
					}
					#ifdef DEBUGGING
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
						int_object *up = CreateIntObject(+((int_object*)tos)->value,0);
						stack_Push(bo->stack, up);
					}
				}
				break;

			case OPCODE_UNARY_NEGATIVE:
				{
					if (tos->type == TYPE_INT)
					{
						int_object *up = CreateIntObject(-((int_object*)tos)->value,0);
						stack_Push(bo->stack, up);
					}
				}
				break;

			case OPCODE_UNARY_NOT:
				{
					if (tos->type == TYPE_INT)
					{
						object *up = CreateEmptyObject(((int_object*)tos)->value > 0 ? TYPE_TRUE : TYPE_FALSE,0);
						stack_Push(bo->stack, up);
					}
					else if (tos->type == TYPE_TRUE || tos->type == TYPE_FALSE)
					{
						object *up = CreateEmptyObject(tos->type == TYPE_TRUE ? TYPE_TRUE : TYPE_FALSE,0);
						stack_Push(bo->stack, up);
					}
					else if (tos->type == TYPE_NONE || tos->type == TYPE_NULL)
					{
						object *up = CreateEmptyObject( TYPE_FALSE,0);
						stack_Push(bo->stack, up);
					}
				}
				break;

			case OPCODE_UNARY_INVERT:
				{
					if (tos->type == TYPE_INT)
					{
						int_object *up = CreateIntObject(-((((int_object*)tos)->value) + 1),0);
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
						kw_list = stack_Pop(bo->stack,vm->garbage);
				}
			case OPCODE_CALL_FUNCTION_VAR:
				{
					if(op == OPCODE_CALL_FUNCTION_VAR_KW || op == OPCODE_CALL_FUNCTION_VAR)
						var_list = stack_Pop(bo->stack,vm->garbage);
				}
			case OPCODE_CALL_FUNCTION:
				{
					stack *call = NULL;
					stack *kw_call = NULL;
					NUM narg = (NUM) (arg & 255);
					NUM nkey = (NUM) ((arg >> 8) & 255);
					NUM n = (NUM) (narg + (nkey*2));
					//fetch function object from tos and Resolve to function object
					object *function = stack_Get(bo->stack,stack_Pointer(bo->stack) - n);
					//printf("calling function:\n");
					//DumpObject(function,0);
					function_object *fo = vm_ResolveFunction(vm,function);
					//printf("resolved to:\n");
					//DumpObject(fo,0);
					
					if (narg > 0 || var_list != NULL)
						call = stack_Init();	// arg, 
					if (nkey > 0 )
						kw_call = stack_Init();
					if(kw_list != NULL) //variable keyword arguments
					{
						//printf("var keywords found:%d\n",kw_list->list->num);
						for (NUM i = kw_list->list->num-1; i >= 0 ;i--)
						{
							//printf("i:%d\n",i);
							object *v = GetItem(kw_list,i);
							//stack_Push(call, v);
							//DumpObject(v,0);
							if(fo->func_type == FUNC_PYTHON)
							{
								//printf("setting varname\n");
								SetDictItem(fo->func.code->varnames,((kv_object*)v)->key,((kv_object*)v)->value);
							}
						}	
					}
					
					//printf("kwlist prepared\n");
					if(var_list != NULL) //variable list arguments
					{
						//printf("var list found:%d\n",var_list->list->num);
						for (NUM i = var_list->list->num; i >= 0 ;--i)
						{
							object *v = GetItem(var_list,i);
							stack_Push(call, v);
							//DumpObject(v,0);
							//printf("i:%d\n");
						}	
					}
					//if(call != NULL)
					//	stack_Dump(call);
					//printf("var list prepared\n");
					
					for (NUM i = 0; i < nkey; i++)//keyword arguments
					{
						object *value = stack_Pop(bo->stack,vm->garbage);
						object *key = stack_Pop(bo->stack,vm->garbage);
						//now setdictitem in function code varnames
						//printf("keyword arg:\n");
						//DumpObject(key,0);
						//printf("value:\n");
						//DumpObject(value,0);
						
						if(fo->func_type == FUNC_PYTHON)
						{
							//printf("setting varname\n");
							//SetDictItem(fo->func.code->varnames,key,value);
							kv_object *kv = CreateKVObject(key,value,0);
							stack_Push(kw_call,kv);
						}
					}
					//stack_Dump(kw_call);
					//printf("narg:%d\n",narg);
					for (NUM i = 0; i < narg; i++)//positional arguments
					{
						stack_Push(call, stack_Pop(bo->stack,vm->garbage));
					}
					if(var_list != NULL)
 						narg += var_list->list->num;

					//pop function object
					//object *function_name = stack_Pop(bo->stack,vm->garbage);
					stack_Pop(bo->stack,vm->garbage);
					//printf("pushing function block on block stack\n");
					object *pret = vm_StartFunctionObject(vm,fo, call,kw_call,narg,nkey);
					if (pret != NULL)
					{
							stack_Push(bo->stack, pret);
					}
					if (narg > 0 || var_list != NULL)
						stack_Close(call, 0);
					if (nkey > 0)
						stack_Close(kw_call, 0);
				}
				break;

			default:
				#ifdef DEBUGGING
				if((debug_level & DEBUG_DUMP_UNSUPPORTED) > 0)
				{
					int index = GetOpcodeIndex(op);
					debug_printf(DEBUG_DUMP_UNSUPPORTED,"[%02xh] [ %s ] not supported !!\n",opcodes[index].opcode, opcodes[index].name);
				}
				#endif
				break;
				
			}

			// printf("step\n");
			gc_Clear(vm->garbage);
			return(NULL);
		} //END OF STEP
		#ifdef DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"code thru.\n");
		#endif
		object *ret = NULL;
		if(stack_IsEmpty(bo->stack))
		{
			stack_Pop(vm->blocks,vm->garbage);
			return(NULL);
		}
		else
		{
			stack_Pop(vm->blocks,vm->garbage);
			ret = stack_Pop(bo->stack,vm->garbage);
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

