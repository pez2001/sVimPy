#define DEBUG
#include "unit_tests.h"

//STEFFI 0174 656 9762


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
 int read = fread(&r,1,1,f);
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
 ti = localtime((void*)&time);
 char *bt = (char*)malloc(100);
 strftime(bt,100,"%Y.%m.%d ",ti);
 printf("file date: %s\n", bt); 
 strftime(bt,100,"%H:%M:%S \n",ti);
 printf("file time: %s\n", bt);
 free(bt);
 //8 bytes into the file
 object *obj = ReadObject(f);
 if(obj->type == TYPE_CODE)
 {
  printf("showing bytecodes\n");
  code_object *co = (code_object*)obj->ptr;
  //printf("co_ptr=%x\n",(unsigned long)co);
  printf("stack size:%d\n",co->stacksize);
  string_object *bytecodes = (string_object*)co->code->ptr;
  char *string = bytecodes->content;
  long n = bytecodes->len;
  stack_Init();
  printf("code length: %d\n",n);
  //printf("str_ptr=%x\n",(unsigned long)bytecodes->content);
 //printf("str_so_ptr=%x\n",(unsigned long)bytecodes);
 for(int i=0;i<n;i++)
 {
  printf("%x ",(unsigned char)string[i]);
 }
 printf("\n");
  int i=0;
  while(i<n)
  {
  int index = GetOpcodeIndex((unsigned char)string[i]);
  //printf("opcode:",GetOpcodeName(index));
  //i = i + GetOpcodeArgcount(index);
  if(index >=0)
  {
  tuple_object* co_consts;
  char const_i;
  char name_i;
  tuple_object* co_names;
  unicode_object* name;
  unicode_object* const_content;
  object *tos;
  object *tos1;
  object *tos2;
  switch(opcodes[index].opcode)
  {
   case 0x65:
    printf("");
    //printf("push name\n");
	co_names = (tuple_object*)co->names->ptr;
	name_i = string[i+1];
	name = (unicode_object*)co_names->items[name_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
	stack_Push(co_names->items[name_i]);
    break;
   case 0x64:
    printf("");
    //printf("push const\n");
	co_consts = (tuple_object*)co->consts->ptr;
	const_i = string[i+1];
	const_content = (unicode_object*)co_consts->items[const_i]->ptr;
    //if(co_consts->items[const_i]->type == TYPE_UNICODE)
	// printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,const_i,const_content->content);
	//else
	// printf("opcode: [%s],(%d) [%c] not a unicode object\n",opcodes[index].name,const_i,co_consts->items[const_i]->type);
    stack_Push(co_consts->items[const_i]);
	break;

  case 0x01:
    //printf("Removing TOS\n");
   stack_Pop();
   break;
   case 0x5a:
   printf("");
   //printf("storing in name\n");
   object* obj = stack_Pop();
	co_names = (tuple_object*)co->names->ptr;
	name_i = string[i+1];
	name = (unicode_object*)co_names->items[name_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    co_names->items[name_i]->value_ptr = obj;
   break;
   case 0x17:
   printf("");
   tos = stack_Pop();
   tos1 = stack_Pop();
   //printf("tos.type:%c (%d)\n",(long)tos->type,tos->ptr);
   //printf("tos1.type:%c (%s)(%d)\n",tos1->type,((unicode_object*)tos1->ptr)->content,((object*)tos1->value_ptr)->ptr );
   long a = 0;
   long b = 0;
   if(tos->type == TYPE_INT)
   {
    a = tos->ptr;
   }
   else if(tos->type == TYPE_UNICODE)
   {
   a = ((object*)tos->value_ptr)->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    b = tos->ptr;
   }
   else if(tos1->type == TYPE_UNICODE)
   {
   b = ((object*)tos1->value_ptr)->ptr;
   }
   
    if(tos->type == TYPE_INT)
    {
    tos->ptr = a + b;
	}
    if(tos->type == TYPE_UNICODE)
    {
    ((object*)tos1->value_ptr)->ptr = a + b;
	}
	//printf("%d + %d = %d\n",a,b,a+b);
    stack_Push(tos);
   
   break;
  case 0x83:
    printf("");
    //printf("calling function\n");
	 char argc = string[i+1];
	 //printf("opcode: [%s],(argc:%d)\n",opcodes[index].name,argc);
	 tos = NULL;
	 for(int i = 0;i<argc;i++)
	 {
	  callstack_Push(stack_Pop());
	 }
	 object *function_name = stack_Pop();
	 if(function_name != NULL&& function_name->type == TYPE_UNICODE)
	   //printf("calling %s(\"%s\")\n",((unicode_object*)function_name->ptr)->content,((unicode_object*)tos->ptr)->content );
	  if(!strcmp(((unicode_object*)function_name->ptr)->content,"print"))
	  {
	  	 for(int i = 0;i<argc;i++)
	    {
 	   tos = callstack_Pop();
	  //if(tos != NULL && tos->type == TYPE_UNICODE)
	   //printf("arg[%d]:%s\n",i,((unicode_object*)tos->ptr)->content );
	   //else 
	   if(tos != NULL)
	    //printf("tos type:%c\n",tos->type);
		switch(tos->type)
		{
		case TYPE_INT:
		 printf("%d",(long)tos->ptr);
		 break;
		case TYPE_UNICODE:
		 if((object*)tos->value_ptr != NULL)
		  {
		  if(((object*)tos->value_ptr)->type == TYPE_UNICODE)
		   printf("%s",((unicode_object*)(((object*)tos->value_ptr)->ptr))->content );
		   else
		   if(((object*)tos->value_ptr)->type == TYPE_INT)
		   printf("%d",((object*)tos->value_ptr)->ptr );
		  }
		  else 
		  printf("%s",((unicode_object*)tos->ptr)->content);
		 break;
		}
		 //else 
		 //printf("tos = NULL\n");
	 }

	   //printf("%s",((unicode_object*)tos->ptr)->content); 
	   }
	   else 
	    printf("function: [%s] not found\n",((unicode_object*)function_name->ptr)->content);
 
	 break;
 
	
  default:
  printf("\n[%d,%xh] opcode: [ %s ] ,argcount:%d not supported\n(%s)\n",i,opcodes[index].opcode,opcodes[index].name,opcodes[index].argcount,opcodes[index].description);
   break;
  }
  //if(opcodes[index].argcount==2)
   //printf("\targ1:%d,arg2:%d\n",string[i+1],string[i+2]);
  i = i + 1 + opcodes[index].argcount;
  }
  else
  {
  printf("unknown opcode:%x at %d\n",(char)string[i],i);
   i++;
   }
   
 }
 printf("code thru.\n");
 
 }
 

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
