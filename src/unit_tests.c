#define DEBUG
#include "unit_tests.h"

//STEFFI 0174 656 9762
stack *recycle;

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
 printf("objects num * sizeof(object) : %d\n",objects_num * sizeof(object));
 
 DumpObject(obj,0);
 object *ret = ExecuteObject(obj,NULL,obj,NULL,0);
 FreeObject(obj);
 fclose(f);
 //mem_free(b);
 //mem_free(bh);
}


int main(int argc,char *argv[])
{
 mem_Init();
 recycle = stack_Init(20);
 printf("Calling all Unit Tests\n");
 OpenPYC();
 printf("clearing recycle stack\n");
 stack_Close(recycle,1);
 mem_Close();
 printf("%d memory chunks leaked\n",mem_chunks_num);
 return(0);
}
