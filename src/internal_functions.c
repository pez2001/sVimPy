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

#include "internal_functions.h"



object *if_list(vm *vm,stack * stack)
{
	int num = stack->list->num;
	//printf("num:%d\n",num);
	tuple_object *r = CreateTuple(num,0);
	//IncRefCount(r);
	for (int i = 0; i < num; i++)
	{
		//object *t = stack_Top(stack);
		//IncRefCount(t);
		object *t = stack_Pop(stack,vm->garbage);
		SetItem(r,i,t);
	}
	//DumpObject(r,0);
	return (r);
}

object *if_range(vm *vm,stack * stack)
{
	// TODO dirty hack in a classless world
	// stack_Dump(stack);
	// printf("range called:%d\n",stack->top);
	if (stack->list->num < 1)
	{
		object *tmp = CreateEmptyObject(TYPE_NONE,0);
		//IncRefCount(tmp);
		printf("not enough args for range\n");
		return (tmp);
	}
	object *s = stack_Pop(stack,vm->garbage);

	object *e =  NULL;
	if (stack->list->num > 0)
		e = stack_Pop(stack,vm->garbage);
	object *st = NULL;
	if (stack->list->num > 0)
		st = stack_Pop(stack,vm->garbage);

	if (st == NULL && e == NULL && s->type == TYPE_INT)
	{
		// printf("range called with 2 args\n");
		tuple_object *r = CreateTuple( ((int_object*)s)->value,0);
		//IncRefCount(r);

		for (int i = 0; i < r->list->num; i++)
		{
			r->list->items[i] = CreateIntObject(i,0);
			IncRefCount(r->list->items[i]);
		}
		// DumpObject(r,0);
		return (r);
	}
	else if (st == NULL && s->type == TYPE_INT && e->type == TYPE_INT)
	{
		// printf("range called with 2 args\n");
		tuple_object *r = CreateTuple( ((int_object*)e)->value - ((int_object*)s)->value, 0);
		//IncRefCount(r);
		//printf("range(%d,%d) tuple will contain %d items\n", s->ptr, e->ptr, n);

		//((tuple_object *) r->ptr)->items =
		//	(object **) mem_malloc(n * sizeof(object *), "if_range() items");
		for (int i = 0; i < r->list->num; i++)
		{
			r->list->items[i] = CreateIntObject( ((int_object*)s)->value + i,0);
			IncRefCount(r->list->items[i]);
		}
		// DumpObject(r,0);
		return (r);
	}
	else if (st != NULL && s->type == TYPE_INT && e->type == TYPE_INT
			 && st->type == TYPE_INT)
	{
		tuple_object *r = CreateTuple(((((int_object*)e)->value - ((int_object*)s)->value) / ((int_object*)st)->value) + 1,0);
		//IncRefCount(r);

		// printf("range(%d,%d,%d) step tuple will contain %d
		// items\n",s->ptr,e->ptr,st->ptr,n);
		//((tuple_object *) r->ptr)->items =
		//	(object **) mem_malloc(n * sizeof(object *), "if_range() items");
		// printf("filling tuple\n");
		for (int i = 0; i < r->list->num; i++)
		{
			 r->list->items[i] = CreateIntObject( ((int_object*)s)->value + (i * ((int_object*)st)->value),0);
			IncRefCount(r->list->items[i]);
		}
		// DumpObject(r,0);
		return (r);


	}
	object *tmp = CreateEmptyObject( TYPE_NONE,0);
	//IncRefCount(tmp);

	return (tmp);
}

object *if_print(vm *vm,stack * stack)
{
	// printf("print called\n");
	int num = stack->list->num;

	for (int i = 0; i < num; i++)
	{
		object *tos = stack_Pop(stack,vm->garbage);

		// if(tos != NULL && tos->type == TYPE_UNICODE)
		// printf("arg[%d]:%s\n",i,((unicode_object*)tos->ptr)->content );
		// else 
		if (i)
			printf(" ");
		if (tos != NULL)
			// printf("tos type:%c\n",tos->type);
			switch (tos->type)
			{
			case TYPE_REF:
				PrintObject(((ref_object*)tos)->ref);
				break;
			case TYPE_INT:
				printf("%d", ((int_object*)tos)->value);
				break;
			case TYPE_KV:
				PrintObject(((kv_object*)tos)->value);
				break;
			case TYPE_UNICODE:
				printf("%s", ((unicode_object *)tos)->value);
				break;
			case TYPE_TUPLE:
				PrintObject(tos);
				break;
			}
	
	}
	printf("\n");
	object *tmp =CreateEmptyObject(TYPE_NONE,0);
	//IncRefCount(tmp);
	return (tmp);
}

object *if_sum(vm *vm,stack * stack)
{
	// printf("print called\n");
	int num = stack->list->num;

	long sum = 0;
	for (int i = 0; i < num; i++)
	{
		object *tos = stack_Pop(stack,vm->garbage);

		if (tos != NULL)
			switch (tos->type)
			{
			case TYPE_INT:
				sum += ((int_object*)tos)->value;
				break;
			case TYPE_UNICODE:
				break;
			}
	}
	int_object *tmp = CreateIntObject(sum,0);
	printf("returning sum:%d\n",sum);
	//IncRefCount(tmp);
	return (tmp);
}
