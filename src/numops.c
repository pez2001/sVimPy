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

#include "numops.h"

long long_pow(long base,long exp)
{
long r =base;
for(long i=0;i<exp-1;i++)
{
 r = r * base;
}
return(r);
}

short num_short(unsigned char a,unsigned char b)
{
//printf("num_short:%d\n",(a<<8 + b));
return((short)((((short)b)<<8) | a));
}
long num_long(unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{
return((long)  ((((long)a) << 24) | (((long)b) << 16) | (((long)c)<<8) | d) );
}

