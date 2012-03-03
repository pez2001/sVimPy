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

#ifndef STROPS_H
#define STROPS_H

#include "features.h"
#include "types.h"
#include "debug.h"
#include "memory.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifdef __cplusplus
extern "C"  {
#endif


char *str_Cat(char *a, char *b);

char *str_Copy(char *a);

char *str_Substring(char *a,INDEX start,NUM len);

char *str_FromChar(char c);

char *str_Printf(char *format, ...);

char *str_PrintfVa(char *format,NUM len,va_list va);

NUM str_PrintfVaLen(char *format,va_list va);


#ifdef __cplusplus
} 
#endif


#endif
