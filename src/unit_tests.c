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


#ifdef DEBUGGING
int debug_level = 0;

void ptr_tests(void)
{
	printf("testing ptr list functions\n");
	ptr_list *p = ptr_CreateList(0, 0);

	ptr_Push(p, (void*)1);
	ptr_Push(p, (void*)2);
	ptr_Push(p, (void*)3);
	ptr_Push(p, (void*)4);
	printf("push(1,2,3,4):\n");
	for (NUM i = 0; i < p->num; i++)
		printf("%d ", (int)p->items[i]);
	printf("\n");
	int x =(int)ptr_Pop(p);

	printf("pop(): %d\n", x);
	ptr_Push(p,  (void*)5);
	printf("push(5):\n");
	for (NUM i = 0; i < p->num; i++)
		printf("%d ", p->items[i]);
	printf("\n");
	int r = (int)ptr_Remove(p, 2);

	printf("remove(2): %d\n", r);
	for (NUM i = 0; i < p->num; i++)
		printf("%d ",(int)p->items[i]);
	printf("\ninserting\n");
	ptr_Insert(p, 3, (void*)66);
	printf("insert 3,66:\n");
	for (NUM i = 0; i < p->num; i++)
		printf("%d ",(int)p->items[i]);
	printf("\ninserting\n");
	ptr_Insert(p, 0, (void*)99);
	printf("insert 0,99:\n");
	for (NUM i = 0; i < p->num; i++)
		printf("%d ",(int)p->items[i]);
	printf("\n");
	ptr_Set(p, 1,(void*)22);
	printf("set 1,22:\n");
	for (NUM i = 0; i < p->num; i++)
		printf("%d ",(int)p->items[i]);
	printf("\n");
	ptr_Queue(p, (void*)88);
	printf("queue(88):\n");
	for (NUM i = 0; i < p->num; i++)
		printf("%d ",(int)p->items[i]);
	printf("\n");

	ptr_Clear(p);
	ptr_Push(p, 0);
	for (NUM i = 0; i < 20000; i++)
		ptr_Push(p, (void*)1);
	for (NUM i = 0; i < 20000; i++)
		ptr_Pop(p);
	printf("first:%d \n", (int)ptr_Pop(p));
	printf("top:%d\n", p->num);

	for (NUM i = 0; i < 20000; i++)
	{
		ptr_Push(p, (void*)0);
		ptr_Pop(p);
	}

	ptr_CloseList(p);
	printf("ptr tests thru\n");
}

void stream_tests(void)
{
	long pyc_magic = MAGIC;
	printf("stream tests\n");
	stream *f = stream_CreateFromFile("tests/test52.pyc");
	
	if (!stream_Open(f))
	{
		printf("stream cant be opened\n");
		return;
	}
	printf("stream opened\n");
	long magic = ReadLong(f);

	if (magic != pyc_magic)
	{
		printf("wrong magic:%d (%d)\n",magic,pyc_magic);
		return;
	}
	printf("read magic:%d\n",magic);
	stream_Free(f);
	
	stream *m = stream_CreateFromBytes((char*)&test_pyc,TEST_PYC_LEN);
	printf("opening mstream\n");
	if (!stream_Open(m))
	{
		printf("mstream cant be opened\n");
		return;
	}
	printf("mstream opened\n");
	long mmagic = ReadLong(m);

	if (mmagic != pyc_magic)
	{
		printf("wrong magic:%d (%d)\n",mmagic,pyc_magic);
		return;
	}
	printf("read magic:%d\n",mmagic);
	stream_Free(m);
	
	
	
	
	printf("stream tests done\n");
	
}
#endif

void OpenPYC(stream *f, vm *vm)
{
	// char *b = (char*)mem_malloc(3);
	// char *bh = (char*)mem_malloc(4);
	// memset(bh,0,4);
	long pyc_magic = MAGIC;
	//printf("opening pyc\n");
	//stream *f = stream_CreateFromFile(filename);
	
	if (!stream_Open(f))
		return;
	//#ifdef DEBUGGING
	//debug_printf(DEBUG_VERBOSE_TESTS,"executing object:%s\n", filename);
	//#endif
	// int r = fread(bh,4,1,f);
	// if(!memcmp(bh,(char*)&pyc_magic,4))
	long magic = ReadLong(f);

	if (magic != pyc_magic)
		return;
	ReadLong(f);//skip time

	/* struct tm *ti; //TO DECREASE MEMORY USAGE ti = localtime((void*)&time);
	   char *bt = (char*)mem_malloc(100); strftime(bt,100,"%Y.%m.%d ",ti);
	   printf("file date: %s\n", bt); strftime(bt,100,"%H:%M:%S \n",ti);
	   printf("file time: %s\n", bt); mem_free(bt); //8 bytes into the file */
	//printf("reading object\n");
	object *obj = ReadObject(f);
	//object *obj = NULL;
	//return;
	//printf("read pyc\n");
	// printf("heap bytes used by objects:%d\n",mem_chunks_actual_size);
	// printf("MAX HEAP USAGE:%d\n",mem_chunks_max_size);
	// printf("objects num: %d\n",objects_num);
	// printf("max objects : %d\n",objects_max);
	// printf("objects headers total size : %d\n",objects_header_total);
	vm_AddGlobal(vm, (code_object*)obj);
	#ifdef DEBUGGING
	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		DumpObject(obj, 0);
	#endif
	object *ret = NULL;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_INTERACTIVE) > 0)
		ret = vm_InteractiveRunObject(vm, obj, NULL, 0);	// ,obj
	else
		ret = vm_RunObject(vm, obj, NULL, 0);	// ,obj
	#else
	ret = vm_RunObject(vm, obj, NULL, 0);	// ,obj
	#endif
	#ifdef DEBUGGING
	debug_printf(DEBUG_ALL,"\n");
	#endif
	if (ret != NULL)
	{
		#ifdef DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			DumpObject(ret, 0);
		#endif
		FreeObject(ret);
	}
	#ifdef DEBUGGING
	//debug_printf(DEBUG_VERBOSE_TESTS,"object executed:%s\n", filename);
	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		DumpObject(obj,0);
	debug_printf(DEBUG_VERBOSE_TESTS,"cleaning up object\n");
	#endif
	//DumpObject(obj,0);
	FreeObject(obj);
	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"removing object from globals\n");
	#endif
	vm_RemoveGlobal(vm,(code_object*)obj);
	//printf("freeing stream\n");
	stream_Free(f);
	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"pyc executed\n");
	#endif
	//printf("pyc executed\n");
	// mem_free(b);
	// mem_free(bh);

}

void brute_test(void)
{
	#ifdef DEBUGGING
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
	mem_Init();
	#endif

	//debug_printf(DEBUG_ALL,"hi:%s\n","hi");

	//ptr_tests();
	streams_Init();
	//printf("streams initiated\n");
	//stream_tests();
	
	vm *vm = vm_Init(NULL);
	AddInternalFunctions(vm);
	//#ifdef USE_ARDUINO_FUNCTIONS
	//AddArduinoFunctions(vm);
	//AddArduinoGlobals(vm);
	//#endif
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
	stream *m = stream_CreateFromBytes((char*)&test_pyc,TEST_PYC_LEN);
	OpenPYC(m,vm);
	
	OpenPYC(stream_CreateFromFile("tests/test52.pyc"), vm);
	
	//generators
	OpenPYC(stream_CreateFromFile("tests/test25.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test24b.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test24.pyc"), vm);
	
	
	OpenPYC(stream_CreateFromFile("tests/test26.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test20.pyc"), vm);
	
	//simple generator with yield
	OpenPYC(stream_CreateFromFile("tests/test59.pyc"), vm);

	
	//iters
	OpenPYC(stream_CreateFromFile("tests/test61.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test60.pyc"), vm);

	//print + recursion
	OpenPYC(stream_CreateFromFile("tests/test17.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test8.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test9.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test12.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test11.pyc"), vm);
	
	//function parameters
	OpenPYC(stream_CreateFromFile("tests/test50.pyc"), vm);//with keywords unordered
	OpenPYC(stream_CreateFromFile("tests/test49b.pyc"), vm);//kw unordered
	OpenPYC(stream_CreateFromFile("tests/test49.pyc"), vm);//kw
	OpenPYC(stream_CreateFromFile("tests/test48.pyc"), vm);//var
	OpenPYC(stream_CreateFromFile("tests/test47.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test46.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test_functions.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test36.pyc"), vm);//call_function_var
	OpenPYC(stream_CreateFromFile("tests/test37.pyc"), vm);//call_function_kw
	OpenPYC(stream_CreateFromFile("tests/test38.pyc"), vm);//call_function_var_kw 
	OpenPYC(stream_CreateFromFile("tests/test39.pyc"), vm);//call_function
	
	//pos + kw defaults
	OpenPYC(stream_CreateFromFile("tests/test55c.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test55b.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test55.pyc"), vm);

	//kw defaults
	OpenPYC(stream_CreateFromFile("tests/test54.pyc"), vm);

	//storing of globals/vars and deletion of globals/vars
	OpenPYC(stream_CreateFromFile("tests/test21ba2.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test21.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test21b.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test21ba.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test21ba3.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test21bb.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test21c.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test21d.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test21e.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test21a.pyc"), vm);
	
	//loops + recursion
	OpenPYC(stream_CreateFromFile("tests/test35.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test34.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test33.pyc"), vm);

	//nested tuple printing
	OpenPYC(stream_CreateFromFile("tests/test51.pyc"), vm);
	
	//comparing
	OpenPYC(stream_CreateFromFile("tests/test_compare.pyc"), vm);

	//globals
	OpenPYC(stream_CreateFromFile("tests/test58.pyc"), vm);

	//closures and deref opcodes
	OpenPYC(stream_CreateFromFile("tests/test57.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test29.pyc"), vm);
	
	//brute ops + closures
	OpenPYC(stream_CreateFromFile("tests/test32.pyc"), vm);

	//simple stuff
	OpenPYC(stream_CreateFromFile("tests/test1.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test2.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test3.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test6.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test13.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test15.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test16.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test18.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test19.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test22.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test7.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test5.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test4.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test27.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test28.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test23.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test56.pyc"), vm);
	
	
	//while loop + break + continue
	OpenPYC(stream_CreateFromFile("tests/test_while.pyc"), vm);

	//ellipsis object
	OpenPYC(stream_CreateFromFile("tests/test43.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test40.pyc"), vm);

	//most binary ops test
	OpenPYC(stream_CreateFromFile("tests/test14.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test14b.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test14f.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test14bf.pyc"), vm);

	//floats
	OpenPYC(stream_CreateFromFile("tests/test44.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test42.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test41.pyc"), vm);

	//dictionaries
	OpenPYC(stream_CreateFromFile("tests/test30.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/test31.pyc"), vm);
	
	
	//brute prime (classless it takes longer because of range
	OpenPYC(stream_CreateFromFile("tests/e20.pyc"), vm);
	OpenPYC(stream_CreateFromFile("tests/e_small.pyc"), vm);
	
	//OpenPYC(stream_CreateFromFile("tests/e_med.pyc"), vm);
	
	//OpenPYC("tests/e_bigger.pyc", vm);
	//OpenPYC("tests/e_max.pyc", vm);

	//OpenPYC("tests/test.pyc", vm);
	//OpenPYC("tests/test10.pyc", vm);
	//OpenPYC("tests/e.pyc", vm);
	
	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing vm\n");
	#endif
	vm_Close(vm);
	streams_Close();
	// printf("objects headers total size : %d\n",objects_header_total);
	#ifdef DEBUGGING
	if((debug_level & DEBUG_DUMP_UNSUPPORTED) > 0)
		DumpUnsupportedOpCodes();
	
	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing memory manager\n");
	#endif
	mem_Close();
	if((debug_level & DEBUG_MEMORY)>0)
		printf("%d memory chunks leaked\n", mem_chunks_num);
	#endif


}

void AtomicOpenPYC(char *filename)
{
	#ifdef DEBUGGING
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
	mem_Init();
	#endif
	streams_Init();
	
	vm *vm = vm_Init(NULL);
	AddInternalFunctions(vm);

	long pyc_magic = MAGIC;
	stream *f = stream_CreateFromFile(filename);
	if (!stream_Open(f))
		return;
	//#ifdef DEBUGGING
	//debug_printf(DEBUG_VERBOSE_TESTS,"executing object:%s\n", filename);
	//#endif
	// int r = fread(bh,4,1,f);
	// if(!memcmp(bh,(char*)&pyc_magic,4))
	long magic = ReadLong(f);

	if (magic != pyc_magic)
		return;
	ReadLong(f);//skip time

	/* struct tm *ti; //TO DECREASE MEMORY USAGE ti = localtime((void*)&time);
	   char *bt = (char*)mem_malloc(100); strftime(bt,100,"%Y.%m.%d ",ti);
	   printf("file date: %s\n", bt); strftime(bt,100,"%H:%M:%S \n",ti);
	   printf("file time: %s\n", bt); mem_free(bt); //8 bytes into the file */
	//printf("reading object\n");
	object *obj = ReadObject(f);
	//object *obj = NULL;
	//return;
	//printf("read pyc\n");
	// printf("heap bytes used by objects:%d\n",mem_chunks_actual_size);
	// printf("MAX HEAP USAGE:%d\n",mem_chunks_max_size);
	// printf("objects num: %d\n",objects_num);
	// printf("max objects : %d\n",objects_max);
	// printf("objects headers total size : %d\n",objects_header_total);
	vm_AddGlobal(vm, (code_object*)obj);
	#ifdef DEBUGGING
	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		DumpObject(obj, 0);
	#endif
	object *ret = NULL;
	#ifdef DEBUGGING
	if((debug_level & DEBUG_INTERACTIVE) > 0)
		ret = vm_InteractiveRunObject(vm, obj, NULL, 0);	// ,obj
	else
		ret = vm_RunObject(vm, obj, NULL, 0);	// ,obj
	#else
	ret = vm_RunObject(vm, obj, NULL, 0);	// ,obj
	#endif
	#ifdef DEBUGGING
	debug_printf(DEBUG_ALL,"\n");
	#endif
	if (ret != NULL)
	{
		#ifdef DEBUGGING
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			DumpObject(ret, 0);
		#endif
		FreeObject(ret);
	}
	#ifdef DEBUGGING
	//debug_printf(DEBUG_VERBOSE_TESTS,"object executed:%s\n", filename);
	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		DumpObject(obj,0);
	debug_printf(DEBUG_VERBOSE_TESTS,"cleaning up object:%s\n",filename);
	#endif
	//DumpObject(obj,0);
	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"removing object from globals\n");
	#endif
	vm_RemoveGlobal(vm,(code_object*)obj);

	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"freeing object now\n");
	#endif
	FreeObject(obj);
	//printf("freeing stream\n");
	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"freeing stream\n");
	#endif
	stream_Free(f);
	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"pyc executed\n");
	#endif

	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing vm\n");
	#endif
	vm_Close(vm);
	streams_Close();
	// printf("objects headers total size : %d\n",objects_header_total);
	#ifdef DEBUGGING
	if((debug_level & DEBUG_DUMP_UNSUPPORTED) > 0)
		DumpUnsupportedOpCodes();
	
	#ifdef DEBUGGING
	debug_printf(DEBUG_VERBOSE_TESTS,"closing memory manager\n");
	#endif
	mem_Close();
	if((debug_level & DEBUG_MEMORY)>0)
		printf("%d memory chunks leaked\n", mem_chunks_num);
	#endif

}

void atomic_test(void)
{

	//import	
	//AtomicOpenPYC(stream_CreateFromFile("tests/test_import.pyc"), vm);

	//custom code + import_from + import_star opcodes
	//AtomicOpenPYC("tests/test45.pyc", vm);


	//append ops + generators
	//AtomicOpenPYC("tests/test53.pyc", vm);
	
	//simple generator with yield
	AtomicOpenPYC("tests/test59.pyc");
	//return;
	
	//simple print 
	AtomicOpenPYC("tests/test_print.pyc");
	//return;
	
	AtomicOpenPYC("tests/test_range.pyc");
	AtomicOpenPYC("tests/test_yield.pyc");
	AtomicOpenPYC("tests/test_next.pyc");
	AtomicOpenPYC("tests/test_next2.pyc");
	AtomicOpenPYC("tests/test_next3.pyc");
	AtomicOpenPYC("tests/test_list.pyc");
	AtomicOpenPYC("tests/test_return.pyc");
	//while loop + break + continue
	AtomicOpenPYC("tests/test_while.pyc");
	//return;

	//print + recursion
	AtomicOpenPYC("tests/test17.pyc");
	//return;
	AtomicOpenPYC("tests/test8.pyc");
	AtomicOpenPYC("tests/test9.pyc");
	AtomicOpenPYC("tests/test12.pyc");
	AtomicOpenPYC("tests/test11.pyc");
	
	//generators
	AtomicOpenPYC("tests/test25.pyc");
	//return;
	AtomicOpenPYC("tests/test24b.pyc");
	AtomicOpenPYC("tests/test24.pyc");
	
	AtomicOpenPYC("tests/test52.pyc");

	
	AtomicOpenPYC("tests/test26.pyc");
	AtomicOpenPYC("tests/test20.pyc");
	


	
	//iters
	AtomicOpenPYC("tests/test61.pyc");
	AtomicOpenPYC("tests/test60.pyc");

	
	
	//function parameters
	AtomicOpenPYC("tests/test50.pyc");//with keywords unordered
	AtomicOpenPYC("tests/test49b.pyc");//kw unordered
	AtomicOpenPYC("tests/test49.pyc");//kw
	AtomicOpenPYC("tests/test48.pyc");//var
	AtomicOpenPYC("tests/test47.pyc");
	AtomicOpenPYC("tests/test46.pyc");
	AtomicOpenPYC("tests/test_functions.pyc");
	AtomicOpenPYC("tests/test36.pyc");//call_function_var
	AtomicOpenPYC("tests/test37.pyc");//call_function_kw
	AtomicOpenPYC("tests/test38.pyc");//call_function_var_kw 
	AtomicOpenPYC("tests/test39.pyc");//call_function
	
	//pos + kw defaults
	AtomicOpenPYC("tests/test55c.pyc");
	AtomicOpenPYC("tests/test55b.pyc");
	AtomicOpenPYC("tests/test55.pyc");

	//kw defaults
	AtomicOpenPYC("tests/test54.pyc");

	//storing of globals/vars and deletion of globals/vars
	AtomicOpenPYC("tests/test21ba2.pyc");
	AtomicOpenPYC("tests/test21.pyc");
	AtomicOpenPYC("tests/test21b.pyc");
	AtomicOpenPYC("tests/test21ba.pyc");
	AtomicOpenPYC("tests/test21ba3.pyc");
	AtomicOpenPYC("tests/test21bb.pyc");
	AtomicOpenPYC("tests/test21c.pyc");
	AtomicOpenPYC("tests/test21d.pyc");
	AtomicOpenPYC("tests/test21e.pyc");
	AtomicOpenPYC("tests/test21a.pyc");
	
	//loops + recursion
	AtomicOpenPYC("tests/test35.pyc");
	AtomicOpenPYC("tests/test34.pyc");
	AtomicOpenPYC("tests/test33.pyc");

	//nested tuple printing
	AtomicOpenPYC("tests/test51.pyc");
	
	//comparing
	AtomicOpenPYC("tests/test_compare.pyc");

	//globals
	AtomicOpenPYC("tests/test58.pyc");

	//closures and deref opcodes
	AtomicOpenPYC("tests/test57.pyc");
	AtomicOpenPYC("tests/test29.pyc");
	
	//brute ops + closures
	AtomicOpenPYC("tests/test32.pyc");

	//simple stuff
	AtomicOpenPYC("tests/test1.pyc");
	AtomicOpenPYC("tests/test2.pyc");
	AtomicOpenPYC("tests/test3.pyc");
	AtomicOpenPYC("tests/test6.pyc");
	AtomicOpenPYC("tests/test13.pyc");
	AtomicOpenPYC("tests/test15.pyc");
	AtomicOpenPYC("tests/test16.pyc");
	AtomicOpenPYC("tests/test18.pyc");
	AtomicOpenPYC("tests/test19.pyc");
	AtomicOpenPYC("tests/test22.pyc");
	AtomicOpenPYC("tests/test7.pyc");
	AtomicOpenPYC("tests/test5.pyc");
	AtomicOpenPYC("tests/test4.pyc");
	AtomicOpenPYC("tests/test27.pyc");
	AtomicOpenPYC("tests/test28.pyc");
	AtomicOpenPYC("tests/test23.pyc");
	AtomicOpenPYC("tests/test56.pyc");
	
	
	//ellipsis object
	AtomicOpenPYC("tests/test43.pyc");
	AtomicOpenPYC("tests/test40.pyc");

	//most binary ops test
	AtomicOpenPYC("tests/test14.pyc");
	AtomicOpenPYC("tests/test14b.pyc");
	AtomicOpenPYC("tests/test14f.pyc");
	AtomicOpenPYC("tests/test14bf.pyc");

	//floats
	AtomicOpenPYC("tests/test44.pyc");
	AtomicOpenPYC("tests/test42.pyc");
	AtomicOpenPYC("tests/test41.pyc");

	//dictionaries
	AtomicOpenPYC("tests/test30.pyc");
	AtomicOpenPYC("tests/test31.pyc");
	
	
	//brute prime
	AtomicOpenPYC("tests/e20.pyc");
	AtomicOpenPYC("tests/e_small.pyc");
	
	//OpenPYC(stream_CreateFromFile("tests/e_med.pyc"), vm);
	
	//OpenPYC("tests/e_bigger.pyc", vm);
	//OpenPYC("tests/e_max.pyc", vm);

	//OpenPYC("tests/test.pyc", vm);
	//OpenPYC("tests/test10.pyc", vm);
	//OpenPYC("tests/e.pyc", vm);
	


}


int main(int argc, char *argv[])
{
	//ptr_tests();
	//stream_tests();
	//brute_test();
	atomic_test();
	return (0);
}
