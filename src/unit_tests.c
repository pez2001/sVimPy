#define DEBUG
#include "unit_tests.h"

//STEFFI 0174 656 9762


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
 object *obj = ReadObject(f);

 printf("read pyc file\n");
 object *ret = ExecuteObject(obj,NULL,obj);
 FreeObject(obj);
 fclose(f);
 //mem_free(b);
 //mem_free(bh);
}


int main(int argc,char *argv[])
{
 mem_Init();
 stack_Init();
 printf("Calling all Unit Tests\n");
 OpenPYC();
 stack_Close();
 mem_Close();
 printf("%d memory chunks leaked\n",mem_chunks_num);
 return(0);
}
