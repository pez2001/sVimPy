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



object *BuildList(stack * stack)
{
	int num = stack->list->num;

	object *r = AllocObject();

	r->type = TYPE_TUPLE;
	r->flags = OFLAG_ON_STACK;
	r->ptr = AllocTupleObject();
	r->value_ptr = NULL;
	((tuple_object *) r->ptr)->num = num;
	((tuple_object *) r->ptr)->items =
		(object **) mem_malloc(num * sizeof(object *), "BuildList() items");
	for (int i = 0; i < num; i++)
	{
		((tuple_object *) r->ptr)->items[i] = stack_Pop(stack);
	}
	// DumpObject(r,0);
	return (r);
}



object *if_range(stack * stack)
{
	// TODO dirty hack in a classless world
	// stack_Dump(stack);
	// printf("range called:%d\n",stack->top);
	if (stack->list->num < 1)
	{
		object *tmp = AllocEmptyObject();

		tmp->type = TYPE_NONE;
		tmp->flags = OFLAG_ON_STACK;
		printf("not enough args for range\n");
		return (tmp);
	}
	object *s = stack_Pop(stack);

	object *e =  NULL;
	if (stack->list->num > 0)
		e = stack_Pop(stack);
	object *st = NULL;
	if (stack->list->num > 0)
		st = stack_Pop(stack);

	if (st == NULL && e == NULL && s->type == TYPE_INT)
	{
		// printf("range called with 2 args\n");
		object *r = AllocObject();

		r->type = TYPE_TUPLE;
		r->flags = OFLAG_ON_STACK;
		r->ptr = AllocTupleObject();
		r->value_ptr = NULL;
		int n = (long)s->ptr;
		//printf("range(%d,%d) tuple will contain %d items\n", s->ptr, e->ptr, n);

		((tuple_object *) r->ptr)->num = n;
		((tuple_object *) r->ptr)->items =
			(object **) mem_malloc(n * sizeof(object *), "if_range() items");
		for (int i = 0; i < n; i++)
		{
			((tuple_object *) r->ptr)->items[i] = AllocObject();
			((tuple_object *) r->ptr)->items[i]->type = TYPE_INT;
			((tuple_object *) r->ptr)->items[i]->ptr = i;
			((tuple_object *) r->ptr)->items[i]->value_ptr = NULL;
		}
		// DumpObject(r,0);
		return (r);
	}
	else if (st == NULL && s->type == TYPE_INT && e->type == TYPE_INT)
	{
		// printf("range called with 2 args\n");
		object *r = AllocObject();

		r->type = TYPE_TUPLE;
		r->flags = OFLAG_ON_STACK;
		r->ptr = AllocTupleObject();
		r->value_ptr = NULL;
		int n = (long)e->ptr - (long)s->ptr;
		//printf("range(%d,%d) tuple will contain %d items\n", s->ptr, e->ptr, n);

		((tuple_object *) r->ptr)->num = n;
		((tuple_object *) r->ptr)->items =
			(object **) mem_malloc(n * sizeof(object *), "if_range() items");
		for (int i = 0; i < n; i++)
		{
			((tuple_object *) r->ptr)->items[i] = AllocObject();
			((tuple_object *) r->ptr)->items[i]->type = TYPE_INT;
			((tuple_object *) r->ptr)->items[i]->ptr = (long)s->ptr + i;
			((tuple_object *) r->ptr)->items[i]->value_ptr = NULL;
		}
		// DumpObject(r,0);
		return (r);
	}
	else if (st != NULL && s->type == TYPE_INT && e->type == TYPE_INT
			 && st->type == TYPE_INT)
	{
		object *r = AllocObject();

		r->type = TYPE_TUPLE;
		r->flags = OFLAG_ON_STACK;
		r->ptr = AllocTupleObject();
		r->value_ptr = NULL;
		int n = (((long)e->ptr - (long)s->ptr) / (long)st->ptr) + 1;
		// printf("range(%d,%d,%d) step tuple will contain %d
		// items\n",s->ptr,e->ptr,st->ptr,n);
		((tuple_object *) r->ptr)->num = n;
		((tuple_object *) r->ptr)->items =
			(object **) mem_malloc(n * sizeof(object *), "if_range() items");
		// printf("filling tuple\n");
		for (int i = 0; i < n; i++)
		{
			((tuple_object *) r->ptr)->items[i] = AllocObject();
			((tuple_object *) r->ptr)->items[i]->type = TYPE_INT;
			((tuple_object *) r->ptr)->items[i]->flags = OFLAG_ON_STACK;
			((tuple_object *) r->ptr)->items[i]->ptr =
				(long)s->ptr + (i * (long)st->ptr);
			((tuple_object *) r->ptr)->items[i]->value_ptr = NULL;
		}
		// DumpObject(r,0);
		return (r);


	}
	object *tmp = AllocEmptyObject();

	tmp->type = TYPE_NONE;
	tmp->flags = OFLAG_ON_STACK;
	return (tmp);
}

object *if_print(stack * stack)
{
	// printf("print called\n");
	int num = stack->list->num;

	for (int i = 0; i < num; i++)
	{
		object *tos = stack_Pop(stack);

		// if(tos != NULL && tos->type == TYPE_UNICODE)
		// printf("arg[%d]:%s\n",i,((unicode_object*)tos->ptr)->content );
		// else 
		if (i)
			printf(" ");
		if (tos != NULL)
			// printf("tos type:%c\n",tos->type);
			switch (tos->type)
			{
			case TYPE_INT:
				printf("%d", (long)tos->ptr);
				break;
			case TYPE_UNICODE:
				if ((object *) tos->value_ptr != NULL)
				{
					if (((object *) tos->value_ptr)->type == TYPE_UNICODE)
						printf("%s", (char *)((object *) tos->value_ptr)->ptr);
					else if (((object *) tos->value_ptr)->type == TYPE_INT)
						printf("%d", ((object *) tos->value_ptr)->ptr);
				}
				else
					printf("%s", (char *)tos->ptr);
				break;
			}
		// else 
		// printf("tos = NULL\n");
	}
	object *tmp = AllocEmptyObject();

	tmp->type = TYPE_NONE;
	tmp->flags = OFLAG_ON_STACK;
	return (tmp);
}
