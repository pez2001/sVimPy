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

function_definition *
CreateCFunction (object * (*func) (stack * stack), char *name)
{
  function_definition *fd =
    mem_malloc (sizeof (function_definition), "CreateCFunction() fd");
  fd->type = FUNC_C;
  fd->func.func = func;
  fd->name = str_Copy (name);
  return (fd);
}

function_definition *
CreateCObjFunction (object * (*func) (object * obj), char *name)
{
  function_definition *fd =
    mem_malloc (sizeof (function_definition), "CreateCObjFunction() fd");
  fd->type = FUNC_C_OBJ;
  fd->func.func_obj = (*func);
  fd->name = str_Copy (name);
  return (fd);
}

function_definition *
CreatePythonFunction (object * code, char *name)
{
  function_definition *fd =
    mem_malloc (sizeof (function_definition), "CreatePythonFunction() fd");
  fd->type = FUNC_PYTHON;
  fd->func.code = code;
  fd->name = str_Copy (name);
  return (fd);
}

int
vm_AddFunctionDefinition (vm * vm, function_definition * fd)
{
  ptr_Push (vm->functions, fd);
  return (vm->functions->num - 1);
}

void
vm_RemoveFunction (vm * vm, char *name)
{
  for (int i = 0; i < vm->functions->num; i++)
    {
      if (!strcmp
	  (((function_definition *) vm->functions->items[i])->name, name))
	{
	  FreeFunctionDefinition ((function_definition *) vm->
				  functions->items[i]);
	  ptr_Remove (vm->functions, i);
	}
    }
}

void
vm_RemoveFunctionDefinition (vm * vm, function_definition * fd)
{
  for (int i = 0; i < vm->functions->num; i++)
    {
      if (vm->functions->items[i] == fd)
	{
	  FreeFunctionDefinition (fd);
	  ptr_Remove (vm->functions, i);
	}
    }
}

object *
vm_ExecuteCFunction (vm * vm, char *name, stack * stack)
{
  function_definition *fd = vm_FindFunction (vm, name);

  if (fd != NULL)
    {
      //printf("fd found:%s\n",fd->name);
      return ((*fd->func.func) (stack));

    }
}

object *
vm_ExecuteCObjFunction (vm * vm, char *name, object * obj)
{
  function_definition *fd = vm_FindFunction (vm, name);

  if (fd != NULL)
    {
      return ((*fd->func.func_obj) (obj));
    }
}

function_definition *
vm_FindFunction (vm * vm, char *name)
{
  for (int i = 0; i < vm->functions->num; i++)
    {
      if (!strcmp
	  (((function_definition *) vm->functions->items[i])->name, name))
	return ((function_definition *) (vm->functions->items[i]));
    }
  return (NULL);
}

void
FreeFunctionDefinition (function_definition * fd)
{
  assert (mem_free (fd->name));
  assert (mem_free (fd));
}

vm *
vm_Init (code_object * co)
{
  vm *tmp = (vm *) mem_malloc (sizeof (vm), "vm_Init() return");

  tmp->functions = ptr_CreateList (0);
  tmp->recycle = stack_Init (90, NULL);
  tmp->blocks = stack_Init (10, tmp->recycle);

  if (co != NULL)
    {
      block_object *bo = AllocBlockObject ();

      bo->code = co;
      bo->ip = 0;
      bo->iter = NULL;
      bo->start = 0;
      bo->len = 0;
      stack_Push (bo, tmp->blocks);
    }
  return (tmp);
}

void
vm_Close (vm * vm)
{
  if (vm->functions->num)
    {
      for (int i = 0; i < vm->functions->num; i++)
	FreeFunctionDefinition ((function_definition *) vm->
				functions->items[i]);
    }
  ptr_CloseList (vm->functions);
//stack_Close(vm->blocks,0);
  stack_Close (vm->recycle, 1);
  //stack_Dump(vm->blocks);
  stack_Close (vm->blocks, 0);
  assert (mem_free (vm));
}

void
vm_SetGlobal (vm * vm, code_object * co)
{
  block_object *bo = AllocBlockObject ();

  bo->code = co;
  bo->ip = 0;
  bo->iter = NULL;
  bo->start = 0;
  bo->len = 0;
  if (vm->blocks->top)
    stack_SetBottom (bo, vm->blocks);
  else
    stack_Push (bo, vm->blocks);


}

object *
vm_RunObject (vm * vm, object * obj, object * caller, stack * locals, int argc)	//
{
  if (obj->type == TYPE_CODE)
    {
      //printf("showing bytecodes\n");
      code_object *co = (code_object *) obj->ptr;

      //printf("co_ptr=%x\n",(unsigned long)co);
      //printf("stack size:%d\n",co->stacksize);
      object *global = ((block_object *) stack_Bottom (vm->blocks))->code;

      stack *_stack = stack_Init (co->stacksize, vm->recycle);

      string_object *bytecodes = (string_object *) co->code->ptr;

      char *string = bytecodes->content;

      long n = bytecodes->len;

      if (argc > 0 && locals != NULL)
	{
	  for (int i = 0; i < argc; i++)
	    {
	      //printf("adding local %d\n",i);
	      object *local = stack_Pop (locals);

	      tuple_object *co_varnames = (tuple_object *) co->varnames->ptr;

	      char *varname = (char *) co_varnames->items[i]->ptr;

	      if (local->type == TYPE_UNICODE)
		printf ("fast storing %s in %s\n", (char *) local->ptr,
			varname);
	      if (local->type == TYPE_INT)
		printf ("fast storing %d in %s\n", local->ptr, varname);
	      if (local->type == TYPE_CODE)
		printf ("fast storing function %s in %s\n",
			((code_object *) local->ptr)->name, varname);
	      co_varnames->items[i]->value_ptr = local;
	    }
	}

      int i = 0;

      int has_extended_arg = 0;

      short extended_arg = 0;

      while (i < n)
	{
	  unsigned char op = (unsigned char) string[i++];	//get op and increment code pointer

	  long arg = 0;		//holds the argument if present

	  int op_thru = 0;	//used to skip unneccessary switches

	  object *tos = NULL;

	  object *tos1 = NULL;

	  object *tos2 = NULL;

	  //FOR DEBUGGING
	  
	     int old_i = i;
	     int index = GetOpcodeIndex(op);
	     if(index >=0)
	     {
	     if(opcodes[index].argcount!=0)
	     printf("[%d,%xh] opcode: [ %s ] (%d)\n",i,opcodes[index].opcode,opcodes[index].name,num_short(string[i],string[i+1]));
	     else
	     printf("[%d,%xh] opcode: [ %s ]\n",i,opcodes[index].opcode,opcodes[index].name);
	     }else
	     {
	     printf("unknown opcode:%x at %d\n",(char)string[i-1],i-1);
	     }
	  
	  // -- FOR DEBUGGING

	  tuple_object *co_consts;

	  tuple_object *co_names;

	  tuple_object *co_varnames;

	  char *name;

	  char *varname;

	  char *const_content;


	  //prepare opcode argument,increment codepointer  and intepret small opcodes not using an arg etc
	  switch (op)
	    {
		case OPCODE_NOP:
		  op_thru = 1;
		  break;

		case OPCODE_MAKE_FUNCTION:
		  i += 2;
		  op_thru = 1;
		  break;

		case OPCODE_RETURN_VALUE:
		  {
		    object *ret = stack_Pop (_stack);

		    stack_Close (_stack, 0);
		    return (ret);
		  }

		case OPCODE_STOP_CODE:
		  i = n;
		  op_thru = 1;
		  break;

		case OPCODE_POP_TOP:
		  stack_Pop (_stack);
		  op_thru = 1;
		  break;

		case OPCODE_GET_ITER:
		  {
		    object *iter = stack_Pop (_stack);

		    block_object *abo =
		      (block_object *) stack_Top (vm->blocks)->ptr;
		    abo->iter = iter;
		    ResetIteration (iter);
		    op_thru = 1;
		  }
		  break;

		case OPCODE_POP_BLOCK:
		  stack_Pop (vm->blocks);
		  op_thru = 1;
		  break;

		case OPCODE_BREAK_LOOP:
		  {
		    block_object *bbo =
		      (block_object *) stack_Top (vm->blocks)->ptr;
		    i = bbo->start + bbo->len - 1;
		    //printf("break to: %d ,start: %d ,len: %d\n",i,bbo->start,bbo->len);
		    op_thru = 1;
		  }
		  break;


		case OPCODE_ROT_TWO:
		  tos = stack_Top (_stack);
		  tos1 = stack_Second (_stack);
		  stack_SetTop (tos1, _stack);
		  stack_SetSecond (tos, _stack);
		  op_thru = 1;
		  break;

		case OPCODE_ROT_THREE:
		  tos = stack_Top (_stack);
		  tos1 = stack_Second (_stack);
		  tos2 = stack_Third (_stack);
		  stack_SetTop (tos1, _stack);
		  stack_SetSecond (tos2, _stack);
		  stack_SetThird (tos, _stack);
		  op_thru = 1;
		  break;

		case OPCODE_DUP_TOP:
		  tos = stack_Top (_stack);
		  stack_Push (tos, _stack);
		  op_thru = 1;
		  break;

		case OPCODE_DUP_TOP_TWO:
		  tos = stack_Top (_stack);
		  tos1 = stack_Second (_stack);
		  stack_Adjust (2, _stack);
		  stack_SetTop (tos, _stack);
		  stack_SetSecond (tos1, _stack);
		  op_thru = 1;
		  break;

		case OPCODE_EXTENDED_ARG:
		  extended_arg = num_short (string[i + 1], string[i + 2]);
		  i += 2;
		  op_thru = 1;
		  has_extended_arg = 1;
		  break;

		  //case OPCODE_LOAD_DEREF:
		  //case OPCODE_STORE_DEREF:
		  //case OPCODE_STORE_MAP:
		  //case OPCODE_STORE_SUBSCR:
		  //case OPCODE_DELETE_SUBSCR:
		  //case OPCODE_STORE_ATTR:
		  //case OPCODE_DELETE_ATTR:
		  //case OPCODE_LOAD_ATTR:
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
		case OPCODE_MAKE_CLOSURE:
		case OPCODE_LOAD_CLOSURE:
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
		  if (has_extended_arg)
		    {
		      arg =
			num_long ((extended_arg >> 8) & 0xFF,
				  extended_arg & 0xFF, string[i],
				  string[i + 1]);
		      has_extended_arg = 0;
		      extended_arg = 0;
		    }
		  else
		    arg = (long) num_short (string[i], string[i + 1]);
		  i += 2;
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

	  //fetch tos items 
	  switch (op)
	    {
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
		case OPCODE_STORE_SUBSCR:
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
		  tos = stack_Pop (_stack);
		  tos1 = stack_Pop (_stack);
		    if (tos->type != TYPE_NONE && tos->type != TYPE_NULL && tos->type != TYPE_TRUE
			&& tos->type != TYPE_FALSE && tos->value_ptr != NULL)
				tos = (object *) tos->value_ptr;
		    if (tos1->type != TYPE_NONE && tos1->type != TYPE_NULL && tos1->type != TYPE_TRUE
			&& tos1->type != TYPE_FALSE && tos1->value_ptr != NULL)
				tos1 = (object *) tos1->value_ptr;
		  break;

	    }
	  //execute remaining ops here
	  switch (op)
	    {
		case OPCODE_SETUP_LOOP:
		  {
		    object *bo = AllocObject ();

		    block_object *block = AllocBlockObject ();

		    bo->type = TYPE_BLOCK;
		    bo->ptr = block;
		    bo->flags = OFLAG_ON_STACK;
		    block->start = i;
		    block->len = arg;
		    //printf("block - start: %d, len: %d\n",block->start,block->len);
		    stack_Push (bo, vm->blocks);
		  }
		  break;

		case OPCODE_POP_JUMP_IF_FALSE:
		  {
		    tos = stack_Pop (_stack);
		    if (tos->type == TYPE_FALSE)
		      {
			i = arg - 3;
		      }
		  }
		  break;
		case OPCODE_POP_JUMP_IF_TRUE:
		  {
		    tos = stack_Pop (_stack);
		    if (tos->type == TYPE_TRUE)
		      {
			i = arg - 3;
		      }
		  }
		  break;
		case OPCODE_JUMP_FORWARD:
		  {
		    i = i + arg;
		  }
		  break;
		case OPCODE_JUMP_ABSOLUTE:
		  {
		    i = arg ;
		  }
		  break;
		case OPCODE_JUMP_IF_FALSE:
		  {
		    tos = stack_Top (_stack);
		    if (tos->type == TYPE_FALSE)
		      {
			i = arg - 3;
		      }
		  }
		  break;
		case OPCODE_JUMP_IF_TRUE:
		  {
		    tos = stack_Top (_stack);
		    if (tos->type == TYPE_TRUE)
		      {
			i = arg - 3;
		      }
		  }
		  break;

		case OPCODE_FOR_ITER:
		  {
		    //              printf("for_iter delta jump\n");
		    //              i = i + delta;
		    //              break;
		    block_object *fabo =
		      (block_object *) stack_Top (vm->blocks)->ptr;
		    object *next = GetNextItem (fabo->iter);

		    if (next != NULL)
		      {
			//if(next->value_ptr != NULL)
			//      next = next->value_ptr;
			//printf("next pushed:");
			//PrintObject(next);
			stack_Push (next, _stack);
			//printf("\n");
		      }
		    else
		      {
			//printf("for_iter delta jump\n");
			i = i + arg;
		      }
		  }
		  break;

		case OPCODE_BUILD_TUPLE:
		case OPCODE_BUILD_LIST:
		  {
		//stack_Push(BuildList(_stack,arg),_stack);
		  stack *blcall = NULL;

		  if (arg > 0)
		    blcall = stack_Init (arg, vm->recycle);
		  tos = NULL;
		  for (int i = 0; i < arg; i++)
		    {
		      stack_Push (stack_Pop (_stack), blcall);
		    }
		  object *tmp =
		    vm_ExecuteCFunction (vm, "internal.BuildList", blcall);
		  if (tmp != NULL)
		    {
		      stack_Push (tmp, _stack);
		    }
		  if (arg > 0)
		    stack_Close (blcall, 0);
			}
		  break;
		case OPCODE_LOAD_GLOBAL:
		   {
		  //printf("push global name\n");
		  //code_object *co_global = (code_object*)global->ptr;

		  co_names = (tuple_object *) co->names->ptr;
		  code_object *co_global = (code_object *) global->ptr;

		  //co_names = (tuple_object*)co->names->ptr;
		  name = (char *) co_names->items[arg]->ptr;
		  object *lgo =
		    FindUnicodeTupleItem (co_global->varnames, name);
		  //DumpObject(lgo,1);
		  if (lgo == NULL)
		    if (vm_FindFunction (vm, name) != NULL)
		      {
			lgo = AllocObject ();
			lgo->type = TYPE_UNICODE;
			lgo->flags = OFLAG_ON_STACK;
			lgo->ptr = str_Copy (name);
			lgo->value_ptr = NULL;
		      }
		  stack_Push (lgo, _stack);
		  //stack_Dump(_stack);
		  //if(co_names->items[name_i]->type== TYPE_UNICODE)
		  // printf("pushing global (%d)%s = %s\n",name_i,name->content,AsUnicodeObject(co_names->items[name_i])->content);
		  //if(co_names->items[name_i]->type== TYPE_INT)
		  // printf("pushing global (%d)%s = %d\n",name_i,name->content,co_names->items[name_i]->ptr);
		  //printf("loading global\n");
		  //DumpObject(co_names->items[name_i],0);
		  //printf("pushed global\n");
		  //stack_Push(co_varnames->items[arg],_stack);
		  }
		  break;

		case OPCODE_STORE_GLOBAL:
		  {
		  tos = stack_Pop (_stack);
		  co_names = (tuple_object *) co->names->ptr;
		  code_object *co_global = (code_object *) global->ptr;

		  //co_names = (tuple_object*)co->names->ptr;
		  name = (char *) co_names->items[arg]->ptr;
		  object *sgo =
		    FindUnicodeTupleItem (co_global->varnames, name);

		  if (tos->type == TYPE_UNICODE)
		    printf ("fast storing %s in %s\n", (char *) tos->ptr,
			    (char *) sgo->ptr);
		  if (tos->type == TYPE_INT)
		    printf ("fast storing %d in %s\n", tos->ptr,
			    (char *) sgo->ptr);
		  if (tos->type == TYPE_CODE)
		    printf ("fast storing function %s in %s\n",
			    ((code_object *) tos->ptr)->name,
			    (char *) sgo->ptr);
		  //co_varnames->items[arg]->value_ptr = tos;
		  sgo->value_ptr = tos;
		  }
		  break;

		case OPCODE_DELETE_GLOBAL:
		  {
		  co_names = (tuple_object *) co->names->ptr;
		  code_object *co_global = (code_object *) global->ptr;

		  name = (char *) co_names->items[arg]->ptr;
		  DeleteItem (co_global->varnames, arg);
		  }
		  break;

		case OPCODE_LOAD_NAME:
		  {
		  //printf("push name\n");
		  co_names = (tuple_object *) co->names->ptr;
		  //name = (char *) co_names->items[arg]->ptr;
		  //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
		  //if(co_names->items[name_i]->type== TYPE_UNICODE)
		  // printf("pushing local (%d)%s = %s\n",name_i,name->content,AsUnicodeObject(co_names->items[name_i])->content);
		  //if(co_names->items[name_i]->type== TYPE_INT)
		  // printf("pushing local (%d)%s = %d\n",name_i,name->content,co_names->items[name_i]->ptr);
		  stack_Push (co_names->items[arg], _stack);
		    }
		  break;

		case OPCODE_LOAD_CONST:
		  {
		  //printf("push const\n");
		  co_consts = (tuple_object *) co->consts->ptr;
		  //const_content = (char *) co_consts->items[arg]->ptr;
		  //if(((object*)co_consts->items[const_i]->value_ptr)->type== TYPE_UNICODE)
		  //printf("pushing local const (%d)%s = %s\n",const_i,const_content->content,AsUnicodeObject((object*)co_consts->items[const_i]->value_ptr)->content);
		  //if(co_consts->items[const_i]->type== TYPE_INT)
		  // printf("pushing local const (%d)%s = %d\n",const_i,const_content->content,co_consts->items[const_i]->ptr);
		  stack_Push (co_consts->items[arg], _stack);
		  }
		  break;

		case OPCODE_LOAD_FAST:
		  {
		  //printf("push name\n");
		  co_varnames = (tuple_object *) co->varnames->ptr;
		  //varname = (char *) co_varnames->items[arg]->ptr;
		  //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
		  //if(co_varnames->items[varname_i]->type== TYPE_UNICODE)
		  // printf("pushing local fast (%d,s)%s = %s\n",varname_i,varname->content,AsUnicodeObject((object*)co_varnames->items[varname_i]->value_ptr)->content);
		  //if(co_varnames->items[varname_i]->type== TYPE_INT)
		  // printf("pushing local fast (%d,i)%s = %d\n",varname_i,varname->content,co_varnames->items[varname_i]->ptr);
		  stack_Push (co_varnames->items[arg]->value_ptr, _stack);
		  }
		  break;

		case OPCODE_STORE_NAME:
		  {
		  //printf("storing in name\n");
		  tos = stack_Pop (_stack);
		  co_names = (tuple_object *) co->names->ptr;
		  name = (char *) co_names->items[arg]->ptr;
		  //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
		  if (tos->type == TYPE_UNICODE)
		    printf ("storing %s in %s\n", (char *) tos->ptr, name);
		  if (tos->type == TYPE_INT)
		    printf ("storing %d in %s\n", tos->ptr, name);
		  if (tos->type == TYPE_CODE)
		    {
		      //co_names = (tuple_object*)AsCodeObject(global)->names->ptr;
		      //name_i = num_short(string[i+1],string[i+2]);
		      //name = (unicode_object*)co_names->items[name_i]->ptr;
		      printf ("storing function %s in %s\n",
			      ((code_object *) tos->ptr)->name, name);
		      //co_names->items[name_i]->value_ptr = tos;

		    }
		  //else 
		  co_names->items[arg]->value_ptr = tos;
		  }
		  break;

		case OPCODE_STORE_FAST:
		  {
		  //printf("storing in fast\n");
		  tos = stack_Pop (_stack);
		  //printf("tos type:%c\n",tos->type);
		  co_varnames = (tuple_object *) co->varnames->ptr;
		  varname = (char *) co_varnames->items[arg]->ptr;
		  //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
		  if (tos->type == TYPE_UNICODE)
		    printf ("fast storing %s in %s\n", (char *) tos->ptr,
			    varname);
		  if (tos->type == TYPE_INT)
		    printf ("fast storing %d in %s\n", tos->ptr, varname);
		  if (tos->type == TYPE_CODE)
		    printf ("fast storing function %s in %s\n",
			    ((code_object *) tos->ptr)->name, varname);
		  tos->flags ^= OFLAG_ON_STACK;
		  co_varnames->items[arg]->value_ptr = tos;
		  }
		  break;

		case OPCODE_DELETE_NAME:
		  DeleteItem (co->names, arg);
		  break;

		case OPCODE_DELETE_FAST:
		  DeleteItem (co->varnames, arg);
		  break;

		case OPCODE_DELETE_SUBSCR:
		  {
		    long dsa = 0;

		    if (tos->type == TYPE_INT)
		      {
			dsa = tos->ptr;
		      }
		    if (tos1->type == TYPE_TUPLE)
		      {
			//DumpObject(tos2,0);
			//printf("ssa: %d\n",ssa);
			DeleteItem (tos1, dsa);
			DumpObject (tos1, 0);
		      }
		  }
		  break;

		case OPCODE_UNPACK_SEQUENCE:
		  {
		  tos = stack_Pop (_stack);
		  if (tos->value_ptr != NULL)
		    tos = (object *) tos->value_ptr;
		  for (int i = 0; i < arg; i++)
		    {
		      switch (tos->type)
			{
			    case TYPE_UNICODE:
			      {
				object *uso = AllocObject ();

				uso->flags = OFLAG_ON_STACK;
				uso->type = TYPE_UNICODE;
				uso->ptr =
				  str_FromChar (((char *) tos->ptr)[i]);
				stack_Push (uso, _stack);
			      }
			}
		    }
		  //stack_Dump(_stack);
		  }
		  break;

		case OPCODE_UNARY_POSITIVE:
		{
		  tos = stack_Pop (_stack);
		  if (tos->type == TYPE_INT)
		    {
		      object *up = AllocObject ();

		      up->type = TYPE_INT;
		      up->flags = OFLAG_ON_STACK;
		      up->value_ptr = NULL;
		      up->ptr = +(long) tos->ptr;
		      stack_Push (up, _stack);
		    }
		}
		  break;

		case OPCODE_UNARY_NEGATIVE:
		{
		  tos = stack_Pop (_stack);
		  if (tos->type == TYPE_INT)
		    {
		      object *up = AllocObject ();

		      up->flags = OFLAG_ON_STACK;
		      up->type = TYPE_INT;
		      up->value_ptr = NULL;
		      up->ptr = -(long) tos->ptr;
		      stack_Push (up, _stack);
		    }
		}
		  break;

		case OPCODE_UNARY_NOT:
		{
		  tos = stack_Pop (_stack);
		  if (tos->type == TYPE_INT)
		    {
		      object *up = AllocEmptyObject ();

		      up->flags = OFLAG_ON_STACK;
		      up->type = tos->ptr > 0 ? TYPE_TRUE : TYPE_FALSE;
		      stack_Push (up, _stack);
		    }
		  else if (tos->type == TYPE_TRUE || tos->type == TYPE_FALSE)
		    {
		      object *up = AllocEmptyObject ();

		      up->flags = OFLAG_ON_STACK;
		      up->type =
			tos->type == TYPE_TRUE ? TYPE_TRUE : TYPE_FALSE;
		      stack_Push (up, _stack);
		    }
		  else if (tos->type == TYPE_NONE || tos->type == TYPE_NULL)
		    {
		      object *up = AllocEmptyObject ();

		      up->flags = OFLAG_ON_STACK;
		      up->type = TYPE_FALSE;
		      stack_Push (up, _stack);
		    }
			 }
		  break;

		case OPCODE_UNARY_INVERT:
		 {
		  tos = stack_Pop (_stack);
		  if (tos->type == TYPE_INT)
		    {
		      object *up = AllocObject ();

		      up->flags = OFLAG_ON_STACK;
		      up->type = TYPE_INT;
		      up->value_ptr = NULL;
		      up->ptr = -(((long) tos->ptr) + 1);
		      stack_Push (up, _stack);
		    }
			}
		  break;


		case OPCODE_BINARY_ADD:
		{
		  if (tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE)
		    {
		      char *tos_tmp = (char *) tos->ptr;

		      char *tos1_tmp = (char *) tos1->ptr;

		      char *tmp = str_Cat (tos1_tmp, tos_tmp);

		      //printf("cat:[%s]\n",tmp);
		      object *new_tos = AllocObject ();

		      new_tos->flags = OFLAG_ON_STACK;
		      new_tos->type = TYPE_UNICODE;
		      new_tos->ptr = tmp;
		      new_tos->value_ptr = NULL;
		      stack_Push (new_tos, _stack);
		    }
		  else
		    {
		      long a = 0;

		      long b = 0;

		      object *new_tos = AllocObject ();

		      new_tos->flags = OFLAG_ON_STACK;
		      new_tos->type = TYPE_INT;
		      new_tos->ptr = 0;
		      new_tos->value_ptr = NULL;
		      if (tos->type == TYPE_INT)
			{
			  a = tos->ptr;
			}
		      if (tos1->type == TYPE_INT)
			{
			  b = tos1->ptr;
			}
		      new_tos->ptr = a + b;
		      printf ("%d + %d = %d (%d)\n", a, b, a + b,
			      new_tos->ptr);
		      stack_Push (new_tos, _stack);
		    }
			}
		  break;

		case OPCODE_BINARY_SUBTRACT:
		  {
		    long a = 0;

		    long b = 0;

		    object *new_tos = AllocObject ();

		    new_tos->flags = OFLAG_ON_STACK;
		    new_tos->type = TYPE_INT;
		    new_tos->ptr = 0;
		    new_tos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			a = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			b = tos1->ptr;
		      }
		    new_tos->ptr = b - a;
		    printf ("%d - %d = %d (%d)\n", b, a, b - a, new_tos->ptr);
		    stack_Push (new_tos, _stack);
		  }
		  break;

		case OPCODE_BINARY_TRUE_DIVIDE:
		  {
		    long da = 0;

		    long db = 0;

		    object *new_dtos = AllocObject ();

		    new_dtos->flags = OFLAG_ON_STACK;
		    new_dtos->type = TYPE_INT;
		    new_dtos->ptr = 0;
		    new_dtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			da = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			db = tos1->ptr;
		      }
		    new_dtos->ptr = (long) db / da;	//TODO catch divide by zero
		    printf ("%d / %d = %d (%d)\n", db, da, db / da,
			    new_dtos->ptr);
		    stack_Push (new_dtos, _stack);
		  }
		  break;

		case OPCODE_BINARY_FLOOR_DIVIDE:
		  {
		    long fda = 0;

		    long fdb = 0;

		    object *new_fdtos = AllocObject ();

		    new_fdtos->flags = OFLAG_ON_STACK;
		    new_fdtos->type = TYPE_INT;
		    new_fdtos->ptr = 0;
		    new_fdtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			fda = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			fdb = tos1->ptr;
		      }
		    new_fdtos->ptr = (long) fdb / fda;	//TODO catch divide by zero
		    printf ("%d / %d = %d (%d)\n", fdb, fda, fdb / fda,
			    new_fdtos->ptr);
		    stack_Push (new_fdtos, _stack);
		  }
		  break;

		case OPCODE_BINARY_MODULO:
		  {
		    long mda = 0;

		    long mdb = 0;

		    object *new_mdtos = AllocObject ();

		    new_mdtos->flags = OFLAG_ON_STACK;
		    new_mdtos->type = TYPE_INT;
		    new_mdtos->ptr = 0;
		    new_mdtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			mda = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			mdb = tos1->ptr;
		      }
		    new_mdtos->ptr = (long) mdb % mda;	//TODO catch divide by zero
		    printf ("%d %% %d = %d\n", mdb, mda, new_mdtos->ptr);
		    stack_Push (new_mdtos, _stack);
		  }
		  break;

		case OPCODE_BINARY_RSHIFT:
		  {
		    long brsa = 0;

		    long brsb = 0;

		    object *new_brstos = AllocObject ();

		    new_brstos->flags = OFLAG_ON_STACK;
		    new_brstos->type = TYPE_INT;
		    new_brstos->ptr = 0;
		    new_brstos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			brsa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			brsb = tos1->ptr;
		      }
		    new_brstos->ptr = (long) brsb >> brsa;
		    printf ("%d >> %d = %d\n", brsb, brsa, new_brstos->ptr);
		    stack_Push (new_brstos, _stack);
		  }
		  break;

		case OPCODE_BINARY_LSHIFT:
		  {
		    long blsa = 0;

		    long blsb = 0;

		    object *new_blstos = AllocObject ();

		    new_blstos->flags = OFLAG_ON_STACK;
		    new_blstos->type = TYPE_INT;
		    new_blstos->ptr = 0;
		    new_blstos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			blsa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			blsb = tos1->ptr;
		      }
		    new_blstos->ptr = (long) blsb << blsa;
		    printf ("%d << %d = %d\n", blsb, blsa, new_blstos->ptr);
		    stack_Push (new_blstos, _stack);
		  }
		  break;

		case OPCODE_BINARY_POWER:
		  {
		    long pa = 0;

		    long pb = 0;

		    object *new_ptos = AllocObject ();

		    new_ptos->flags = OFLAG_ON_STACK;
		    new_ptos->type = TYPE_INT;
		    new_ptos->ptr = 0;
		    new_ptos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			pa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			pb = tos1->ptr;
		      }
		    new_ptos->ptr = long_pow (pb, pa);	//TODO catch divide by zero
		    printf ("%d ** %d = %d\n", pb, pa, new_ptos->ptr);
		    stack_Push (new_ptos, _stack);
		  }
		  break;

		case OPCODE_BINARY_MULTIPLY:
		  {
		    long ma = 0;

		    long mb = 0;

		    object *new_mtos = AllocObject ();
			
			printf("tos:%c\n",tos->type);
			printf("tos1:%c\n",tos1->type);
		    new_mtos->flags = OFLAG_ON_STACK;
		    new_mtos->type = TYPE_INT;
		    new_mtos->ptr = 0;
		    new_mtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			ma = tos->ptr;
		      }
			if(tos1->type == TYPE_TUPLE)
				{
			printf("multiplying with a tuple\n");
			int mnum = ((tuple_object*)tos1->ptr)->num;
			if(mnum==1)
				tos1 = ((tuple_object*)tos1->ptr)->items[0];
			stack *mstack = stack_Init(ma,vm->recycle);
			for(int i=0;i<ma;i++)
				{
			stack_Push(tos1,mstack);
				}
			printf("mnum:%d\n",mnum);
			object *mtr = BuildList(mstack);
			stack_Push(mtr,_stack);
			//DumpObject(mtr,0);
			break;
				}
		    if (tos1->type == TYPE_INT)
		      {
			mb = tos1->ptr;
		      }
		    new_mtos->ptr = ma * mb;
		    printf ("%d * %d = %d (%d)\n", ma, mb, ma * mb,
			    new_mtos->ptr);
		    stack_Push (new_mtos, _stack);
		  }
		  break;

		case OPCODE_STORE_SUBSCR:
		  {
		    long ssa = 0;

		    if (tos->type == TYPE_INT)
		      {
			ssa = tos->ptr;
		      }
		    if (tos1->type == TYPE_TUPLE)
		      {
			//DumpObject(tos2,0);
			//printf("ssa: %d\n",ssa);
			SetItem (tos1, ssa, tos2);
			//DumpObject(tos1,0);
		      }
		  }
		  break;

		case OPCODE_BINARY_SUBSCR:
		  {
		    long bsa = 0;

		    if (tos->type == TYPE_INT)
		      {
			bsa = tos->ptr;
		      }
		    if (tos1->type == TYPE_TUPLE)
		      {
			//DumpObject(tos1,0);
			printf("bsa:%d\n",bsa);
			object *bst = GetItem (tos1, bsa);

			DumpObject(bst,0);
			stack_Push (bst, _stack);
		      }
		  }
		  break;

		case OPCODE_INPLACE_RSHIFT:
		  {
		    long ira = 0;

		    long irb = 0;

		    object *new_irtos = AllocObject ();

		    new_irtos->flags = OFLAG_ON_STACK;
		    new_irtos->type = TYPE_INT;
		    new_irtos->ptr = 0;
		    new_irtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			ira = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			irb = tos1->ptr;
		      }
		    new_irtos->ptr = irb >> ira;
		    printf ("%d >> %d = %d\n", irb, ira, new_irtos->ptr);
		    stack_Push (new_irtos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_AND:
		  {
		    long iaa = 0;

		    long iab = 0;

		    object *new_iatos = AllocObject ();

		    new_iatos->flags = OFLAG_ON_STACK;
		    new_iatos->type = TYPE_INT;
		    new_iatos->ptr = 0;
		    new_iatos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			iaa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			iab = tos1->ptr;
		      }
		    new_iatos->ptr = iab & iaa;
		    printf ("%d & %d = %d\n", iab, iaa, new_iatos->ptr);
		    stack_Push (new_iatos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_XOR:
		  {
		    long ixa = 0;

		    long ixb = 0;

		    object *new_ixtos = AllocObject ();

		    new_ixtos->flags = OFLAG_ON_STACK;
		    new_ixtos->type = TYPE_INT;
		    new_ixtos->ptr = 0;
		    new_ixtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			ixa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			ixb = tos1->ptr;
		      }
		    new_ixtos->ptr = ixb ^ ixa;
		    printf ("%d ^ %d = %d\n", ixb, ixa, new_ixtos->ptr);
		    stack_Push (new_ixtos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_LSHIFT:
		  {
		    long ilsa = 0;

		    long ilsb = 0;

		    object *new_ilstos = AllocObject ();

		    new_ilstos->flags = OFLAG_ON_STACK;
		    new_ilstos->type = TYPE_INT;
		    new_ilstos->ptr = 0;
		    new_ilstos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			ilsa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			ilsb = tos1->ptr;
		      }
		    new_ilstos->ptr = ilsb << ilsa;
		    printf ("%d << %d = %d\n", ilsb, ilsa, new_ilstos->ptr);
		    stack_Push (new_ilstos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_OR:
		  {
		    long ioa = 0;

		    long iob = 0;

		    object *new_iotos = AllocObject ();

		    new_iotos->flags = OFLAG_ON_STACK;
		    new_iotos->type = TYPE_INT;
		    new_iotos->ptr = 0;
		    new_iotos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			ioa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			iob = tos1->ptr;
		      }
		    new_iotos->ptr = iob | ioa;
		    printf ("%d | %d = %d\n", iob, ioa, new_iotos->ptr);
		    stack_Push (new_iotos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_ADD:
		  {
		    long iada = 0;

		    long iadb = 0;

		    object *new_iadtos = AllocObject ();

		    new_iadtos->flags = OFLAG_ON_STACK;
		    new_iadtos->type = TYPE_INT;
		    new_iadtos->ptr = 0;
		    new_iadtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			iada = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			iadb = tos1->ptr;
		      }
		    new_iadtos->ptr = iadb + iada;
		    printf ("%d + %d = %d\n", iadb, iada, new_iadtos->ptr);
		    stack_Push (new_iadtos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_MULTIPLY:
		  {
		    long ima = 0;

		    long imb = 0;

		    object *new_imtos = AllocObject ();

		    new_imtos->flags = OFLAG_ON_STACK;
		    new_imtos->type = TYPE_INT;
		    new_imtos->ptr = 0;
		    new_imtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			ima = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			imb = tos1->ptr;
		      }
		    new_imtos->ptr = imb * ima;
		    printf ("%d * %d = %d\n", imb, ima, new_imtos->ptr);
		    stack_Push (new_imtos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_SUBTRACT:
		  {
		    long isa = 0;

		    long isb = 0;

		    object *new_istos = AllocObject ();

		    new_istos->flags = OFLAG_ON_STACK;
		    new_istos->type = TYPE_INT;
		    new_istos->ptr = 0;
		    new_istos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			isa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			isb = tos1->ptr;
		      }
		    new_istos->ptr = isb - isa;
		    printf ("%d - %d = %d\n", isb, isa, new_istos->ptr);
		    stack_Push (new_istos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_MODULO:
		  {
		    long imoa = 0;

		    long imob = 0;

		    object *new_imotos = AllocObject ();

		    new_imotos->flags = OFLAG_ON_STACK;
		    new_imotos->type = TYPE_INT;
		    new_imotos->ptr = 0;
		    new_imotos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			imoa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			imob = tos1->ptr;
		      }
		    new_imotos->ptr = imob % imoa;
		    printf ("%d %% %d = %d\n", imob, imoa, new_imotos->ptr);
		    stack_Push (new_imotos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_TRUE_DIVIDE:
		  {
		    long itda = 0;

		    long itdb = 0;

		    object *new_itdtos = AllocObject ();

		    new_itdtos->flags = OFLAG_ON_STACK;
		    new_itdtos->type = TYPE_INT;
		    new_itdtos->ptr = 0;
		    new_itdtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			itda = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			itdb = tos1->ptr;
		      }
		    new_itdtos->ptr = itdb / itda;
		    printf ("%d / %d = %d\n", itdb, itda, new_itdtos->ptr);
		    stack_Push (new_itdtos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_FLOOR_DIVIDE:
		  {
		    long ifda = 0;

		    long ifdb = 0;

		    object *new_ifdtos = AllocObject ();

		    new_ifdtos->flags = OFLAG_ON_STACK;
		    new_ifdtos->type = TYPE_INT;
		    new_ifdtos->ptr = 0;
		    new_ifdtos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			ifda = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			ifdb = tos1->ptr;
		      }
		    new_ifdtos->ptr = ifdb / ifda;
		    printf ("%d // %d = %d\n", ifdb, ifda, new_ifdtos->ptr);
		    stack_Push (new_ifdtos, _stack);
		  }
		  break;

		case OPCODE_INPLACE_POWER:
		  {
		    long ifpa = 0;

		    long ifpb = 0;

		    object *new_ifptos = AllocObject ();

		    new_ifptos->flags = OFLAG_ON_STACK;
		    new_ifptos->type = TYPE_INT;
		    new_ifptos->ptr = 0;
		    new_ifptos->value_ptr = NULL;
		    if (tos->type == TYPE_INT)
		      {
			ifpa = tos->ptr;
		      }
		    if (tos1->type == TYPE_INT)
		      {
			ifpb = tos1->ptr;
		      }
		    new_ifptos->ptr = long_pow (ifpb, ifpa);
		    printf ("%d ** %d = %d\n", ifpb, ifpa, new_ifptos->ptr);
		    stack_Push (new_ifptos, _stack);
		  }
		  break;

		case OPCODE_COMPARE_OP:
		  printf("compare op:%d\n",arg);
		  switch (arg)
		    {
			case 0:	// <
			  printf ("");
			  long ca = 0;

			  long cb = 0;

			  object *new_ctos = AllocEmptyObject ();

			  new_ctos->flags = OFLAG_ON_STACK;
			  if (tos->type == TYPE_INT)
			    {
			      ca = tos->ptr;
			    }
			  if (tos1->type == TYPE_INT)
			    {
			      cb = tos1->ptr;
			    }
			  new_ctos->type = cb < ca ? TYPE_TRUE : TYPE_FALSE;
			  printf ("%d < %d == %c\n", cb, ca, new_ctos->type);
			  stack_Push (new_ctos, _stack);
			  break;
			case 2:	// ==
			printf("cmp2\n");
			printf("tos:%c\n",tos->type);
			  printf("tos1:%c\n",tos1->type);
			  if (tos->type == TYPE_UNICODE
			      && tos1->type == TYPE_UNICODE)
			    {
			      char *tos_tmp = (char *) tos->ptr;

			      char *tos1_tmp = (char *) tos1->ptr;

			      object *new_ctos = AllocEmptyObject ();

			      new_ctos->flags = OFLAG_ON_STACK;
			      new_ctos->type =
				!strcmp (tos_tmp,
					 tos1_tmp) ? TYPE_TRUE : TYPE_FALSE;
			      printf ("%s == %s == %c\n", tos_tmp, tos1_tmp,
				      new_ctos->type);
			      stack_Push (new_ctos, _stack);
			    }
			  else
			    {
			      long ca = 0;

			      long cb = 0;

			      object *new_ctos = AllocEmptyObject ();

			      new_ctos->flags = OFLAG_ON_STACK;
			      if (tos->type == TYPE_INT)
				{
				  ca = tos->ptr;
				}
			      if (tos1->type == TYPE_INT)
				{
				  cb = tos1->ptr;
				}
			      new_ctos->type =
				cb == ca ? TYPE_TRUE : TYPE_FALSE;
			      printf ("%d == %d == %c\n", cb, ca,
				      new_ctos->type);
			      stack_Push (new_ctos, _stack);
			    }
			  break;

			default:
			  printf ("compare op:%d not supported\n", arg);
		    }
		  printf("compare op thru:%d\n",arg);
		  break;

/*    case OPCODE_PRINT_ITEM:
		tos = stack_Pop(_stack);
				if(tos != NULL)
					switch(tos->type)
					{
						case TYPE_INT:
							printf("%d",(long)tos->ptr);
							break;
						case TYPE_UNICODE:
							if((object*)tos->value_ptr != NULL)
							{
								if(((object*)tos->value_ptr)->type == TYPE_UNICODE)
									printf("%s",(char*)((object*)tos->value_ptr)->ptr );
								else
									if(((object*)tos->value_ptr)->type == TYPE_INT)
										printf("%d",((object*)tos->value_ptr)->ptr );
							}
						else 
							printf("%s",(char*)tos->ptr);
							break;
						default:
							printf("object on tos (type:%c) not supported for printing\n",tos->type);
					}
		
		break;
*/

		case OPCODE_CALL_FUNCTION:
		  {
		    stack *call = NULL;

		    if (arg > 0)
		      call = stack_Init (arg, vm->recycle);
		    tos = NULL;
		    for (int i = 0; i < arg; i++)
		      {
			stack_Push (stack_Pop (_stack), call);
		      }
		    object *function_name = stack_Pop (_stack);

		    if (function_name != NULL
			&& function_name->type == TYPE_CODE)
		      {
			printf
			  ("executing direct local function object: %s\n",
			   ((code_object *) function_name->ptr)->name);
			for (int i = 0; i < arg; i++)
			  {
			    stack_Push (stack_Pop (call), _stack);
			  }
			object *ret = vm_RunObject (vm, (object *) function_name, obj, call, arg);	//,global

			if (ret != NULL)
			  stack_Push (ret, _stack);
			if (arg > 0)
			  stack_Close (call, 0);
			break;
		      }

		    if (function_name->value_ptr != NULL
			&& ((object *) function_name->value_ptr)->type ==
			TYPE_CODE)
		      {
			printf ("executing local function object: %s\n",
				(char *) function_name->ptr);
			for (int i = 0; i < arg; i++)
			  {
			    stack_Push (stack_Pop (call), _stack);
			  }
			object *ret = vm_RunObject (vm, (object *) function_name->value_ptr, obj, NULL, 0);	//,global

			if (ret != NULL)
			  stack_Push (ret, _stack);
			if (arg > 0)
			  stack_Close (call, 0);
			break;
		      }

		    if (function_name != NULL
			&& function_name->type == TYPE_UNICODE)
		      {
			printf ("executing C function: %s\n",
				(char *) function_name->ptr);
			object *tmp =
			  vm_ExecuteCFunction (vm,
					       (char *) function_name->ptr,
					       call);

			if (tmp != NULL)
			  {
			    stack_Push (tmp, _stack);
			    if (arg > 0)
			      stack_Close (call, 0);

			    break;
			  }
			else
			  {
			    printf ("searching %s\n",
				    (char *) function_name->ptr);
			    object *caller_func =
			      FindUnicodeTupleItem (AsCodeObject (global)->
						    names,
						    (char *) function_name->
						    ptr);
			    if (caller_func == NULL)
			      printf ("global function not found\n");
			    if (caller_func != NULL
				&& ((object *) caller_func->value_ptr)->
				type == TYPE_CODE)
			      {
				printf
				  ("executing global function object: %s\n",
				   (char *) function_name->ptr);
				object *ret = vm_RunObject (vm, (object *) caller_func->value_ptr, obj, call, arg);	//,global

				if (ret != NULL)
				  stack_Push (ret, _stack);
				if (arg > 0)
				  stack_Close (call, 0);
				break;
			      }
			  }
			printf ("function: [%s] not found\n",
				(char *) function_name->ptr);
		      }
		  }
		  break;

		default:
		  //printf("\n[%d,%xh] opcode: [ %s ] ,argcount:%d not supported\n(%s)\n",old_i,op,opcodes[index].name,opcodes[index].argcount,opcodes[index].description);
		  break;
	    }


	}
      printf ("code thru.\n");
      object *ret = stack_Pop (_stack);

      stack_Close (_stack, 0);
      return (ret);
    }

  return (NULL);
}
