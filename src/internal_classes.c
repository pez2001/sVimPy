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
		if(ft->tag != 0)
		{
			stream_Free(ft->tag);
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
	if(file_tag->type == TYPE_TAG && file_tag->tag != 0);
	{
		MEM_ID buf_id = mem_create_buf(512);
		char *buf = (char*)mem_lock(buf_id);
		INDEX i = 0;
		while(i<512)
		{
			if(!stream_Read(file_tag->tag,buf_id,i,1))
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
	//printf("assertion error init\n");
	//obj_Print(exception_message);
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



OBJECT_ID ic_vm_add_global(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID ic_vm_remove_global(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_RemoveGlobal(vm,key);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID ic_vm_interrupt(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	vm_Interrupt(vm,NULL);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID ic_vm_get_global(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID ic_vm_get_global_by_index(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID ic_vm_continue(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	vm_Continue(vm);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID ic_vm_exit(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID message = tuple_GetItem(locals,1);
	unicode_object *m = (unicode_object*)mem_lock(message);
	BYTES_ID msg = m->value;
	mem_unlock(message,0);
	OBJECT_ID err_no = tuple_GetItem(locals,2);
	int_object *e = (int_object*)mem_lock(err_no);
	NUM eno = e->value;
	mem_unlock(err_no,0);
	vm_Exit(vm,msg,eno);//TODO crashing ?
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID ic_vm_stop(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	vm_Stop(vm);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID ic_vm_run_pyc(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
OBJECT_ID ic_vm_run_rpycplus(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
OBJECT_ID ic_vm_run_rpyc(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
OBJECT_ID ic_vm_run_function(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
OBJECT_ID ic_vm_run_object(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
OBJECT_ID ic_vm_start_method(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
OBJECT_ID ic_vm_run_method(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
OBJECT_ID ic_vm_start_object(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
/*
OBJECT_ID ic_execute_cfunction_by_name(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID key = tuple_GetItem(locals,1);
	OBJECT_ID new_global = tuple_GetItem(locals,2);
	//obj_Print(key);
	//obj_Print(new_global);	
	vm_AddGlobal(vm,key,new_global);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}*/

OBJECT_ID ic_vm_clear_gc(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	obj_ClearGC();
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

#ifdef USE_DEBUGGING
OBJECT_ID ic_vm_dump_object(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID o = tuple_GetItem(locals,1);
	obj_Dump(o,0,1);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
#endif

OBJECT_ID ic_vm_copy_object(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID self = tuple_GetItem(locals,0);
	OBJECT_ID o = tuple_GetItem(locals,1);
	OBJECT_ID c = obj_Copy(o);
	return(c);
}



CLASS_ID ic_CreateVmClass(void)
{
	CODE_ID vm_code = obj_CreateCode(mem_create_string("vm_code"));
	obj_AddCodeCFunction(vm_code,mem_create_string("AddGlobal"),&ic_vm_add_global);
	obj_AddCodeCFunction(vm_code,mem_create_string("RemoveGlobal"),&ic_vm_remove_global);
	obj_AddCodeCFunction(vm_code,mem_create_string("GetGlobal"),&ic_vm_get_global);
	obj_AddCodeCFunction(vm_code,mem_create_string("GetGlobalByIndex"),&ic_vm_get_global_by_index);
	obj_AddCodeCFunction(vm_code,mem_create_string("Interrupt"),&ic_vm_interrupt);
	obj_AddCodeCFunction(vm_code,mem_create_string("Continue"),&ic_vm_continue);
	obj_AddCodeCFunction(vm_code,mem_create_string("Exit"),&ic_vm_exit);
	obj_AddCodeCFunction(vm_code,mem_create_string("Stop"),&ic_vm_stop);
	//obj_AddCodeCFunction(vm_code,mem_create_string("RunPYC"),&ic_vm_run_pyc);
	//obj_AddCodeCFunction(vm_code,mem_create_string("RunRPYC"),&ic_vm_run_rpyc);
	//obj_AddCodeCFunction(vm_code,mem_create_string("RunRPYCPlus"),&ic_vm_run_rpycplus);
	//obj_AddCodeCFunction(vm_code,mem_create_string("RunFunction"),&ic_vm_run_function);
	//obj_AddCodeCFunction(vm_code,mem_create_string("RunObject"),&ic_vm_run_object);
	//obj_AddCodeCFunction(vm_code,mem_create_string("StartMethod"),&ic_vm_start_method);
	//obj_AddCodeCFunction(vm_code,mem_create_string("RunMethod"),&ic_vm_run_method);
	//obj_AddCodeCFunction(vm_code,mem_create_string("StartObject"),&ic_vm_start_object);
	obj_AddCodeCFunction(vm_code,mem_create_string("ClearGC"),&ic_vm_clear_gc);
	#ifdef USE_DEBUGGING
	obj_AddCodeCFunction(vm_code,mem_create_string("DumpObject"),&ic_vm_dump_object);
	#endif
	obj_AddCodeCFunction(vm_code,mem_create_string("CopyObject"),&ic_vm_copy_object);
	CLASS_ID vm_class = obj_CreateClass(vm_code,0);		
	return(vm_class);
}
/*
CLASS_ID ic_CreateObjClass(void)
{
	CODE_ID obj_code = obj_CreateCode(mem_create_string("obj_code"));
	obj_AddCodeName(obj_code,obj_CreateUnicode(mem_create_string("TYPE_NULL")),obj_CreateInt('0'));
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	obj_AddCodeCFunction(obj_code,mem_create_string(""),&ic_obj_);
	
	CLASS_ID obj_class = obj_CreateClass(obj_code,0);		
	return(obj_class);
}
*/



void AddInternalClasses(VM_ID vm)
{
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("file_class")),ic_CreateFileClass());
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("AssertionError")),ic_CreateAssertionErrorClass());
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("Vm")),ic_CreateVmClass());
	//vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("Obj")),ic_CreateObjClass());
}
#endif
