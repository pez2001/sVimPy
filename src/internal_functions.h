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

struct _vm;

#ifdef USE_INTERNAL_FUNCTIONS

//OBJECT_ID if_assert(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_build_class(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_open(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_range(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_print(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_sum(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_list(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_max(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_abs(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_len(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_pow(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_min(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_sorted(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_reversed(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_all(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_any(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_chr(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_ord(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_cmp(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_hex(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_int(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_float(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_iter(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_map(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID if_next(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

OBJECT_ID custom_code(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals);

void AddArduinoGlobal(VM_ID vm);

void AddInternalFunctions(VM_ID vm);
#endif

OBJECT_ID BinaryOp(OBJECT_ID tos_id,OBJECT_ID tos1_id,unsigned char op);

OBJECT_ID CompareOp(OBJECT_ID tos_id,OBJECT_ID tos1_id,unsigned char cmp_op);

#ifdef __cplusplus
} 
#endif


#endif
