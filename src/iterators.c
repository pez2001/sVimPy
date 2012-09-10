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

#include "iterators.h"


ITER_ID iter_CreateIter(OBJECT_ID iteration_id,VM_ID vm_id)
{
	object *iteration = (object*)mem_lock(iteration_id);
	if(iteration->type == TYPE_KV)
	{//TODO remove here
		OBJECT_ID old  = iteration_id;
		iteration_id = ((kv_object*)iteration)->value;
		mem_unlock(old,0);
		iteration = (object*)mem_lock(iteration_id);
	}
	if(iteration->type == TYPE_TUPLE)
	{
		mem_unlock(iteration_id,0);
		OBJECT_ID iter = obj_CreateIter();
		iter_InitIteration(iter,vm_id,iteration_id);
		return(iter);
	}
	else if(iteration->type == TYPE_CFUNCTION)
	{
		mem_unlock(iteration_id,0);
		OBJECT_ID iter = obj_CreateIter();
		iter_InitCFGenerator(iter,vm_id,iteration_id);	
		return(iter);
	}
	else if(iteration->type == TYPE_METHOD)
	{
		mem_unlock(iteration_id,0);
		OBJECT_ID iter = obj_CreateIter();
		iter_InitMGenerator(iter,vm_id,iteration_id);	
		return(iter);
	}
	else if(iteration->type == TYPE_ITER)
	{
		mem_unlock(iteration_id,0);
		return(iteration_id);
	}
	else
	{
		mem_unlock(iteration_id,0);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"no appropriate iter found for obj @%d\n",iteration_id);
		#endif
		return(0);
	}
}

void iter_RestoreBlockStack(ITER_ID iter_id,VM_ID vm_id)
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	
	stack_Push(vm->blocks,iter->tag);
	while(!stack_IsEmpty(iter->block_stack))
	{
		OBJECT_ID bo = stack_Pop(iter->block_stack);
		stack_Push(vm->blocks,bo);//reverserd order
	}
	mem_unlock(iter_id,0);
	mem_unlock(vm_id,0);
}

void iter_ClearBlockStack(ITER_ID iter_id,VM_ID vm_id)
{
	//struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	while(!stack_IsEmpty(iter->block_stack))
	{
		//OBJECT_ID bo = stack_Pop(iter->block_stack);
		stack_Pop(iter->block_stack);
		//obj_DecRefCount(bo);
	}
	//mem_unlock(vm_id,0);
	mem_unlock(iter_id,0);
}

void iter_SaveBlockStack(ITER_ID iter_id,VM_ID vm_id)
{
	struct _vm *vm = (struct _vm*)mem_lock(vm_id);
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	OBJECT_ID bo = 0;
	while((bo = stack_Pop(vm->blocks)) != iter->tag)
	{
		stack_Push(iter->block_stack,bo);//reverserd order
	}
	mem_unlock(iter_id,0);
	mem_unlock(vm_id,0);
}

OBJECT_ID iter_Next(ITER_ID iter_id,VM_ID vm_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	OBJECT_ID next = iter->iter_func(iter_id,vm_id);
	mem_unlock(iter_id,0);
	return(next);
}

void iter_Expand(ITER_ID iter_id,VM_ID vm_id,STACK_ID stack_id)
{
	obj_IncRefCount(iter_id);
	OBJECT_ID n = 0;
	STACK_ID tmp = stack_Create();
	//BOOL found_none = 0;
	do
	{
		n = iter_NextNow(iter_id,vm_id);
		stack_Push(tmp,n);
	}while(obj_GetType(n) != TYPE_NONE);
	while(!stack_IsEmpty(tmp))
	{
		OBJECT_ID s = stack_Pop(tmp);
		stack_Push(stack_id,s);
	}
	stack_Close(tmp,0);
	obj_DecRefCount(iter_id);
}

TUPLE_ID iter_TupleExpand(ITER_ID iter_id,VM_ID vm_id)
{
	obj_IncRefCount(iter_id);
	OBJECT_ID n = 0;
	TUPLE_ID to = obj_CreateTuple(0);
	do
	{
		n = iter_NextNow(iter_id,vm_id);
		tuple_AppendItem(to,n);
	}while(obj_GetType(n) != TYPE_NONE);
	obj_DecRefCount(iter_id);
	return(to);
}

void iter_ExpandTuple(ITER_ID iter_id,VM_ID vm_id,TUPLE_ID to_id)
{
	obj_IncRefCount(iter_id);
	OBJECT_ID n = 0;
	do
	{
		n = iter_NextNow(iter_id,vm_id);
		tuple_AppendItem(to_id,n);
	}while(obj_GetType(n) != TYPE_NONE);
	obj_DecRefCount(iter_id);
}

OBJECT_ID iter_NextNow(ITER_ID iter_id,VM_ID vm_id)
{
	OBJECT_ID next = iter_Next(iter_id,vm_id);
	if(obj_GetType(next) == TYPE_BLOCK) //used only to differentiate between an object to return and a generator not finished
	{
		struct _vm *vm = (struct _vm*)mem_lock(vm_id);
		iter_object *iter = (iter_object*)mem_lock(iter_id);
		OBJECT_ID ret = 0;
		BLOCK_ID bo = iter->tag;
		iter_RestoreBlockStack(iter_id,vm_id);
		obj_IncRefCount(iter_id);
		
		while(ret == 0)//TODO doesnt stop after one block
		{
			ret = vm_Step(vm_id,0);
			if(!stack_Contains(vm->blocks,bo)) //via normal return 
			//TODO switch to integer comparison to block stack num with the block stack num at the beginning
			{
				BLOCK_ID n = stack_Top(vm->blocks);
				if(!stack_IsEmpty(vm->blocks))
				{
					block_object *bn = (block_object*)mem_lock(n);
					ret = stack_Pop(bn->stack);
					mem_unlock(n,0);
				}	
				iter_ClearBlockStack(iter_id,vm_id);
				break;
			}
			else if(ret != 0) //via yield 
			{
				iter_SaveBlockStack(iter_id,vm_id);
				obj_DecRefCount(ret);
			}
		}
		mem_unlock(iter_id,0);
		mem_unlock(vm_id,0);
		obj_DecRefCount(iter_id);
		return(ret);
	}
	obj_DecRefCount(next);
	return(next);
}

OBJECT_ID iter_Sequence(ITER_ID iter_id,VM_ID vm_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	TUPLE_ID seq = iter->tag;
	mem_unlock(iter_id,0);
	INT_ID pos_id = tuple_GetItem(seq,2);
	INT_ID step_id = tuple_GetItem(seq,1);
	INT_ID end_id = tuple_GetItem(seq,0);
	int_object *pos = (int_object*)mem_lock(pos_id);
	int_object *step = (int_object*)mem_lock(step_id);
	int_object *end= (int_object*)mem_lock(end_id);
	if((step->value>0  && pos->value < end->value) || (step->value<0  && pos->value > end->value) ) //absolute compare
	{
		INT_ID r = obj_CreateInt(pos->value);
		pos->value+=step->value;
		obj_IncRefCount(r);
		mem_unlock(end_id,0);
		mem_unlock(step_id,0);
		mem_unlock(pos_id,1);
		return(r);
	}
	else
	{
		INT_ID start_id = tuple_GetItem(seq,3);
		int_object *start = (int_object*)mem_lock(start_id);
		pos->value = start->value;
		OBJECT_ID r = obj_CreateEmpty( TYPE_NONE);
		obj_IncRefCount(r);
		mem_unlock(start_id,0);
		mem_unlock(end_id,0);
		mem_unlock(step_id,0);
		mem_unlock(pos_id,1);
		return(r);
	}
	mem_unlock(end_id,0);
	mem_unlock(step_id,0);
	mem_unlock(pos_id,1);
}

void iter_InitSequence(ITER_ID iter_id,VM_ID vm_id,INDEX start,NUM end,NUM step)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	TUPLE_ID seq = obj_CreateTuple(4);
	INT_ID iend = obj_CreateInt(end);
	tuple_SetItem(seq,0,iend);
	INT_ID istep = obj_CreateInt(step);
	tuple_SetItem(seq,1,istep);
	INT_ID ipos = obj_CreateInt(start);
	tuple_SetItem(seq,2,ipos);
	INT_ID istart = obj_CreateInt(start);
	tuple_SetItem(seq,3,istart);
	iter->tag = seq;
	obj_IncRefCount(seq);
	iter->block_stack = 0;
	iter->iter_func = &iter_Sequence;
	mem_unlock(iter_id,1);
}

OBJECT_ID iter_Generator(ITER_ID iter_id,VM_ID vm_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	BLOCK_ID bo_id = iter->tag;
	mem_unlock(iter_id,1);
	block_object *bo = (block_object*)mem_lock(bo_id);
	if(bo->ip < bo->len)
	{
		mem_unlock(bo_id,0);
		return(bo_id);
	}
	bo->ip = bo->start;//reset iterator for further uses
	mem_unlock(bo_id,1);
	OBJECT_ID r = obj_CreateEmpty(TYPE_NONE);
	obj_IncRefCount(r);
	return(r);
}

void iter_InitGenerator(ITER_ID iter_id,VM_ID vm_id,BLOCK_ID bo_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	iter->tag = bo_id;
	obj_IncRefCount(bo_id);
	block_object *bo = (block_object*)mem_lock(bo_id);
	bo->ip = bo->start;
	mem_unlock(bo_id,1);
	iter->block_stack = stack_Create();
	iter->iter_func = &iter_Generator;
	mem_unlock(iter_id,1);
}

OBJECT_ID iter_CFGenerator(ITER_ID iter_id,VM_ID vm_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	CFUNCTION_ID cfo = iter->tag;
	mem_unlock(iter_id,0);
	OBJECT_ID r = vm_StartCFunctionObject(vm_id,cfo,0,0);
	obj_IncRefCount(r);
	return(r);
}

void iter_InitCFGenerator(ITER_ID iter_id,VM_ID vm_id,CFUNCTION_ID cfo_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	iter->tag = cfo_id;
	obj_IncRefCount(cfo_id);
	iter->block_stack = stack_Create();
	iter->iter_func = &iter_CFGenerator;
	mem_unlock(iter_id,1);
}

OBJECT_ID iter_MGenerator(ITER_ID iter_id,VM_ID vm_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	METHOD_ID mo = iter->tag;
	mem_unlock(iter_id,0);
	OBJECT_ID r = vm_StartMethodObject(vm_id,mo,0,0);
	obj_IncRefCount(r);
	return(r);
}

void iter_InitMGenerator(ITER_ID iter_id,VM_ID vm_id,METHOD_ID mo_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	iter->tag = mo_id;
	obj_IncRefCount(mo_id);
	iter->block_stack = stack_Create();
	iter->iter_func = &iter_MGenerator;
	mem_unlock(iter_id,1);
}

OBJECT_ID iter_Iteration(ITER_ID iter_id,VM_ID vm_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	TUPLE_ID it = iter->tag;
	mem_unlock(iter_id,0);
	OBJECT_ID next = tuple_GetNextItem(it);
	if(next == 0 || obj_GetType(next) == TYPE_NONE)
	{
		tuple_ResetIteration(it);
		if(next == 0)
		{
			OBJECT_ID r = obj_CreateEmpty(TYPE_NONE);
			obj_IncRefCount(r);
			return(r);
		}
	}
	obj_IncRefCount(next);
	return(next);
}

void iter_InitIteration(ITER_ID iter_id,VM_ID vm_id,TUPLE_ID to_id)
{
	iter_object *iter = (iter_object*)mem_lock(iter_id);
	tuple_ResetIteration(to_id);
	iter->tag = to_id;
	obj_IncRefCount(to_id);
	iter->block_stack = 0;
	iter->iter_func = &iter_Iteration;
	mem_unlock(iter_id,1);
}


