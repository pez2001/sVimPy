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


#ifndef DEBUG_H
#define DEBUG_H

#include "stdarg.h"

#define DEBUG_MEMORY 1
#define DEBUG_SHOW_OPCODES 2
#define DEBUG_FULL_DUMP 4
#define DEBUG_STACK 8
#define DEBUG_LISTS 16
#define DEBUG_GC 32
#define DEBUG_VERBOSE_STEP 64
#define DEBUG_INTERNAL_FUNCTIONS 128
#define DEBUG_VM 256
#define DEBUG_FREEING 512
#define DEBUG_ALLOCS 1024
#define DEBUG_DUMP_UNSUPPORTED 2048
#define DEBUG_DUMP_OBJECT 4096
#define DEBUG_INTERACTIVE 8192
#define DEBUG_COUNT_OBJECTS 16384
#define DEBUG_CREATION 32768
#define DEBUG_VERBOSE_FREEING 65536
#define DEBUG_PTR_LISTS 131072
#define DEBUG_VERBOSE_TESTS 262144

void debug_printf(int debug_level,char* format, ...);

extern int debug_level;


#endif
