#include "vm.h"


object *ExecuteObject(object *obj,object* caller,object *global,stack *locals,int argc)
{
 if(obj->type == TYPE_CODE)
 {
  object *tos;
  //printf("showing bytecodes\n");
  code_object *co = (code_object*)obj->ptr;
  //printf("co_ptr=%x\n",(unsigned long)co);
  //printf("stack size:%d\n",co->stacksize);
  stack *_stack = stack_Init(co->stacksize);
  string_object *bytecodes = (string_object*)co->code->ptr;
  char *string = bytecodes->content;
  long n = bytecodes->len;

  if(argc>0 && locals != NULL)
  {
  for(int i=0;i<argc;i++)
  {
    //printf("adding local %d\n",i);
    tos = stack_Pop(locals);
	tuple_object *co_varnames = (tuple_object*)co->varnames->ptr;
	char varname_i = i;
	char *varname = (char*)co_varnames->items[varname_i]->ptr;
    if(tos->type == TYPE_UNICODE)
	 printf("fast storing %s in %s\n",(char*)tos->ptr,varname);
    if(tos->type == TYPE_INT)
	 printf("fast storing %d in %s\n",tos->ptr,varname);
    if(tos->type == TYPE_CODE)
	 printf("fast storing function %s in %s\n",((code_object*)tos->ptr)->name,varname);
	co_varnames->items[varname_i]->value_ptr = tos;
   }
   }
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
  short const_i;
  short name_i;
  short varname_i;
  tuple_object* co_names;
  tuple_object* co_varnames;
  char *name;
  char *varname;
  char *const_content;
  object *tos1;
  object *tos2;
  printf("[%d,%xh] opcode: [ %s ]\n",i,opcodes[index].opcode,opcodes[index].name);
  switch(opcodes[index].opcode)
  {
   case 0x74://LOAD_GLOBAL
    printf("");
    //printf("push global name\n");
    //code_object *co_global = (code_object*)global->ptr;
    //code_object *co_global = (code_object*)caller->ptr;
	//co_names = (tuple_object*)co_global->names->ptr;
	co_names = (tuple_object*)co->names->ptr;
	name_i = num_short(string[i+1],string[i+2]);
	name = (char*)co_names->items[name_i]->ptr;
    //if(co_names->items[name_i]->type== TYPE_UNICODE)
	// printf("pushing global (%d)%s = %s\n",name_i,name->content,AsUnicodeObject(co_names->items[name_i])->content);
    //if(co_names->items[name_i]->type== TYPE_INT)
	// printf("pushing global (%d)%s = %d\n",name_i,name->content,co_names->items[name_i]->ptr);
	//printf("loading global\n");
	//DumpObject(co_names->items[name_i],0);
	//printf("pushed global\n");
	stack_Push(co_names->items[name_i],_stack);
    break;
   case 0x65://LOAD_NAME
    printf("");
    //printf("push name\n");
	co_names = (tuple_object*)co->names->ptr;
	name_i = num_short(string[i+1],string[i+2]);
	name = (char*)co_names->items[name_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    //if(co_names->items[name_i]->type== TYPE_UNICODE)
	// printf("pushing local (%d)%s = %s\n",name_i,name->content,AsUnicodeObject(co_names->items[name_i])->content);
    //if(co_names->items[name_i]->type== TYPE_INT)
	// printf("pushing local (%d)%s = %d\n",name_i,name->content,co_names->items[name_i]->ptr);
	stack_Push(co_names->items[name_i],_stack);
    break;
   case 0x64://LOAD_CONST
    printf("");
    //printf("push const\n");
	co_consts = (tuple_object*)co->consts->ptr;
	const_i = num_short(string[i+1],string[i+2]);
	const_content = (char*)co_consts->items[const_i]->ptr;
    //if(((object*)co_consts->items[const_i]->value_ptr)->type== TYPE_UNICODE)
	//printf("pushing local const (%d)%s = %s\n",const_i,const_content->content,AsUnicodeObject((object*)co_consts->items[const_i]->value_ptr)->content);
    //if(co_consts->items[const_i]->type== TYPE_INT)
	// printf("pushing local const (%d)%s = %d\n",const_i,const_content->content,co_consts->items[const_i]->ptr);
    stack_Push(co_consts->items[const_i],_stack);
	break;
   case 0x7c://LOAD_FAST
    printf("");
    //printf("push name\n");
	co_varnames = (tuple_object*)co->varnames->ptr;
	varname_i = num_short(string[i+1],string[i+2]);
	varname = (char*)co_varnames->items[varname_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    //if(co_varnames->items[varname_i]->type== TYPE_UNICODE)
	// printf("pushing local fast (%d,s)%s = %s\n",varname_i,varname->content,AsUnicodeObject((object*)co_varnames->items[varname_i]->value_ptr)->content);
    //if(co_varnames->items[varname_i]->type== TYPE_INT)
	// printf("pushing local fast (%d,i)%s = %d\n",varname_i,varname->content,co_varnames->items[varname_i]->ptr);
	stack_Push(co_varnames->items[varname_i]->value_ptr,_stack);
    break;

  case 0x84://MAKE_FUNCTION
		 //not used right now
		break;
  case 0x01://POP_TOP
    //printf("Removing TOS\n");
   stack_Pop(_stack);
   break;
   case 0x5a://STORE_NAME
   printf("");
   //printf("storing in name\n");
    tos = stack_Pop(_stack);
	co_names = (tuple_object*)co->names->ptr;
	name_i = num_short(string[i+1],string[i+2]);
	name = (char*)co_names->items[name_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    if(tos->type == TYPE_UNICODE)
	 printf("storing %s in %s\n",(char*)tos->ptr,name);
    if(tos->type == TYPE_INT)
	 printf("storing %d in %s\n",tos->ptr,name);
    if(tos->type == TYPE_CODE)
	{
	//co_names = (tuple_object*)AsCodeObject(global)->names->ptr;
	//name_i = num_short(string[i+1],string[i+2]);
	//name = (unicode_object*)co_names->items[name_i]->ptr;
	 printf("storing function %s in %s\n",((code_object*)tos->ptr)->name,name);
	//co_names->items[name_i]->value_ptr = tos;
 
	 }
	 //else 
	co_names->items[name_i]->value_ptr = tos;
   break;
   case 0x7d://STORE_FAST
   printf("");
   //printf("storing in fast\n");
    tos = stack_Pop(_stack);
	//printf("tos type:%c\n",tos->type);
	co_varnames = (tuple_object*)co->varnames->ptr;
	varname_i = num_short(string[i+1],string[i+2]);
	varname = (char*)co_varnames->items[varname_i]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    if(tos->type == TYPE_UNICODE)
	 printf("fast storing %s in %s\n",(char*)tos->ptr,varname);
    if(tos->type == TYPE_INT)
	 printf("fast storing %d in %s\n",tos->ptr,varname);
    if(tos->type == TYPE_CODE)
	 printf("fast storing function %s in %s\n",((code_object*)tos->ptr)->name,varname);
	co_varnames->items[varname_i]->value_ptr = tos;
   break;
   case 0x17: //BINARY_ADD
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
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
     char *tos_tmp = (char*)tos->ptr;
	 char *tos1_tmp = (char*)tos1->ptr;
     char *tmp = str_Cat(tos1_tmp,tos_tmp);
     //free(((unicode_object*)tos->ptr)->content);
	 //((unicode_object*)tos->ptr)->content = tmp;
	 //printf("cat:[%s]\n",tmp);
	 object *new_tos = AllocObject();
	 new_tos->flags = OFLAG_ON_STACK;
	 new_tos->type = TYPE_UNICODE;
	 new_tos->ptr = tmp;
	 new_tos->value_ptr = NULL;
	 //new_tos_value->len = strlen(tmp);
     stack_Push(new_tos,_stack);
   }
   else
   {
   long a = 0;
   long b = 0;
   object *new_tos = AllocObject();
   new_tos->flags = OFLAG_ON_STACK;
   new_tos->type = TYPE_INT;
   new_tos->ptr = 0;
   new_tos->value_ptr = NULL;
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
    stack_Push(new_tos,_stack);
	}
   break;

   case 0x18://BINARY_SUBSTRACT
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   //object* org_tos = tos;
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;

   long a = 0;
   long b = 0;
   object *new_tos = AllocObject();
   new_tos->flags = OFLAG_ON_STACK;
   new_tos->type = TYPE_INT;
   new_tos->ptr = 0;
   new_tos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     a = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    b = tos1->ptr;
   }
   new_tos->ptr = b - a;
   printf("%d - %d = %d (%d)\n",b,a,b-a,new_tos->ptr);
   stack_Push(new_tos,_stack);
   break;

   case 0x1b://BINARY_TRUE_DIVIDE
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   //object* org_tos = tos;
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;

   long da = 0;
   long db = 0;
   object *new_dtos = AllocObject();
   new_dtos->flags = OFLAG_ON_STACK;
   new_dtos->type = TYPE_INT;
   new_dtos->ptr = 0;
   new_dtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     da = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    db = tos1->ptr;
   }
   new_dtos->ptr =(long) db / da;//TODO catch divide by zero
   printf("%d / %d = %d (%d)\n",db,da,db/da,new_dtos->ptr);
   stack_Push(new_dtos,_stack);
   break;

   case 0x16://BINARY_MODULO
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   //object* org_tos = tos;
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;

   long mda = 0;
   long mdb = 0;
   object *new_mdtos = AllocObject();
   new_mdtos->flags = OFLAG_ON_STACK;
   new_mdtos->type = TYPE_INT;
   new_mdtos->ptr = 0;
   new_mdtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     mda = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    mdb = tos1->ptr;
   }
   new_mdtos->ptr =(long) mdb % mda;//TODO catch divide by zero
   printf("%d %% %d = %d\n",mdb,mda,new_mdtos->ptr);
   stack_Push(new_mdtos,_stack);
   break;

   case 0x13://BINARY_POWER
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   //object* org_tos = tos;
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long pa = 0;
   long pb = 0;
   object *new_ptos = AllocObject();
   new_ptos->flags = OFLAG_ON_STACK;
   new_ptos->type = TYPE_INT;
   new_ptos->ptr = 0;
   new_ptos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     pa = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    pb = tos1->ptr;
   }
   //printf("getting pow\n");
   new_ptos->ptr = long_pow(pb , pa);//TODO catch divide by zero
   printf("%d ** %d = %d\n",pb,pa,new_ptos->ptr);
   stack_Push(new_ptos,_stack);
   break;

   
   case 0x14://BINARY_MULTIPLY
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   //object* org_tos = tos;
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;

   long ma = 0;
   long mb = 0;
   object *new_mtos = AllocObject();
   new_mtos->flags = OFLAG_ON_STACK;
   new_mtos->type = TYPE_INT;
   new_mtos->ptr = 0;
   new_mtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     ma = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    mb = tos1->ptr;
   }
   new_mtos->ptr = ma * mb;
   printf("%d * %d = %d (%d)\n",ma,mb,ma*mb,new_mtos->ptr);
   stack_Push(new_mtos,_stack);
   break;

   case 0x6b: //COMPARE_OP
   printf("");
    tos = stack_Pop(_stack);
    tos1 = stack_Pop(_stack);

	if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
	short op = num_short(string[i+1],string[i+2]);
	switch(op)
	{
	  case 0: // <
			printf("");
			long ca = 0;
			long cb = 0;
			object *new_ctos = AllocEmptyObject();
			new_ctos->flags = OFLAG_ON_STACK;
			if(tos->type == TYPE_INT)
			{
				ca = tos->ptr;
			}
			if(tos1->type == TYPE_INT)
			{
				cb = tos1->ptr;
			}
			new_ctos->type = cb < ca  ? TYPE_TRUE :  TYPE_FALSE;
			printf("%d < %d == %c\n",cb,ca,new_ctos->type);
			stack_Push(new_ctos,_stack);
			break;
		case 2: // ==
				if(tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE)
				{
				char *tos_tmp = (char*)tos->ptr;
				char *tos1_tmp = (char*)tos1->ptr;
				object *new_ctos = AllocEmptyObject();
				new_ctos->flags = OFLAG_ON_STACK;
				new_ctos->type = !strcmp(tos_tmp,tos1_tmp)  ? TYPE_TRUE :  TYPE_FALSE;
				printf("%s == %s == %c\n",tos_tmp,tos1_tmp,new_ctos->type);
				stack_Push(new_ctos,_stack);
				}
				else
			{
			long ca = 0;
			long cb = 0;
			object *new_ctos = AllocEmptyObject();
			new_ctos->flags = OFLAG_ON_STACK;
			if(tos->type == TYPE_INT)
			{
				ca = tos->ptr;
			}
			if(tos1->type == TYPE_INT)
			{
				cb = tos1->ptr;
			}
			new_ctos->type = cb == ca  ? TYPE_TRUE :  TYPE_FALSE;
			printf("%d == %d == %c\n",cb,ca,new_ctos->type);
			stack_Push(new_ctos,_stack);
			}
			break;

		default:
			printf("compare op:%d not supported\n",op);
	}
    //printf("compare op:%d\n",op);
	break;
   
   case 0x72: //POP_JUMP_IF_FALSE
   printf("");
    tos = stack_Pop(_stack);
    short jmp = num_short(string[i+1],string[i+2]);
	if(tos->type == TYPE_FALSE)
	{
	printf("jump to:%d\n",jmp);
	 i = jmp -3;
	 }
	break;

	case 0x73: //POP_JUMP_IF_TRUE
   printf("");
    tos = stack_Pop(_stack);
    short tjmp = num_short(string[i+1],string[i+2]);
	if(tos->type == TYPE_TRUE)
	{
	printf("true jmp arg:%d\n",tjmp);
	 i = tjmp -3;
	 }
	break;

	case 0x6e: //JUMP_FORWARD
	printf("");
    short jmpf = num_short(string[i+1],string[i+2]);
	printf("jump forward to:%d\n",i+jmpf+3);
	 i = i + jmpf;
	break;
   
   case 0x83://CALL_FUNCTION
    printf("");
	char argc = string[i+1];
    //printf("calling function with %d arguments\n",argc);
	//printf("opcode: [%s],(argc:%d)\n",opcodes[index].name,argc);
    stack *call = NULL;
	if(argc>0)
		call = stack_Init((long)argc);
 	tos = NULL;
	for(int i = 0;i<argc;i++)
	{
		stack_Push(stack_Pop(_stack),call);
	}
	//if(argc>0)
	//  printf("created callstack\n");
	object *function_name = stack_Pop(_stack);
	if(function_name != NULL&& function_name->type == TYPE_CODE)
		{
		printf("executing direct local function object: %s\n",((code_object*)function_name->ptr)->name);
		for(int i = 0;i<argc;i++)
	    { 
			stack_Push(stack_Pop(call),_stack);
	    }
	    object *ret = ExecuteObject((object*)function_name,obj,global,call,argc);
		if(ret != NULL)
		 stack_Push(ret,_stack);
		if(argc>0)
			stack_Close(call,0);
		 break;
        }
    
	if(function_name->value_ptr !=NULL && ((object*)function_name->value_ptr)->type == TYPE_CODE)
	{
		printf("executing local function object: %s\n",(char*)function_name->ptr);
		for(int i = 0;i<argc;i++)
	    { 
			stack_Push(stack_Pop(call),_stack);
	    }
	    object *ret = ExecuteObject((object*)function_name->value_ptr,obj,global,NULL,0);
		if(ret != NULL)
		 stack_Push(ret,_stack);
		break;
	}
	
	if(function_name != NULL&& function_name->type == TYPE_UNICODE)
		{
  	    if(!strcmp((char*)function_name->ptr,"print"))
	    {
			for(int i = 0;i<argc;i++)
			{
				tos = stack_Pop(call);
				//if(tos != NULL && tos->type == TYPE_UNICODE)
				//printf("arg[%d]:%s\n",i,((unicode_object*)tos->ptr)->content );
				//else 
				if(i)
				 printf(" ");
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
									printf("%s",(char*)((object*)tos->value_ptr)->ptr );
								else
									if(((object*)tos->value_ptr)->type == TYPE_INT)
										printf("%d",((object*)tos->value_ptr)->ptr );
							}
						else 
							printf("%s",(char*)tos->ptr);
							break;
					}
				//else 
				//printf("tos = NULL\n");
			}
		object *tmp = AllocEmptyObject();
		tmp->type = TYPE_NONE;
		tmp->flags = OFLAG_ON_STACK;
		stack_Push(tmp,_stack);	
		if(argc>0)
			stack_Close(call,0);
		break;
		}
		else
		{
			    printf("searching %s\n",(char*)function_name->ptr);
		object * caller_func = FindUnicodeTupleItem( AsCodeObject(global)->names,(char*)function_name->ptr);
		if(caller_func == NULL)
			printf("global function not found\n");		  
		if(caller_func!= NULL && ((object*)caller_func->value_ptr)->type == TYPE_CODE) 
		{
  		    printf("executing global function object: %s\n",(char*)function_name->ptr);
			object *ret = ExecuteObject((object*)caller_func->value_ptr,obj,global,call,argc);
			if(ret != NULL)
			stack_Push(ret,_stack);
			if(argc>0)
				stack_Close(call,0);
			break;
			}
		}

	   //printf("%s",((unicode_object*)tos->ptr)->content); 
	   
	   //else 
	    printf("function: [%s] not found\n",(char*)function_name->ptr);
	}
 
	 break;
  case 0x53: //RETURN_VALUE
		printf("");
        object *ret = stack_Pop(_stack);
		stack_Close(_stack,0);
		//printf("stack closed\n");
		return(ret); 
	
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
 object *ret = stack_Pop(_stack);
 stack_Close(_stack,0);
 //printf("stack closed\n");
 return(ret); 
 }


}
