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
case TYPE_UNICODE:
   printf("freeing unicode object @%x\n",obj);
   if(((unicode_object*)obj->ptr)->content != NULL)
    mem_free(((unicode_object*)obj->ptr)->content);
   break;
case TYPE_STRING:
   printf("freeing string object @%x\n",obj);
   mem_free(((string_object*)obj->ptr)->content);
  break;
case TYPE_TUPLE:
   printf("freeing tuple object @%x\n",obj);
 for(int i=0;i<((tuple_object*)obj->ptr)->num;i++)
 {
  FreeObject(((tuple_object*)obj->ptr)->items[i]);
 }
 break; 
case TYPE_CODE:
 printf("freeing code object @%x\n",obj);
 mem_free(obj->name);
 FreeObject(((code_object*)obj->ptr)->code);
 FreeObject(((code_object*)obj->ptr)->consts);
 FreeObject(((code_object*)obj->ptr)->names);
 FreeObject(((code_object*)obj->ptr)->varnames);
 FreeObject(((code_object*)obj->ptr)->freevars);
 FreeObject(((code_object*)obj->ptr)->cellvars);
 FreeObject(((code_object*)obj->ptr)->filename);
 FreeObject(((code_object*)obj->ptr)->lnotab);
 break; 

}
if(obj->type != TYPE_INT && obj->type != TYPE_NONE && obj->type != TYPE_NULL && obj->ptr != NULL)
 mem_free(obj->ptr);
if(obj->value_ptr != NULL)
 FreeObject((object*)obj->value_ptr);
printf("freed object @%x\n",obj);
 mem_free(obj);
}


object *ReadObject(FILE *f)
{
 char type = ReadChar(f);
 //printf("type:%c\n",type);
 //long magic = ReadLong(f);
 object *obj = AllocObject();
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
  object *tmp_name = ReadObject(f);
  co->firstlineno = ReadLong(f);
  co->lnotab = ReadObject(f);
  obj->name = ((string_object*)tmp_name->ptr)->content;//TODO free object tmp_name and string_copy it first
  printf("module name:%s\n",obj->name);
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
 to->items = (object**)mem_malloc(n*sizeof(object*));
 to->num = n;
 for(int i=0;i<n;i++)
 {
  object* tuple = ReadObject(f);
  //printf("object type:%c\n",tuple->type);
  to->items[i] = tuple;
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
 uo->len = n;
 uo->content = unicode_string;
 obj->ptr = uo;
 obj->type = TYPE_UNICODE;
 obj->value_ptr = NULL;
 break;
 case TYPE_INT:
 //printf("reading unicode\n");
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


