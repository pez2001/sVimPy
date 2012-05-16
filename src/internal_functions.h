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

#include "features.h"


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

object *if_assert(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_build_class(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_open(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_range(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_print(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_sum(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_list(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_max(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_abs(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_len(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_pow(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_min(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_sorted(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_reversed(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_all(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_any(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_chr(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_ord(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_cmp(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_hex(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_int(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_float(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_iter(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_map(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

object *if_next(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);


object *BinaryOp(object *tos,object *tos1,unsigned char op);

object *CompareOp(object *tos,object *tos1,unsigned char cmp_op);

object *custom_code(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals);

void AddInternalFunctions(struct _vm *vm);

object *BinaryOp(object *tos,object *tos1,unsigned char op);

object *CompareOp(object *tos,object *tos1,unsigned char cmp_op);

#ifdef __cplusplus
} 
#endif


#endif
