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

#include "debug.h"

#ifdef DEBUGGING
void debug_printf(int msg_debug_level,char* format, ...)
{
	if(msg_debug_level != 0 && (debug_level & msg_debug_level) == 0)
	return;
	va_list va;
	va_start(va,format);
	//printf("getting len\n");
	int n = str_PrintfVaLen(format,va);
	va_end(va);
	//printf("getting string\n");
	va_start(va,format);
	char *output = str_PrintfVa(format,n,va);
	va_end(va);
	//printf("got string\n");
	printf(output);
	mem_free(output);
}

#endif

