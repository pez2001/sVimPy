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

object *StringAdd(object *a,object *b)
{
	char *as;
	char *bs;
	as = ((unicode_object *)a)->value;
	bs = ((unicode_object *)b)->value;
	char *tmp = str_Cat(as,bs);
	unicode_object *r = CreateUnicodeObject(tmp,0);
	return(r);
}

object *BinaryOp(object *tos,object *tos1,unsigned char op)
{
	object *new_tos = NULL;
	if (tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE && op == OPCODE_BINARY_ADD)
	{
		return(StringAdd(tos,tos1));
	}
	
	if (tos->type == TYPE_INT && tos1->type == TYPE_TUPLE && op == OPCODE_BINARY_MULTIPLY)
	{
		long a = ((int_object*)tos)->value;

		int mnum = ((tuple_object *) tos1)->list->num;
		if (mnum == 1)
			tos1 = ((tuple_object *) tos1)->list->items[0];

		tuple_object *mtr = CreateTuple(a,0);
		for (int i = 0; i < a; i++)
		{
			SetItem(mtr,i,tos1);
		}
		if((debug_level & DEBUG_VERBOSE_STEP) > 0)
			DumpObject(mtr,0);
		return(mtr);
	}
	//printf("binary op:%02x\n",op);
	if(tos->type == TYPE_BINARY_FLOAT || tos1->type == TYPE_BINARY_FLOAT)
	{
		float af = 0.0f;
		float bf = 0.0f;
		//printf("float ret\n");
		
		if (tos->type == TYPE_INT)
		{
			af = (float) ((int_object*)tos)->value;
		}	
		if (tos1->type == TYPE_INT)
		{
			bf = (float) ((int_object*)tos1)->value;
		}	
		if (tos->type == TYPE_BINARY_FLOAT)
		{
			af = ((float_object*)tos)->value;
		}	
		if (tos1->type == TYPE_BINARY_FLOAT)
		{
			bf = ((float_object*)tos1)->value;
		}	
		new_tos =CreateFloatObject(0,0);
		switch(op)
		{
			case OPCODE_INPLACE_MULTIPLY:
			case OPCODE_BINARY_MULTIPLY:
				{
					((float_object*)new_tos)->value = bf  * af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g * %7g = %7g\n", bf, af, bf  * af);
				}
				break;
			case OPCODE_INPLACE_OR:
			case OPCODE_BINARY_OR:
				{
					((float_object*)new_tos)->value = (long)bf  | (long)af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g | %7g = %7g\n", bf, af, (long)bf  | (long)af);
				}
				break;
			case OPCODE_INPLACE_XOR:
			case OPCODE_BINARY_XOR:
				{
					((float_object*)new_tos)->value = (long)bf  ^ (long)af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g ^ %7g = %7g\n", bf, af, (long)bf  ^ (long)af);
				}
				break;			
			case OPCODE_INPLACE_AND:
			case OPCODE_BINARY_AND:
				{
					((float_object*)new_tos)->value = (long)bf  & (long)af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g & %7g = %7g\n", bf, af, (long)bf  & (long)af);
				}
				break;
			case OPCODE_INPLACE_LSHIFT:
			case OPCODE_BINARY_LSHIFT:
				{
					((float_object*)new_tos)->value = (long)bf << (long)af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g << %7g = %7g\n", bf, af, (long)bf << (long)af);
				}
				break;
			case OPCODE_INPLACE_RSHIFT:
			case OPCODE_BINARY_RSHIFT:
				{
					((float_object*)new_tos)->value =  (long)bf >>  (long)af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g>> %7g = %7g\n", bf, af,  (long)bf >>  (long)af);
				}
				break;
			case OPCODE_INPLACE_MODULO:
			case OPCODE_BINARY_MODULO:
				{
					((float_object*)new_tos)->value =  (long)bf %  (long)af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g %% %7g = %7g\n", bf, af,  (long)bf %  (long)af);
				}
				break;
			case OPCODE_INPLACE_FLOOR_DIVIDE:
			case OPCODE_BINARY_FLOOR_DIVIDE:
				{
					((float_object*)new_tos)->value = floor(bf / af);
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g // %7g = %7g\n", bf, af, floor(bf / af));
				}
				break;
			case OPCODE_INPLACE_TRUE_DIVIDE:
			case OPCODE_BINARY_TRUE_DIVIDE:
				{
					((float_object*)new_tos)->value = bf / af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g / %7g = %7g\n", bf, af, bf / af);
				}
				break;
			case OPCODE_INPLACE_SUBTRACT:
			case OPCODE_BINARY_SUBTRACT:
				{
					((float_object*)new_tos)->value = bf-af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g - %7g = %7g\n", bf, af, bf - af);
				}
				break;
			case OPCODE_INPLACE_POWER:
			case OPCODE_BINARY_POWER:
				{
					((float_object*)new_tos)->value = long_pow(bf, af);
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g ** %7g = %7g\n", bf, af, long_pow(bf, af));
				}
				break;
			case OPCODE_INPLACE_ADD:
			case OPCODE_BINARY_ADD:
				{
					((float_object*)new_tos)->value = bf + af;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%7g + %7g = %7g\n", bf, af, bf + af);
				}
				break;
		}
	}
	else
	{
		long a = 0;
		long b = 0;
		a = ((int_object*)tos)->value;
		b = ((int_object*)tos1)->value;
		new_tos = CreateIntObject(0,0);
		switch(op)
		{
			case OPCODE_INPLACE_MULTIPLY:
			case OPCODE_BINARY_MULTIPLY:
				{
					((int_object*)new_tos)->value = b * a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d * %d = %d\n", b, a, b * a);
				}
				break;	
			case OPCODE_INPLACE_OR:
			case OPCODE_BINARY_OR:
				{
					((int_object*)new_tos)->value = b | a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d | %d = %d\n", b, a, b | a);
				}
				break;
			case OPCODE_INPLACE_XOR:
			case OPCODE_BINARY_XOR:
				{
					((int_object*)new_tos)->value = b ^ a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d ^ %d = %d\n", b, a, b ^ a);
				}
				break;
			case OPCODE_INPLACE_AND:
			case OPCODE_BINARY_AND:
				{
					((int_object*)new_tos)->value = b & a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d & %d = %d\n", b, a, b & a);
				}
				break;
			case OPCODE_INPLACE_LSHIFT:
			case OPCODE_BINARY_LSHIFT:
				{
					((int_object*)new_tos)->value = b << a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d << %d = %d\n", b, a, b << a);
				}
				break;
			case OPCODE_INPLACE_RSHIFT:
			case OPCODE_BINARY_RSHIFT:
				{
					((int_object*)new_tos)->value = b >> a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d >> %d = %d\n", b, a, b >> a);
				}
				break;
			case OPCODE_INPLACE_MODULO:
			case OPCODE_BINARY_MODULO:
				{
					((int_object*)new_tos)->value = b % a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d %% %d = %d\n", b, a, b % a);
				}
				break;
			case OPCODE_INPLACE_FLOOR_DIVIDE:
			case OPCODE_BINARY_FLOOR_DIVIDE:
				{
					((int_object*)new_tos)->value = b / a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d / %d = %d\n", b, a, b / a);
				}
				break;
			case OPCODE_INPLACE_TRUE_DIVIDE:
			case OPCODE_BINARY_TRUE_DIVIDE:
				{
					((int_object*)new_tos)->value = b / a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d / %d = %d\n", b, a, b / a);
				}
				break;
			case OPCODE_INPLACE_SUBTRACT:
			case OPCODE_BINARY_SUBTRACT:
				{
					((int_object*)new_tos)->value = b-a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d - %d = %d\n", b, a, b - a);
				}
				break;
			case OPCODE_INPLACE_POWER:
			case OPCODE_BINARY_POWER:
				{
					((int_object*)new_tos)->value = long_pow(b, a);
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d ** %d = %d\n", b, a, long_pow(b, a));
				}
				break;
			case OPCODE_INPLACE_ADD:
			case OPCODE_BINARY_ADD:
				{
					((int_object*)new_tos)->value = b + a;
					if((debug_level & DEBUG_VERBOSE_STEP) > 0)
						printf ("%d + %d = %d\n", b, a, b + a);
				}
				break;
		}
	}
	return(new_tos);
}

object *custom_code(vm *vm,stack * stack)
{
	object *a = stack_Pop(stack,vm->garbage);
	object *b = stack_Pop(stack,vm->garbage);
	//if(a->type == TYPE_INT && b->type == TYPE_INT)
	//{
	//	int_object *r = CreateIntObject(((int_object*)a)->value+((int_object*)b)->value,0);
	//return (r);
	//}
	//object *tmp = CreateEmptyObject(TYPE_NONE,0);
	//printf("wrong args for custom_code\n");
	return(BinaryOp(a,b,OPCODE_BINARY_ADD));
	//return (tmp);
}


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
			PrintObject(tos);
	
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
