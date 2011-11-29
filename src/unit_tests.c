#include "unit_tests.h"

long ReadLong(FILE *f)
{
 long r = 0;
 char *b = (char*)malloc(4);
 int read = fread(b,4,1,f);
 r = *(long*)b;
 free(b);
 return(r);
}
char ReadChar(FILE *f)
{
 char r = 0;
 int read = fread(&c,1,1,f);
 return(r);
}

void OpenPYC()
{
 char *b = (char*)malloc(3);
 char *bh = (char*)malloc(4);
 memset(bh,0,4);
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
 struct tm *ti;
 ti = localtime((time_t*)&time);
 char *bt = (char*)malloc(100);
 strftime(bt,100,"%Y.%m.%d ",ti);
 printf("file date: %s\n", bt); 
 strftime(bt,100,"%H:%M:%S \n",ti);
 printf("file time: %s\n", bt);
 free(bt);
 //8 bytes into the file


 fclose(f);
 free(b);
 free(bh);
}


int main(int argc,char *argv[])
{
 printf("Calling all Unit Tests\n");
 OpenPYC();
 return(0);
}
