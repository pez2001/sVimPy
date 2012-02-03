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

char *str_Cat(char *a, char *b)
{
	if(a == NULL && b != NULL)
		return(str_Copy(b));
	else
		if(a != NULL && b == NULL)
			return(str_Copy(a));
	else
		if(a == NULL && b == NULL)
			return(NULL);
	#ifdef DEBUGGING
	char *tmp = (char *)mem_malloc(strlen(a) + strlen(b) + 1, "str_Cat() return");
	#else
	char *tmp = (char *)malloc(strlen(a) + strlen(b) + 1);
	#endif
	memset(tmp, 0, strlen(a) + strlen(b) + 1);
	memcpy(tmp, a, strlen(a));
	memcpy(tmp + strlen(a), b, strlen(b));
	return (tmp);
}

char *str_Substring(char *a,INDEX start,NUM len)
{
	NUM e = start + len;
	if(e> strlen(a))
		e = strlen(a) - start;
	else
		e = len;
	#ifdef DEBUGGING
	char *tmp = (char *)mem_malloc(e + 1, "str_Copy() return");
	#else
	char *tmp = (char *)malloc(e + 1);
	#endif
	memset(tmp, 0, e + 1);
	memcpy(tmp, a+start, e);
	return(tmp);
}

char *str_Copy(char *a)
{
	#ifdef DEBUGGING
	char *tmp = (char *)mem_malloc(strlen(a) + 1, "str_Copy() return");
	#else
	char *tmp = (char *)malloc(strlen(a) + 1);
	#endif

	memset(tmp, 0, strlen(a) + 1);
	memcpy(tmp, a, strlen(a));
	return(tmp);
}

char *str_FromChar(char c)
{
	#ifdef DEBUGGING
	char *tmp = (char *)mem_malloc(2, "str_FromChar() return");
	#else
	char *tmp = (char *)malloc(2);
	#endif
	
	memset(tmp, 0, 2);
	memset(tmp, c, 1);
	return(tmp);
}

char *str_Printf(char *format, ...)
{
	va_list va;
	va_start(va,format);
	//printf("getting len\n");
	NUM n = str_PrintfVaLen(format,va);
	va_end(va);
	//printf("n:%d\n",n);
	va_start(va,format);
	char *output = str_PrintfVa(format,n,va);
	va_end(va);
	return(output);
}

NUM str_PrintfVaLen(char *format,va_list va)
{
	NUM n = vsnprintf(NULL,0,format,va);
	return(n);
}

char *str_PrintfVa(char *format,NUM len,va_list va)
{
	char *output = NULL;
	//va_start(va,format);
	//va_list vc = va;
	//int n = vsnprintf(NULL,0,format,va) + 1;
	//va_end(va);
	//printf("len:%d\n",len);
	//printf("format:%s\n",format);
	#ifdef DEBUGGING
	output = (char*) mem_malloc((len),"str_PrintfVA() - return");
	#else
	output = (char*) malloc((len));
	#endif
	memset(output,0,(len));
	//va_start(va,format);
	//printf("alloced\n");
	vsnprintf(output,(len),format,va);
	//va_end(va);
	//printf("output:%s\n",output);
	return(output);
}


