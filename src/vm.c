#include "vm.h"


object *ExecuteObject(object *obj,object* caller,object *global)
{
 if(obj->type == TYPE_CODE)
 {
  //printf("showing bytecodes\n");
  code_object *co = (code_object*)obj->ptr;
  //printf("co_ptr=%x\n",(unsigned long)co);
  //printf("stack size:%d\n",co->stacksize);
  string_object *bytecodes = (string_object*)co->code->ptr;
  char *string = bytecodes->content;
  long n = bytecodes->len;
  //printf("code length: %d\n",n);
  //printf("str_ptr=%x\n",(unsigned long)bytecodes->content);
 //printf("str_so_ptr=%x\n",(unsigned long)bytecodes);
 /*
 for(int i=0;i<n;i++)
 {
  printf("%x ",(unsigned char)string[i]);
 }
 printf("\n");
 */
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
  char varname_i;
  tuple_object* co_names;
  tuple_object* co_varnames;
  unicode_object* name;
  unicode_object* varname;
  unicode_object* const_content;
  object *tos;
  object *tos1;
  object *tos2;
  //printf("\n[%d,%xh] opcode: [ %s ]\n",i,opcodes[index].opcode,opcodes[index].name);
  switch(opcodes[index].opcode)
  {
   case 0x74://LOAD_GLOBAL
    printf("");
    //printf("push global name\n");
    //code_object *co_global = (code_object*)global->ptr;
	//co_names = (tuple_object*)co_global->names->ptr;
	co_names = (tuple_object*)co->names->ptr;
	name_i = string[i+1];
	name = (unicode_object*)co_names->items[name_i]->ptr;
    //if(co_names->items[name_i]->type== TYPE_UNICODE)
	// printf("pushing global (%d)%s = %s\n",name_i,name->content,AsUnicodeObject(co_names->items[name_i])->content);
    //if(co_names->items[name_i]->type== TYPE_INT)
	// printf("pushing global (%d)%s = %d\n",name_i,name->content,co_names->items[name_i]->ptr);
	stack_Push(co_names->items[name_i]);
    break;
   case 0x65://LOAD_NAME
    printf("");
    //printf("push name\n");
	co_names = (tuple_object*)co->names->ptr;
	name_i = string[i+1];
	name = (unicode_object*)co_names->items[name_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    //if(co_names->items[name_i]->type== TYPE_UNICODE)
	// printf("pushing local (%d)%s = %s\n",name_i,name->content,AsUnicodeObject(co_names->items[name_i])->content);
    //if(co_names->items[name_i]->type== TYPE_INT)
	// printf("pushing local (%d)%s = %d\n",name_i,name->content,co_names->items[name_i]->ptr);
	stack_Push(co_names->items[name_i]);
    break;
   case 0x64://LOAD_CONST
    printf("");
    //printf("push const\n");
	co_consts = (tuple_object*)co->consts->ptr;
	const_i = string[i+1];
	const_content = (unicode_object*)co_consts->items[const_i]->ptr;
    //if(((object*)co_consts->items[const_i]->value_ptr)->type== TYPE_UNICODE)
	//printf("pushing local const (%d)%s = %s\n",const_i,const_content->content,AsUnicodeObject((object*)co_consts->items[const_i]->value_ptr)->content);
    //if(co_consts->items[const_i]->type== TYPE_INT)
	// printf("pushing local const (%d)%s = %d\n",const_i,const_content->content,co_consts->items[const_i]->ptr);
    stack_Push(co_consts->items[const_i]);
	break;
   case 0x7c://LOAD_FAST
    printf("");
    //printf("push name\n");
	co_varnames = (tuple_object*)co->varnames->ptr;
	varname_i = string[i+1];
	varname = (unicode_object*)co_varnames->items[varname_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    //if(co_varnames->items[varname_i]->type== TYPE_UNICODE)
	// printf("pushing local fast (%d,s)%s = %s\n",varname_i,varname->content,AsUnicodeObject((object*)co_varnames->items[varname_i]->value_ptr)->content);
    //if(co_varnames->items[varname_i]->type== TYPE_INT)
	// printf("pushing local fast (%d,i)%s = %d\n",varname_i,varname->content,co_varnames->items[varname_i]->ptr);
	stack_Push(co_varnames->items[varname_i]);
    break;

  case 0x01://POP_TOP
    printf("Removing TOS\n");
   stack_Pop();
   break;
   case 0x5a://STORE_NAME
   printf("");
   //printf("storing in name\n");
    tos = stack_Pop();
	co_names = (tuple_object*)co->names->ptr;
	name_i = string[i+1];
	name = (unicode_object*)co_names->items[name_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    if(tos->type == TYPE_UNICODE)
	 printf("storing %s in %s\n",((unicode_object*)tos->ptr)->content,name->content);
    if(tos->type == TYPE_INT)
	 printf("storing %d in %s\n",tos->ptr,name->content);
    if(tos->type == TYPE_CODE)
	 printf("storing function %s in %s\n",((code_object*)tos->ptr)->name,name->content);
	co_names->items[name_i]->value_ptr = tos;
   break;
   case 0x7d://STORE_FAST
   printf("");
   //printf("storing in fast\n");
    tos = stack_Pop();
	co_varnames = (tuple_object*)co->varnames->ptr;
	varname_i = string[i+1];
	varname = (unicode_object*)co_varnames->items[varname_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    if(tos->type == TYPE_UNICODE)
	 printf("fast storing %s in %s\n",((unicode_object*)tos->ptr)->content,varname->content);
    if(tos->type == TYPE_INT)
	 printf("fast storing %d in %s\n",tos->ptr,varname->content);
    if(tos->type == TYPE_CODE)
	 printf("fast storing function %s in %s\n",((code_object*)tos->ptr)->name,varname->content);
	co_varnames->items[varname_i]->value_ptr = tos;
   break;
   case 0x17: //BINARY_ADD
   printf("");
   tos = stack_Pop();
   tos1 = stack_Pop();
   //printf("tos1.type:%c (%s)(%d)\n",tos1->type,((unicode_object*)tos1->ptr)->content,((object*)tos1->value_ptr)->ptr );
   object* org_tos = tos;
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   //printf("tos.type:%c (%d)\n",(long)tos->type,tos->ptr);
   //printf("tos1.type:%c (%d)\n",(long)tos->type,tos->ptr);


	if(tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE)
    {
     char *tos_tmp = ((unicode_object*)tos->ptr)->content;
	 char *tos1_tmp = ((unicode_object*)tos1->ptr)->content;
     char *tmp = str_Cat(tos1_tmp,tos_tmp);
     //free(((unicode_object*)tos->ptr)->content);
	 //((unicode_object*)tos->ptr)->content = tmp;
	 //printf("cat:[%s]\n",tmp);
	 object *new_tos = AllocObject();
	 new_tos->flags = OFLAG_ON_STACK;
	 unicode_object* new_tos_value = AllocUnicodeObject();
	 new_tos->type = TYPE_UNICODE;
	 new_tos->ptr = new_tos_value;
	 new_tos_value->content = tmp;
	 //new_tos_value->len = strlen(tmp);
     stack_Push(new_tos);
   }
   else
   {
   long a = 0;
   long b = 0;
   object *new_tos = AllocObject();
   new_tos->flags = OFLAG_ON_STACK;
   new_tos->type = TYPE_INT;
   new_tos->ptr = 0;
   if(tos->type == TYPE_INT)
   {
    a = tos->ptr;
   }
   /*
   else if(tos->type == TYPE_UNICODE)
   {
   //printf("tos:u:%s\n",((unicode_object*)tos->ptr)->content);
   a = ((object*)tos->value_ptr)->ptr;
   }
   */
   if(tos1->type == TYPE_INT)
   {
    b = tos1->ptr;
   }
   /*
   else if(tos1->type == TYPE_UNICODE)
   {
   b = ((object*)tos1->value_ptr)->ptr;
   }
   */
    if(tos->type == TYPE_INT)
    {
    //tos->ptr = a + b;
	}
	new_tos->ptr = a + b;
	/*
    if(tos->type == TYPE_UNICODE)
    {
    ((object*)tos1->value_ptr)->ptr = a + b;
	}
	*/
	printf("%d + %d = %d (%d)\n",a,b,a+b,new_tos->ptr);
    stack_Push(new_tos);
	}
   
   break;
   case 0x18://BINARY_SUBSTRACT
   printf("");
   tos = stack_Pop();
   tos1 = stack_Pop();
   //object* org_tos = tos;
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;

	if(tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE)
    {
     char *tos_tmp = ((unicode_object*)tos->ptr)->content;
	 char *tos1_tmp = ((unicode_object*)tos1->ptr)->content;
     char *tmp = str_Cat(tos1_tmp,tos_tmp);
	 object *new_tos = AllocObject();
	 new_tos->flags = OFLAG_ON_STACK;
	 unicode_object* new_tos_value = AllocUnicodeObject();
	 new_tos->type = TYPE_UNICODE;
	 new_tos->ptr = new_tos_value;
	 new_tos_value->content = tmp;
	 //new_tos_value->len = strlen(tmp);
     stack_Push(new_tos);
   }
   else
   {
   long a = 0;
   long b = 0;
   object *new_tos = AllocObject();
   new_tos->flags = OFLAG_ON_STACK;
   new_tos->type = TYPE_INT;
   new_tos->ptr = 0;
   if(tos->type == TYPE_INT)
   {
    a = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    b = tos1->ptr;
   }
    if(tos->type == TYPE_INT)
    {
	}
	new_tos->ptr = b - a;
	printf("%d - %d = %d (%d)\n",b,a,b-a,new_tos->ptr);
    stack_Push(new_tos);
	}
   
   break;
  case 0x83://CALL_FUNCTION
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
	 if(function_name->value_ptr !=NULL && ((object*)function_name->value_ptr)->type == TYPE_CODE)
	 {
	   printf("executing function: %s\n",((unicode_object*)function_name->ptr)->content);
	   ExecuteObject((object*)function_name->value_ptr,obj,global);
	 }else
	 
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


}
