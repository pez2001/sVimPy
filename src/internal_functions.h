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

#ifndef INTERNAL_FUNCTIONS_H
#define INTERNAL_FUNCTIONS_H

#include "object.h"
#include "stack.h"
#include "vm.h"
#include "debug.h"

// object *pow(object *base,object *exp);

object *if_range(vm *vm,stack * stack);

object *if_print(vm *vm,stack * stack);

object *if_sum(vm *vm,stack * stack);

object *if_list(vm *vm,stack * stack);

object *BinaryOp(object *tos,object *tos1,unsigned char op);

object *CompareOp(object *tos,object *tos1,unsigned char cmp_op);

object *custom_code(vm *vm,stack * stack);


object *iter_Sequence(iter_object *iter);

void iter_InitSequence(iter_object *iter,int start,int len,int step);



#endif
