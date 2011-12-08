#include "object.h"

object *AllocObject()
{
return((object*)mem_malloc(sizeof(object)));
}
code_object *AllocCodeObject()
{
return((code_object*)mem_malloc(sizeof(code_object)));
}
caller_object *AllocCallerObject()
{
return((caller_object*)mem_malloc(sizeof(caller_object)));
}
function_object *AllocFunctionObject()
{
return((function_object*)mem_malloc(sizeof(function_object)));
}
string_object *AllocStringObject()
{
return((string_object*)mem_malloc(sizeof(string_object)));
}
tuple_object *AllocTupleObject()
{
return((tuple_object*)mem_malloc(sizeof(tuple_object)));
}
unicode_object *AllocUnicodeObject()
{
return((unicode_object*)mem_malloc(sizeof(unicode_object)));
}

int IsIntObject(object *obj)
{
 return(obj->type == TYPE_INT);
}
int IsStringObject(object *obj)
{
 return(obj->type == TYPE_STRING);
}
int IsUnicodeObject(object *obj)
{
 return(obj->type == TYPE_UNICODE);
}
int IsCodeObject(object *obj)
{
 return(obj->type == TYPE_CODE);
}
int IsTupleObject(object *obj)
{
 return(obj->type == TYPE_TUPLE);
}


string_object *AsStringObject(object *obj)
{
return((string_object*)obj->ptr);
}
code_object *AsCodeObject(object *obj)
{
return((code_object*)obj->ptr);
}
caller_object *AsCallerObject(object *obj)
{
return((caller_object*)obj->ptr);
}
function_object *AsFunctionObject(object *obj)
{
return((function_object*)obj->ptr);
}
tuple_object *AsTupleObject(object *obj)
{
return((tuple_object*)obj->ptr);
}
unicode_object *AsUnicodeObject(object *obj)
{
return((unicode_object*)obj->ptr);
}


void FreeObject(object *obj)
{
switch(obj->type)
{
case TYPE_NULL:
   //printf("freeing NULL object @%x\n",obj);
   break;
case TYPE_NONE:
   //printf("freeing NONE object @%x\n",obj);
   break;
case TYPE_INT:
   //printf("freeing int object @%x\n",obj);
   break;
case TYPE_UNICODE:
   //printf("freeing unicode object @%x\n",obj);
   if(((unicode_object*)obj->ptr)->content != NULL)
    mem_free(((unicode_object*)obj->ptr)->content);
   break;
case TYPE_STRING:
   //printf("freeing string object @%x\n",obj);
   mem_free(((string_object*)obj->ptr)->content);
  break;
case TYPE_TUPLE:
   //printf("freeing tuple object @%x\n",obj);
   if(((tuple_object*)obj->ptr)->num>0)
   {
 for(int i=0;i<((tuple_object*)obj->ptr)->num;i++)
 {
  FreeObject(((tuple_object*)obj->ptr)->items[i]);
 }
  mem_free(((tuple_object*)obj->ptr)->items);
  }
 break; 
case TYPE_CODE:
 //printf("freeing code object @%x\n",obj);
 mem_free(((code_object*)obj->ptr)->name);
 FreeObject(((code_object*)obj->ptr)->code);
 FreeObject(((code_object*)obj->ptr)->consts);
 FreeObject(((code_object*)obj->ptr)->names);
 FreeObject(((code_object*)obj->ptr)->varnames);
 FreeObject(((code_object*)obj->ptr)->freevars);
 FreeObject(((code_object*)obj->ptr)->cellvars);
 //FreeObject(((code_object*)obj->ptr)->filename); //TO DECREASE MEMORY USAGE
 //FreeObject(((code_object*)obj->ptr)->lnotab);//TO DECREASE MEMORY USAGE
 break; 

}
//recycle_Remove(obj);
 
if(obj->type != TYPE_INT && obj->type != TYPE_NONE && obj->type != TYPE_NULL && obj->ptr != NULL)
 mem_free(obj->ptr);
//if(obj->value_ptr != NULL)
// FreeObject((object*)obj->value_ptr);
//printf("freed object(%c) @%x\n",obj->type,obj);
mem_free(obj);
}

void DumpObject(object *obj,int level)
{
for(int i=0;i<level;i++)
 printf("\t");
switch(obj->type)
{
case TYPE_NULL:
   printf("NULL object\n");
   break;
case TYPE_NONE:
   printf("NONE object\n");
   break;
case TYPE_INT:
   printf("int object: %d\n",obj->ptr);
   break;
case TYPE_UNICODE:
    printf("unicode object: %s\n",((unicode_object*)obj->ptr)->content);
   break;
case TYPE_STRING:
   printf("string object: ");
   for(int i=0;i<((string_object*)obj->ptr)->len;i++)
    printf("%02x ",(unsigned char)((string_object*)obj->ptr)->content[i]);
   printf("\n");
  break;
case TYPE_TUPLE:
   printf("tuple object\n");
   if(((tuple_object*)obj->ptr)->num>0)
   {
   for(int i=0;i<((tuple_object*)obj->ptr)->num;i++)
    {
    DumpObject(((tuple_object*)obj->ptr)->items[i],level+1);
    }
    for(int i=0;i<level;i++)
     printf("\t");
	printf("-- tuple object\n");
   }
 break; 
case TYPE_CODE:
 printf("code object\n");
 for(int i=0;i<level;i++)
 printf("\t");
 printf("name: %s\n",((code_object*)obj->ptr)->name);
 for(int i=0;i<level;i++)
 printf("\t");
 printf("code:\n");
 DumpObject(((code_object*)obj->ptr)->code,level+1);
 for(int i=0;i<level;i++)
 printf("\t");
 printf("consts:\n");
 DumpObject(((code_object*)obj->ptr)->consts,level+1);
 for(int i=0;i<level;i++)
 printf("\t");
 printf("names:\n");
 DumpObject(((code_object*)obj->ptr)->names,level+1);
 for(int i=0;i<level;i++)
 printf("\t");
 printf("varnames:\n");
 DumpObject(((code_object*)obj->ptr)->varnames,level+1);
 for(int i=0;i<level;i++)
 printf("\t");
 printf("freevars:\n");
 DumpObject(((code_object*)obj->ptr)->freevars,level+1);
 for(int i=0;i<level;i++)
 printf("\t");
 printf("cellvars:\n");
 DumpObject(((code_object*)obj->ptr)->cellvars,level+1);
 //for(int i=0;i<level;i++)
 //printf("\t");
 //printf("lnotab:\n");
 //DumpObject(((code_object*)obj->ptr)->lnotab,level+1);
 for(int i=0;i<level;i++)
 printf("\t");
 printf("-- code object\n");
 break; 
}
}
object *FindUnicodeTupleItem(tuple_object *tuple,char *name)
{
   //printf("checking in %d tuple items\n",tuple->num);
   for(int i=0;i<tuple->num;i++)
    {
	//printf("checking tuple:%d\n",i);
	//if(tuple->items[i]->type == TYPE_UNICODE)
	// printf("checking %s against: %s\n",name,AsUnicodeObject(tuple->items[i])->content);
     if(tuple->items[i]->type == TYPE_UNICODE && !strcmp( ((unicode_object*)tuple->items[i]->ptr)->content,name) )
	   return(tuple->items[i]);
    }
	return(NULL);
}


object *ReadObject(FILE *f)
{
 char type = ReadChar(f);
 //printf("type:%c\n",type);
 //long magic = ReadLong(f);
 object *obj = AllocObject();
 //printf("allocated object @%x\n",obj);
 obj->flags = 0;
 long n;
 switch(type)
 {
 case TYPE_CODE:
  //printf("reading code chunk\n");
  obj->type = TYPE_CODE;
  code_object *co = AllocCodeObject();
  //printf("co_ptr=%x\n",(unsigned long)co);
  co->argcount = ReadLong(f);
  co->kwonlyargcount= ReadLong(f);
  co->nlocals = ReadLong(f);
  co->stacksize = ReadLong(f);
  co->flags = ReadLong(f);
  //printf("argcount:%d,kwonlyargcount:%d,nlocals:%d,stacksize:%d,flags:%d\n",co->argcount,co->kwonlyargcount,co->nlocals,co->stacksize,co->flags);
  co->code = ReadObject(f);
  co->consts = ReadObject(f);
  co->names = ReadObject(f);
  co->varnames = ReadObject(f);
  co->freevars = ReadObject(f);
  co->cellvars = ReadObject(f);
  co->filename = ReadObject(f);
  FreeObject(co->filename); //TO DECREASE MEMORY USAGE
  object *tmp_name = ReadObject(f);
  co->firstlineno = ReadLong(f);
  co->lnotab = ReadObject(f);
  FreeObject(co->lnotab);//TO DECREASE MEMORY USAGE
  co->name = str_Copy(((string_object*)tmp_name->ptr)->content);//TODO free object tmp_name and string_copy it first
  FreeObject(tmp_name);
  printf("module name:%s\n",co->name);
  //printf("filename:%s\n",((string_object*)co->filename->ptr)->content);
  //printf("firstlineno:%d\n",co->firstlineno);
  obj->ptr = co;
  obj->value_ptr = NULL;
  //ReadChunk(f);
  break;
 case TYPE_STRING:
  //printf("reading string chunk\n");
   n= ReadLong(f);
   if(n>0)
   {
 char *string = (char*)mem_malloc(n);
 //printf("str_ptr=%x\n",(unsigned long)string);
 int read = fread(string,n,1,f);
 string_object *so = AllocStringObject();
 //printf("str_so_ptr=%x\n",(unsigned long)so);
 so->len = n;
 so->content = string;
 //free(string); 
 
 //printf("read string:\"%s\"\n",so->content);
 //printf("n:%d\n",so->len);
 obj->ptr = so;
 obj->type = TYPE_STRING;
 obj->value_ptr = NULL;
 }
 else
 {
 //printf("read empty string\n");
 obj->ptr = NULL;
 obj->type = TYPE_NULL; 
 obj->value_ptr = NULL;
 }
 break;
 case TYPE_TUPLE:
 //printf("reading tuple\n");
 n = ReadLong(f);
 tuple_object *to = AllocTupleObject(); 
 //printf("items_num:%d\n",n);
 if(n>0)
 {
 to->items = (object**)mem_malloc(n*sizeof(object*));
 to->num = n;
 for(int i=0;i<n;i++)
 {
  object* tuple = ReadObject(f);
  //printf("object type:%c\n",tuple->type);
  to->items[i] = tuple;
 }
 }
 obj->ptr = to;
 obj->type = TYPE_TUPLE;
 obj->value_ptr = NULL;
 break;
 case TYPE_UNICODE:
 //printf("reading unicode\n");
 n = ReadLong(f);
 unicode_object *uo = AllocUnicodeObject(); 
 //printf("len:%d\n",n);
 char *unicode_string = (char*)mem_malloc(n+1);
 memset(unicode_string,0,n+1);
 int uread = fread(unicode_string,n,1,f);
 //uo->len = n;
 uo->content = unicode_string;
 obj->ptr = uo;
 obj->type = TYPE_UNICODE;
 obj->value_ptr = NULL;
 break;
 case TYPE_INT:
 //printf("reading int\n");
 n = ReadLong(f);
 //unicode_object *uo = AllocUnicodeObject(); 
 //printf("len:%d\n",n);
 //char *unicode_string = (char*)mem_malloc(n+1);
 //memset(unicode_string,0,n+1);
 //int uread = fread(unicode_string,n,1,f);
 //uo->len = n;
 //uo->content = unicode_string;
 //obj->ptr = uo;
 obj->ptr = n;
 obj->type = TYPE_INT;
 obj->value_ptr = NULL;
 break;
 case TYPE_NONE:
  //printf("read none type\n");
  obj->ptr = NULL;
  obj->value_ptr = NULL;
  obj->type = TYPE_NONE;
 break;
 default:
 obj->ptr = NULL;
 obj->value_ptr = NULL;
 obj->type = TYPE_UNKNOWN;
  printf("unknown chunk type:%c\n",type);
}  
 return(obj); 
}


