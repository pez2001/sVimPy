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

#include "strops.h"

BYTES_ID str_Cat(BYTES_ID a_id, BYTES_ID b_id)
{
	if(a_id == 0 && b_id != 0)
		return(str_Copy(b_id));
	else
		if(a_id != 0 && b_id == 0)
			return(str_Copy(a_id));
	else
		if(a_id == 0 && b_id == 0)
			return(0);
	char *a = (char*)mem_lock(a_id);
	char *b = (char*)mem_lock(b_id);
	#ifdef USE_MEMORY_DEBUGGING
	MEM_ID tmp_id = mem_malloc_debug(strlen(a) + strlen(b) + 1, MEM_POOL_CLASS_DYNAMIC,"str_Cat() return");
	#else
	MEM_ID tmp_id = mem_malloc(strlen(a) + strlen(b) + 1,MEM_POOL_CLASS_DYNAMIC);
	#endif
	char *tmp = (char*)mem_lock(tmp_id);
	
	memset(tmp, 0, strlen(a) + strlen(b) + 1);
	memcpy(tmp, a, strlen(a));
	memcpy(tmp + strlen(a), b, strlen(b));
	mem_unlock(tmp_id,1);
	mem_unlock(b_id,0);	
	mem_unlock(a_id,0);
	return(tmp_id);
}

BYTES_ID str_Substring(BYTES_ID a_id,INDEX start,NUM len)
{
	char *a = (char*)mem_lock(a_id);
	NUM e = start + len;
	if(e> strlen(a))
		e = strlen(a) - start;
	else
		e = len;
	#ifdef USE_MEMORY_DEBUGGING
	BYTES_ID tmp_id = mem_malloc_debug(e + 1, MEM_POOL_CLASS_DYNAMIC,"str_Copy() return");
	#else
	BYTES_ID tmp_id = mem_malloc(e + 1,MEM_POOL_CLASS_DYNAMIC);
	#endif
	char *tmp = (char*)mem_lock(tmp_id);
	memset(tmp, 0, e + 1);
	memcpy(tmp, a+start, e);
	mem_unlock(tmp_id,1);
	mem_unlock(a_id,0);
	return(tmp_id);
}

BYTES_ID str_Copy(BYTES_ID a_id)
{
	char *a = (char*)mem_lock(a_id);
	#ifdef USE_MEMORY_DEBUGGING
	BYTES_ID tmp_id = mem_malloc_debug(strlen(a) + 1,MEM_POOL_CLASS_DYNAMIC, "str_Copy() return");
	#else
	BYTES_ID tmp_id = mem_malloc(strlen(a) + 1,MEM_POOL_CLASS_DYNAMIC);
	#endif

	char *tmp = (char*)mem_lock(tmp_id);
	memset(tmp, 0, strlen(a) + 1);
	memcpy(tmp, a, strlen(a));
	mem_unlock(tmp_id,1);
	mem_unlock(a_id,0);
	return(tmp_id);
}

BYTES_ID str_FromChar(char c)
{
	#ifdef USE_MEMORY_DEBUGGING
	BYTES_ID tmp_id = mem_malloc_debug(2, MEM_POOL_CLASS_STATIC,"str_FromChar() return");
	#else
	BYTES_ID tmp_id = mem_malloc(2, MEM_POOL_CLASS_STATIC);
	#endif
	char *tmp = (char*)mem_lock(tmp_id);
	
	memset(tmp, 0, 2);
	memset(tmp, c, 1);
	mem_unlock(tmp_id,1);
	return(tmp_id);
}

BYTES_ID str_Printf(BYTES_ID format, ...)
{
	va_list va;
	va_start(va,format);
	//printf("getting len\n");
	NUM n = str_PrintfVaLen(format,va);
	va_end(va);
	//printf("n:%d\n",n);
	va_start(va,format);
	BYTES_ID output = str_PrintfVa(format,n,va);
	va_end(va);
	return(output);
}

NUM str_PrintfVaLen(BYTES_ID format,va_list va)
{
	char *f = (char*)mem_lock(format);
	NUM n = vsnprintf(NULL,0,f,va);
	mem_unlock(format,0);
	return(n+1);
}

BYTES_ID str_PrintfVa(BYTES_ID format,NUM len,va_list va)
{
	BYTES_ID output = 0;
	//va_start(va,format);
	//va_list vc = va;
	//int n = vsnprintf(NULL,0,format,va) + 1;
	//va_end(va);
	//printf("len:%d\n",len);
	//printf("format:%s\n",format);
	#ifdef USE_MEMORY_DEBUGGING
	output = mem_malloc_debug((len),MEM_POOL_CLASS_DYNAMIC,"str_PrintfVA() - return");
	#else
	output = mem_malloc((len),MEM_POOL_CLASS_DYNAMIC);
	#endif
	char *o = (char*)mem_lock(output);
	char *f = (char*)mem_lock(format);
	memset(o,0,(len));
	//va_start(va,format);
	//printf("alloced\n");
	vsnprintf(o,(len),f,va);
	//va_end(va);
	//printf("output:%s\n",output);
	mem_unlock(format,0);
	mem_unlock(output,1);
	return(output);
}


