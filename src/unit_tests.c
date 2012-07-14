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

#include "unit_tests.h"


#ifdef USE_DEBUGGING
int debug_level = 0;

void ptr_tests(void)
{
	printf("testing ptr list functions\n");
	LIST_ID p = list_Create(0, 0);
	
	list_Insert(p,0,(MEM_ID)2);
	list_Insert(p,0,(MEM_ID)1);
	for(NUM i = 0; i < list_GetLen(p); i++)
		printf("%d ", (int)list_Get(p,i));
	list_Pop(p);
	list_Pop(p);
	printf("\n");
		
	list_Push(p, (MEM_ID)1);
	list_Push(p, (MEM_ID)2);
	list_Push(p, (MEM_ID)3);
	list_Push(p, (MEM_ID)4);
	printf("push(1,2,3,4):\n");
	for(NUM i = 0; i < list_GetLen(p); i++)
		printf("%d ", (int)list_Get(p,i));
	printf("\n");
	int x =(int)list_Pop(p);

	printf("pop(): %d\n", x);
	list_Push(p,  (MEM_ID)5);
	printf("push(5):\n");
	for(NUM i = 0; i < list_GetLen(p); i++)
		printf("%d ", list_Get(p,i));
	printf("\n");
	int r = (int)list_Remove(p, 2);

	printf("remove(2): %d\n", r);
	for(NUM i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\ninserting\n");
	list_Insert(p, 3, (MEM_ID)66);
	printf("insert 3,66:\n");
	for(NUM i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\ninserting\n");
	list_Insert(p, 0, (MEM_ID)99);
	printf("insert 0,99:\n");
	for(NUM i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\n");
	list_Set(p, 1,(MEM_ID)22);
	printf("set 1,22:\n");
	for(NUM i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\n");
	list_Queue(p, (MEM_ID)88);
	printf("queue(88):\n");
	for(NUM i = 0; i < list_GetLen(p); i++)
		printf("%d ",(int)list_Get(p,i));
	printf("\n");

	list_Clear(p);
	list_Push(p, 0);
	for(NUM i = 0; i < 20000; i++)
		list_Push(p, (MEM_ID)1);
	for(NUM i = 0; i < 20000; i++)
		list_Pop(p);
	printf("first:%d \n", (int)list_Pop(p));
	printf("top:%d\n", list_GetLen(p));

	for(NUM i = 0; i < 20000; i++)
	{
		list_Push(p, (MEM_ID)0);
		list_Pop(p);
	}

	list_Close(p);
	printf("ptr tests thru\n");
}

void stream_tests(void)
{
	long pyc_magic = MAGIC;
	streams_Init();
	printf("stream tests\n");
	STREAM_ID f = stream_CreateFromFile(mem_create_string("tests/test52.pyc"),mem_create_string("rb"));
	
	if (!stream_Open(f))
	{
		printf("stream cant be opened\n");
		return;
	}
	printf("stream opened\n");
	long magic = stream_ReadLong(f);

	if (magic != pyc_magic)
	{
		printf("wrong magic:%d (%d)\n",magic,pyc_magic);
		return;
	}
	printf("read magic:%d\n",magic);
	stream_Free(f);
	
	STREAM_ID m = stream_CreateFromBytes(mem_create((char*)&test_pyc,TEST_PYC_LEN),TEST_PYC_LEN);
	printf("opening mstream\n");
	if (!stream_Open(m))
	{
		printf("mstream cant be opened\n");
		return;
	}
	printf("mstream opened\n");
	long mmagic = stream_ReadLong(m);

	if (mmagic != pyc_magic)
	{
		printf("wrong magic:%d (%d)\n",mmagic,pyc_magic);
		return;
	}
	printf("read magic:%d\n",mmagic);
	stream_Free(m);
	streams_Close();
	printf("stream tests done\n");
}
#endif

void OpenPYC(STREAM_ID f, VM_ID vm)
{
	// char *b = (char*)mem_malloc(3);
	// char *bh = (char*)mem_malloc(4);
	// memset(bh,0,4);
	long pyc_magic = MAGIC;
	//printf("opening pyc\n");
	//stream *f = stream_CreateFromFile(filename);
	
	if (!stream_Open(f))
		return;
	//#ifdef USE_DEBUGGING
	//debug_printf(DEBUG_VERBOSE_TESTS,"executing object:%s\n", filename);
	//#endif
	// int r = fread(bh,4,1,f);
	// if(!memcmp(bh,(char*)&pyc_magic,4))
	long magic = stream_ReadLong(f);

	if (magic != pyc_magic)
		return;
	stream_ReadLong(f);//skip time

	/* struct tm *ti; //TO DECREASE MEMORY USAGE ti = localtime((void*)&time);
	   char *bt = (char*)mem_malloc(100); strftime(bt,100,"%Y.%m.%d ",ti);
	   printf("file date: %s\n", bt); strftime(bt,100,"%H:%M:%S \n",ti);
	   printf("file time: %s\n", bt); mem_free(bt); //8 bytes into the file */
	//printf("reading object\n");
	OBJECT_ID obj = stream_ReadObject(f);
	//object *obj = NULL;
	//return;
	//printf("read pyc\n");
	// printf("heap bytes used by objects:%d\n",mem_chunks_actual_size);
	// printf("MAX HEAP USAGE:%d\n",mem_chunks_max_size);
	// printf("objects num: %d\n",objects_num);
	// printf("max objects : %d\n",objects_max);
	// printf("objects headers total size : %d\n",objects_header_total);
	code_object *co = (code_object*)mem_lock(obj);
	OBJECT_ID global_key = obj_CreateUnicode(mem_copy(co->name));
	mem_unlock(obj,0);
	vm_AddGlobal(vm, global_key,obj);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		obj_Dump(obj, 0,1);
	#endif
	OBJECT_ID ret = 0;
	#ifdef USE_DEBUGGING
	//if((debug_level & DEBUG_INTERACTIVE) > 0)
	//	ret = vm_InteractiveRunObject(vm, obj, 0,0);	
	//else
		ret = vm_RunObject(vm, obj, 0,0);
	#else
	ret = vm_RunObject(vm, obj, 0,0);	
	#endif
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"\n");
	#endif
	if (ret != 0)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			obj_Dump(ret, 0,1);
		#endif
		obj_DecRefCount(ret);
	}
	#ifdef USE_DEBUGGING
	//debug_printf(DEBUG_VERBOSE_TESTS,"object executed:%s\n", filename);
	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		obj_Dump(obj,0,1);
	debug_printf(DEBUG_VERBOSE_TESTS,"cleaning up object\n");
	#endif
	//DumpObject(obj,0);
	obj_DecRefCount(obj);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"removing object from globals\n");
	#endif
	vm_RemoveGlobal(vm,global_key);
	//printf("freeing stream\n");
	stream_Free(f);
	obj_ClearGC();
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"pyc executed\n");
	#endif
	//printf("pyc executed\n");
	// mem_free(b);
	// mem_free(bh);
	
}

void brute_test(void)
{
	#ifdef USE_DEBUGGING
	//debug_level |= DEBUG_INTERACTIVE;
	debug_level |= DEBUG_MEMORY;
	// debug_level |= DEBUG_SHOW_OPCODES;
	//debug_level |= DEBUG_FULL_DUMP;
	//debug_level |= DEBUG_STACK;
	//debug_level |= DEBUG_LISTS;
	//debug_level |= DEBUG_GC;
	//debug_level |= DEBUG_VERBOSE_STEP;
	//debug_level |= DEBUG_VM;
	//debug_level |= DEBUG_FREEING;
	//debug_level |= DEBUG_ALLOCS;
	//debug_level |= DEBUG_DUMP_UNSUPPORTED;
	//debug_level |= DEBUG_DUMP_OBJECT;
	//debug_level |= DEBUG_CREATION;
	//debug_level |= DEBUG_VERBOSE_FREEING;
	debug_level |= DEBUG_VERBOSE_TESTS;	
	//debug_level |= DEBUG_PTR_LISTS;
	//debug_level |= DEBUG_INTERNAL_FUNCTIONS;
	//debug_level |= DEBUG_COUNT_OBJECTS;
	#endif
	
	VM_ID vm = vm_Init(0);

	// printf("Calling all Unit Tests\n");

	//stream *mb = stream_CreateFromBytes((char*)&blink,BLINK_LEN);
	//stream_Free(mb);
	/*vm_RunPYC(vm,mb,0);
	printf("calling funcs\n");
	vm_CallFunction(vm,"setup",NULL,0);
	vm_CallFunction(vm,"loop",NULL,0);
	printf("called funcs\n");
	*/
	


	//import	
	//OpenPYC(stream_CreateFromFile("tests/test_import.pyc"), vm);

	//custom code + import_from + import_star opcodes
	//OpenPYC("tests/test45.pyc", vm);


	//append ops + generators
	//OpenPYC("tests/test53.pyc", vm);
	
	//OpenPYC(stream_CreateFromFile("tests/blink.pyc"), vm);

	//stream test
	STREAM_ID m = stream_CreateFromBytes(mem_create((char*)&test_pyc,TEST_PYC_LEN),TEST_PYC_LEN);
	OpenPYC(m,vm);
	
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test52.pyc"),mem_create_string("rb")), vm);
	
	//generators
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test25.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test24b.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test24.pyc"),mem_create_string("rb")), vm);
	
	
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test26.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test20.pyc"),mem_create_string("rb")), vm);
	
	//simple generator with yield
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test59.pyc"),mem_create_string("rb")), vm);

	
	//iters
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test61.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test60.pyc"),mem_create_string("rb")), vm);

	//print + recursion
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test17.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test8.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test9.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test12.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test11.pyc"),mem_create_string("rb")), vm);
	
	//function parameters
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test50.pyc"),mem_create_string("rb")), vm);//with keywords unordered
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test49b.pyc"),mem_create_string("rb")), vm);//kw unordered
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test49.pyc"),mem_create_string("rb")), vm);//kw
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test48.pyc"),mem_create_string("rb")), vm);//var
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test47.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test46.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test_functions.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test36.pyc"),mem_create_string("rb")), vm);//call_function_var
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test37.pyc"),mem_create_string("rb")), vm);//call_function_kw
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test38.pyc"),mem_create_string("rb")), vm);//call_function_var_kw 
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test39.pyc"),mem_create_string("rb")), vm);//call_function
	
	//pos + kw defaults
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test55c.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test55b.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test55.pyc"),mem_create_string("rb")), vm);

	//kw defaults
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test54.pyc"),mem_create_string("rb")), vm);

	//storing of globals/vars and deletion of globals/vars
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21ba2.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21b.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21ba.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21ba3.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21bb.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21c.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21d.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21e.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test21a.pyc"),mem_create_string("rb")), vm);
	
	//loops + recursion
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test35.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test34.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test33.pyc"),mem_create_string("rb")), vm);

	//nested tuple printing
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test51.pyc"),mem_create_string("rb")), vm);
	
	//comparing
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test_compare.pyc"),mem_create_string("rb")), vm);

	//globals
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test58.pyc"),mem_create_string("rb")), vm);

	//closures and deref opcodes
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test57.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test29.pyc"),mem_create_string("rb")), vm);
	
	//brute ops + closures
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test32.pyc"),mem_create_string("rb")), vm);

	//simple stuff
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test1.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test2.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test3.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test6.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test13.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test15.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test16.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test18.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test19.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test22.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test7.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test5.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test4.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test27.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test28.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test23.pyc)"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test56.pyc"),mem_create_string("rb")), vm);
	
	
	//while loop + break + continue
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test_while.pyc"),mem_create_string("rb")), vm);

	//ellipsis object
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test43.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test40.pyc"),mem_create_string("rb")), vm);

	//most binary ops test
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test14.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test14b.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test14f.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test14bf.pyc"),mem_create_string("rb")), vm);

	//floats
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test44.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test42.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test41.pyc"),mem_create_string("rb")), vm);

	//dictionaries
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test30.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/test31.pyc"),mem_create_string("rb")), vm);
	
	
	//brute prime (classless it takes longer because of range
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/e20.pyc"),mem_create_string("rb")), vm);
	OpenPYC(stream_CreateFromFile(mem_create_string("tests/e_small.pyc"),mem_create_string("rb")), vm);
	
	//OpenPYC(stream_CreateFromFile("tests/e_med.pyc","rb"), vm);
	
	//OpenPYC("tests/e_bigger.pyc", vm);
	//OpenPYC("tests/e_max.pyc", vm);

	//OpenPYC("tests/test.pyc", vm);
	//OpenPYC("tests/test10.pyc", vm);
	//OpenPYC("tests/e.pyc", vm);
	
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing vm\n");
	#endif
	vm_Close(vm);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_DUMP_UNSUPPORTED) > 0)
		DumpUnsupportedOpCodes();
	
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing memory manager\n");
	#endif
	mem_Close();
	#endif


}

OBJECT_ID ImportModule(VM_ID vm,STRING_ID module_name)
{
	long pyc_magic = MAGIC;
	BYTES_ID path = mem_create_string("tests/");
	BYTES_ID mod_path = str_Cat(path,module_name);
	BYTES_ID ext = mem_create_string(".pyc");
	BYTES_ID filename = str_Cat(mod_path,ext);
	mem_free(path);
	mem_free(mod_path);
	mem_free(ext);
	
	//printf("trying to import module:%s\n",filename);
	STREAM_ID f = stream_CreateFromFile(filename,mem_create_string("rb"));
	if (!stream_Open(f))
		return(0);
	long magic =stream_ReadLong(f);
	if(magic != pyc_magic)
		return(0);
	stream_ReadLong(f);//skip time
	OBJECT_ID obj = stream_ReadObject(f);
	if(obj_GetType(obj) != TYPE_CODE)
	{
		mem_free(filename);
		//obj_IncRefCount(obj);
		obj_DecRefCount(obj);
		return(0);
	}
	else
	{
		code_object *o = (code_object*)mem_lock(obj);
		o->co_flags ^=	CO_MODULE_ROOT;
		mem_unlock(obj,1);
	}
	mem_free(filename);
	stream_Free(f);
	return(obj);
}

OBJECT_ID CatchException(VM_ID vm,OBJECT_ID exception)
{
	printf("caught exception:");
	UNICODE_ID msg_name = obj_CreateUnicode(mem_create_string("message"));
	obj_IncRefCount(msg_name);
	OBJECT_ID message = obj_GetAttribute(exception,msg_name);
	obj_DecRefCount(msg_name);
	obj_Print(message);
	unicode_object *m = (unicode_object*)mem_lock(message);
	printf("\n");
	vm_Exit(vm,m->value,1);
	mem_unlock(message,0);
	return(0);
}

NUM AtomicOpenPYC(char *filename)
{
	printf("executing object:%s\n", filename);

	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"vm_Init\n");
	#endif
	VM_ID vm = vm_Init(0);
	vm_SetImportModuleHandler(vm,&ImportModule);
	vm_SetExceptionHandler(vm,&CatchException);
	long pyc_magic = MAGIC;
	STREAM_ID f = stream_CreateFromFile(mem_create_string(filename),mem_create_string("rb"));
	if(!stream_Open(f))
	{
		printf("error opening pyc file: %s\n",filename);
		return(1);
	}
	long magic = stream_ReadLong(f);
	if(magic != pyc_magic)
	{
		printf("error wrong file magic: %s\n",filename);
		return(1);
	}
	stream_ReadLong(f);//skip time
	OBJECT_ID obj = stream_ReadObject(f);
	code_object *co = (code_object*)mem_lock(obj);
	OBJECT_ID global_key = obj_CreateUnicode(mem_copy(co->name));
	obj_IncRefCount(global_key);
	mem_unlock(obj,0);
	vm_AddGlobal(vm, global_key,obj);
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		obj_Dump(obj, 0,1);
	#endif
	OBJECT_ID ret = 0;
	//#ifdef USE_DEBUGGING
	//if((debug_level & DEBUG_INTERACTIVE) > 0)
	//	ret = vm_InteractiveRunObject(vm, obj, 0,0);
	//else
	//	ret = vm_RunObject(vm, obj, 0,0);
	//#else
	printf("sVimPy Python Output\n{\n");
	ret = vm_RunObject(vm, obj, 0,0);
	//#endif
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_ALL,"\n");
	#endif
	if (ret != 0)
	{
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			obj_Dump(ret, 0,1);
		#endif
		obj_DecRefCount(ret);
	}
	printf("}\n");
	#ifdef USE_DEBUGGING
	//debug_printf(DEBUG_VERBOSE_TESTS,"object executed:%s\n", filename);
	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		obj_Dump(obj,0,0);
	debug_printf(DEBUG_VERBOSE_TESTS,"cleaning up object:%s\n",filename);
	#endif
	//DumpObject(obj,0);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"removing object from globals\n");
	#endif
	vm_RemoveGlobal(vm,global_key);
	obj_DecRefCount(global_key);
	//obj_ClearGC();
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"freeing object now\n");
	#endif
	obj_DecRefCount(obj);
	//printf("freeing stream\n");
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"freeing stream\n");
	#endif
	stream_Free(f);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"pyc executed\n");
	#endif
	#ifdef USE_DEBUGGING
	if((debug_level & DEBUG_DUMP_UNSUPPORTED) > 0)
		DumpUnsupportedOpCodes();
	#endif
	
	/*
	printf("Original Python Output\n{\n");
	char *py_cmd = NULL;
	py_cmd = str_Cat(py_cmd,"c:/python32/python ");
	//printf("executing python cmd:%s\n",py_cmd);
	char *py_cmdf = NULL;
	py_cmdf = str_Cat(py_cmd,filename);
	//printf("executing python cmd:%s\n",py_cmd);
	system(py_cmdf);
	printf("\n}\n");
	#ifdef USE_DEBUGGING
	assert(mem_free(py_cmd));
	assert(mem_free(py_cmdf));
	debug_printf(DEBUG_VERBOSE_TESTS,"closing memory manager\n");
	#else
	free(py_cmd);
	free(py_cmdf);
	#endif
	*/
	struct _vm *v = (struct _vm*)mem_lock(vm);
	if(v->error_message != 0)
	{
		char *m = (char*)mem_lock(v->error_message);
		printf("error occured:\n%s\n",m);
		mem_unlock(v->error_message,0);
	}
	mem_unlock(vm,0);
	//mem_DebugHeapWalk(0,1);
	NUM exit_code = vm_Close(vm);

	printf("vm exit code:%d\n",exit_code);
	return(exit_code);
}

NUM atomic_test(void)
{
	#ifdef USE_DEBUGGING
	debug_level = 0;
	//debug_level |= DEBUG_INTERACTIVE;
	//debug_level |= DEBUG_MEMORY;
	//debug_level |= DEBUG_SHOW_OPCODES;
	//debug_level |= DEBUG_FULL_DUMP;
	//debug_level |= DEBUG_STACK;
	//debug_level |= DEBUG_LISTS;
	//debug_level |= DEBUG_GC;
	//debug_level |= DEBUG_VERBOSE_STEP;
	//debug_level |= DEBUG_VM;
	//debug_level |= DEBUG_FREEING;
	//debug_level |= DEBUG_ALLOCS;
	//debug_level |= DEBUG_DUMP_UNSUPPORTED;
	//debug_level |= DEBUG_DUMP_OBJECT;
	//debug_level |= DEBUG_CREATION;
	//debug_level |= DEBUG_VERBOSE_FREEING;
	//debug_level |= DEBUG_VERBOSE_TESTS;	
	//debug_level |= DEBUG_PTR_LISTS;
	//debug_level |= DEBUG_INTERNAL_FUNCTIONS;
	//debug_level |= DEBUG_COUNT_OBJECTS;
	printf("Atomic Tests Version : %d.%d-%d\n",MAJOR_VERSION,MINOR_VERSION,DEBUG_BUILD+1);
    #else
	printf("Atomic Tests Version : %d.%d-%d\n",MAJOR_VERSION,MINOR_VERSION,BUILD+1);
	#endif

	//AtomicOpenPYC("tests/PlayNew.pyc");
		//testing seperated function var spaces
	//AtomicOpenPYC("tests/test_sep_method.pyc");
	//return;
	//AtomicOpenPYC("tests/test_print.pyc");
	//AtomicOpenPYC("tests/test_sep_func.pyc");
		//open file test + if_iter with sentinel
	//AtomicOpenPYC("tests/test_open.pyc");	
	//	AtomicOpenPYC("tests/test_class11.pyc");
	//AtomicOpenPYC("tests/test_class6.pyc");
	//AtomicOpenPYC("tests/test_import.pyc");
//AtomicOpenPYC("tests/test_import.pyc");
	NUM exit_code = 0;
	//exit_code = AtomicOpenPYC("tests/");
	//if(exit_code)
	//	return(exit_code);
	//return(exit_code);
	
	//open file test + if_iter with sentinel
	exit_code = AtomicOpenPYC("tests/test_open.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_functions.pyc"); //leaking lists containing single items
	if(exit_code)
		return(exit_code);
	//return(0);
	exit_code = AtomicOpenPYC("tests/test45.pyc");
	if(exit_code)
		return(exit_code);
	//return(0);

	
	//function parameters
	exit_code = AtomicOpenPYC("tests/test46.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test47.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test50.pyc");//with keywords unordered
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test49b.pyc");//kw unordered
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test49.pyc");//kw
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test48.pyc");//var
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test36.pyc");//call_function_var
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test37.pyc");//call_function_kw
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test38.pyc");//call_function_var_kw 
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test39.pyc");//call_function
	if(exit_code)
		return(exit_code);

	//comparing
	exit_code = AtomicOpenPYC("tests/test_compare.pyc");
	if(exit_code)
		return(exit_code);

	
	
	//storing of globals/vars and deletion of globals/vars
	exit_code = AtomicOpenPYC("tests/test21ba2.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test21.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test21b.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test21ba.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test21ba3.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test21bb.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test21c.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test21d.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test21e.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test21a.pyc");
	if(exit_code)
		return(exit_code);
	

	//test generators
	exit_code = AtomicOpenPYC("tests/test20.pyc");
	if(exit_code)
		return(exit_code);

	
	//kw defaults
	exit_code = AtomicOpenPYC("tests/test54.pyc");
	if(exit_code)
		return(exit_code);
	
	
	//pos + kw defaults
	exit_code = AtomicOpenPYC("tests/test55c.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test55b.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test55.pyc");
	if(exit_code)
		return(exit_code);
	//return;

	

	//simple print 
	exit_code = AtomicOpenPYC("tests/test_print.pyc");
	if(exit_code)
		return(exit_code);

	exit_code = AtomicOpenPYC("tests/prng_single.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/prng.pyc");
	if(exit_code)
		return(exit_code);
	//return(0);

	printf("executing arduino object:tests/fade_min.pyc\n");
	printf("sVimPy Python Output\n{\n");
	VM_ID vm = vm_Init(0);
	vm_SetImportModuleHandler(vm, &ImportModule);
	vm_SetExceptionHandler(vm,&CatchException);
	long pyc_magic = MAGIC;
	STREAM_ID f = stream_CreateFromFile(mem_create_string("tests/fade_min.pyc"),mem_create_string("rb"));
	if(!stream_Open(f))
	{
		printf("error opening pyc file: tests/fade_min.pyc\n");
		return(1);
	}
	long magic = stream_ReadLong(f);
	if (magic != pyc_magic)
	{
		printf("error wrong file magic: tests/fade_min.pyc\n");
		return(1);
	}
	stream_ReadLong(f);//skip time
	OBJECT_ID obj = stream_ReadObject(f);
	code_object *co = (code_object*)mem_lock(obj);
	OBJECT_ID global_key = obj_CreateUnicode(mem_copy(co->name));
	mem_unlock(obj,0);
	vm_AddGlobal(vm, global_key,obj);
	obj_IncRefCount(global_key);
	OBJECT_ID ret = vm_RunObject(vm, obj, 0,0);
	printf("ran module main\n");
	if(ret != 0)
		obj_DecRefCount(ret);
	BYTES_ID ssetup = mem_create_string("setup");	
	BYTES_ID sloop = mem_create_string("loop");	
	OBJECT_ID sr = vm_RunFunction(vm,ssetup,0,0);
	obj_DecRefCount(sr);
	printf("ran setup\n");
	for(INDEX i = 0;i<10;i++) //execute one hundred test loops
	{
		OBJECT_ID lr = vm_RunFunction(vm,sloop,0,0);
		obj_DecRefCount(lr);
	}
	printf("ran loops\n");
	mem_free(ssetup);
	mem_free(sloop);
	vm_RemoveGlobal(vm,global_key);
	obj_DecRefCount(global_key);
	obj_DecRefCount(obj);
	stream_Free(f);
	printf("}\n");
	struct _vm *v = (struct _vm*)mem_lock(vm);
	if(v->error_message != 0)
	{
		char *m = (char*)mem_lock(v->error_message);
		printf("error occured:\n%s\n",m);
		mem_unlock(v->error_message,0);
	}
	mem_unlock(vm,0);
	exit_code = vm_Close(vm);
	printf("vm exit code:%d\n",exit_code);
	if(exit_code)
		return(exit_code);
	//return(exit_code);

	printf("executing arduino object:fade_min.h\n");
	printf("sVimPy Python Output\n{\n");
	vm = vm_Init(0);
	vm_SetImportModuleHandler(vm,&ImportModule);
	vm_SetExceptionHandler(vm, &CatchException);
	//stream *f = stream_CreateFromFile("tests/fade_min.pyc","rb");
	MEM_ID fade_mem = mem_create((char*)&fade_min,FADE_MIN_LEN);
	f = stream_CreateFromBytes(fade_mem,FADE_MIN_LEN);
	if(!stream_Open(f))
	{
		printf("error opening pyc file: fade_min\n");
		return(1);
	}
	//long magic = stream_ReadLong(f);
	//if (magic != pyc_magic)
	//{
	//	printf("error wrong file magic: tests/blink.pyc\n");
	//	return(1);
	//}
	//stream_ReadLong(f);//skip time
	obj = stream_ReadObject(f);
	co = (code_object*)mem_lock(obj);
	global_key = obj_CreateUnicode(mem_copy(co->name));
	mem_unlock(obj,0);
	vm_AddGlobal(vm, global_key,obj);
	obj_IncRefCount(global_key);
	ret = vm_RunObject(vm, obj, 0,0);
	printf("ran module main\n");
	if(ret != 0)
		obj_DecRefCount(ret);
		
	ssetup = mem_create_string("setup");	
	sloop = mem_create_string("loop");	
	sr = vm_RunFunction(vm,ssetup,0,0);
	obj_DecRefCount(sr);
	printf("ran setup\n");
	for(INDEX i = 0;i<100;i++) //execute one hundred test loops
	{
		OBJECT_ID lr = vm_RunFunction(vm,sloop,0,0);
		obj_DecRefCount(lr);
	}
	printf("ran loops\n");
	mem_free(ssetup);
	mem_free(sloop);
	
	vm_RemoveGlobal(vm,global_key);
	obj_DecRefCount(global_key);
	obj_DecRefCount(obj);
	stream_Free(f);
	mem_free(fade_mem);
	printf("}\n");
	v = (struct _vm*)mem_lock(vm);
	if(v->error_message != 0)
	{
		char *m = (char*)mem_lock(v->error_message);
		printf("error occured:\n%s\n",m);
		mem_unlock(v->error_message,0);
	}
	mem_unlock(vm,0);
	exit_code = vm_Close(vm);
	printf("vm exit code:%d\n",exit_code);
	if(exit_code)
		return(exit_code);
	//return(exit_code);



	//exit_code = AtomicOpenPYC("tests/queens2a.pyc");
	//if(exit_code)
	//	return(exit_code);
	//return(exit_code);
	exit_code = AtomicOpenPYC("tests/test32b.pyc");
	if(exit_code)
		return(exit_code);
	//return;

	//exceptions
	exit_code = AtomicOpenPYC("tests/test_assert.pyc");
	if(exit_code != 1)
		return(1);
	exit_code = 0;
	//return;
	
	//AtomicOpenPYC("tests/Queens2a.pyc");
	//return;

	//custom code + import_from + import_star opcodes
	//AtomicOpenPYC("tests/test_import.pyc");

	//classes tests
	exit_code = AtomicOpenPYC("tests/test_class7.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class11.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class10.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class9.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class8.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class5.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class6.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class4.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class3.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class2.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_class.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_sep_method2.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_sep_method.pyc");
	if(exit_code)
		return(exit_code);

	//testing seperated function var spaces
	exit_code = AtomicOpenPYC("tests/test_sep_func.pyc");
	if(exit_code)
		return(exit_code);
	
	//brute prime
	exit_code = AtomicOpenPYC("tests/e20.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/e_small.pyc");	
	if(exit_code)
		return(exit_code);

	//brute recursion queens test
	//AtomicOpenPYC("tests/Queens2a.pyc");
	//AtomicOpenPYC("tests/Queens4.pyc");
	//return;

	//fmod tests + classes as globals
	//AtomicOpenPYC("tests/Play.pyc");//old version without class support
	//AtomicOpenPYC("tests/PlayNew.pyc");


	//module name set test
	exit_code = AtomicOpenPYC("tests/test_main.pyc");
	if(exit_code)
		return(exit_code);
	
	//append ops + generators
	//AtomicOpenPYC("tests/test53.pyc", vm);
	exit_code = AtomicOpenPYC("tests/test_yield.pyc");
	if(exit_code)
		return(exit_code);

	//iters
	exit_code = AtomicOpenPYC("tests/test61.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test60.pyc");
	if(exit_code)
		return(exit_code);

	


	//simple stuff
	exit_code = AtomicOpenPYC("tests/test57.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test16.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test15.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test1.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test2.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test3.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test6.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test13.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test18.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test19.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test22.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test7.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test5.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test4.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test27.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test28.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test23.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test56.pyc");
	if(exit_code)
		return(exit_code);

	//generators
	exit_code = AtomicOpenPYC("tests/test25.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test26.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test24b.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test24.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test52.pyc");
	if(exit_code)
		return(exit_code);

	//print + recursion
	exit_code = AtomicOpenPYC("tests/test17.pyc");
	if(exit_code)
		return(exit_code);
	
	//globals
	exit_code = AtomicOpenPYC("tests/test58.pyc");
	if(exit_code)
		return(exit_code);

	//brute ops + closures
	exit_code = AtomicOpenPYC("tests/test32.pyc");
	if(exit_code)
		return(exit_code);

	//builtins
	exit_code = AtomicOpenPYC("tests/test_range.pyc");
	if(exit_code)
		return(exit_code);
	//AtomicOpenPYC("tests/test_builtins2.pyc");
	exit_code = AtomicOpenPYC("tests/test_builtins.pyc");
	if(exit_code)
		return(exit_code);
	
	//simple generator with yield
	exit_code = AtomicOpenPYC("tests/test59.pyc");
	if(exit_code)
		return(exit_code);

	//simple print 
	exit_code = AtomicOpenPYC("tests/test_print.pyc");
	if(exit_code)
		return(exit_code);

	//basic stuff
	exit_code = AtomicOpenPYC("tests/test_yield.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_next.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_next2.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_next3.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_list.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test_return.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test8.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test9.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test12.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test11.pyc");
	if(exit_code)
		return(exit_code);

	//while loop + break + continue
	exit_code = AtomicOpenPYC("tests/test_while.pyc");
	if(exit_code)
		return(exit_code);

	
	//loops + recursion
	exit_code = AtomicOpenPYC("tests/test35.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test34.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test33.pyc");
	if(exit_code)
		return(exit_code);

	//nested tuple printing
	exit_code = AtomicOpenPYC("tests/test51.pyc");
	if(exit_code)
		return(exit_code);
	

	//closures and deref opcodes
	exit_code = AtomicOpenPYC("tests/test57.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test29.pyc");
	if(exit_code)
		return(exit_code);
	
	//ellipsis object
	exit_code = AtomicOpenPYC("tests/test43.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test40.pyc");
	if(exit_code)
		return(exit_code);

	//most binary ops test
	exit_code = AtomicOpenPYC("tests/test14.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test14b.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test14f.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test14bf.pyc");
	if(exit_code)
		return(exit_code);

	//floats
	exit_code = AtomicOpenPYC("tests/test44.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test42.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test41.pyc");
	if(exit_code)
		return(exit_code);

	//dictionaries
	exit_code = AtomicOpenPYC("tests/test30.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/test31.pyc");
	if(exit_code)
		return(exit_code);
	
	//brute prime
	exit_code = AtomicOpenPYC("tests/e20.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/e_small.pyc");	
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/e_med.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/e_big.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/e_bigger.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/e_max.pyc");
	if(exit_code)
		return(exit_code);
	exit_code = AtomicOpenPYC("tests/Queens2a.pyc");
	//exit_code = AtomicOpenPYC("tests/e_huge.pyc");
	//if(exit_code)
	//	return(exit_code);
	return(exit_code);
}

int main(int argc, char *argv[])
{
	//ptr_tests();
	//return;
	//stream_tests();
	//brute_test();
	NUM exit_code = atomic_test();
/*	for(int i=0;i<100000;i++)
	{
		printf("[[[[[%d]]]]]\n",i);
		char *tmp = (char*)malloc(i);
		atomic_test();
		//mem_Init();
		//ptr_tests();
		//mem_Close();
		free(tmp);
	}*/
	//AtomicOpenPYC("tests/test_nested_function.pyc");
	//AtomicOpenPYC("tests/test_function_default_value.pyc");
	//AtomicOpenPYC("tests/test_function_default_value2.pyc");
	//AtomicOpenPYC("tests/test_functions.pyc");
	//AtomicOpenPYC("tests/test_function_kw.pyc");
	//	AtomicOpenPYC("tests/e_med.pyc");
	return(exit_code);
}
