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

#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include "vm.h"
#include "object.h"
#include "memory.h"
#include "internal_functions.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern long mem_chunks_num;
extern long mem_chunks_actual_size;
extern long mem_chunks_max_size;

extern long objects_num;
extern long objects_max;
extern long objects_header_total;


#define MAGIC (3180 | ((long)'\r'<<16) | ((long)'\n'<<24))
#define TAG "cpython-32"
#define CACHEDIR "__pycache__"
/* Current magic word and string tag as globals. */
static long pyc_magic = MAGIC;
static const char *pyc_tag = TAG;
static short pyc_magic_short = 3180;




#endif
