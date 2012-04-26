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


#ifndef GARBAGE_H
#define GARBAGE_H

#include "features.h"
#include "types.h"
#include "lists.h"
#include "object.h"
#include "debug.h"
//#include "vm.h"

#ifdef __cplusplus
extern "C"  {
#endif

struct _object;

struct _vm;

struct _tuple_object;

struct _class_instance_object;

struct _object *vm_RunMethod(struct _vm *vm,struct _object *key,struct _class_instance_object *cio,struct _tuple_object *locals,struct _tuple_object *kw_locals);

void gc_FreeObject(struct _object *obj);

void gc_IncRefCount(struct _object *obj);

void gc_DecRefCount(struct _object *obj);

BOOL gc_HasNoRefs(struct _object *obj);

BOOL gc_HasRefs(struct _object *obj);

void gc_Init(struct _vm *vm);

void gc_Close(void);

void gc_Clear(void);

#ifdef __cplusplus
} 
#endif


#endif
