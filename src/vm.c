#include "vm.h"

function_definition **functions;
unsigned long functions_num = 0;
unsigned long functions_total = 0;

function_definition *CreateCFunction(object *(*func)(stack *stack),char *name)
{
function_definition *fd = mem_malloc(sizeof(function_definition),"CreateCFunction() fd");
fd->type = FUNC_C;
fd->func.func = func;
fd->name = name;
return(fd);
}

function_definition *CreateCObjFunction(object *(*func)(object *obj),char *name)
{
function_definition *fd = mem_malloc(sizeof(function_definition),"CreateCObjFunction() fd");
fd->type = FUNC_C_OBJ;
fd->func.func_obj = (*func);
fd->name = name;
return(fd);
}

function_definition *CreatePythonFunction(object *code,char *name)
{
function_definition *fd = mem_malloc(sizeof(function_definition),"CreatePythonFunction() fd");
fd->type = FUNC_PYTHON;
fd->func.code = code;
fd->name = name;
return(fd);
}

int AddFunctionDefinition(function_definition *fd)
{
functions = (function_definition**)mem_realloc(functions,(functions_total + 1) *sizeof(function_definition*));
functions[functions_total] = fd;
functions_total++;
functions_num++;
return(functions_total -1);
}

void RemoveFunction(char *name)
{
for(int i=0;i<functions_total;i++)
{
 if(functions[i]!=NULL)
  {
   if(!strcmp(functions[i]->name,name))
	{
	 assert(mem_free(functions[i]));
	 functions[i] = NULL;
	 functions_num--;
	}
   }
}
}

void RemoveFunctionDefinition(function_definition *fd)
{
for(int i=0;i<functions_total;i++)
{
 if(functions[i]!=NULL)
  {
   if(functions[i] == fd)
	{
	 assert(mem_free(functions[i]));
	 functions[i] = NULL;
	 functions_num--;
	}
  }
}
}

object *ExecuteCFunction(char *name,stack *stack)
{
function_definition *fd = FindFunction(name);
if(fd != NULL)
 {
  //printf("fd found:%s\n",fd->name);
  return((*fd->func.func)(stack));
 
 } 
}

object *ExecuteCObjFunction(char *name,object *obj)
{
function_definition *fd = FindFunction(name);
if(fd != NULL)
 {
  return((*fd->func.func_obj)(obj));
 }
}

function_definition *FindFunction(char *name)
{
for(int i=0;i<functions_total;i++)
{
 if(functions[i]!=NULL)
  {
   if(!strcmp(functions[i]->name,name))
    return(functions[i]);
  }
}
return(NULL);
}

void vm_Init()
{
functions = (function_definition**)mem_malloc(0,"vm_Init() functions");
functions_num = 0;
functions_total = 0;
}

void vm_Close()
{
if(functions_total >0)
{
for(int i=0;i<functions_total;i++)
 if(functions[i] != NULL)
 assert(mem_free(functions[i]));
assert(mem_free(functions)); 
}
}

object *BuildList(stack *stack)
{
int num =stack->top;
object *r = AllocObject();
r->type = TYPE_TUPLE;
r->flags = OFLAG_ON_STACK;
r->ptr = AllocTupleObject();
((tuple_object*)r->ptr)->num = num;
((tuple_object*)r->ptr)->items = (object**)mem_malloc(num*sizeof(object*),"BuildList() items");
for(int i = 0;i<num;i++)
{
((tuple_object*)r->ptr)->items[i] = stack_Pop(stack);
}
//DumpObject(r,0);
return(r);
}



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
	char *varname = (char*)co_varnames->items[i]->ptr;
    if(tos->type == TYPE_UNICODE)
	 printf("fast storing %s in %s\n",(char*)tos->ptr,varname);
    if(tos->type == TYPE_INT)
	 printf("fast storing %d in %s\n",tos->ptr,varname);
    if(tos->type == TYPE_CODE)
	 printf("fast storing function %s in %s\n",((code_object*)tos->ptr)->name,varname);
	co_varnames->items[i]->value_ptr = tos;
   }
   }

  int i=0;
  int has_extended_arg = 0;
  short extended_arg = 0;
  while(i<n)
  {
  unsigned char op = (unsigned char)string[i++]; //get op and increment code pointer
  long arg = 0; //holds the argument if present
  int op_thru = 0;//used to skip unneccessary switches

  //FOR DEBUGGING
  int old_i = i;
  int index = GetOpcodeIndex(op);
  if(index >=0)
  {
	if(opcodes[index].argcount!=0)
		printf("[%d,%xh] opcode: [ %s ] (%d)\n",i,opcodes[index].opcode,opcodes[index].name,num_short(string[i],string[i+1]));
	else
		printf("[%d,%xh] opcode: [ %s ]\n",i,opcodes[index].opcode,opcodes[index].name);
  }else
  {
	printf("unknown opcode:%x at %d\n",(char)string[i-1],i-1);
  }
  // -- FOR DEBUGGING

  tuple_object* co_consts;
  tuple_object* co_names;
  tuple_object* co_varnames;
  char *name;
  char *varname;
  char *const_content;
  object *tos1;
  object *tos2;
   
   
   //prepare opcode argument,increment codepointer  and intepret small opcodes not using an arg etc
   switch(opcodes[index].opcode)
   {
	case OPCODE_NOP:
		break;
    case OPCODE_RETURN_VALUE:
		{
        object *ret = stack_Pop(_stack);
		stack_Close(_stack,0);
		return(ret); 
		}
	case OPCODE_STOP_CODE:
		i = n;
		op_thru = 1;
		break;
	case OPCODE_BREAK_LOOP:
		{
		block_object *bbo = (block_object*)stack_Top(blocks)->ptr;
		i = bbo->start + bbo->len - 1;
		//printf("break to: %d ,start: %d ,len: %d\n",i,bbo->start,bbo->len);
		op_thru = 1;
		}
		//break;
	case OPCODE_EXTENDED_ARG:
		extended_arg = num_short(string[i+1],string[i+2]);
		i += 2;
		op_thru = 1;
		has_extended_arg = 1;
		break;

	//case OPCODE_LOAD_DEREF:
	//case OPCODE_STORE_DEREF:
	//case OPCODE_STORE_MAP:
	//case OPCODE_STORE_SUBSCR:
	//case OPCODE_DELETE_SUBSCR:
	//case OPCODE_STORE_ATTR:
	//case OPCODE_DELETE_ATTR:
	//case OPCODE_LOAD_ATTR:
	case OPCODE_LOAD_GLOBAL:
	case OPCODE_STORE_GLOBAL:
	case OPCODE_DELETE_GLOBAL:
	case OPCODE_LOAD_CONST:
	case OPCODE_LOAD_NAME:
	case OPCODE_STORE_NAME:
	case OPCODE_DELETE_NAME:
	case OPCODE_LOAD_FAST:
	case OPCODE_STORE_FAST:
	case OPCODE_DELETE_FAST:
	case OPCODE_BUILD_TUPLE:
	case OPCODE_BUILD_LIST:
	case OPCODE_BUILD_MAP:
	case OPCODE_FOR_ITER:
	case OPCODE_UNPACK_SEQUENCE:
	case OPCODE_SETUP_LOOP:
	case OPCODE_CALL_FUNCTION:
	case OPCODE_CALL_FUNCTION_VAR:
	case OPCODE_CALL_FUNCTION_KW:
	case OPCODE_CALL_FUNCTION_VAR_KW:
	case OPCODE_JUMP_FORWARD:
	case OPCODE_JUMP_IF_FALSE:
	case OPCODE_JUMP_IF_TRUE:
	case OPCODE_JUMP_ABSOLUTE:
	case OPCODE_POP_JUMP_IF_FALSE:
	case OPCODE_POP_JUMP_IF_TRUE:
	case OPCODE_MAKE_FUNCTION:
	case OPCODE_BUILD_SLICE:
	case OPCODE_MAKE_CLOSURE:
	case OPCODE_LOAD_CLOSURE:
	case OPCODE_STORE_LOCALS:
	case OPCODE_PRINT_EXPR:
	case OPCODE_LOAD_BUILD_CLASS:
	case OPCODE_WITH_CLEANUP:
	case OPCODE_IMPORT_STAR:
	case OPCODE_END_FINALLY:
	case OPCODE_BUILD_CLASS:
	case OPCODE_COMPARE_OP:
	case OPCODE_IMPORT_FROM:
	case OPCODE_SETUP_EXCEPT:
	case OPCODE_SETUP_FINALLY:
	case OPCODE_RAISE_VARARGS:
		if(has_extended_arg)
		{
		 arg = num_long((extended_arg>> 8) & 0xFF,extended_arg & 0xFF,string[i],string[i+1]);
		 has_extended_arg = 0;
		 extended_arg = 0;
		}
		else
		 arg = (long)num_short(string[i],string[i+1]);
		i += 2;
		break;

	case OPCODE_POP_TOP:
		stack_Pop(_stack);
		op_thru = 1;
		break;

	case OPCODE_GET_ITER:
		{
		object *iter = stack_Pop(_stack);
		block_object *abo = (block_object*)stack_Top(blocks)->ptr;
		abo->iter = iter;
		ResetIteration(iter);
		op_thru = 1;
		}
		//break;
	
	case OPCODE_POP_BLOCK:
		stack_Pop(blocks);
		op_thru = 1;
		break;

	case OPCODE_ROT_TWO:
	case OPCODE_ROT_THREE:
	case OPCODE_DUP_TOP:
	case OPCODE_DUP_TOP_TWO:
	case OPCODE_UNARY_POSITIVE:
	case OPCODE_UNARY_NEGATIVE:
	case OPCODE_UNARY_NOT:
	case OPCODE_UNARY_INVERT:
	case OPCODE_BINARY_POWER:
	case OPCODE_BINARY_MULTIPLY:
	case OPCODE_BINARY_DIVIDE:
	case OPCODE_BINARY_MODULO:
	case OPCODE_BINARY_ADD:
	case OPCODE_BINARY_SUBTRACT:
	case OPCODE_BINARY_SUBSCR:
	case OPCODE_BINARY_FLOOR_DIVIDE:
	case OPCODE_BINARY_TRUE_DIVIDE:
	case OPCODE_INPLACE_FLOOR_DIVIDE:
	case OPCODE_INPLACE_TRUE_DIVIDE:
	case OPCODE_INPLACE_ADD:
	case OPCODE_INPLACE_SUBTRACT:
	case OPCODE_INPLACE_MULTIPLY:
	case OPCODE_INPLACE_DIVIDE:
	case OPCODE_INPLACE_MODULO:
	case OPCODE_BINARY_LSHIFT:
	case OPCODE_BINARY_RSHIFT:
	case OPCODE_BINARY_AND:
	case OPCODE_BINARY_XOR:
	case OPCODE_BINARY_OR:
	case OPCODE_INPLACE_POWER:
	case OPCODE_INPLACE_LSHIFT:
	case OPCODE_INPLACE_RSHIFT:
	case OPCODE_INPLACE_AND:
	case OPCODE_INPLACE_XOR:
	case OPCODE_INPLACE_OR:
	case OPCODE_YIELD_VALUE:
		break;
	}
   
   if(op_thru)
	continue;
   
   
   
   
  switch(opcodes[index].opcode)
  {
   case OPCODE_POP_JUMP_IF_FALSE:
   {
    tos = stack_Pop(_stack);
	if(tos->type == TYPE_FALSE)
	{
	 i = arg -3;
	 }
	 }
	break;
	case OPCODE_POP_JUMP_IF_TRUE:
	{
    tos = stack_Pop(_stack);
	if(tos->type == TYPE_TRUE)
	{
	 i = arg -3;
	 }
	 }
	break;
	case OPCODE_JUMP_FORWARD:
	{
	 i = i + arg;
	 }
	break;
	case OPCODE_JUMP_ABSOLUTE:
	{
	 i = arg - 3;
	 }
	break;
	case OPCODE_JUMP_IF_FALSE:
	{
		tos = stack_Top(_stack);
		if(tos->type == TYPE_FALSE)
		{
			i = arg -3;
		}
		}
		break;
	case OPCODE_JUMP_IF_TRUE:
	{
		tos = stack_Top(_stack);
		if(tos->type == TYPE_TRUE)
		{
			i = arg -3;
		}
		}
		break;

	case OPCODE_FOR_ITER:
		{
		//		printf("for_iter delta jump\n");
		//		i = i + delta;
		//		break;
		block_object *fabo = (block_object*)stack_Top(blocks)->ptr;
		object *next = GetNextItem(fabo->iter);
		if(next != NULL)
		{
			//if(next->value_ptr != NULL)
			//	next = next->value_ptr;
			//printf("next pushed:");
			//PrintObject(next);
			stack_Push(next,_stack);
			//printf("\n");
		}
		else
			{
				//printf("for_iter delta jump\n");
				i = i + arg;
			}
		}
		//break;

	
	case OPCODE_BUILD_TUPLE:
	case OPCODE_BUILD_LIST:
		printf("");
		//stack_Push(BuildList(_stack,arg),_stack);
		stack *blcall = NULL;
		if(arg>0)
		blcall = stack_Init(arg);
		tos = NULL;
		for(int i = 0;i<arg;i++)
		{
			stack_Push(stack_Pop(_stack),blcall);
		}
		object *tmp = ExecuteCFunction("internal.BuildList",blcall);
  	    if(tmp != NULL)
	    {
				stack_Push(tmp,_stack);	
		}
		if(arg>0)
			stack_Close(blcall,0);
		break;
   case OPCODE_LOAD_GLOBAL:
    printf("");
    //printf("push global name\n");
    //code_object *co_global = (code_object*)global->ptr;
    
	co_names = (tuple_object*)co->names->ptr;
	code_object *co_global = (code_object*)global->ptr;
	//co_names = (tuple_object*)co->names->ptr;
	name = (char*)co_names->items[arg]->ptr;
    object *lgo = FindUnicodeTupleItem(co_global->varnames,name);
	//DumpObject(lgo,1);
	if(lgo == NULL)
	 if(FindFunction(name)!= NULL)
	 {
	  lgo = AllocObject();
	  lgo->type = TYPE_UNICODE;
	  lgo->flags = OFLAG_ON_STACK;
	  lgo->ptr = str_Copy(name);
	  lgo->value_ptr = NULL;
	 }
	stack_Push(lgo,_stack);
	//stack_Dump(_stack);
	//if(co_names->items[name_i]->type== TYPE_UNICODE)
	// printf("pushing global (%d)%s = %s\n",name_i,name->content,AsUnicodeObject(co_names->items[name_i])->content);
    //if(co_names->items[name_i]->type== TYPE_INT)
	// printf("pushing global (%d)%s = %d\n",name_i,name->content,co_names->items[name_i]->ptr);
	//printf("loading global\n");
	//DumpObject(co_names->items[name_i],0);
	//printf("pushed global\n");
	//stack_Push(co_varnames->items[arg],_stack);
    break;

   case OPCODE_STORE_GLOBAL:
   printf("");
    tos = stack_Pop(_stack);
	co_names = (tuple_object*)co->names->ptr;
	co_global = (code_object*)global->ptr;
	//co_names = (tuple_object*)co->names->ptr;
	name = (char*)co_names->items[arg]->ptr;
    object *sgo = FindUnicodeTupleItem(co_global->varnames,name);

    if(tos->type == TYPE_UNICODE)
	 printf("fast storing %s in %s\n",(char*)tos->ptr,(char*)lgo->ptr);
    if(tos->type == TYPE_INT)
	 printf("fast storing %d in %s\n",tos->ptr,(char*)lgo->ptr);
    if(tos->type == TYPE_CODE)
	 printf("fast storing function %s in %s\n",((code_object*)tos->ptr)->name,(char*)lgo->ptr);
	//co_varnames->items[arg]->value_ptr = tos;
	sgo->value_ptr = tos;
   break;

   case OPCODE_DELETE_GLOBAL:
   printf("");
	co_names = (tuple_object*)co->names->ptr;
    co_global = (code_object*)global->ptr;
	name = (char*)co_names->items[arg]->ptr;
	DeleteItem(co_global->varnames,arg);
   break;

   case OPCODE_LOAD_NAME:
    printf("");
    //printf("push name\n");
	co_names = (tuple_object*)co->names->ptr;
	name = (char*)co_names->items[arg]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    //if(co_names->items[name_i]->type== TYPE_UNICODE)
	// printf("pushing local (%d)%s = %s\n",name_i,name->content,AsUnicodeObject(co_names->items[name_i])->content);
    //if(co_names->items[name_i]->type== TYPE_INT)
	// printf("pushing local (%d)%s = %d\n",name_i,name->content,co_names->items[name_i]->ptr);
	stack_Push(co_names->items[arg],_stack);
    break;

	case OPCODE_LOAD_CONST:
    printf("");
    //printf("push const\n");
	co_consts = (tuple_object*)co->consts->ptr;
	const_content = (char*)co_consts->items[arg]->ptr;
    //if(((object*)co_consts->items[const_i]->value_ptr)->type== TYPE_UNICODE)
	//printf("pushing local const (%d)%s = %s\n",const_i,const_content->content,AsUnicodeObject((object*)co_consts->items[const_i]->value_ptr)->content);
    //if(co_consts->items[const_i]->type== TYPE_INT)
	// printf("pushing local const (%d)%s = %d\n",const_i,const_content->content,co_consts->items[const_i]->ptr);
    stack_Push(co_consts->items[arg],_stack);
	break;

	case OPCODE_LOAD_FAST:
    printf("");
    //printf("push name\n");
	co_varnames = (tuple_object*)co->varnames->ptr;
	varname = (char*)co_varnames->items[arg]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    //if(co_varnames->items[varname_i]->type== TYPE_UNICODE)
	// printf("pushing local fast (%d,s)%s = %s\n",varname_i,varname->content,AsUnicodeObject((object*)co_varnames->items[varname_i]->value_ptr)->content);
    //if(co_varnames->items[varname_i]->type== TYPE_INT)
	// printf("pushing local fast (%d,i)%s = %d\n",varname_i,varname->content,co_varnames->items[varname_i]->ptr);
	stack_Push(co_varnames->items[arg]->value_ptr,_stack);
    break;

    case OPCODE_MAKE_FUNCTION:
		 //not used right now
		break;


   case OPCODE_STORE_NAME:
   printf("");
   //printf("storing in name\n");
    tos = stack_Pop(_stack);
	co_names = (tuple_object*)co->names->ptr;
	name = (char*)co_names->items[arg]->ptr;
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
	co_names->items[arg]->value_ptr = tos;
   break;

   case OPCODE_STORE_FAST:
   printf("");
   //printf("storing in fast\n");
    tos = stack_Pop(_stack);
	//printf("tos type:%c\n",tos->type);
	co_varnames = (tuple_object*)co->varnames->ptr;
	varname = (char*)co_varnames->items[arg]->ptr;
    //printf("opcode: [%s],(%d) [%s]\n",opcodes[index].name,name_i,name->content);
    if(tos->type == TYPE_UNICODE)
	 printf("fast storing %s in %s\n",(char*)tos->ptr,varname);
    if(tos->type == TYPE_INT)
	 printf("fast storing %d in %s\n",tos->ptr,varname);
    if(tos->type == TYPE_CODE)
	 printf("fast storing function %s in %s\n",((code_object*)tos->ptr)->name,varname);
	 tos->flags ^= OFLAG_ON_STACK;
	co_varnames->items[arg]->value_ptr = tos;
   break;

   case OPCODE_DELETE_NAME:
	DeleteItem(co->names,arg);
   break;

   case OPCODE_DELETE_FAST:
	DeleteItem(co->varnames,arg);
   break;

    case OPCODE_DELETE_SUBSCR:
    printf(""); //Implements  del TOS1[TOS] 
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
 
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
	long dsa = 0;
   if(tos->type == TYPE_INT)
   {
     dsa = tos->ptr;
   }
   if(tos1->type == TYPE_TUPLE)
   {
    //DumpObject(tos2,0);
	//printf("ssa: %d\n",ssa);
    DeleteItem(tos1,dsa);
	DumpObject(tos1,0);
   }
	break;
	
	case OPCODE_UNPACK_SEQUENCE:
	   printf(""); 
		tos = stack_Pop(_stack);
		  if(tos->value_ptr != NULL)
			tos = (object*)tos->value_ptr;
		for(int i=0;i<arg;i++)
		 {
		  switch(tos->type)
		   {
			case TYPE_UNICODE:
				{
				object *uso = AllocObject();
				uso->flags = OFLAG_ON_STACK;
				uso->type = TYPE_UNICODE;
				uso->ptr = str_FromChar(((char*)tos->ptr)[i]);
				stack_Push(uso,_stack);
				}
		 }
		 }
		  //stack_Dump(_stack);
		break;
   
  case OPCODE_ROT_TWO:
    tos = stack_Top(_stack);
    tos1 = stack_Second(_stack);
   stack_SetTop(tos1,_stack);
   stack_SetSecond(tos,_stack);
   break;

   case OPCODE_ROT_THREE:
    tos = stack_Top(_stack);
    tos1 = stack_Second(_stack);
    tos2 = stack_Third(_stack);
   stack_SetTop(tos1,_stack);
   stack_SetSecond(tos2,_stack);
   stack_SetThird(tos,_stack);
   break;

   case OPCODE_DUP_TOP:
   tos = stack_Top(_stack);
   stack_Push(tos,_stack);
   break;

   case OPCODE_DUP_TOP_TWO:
   tos = stack_Top(_stack);
   tos1 = stack_Second(_stack);
   stack_Adjust(2,_stack);
   stack_SetTop(tos,_stack);
   stack_SetSecond(tos1,_stack);
   break;
	
	case OPCODE_UNARY_POSITIVE:
    tos = stack_Pop(_stack);
    if(tos->type == TYPE_INT)
	{
	object *up = AllocObject();
	up->type = TYPE_INT;
    up->flags = OFLAG_ON_STACK;
	up->value_ptr = NULL;
	up->ptr = +(long)tos->ptr;
     stack_Push(up,_stack);
	}
	break;

	case OPCODE_UNARY_NEGATIVE:
    tos = stack_Pop(_stack);
    if(tos->type == TYPE_INT)
	{
	object *up = AllocObject();
    up->flags = OFLAG_ON_STACK;
	up->type = TYPE_INT;
	up->value_ptr = NULL;
	up->ptr = -(long)tos->ptr;
    stack_Push(up,_stack);
	}
	break;

	case OPCODE_UNARY_NOT:
    tos = stack_Pop(_stack);
    if(tos->type == TYPE_INT)
	{
	object *up = AllocEmptyObject();
    up->flags = OFLAG_ON_STACK;
	up->type = tos->ptr > 0  ? TYPE_TRUE :  TYPE_FALSE;
    stack_Push(up,_stack);
	}
	else
    if(tos->type == TYPE_TRUE || tos->type == TYPE_FALSE)
	{
	object *up = AllocEmptyObject();
    up->flags = OFLAG_ON_STACK;
	up->type = tos->type == TYPE_TRUE  ? TYPE_TRUE :  TYPE_FALSE;
    stack_Push(up,_stack);
	}
	else
    if(tos->type == TYPE_NONE || tos->type == TYPE_NULL)
	{
	object *up = AllocEmptyObject();
    up->flags = OFLAG_ON_STACK;
	up->type = TYPE_FALSE;
    stack_Push(up,_stack);
	}
	break;

	case OPCODE_UNARY_INVERT:
    tos = stack_Pop(_stack);
    if(tos->type == TYPE_INT)
	{
	object *up = AllocObject();
    up->flags = OFLAG_ON_STACK;
	up->type = TYPE_INT;
	up->value_ptr = NULL;
	up->ptr = -(((long)tos->ptr)+1);
     stack_Push(up,_stack);
	}
	break;
	
	
   case OPCODE_BINARY_ADD:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   //printf("tos1.type:%c (%s)(%d)\n",tos1->type,((unicode_object*)tos1->ptr)->content,((object*)tos1->value_ptr)->ptr );
   object* org_tos = tos;
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   printf("tos.type:%c (%d)\n",(long)tos->type,tos->ptr);
   printf("tos1.type:%c (%d)\n",(long)tos->type,tos->ptr);


	if(tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE)
    {
     char *tos_tmp = (char*)tos->ptr;
	 char *tos1_tmp = (char*)tos1->ptr;
     char *tmp = str_Cat(tos1_tmp,tos_tmp);
     //free(((unicode_object*)tos->ptr)->content);
	 //((unicode_object*)tos->ptr)->content = tmp;
	 printf("cat:[%s]\n",tmp);
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
   if(tos1->type == TYPE_INT)
   {
    b = tos1->ptr;
   }
	new_tos->ptr = a + b;
	printf("%d + %d = %d (%d)\n",a,b,a+b,new_tos->ptr);
    stack_Push(new_tos,_stack);
	}
   break;

   case OPCODE_BINARY_SUBTRACT:
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

   case OPCODE_BINARY_TRUE_DIVIDE:
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

   case OPCODE_BINARY_FLOOR_DIVIDE:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   //object* org_tos = tos;
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;

   long fda = 0;
   long fdb = 0;
   object *new_fdtos = AllocObject();
   new_fdtos->flags = OFLAG_ON_STACK;
   new_fdtos->type = TYPE_INT;
   new_fdtos->ptr = 0;
   new_fdtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     fda = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    fdb = tos1->ptr;
   }
   new_fdtos->ptr =(long) fdb / fda;//TODO catch divide by zero
   printf("%d / %d = %d (%d)\n",fdb,fda,fdb/fda,new_fdtos->ptr);
   stack_Push(new_fdtos,_stack);
   break;

   case OPCODE_BINARY_MODULO:
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

   case OPCODE_BINARY_POWER:
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

   
   case OPCODE_BINARY_MULTIPLY:
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

   case OPCODE_STORE_SUBSCR:
    printf(""); //Implements TOS1[TOS] = TOS2.
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   tos2 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   if(tos2->value_ptr != NULL)
    tos2 = (object*)tos2->value_ptr;
	long ssa = 0;
   if(tos->type == TYPE_INT)
   {
     ssa = tos->ptr;
   }
   if(tos1->type == TYPE_TUPLE)
   {
    //DumpObject(tos2,0);
	//printf("ssa: %d\n",ssa);
    SetItem(tos1,ssa,tos2);
	//DumpObject(tos1,0);
   }
	break;
   
   case OPCODE_BINARY_SUBSCR:
   printf("");// TOS = TOS1[TOS].
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   //tos2 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   //if(tos2->value_ptr != NULL)
   // tos2 = (object*)tos2->value_ptr;

   long bsa = 0;
   if(tos->type == TYPE_INT)
   {
     bsa = tos->ptr;
   }
   if(tos1->type == TYPE_TUPLE)
   {
    //DumpObject(tos1,0);
    //printf("bsa:%d\n",bsa);
   object *bst = GetItem(tos1,bsa);
   //DumpObject(bst,0);
   stack_Push(bst,_stack);
   }
   break;
   
   case OPCODE_INPLACE_RSHIFT:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long ira = 0;
   long irb = 0;
   object *new_irtos = AllocObject();
   new_irtos->flags = OFLAG_ON_STACK;
   new_irtos->type = TYPE_INT;
   new_irtos->ptr = 0;
   new_irtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     ira = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    irb = tos1->ptr;
   }
   new_irtos->ptr = irb << ira;
   printf("%d << %d = %d\n",irb,ira,new_irtos->ptr);
   stack_Push(new_irtos,_stack);
   break;

   case OPCODE_INPLACE_AND:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long iaa = 0;
   long iab = 0;
   object *new_iatos = AllocObject();
   new_iatos->flags = OFLAG_ON_STACK;
   new_iatos->type = TYPE_INT;
   new_iatos->ptr = 0;
   new_iatos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     iaa = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    iab = tos1->ptr;
   }
   new_iatos->ptr = iab & iaa;
   printf("%d & %d = %d\n",iab,iaa,new_iatos->ptr);
   stack_Push(new_iatos,_stack);
   break;

   case OPCODE_INPLACE_XOR:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long ixa = 0;
   long ixb = 0;
   object *new_ixtos = AllocObject();
   new_ixtos->flags = OFLAG_ON_STACK;
   new_ixtos->type = TYPE_INT;
   new_ixtos->ptr = 0;
   new_ixtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     ixa = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    ixb = tos1->ptr;
   }
   new_ixtos->ptr = ixb ^ ixa;
   printf("%d ^ %d = %d\n",ixb,ixa,new_ixtos->ptr);
   stack_Push(new_ixtos,_stack);
   break;

   case OPCODE_INPLACE_OR:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long ioa = 0;
   long iob = 0;
   object *new_iotos = AllocObject();
   new_iotos->flags = OFLAG_ON_STACK;
   new_iotos->type = TYPE_INT;
   new_iotos->ptr = 0;
   new_iotos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     ioa = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    iob = tos1->ptr;
   }
   new_iotos->ptr = iob | ioa;
   printf("%d | %d = %d\n",iob,ioa,new_iotos->ptr);
   stack_Push(new_iotos,_stack);
   break;

   case OPCODE_INPLACE_ADD:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long iada = 0;
   long iadb = 0;
   object *new_iadtos = AllocObject();
   new_iadtos->flags = OFLAG_ON_STACK;
   new_iadtos->type = TYPE_INT;
   new_iadtos->ptr = 0;
   new_iadtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     iada = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    iadb = tos1->ptr;
   }
   new_iadtos->ptr = iadb + iada;
   printf("%d + %d = %d\n",iadb,iada,new_iadtos->ptr);
   stack_Push(new_iadtos,_stack);
   break;

   case OPCODE_INPLACE_MULTIPLY:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long ima = 0;
   long imb = 0;
   object *new_imtos = AllocObject();
   new_imtos->flags = OFLAG_ON_STACK;
   new_imtos->type = TYPE_INT;
   new_imtos->ptr = 0;
   new_imtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     ima = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    imb = tos1->ptr;
   }
   new_imtos->ptr = imb * ima;
   printf("%d * %d = %d\n",imb,ima,new_imtos->ptr);
   stack_Push(new_imtos,_stack);
   break;

   case OPCODE_INPLACE_SUBTRACT:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long isa = 0;
   long isb = 0;
   object *new_istos = AllocObject();
   new_istos->flags = OFLAG_ON_STACK;
   new_istos->type = TYPE_INT;
   new_istos->ptr = 0;
   new_istos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     isa = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    isb = tos1->ptr;
   }
   new_istos->ptr = isb - isa;
   printf("%d - %d = %d\n",isb,isa,new_istos->ptr);
   stack_Push(new_istos,_stack);
   break;

   case OPCODE_INPLACE_MODULO:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long imoa = 0;
   long imob = 0;
   object *new_imotos = AllocObject();
   new_imotos->flags = OFLAG_ON_STACK;
   new_imotos->type = TYPE_INT;
   new_imotos->ptr = 0;
   new_imotos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     imoa = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    imob = tos1->ptr;
   }
   new_imotos->ptr = imob % imoa;
   printf("%d %% %d = %d\n",imob,imoa,new_imotos->ptr);
   stack_Push(new_imotos,_stack);
   break;

   case OPCODE_INPLACE_TRUE_DIVIDE:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long itda = 0;
   long itdb = 0;
   object *new_itdtos = AllocObject();
   new_itdtos->flags = OFLAG_ON_STACK;
   new_itdtos->type = TYPE_INT;
   new_itdtos->ptr = 0;
   new_itdtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     itda = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    itdb = tos1->ptr;
   }
   new_itdtos->ptr = itdb / itda;
   printf("%d / %d = %d\n",itdb,itda,new_itdtos->ptr);
   stack_Push(new_itdtos,_stack);
   break;
   
   case OPCODE_INPLACE_FLOOR_DIVIDE:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long ifda = 0;
   long ifdb = 0;
   object *new_ifdtos = AllocObject();
   new_ifdtos->flags = OFLAG_ON_STACK;
   new_ifdtos->type = TYPE_INT;
   new_ifdtos->ptr = 0;
   new_ifdtos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     ifda = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    ifdb = tos1->ptr;
   }
   new_ifdtos->ptr = ifdb / ifda;
   printf("%d // %d = %d\n",ifdb,ifda,new_ifdtos->ptr);
   stack_Push(new_ifdtos,_stack);
   break;
   
   case OPCODE_INPLACE_POWER:
   printf("");
   tos = stack_Pop(_stack);
   tos1 = stack_Pop(_stack);
   if(tos->value_ptr != NULL)
    tos = (object*)tos->value_ptr;
   if(tos1->value_ptr != NULL)
    tos1 = (object*)tos1->value_ptr;
   long ifpa = 0;
   long ifpb = 0;
   object *new_ifptos = AllocObject();
   new_ifptos->flags = OFLAG_ON_STACK;
   new_ifptos->type = TYPE_INT;
   new_ifptos->ptr = 0;
   new_ifptos->value_ptr = NULL;
   if(tos->type == TYPE_INT)
   {
     ifpa = tos->ptr;
   }
   if(tos1->type == TYPE_INT)
   {
    ifpb = tos1->ptr;
   }
   new_ifptos->ptr = long_pow(ifpb , ifpa);
   printf("%d ** %d = %d\n",ifpb,ifpa,new_ifptos->ptr);
   stack_Push(new_ifptos,_stack);
   break;
   
   case OPCODE_COMPARE_OP:
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

/*    case OPCODE_PRINT_ITEM:
		tos = stack_Pop(_stack);
				if(tos != NULL)
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
						default:
							printf("object on tos (type:%c) not supported for printing\n",tos->type);
					}
		
		break;
*/	
	
	case OPCODE_SETUP_LOOP:
	{
	object *bo = AllocObject();
	block_object *block = AllocBlockObject();
	bo->type = TYPE_BLOCK;
	bo->ptr = block;
	bo->flags = OFLAG_ON_STACK;
	block->start = i;
	block->len = arg;
	printf("block - start: %d, len: %d\n",block->start,block->len);
	stack_Push(bo,blocks);
	}	
	//break;
	
    case OPCODE_CALL_FUNCTION:
	{
    stack *call = NULL;
	if(arg>0)
		call = stack_Init(arg);
 	tos = NULL;
	for(int i = 0;i<arg;i++)
	{
		stack_Push(stack_Pop(_stack),call);
	}
	object *function_name = stack_Pop(_stack);
	if(function_name != NULL&& function_name->type == TYPE_CODE)
		{
		printf("executing direct local function object: %s\n",((code_object*)function_name->ptr)->name);
		for(int i = 0;i<arg;i++)
	    { 
			stack_Push(stack_Pop(call),_stack);
	    }
	    object *ret = ExecuteObject((object*)function_name,obj,global,call,arg);
		if(ret != NULL)
		 stack_Push(ret,_stack);
		if(arg>0)
			stack_Close(call,0);
		 break;
        }
    
	if(function_name->value_ptr !=NULL && ((object*)function_name->value_ptr)->type == TYPE_CODE)
	{
		printf("executing local function object: %s\n",(char*)function_name->ptr);
		for(int i = 0;i<arg;i++)
	    { 
			stack_Push(stack_Pop(call),_stack);
	    }
	    object *ret = ExecuteObject((object*)function_name->value_ptr,obj,global,NULL,0);
		if(ret != NULL)
		 stack_Push(ret,_stack);
		if(arg>0)
			stack_Close(call,0);
		break;
	}
	
	if(function_name != NULL&& function_name->type == TYPE_UNICODE)
		{
		printf("executing C function: %s\n",(char*)function_name->ptr);
		 object *tmp = ExecuteCFunction((char*)function_name->ptr,call);
  	    if(tmp != NULL)
	    {
				stack_Push(tmp,_stack);	
		if(arg>0)
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
			object *ret = ExecuteObject((object*)caller_func->value_ptr,obj,global,call,arg);
			if(ret != NULL)
			stack_Push(ret,_stack);
			if(arg>0)
				stack_Close(call,0);
			break;
			}
		}
	    printf("function: [%s] not found\n",(char*)function_name->ptr);
	}
	}
	 break;
	
  default:
  printf("\n[%d,%xh] opcode: [ %s ] ,argcount:%d not supported\n(%s)\n",old_i,op,opcodes[index].name,opcodes[index].argcount,opcodes[index].description);
   break;
  }
  
   
 }
 printf("code thru.\n");
 object *ret = stack_Pop(_stack);
 stack_Close(_stack,0);
 return(ret); 
 }

return(NULL);
}
