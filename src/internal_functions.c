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
	unicode_object *r = CreateUnicodeObject(tmp);
	return((object*)r);
}

object *StringCompare(object *a,object *b)
{
	char *as;
	char *bs;
	as = ((unicode_object *)a)->value;
	bs = ((unicode_object *)b)->value;
	object *r = CreateEmptyObject(!strcmp(as,bs) ? TYPE_TRUE : TYPE_FALSE);
	#ifdef USE_DEBUGGING
	debug_printf (DEBUG_VERBOSE_STEP,"%s == %s == %c\n", as, bs,r->type);
	#endif
	return(r);
}

object *BinaryOp(object *tos,object *tos1,unsigned char op)
{
	object *new_tos = NULL;
	if (tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE && op == OPCODE_BINARY_ADD) //string add -> returns string
	{
		return(StringAdd(tos1,tos));
	}
	
	if (tos->type == TYPE_INT && tos1->type == TYPE_TUPLE && op == OPCODE_BINARY_MULTIPLY) //tuple multiply -> returns tuple
	{
		NUM a =(NUM) ((int_object*)tos)->value;

		NUM mnum = ((tuple_object *) tos1)->list->num;
		if (mnum == 1)
			tos1 = ((tuple_object *) tos1)->list->items[0];

		tuple_object *mtr = CreateTuple(a);
		for (NUM i = 0; i < a; i++)
		{
			SetItem((object*)mtr,i,tos1);
		}
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_VERBOSE_STEP) > 0)
			DumpObject((object*)mtr,0);
		#endif
		return((object*)mtr);
	}
	if(tos->type == TYPE_BINARY_FLOAT || tos1->type == TYPE_BINARY_FLOAT) //mixed op -> returns float
	{
		FLOAT af = 0.0f;
		FLOAT bf = 0.0f;
		//printf("float ret\n");
		
		if (tos->type == TYPE_INT)
		{
			af = (FLOAT) ((int_object*)tos)->value;
		}	
		if (tos1->type == TYPE_INT)
		{
			bf = (FLOAT) ((int_object*)tos1)->value;
		}	
		if (tos->type == TYPE_BINARY_FLOAT)
		{
			af = ((float_object*)tos)->value;
		}	
		if (tos1->type == TYPE_BINARY_FLOAT)
		{
			bf = ((float_object*)tos1)->value;
		}	
		new_tos = (object*)CreateFloatObject(0);
		switch(op)
		{
			case OPCODE_INPLACE_MULTIPLY:
			case OPCODE_BINARY_MULTIPLY:
				{
					((float_object*)new_tos)->value = bf  * af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g * %7g = %7g\n", bf, af, bf  * af);
					#endif
				}
				break;
			case OPCODE_INPLACE_OR:
			case OPCODE_BINARY_OR:
				{
					((float_object*)new_tos)->value = (long)bf  | (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g | %7g = %7g\n", bf, af, (long)bf  | (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_XOR:
			case OPCODE_BINARY_XOR:
				{
					((float_object*)new_tos)->value = (long)bf  ^ (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g ^ %7g = %7g\n", bf, af, (long)bf  ^ (long)af);
					#endif
				}
				break;			
			case OPCODE_INPLACE_AND:
			case OPCODE_BINARY_AND:
				{
					((float_object*)new_tos)->value = (long)bf  & (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g & %7g = %7g\n", bf, af, (long)bf  & (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_LSHIFT:
			case OPCODE_BINARY_LSHIFT:
				{
					((float_object*)new_tos)->value = (long)bf << (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g << %7g = %7g\n", bf, af, (long)bf << (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_RSHIFT:
			case OPCODE_BINARY_RSHIFT:
				{
					((float_object*)new_tos)->value =  (long)bf >>  (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g>> %7g = %7g\n", bf, af,  (long)bf >>  (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_MODULO:
			case OPCODE_BINARY_MODULO:
				{
					((float_object*)new_tos)->value =  (long)bf %  (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g %% %7g = %7g\n", bf, af,  (long)bf %  (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_FLOOR_DIVIDE:
			case OPCODE_BINARY_FLOOR_DIVIDE:
				{
					((float_object*)new_tos)->value = floor(bf / af);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g // %7g = %7g\n", bf, af, floor(bf / af));
					#endif
				}
				break;
			case OPCODE_INPLACE_TRUE_DIVIDE:
			case OPCODE_BINARY_TRUE_DIVIDE:
				{
					((float_object*)new_tos)->value = bf / af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g / %7g = %7g\n", bf, af, bf / af);
					#endif
				}
				break;
			case OPCODE_INPLACE_SUBTRACT:
			case OPCODE_BINARY_SUBTRACT:
				{
					((float_object*)new_tos)->value = bf-af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g - %7g = %7g\n", bf, af, bf - af);
					#endif
				}
				break;
			case OPCODE_INPLACE_POWER:
			case OPCODE_BINARY_POWER:
				{
					((float_object*)new_tos)->value = long_pow(bf, af);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g ** %7g = %7g\n", bf, af, long_pow(bf, af));
					#endif
				}
				break;
			case OPCODE_INPLACE_ADD:
			case OPCODE_BINARY_ADD:
				{
					((float_object*)new_tos)->value = bf + af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g + %7g = %7g\n", bf, af, bf + af);
					#endif					
				}
				break;
		}
	}
	else	//int op -> returns int
	{
		INT a = 0;
		INT b = 0;
		a = ((int_object*)tos)->value;
		b = ((int_object*)tos1)->value;
		new_tos = (object*)CreateIntObject(0);
		switch(op)
		{
			case OPCODE_INPLACE_MULTIPLY:
			case OPCODE_BINARY_MULTIPLY:
				{
					((int_object*)new_tos)->value = b * a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d * %d = %d\n", b, a, b * a);
					#endif
				}
				break;	
			case OPCODE_INPLACE_OR:
			case OPCODE_BINARY_OR:
				{
					((int_object*)new_tos)->value = b | a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d | %d = %d\n", b, a, b | a);
					#endif
				}
				break;
			case OPCODE_INPLACE_XOR:
			case OPCODE_BINARY_XOR:
				{
					((int_object*)new_tos)->value = b ^ a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d ^ %d = %d\n", b, a, b ^ a);
					#endif
				}
				break;
			case OPCODE_INPLACE_AND:
			case OPCODE_BINARY_AND:
				{
					((int_object*)new_tos)->value = b & a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d & %d = %d\n", b, a, b & a);
					#endif
				}
				break;
			case OPCODE_INPLACE_LSHIFT:
			case OPCODE_BINARY_LSHIFT:
				{
					((int_object*)new_tos)->value = b << a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d << %d = %d\n", b, a, b << a);
					#endif
				}
				break;
			case OPCODE_INPLACE_RSHIFT:
			case OPCODE_BINARY_RSHIFT:
				{
					((int_object*)new_tos)->value = b >> a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d >> %d = %d\n", b, a, b >> a);
					#endif
				}
				break;
			case OPCODE_INPLACE_MODULO:
			case OPCODE_BINARY_MODULO:
				{
					((int_object*)new_tos)->value = b % a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d %% %d = %d\n", b, a, b % a);
					#endif
				}
				break;
			case OPCODE_INPLACE_FLOOR_DIVIDE:
			case OPCODE_BINARY_FLOOR_DIVIDE:
				{
					((int_object*)new_tos)->value = b / a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d / %d = %d\n", b, a, b / a);
					#endif
				}
				break;
			case OPCODE_INPLACE_TRUE_DIVIDE:
			case OPCODE_BINARY_TRUE_DIVIDE:
				{
					((int_object*)new_tos)->value = b / a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d / %d = %d\n", b, a, b / a);
					#endif
				}
				break;
			case OPCODE_INPLACE_SUBTRACT:
			case OPCODE_BINARY_SUBTRACT:
				{
					((int_object*)new_tos)->value = b-a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d - %d = %d\n", b, a, b - a);
					#endif
				}
				break;
			case OPCODE_INPLACE_POWER:
			case OPCODE_BINARY_POWER:
				{
					((int_object*)new_tos)->value = long_pow(b, a);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d ** %d = %d\n", b, a, long_pow(b, a));
					#endif
				}
				break;
			case OPCODE_INPLACE_ADD:
			case OPCODE_BINARY_ADD:
				{
					((int_object*)new_tos)->value = b + a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d + %d = %d\n", b, a, b + a);
					#endif
				}
				break;
		}
	}
	return(new_tos);
}

object *CompareOp(object *tos,object *tos1,unsigned char cmp_op)
{
	object *new_tos = NULL;
	if (tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE && cmp_op == 2) //string compare
	{
	return(StringCompare(tos,tos1));
	}
	if ((tos->type == TYPE_FALSE || tos->type == TYPE_TRUE) && (tos1->type == TYPE_FALSE || tos1->type == TYPE_TRUE)) //bool compare
	{
		new_tos = CreateEmptyObject(tos1->type == tos->type ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf (DEBUG_VERBOSE_STEP,"%c == %c == %c\n", tos1->type, tos->type, new_tos->type);
		#endif
		return(new_tos);
	}
	if(cmp_op == 8) // is (compare instances)
	{
		//new_tos = CreateEmptyObject(tos == tos1 ? TYPE_TRUE : TYPE_FALSE);
		new_tos = CreateEmptyObject(!object_compare(tos ,tos1) ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"%x is %x == %c\n",tos, tos1, new_tos->type);
		#endif
		return(new_tos);
	}
	if(cmp_op == 9) // is not (compare instances)
	{
		//new_tos = CreateEmptyObject(tos != tos1 ? TYPE_TRUE : TYPE_FALSE);
		new_tos = CreateEmptyObject(object_compare(tos ,tos1) ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"%x is not %x == %c\n",tos, tos1, new_tos->type);
		#endif
		return(new_tos);
		}
	if(cmp_op == 6) // in (contained in tuple)
	{
		new_tos = CreateEmptyObject( GetItemIndex(tos, tos1) != -1 ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"%x in %x == %c\n",tos, tos1, new_tos->type);
		#endif
		return(new_tos);
		}
	if(cmp_op == 7) // not int (not contained in tuple)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"index:%d\n",GetItemIndex(tos, tos1));
		#endif
		new_tos = CreateEmptyObject( GetItemIndex(tos, tos1) == -1 ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"%x not in %x == %c\n",tos, tos1, new_tos->type);
		#endif
		return(new_tos);
		}
	if(tos->type == TYPE_BINARY_FLOAT || tos1->type == TYPE_BINARY_FLOAT) //mixed objects
	{
		FLOAT af = 0.0f;
		FLOAT bf = 0.0f;
		
		if (tos->type == TYPE_INT)
		{
			af = (FLOAT) ((int_object*)tos)->value;
		}	
		if (tos1->type == TYPE_INT)
		{
			bf = (FLOAT) ((int_object*)tos1)->value;
		}	
		if (tos->type == TYPE_BINARY_FLOAT)
		{
			af = ((float_object*)tos)->value;
		}	
		if (tos1->type == TYPE_BINARY_FLOAT)
		{
			bf = ((float_object*)tos1)->value;
		}	
		switch (cmp_op)
		{
			case 0:	// <
				{
					new_tos = CreateEmptyObject(bf < af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g < %7g == %c\n", bf, af, new_tos->type);
					#endif
				}
				break;
			case 1:	// <=
				{
					new_tos = CreateEmptyObject(bf <= af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g <= %7g == %c\n", bf, af, new_tos->type);
					#endif
				}
				break;
			case 2:	// ==
				{
					new_tos = CreateEmptyObject(bf == af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g == %7g == %c\n", bf, af, new_tos->type);
					#endif
				}
				break;
			case 3:	// !=
				{
					new_tos = CreateEmptyObject(bf != af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g != %7g == %c\n", bf, af, new_tos->type);
					#endif
				}
				break;
			case 4:	// >
				{
					new_tos = CreateEmptyObject( bf > af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g < %7g == %c\n", bf, af, new_tos->type);
					#endif
				}
				break;
			case 5:	// >=
				{
					new_tos = CreateEmptyObject(bf >= af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g >= %7g == %c\n", bf, af, new_tos->type);
					#endif
				}
				break;
		}
	}
	else //int objects
	{
		INT a = 0;
		INT b = 0;
		a = ((int_object*)tos)->value;
		b = ((int_object*)tos1)->value;
		switch (cmp_op)
		{
			case 0:	// <
				{
					new_tos = CreateEmptyObject(b < a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d < %d == %c\n", b, a, new_tos->type);
					#endif
			}
			break;
			case 1:	// <=
				{
					new_tos = CreateEmptyObject(b <= a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d <= %d == %c\n", b, a, new_tos->type);
					#endif
			}
			break;
			case 2:	// ==
				{
					new_tos = CreateEmptyObject(b == a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d == %d == %c\n", b, a, new_tos->type);
					#endif
				}
				break;
			case 3:	// !=
				{
					new_tos = CreateEmptyObject(b != a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d != %d == %c\n", b, a, new_tos->type);
					#endif
			}
			break;
			case 4:	// >
				{
					new_tos = CreateEmptyObject( b > a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d < %d == %c\n", b, a, new_tos->type);
					#endif
				}
				break;
			case 5:	// >=
				{
					new_tos = CreateEmptyObject(b >= a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d >= %d == %c\n", b, a, new_tos->type);
					#endif
				}
				break;
		}
	}
	return(new_tos);
}

object *custom_code(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *a = GetItem((object*)locals,0);
	object *b = GetItem((object*)locals,1);
	return(BinaryOp(a,b,OPCODE_BINARY_ADD));
}

object *if_list(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	tuple_object *r = CreateTuple(0);
	INDEX i = 0;
	while(i < locals->list->num)
	{
		object *t = GetItem((object *)locals,i);
		if(t->type == TYPE_ITER)
			iter_ExpandTuple((iter_object*)t,vm,r);
		else
			AppendItem((object*)r,t);
		i++;
	}
	return ((object*)r);
}

object *if_next(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *iter = GetItem((object*)locals,0);
	object *next = iter_NextNow((iter_object*)iter,vm);
	return(next);
}

object *if_range(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	if (locals->list->num < 1)
	{
		object *tmp = CreateEmptyObject(TYPE_NONE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"not enough args for range\n");
		#endif
		return (tmp);
	}
	iter_object *iter = CreateIterObject();
	object *s = GetItem((object*)locals,0);
	object *e =  NULL;
	if (locals->list->num > 0)
		e = GetItem((object*)locals,1);
	object *st = NULL;
	if (locals->list->num > 1)
		st = GetItem((object*)locals,2);

	if (st == NULL && e == NULL && s->type == TYPE_INT)
	{
		iter_InitSequence(iter,0,((int_object*)s)->value,1);
	}
	else if (st == NULL && s->type == TYPE_INT && e->type == TYPE_INT)
	{
		iter_InitSequence(iter,((int_object*)s)->value,((int_object*)e)->value,1);
	}
	else if (st != NULL && s->type == TYPE_INT && e->type == TYPE_INT
			 && st->type == TYPE_INT)
	{
		iter_InitSequence(iter,((int_object*)s)->value,((int_object*)e)->value,((int_object*)st)->value);
	}
	return ((object*)iter);
}

object *if_print(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	BOOL printed_something = 0;
	INDEX i = 0; 
	while(i < locals->list->num)
	{
		object *t = GetItem((object *)locals,i);//TODO maybe move this into PrintObject() as case ITER:	
		if(t->type == TYPE_ITER)
		{
			//printf("TUPLE EXPAND in print\n");
			tuple_object *to = iter_TupleExpand((iter_object*)t,vm);
			gc_IncRefCount((object*)to);
			PrintObject((object*)to);
			gc_DecRefCount((object*)to);
			i++;
			continue;
		}
		if(t->type == TYPE_NONE)
		{
			i++;
			//gc_DecRefCount((object*)t);
			continue;
		}
		if (printed_something)
			printf(" ");
		if (t != NULL)
			PrintObject(t);
		printed_something = 1;
		i++;
	}
	if(printed_something)
		printf("\n");
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_sum(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	INT sum = 0;
	INDEX i = 0;
	while(i < locals->list->num)
	{
		object *t = GetItem((object *)locals,i);//TODO maybe move this into PrintObject() as case ITER:	
		if(t->type == TYPE_ITER)
		{
			tuple_object *to = iter_TupleExpand((iter_object*)t,vm);
			gc_IncRefCount((object*)to);
			object *r = if_sum(vm,to,NULL);
			switch (r->type)
			{
				case TYPE_INT:
					sum += ((int_object*)r)->value;
					break;
			}
			gc_IncRefCount((object*)r);
			gc_DecRefCount((object*)r);
			gc_DecRefCount((object*)to);
			i++;
			continue;
		}

		if (t != NULL)
			switch (t->type)
			{
				case TYPE_INT:
					sum += ((int_object*)t)->value;
					break;
			}
		i++;
	}
	int_object *tmp = CreateIntObject(sum);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"returning sum:%d\n",sum);
	#endif
	return ((object*)tmp);
}



object *if_max(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_abs(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_len(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_pow(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_min(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_sorted(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_reversed(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_all(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_any(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_chr(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_ord(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_cmp(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_hex(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_int(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_float(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_iter(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_map(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}




void AddInternalFunctions(struct _vm *vm)
{
	cfunction *list = CreateCFunction(&if_list, "list");
	cfunction *range = CreateCFunction(&if_range, "range");
	cfunction *print = CreateCFunction(&if_print, "print");
	cfunction *sum = CreateCFunction(&if_sum, "sum");
	cfunction *next = CreateCFunction(&if_next, "next");
	vm_AddFunction(vm, list);
	vm_AddFunction(vm, range);
	vm_AddFunction(vm, print);
	vm_AddFunction(vm, sum);
	vm_AddFunction(vm, next);

	cfunction *cc = CreateCFunction(&custom_code, "custom_code");
	vm_AddFunction(vm, cc);

}

