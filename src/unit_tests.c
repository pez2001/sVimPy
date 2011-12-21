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



void
ptr_tests ()
{
  printf ("testing ptr list functions\n");
  ptr_list *p = ptr_CreateList (0,0);

  ptr_Push (p, 1);
  ptr_Push (p, 2);
  ptr_Push (p, 3);
  ptr_Push (p, 4);
  printf ("push(1,2,3,4):\n");
  for (int i = 0; i < p->num; i++)
    printf ("%d ", p->items[i]);
  printf ("\n");
  long x = ptr_Pop (p);

  printf ("pop(): %d\n", x);
  ptr_Push (p, 5);
  printf ("push(5):\n");
  for (int i = 0; i < p->num; i++)
    printf ("%d ", p->items[i]);
  printf ("\n");
  long r = ptr_Remove (p, 2);

  printf ("remove(2): %d\n", r);
  for (int i = 0; i < p->num; i++)
    printf ("%d ", p->items[i]);
  printf ("\ninserting\n");
  ptr_Insert (p, 3, 66);
  printf ("insert 3,66:\n");
  for (int i = 0; i < p->num; i++)
    printf ("%d ", p->items[i]);
  printf ("\ninserting\n");
  ptr_Insert (p, 0, 99);
  printf ("insert 0,99:\n");
  for (int i = 0; i < p->num; i++)
    printf ("%d ", p->items[i]);
  printf ("\n");
  ptr_Set (p, 1, 22);
  printf ("set 1,22:\n");
  for (int i = 0; i < p->num; i++)
    printf ("%d ", p->items[i]);
  printf ("\n");
  ptr_Queue (p, 88);
  printf ("queue(88):\n");
  for (int i = 0; i < p->num; i++)
    printf ("%d ", p->items[i]);
  printf ("\n");
  ptr_CloseList (p);
  printf ("ptr tests thru\n");
}



long
ReadLong (FILE * f)
{
  long r = 0;

  //char *b = (char*)mem_malloc(4);
  int read = fread (&r, 4, 1, f);

  //r = *(long*)b;
  //free(b);
  return (r);
}

char
ReadChar (FILE * f)
{
  char r = 0;

  int read = fread (&r, 1, 1, f);

  return (r);
}

void
OpenPYC (vm * vm)
{
  //char *b = (char*)mem_malloc(3);
  //char *bh = (char*)mem_malloc(4);
  //memset(bh,0,4);

  //printf("opening pyc file\n");
  FILE *f;

  f = fopen ("tests/e.pyc", "rb");
  if (f == NULL)
    return;
  //int r = fread(bh,4,1,f);
  //if(!memcmp(bh,(char*)&pyc_magic,4))
  long magic = ReadLong (f);

  if (magic != pyc_magic)
    return;
  long time = ReadLong (f);

  /*struct tm *ti; //TO DECREASE MEMORY USAGE
     ti = localtime((void*)&time);
     char *bt = (char*)mem_malloc(100);
     strftime(bt,100,"%Y.%m.%d ",ti);
     printf("file date: %s\n", bt); 
     strftime(bt,100,"%H:%M:%S \n",ti);
     printf("file time: %s\n", bt);
     mem_free(bt);
     //8 bytes into the file
   */
  //printf("reading object\n");
  object *obj = ReadObject (f);

  //printf("read pyc file\n");
  //printf("heap bytes used by objects:%d\n",mem_chunks_actual_size);
  //printf("MAX HEAP USAGE:%d\n",mem_chunks_max_size);
  //printf("objects num: %d\n",objects_num);
  //printf("max objects : %d\n",objects_max);
  //printf("objects headers total size  : %d\n",objects_header_total);
  vm_SetGlobal (vm, obj);
  DumpObject(obj,0);
  object *ret = vm_RunObject (vm, obj, obj, NULL, 0);	//,obj

  printf ("object executed\n");
  //DumpObject(obj,0);
  //printf("cleaning up object\n");
  FreeObject (obj);
  //printf("objects headers total size  : %d\n",objects_header_total);
  fclose (f);
  //mem_free(b);
  //mem_free(bh);

}


int
main (int argc, char *argv[])
{
  mem_Init ();
  //ptr_tests ();
  vm *vm = vm_Init (NULL);

  function_definition *build_list =
    CreateCFunction (&BuildList, "internal.BuildList");
  vm_AddFunctionDefinition (vm, build_list);
  function_definition *range = CreateCFunction (&if_range, "range");

  vm_AddFunctionDefinition (vm, range);
  function_definition *print = CreateCFunction (&if_print, "print");

  vm_AddFunctionDefinition (vm, print);
  //vm_RemoveFunction (vm, "range");
  //printf("Calling all Unit Tests\n");
  OpenPYC (vm);
  //printf("clearing recycle stack\n");
  //stack_Dump(vm->recycle);
  printf ("closing vm\n");
  vm_Close (vm);
  //printf("objects headers total size  : %d\n",objects_header_total);
  mem_Close ();
  printf ("%d memory chunks leaked\n", mem_chunks_num);
  DumpUnsupportedOpCodes ();
  return (0);
}
