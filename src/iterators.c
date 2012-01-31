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
iter_object *iter_CreateIter(object *iteration)//,struct _vm *vm
{
	if(iteration->type == TYPE_TUPLE)
	{
		iter_object *iter = CreateIterObject(0);
		iter_InitIteration(iter,iteration);
		return(iter);
	}
	else	if(iteration->type == TYPE_FUNCTION && ((function_object *)iteration)->func_type == FUNC_PYTHON && ( ((((function_object *)iteration)->func.code->co_flags & CO_GENERATOR)>0) || ((((function_object *)iteration)->func.code->co_flags & CO_NESTED)>0) ) )
	{
		iter_object *iter = CreateIterObject(0);
		iter_InitGenerator(iter,iteration);	
		return(iter);
	}
	else if(iteration->type == TYPE_ITER)
	{
		//printf("iteration already created\n");
		return(iteration);
	}
	else
	{
		printf("no appropriate iter found for obj @%x\n",iteration);
		return(NULL);
	}
}

void iter_RestoreBlockStack(iter_object *iter,struct _vm *vm)
{
	stack_Push(vm->blocks,iter->tag);
	while(iter->block_stack->list->num)
	{
		block_object *bo = stack_Pop(iter->block_stack,vm->garbage);
		stack_Push(vm->blocks,bo);//reverserd order
		//printf("restoring bo:%x\n",bo);
		//DumpObject(bo,0);
	}
}

void iter_SaveBlockStack(iter_object *iter,struct _vm *vm)
{
	block_object *bo = NULL;
	while((bo = stack_Pop(vm->blocks,vm->garbage)) != iter->tag)
	{
		//printf("saving bo:%x\n",bo);
		//DumpObject(bo,0);
		stack_Push(iter->block_stack,bo);//reverserd order
	}//while(bo != iter);
}

object *iter_Next(iter_object *iter,vm *vm)
{
	object *next = iter->iter_func(iter);
	return(next);
}

void iter_Expand(iter_object *iter,struct _vm *vm,stack *stack)
{
	IncRefCount(iter);
	object *n = NULL;
	//printf("expanding iter\n");
	struct _stack *tmp = stack_Init();
	do
	{
		n = iter_NextNow(iter,vm);
		//if(n!= NULL)
		//	printf("n:%c\n",n->type);
		//else 
		//	printf("n: NULL\n");
		stack_Push(tmp,n);
	}while(n != NULL && n->type != TYPE_NONE);
	//printf("pushing expanded items\n");
	while(tmp->list->num)
	{
		object *s = stack_Pop(tmp,vm->garbage);
		if(s->type != TYPE_NONE)
			stack_Push(stack,s);
	}
	stack_Close(tmp,0);
	DecRefCountGC(iter,vm->garbage);
	//printf("expanded iter\n");
}

object *iter_NextNow(iter_object *iter,vm *vm)
{
	//printf("nextNow()\n");
	object *next = iter->iter_func(iter);
	//printf("iter executed:%x\n",iter);
	if(next->type == TYPE_BLOCK) //used only to differentiate between an object to return and a generator not finished
	{
		object *ret = NULL;
		block_object *bo = (block_object*)iter->tag;
		//code_object *co = bo->code;
		
		//stack *saved_blocks = vm->blocks
		//vm->blocks = stack_Init();
		//stack_Push(vm->blocks, bo);
		//vm_DumpCode(vm,0,0);
		iter_RestoreBlockStack(iter,vm);
		//printf("restored block stack for iter\n");
		//printf("top:%x, bo:%x\n",stack_Top(vm->blocks),bo);
		
		while(ret == NULL)//TODO doesnt stop after one block
		{
			//printf("in next now loop\n");
			ret = vm_StepObject(vm);
			//block_object *n = (block_object*)stack_Top(vm->blocks);
			if(!stack_Contains(vm->blocks,bo)) //via normal return
			{
				//printf("block executed\n");
				//stack_Dump(n->stack);
				block_object *n = (block_object*)stack_Top(vm->blocks);
				if(vm->blocks->list->num)
					ret = stack_Pop(n->stack,vm->garbage);
					
				break;
			}
			else if(ret != NULL) //via yield 
			{
				//printf("saving block stack of iter\n");
				iter_SaveBlockStack(iter,vm);
				DecRefCountGC(ret,vm->garbage);
				//printf("decremented iter\n");
			}
		}
		//printf("ret object\n");
		//DumpObject(ret,0);
		return(ret);
	}
	//printf("iter thru next:\n");
	//DumpObject(next,0);
	return(next);
}

object *iter_Sequence(iter_object *iter)
{
	tuple_object *seq = (tuple_object*)iter->tag;
	int_object *pos = GetItem(seq,2);
	int_object *step = GetItem(seq,1);
	int_object *end = GetItem(seq,0);

	if(pos->value < end->value)
	{
		int_object *r = CreateIntObject(pos->value,0);
		pos->value+=step->value;
		return(r);
	}
	else
	{
		object *r = CreateEmptyObject( TYPE_NONE,0);
		IncRefCount(r);
		return(r);
	}
}

void iter_InitSequence(iter_object *iter,INDEX start,NUM end,NUM step)
{
	tuple_object *seq = CreateTuple(3,0);
	int_object *iend = CreateIntObject(end,0);
	SetItem(seq,0,iend);
	int_object *istep = CreateIntObject(step,0);
	SetItem(seq,1,istep);
	int_object *ipos = CreateIntObject(start,0);
	SetItem(seq,2,ipos);
	iter->tag = seq;
	IncRefCount(seq);
	iter->iter_func = &iter_Sequence;
}

object *iter_Generator(iter_object *iter)
{
	//printf("iter->ref_count:%d\n",iter->ref_count);
	block_object *bo = (block_object*)iter->tag;
	//code_object *co = bo->code;
	//stack_Push(vm->blocks, bo);
	//printf("checking if block has finished\n");
	if(bo->ip < bo->len)
		return(bo);
	//printf("finished returning empty object\n");	
	object *r = CreateEmptyObject( TYPE_NONE,0);
	IncRefCount(r);
	return(r);
}

void iter_InitGenerator(iter_object *iter,block_object *bo)
{
	iter->tag = bo;
	IncRefCount(bo);
	iter->block_stack = stack_Init();
	iter->iter_func = &iter_Generator;
}

object *iter_Iteration(iter_object *iter)
{
	tuple_object *it = (tuple_object*)iter->tag;
	object *next = GetNextItem(it);
	if(next == NULL || next->type == TYPE_NONE)
	{
		//printf("tuple iterated thru\n");
		object *r = CreateEmptyObject( TYPE_NONE,0);
		IncRefCount(r);
		return(r);
	}
	return(next);
}

void iter_InitIteration(iter_object *iter,tuple_object *to)
{
	ResetIteration(to);
	iter->tag = to;
	IncRefCount(to);
	iter->iter_func = &iter_Iteration;
}


