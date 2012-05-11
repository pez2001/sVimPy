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


object *if_file_close(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
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

object *if_file_readline(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
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

object *if_open(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *x = GetItem((object*)locals,0);
	if(x->type == TYPE_UNICODE)
		printf("opening file:%s\n",((unicode_object*)x)->value);
		
	code_object *file = AllocCodeObject();
	file->type = TYPE_CODE;
	file->name = str_Copy(((unicode_object*)x)->value);
	file->argcount = 0;
	file->kwonlyargcount = 0;
	file->nlocals = 2;
	file->stacksize = 0;
	file->co_flags = 0;
	file->code = NULL;
	file->consts = NULL;
	file->varnames = NULL;
	file->freevars = NULL;
	file->cellvars = NULL;
	file->names = (object*)CreateTuple(3);
	gc_IncRefCount(file->names);
	file->ref_count = 0;
	
	
	stream *fs = stream_CreateFromFile(((unicode_object*)x)->value,"rb");
	stream_Open(fs);
	//printf("stream @ %x\n",fs);
	tag_object *file_tag = CreateTagObject(fs);
	//printf("stream tag @ %x\n",file_tag);
	
	unicode_object *file_name = CreateUnicodeObject(str_Copy("__file__"));
	kv_object *kvfile = CreateKVObject((object*)file_name,(object*) file_tag);
	SetItem(file->names,0,(object*)kvfile);

	cfunction_object *readline_cfo = CreateCFunctionObject(&if_file_readline,NULL,NULL);
	unicode_object *readline = CreateUnicodeObject(str_Copy("readline"));
	kv_object *kvreadline = CreateKVObject((object*)readline,(object*) readline_cfo);
	SetItem(file->names,1,(object*)kvreadline);

	cfunction_object *close_cfo = CreateCFunctionObject(&if_file_close,NULL,NULL);
	unicode_object *close = CreateUnicodeObject(str_Copy("__del__"));
	kv_object *kvclose = CreateKVObject((object*)close,(object*) close_cfo);
	SetItem(file->names,2,(object*)kvclose);

	
	class_object *file_class = AllocClassObject();
	file_class->type = TYPE_CLASS;
	file_class->name = str_Copy("file");
	file_class->base_classes = NULL;
	file_class->code = file;
	gc_IncRefCount((object*)file);
	file_class->code->co_flags ^= CO_CLASS_ROOT;
	file_class->ref_count = 0;
	
	class_instance_object *file_instance = AllocClassInstanceObject();
	file_instance->type = TYPE_CLASS_INSTANCE;
	file_instance->ref_count = 0;
	file_instance->instance_of = file_class;
	gc_IncRefCount((object*)file_class);
	file_instance->methods = (object*)CreateTuple(0);
	gc_IncRefCount((object*)file_instance->methods);
	file_instance->vars = (object*)CreateTuple(0);
	gc_IncRefCount((object*)file_instance->vars);
	
	
	return((object*)file_instance);
		
	//object *tmp =CreateEmptyObject(TYPE_NONE);
	//return (tmp);
}

void AddInternalClasses(struct _vm *vm)
{

}

