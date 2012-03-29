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


#ifndef ITERATORS_H
#define ITERATORS_H

#include "types.h"
#include "object.h"
#include "vm.h"
#include "stack.h"

#ifdef __cplusplus
extern "C"  {
#endif

void iter_Expand(iter_object *iter,struct _vm *vm,stack *stack);

void iter_ExpandTuple(iter_object *iter,struct _vm *vm,tuple_object *to);

tuple_object *iter_TupleExpand(iter_object *iter,struct _vm *vm);

object *iter_NextNow(iter_object *iter,struct _vm *vm);

object *iter_Next(iter_object *iter,struct _vm *vm);

iter_object *iter_CreateIter(object *iteration,struct _vm *vm);//struct _vm *vm

object *iter_Sequence(iter_object *iter,struct _vm *vm);

void iter_InitSequence(iter_object *iter,struct _vm *vm,INDEX start,NUM end,NUM step);

object *iter_Generator(iter_object *iter,struct _vm *vm);

void iter_InitGenerator(iter_object *iter,struct _vm *vm,block_object *bo);

object *iter_CFGenerator(iter_object *iter,struct _vm *vm);

void iter_InitCFGenerator(iter_object *iter,struct _vm *vm,cfunction_object *cfo);

object *iter_Iteration(iter_object *iter,struct _vm *vm);

void iter_InitIteration(iter_object *iter,struct _vm *vm,tuple_object *to);

#ifdef __cplusplus
} 
#endif


#endif
