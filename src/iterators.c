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
iter_object *iter_CreateIter(object *iteration)
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

object *iter_Next(iter_object *iter)
{
	object *next = iter->iter_func(iter);
	return(next);
}

object *iter_NextNow(iter_object *iter,vm *vm)
{
	object *next = iter->iter_func(iter);
	if(next->type == TYPE_BLOCK)
	{
		object *ret = NULL;
		block_object *bo = (block_object*)iter->tag;
		//code_object *co = bo->code;
		stack_Push(vm->blocks, bo);
		while(ret == NULL)//TODO doesnt stop after one block
			ret = vm_StepObject(vm);
		return(ret);
	}
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

void iter_InitSequence(iter_object *iter,int start,int end,int step)
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
	block_object *bo = (block_object*)iter->tag;
	//code_object *co = bo->code;
	//stack_Push(vm->blocks, bo);
	if(bo->ip < bo->len)
		return(bo);
	object *r = CreateEmptyObject( TYPE_NONE,0);
	IncRefCount(r);
	return(r);
}

void iter_InitGenerator(iter_object *iter,block_object *bo)
{
	iter->tag = bo;
	IncRefCount(bo);
	iter->iter_func = &iter_Generator;
}

object *iter_Iteration(iter_object *iter)
{
	tuple_object *it = (tuple_object*)iter->tag;
	return(GetNextItem(it));
}

void iter_InitIteration(iter_object *iter,tuple_object *to)
{
	ResetIteration(to);
	iter->tag = to;
	IncRefCount(to);
	iter->iter_func = &iter_Iteration;
}


