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

function_definition *CreateCFunction(object * (*func) (vm *vm,stack * stack),
									 char *name)
{
	function_definition *fd =
		mem_malloc(sizeof(function_definition), "CreateCFunction() fd");
	fd->type = FUNC_C;
	fd->func.func = func;
	fd->name = str_Copy(name);
	return (fd);
}

function_definition *CreateCObjFunction(object * (*func) (vm *vm,object * obj),
										char *name)
{
	function_definition *fd =
		mem_malloc(sizeof(function_definition), "CreateCObjFunction() fd");
	fd->type = FUNC_C_OBJ;
	fd->func.func_obj = (*func);
	fd->name = str_Copy(name);
	return (fd);
}

function_definition *CreatePythonFunction(object * code, char *name)
{
	function_definition *fd =
		mem_malloc(sizeof(function_definition), "CreatePythonFunction() fd");
	fd->type = FUNC_PYTHON;
	fd->func.code = code;
	fd->name = str_Copy(name);
	return (fd);
}

int vm_AddFunctionDefinition(vm * vm, function_definition * fd)
{
	ptr_Push(vm->functions, fd);
	return (vm->functions->num - 1);
}

void vm_RemoveFunction(vm * vm, char *name)
{
	for (int i = 0; i < vm->functions->num; i++)
	{
		if (!strcmp
			(((function_definition *) vm->functions->items[i])->name, name))
		{
			FreeFunctionDefinition((function_definition *) vm->functions->
								   items[i]);
			ptr_Remove(vm->functions, i);
		}
	}
}

void vm_RemoveFunctionDefinition(vm * vm, function_definition * fd)
{
	for (int i = 0; i < vm->functions->num; i++)
	{
		if (vm->functions->items[i] == fd)
		{
			FreeFunctionDefinition(fd);
			ptr_Remove(vm->functions, i);
		}
	}
}

object *vm_ExecuteCFunction(vm * vm, char *name, stack * stack)
{
	function_definition *fd = vm_FindFunction(vm, name);

	if (fd != NULL)
	{
		// printf("fd found:%s\n",fd->name);
		return ((*fd->func.func) (vm,stack));

	}
}

object *vm_ExecuteCObjFunction(vm * vm, char *name, object * obj)
{
	function_definition *fd = vm_FindFunction(vm, name);

	if (fd != NULL)
	{
		return ((*fd->func.func_obj) (vm,obj));
	}
}

function_definition *vm_FindFunction(vm * vm, char *name)
{
	for (int i = 0; i < vm->functions->num; i++)
	{
		if (!strcmp
			(((function_definition *) vm->functions->items[i])->name, name))
			return ((function_definition *) (vm->functions->items[i]));
	}
	return (NULL);
}

void FreeFunctionDefinition(function_definition * fd)
{
	assert(mem_free(fd->name));
	assert(mem_free(fd));
}

vm *vm_Init(code_object * co)
{
	vm *tmp = (vm *) mem_malloc(sizeof(vm), "vm_Init() return");

	tmp->functions = ptr_CreateList(0, 0);
	//tmp->recycle = stack_Init(NULL);
	tmp->blocks = stack_Init();
	tmp->garbage = ptr_CreateList(0,0);

	if (co != NULL)
	{
		block_object *bo = AllocBlockObject();
		bo->type = TYPE_BLOCK;
		bo->code = co;
		bo->ip = 0;
		bo->iter = NULL;
		bo->start = 0;
		bo->len = 0;
		bo->ref_count = 0;
		stack_Push(tmp->blocks, bo);
	}
	// else
	// stack_Adjust(tmp->blocks,1);//create space for global object either way
	return (tmp);
}

void vm_Close(vm * vm)
{
	if (vm->functions->num)
	{
		for (int i = 0; i < vm->functions->num; i++)
			FreeFunctionDefinition((function_definition *) vm->functions->
								   items[i]);
	}
	ptr_CloseList(vm->functions);
	
	gc_Clear(vm->garbage);
	ptr_CloseList(vm->garbage);
	// stack_Close(vm->blocks,0);
	for(int i = 0;i<vm->blocks->list->num;i++)
		assert(mem_free(vm->blocks->list->items[i]));
	//stack_Close(vm->recycle, 1);
	
	// stack_Dump(vm->blocks);
	stack_Close(vm->blocks, 0);
	assert(mem_free(vm));
}

void vm_SetGlobal(vm * vm, code_object * co)
{
	block_object *bo = AllocBlockObject();
	bo->type = TYPE_BLOCK;
	bo->code = co;
	bo->ip = 0;
	bo->iter = NULL;
	bo->start = 0;
	bo->len = 0;
	bo->ref_count = 0;
	// printf("global : %d\n",vm->blocks->list->num);
	if (vm->blocks->list->num > 0)
		stack_SetBottom(vm->blocks, bo);
	else
		stack_Push(vm->blocks, bo);
}

object *vm_RunObject(vm * vm, object * obj, object * caller, stack * locals, int argc)	// 
{
	if (obj->type == TYPE_CODE)
	{
		// printf("showing bytecodes\n");
		code_object *co = (code_object *) obj;
		string_object *bytecodes = (string_object *) co->code;
		char *string = bytecodes->content;
		//long n = bytecodes->len;

		block_object *bo = AllocBlockObject();
		bo->type = TYPE_BLOCK;
		bo->code = co;
		bo->ip = 0;
		bo->iter = NULL;
		bo->start = 0;
		bo->len = bytecodes->len;
		bo->ref_count = 0;
		// printf("global : %d\n",vm->blocks->list->num);
		stack_Push(vm->blocks, bo);
		
		// printf("co_ptr=%x\n",(unsigned long)co);
		// printf("stack size:%d\n",co->stacksize);
		code_object *global = ((block_object *) stack_Bottom(vm->blocks))->code;

		stack *_stack = stack_Init();	// co->stacksize
		// printf("local stack created\n");


		// printf("run init complete\n");
		if (argc > 0 && locals != NULL)
		{
			for (int i = 0; i < argc; i++)
			{
				// printf("adding local %d\n",i);
				object *local = stack_Pop(locals,vm->garbage);

				tuple_object *co_varnames = (tuple_object *) co->varnames;

				//char *varname = ((unicode_object*)co_varnames->list->items[i])->value;

				//if (debug && local->type == TYPE_UNICODE)
				//	printf("fast storing %s in %s\n", ((unicode_object *)local)->value,
				//		   varname);
				//if (debug && local->type == TYPE_INT)
				//	printf("fast storing %d in %s\n", local->ptr, varname);
				//if (debug && local->type == TYPE_CODE)
				//	printf("fast storing function %s in %s\n",
				//		   ((code_object *) local->ptr)->name, varname);
				
				if(GetItem(co_varnames,i)->type != TYPE_KV)
					{
						//printf("converting\n");
						SetItem(co_varnames,i,ConvertToKVObjectValued(GetItem(co_varnames,i),local));
					}
				else
				{
					SetDictItemByIndex(co_varnames,i,local);
				}
			/*	if(((kv_object*)co_varnames->list->items[i])->type != TYPE_KV)
					{
						co_varnames->list->items[i] = ConvertToKVObjectValued(co_varnames->list->items[i],local);
					}
				else
					((kv_object*)co_varnames->list->items[i])->value = local;*/
			}
		}

		//int i = 0;

		int has_extended_arg = 0;
		short extended_arg = 0;

		while (bo->ip < bo->len)
		{
			unsigned char op = (unsigned char)string[bo->ip++];	// get op and increment code pointer

			long arg = 0;		// holds the argument if present
			int op_thru = 0;	// used to skip unneccessary switches

			object *tos = NULL;
			object *tos1 = NULL;
			object *tos2 = NULL;

			// FOR DEBUGGING
			if (debug_level)
			{
				int old_i = bo->ip;
				int index = GetOpcodeIndex(op);
				if (index >= 0)
				{
					if (opcodes[index].argcount != 0)
						printf("[%d,%xh] opcode: [ %s ] (%d)\n", bo->ip,
							   opcodes[index].opcode, opcodes[index].name,
							   num_short(string[bo->ip], string[bo->ip + 1]));
					else
						printf("[%d,%xh] opcode: [ %s ]\n", bo->ip,
							   opcodes[index].opcode, opcodes[index].name);
				}
				else
				{
					printf("unknown opcode:%x at %d\n", (char)string[bo->ip - 1],
						   bo->ip - 1);
				}
			}
			// -- FOR DEBUGGING

			// printf("preparing opcode\n");
			// prepare opcode argument,increment codepointer and intepret
			// small opcodes not using an arg etc
			switch (op)
			{
			case OPCODE_NOP:
				op_thru = 1;
				break;

			case OPCODE_MAKE_FUNCTION:
				{
					bo->ip += 2;
					op_thru = 1;
				}
				break;

			case OPCODE_RETURN_VALUE:
				{
					object *ret = stack_Top(_stack);
					//IncRefCount(ret);
					//ret->flags |= OFLAG_LEFT_NAMESPACE;
					//ret->flags ^= OFLAG_ON_STACK;
					//ret = DissolveRef(ret);
					//	if(ret->type == TYPE_KV)
					//	{
					//		ret = ((kv_object*)ret)->value;
					//		((kv_object*)ret)->value = NULL;
					//	}
					//assert(ret != NULL);
					//ret->flags ^= OFLAG_ON_STACK;
					//ret = stack_Pop(_stack,vm->garbage);
					//object *t = stack_Pop(_stack);
					//if((t->flags & OFLAG_ON_STACK) > 0)
					//	printf("t on stack\n");
					if(debug_level > 1)
					{
					printf("ret object @:%x\n",ret);
					DumpObject(ret,0);
					}
					//ret = CopyObject(ret);
					//ret = CreateRefObject(ret,0);
					stack_Close(_stack, 0);
					stack_Pop(vm->blocks,vm->garbage);
					gc_Clear(vm->garbage);
					//assert(mem_free(bo));
					return (ret);
				}

			case OPCODE_STOP_CODE:
				{
					bo->ip = bo->len;
					op_thru = 1;
				}
				break;

			case OPCODE_POP_TOP:
				{
					object *pt = stack_Pop(_stack,vm->garbage);
					//FreeObject(pt);
					op_thru = 1;
				}
				break;

			case OPCODE_GET_ITER:
				{
					object *iter = stack_Pop(_stack,vm->garbage);
					if(debug_level >= 2)
					{
					 printf("iter:\n");
					 DumpObject(iter,0);
					//printf("bo_stack:\n");
					//stack_Dump(vm->blocks);
					//printf("bo:\n");
					//DumpObject(stack_Top(vm->blocks),0);
					}
					block_object *abo =
						(block_object *) stack_Top(vm->blocks);
					abo->iter = iter;
					//IncRefCount(iter);
					ResetIteration(iter);
					stack_Push(_stack,iter);
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
					block_object *bbo =
						(block_object *) stack_Top(vm->blocks);
					bo->ip = bbo->start + bbo->len - 1;
					// printf("break to: %d ,start: %d ,len:
					// %d\n",bo->ip,bbo->start,bbo->len);
					op_thru = 1;
				}
				break;

			case OPCODE_ROT_TWO:
				{
					tos = stack_Top(_stack);
					tos1 = stack_Second(_stack);
					stack_SetTop(_stack, tos1);
					stack_SetSecond(_stack, tos);
					op_thru = 1;
				}
				break;

			case OPCODE_ROT_THREE:
				{
					tos = stack_Top(_stack);
					tos1 = stack_Second(_stack);
					tos2 = stack_Third(_stack);
					stack_SetTop(_stack, tos1);
					stack_SetSecond(_stack, tos2);
					stack_SetThird(_stack, tos);
					op_thru = 1;
				}
				break;

			case OPCODE_DUP_TOP:
				{
					tos = stack_Top(_stack);
					stack_Push(_stack, tos);
					op_thru = 1;
				}
				break;

			case OPCODE_DUP_TOP_TWO:
				{
					tos = stack_Top(_stack);
					tos1 = stack_Second(_stack);
					stack_Adjust(_stack, 2);
					stack_SetTop(_stack, tos);
					stack_SetSecond(_stack, tos1);
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

				// case OPCODE_LOAD_DEREF:
				// case OPCODE_STORE_DEREF:
				// case OPCODE_STORE_MAP:
				// case OPCODE_STORE_SUBSCR:
				// case OPCODE_DELETE_SUBSCR:
				// case OPCODE_STORE_ATTR:
				// case OPCODE_DELETE_ATTR:
				// case OPCODE_LOAD_ATTR:
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
			//case OPCODE_MAKE_CLOSURE:
			//case OPCODE_LOAD_CLOSURE:
			case OPCODE_STORE_LOCALS:
			case OPCODE_PRINT_EXPR:
			case OPCODE_LOAD_BUILD_CLASS:
			case OPCODE_WITH_CLEANUP:
			case OPCODE_IMPORT_STAR:
			case OPCODE_END_FINALLY:
			case OPCODE_BUILD_CLASS:
			case OPCODE_COMPARE_OP:
			case OPCODE_IMPORT_FROM:
			case OPCODE_SETUP_EXCEPT:
			case OPCODE_SETUP_FINALLY:
			case OPCODE_RAISE_VARARGS:
				{
					if (has_extended_arg)
					{
						arg =
							num_long((extended_arg >> 8) & 0xFF,
									 extended_arg & 0xFF, string[bo->ip],
									 string[bo->ip + 1]);
						has_extended_arg = 0;
						extended_arg = 0;
					}
					else
						arg = (long)num_short(string[bo->ip], string[bo->ip + 1]);
					bo->ip += 2;
				}
				break;

			case OPCODE_UNARY_POSITIVE:
			case OPCODE_UNARY_NEGATIVE:
			case OPCODE_UNARY_NOT:
			case OPCODE_UNARY_INVERT:
			case OPCODE_YIELD_VALUE:
				break;
			}

			if (op_thru)
				continue;

			// printf("fetching tos items\n");
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
			case OPCODE_STORE_FAST:
			case OPCODE_STORE_NAME:
			case OPCODE_STORE_GLOBAL:
					tos = stack_Pop(_stack,vm->garbage);
					tos = DissolveRef(tos);
					if(tos->type == TYPE_KV)
						tos = ((kv_object*)tos)->value;
					assert(tos != NULL);
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
				{
					tos = stack_Pop(_stack,vm->garbage);
					tos1 = stack_Pop(_stack,vm->garbage);
/*					if (tos->type != TYPE_NONE && tos->type != TYPE_NULL
						&& tos->type != TYPE_TRUE && tos->type != TYPE_FALSE
						&& tos->value_ptr != NULL)
						tos = (object *) tos->value_ptr;
					if (tos1->type != TYPE_NONE && tos1->type != TYPE_NULL
						&& tos1->type != TYPE_TRUE && tos1->type != TYPE_FALSE
						&& tos1->value_ptr != NULL)
						tos1 = (object *) tos1->value_ptr;
						*/
						tos = DissolveRef(tos);
						tos1 = DissolveRef(tos1);
						if(tos->type == TYPE_KV)
							tos = ((kv_object*)tos)->value;
						if(tos1->type == TYPE_KV)
							tos1 = ((kv_object*)tos1)->value;
					assert(tos != NULL);
					assert(tos1 != NULL);
				}
				break;
			case OPCODE_STORE_SUBSCR:
			case OPCODE_STORE_MAP:
				{
					if(debug_level >=3)
					    stack_Dump(_stack);
					tos = stack_Pop(_stack,vm->garbage);
					tos1 = stack_Pop(_stack,vm->garbage);
					tos2 = stack_Pop(_stack,vm->garbage);
						tos = DissolveRef(tos);
						tos1 = DissolveRef(tos1);
						tos2 = DissolveRef(tos2);
						if(tos->type == TYPE_KV)
							tos = ((kv_object*)tos)->value;
						if(tos1->type == TYPE_KV)
							tos1 = ((kv_object*)tos1)->value;
						if(tos2->type == TYPE_KV)
							tos2 = ((kv_object*)tos2)->value;

					/*					if (tos->type != TYPE_NONE && tos->type != TYPE_NULL
						&& tos->type != TYPE_TRUE && tos->type != TYPE_FALSE
						&& tos->value_ptr != NULL)
						tos = (object *) tos->value_ptr;
					//if(debug && tos1->value_ptr != NULL)
					//	printf("value ptr set\n");
					if (tos1->type != TYPE_NONE && tos1->type != TYPE_NULL
						&& tos1->type != TYPE_TRUE && tos1->type != TYPE_FALSE
						&& tos1->value_ptr != NULL)
						tos1 = (object *) tos1->value_ptr;
						
					if (tos2->type != TYPE_NONE && tos2->type != TYPE_NULL
						&& tos2->type != TYPE_TRUE && tos2->type != TYPE_FALSE
						&& tos2->value_ptr != NULL)
						tos2 = (object *) tos2->value_ptr;
					if(debug >= 2)
					    stack_Dump(_stack);
					*/
				}
				break;
			}
			if (debug_level > 2)
				printf("executing main ops\n");
			// execute remaining ops here
			switch (op)
			{
			case OPCODE_SETUP_LOOP:
				{
					//object *bo = AllocObject();

					block_object *block = AllocBlockObject();

					//bo->type = TYPE_BLOCK;
					//bo->ptr = block;
					//bo->flags = OFLAG_ON_STACK;
					block->start = bo->ip;
					block->len = arg;
					block->ref_count = 0;
					block->type = TYPE_BLOCK;
					block->iter = NULL;
					// printf("block - start: %d, len:
					// %d\n",block->start,block->len);
					stack_Push(vm->blocks, block);
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
			case OPCODE_FOR_ITER:
				{
					// printf("for_iter delta jump\n");
					// i = i + delta;
					// break;
					block_object *fabo =
						(block_object *) stack_Top(vm->blocks);
					object *next = GetNextItem(fabo->iter);

					if (next != NULL)
					{
						// if(next->value_ptr != NULL)
						// next = next->value_ptr;
						// printf("next pushed:");
						// PrintObject(next);
						// stack_Dump(_stack);
						stack_Push(_stack, next);
						// printf("\n");
					}
					else
					{
						// printf("for_iter delta jump\n");
						bo->ip = bo->ip + arg;
						//block_object *rob = stack_Pop(vm->blocks,vm->garbage);
					}
				}
				break;
			case OPCODE_BUILD_MAP:
				{
					tuple_object *to =CreateTuple(0,OFLAG_ON_STACK | OFLAG_IS_DICT);
					stack_Push(_stack,to);
				}
				break;
			case OPCODE_BUILD_TUPLE:
			case OPCODE_BUILD_LIST:
				{
					// stack_Push(BuildList(_stack,arg),_stack);
					stack *blcall = NULL;

					if (arg > 0)
						blcall = stack_Init();	// arg
					tos = NULL;
					for (int i = 0; i < arg; i++)
					{
						stack_Push(blcall, stack_Pop(_stack,vm->garbage));
					}
					object *tmp =
						vm_ExecuteCFunction(vm, "list", blcall);
					if (tmp != NULL)
					{
						stack_Push(_stack, tmp);
						//DecRefCount(tmp);
					}
					if (arg > 0)
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
					
					if (debug_level>1)
						printf("searching for:%s\n", name);
					int index = GetItemIndexByName(global->names,name);
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
					if (vm_FindFunction(vm, name) != NULL)
						{
							lgo = CreateUnicodeObject(str_Copy(name),OFLAG_ON_STACK);
						}
						else
						{
							printf("global not found\n");
							break;
						}
					}
					stack_Push(_stack, lgo);
					if(debug_level >1)
						DumpObject(lgo,1);
					// stack_Dump(_stack);
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
					if(debug_level > 1)
					{
						printf("name:%s\n",name);
						DumpObject(tos,0);
					}
					
					int index = GetItemIndexByName(global->names,name);
					if(index != -1)
					{
					if(((object*)((tuple_object*)global->names)->list->items[index])->type != TYPE_KV)
					{
						((tuple_object*)global->names)->list->items[index] = ConvertToKVObjectValued(((tuple_object*)global->names)->list->items[index],tos);
						IncRefCount(((tuple_object*)global->names)->list->items[index]);
					}
					else	
					{
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
						IncRefCount(((tuple_object*)global->varnames)->list->items[index]);
					}
					else
					{
							((kv_object*)((tuple_object*)global->varnames)->list->items[index])->value = tos;
							IncRefCount(tos);
					}
					break;
					}					
				if(debug_level > 1)
					printf("global not found\n");
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

					int index = GetItemIndexByName(global->names,name);
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
					tuple_object *co_names = (tuple_object *) co->names;
					object *name = (object*)co_names->list->items[arg];
					if(name->type == TYPE_KV)
						name = ((kv_object*)name)->value;
					stack_Push(_stack, name);
					if(debug_level >1)
						DumpObject(name,1);
				}
				break;

			case OPCODE_LOAD_CONST:
				{
					tuple_object *co_consts = (tuple_object *) co->consts;
					object *lconst = (object*)co_consts->list->items[arg];
					if(lconst->type == TYPE_KV)
						lconst = ((kv_object*)lconst)->value;
					//lconst = CreateRefObject(lconst,lconst->flags | OFLAG_ON_STACK); 
					//printf("const:\n");
					//DumpObject(lconst,0);
					stack_Push(_stack, lconst);
					if(debug_level >1)
						DumpObject(lconst,1);
				}
				break;

			case OPCODE_LOAD_FAST:
				{
					tuple_object *co_varnames = (tuple_object *) co->varnames;
					object *fast = (object*)co_varnames->list->items[arg];
					if(fast->type == TYPE_KV)
						fast = ((kv_object*)fast)->value;
					//fast = CreateRefObject(fast,fast->flags | OFLAG_ON_STACK); 
					stack_Push(_stack, fast);
					if(debug_level >1)
						DumpObject(fast,1);
				}
				break;

/*				
		case OPCODE_MAP_ADD:
				{
					// printf("push name\n");
					co_varnames = (tuple_object *) co->varnames->ptr;
					// varname = (char *) co_varnames->items[arg]->ptr;
					// printf("opcode: [%s],(%d)
					// [%s]\n",opcodes[index].name,name_i,name->content);
					// if(co_varnames->items[varname_i]->type== TYPE_UNICODE)
					// printf("pushing local fast (%d,s)%s =
					// %s\n",varname_i,varname->content,AsUnicodeObject((object*)co_varnames->items[varname_i]->value_ptr)->content);
					// if(co_varnames->items[varname_i]->type== TYPE_INT)
					// printf("pushing local fast (%d,i)%s =
					// %d\n",varname_i,varname->content,co_varnames->items[varname_i]->ptr);
					stack_Push(_stack, ((object*)co_varnames->list->items[arg])->value_ptr);
				}
				break;
	*/			
	
				
			case OPCODE_STORE_NAME:
				{
					tuple_object *co_names = (tuple_object *) co->names;
					//if(tos->type == TYPE_CODE)
						//tos = CreateRefObject(tos,0);
					//else
						//tos = CopyObject(tos);
					if(GetItem(co_names,arg)->type != TYPE_KV)
					{
						SetItem(co_names,arg,ConvertToKVObjectValued(GetItem(co_names,arg),tos));
					}
				else
				{
					SetDictItemByIndex(co_names,arg,tos);
				}
				if(debug_level >1)
					DumpObject(tos,1);
				}
				break;

			case OPCODE_STORE_MAP:
				{
					SetDictItem(tos2,tos,tos1);
					stack_Push(_stack,tos2);
				}
				break;
				
			case OPCODE_STORE_FAST:
				{
					tuple_object *co_varnames = (tuple_object*) co->varnames;
					if(GetItem(co_varnames,arg)->type != TYPE_KV)
					{
						SetItem(co_varnames,arg,ConvertToKVObjectValued(GetItem(co_varnames,arg),tos));
					}
				else
				{
					SetDictItemByIndex(co_varnames,arg,tos);
				}
					if(debug_level >1)
						DumpObject(tos,1);
				}
				break;

			case OPCODE_DELETE_NAME:
				DeleteItem(co->names, arg);
				break;

			case OPCODE_DELETE_FAST:
				DeleteItem(co->varnames, arg);
				break;

			case OPCODE_DELETE_SUBSCR:
				{
					long dsa = 0;

					if (tos->type == TYPE_INT)
					{
						dsa = ((int_object*)tos)->value;
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

			case OPCODE_UNPACK_SEQUENCE:
				{
					for (int i = arg-1; i >= 0; i--)
					{
						switch (tos->type)
						{
						case TYPE_UNICODE:
							{
								unicode_object *uso = CreateUnicodeObject(str_FromChar(((unicode_object*)tos)->value[i]),OFLAG_ON_STACK);
								stack_Push(_stack, uso);
							}
						}
					}
					if(debug_level>1)
						stack_Dump(_stack);
				}
				break;

			case OPCODE_UNARY_POSITIVE:
				{
					if (tos->type == TYPE_INT)
					{
						int_object *up = CreateIntObject(+((int_object*)tos)->value,OFLAG_ON_STACK);
						stack_Push(_stack, up);
					}
				}
				break;

			case OPCODE_UNARY_NEGATIVE:
				{
					if (tos->type == TYPE_INT)
					{
						int_object *up = CreateIntObject(-((int_object*)tos)->value,OFLAG_ON_STACK);
						stack_Push(_stack, up);
					}
				}
				break;

			case OPCODE_UNARY_NOT:
				{
					if (tos->type == TYPE_INT)
					{
						object *up = CreateEmptyObject(((int_object*)tos)->value > 0 ? TYPE_TRUE : TYPE_FALSE,OFLAG_ON_STACK);
						stack_Push(_stack, up);
					}
					else if (tos->type == TYPE_TRUE || tos->type == TYPE_FALSE)
					{
						object *up = CreateEmptyObject(tos->type == TYPE_TRUE ? TYPE_TRUE : TYPE_FALSE,OFLAG_ON_STACK);
						stack_Push(_stack, up);
					}
					else if (tos->type == TYPE_NONE || tos->type == TYPE_NULL)
					{
						object *up = CreateEmptyObject( TYPE_FALSE,OFLAG_ON_STACK);
						stack_Push(_stack, up);
					}
				}
				break;

			case OPCODE_UNARY_INVERT:
				{
					if (tos->type == TYPE_INT)
					{
						int_object *up = CreateIntObject(-((((int_object*)tos)->value) + 1),OFLAG_ON_STACK);
						stack_Push(_stack, up);
					}
				}
				break;

			case OPCODE_BINARY_ADD:
				{
					if (tos->type == TYPE_UNICODE
						&& tos1->type == TYPE_UNICODE)
					{
						char *tos_tmp = ((unicode_object *)tos)->value;
						char *tos1_tmp = ((unicode_object *)tos1)->value;
						char *tmp = str_Cat(tos1_tmp, tos_tmp);
						unicode_object *new_tos = CreateUnicodeObject(tmp,OFLAG_ON_STACK);
						stack_Push(_stack, new_tos);
					}
					else
					{
						long a = 0;
						long b = 0;

						if (tos->type == TYPE_INT)
						{
							a = ((int_object*)tos)->value;
						}
						if (tos1->type == TYPE_INT)
						{
							b = ((int_object*)tos1)->value;
						}
						int_object *new_tos = CreateIntObject(a + b,OFLAG_ON_STACK);
						
					if (debug_level)
						printf ("%d + %d = %d (%d)\n", a, b, a + b, new_tos->value);
						stack_Push(_stack, new_tos);
					}
				}
				break;

			case OPCODE_BINARY_SUBTRACT:
				{
					long a = 0;
					long b = 0;
					
					if (tos->type == TYPE_INT)
					{
						a = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						b = ((int_object*)tos1)->value;
					}
					int_object *new_tos = CreateIntObject(b - a, OFLAG_ON_STACK);
 
					if (debug_level)
						printf("%d - %d = %d (%d)\n", b, a, b - a,
							   new_tos->value);
					stack_Push(_stack, new_tos);
				}
				break;

			case OPCODE_BINARY_TRUE_DIVIDE:
				{
					long da = 0;
					long db = 0;

					if (tos->type == TYPE_INT)
					{
						da = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						db = ((int_object*)tos1)->value;
					}
					int_object *new_dtos = CreateIntObject(db / da,OFLAG_ON_STACK);// TODO catch divide by zero
					if (debug_level)
						printf("%d / %d = %d (%d)\n", db, da, db / da,  new_dtos->value);
					stack_Push(_stack, new_dtos);
				}
				break;

			case OPCODE_BINARY_FLOOR_DIVIDE:
				{
					long fda = 0;
					long fdb = 0;

					if (tos->type == TYPE_INT)
					{
						fda = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						fdb = ((int_object*)tos1)->value;
					}
					int_object *new_fdtos = CreateIntObject(fdb / fda,OFLAG_ON_STACK);// TODO catch divide by zero
					if (debug_level)
						printf("%d / %d = %d (%d)\n", fdb, fda, fdb / fda, new_fdtos->value);
					stack_Push(_stack, new_fdtos);
				}
				break;

			case OPCODE_BINARY_MODULO:
				{
					long mda = 0;
					long mdb = 0;

					if (tos->type == TYPE_INT)
					{
						mda = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						mdb = ((int_object*)tos1)->value;
					}
					int_object *new_mdtos = CreateIntObject(mdb % mda,OFLAG_ON_STACK);

					if (debug_level)
						printf("%d %% %d = %d\n", mdb, mda, new_mdtos->value);
					stack_Push(_stack, new_mdtos);
				}
				break;

			case OPCODE_BINARY_RSHIFT:
				{
					long brsa = 0;
					long brsb = 0;

					if (tos->type == TYPE_INT)
					{
						brsa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						brsb = ((int_object*)tos1)->value;
					}
					int_object *new_brstos = CreateIntObject(brsb >> brsa,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d >> %d = %d\n", brsb, brsa, new_brstos->value);
					stack_Push(_stack, new_brstos);
				}
				break;

			case OPCODE_BINARY_LSHIFT:
				{
					long blsa = 0;
					long blsb = 0;

					if (tos->type == TYPE_INT)
					{
						blsa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						blsb = ((int_object*)tos1)->value;
					}
					int_object *new_blstos = CreateIntObject(blsb << blsa,OFLAG_ON_STACK);
					
					if (debug_level)
						printf("%d << %d = %d\n", blsb, blsa, new_blstos->value);
					stack_Push(_stack, new_blstos);
				}
				break;

			case OPCODE_BINARY_POWER:
				{
					long pa = 0;
					long pb = 0;

					if (tos->type == TYPE_INT)
					{
						pa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						pb = ((int_object*)tos1)->value;
					}
					int_object *new_ptos = CreateIntObject(long_pow(pb, pa),OFLAG_ON_STACK);
					if (debug_level)
						printf("%d ** %d = %d\n", pb, pa, new_ptos->value);
					stack_Push(_stack, new_ptos);
				}
				break;

			case OPCODE_BINARY_MULTIPLY:
				{
					long ma = 0;
					long mb = 0;

					if (tos->type == TYPE_INT)
					{
						ma = ((int_object*)tos)->value;
						if(debug_level > 1)
							printf("ma:%d\n",ma);
					}
					if (tos1->type == TYPE_TUPLE)
					{
						// printf("multiplying with a tuple\n");
						int mnum = ((tuple_object *) tos1)->list->num;
						if (mnum == 1)
							tos1 = ((tuple_object *) tos1)->list->items[0];
						stack *mstack = stack_Init();	// ma
						for (int i = 0; i < ma; i++)
						{
							stack_Push(mstack, tos1);
						}
						if(debug_level > 1)
						{
							stack_Dump(mstack);
							printf("mnum:%d\n",mnum);
						}
						object *mtr = if_list(vm,mstack);
						stack_Close(mstack,0);
						stack_Push(_stack, mtr);
						if(debug_level > 1)
							DumpObject(mtr,0);
						break;
					}
					else
					{
						if (tos1->type == TYPE_INT)
						{
							mb = ((int_object*)tos1)->value;
						}
						int_object *new_mtos = CreateIntObject(ma * mb,OFLAG_ON_STACK);
						// printf ("%d * %d = %d (%d)\n", ma, mb, ma * mb,
						// new_mtos->ptr);
						stack_Push(_stack, new_mtos);
					}
				}
				break;

			case OPCODE_STORE_SUBSCR:
				{
					long ssa = 0;
					//tos2 = CopyObject(tos2);
					//IncRefCount(tos2);
					if(debug_level >= 2)
					 {
					 printf("tos:%c\n",tos->type);
					 printf("tos1:%c\n",tos1->type);
					 }
					if (tos->type == TYPE_INT)
					{
						ssa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_TUPLE)
					{
						// DumpObject(tos2,0);
					if(debug_level >= 2)
						 printf("ssa: %d\n",ssa);
					if (tos->type == TYPE_INT)
						SetItem(tos1, ssa, tos2);
					else
						SetDictItem(tos1,tos,tos2);
						// DumpObject(tos1,0);
					if(debug_level >= 2)
						 DumpObject(tos1,0);
					}
				}
				break;

			case OPCODE_BINARY_SUBSCR:
				{
					long bsa = 0;

					// printf("binary subscribe");
					if (tos->type == TYPE_INT)
					{
						bsa = ((int_object*)tos)->value;
					}
				
					if (tos1->type == TYPE_TUPLE)
					{
						if (debug_level >= 2)
						{
							DumpObject(tos1, 0);
							DumpObject(tos,0);
							printf("bsa:%d\n", bsa);
						}
					object *bst= NULL;
					if (tos->type == TYPE_INT)
						bst = GetItem(tos1, bsa);
					else
						bst = GetDictItem(tos1,tos);
						
						if (debug_level>= 2)
							DumpObject(bst, 0);
						stack_Push(_stack, bst);
					}
				}
				break;

			case OPCODE_INPLACE_RSHIFT:
				{
					long ira = 0;
					long irb = 0;

					if (tos->type == TYPE_INT)
					{
						ira = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						irb = ((int_object*)tos1)->value;
					}
					int_object *new_irtos = CreateIntObject(irb >> ira,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d >> %d = %d\n", irb, ira, new_irtos->value);
					stack_Push(_stack, new_irtos);
				}
				break;

			case OPCODE_INPLACE_AND:
				{
					long iaa = 0;
					long iab = 0;

					if (tos->type == TYPE_INT)
					{
						iaa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						iab = ((int_object*)tos1)->value;
					}
					int_object *new_iatos = CreateIntObject(iab & iaa,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d & %d = %d\n", iab, iaa, new_iatos->value);
					stack_Push(_stack, new_iatos);
				}
				break;

			case OPCODE_INPLACE_XOR:
				{
					long ixa = 0;
					long ixb = 0;

					if (tos->type == TYPE_INT)
					{
						ixa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						ixb = ((int_object*)tos1)->value;
					}
					int_object *new_ixtos = CreateIntObject(ixb ^ ixa,OFLAG_ON_STACK);
					if(debug_level > 1)
						printf("%d ^ %d = %d\n", ixb, ixa, new_ixtos->value);
					stack_Push(_stack, new_ixtos);
				}
				break;

			case OPCODE_INPLACE_LSHIFT:
				{
					long ilsa = 0;
					long ilsb = 0;

					if (tos->type == TYPE_INT)
					{
						ilsa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						ilsb = ((int_object*)tos1)->value;
					}
					int_object *new_ilstos = CreateIntObject(ilsb << ilsa,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d << %d = %d\n", ilsb, ilsa, new_ilstos->value);
					stack_Push(_stack, new_ilstos);
				}
				break;

			case OPCODE_INPLACE_OR:
				{
					long ioa = 0;
					long iob = 0;

					if (tos->type == TYPE_INT)
					{
						ioa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						iob = ((int_object*)tos1)->value;
					}
					int_object *new_iotos = CreateIntObject(iob | ioa,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d | %d = %d\n", iob, ioa, new_iotos->value);
					stack_Push(_stack, new_iotos);
				}
				break;

			case OPCODE_INPLACE_ADD:
				{
					long iada = 0;
					long iadb = 0;

					if (tos->type == TYPE_INT)
					{
						iada = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						iadb = ((int_object*)tos1)->value;
					}
					int_object *new_iadtos = CreateIntObject(iadb + iada,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d + %d = %d\n", iadb, iada, new_iadtos->value);
					stack_Push(_stack, new_iadtos);
				}
				break;

			case OPCODE_INPLACE_MULTIPLY:
				{
					long ima = 0;
					long imb = 0;

					if (tos->type == TYPE_INT)
					{
						ima = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						imb = ((int_object*)tos1)->value;
					}
					int_object *new_imtos = CreateIntObject(imb * ima,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d * %d = %d\n", imb, ima, new_imtos->value);
					stack_Push(_stack, new_imtos);
				}
				break;

			case OPCODE_INPLACE_SUBTRACT:
				{
					long isa = 0;
					long isb = 0;

					if (tos->type == TYPE_INT)
					{
						isa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						isb = ((int_object*)tos1)->value;
					}
					int_object *new_istos = CreateIntObject(isb - isa,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d - %d = %d\n", isb, isa, new_istos->value);
					stack_Push(_stack, new_istos);
				}
				break;

			case OPCODE_INPLACE_MODULO:
				{
					long imoa = 0;
					long imob = 0;

					if (tos->type == TYPE_INT)
					{
						imoa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						imob = ((int_object*)tos1)->value;
					}
					int_object *new_imotos = CreateIntObject(imob % imoa,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d %% %d = %d\n", imob, imoa, new_imotos->value);
					stack_Push(_stack, new_imotos);
				}
				break;

			case OPCODE_INPLACE_TRUE_DIVIDE:
				{
					long itda = 0;
					long itdb = 0;

					if (tos->type == TYPE_INT)
					{
						itda = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						itdb = ((int_object*)tos1)->value;
					}
					int_object *new_itdtos = CreateIntObject(itdb / itda,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d / %d = %d\n", itdb, itda, new_itdtos->value);
					stack_Push(_stack, new_itdtos);
				}
				break;

			case OPCODE_INPLACE_FLOOR_DIVIDE:
				{
					long ifda = 0;
					long ifdb = 0;

					if (tos->type == TYPE_INT)
					{
						ifda = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						ifdb = ((int_object*)tos1)->value;
					}
					int_object *new_ifdtos = CreateIntObject(ifdb / ifda,OFLAG_ON_STACK);
					if (debug_level)
						printf("%d // %d = %d\n", ifdb, ifda, new_ifdtos->value);
					stack_Push(_stack, new_ifdtos);
				}
				break;

			case OPCODE_INPLACE_POWER:
				{
					long ifpa = 0;
					long ifpb = 0;

					if (tos->type == TYPE_INT)
					{
						ifpa = ((int_object*)tos)->value;
					}
					if (tos1->type == TYPE_INT)
					{
						ifpb = ((int_object*)tos1)->value;
					}
					int_object *new_ifptos = CreateIntObject(long_pow(ifpb, ifpa),OFLAG_ON_STACK);
					if (debug_level)
						printf("%d ** %d = %d\n", ifpb, ifpa, new_ifptos->value);
					stack_Push(_stack, new_ifptos);
				}
				break;

			case OPCODE_COMPARE_OP:
				{
					// printf("compare op:%d\n",arg);
					switch (arg)
					{
					case 0:	// <
						{
						long ca = 0;
						long cb = 0;

						if (tos->type == TYPE_INT)
						{
							ca = ((int_object*)tos)->value;
						}
						if (tos1->type == TYPE_INT)
						{
							cb = ((int_object*)tos1)->value;
						}
						empty_object *new_ctos = CreateEmptyObject(cb < ca ? TYPE_TRUE : TYPE_FALSE,OFLAG_ON_STACK);
						// printf ("%d < %d == %c\n", cb, ca, new_ctos->type);
						stack_Push(_stack, new_ctos);
						}
						break;
					case 4:	// >
						{
						long ca = 0;
						long cb = 0;

						if (tos->type == TYPE_INT)
						{
							ca = ((int_object*)tos)->value;
						}
						if (tos1->type == TYPE_INT)
						{
							cb = ((int_object*)tos1)->value;
						}
						empty_object *new_ctos = CreateEmptyObject( cb > ca ? TYPE_TRUE : TYPE_FALSE,OFLAG_ON_STACK);
						// printf ("%d < %d == %c\n", cb, ca, new_ctos->type);
						stack_Push(_stack, new_ctos);
						}
						break;
					case 2:	// ==
						// printf("cmp2\n");
						// printf("tos:%c\n",tos->type);
						// printf("tos1:%c\n",tos1->type);
						if ((tos->type == TYPE_FALSE || tos->type == TYPE_TRUE)
							&& (tos1->type == TYPE_FALSE
								|| tos1->type == TYPE_TRUE))
						{
							empty_object *new_ctos = CreateEmptyObject(tos1->type == tos->type ? TYPE_TRUE : TYPE_FALSE,OFLAG_ON_STACK);
							// printf ("%c == %c == %c\n", tos1->type,
							// tos->type, new_ctos->type);
							stack_Push(_stack, new_ctos);

						}
						else if (tos->type == TYPE_UNICODE
								 && tos1->type == TYPE_UNICODE)
						{
							char *tos_tmp = ((unicode_object*)tos)->value;
							char *tos1_tmp = ((unicode_object*)tos1)->value;

							empty_object *new_ctos = CreateEmptyObject(!strcmp(tos_tmp,tos1_tmp) ? TYPE_TRUE : TYPE_FALSE,OFLAG_ON_STACK);
							// printf ("%s == %s == %c\n", tos_tmp, tos1_tmp,
							// new_ctos->type);
							stack_Push(_stack, new_ctos);
						}
						else
						{
							long ca = 0;
							long cb = 0;

							if (tos->type == TYPE_INT)
							{
								ca = ((int_object*)tos)->value;
							}
							if (tos1->type == TYPE_INT)
							{
								cb = ((int_object*)tos1)->value;
							}
							empty_object *new_ctos = CreateEmptyObject(cb == ca ? TYPE_TRUE : TYPE_FALSE,OFLAG_ON_STACK);
							
							if (debug_level)
								printf("%d == %d == %c\n", cb, ca, new_ctos->type);
							stack_Push(_stack, new_ctos);
						}
						break;

					default:
						if (debug_level)
							printf("compare op:%d not supported\n", arg);
					}
					// printf("compare op thru:%d\n",arg);
				}
				break;

				/* case OPCODE_PRINT_ITEM: tos = stack_Pop(_stack); if(tos !=
				   NULL) switch(tos->type) { case TYPE_INT:
				   printf("%d",(long)tos->ptr); break; case TYPE_UNICODE:
				   if((object*)tos->value_ptr != NULL) {
				   if(((object*)tos->value_ptr)->type == TYPE_UNICODE)
				   printf("%s",(char*)((object*)tos->value_ptr)->ptr ); else
				   if(((object*)tos->value_ptr)->type == TYPE_INT)
				   printf("%d",((object*)tos->value_ptr)->ptr ); } else
				   printf("%s",(char*)tos->ptr); break; default:
				   printf("object on tos (type:%c) not supported for
				   printing\n",tos->type); }

				   break; */

			case OPCODE_CALL_FUNCTION:
				{
					stack *call = NULL;

					if (arg > 0)
						call = stack_Init();	// arg, 
					tos = NULL;
					for (int i = 0; i < arg; i++)
					{
						stack_Push(call, stack_Pop(_stack,vm->garbage));
					}
					object *function_name = stack_Pop(_stack,vm->garbage);
					if(function_name->type == TYPE_KV)
						function_name = ((kv_object*)function_name)->value;
					function_name = DissolveRef(function_name);
					
					if (function_name != NULL
						&& function_name->type == TYPE_CODE)
					{
						if (debug_level>1)
							printf
								("executing direct local function object: %s\n",
								 ((code_object *) function_name)->name);
		
						object *ret = vm_RunObject(vm, (object *) function_name, obj, call, arg);	// ,global

						if (ret != NULL)
						{
							//ret->flags ^= OFLAG_LEFT_NAMESPACE;
							stack_Push(_stack, ret);
							DecRefCountGC(ret,vm->garbage);
						}
						if (arg > 0)
							stack_Close(call, 0);
						break;
					}

					if (function_name != NULL
						&& function_name->type == TYPE_UNICODE)
					{
						if (debug_level>1)
							printf("executing C function: %s\n",  ((unicode_object*)function_name)->value);
						object *tmp = vm_ExecuteCFunction(vm,
														  (char *)
														  ((unicode_object*)function_name)->value,
														  call);

						if (tmp != NULL)
						{
							stack_Push(_stack, tmp);
							//DecRefCount(tmp);
							if (arg > 0)
								stack_Close(call, 0);

							break;
						}
						else
						{
							object *caller_func = NULL;
							int index = GetItemIndexByName(global->names,((unicode_object*)function_name)->value);
							if(index != -1)
							 caller_func = GetItem(global->names,index);
							
							if (debug_level>1 && caller_func == NULL)
								printf("global function not found\n");
							if (caller_func != NULL && ((object *) caller_func)->type == TYPE_CODE)
							{
								if (debug_level>1)
									printf	("executing global function object: %s\n", ((unicode_object*)function_name)->value);
								object *ret = vm_RunObject(vm, caller_func, obj, call, arg);

								if (ret != NULL)
								{
									stack_Push(_stack, ret);
									DecRefCountGC(ret,vm->garbage);
								}
								if (arg > 0)
									stack_Close(call, 0);
								break;
							}
						}
						if (debug_level)
							printf("function: [%s] not found\n",
								   ((unicode_object*)function_name)->value);
					}
				}
				break;

			default:
				// printf("\n[%d,%xh] opcode: [ %s ] ,argcount:%d not
				// supported\n(%s)\n",old_i,op,opcodes[index].name,opcodes[index].argcount,opcodes[index].description);
				break;
			}

			// printf("step\n");
			gc_Clear(vm->garbage);
		} //END OF WHILE LOOP
		if (debug_level)
			printf("code thru.\n");
		object *ret = stack_Pop(_stack,vm->garbage);
		stack_Close(_stack, 0);
		stack_Pop(vm->blocks,vm->garbage);
		//assert(mem_free(bo));

		return (ret);
	}

	return (NULL);
}

#ifdef CREATELIB

int main(int argc, char *argv[])
{

}

#endif
