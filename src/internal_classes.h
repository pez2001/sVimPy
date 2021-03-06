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

#ifndef INTERNAL_CLASSES_H
#define INTERNAL_CLASSES_H

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


#ifdef __cplusplus
} 
#endif

#ifdef USE_INTERNAL_CLASSES
void AddInternalClasses(VM_ID vm);
#endif
#endif
