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

#include "garbage.h"


void gc_Clear(ptr_list *gc_collection)
{
for(int i=0;i<gc_collection->num;i++)
	if(HasNoRefs(gc_collection->items[i]))
	{
	if(debug_level >1)
	{
		printf("object has no refs\n");
		DumpObject(gc_collection->items[i],0);
	}
	FreeObject(gc_collection->items[i]);
	ptr_Remove(gc_collection,i);
	}
	else
	{
		if(debug_level > 2)
		{
			printf("object has gained refs\n");
			DumpObject(gc_collection->items[i],0);
		}
		ptr_Remove(gc_collection,i);
	}
}
