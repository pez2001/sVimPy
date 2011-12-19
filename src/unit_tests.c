#define DEBUG
#include "unit_tests.h"

//STEFFI 0174 656 9762
stack *recycle;
stack *blocks;


long ReadLong(FILE *f)
{
 long r = 0;
 //char *b = (char*)mem_malloc(4);
 int read = fread(&r,4,1,f);
 //r = *(long*)b;
 //free(b);
 return(r);
}
char ReadChar(FILE *f)
{
 char r = 0;
 int read = fread(&r,1,1,f);
 return(r);
}

void OpenPYC()
{
 //char *b = (char*)mem_malloc(3);
 //char *bh = (char*)mem_malloc(4);
 //memset(bh,0,4);

 printf("opening pyc file\n");
 FILE *f;
 f=fopen("test.pyc","rb");
 if(f==NULL)
  return;
 //int r = fread(bh,4,1,f);
 //if(!memcmp(bh,(char*)&pyc_magic,4))
 long magic = ReadLong(f);
 if(magic == pyc_magic)
  printf("pyc magic found\n");
 else
  printf("not a pyc file");
 long time = ReadLong(f); 
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
 printf("reading object\n");
 object *obj = ReadObject(f);

 printf("read pyc file\n");
 printf("heap bytes used by objects:%d\n",mem_chunks_actual_size);
 printf("MAX HEAP USAGE:%d\n",mem_chunks_max_size);
 printf("objects num: %d\n",objects_num);
 printf("max objects : %d\n",objects_max);
 printf("objects headers total size  : %d\n",objects_header_total);
 
 DumpObject(obj,0);
 object *ret = ExecuteObject(obj,obj,obj,NULL,0);
 printf("object executed\n");
 DumpObject(obj,0);
 printf("cleaning up object\n");
 FreeObject(obj);
 printf("objects headers total size  : %d\n",objects_header_total);
 fclose(f);
 //mem_free(b);
 //mem_free(bh);
 
}


int main(int argc,char *argv[])
{
 mem_Init();
 vm_Init();
 function_definition *build_list = CreateCFunction(&BuildList,"internal.BuildList");
 AddFunctionDefinition(build_list);
 function_definition *range = CreateCFunction(&if_range,"range");
 AddFunctionDefinition(range);
 function_definition *print = CreateCFunction(&if_print,"print");
 AddFunctionDefinition(print);
 recycle = stack_Init(90);
 blocks = stack_Init(5);
 printf("Calling all Unit Tests\n");
 OpenPYC();
 printf("clearing recycle stack\n");
 //stack_Dump(recycle);
 stack_Close(recycle,1);
 //stack_Dump(blocks);
 stack_Close(blocks,1);
 printf("objects headers total size  : %d\n",objects_header_total);
 vm_Close();
 mem_Close();
 printf("%d memory chunks leaked\n",mem_chunks_num);
 DumpUnsupportedOpCodes();
 return(0);
}
