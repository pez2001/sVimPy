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

#ifdef USE_INTERNAL_CLASSES

OBJECT_ID ic_file_close(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	UNICODE_ID file_name = obj_CreateUnicode(mem_create_string("__file__"));
	OBJECT_ID file_tag = obj_GetAttribute(self,file_name);
	obj_IncRefCount(file_name);
	obj_DecRefCount(file_name);
	if(obj_GetType(file_tag) == TYPE_TAG)
	{
		//#ifdef USE_MEMORY_LOCK_DEBUGGING
		//tag_object *ft = (tag_object*)mem_lock_debug(file_tag,"ic_file_close taglock - stream freeing");
		//#else
		tag_object *ft = (tag_object*)mem_lock(file_tag);
		//#endif
		if(ft->tag != NULL)
		{
			stream_Free((STREAM_ID)ft->tag);
		}
		//#ifdef USE_MEMORY_LOCK_DEBUGGING
		//mem_unlock_debug(file_tag,0,"ic_file_close taglock - stream freeing");
		//#else
		mem_unlock(file_tag,0);
		//#endif
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID ic_file_readline(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	UNICODE_ID file_name = obj_CreateUnicode(mem_create_string("__file__"));
	OBJECT_ID file_tag_id = obj_GetAttribute(self,file_name);
	obj_IncRefCount(file_name);
	obj_DecRefCount(file_name);
	tag_object *file_tag = (tag_object*)mem_lock(file_tag_id);
	if(file_tag->type == TYPE_TAG && file_tag->tag != NULL);
	{
		MEM_ID buf_id = mem_create_buf(512);
		char *buf = (char*)mem_lock(buf_id);
		INDEX i = 0;
		while(i<512)
		{
			if(!stream_Read((STREAM_ID)file_tag->tag,buf_id,i,1))
			{
				break;
			}
			if(buf[i] == '\n')
			{
				break;
			}
			i++;
		}
		if(i>0)
		{
			buf[i] = 0;
			UNICODE_ID line = obj_CreateUnicode(mem_create_string(buf));
			mem_unlock(buf_id,1);
			mem_free(buf_id);
			mem_unlock(file_tag_id,0);
			return(line);
		}
		mem_unlock(buf_id,1);
		mem_free(buf_id);
		mem_unlock(file_tag_id,0);
	}	
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

CLASS_ID ic_CreateFileClass(void)
{
	CODE_ID file = obj_CreateCode(mem_create_string("file_code"));
	obj_AddCodeCFunction(file,mem_create_string("readline"),&ic_file_readline);
	obj_AddCodeCFunction(file,mem_create_string("__del__"),&ic_file_close);
	CLASS_ID file_class = obj_CreateClass(file,0);		
	return(file_class);
}

OBJECT_ID ic_assertion_error_init(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID exception_message = tuple_GetItem(locals,1);
	obj_SetAttribute(self,obj_CreateUnicode(mem_create_string("message")),exception_message);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

CLASS_ID ic_CreateAssertionErrorClass(void)
{
	CODE_ID assertion_error_code = obj_CreateCode(mem_create_string("assertion_error_code"));
	obj_AddCodeCFunction(assertion_error_code,mem_create_string("__init__"),&ic_assertion_error_init);
	//AddCodeCFunction((object*)file,"__del__",&ic_file_close);
	CLASS_ID assert_class = obj_CreateClass(assertion_error_code,0);		
	return(assert_class);
}

void AddInternalClasses(VM_ID vm)
{
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("file_class")),ic_CreateFileClass());
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("AssertionError")),ic_CreateAssertionErrorClass());
}
#endif
