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

#include "internal_classes.h"


object *ic_file_close(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	//printf("file close called\n");
	object *self = GetItem((object*)locals,0);
	//DumpObject(self,0);
	//DumpObject(((class_object*)((class_instance_object*)self)->instance_of)->code->names,0);
	unicode_object *file_name = CreateUnicodeObject(str_Copy("__file__"));
	object *file_tag = GetAttribute(self,(object*)file_name);
	//DumpObject(file_tag,0);
	gc_IncRefCount((object*)file_name);
	gc_DecRefCount((object*)file_name);
	if(file_tag->type == TYPE_TAG && ((tag_object*)file_tag)->tag != NULL);
	{
		//printf("stream @ %x\n",((tag_object*)file_tag)->tag);
		stream_Free(((tag_object*)file_tag)->tag);
	}
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *ic_file_readline(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	//printf("readline called\n");
	object *self = GetItem((object*)locals,0);
	//DumpObject(self,0);
	//DumpObject(((class_object*)((class_instance_object*)self)->instance_of)->code->names,0);
	unicode_object *file_name = CreateUnicodeObject(str_Copy("__file__"));
	object *file_tag = GetAttribute(self,(object*)file_name);
	//DumpObject(file_tag,0);
	gc_IncRefCount((object*)file_name);
	gc_DecRefCount((object*)file_name);
	if(file_tag->type == TYPE_TAG && ((tag_object*)file_tag)->tag != NULL);
	{
		//printf("stream @ %x\n",((tag_object*)file_tag)->tag);
		char buf[512];
		INDEX i = 0;
		while(i<512)
		{
			if(!stream_Read(((tag_object*)file_tag)->tag,&buf[i],1))
			{
				//printf("eof\n");
				break;
			}
			if(buf[i] == '\n')
			{
				//printf("eol\n");
				break;
			}
			i++;
		}
		//printf("line len:%d\n",i);
		if(i>0)
		{
			buf[i] = 0;
			//printf("readline(%d):%s\n",i,&buf);
			unicode_object *line = CreateUnicodeObject(str_Copy((char*)&buf));
			return((object*)line);
		}
	}	
	
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

class_object *ic_CreateFileClass(void)
{

	code_object *file = CreateCodeObject(str_Copy("file_code"));
	AddCodeCFunction((object*)file,"readline",&ic_file_readline);
	AddCodeCFunction((object*)file,"__del__",&ic_file_close);
	class_object *file_class = CreateClassObject(file,NULL);		
	return(file_class);
}

class_object *ic_CreateAssertionErrorClass(void)
{

	code_object *assertion_error_code = CreateCodeObject(str_Copy("assertion_error_code"));
	//AddCodeCFunction((object*)file,"readline",&ic_file_readline);
	//AddCodeCFunction((object*)file,"__del__",&ic_file_close);
	class_object *assert_class = CreateClassObject(assertion_error_code,NULL);		
	return(assert_class);
}

void AddInternalClasses(struct _vm *vm)
{
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("file_class")),(object*)ic_CreateFileClass());
}

