/* 
 * sVimPy - small Virtual interpreting machine for Python
 * (c) 2012 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */


#include "object.h"

object *AllocEmptyObject()
{
	return ((object *)
			mem_malloc(sizeof(empty_object), "AllocEmptyObject() return"));
}

object *AllocValuedObject()
{
	return ((object *)
			mem_malloc(sizeof(valued_object), "AllocValuedObject() return"));
}

object *AllocObject()
{
	return ((object *) mem_malloc(sizeof(object), "AllocObject() return"));
}

code_object *AllocCodeObject()
{
	return ((code_object *)
			mem_malloc(sizeof(code_object), "AllocCodeObject() return"));
}

caller_object *AllocCallerObject()
{
	return ((caller_object *)
			mem_malloc(sizeof(caller_object), "AllocCallerObject() return"));
}

function_object *AllocFunctionObject()
{
	return ((function_object *)
			mem_malloc(sizeof(function_object),
					   "AllocFunctionObject() return"));
}

string_object *AllocStringObject()
{
	return ((string_object *)
			mem_malloc(sizeof(string_object), "AllocStringObject() return"));
}

tuple_object *AllocTupleObject()
{
	return ((tuple_object *)
			mem_malloc(sizeof(tuple_object), "AllocTupleObject() return"));
}

block_object *AllocBlockObject()
{
	return ((block_object *)
			mem_malloc(sizeof(block_object), "AllocBlockObject() return"));
}

/* unicode_object *AllocUnicodeObject() {
   return((unicode_object*)mem_malloc(sizeof(unicode_object))); } */
int IsIntObject(object * obj)
{
	return (obj->type == TYPE_INT);
}

int IsStringObject(object * obj)
{
	return (obj->type == TYPE_STRING);
}

int IsUnicodeObject(object * obj)
{
	return (obj->type == TYPE_UNICODE);
}

int IsCodeObject(object * obj)
{
	return (obj->type == TYPE_CODE);
}

int IsTupleObject(object * obj)
{
	return (obj->type == TYPE_TUPLE);
}


string_object *AsStringObject(object * obj)
{
	return ((string_object *) obj->ptr);
}

code_object *AsCodeObject(object * obj)
{
	return ((code_object *) obj->ptr);
}

caller_object *AsCallerObject(object * obj)
{
	return ((caller_object *) obj->ptr);
}

function_object *AsFunctionObject(object * obj)
{
	return ((function_object *) obj->ptr);
}

tuple_object *AsTupleObject(object * obj)
{
	return ((tuple_object *) obj->ptr);
}

/* unicode_object *AsUnicodeObject(object *obj) {
   return((unicode_object*)obj->ptr); } */
long objects_num = 0;

long objects_max = 0;

long objects_header_total = 0;

void FreeObject(object * obj)
{
	assert(obj != NULL);
	if (obj == NULL)
		return;
	// printf("object to be freed\n");
	// DumpObject(obj,1);

	switch (obj->type)
	{
	case TYPE_NULL:
		// printf("freeing NULL object @%x\n",obj);
		objects_header_total -= sizeof(empty_object);
		break;
	case TYPE_NONE:
		// printf("freeing NONE object @%x\n",obj);
		objects_header_total -= sizeof(empty_object);
		break;
	case TYPE_INT:
		// printf("freeing int object @%x\n",obj);
		objects_header_total -= sizeof(empty_object);
		break;
	case TYPE_UNICODE:
		// printf("freeing unicode object @%x\n",obj);
		// if(((unicode_object*)obj->ptr)->content != NULL)
		// if(obj->ptr != NULL)
		// mem_free(obj->ptr);
		objects_header_total -= sizeof(object);
		break;
	case TYPE_STRING:
		// printf("freeing string object @%x\n",obj);
		objects_header_total -= sizeof(object);
		assert(mem_free(((string_object *) obj->ptr)->content));
		break;
	case TYPE_TUPLE:
		// printf("freeing tuple object @%x\n",obj);
		objects_header_total -= sizeof(object);
		if (((tuple_object *) obj->ptr)->num > 0)
		{
			for (int i = 0; i < ((tuple_object *) obj->ptr)->num; i++)
			{
				if ((((tuple_object *) obj->ptr)->items[i]) != NULL
					&& !(((tuple_object *) obj->ptr)->items[i]->
						 flags & OFLAG_ON_STACK))
				{
					FreeObject(((tuple_object *) obj->ptr)->items[i]);
				}
				((tuple_object *) obj->ptr)->items[i] = NULL;
			}
			assert(mem_free(((tuple_object *) obj->ptr)->items));
			((tuple_object *) obj->ptr)->items = NULL;
		}
		break;
	case TYPE_CODE:
		// printf("freeing code object @%x\n",obj);
		objects_header_total -= sizeof(object);
		objects_header_total -= strlen(((code_object *) obj->ptr)->name) + 1;
		assert(mem_free(((code_object *) obj->ptr)->name));
		FreeObject(((code_object *) obj->ptr)->code);
		FreeObject(((code_object *) obj->ptr)->consts);
		FreeObject(((code_object *) obj->ptr)->names);
		FreeObject(((code_object *) obj->ptr)->varnames);
		FreeObject(((code_object *) obj->ptr)->freevars);
		FreeObject(((code_object *) obj->ptr)->cellvars);
		// FreeObject(((code_object*)obj->ptr)->filename); //TO DECREASE
		// MEMORY USAGE
		// FreeObject(((code_object*)obj->ptr)->lnotab);//TO DECREASE MEMORY
		// USAGE
		break;

	}
	// recycle_Remove(obj);

	if (obj->type != TYPE_INT && obj->type != TYPE_NONE
		&& obj->type != TYPE_NULL && obj->type != TYPE_TRUE
		&& obj->type != TYPE_FALSE && obj->ptr != NULL)
		assert(mem_free(obj->ptr));
	// if(obj->value_ptr != NULL)
	// FreeObject((object*)obj->value_ptr);
	// printf("freed object(%c) @%x\n",obj->type,obj);
	assert(mem_free(obj));
	objects_num--;
}

void PrintObject(object * obj)
{
	if (obj != NULL)
	{
		// printf("tos type:%c\n",obj->type);
		switch (obj->type)
		{
		case TYPE_INT:
			printf("%d", (long)obj->ptr);
			break;
		case TYPE_UNICODE:
			if ((object *) obj->value_ptr != NULL)
			{
				if (((object *) obj->value_ptr)->type == TYPE_UNICODE)
					printf("%s", (char *)((object *) obj->value_ptr)->ptr);
				else if (((object *) obj->value_ptr)->type == TYPE_INT)
					printf("%d", ((object *) obj->value_ptr)->ptr);
			}
			else
				printf("%s", (char *)obj->ptr);
			break;
		}

	}
}

/* void DumpObject(object *obj) { DumpObject(obj,0); } */
void DumpObject(object * obj, int level)
{
	if (obj == NULL)
		return;
	// printf("level:%d\n",level);
	for (int i = 0; i < level; i++)
		printf("\t");
	switch (obj->type)
	{
	case TYPE_TRUE:
		printf("true object\n");
		break;
	case TYPE_FALSE:
		printf("false object\n");
		break;
	case TYPE_NULL:
		printf("NULL object\n");
		break;
	case TYPE_NONE:
		printf("NONE object\n");
		break;
	case TYPE_INT:
		printf("int object: %d\n", obj->ptr);
		break;
	case TYPE_UNICODE:
		// printf("unicode object:
		// %s\n",((unicode_object*)obj->ptr)->content);
		printf("unicode object: %s\n", obj->ptr);
		break;
	case TYPE_STRING:
		printf("string object: ");
		for (int i = 0; i < ((string_object *) obj->ptr)->len; i++)
			printf("%02x ",
				   (unsigned char)((string_object *) obj->ptr)->content[i]);
		printf("\n");
		break;
	case TYPE_TUPLE:
		printf("tuple object");
		if (((tuple_object *) obj->ptr)->num > 0)
		{
			printf(" contains %d items\n", ((tuple_object *) obj->ptr)->num);
			for (int i = 0; i < ((tuple_object *) obj->ptr)->num; i++)
			{
				if (((tuple_object *) obj->ptr)->items[i] != NULL)
				{

					if ((((tuple_object *) obj->ptr)->items[i]->
						 flags & OFLAG_TUPLE_PTR) > 0)
						printf("->");
					DumpObject(((tuple_object *) obj->ptr)->items[i],
							   level + 1);
				}
			}
		}
		else
		{
			printf("\n");
			for (int i = 0; i < level; i++)
				printf("\t");
			printf("empty tuple\n");
		}
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("-- tuple object\n");
		break;
	case TYPE_CODE:
		printf("code object\n");
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("name: %s\n", ((code_object *) obj->ptr)->name);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("code:\n");
		DumpObject(((code_object *) obj->ptr)->code, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("consts:\n");
		DumpObject(((code_object *) obj->ptr)->consts, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("names:\n");
		DumpObject(((code_object *) obj->ptr)->names, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("varnames:\n");
		DumpObject(((code_object *) obj->ptr)->varnames, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("freevars:\n");
		DumpObject(((code_object *) obj->ptr)->freevars, level + 1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("cellvars:\n");
		DumpObject(((code_object *) obj->ptr)->cellvars, level + 1);
		// for(int i=0;i<level;i++)
		// printf("\t");
		// printf("lnotab:\n");
		// DumpObject(((code_object*)obj->ptr)->lnotab,level+1);
		for (int i = 0; i < level; i++)
			printf("\t");
		printf("-- code object\n");
		break;
	default:
		printf("object type is unknown:%c\n", obj->type);
	}
}

object *FindUnicodeTupleItem(object * tuple, char *name)
{
	// printf("checking for correct type\n");
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return (NULL);
	// printf("checking in %d tuple items for
	// %s\n",((tuple_object*)tuple->ptr)->num,name);
	for (int i = 0; i < ((tuple_object *) tuple->ptr)->num; i++)
	{
		// printf("checking tuple:%d\n",i);
		// if(((tuple_object*)tuple->ptr)->items[i]->type == TYPE_UNICODE)
		// printf("checking %s against:
		// %s\n",name,(char*)((tuple_object*)tuple->ptr)->items[i]->ptr);
		if (((tuple_object *) tuple->ptr)->items[i] == NULL)
			continue;
		if (((tuple_object *) tuple->ptr)->items[i]->type == TYPE_UNICODE
			&& !strcmp((char *)((tuple_object *) tuple->ptr)->items[i]->ptr,
					   name))
			return (((tuple_object *) tuple->ptr)->items[i]);
	}
	return (NULL);
}

void ResetIteration(object * tuple)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;

	if ((tuple->flags & OFLAG_TUPLE_RESTART_FLAG))
	{
		tuple->flags ^= OFLAG_TUPLE_RESTART_FLAG;
		return;
	}

	for (int i = 0; i < ((tuple_object *) tuple->ptr)->num; i++)
	{
		// printf("checking tuple:%d\n",i);
		// if(((tuple_object*)tuple->ptr)->items[i]->type == TYPE_UNICODE)
		// printf("checking %s against:
		// %s\n",name,(char*)((tuple_object*)tuple->ptr)->items[i]->ptr);
		if (((tuple_object *) tuple->ptr)->items[i] != NULL
			&& (((tuple_object *) tuple->ptr)->items[i]->
				flags & OFLAG_TUPLE_PTR) > 0)
		{
			// update tuple ptr
			((tuple_object *) tuple->ptr)->items[i]->flags ^= OFLAG_TUPLE_PTR;
		}
	}
	if (((tuple_object *) tuple->ptr)->items[0] != NULL)
		((tuple_object *) tuple->ptr)->items[0]->flags |= OFLAG_TUPLE_PTR;

}

void SetItem(object * tuple, int index, object * obj)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if (index >= ((tuple_object *) tuple->ptr)->num || index < 0)
		return;
	((tuple_object *) tuple->ptr)->items[index] = obj;
	// obj->flags ^= OFLAG_ON_STACK;
}

void DeleteItem(object * tuple, int index)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return;
	if (index >= ((tuple_object *) tuple->ptr)->num || index < 0)
		return;
	if (((tuple_object *) tuple->ptr)->items[index] == NULL)
		return;
	if (!(((tuple_object *) tuple->ptr)->items[index]->type & OFLAG_ON_STACK))
		FreeObject(((tuple_object *) tuple->ptr)->items[index]);
	((tuple_object *) tuple->ptr)->items[index] = NULL;
}




object *GetItem(object * tuple, int index)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return (NULL);
	if (index >= ((tuple_object *) tuple->ptr)->num || index < 0)
		return (NULL);
	return (((tuple_object *) tuple->ptr)->items[index]);
}

object *GetNextItem(object * tuple)
{
	if (tuple == NULL || tuple->type != TYPE_TUPLE)
		return (NULL);

	if ((tuple->flags & OFLAG_TUPLE_RESTART_FLAG))
	{
		tuple->flags ^= OFLAG_TUPLE_RESTART_FLAG;
		return (NULL);
	}


	for (int i = 0; i < ((tuple_object *) tuple->ptr)->num; i++)
	{
		if (((tuple_object *) tuple->ptr)->items[i] == NULL)
			continue;
		// printf ("checking tuple:%d\n", i);
		// if(((tuple_object*)tuple->ptr)->items[i]->type == TYPE_UNICODE)
		// printf("checking %s against:
		// %s\n",name,(char*)((tuple_object*)tuple->ptr)->items[i]->ptr);
		if ((((tuple_object *) tuple->ptr)->items[i]->
			 flags & OFLAG_TUPLE_PTR) > 0)
		{
			// update tuple ptr
			((tuple_object *) tuple->ptr)->items[i]->flags ^= OFLAG_TUPLE_PTR;
			if (i + 1 < ((tuple_object *) tuple->ptr)->num)
				((tuple_object *) tuple->ptr)->items[i + 1]->flags |=
					OFLAG_TUPLE_PTR;
			else				// reset
			{
				((tuple_object *) tuple->ptr)->items[0]->flags |=
					OFLAG_TUPLE_PTR;
				tuple->flags |= OFLAG_TUPLE_RESTART_FLAG;
				// return(NULL);
			}
			// printf("returning tuple iteration\n");
			return (((tuple_object *) tuple->ptr)->items[i]);
		}
	}
	return (NULL);
}


object *ReadObject(FILE * f)
{
	objects_num++;
	// printf("increasing objects num to %d\n",objects_num);
	if (objects_num > objects_max)
		objects_max = objects_num;
	char type = ReadChar(f);

	// printf("type:%c\n",type);
	// long magic = ReadLong(f);
	object *obj = NULL;

	if (type == TYPE_NONE || type == TYPE_NULL || type == TYPE_TRUE
		|| type == TYPE_FALSE)
	{
		// printf("allocating empty object\n");
		obj = AllocEmptyObject();
		obj->type = type;
		obj->flags = 0;
		// printf("allocated empty object @%x\n",obj);
		objects_header_total += sizeof(empty_object);
		return (obj);
	}
	// else
	// printf("allocating object\n");
	obj = AllocObject();
	objects_header_total += sizeof(object);
	// printf("allocated object @%x\n",obj);
	obj->flags = 0;
	long n;

	switch (type)
	{
	case TYPE_CODE:
		// printf("reading code chunk\n");
		obj->type = TYPE_CODE;
		code_object *co = AllocCodeObject();

		// printf("co_ptr=%x\n",(unsigned long)co);
		co->argcount = ReadLong(f);
		co->kwonlyargcount = ReadLong(f);
		co->nlocals = ReadLong(f);
		co->stacksize = ReadLong(f);
		co->flags = ReadLong(f);
		// printf("argcount:%d,kwonlyargcount:%d,nlocals:%d,stacksize:%d,flags:%d\n",co->argcount,co->kwonlyargcount,co->nlocals,co->stacksize,co->flags);
		co->code = ReadObject(f);
		co->consts = ReadObject(f);
		co->names = ReadObject(f);
		co->varnames = ReadObject(f);
		co->freevars = ReadObject(f);
		co->cellvars = ReadObject(f);
		// printf("reading filename\n");
		object *tmp_filename = ReadObject(f);

		// co->filename = ReadObject(f);
		// printf("filename:%s\n",(char*)co->filename->ptr);
		// FreeObject(co->filename); //TO DECREASE MEMORY USAGE
		FreeObject(tmp_filename);	// TO DECREASE MEMORY USAGE
		object *tmp_name = ReadObject(f);

		// if(IsUnicodeObject(tmp_name))
		// printf("name:%s\n",(char*)tmp_name->ptr);
		co->name = str_Copy((char *)tmp_name->ptr);
		objects_header_total += strlen(co->name) + 1;
		FreeObject(tmp_name);
		co->firstlineno = ReadLong(f);
		// co->lnotab = ReadObject(f);
		object *tmp_lnotab = ReadObject(f);

		// printf("read lnotab\n");
		// FreeObject(co->lnotab);//TO DECREASE MEMORY USAGE
		FreeObject(tmp_lnotab);	// TO DECREASE MEMORY USAGE
		// printf("freed lnotab\n");
		// co->name =
		// str_Copy(((string_object*)tmp_name->ptr)->content);//TODO free
		// object tmp_name and string_copy it first

		// printf("string copied");
		// FreeObject(tmp_name);
		// printf ("module name:%s\n", co->name);
		// printf("filename:%s\n",((string_object*)co->filename->ptr)->content);
		// printf("firstlineno:%d\n",co->firstlineno);
		obj->ptr = co;
		obj->value_ptr = NULL;
		// ReadChunk(f);
		break;
	case TYPE_STRING:
		// printf("reading string chunk\n");
		n = ReadLong(f);
		if (n > 0)
		{
			char *string = (char *)mem_malloc(n, "ReadObject() string");

			// printf("str_ptr=%x\n",(unsigned long)string);
			int read = fread(string, n, 1, f);

			string_object *so = AllocStringObject();

			// printf("str_so_ptr=%x\n",(unsigned long)so);
			so->len = n;
			so->content = string;
			// free(string); 

			// printf("read string:\"%s\"\n",so->content);
			// printf("n:%d\n",so->len);
			obj->ptr = so;
			obj->type = TYPE_STRING;
			obj->value_ptr = NULL;
		}
		else
		{
			// printf ("read empty string\n");
			obj->ptr = NULL;
			obj->type = TYPE_NULL;
			obj->value_ptr = NULL;
		}
		break;
	case TYPE_TUPLE:
		// printf("reading tuple\n");
		n = ReadLong(f);
		tuple_object *to = AllocTupleObject();

		// printf("items_num:%d\n",n);
		if (n > 0)
		{
			to->items =
				(object **) mem_malloc(n * sizeof(object *),
									   "ReadObject() to->items");
			for (int i = 0; i < n; i++)
			{
				object *tuple = ReadObject(f);

				if (i == 0)
				{
					tuple->flags |= OFLAG_TUPLE_PTR;
					// printf("setting tuple's iterate over ptr\n");
				}
				// printf("object type:%c\n",tuple->type);
				to->items[i] = tuple;
			}
		}
		to->num = n;
		obj->ptr = to;
		obj->type = TYPE_TUPLE;
		obj->value_ptr = NULL;
		break;
	case TYPE_UNICODE:
		// printf("reading unicode\n");
		n = ReadLong(f);
		// printf("len:%d\n",n);
		char *unicode_string =
			(char *)mem_malloc(n + 1, "ReadObject() unicode_string");
		memset(unicode_string, 0, n + 1);
		int uread = fread(unicode_string, n, 1, f);

		// uo->len = n;
		obj->ptr = unicode_string;
		obj->type = TYPE_UNICODE;
		obj->value_ptr = NULL;
		break;
	case TYPE_INT:
		// printf("reading int\n");
		n = ReadLong(f);
		// unicode_object *uo = AllocUnicodeObject(); 
		// printf("len:%d\n",n);
		// char *unicode_string = (char*)mem_malloc(n+1);
		// memset(unicode_string,0,n+1);
		// int uread = fread(unicode_string,n,1,f);
		// uo->len = n;
		// uo->content = unicode_string;
		// obj->ptr = uo;
		obj->ptr = n;
		obj->type = TYPE_INT;
		obj->value_ptr = NULL;
		break;
	default:
		// obj->ptr = NULL;
		// obj->value_ptr = NULL;
		obj->type = TYPE_UNKNOWN;
		printf("unknown chunk type:%c\n", type);
	}
	// printf("read object\n"); 
	return (obj);
}
