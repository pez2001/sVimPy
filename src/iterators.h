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

ITER_ID iter_CreateIter(OBJECT_ID iteration_id,VM_ID vm_id);

OBJECT_ID iter_Next(ITER_ID iter_id,VM_ID vm_id);

void iter_Expand(ITER_ID iter_id,VM_ID vm_id,STACK_ID stack_id);

TUPLE_ID iter_TupleExpand(ITER_ID iter_id,VM_ID vm_id);

void iter_ExpandTuple(ITER_ID iter_id,VM_ID vm_id,TUPLE_ID to_id);

OBJECT_ID iter_NextNow(ITER_ID iter_id,VM_ID vm_id);

OBJECT_ID iter_Sequence(ITER_ID iter_id,VM_ID vm_id);

void iter_InitSequence(ITER_ID iter_id,VM_ID vm_id,INDEX start,NUM end,NUM step);

OBJECT_ID iter_Generator(ITER_ID iter_id,VM_ID vm_id);

void iter_InitGenerator(ITER_ID iter_id,VM_ID vm_id,BLOCK_ID bo_id);

OBJECT_ID iter_CFGenerator(ITER_ID iter_id,VM_ID vm_id);

void iter_InitCFGenerator(ITER_ID iter_id,VM_ID vm_id,CFUNCTION_ID cfo_id);

OBJECT_ID iter_MGenerator(ITER_ID iter_id,VM_ID vm_id);

void iter_InitMGenerator(ITER_ID iter_id,VM_ID vm_id,METHOD_ID mo_id);

OBJECT_ID iter_Iteration(ITER_ID iter_id,VM_ID vm_id);

void iter_InitIteration(ITER_ID iter_id,VM_ID vm_id,TUPLE_ID to_id);

#ifdef __cplusplus
} 
#endif


#endif
