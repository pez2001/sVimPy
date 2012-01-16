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

#define DEBUG
#include "unit_tests.h"

int debug_level = 0;

void ptr_tests()
{
	printf("testing ptr list functions\n");
	ptr_list *p = ptr_CreateList(0, 0);

	ptr_Push(p, 1);
	ptr_Push(p, 2);
	ptr_Push(p, 3);
	ptr_Push(p, 4);
	printf("push(1,2,3,4):\n");
	for (int i = 0; i < p->num; i++)
		printf("%d ", p->items[i]);
	printf("\n");
	long x = ptr_Pop(p);

	printf("pop(): %d\n", x);
	ptr_Push(p, 5);
	printf("push(5):\n");
	for (int i = 0; i < p->num; i++)
		printf("%d ", p->items[i]);
	printf("\n");
	long r = ptr_Remove(p, 2);

	printf("remove(2): %d\n", r);
	for (int i = 0; i < p->num; i++)
		printf("%d ", p->items[i]);
	printf("\ninserting\n");
	ptr_Insert(p, 3, 66);
	printf("insert 3,66:\n");
	for (int i = 0; i < p->num; i++)
		printf("%d ", p->items[i]);
	printf("\ninserting\n");
	ptr_Insert(p, 0, 99);
	printf("insert 0,99:\n");
	for (int i = 0; i < p->num; i++)
		printf("%d ", p->items[i]);
	printf("\n");
	ptr_Set(p, 1, 22);
	printf("set 1,22:\n");
	for (int i = 0; i < p->num; i++)
		printf("%d ", p->items[i]);
	printf("\n");
	ptr_Queue(p, 88);
	printf("queue(88):\n");
	for (int i = 0; i < p->num; i++)
		printf("%d ", p->items[i]);
	printf("\n");

	ptr_Clear(p);
	ptr_Push(p, 0);
	for (int i = 0; i < 20000; i++)
		ptr_Push(p, 1);
	for (int i = 0; i < 20000; i++)
		ptr_Pop(p);
	printf("first:%d \n", ptr_Pop(p));
	printf("top:%d\n", p->num);

	for (int i = 0; i < 20000; i++)
	{
		ptr_Push(p, 0);
		ptr_Pop(p);
	}

	ptr_CloseList(p);
	printf("ptr tests thru\n");
}

void OpenPYC(char *filename, vm *vm)
{
	// char *b = (char*)mem_malloc(3);
	// char *bh = (char*)mem_malloc(4);
	// memset(bh,0,4);

	// printf("opening pyc file\n");
	FILE *f;

	f = fopen(filename, "rb");
	if (f == NULL)
		return;
	if((debug_level & DEBUG_VERBOSE_TESTS) > 0)
		printf("executing object:%s\n", filename);
	// int r = fread(bh,4,1,f);
	// if(!memcmp(bh,(char*)&pyc_magic,4))
	long magic = ReadLong(f);

	if (magic != pyc_magic)
		return;
	long time = ReadLong(f);

	/* struct tm *ti; //TO DECREASE MEMORY USAGE ti = localtime((void*)&time);
	   char *bt = (char*)mem_malloc(100); strftime(bt,100,"%Y.%m.%d ",ti);
	   printf("file date: %s\n", bt); strftime(bt,100,"%H:%M:%S \n",ti);
	   printf("file time: %s\n", bt); mem_free(bt); //8 bytes into the file */
	// printf("reading object\n");
	object *obj = ReadObject(f);
	//object *obj = NULL;
	//return;
	//printf("read pyc file\n");
	// printf("heap bytes used by objects:%d\n",mem_chunks_actual_size);
	// printf("MAX HEAP USAGE:%d\n",mem_chunks_max_size);
	// printf("objects num: %d\n",objects_num);
	// printf("max objects : %d\n",objects_max);
	// printf("objects headers total size : %d\n",objects_header_total);
	vm_SetGlobal(vm, obj);
	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		DumpObject(obj, 0);
	object *ret = NULL;
	if((debug_level & DEBUG_INTERACTIVE) > 0)
		ret = vm_InteractiveRunObject(vm, obj, NULL, 0);	// ,obj
	else
		ret = vm_RunObject(vm, obj, NULL, 0);	// ,obj
    //object *ret = NULL;
	printf("\n");
	if (ret != NULL)
	{
		if((debug_level & DEBUG_DUMP_OBJECT) > 0)
			DumpObject(ret, 0);
		FreeObject(ret);
	}
	if((debug_level & DEBUG_VERBOSE_TESTS) > 0)
		printf("object executed:%s\n", filename);

	if((debug_level & DEBUG_DUMP_OBJECT) > 0)
		DumpObject(obj,0);

	if((debug_level & DEBUG_VERBOSE_TESTS) > 0)
		printf("cleaning up object\n");
	FreeObject(obj);
	// printf("objects headers total size : %d\n",objects_header_total);
	fclose(f);
	// mem_free(b);
	// mem_free(bh);

}

void AddInternalFunctions(vm *vm)
{
	function_object *list = CreateCFunction(&if_list, "list");
	function_object *range = CreateCFunction(&if_range, "range");
	function_object *print = CreateCFunction(&if_print, "print");
	function_object *sum = CreateCFunction(&if_sum, "sum");
	vm_AddFunctionObject(vm, list);
	vm_AddFunctionObject(vm, range);
	vm_AddFunctionObject(vm, print);
	vm_AddFunctionObject(vm, sum);

	function_object *cc = CreateCFunction(&custom_code, "custom_code");
	vm_AddFunctionObject(vm, cc);

}

int main(int argc, char *argv[])
{

	debug_level |= DEBUG_INTERACTIVE;
	debug_level |= DEBUG_MEMORY;
	debug_level |= DEBUG_SHOW_OPCODES;
	debug_level |= DEBUG_FULL_DUMP;
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
	//ptr_tests();
	vm *vm = vm_Init(NULL);
	AddInternalFunctions(vm);
	// printf("Calling all Unit Tests\n");



	//closures and deref opcodes
	OpenPYC("tests/test29.pyc", vm);//not supported opcode store_deref
	
	//append ops
	OpenPYC("tests/test52.pyc", vm);
	OpenPYC("tests/test53.pyc", vm);
	
	//brute ops + closures
	OpenPYC("tests/test32.pyc", vm);
	
	
	//crashing or leaking memory

	/*
	//custom code + import_from + import_star opcodes
	OpenPYC("tests/test45.pyc", vm);
	
	//function parameters
	OpenPYC("tests/test50.pyc", vm);//with keywords unordered
	OpenPYC("tests/test49b.pyc", vm);//kw unordered
	OpenPYC("tests/test49.pyc", vm);//kw
	OpenPYC("tests/test48.pyc", vm);//var
	OpenPYC("tests/test47.pyc", vm);
	OpenPYC("tests/test46.pyc", vm);
	OpenPYC("tests/test_functions.pyc", vm);
	OpenPYC("tests/test36.pyc", vm);//call_function_var
	OpenPYC("tests/test37.pyc", vm);//call_function_kw
	OpenPYC("tests/test38.pyc", vm);//call_function_var_kw 
	OpenPYC("tests/test39.pyc", vm);//call_function
	
	//storing of globals/vars and deletion of globals/vars
	OpenPYC("tests/test21ba2.pyc", vm);
	OpenPYC("tests/test21.pyc", vm);
	OpenPYC("tests/test21b.pyc", vm);
	OpenPYC("tests/test21ba.pyc", vm);
	OpenPYC("tests/test21ba3.pyc", vm);
	OpenPYC("tests/test21bb.pyc", vm);
	OpenPYC("tests/test21c.pyc", vm);
	OpenPYC("tests/test21d.pyc", vm);
	OpenPYC("tests/test21e.pyc", vm);
	OpenPYC("tests/test21a.pyc", vm);
	
	//loops + recursion
	OpenPYC("tests/test35.pyc", vm);
	OpenPYC("tests/test34.pyc", vm);
	OpenPYC("tests/test33.pyc", vm);

	//print + recursion
	OpenPYC("tests/test8.pyc", vm);
	OpenPYC("tests/test9.pyc", vm);
	OpenPYC("tests/test17.pyc", vm);
	OpenPYC("tests/test12.pyc", vm);
	OpenPYC("tests/test11.pyc", vm);

	//nested tuple printing
	OpenPYC("tests/test51.pyc", vm);
	
	//comparing
	OpenPYC("tests/test_compare.pyc", vm);

	//simple stuff
	OpenPYC("tests/test1.pyc", vm);
	OpenPYC("tests/test2.pyc", vm);
	OpenPYC("tests/test3.pyc", vm);
	OpenPYC("tests/test6.pyc", vm);
	OpenPYC("tests/test13.pyc", vm);
	OpenPYC("tests/test15.pyc", vm);
	OpenPYC("tests/test16.pyc", vm);
	OpenPYC("tests/test18.pyc", vm);
	OpenPYC("tests/test19.pyc", vm);
	OpenPYC("tests/test22.pyc", vm);
	OpenPYC("tests/test7.pyc", vm);
	OpenPYC("tests/test5.pyc", vm);
	OpenPYC("tests/test4.pyc", vm);
	OpenPYC("tests/test27.pyc", vm);
	OpenPYC("tests/test28.pyc", vm);
	OpenPYC("tests/test23.pyc", vm);
	OpenPYC("tests/e_small.pyc", vm);
	OpenPYC("tests/e20.pyc", vm);
	
	
	//while loop + break + continue
	OpenPYC("tests/test_while.pyc", vm);

	//ellipsis object
	OpenPYC("tests/test43.pyc", vm);
	OpenPYC("tests/test40.pyc", vm);

	//most binary ops test
	OpenPYC("tests/test14.pyc", vm);
	OpenPYC("tests/test14b.pyc", vm);
	OpenPYC("tests/test14f.pyc", vm);
	OpenPYC("tests/test14bf.pyc", vm);

	//floats
	OpenPYC("tests/test44.pyc", vm);
	OpenPYC("tests/test42.pyc", vm);
	OpenPYC("tests/test41.pyc", vm);

	//dictionaries
	OpenPYC("tests/test30.pyc", vm);
	OpenPYC("tests/test31.pyc", vm);

	//brute prime (classless it takes longer because of range
	OpenPYC("tests/e_med.pyc", vm);
	OpenPYC("tests/e_bigger.pyc", vm);*/

	//generators
	//OpenPYC("tests/test24.pyc", vm);
	//OpenPYC("tests/test25.pyc", vm);
	//OpenPYC("tests/test26.pyc", vm);
	//OpenPYC("tests/test20.pyc", vm);//leaking

	//OpenPYC("tests/test.pyc", vm);
	//OpenPYC("tests/test10.pyc", vm);
	//OpenPYC("tests/e.pyc", vm);
	
	if((debug_level & DEBUG_VERBOSE_TESTS) > 0)
		printf("closing vm\n");
	vm_Close(vm);
	// printf("objects headers total size : %d\n",objects_header_total);
	mem_Close();
	if((debug_level & DEBUG_MEMORY) > 0)
		printf("%d memory chunks leaked\n", mem_chunks_num);
	if((debug_level & DEBUG_DUMP_UNSUPPORTED) > 0)
		DumpUnsupportedOpCodes();
	return (0);
}
