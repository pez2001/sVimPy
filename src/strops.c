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

char *str_Cat(char *a,char *b)
{
	 char *tmp = (char*)mem_malloc(strlen(a)+strlen(b)+1,"str_Cat() return");
	 memset(tmp,0,strlen(a)+strlen(b)+1);
	 memcpy(tmp,a,strlen(a));
	 memcpy(tmp+strlen(a),b,strlen(b));
     return(tmp);
}

char *str_Copy(char *a)
{
	 char *tmp = (char*)mem_malloc(strlen(a)+1,"str_Copy() return");
	 memset(tmp,0,strlen(a)+1);
	 memcpy(tmp,a,strlen(a));
     return(tmp);
}

char *str_FromChar(char c)
{
 char *tmp = (char*)mem_malloc(2,"str_FromChar() return");
 memset(tmp,0,2);
 memset(tmp,c,1);
 return(tmp);
 
}