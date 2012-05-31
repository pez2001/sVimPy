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


#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

#include "features.h"

#define OBJECT_TYPE char
#define OBJECT_FLAGS unsigned char
//#define OBJECT_REF_COUNT unsigned short

//stream 
#ifdef USE_ARDUINO_FUNCTIONS
#define STREAM_NUM unsigned short
#define OBJECT_REF_COUNT short
#define OBJECT_LOCK_COUNT unsigned char
// used in INDEXING situations
#define INDEX short 
//used in enum over array and total numbers
//#define NUM unsigned short
#define NUM short
//used in relative calcs
#define REL_NUM short
#else
#define STREAM_NUM long 
#define OBJECT_REF_COUNT long
#define OBJECT_LOCK_COUNT long
// used in INDEXING situations
#define INDEX long 
//used in enum over array and total numbers
//#define NUM unsigned short
#define NUM long
//used in relative calcs
#define REL_NUM long
#endif

#define STREAM_TYPE_ID char

//standard int size
#define INT long
//standard floating number size
#define FLOAT float


//used for boolean comparisons
#define BOOL unsigned char

#ifdef USE_DEBUGGING

void DumpTypes(void);

#endif

#endif
