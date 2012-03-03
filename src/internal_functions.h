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

#include "types.h"
#include "object.h"
#include "stack.h"
#include "vm.h"
#include "debug.h"
#include "math.h"

#ifdef __cplusplus
extern "C"  {
#endif

// object *pow(object *base,object *exp);
struct _vm;

object *if_range(struct _vm *vm,stack * stack);

object *if_print(struct _vm *vm,stack * stack);

object *if_sum(struct _vm *vm,stack * stack);

object *if_list(struct _vm *vm,stack * stack);

object *if_next(struct _vm *vm,stack * stack);

object *BinaryOp(object *tos,object *tos1,unsigned char op);

object *CompareOp(object *tos,object *tos1,unsigned char cmp_op);

object *custom_code(struct _vm *vm,stack * stack);

void AddInternalFunctions(struct _vm *vm);

object *BinaryOp(object *tos,object *tos1,unsigned char op);

object *CompareOp(object *tos,object *tos1,unsigned char cmp_op);

#ifdef __cplusplus
} 
#endif


#endif
