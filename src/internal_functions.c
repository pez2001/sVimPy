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

object *StringMultiply(object *a,object *b)
{
	char *as;
	NUM bi;
	as = ((unicode_object *)a)->value;
	bi = ((int_object *)b)->value;

	#ifdef USE_DEBUGGING
	char *tmp = (char*)mem_malloc((strlen(as)*bi) + 1, "str_Cat() return");
	#else
	char *tmp = (char*)malloc((strlen(as)*bi) + 1);
	#endif

	memset(tmp,0,(strlen(as)*bi) + 1);

	for(INDEX i = 0;i< bi;i++)
	{
		memcpy(tmp+(i*strlen(as)), as, strlen(as));
	}
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
	
	if (tos->type == TYPE_INT && tos1->type == TYPE_UNICODE && op == OPCODE_BINARY_MULTIPLY) //unicode multiply -> returns unicode
	{
		//printf("string multiply\n");
		return(StringMultiply(tos1,tos));
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
		//if (tos1->type == TYPE_INT)
		{
			af = (FLOAT) ((int_object*)tos)->value;
			//af = (FLOAT) ((int_object*)tos1)->value;
		}	
		if (tos1->type == TYPE_INT)
		//if (tos->type == TYPE_INT)
		{
			bf = (FLOAT) ((int_object*)tos1)->value;
			//bf = (FLOAT) ((int_object*)tos)->value;
		}	
		if (tos->type == TYPE_BINARY_FLOAT)
		//if (tos1->type == TYPE_BINARY_FLOAT)
		{
			af = ((float_object*)tos)->value;
			//af = ((float_object*)tos1)->value;
		}	
		if (tos1->type == TYPE_BINARY_FLOAT)
		//if (tos->type == TYPE_BINARY_FLOAT)
		{
			bf = ((float_object*)tos1)->value;
			//bf = ((float_object*)tos)->value;
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
		//a = ((int_object*)tos1)->value;
		//b = ((int_object*)tos)->value;
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
		new_tos = CreateEmptyObject(!CompareObjects(tos ,tos1) ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"%x is %x == %c\n",tos, tos1, new_tos->type);
		#endif
		return(new_tos);
	}
	if(cmp_op == 9) // is not (compare instances)
	{
		//new_tos = CreateEmptyObject(tos != tos1 ? TYPE_TRUE : TYPE_FALSE);
		new_tos = CreateEmptyObject(CompareObjects(tos ,tos1) ? TYPE_TRUE : TYPE_FALSE);
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

/*
object *if_assert(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *bool = GetItem((object*)locals,0);
	unicode_object *message = (unicode_object*)GetItem((object*)locals,1);
	if(bool->type == TYPE_FALSE)
	{
		//printf("Assertion failed:\n");
		PrintObject((object*)message);
		vm_Interrupt(vm,NULL);
	}
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}
*/
object *if_build_class(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	//printf("build_class called\n");
	//DumpObject(locals,0);

	function_object *class_function_object = (function_object*)GetItem((object*)locals,0);
	unicode_object *class_name = (unicode_object*)GetItem((object*)locals,1);
	class_object *class = CreateClassObject(class_function_object->func,NULL);

	if(locals->list->num> 2)
	{
		class->base_classes = (object*)CreateTuple(locals->list->num - 2);
		gc_IncRefCount(class->base_classes);
		for(INDEX i = 0;i<locals->list->num-2;i++)
		{
			object *bc = GetItem((object*)locals,2+i);
			SetItem(class->base_classes,i, bc);
		}
	}
	else
		class->base_classes = NULL;
	
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy(class_name->value)),(object*)class);
	//DumpObject(vm->globals,0);
	//printf("returning class\n");
	//FullDumpObject(class,0);
	return((object*)class);
	//object *tmp =CreateEmptyObject(TYPE_NONE);
	//return (tmp);
}

#ifdef USE_INTERNAL_CLASSES
object *if_open(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	//if(x->type == TYPE_UNICODE)
	//	printf("opening file:%s\n",((unicode_object*)x)->value);
	
	unicode_object *file_class_key = CreateUnicodeObject(str_Copy("file_class"));
	class_object *file_class = (class_object*)vm_GetGlobal(vm,(object*)file_class_key);
	gc_IncRefCount((object*)file_class_key);
	gc_DecRefCount((object*)file_class_key);
	class_instance_object *file_instance = CreateClassInstanceObject(file_class);	
	//#ifndef USE_ARDUINO_FUNCTIONS
	object *x = GetItem((object*)locals,0);
	stream *fs = stream_CreateFromFile(((unicode_object*)x)->value,"rb");
	stream_Open(fs);
	tag_object *file_tag = CreateTagObject(fs);
	unicode_object *file_name = CreateUnicodeObject(str_Copy("__file__"));
	SetAttribute((object*)file_instance,(object*)file_name,(object*)file_tag);	
	//#endif
	return((object*)file_instance);
		
	//object *tmp =CreateEmptyObject(TYPE_NONE);
	//return (tmp);
}
#endif

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
	iter_object *iter  = iter_CreateIter(GetItem((object *)locals,0),vm);
	//object *iter = GetItem((object*)locals,0);
	gc_IncRefCount((object*)iter);
	object *next = iter_NextNow((iter_object*)iter,vm);
	gc_DecRefCount((object*)iter);
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
	//FullDumpObject(locals,0);
	iter_object *iter = CreateIterObject();
	object *s = GetItem((object*)locals,0);
	object *e =  obj_NULL;
	if (locals->list->num > 0)
		e = GetItem((object*)locals,1);
	object *st = obj_NULL;
	if (locals->list->num > 1)
		st = GetItem((object*)locals,2);

	//DumpObject(s,0);
	//DumpObject(st,0);
	//DumpObject(e,0);
		
	if (st == obj_NULL && e == obj_NULL && s->type == TYPE_INT)
	{
		iter_InitSequence(iter,vm,0,((int_object*)s)->value,1);//zero to s by 1
	}
	else if (st == obj_NULL && s->type == TYPE_INT && e->type == TYPE_INT)
	{
		iter_InitSequence(iter,vm,((int_object*)s)->value,((int_object*)e)->value,1);
	}
	else if (s->type == TYPE_INT && e->type == TYPE_INT && st->type == TYPE_INT)
	{
		iter_InitSequence(iter,vm,((int_object*)s)->value,((int_object*)e)->value,((int_object*)st)->value);
	}
	return ((object*)iter);
}
#ifndef USE_ARDUINO_FUNCTIONS
object *if_print(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	BOOL printed_something = 0;
	INDEX i = 0; 
	unicode_object *end = CreateUnicodeObject(str_Copy("end"));
	gc_IncRefCount((object*)end);
	object *endval = GetDictItem((object*)kw_locals,(object*)end);
	gc_DecRefCount((object*)end);
	while(i < locals->list->num)
	{
		object *t = GetItem((object *)locals,i);//TODO maybe move this into PrintObject() as case ITER:	
		if(t->type == TYPE_ITER)
		{
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
			continue;
		}
		if (printed_something)
			printf(" ");
		if (t != NULL)
			PrintObject(t);
		printed_something = 1;
		i++;
	}
	if(printed_something && endval!=NULL && endval->type == TYPE_UNICODE)
		printf("%s",((unicode_object*)endval)->value);
	else
	if(printed_something)
		printf("\n");
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}
#endif

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

/*
object *if_max(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *max = NULL;
	INDEX i = 0;
	object *t = GetItem((object *)locals,0);	
	if(t->type == TYPE_ITER)
	{
		tuple_object *to = iter_TupleExpand((iter_object*)t,vm);
		gc_IncRefCount((object*)to);
		object *r = if_max(vm,to,NULL);
		gc_DecRefCount((object*)to);
		return(r);
	}

	while(i < locals->list->num)
	{
		object *t = GetItem((object *)locals,i);//TODO maybe move this into PrintObject() as case ITER:	

		if (t != NULL)
			switch (t->type)
			{
				case TYPE_INT:
					if(max == NULL)
						max = t;
					else
					if(((int_object*)t)->value > ((int_object*)max)->value)
						max = t;//((int_object*)t)->value;
					break;
			}
		i++;
	}
	if(max == NULL)
	{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
	}
	return (max);
}

object *if_abs(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *a = GetItem((object*)locals,0);
	switch(a->type)
	{
		case TYPE_INT:
			{
				int_object *tmp = CreateIntObject(abs(((int_object*)a)->value));
				return ((object*)tmp);
			}
		case TYPE_BINARY_FLOAT:
			{
				float_object *tmp = CreateFloatObject(abs(((float_object*)a)->value));
				return ((object*)tmp);
			}
	}
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_len(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	if(locals->list->num != 1)
	{
		object *tmp = CreateEmptyObject(TYPE_NONE);
		return (tmp);
	}
	int_object *len = CreateIntObject(GetTupleLen(GetItem((object *)locals,0)));
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"returning len:%d\n",len);
	#endif
	return ((object*)len);
}

object *if_pow(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	if (locals->list->num < 2)
	{
		object *tmp = CreateEmptyObject(TYPE_NONE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"not enough args for pow\n");
		#endif
		return (tmp);
	}
	object *x = GetItem((object*)locals,0);
	object *y = GetItem((object*)locals,1);
	object *z =  NULL;
	if (locals->list->num > 2)
		z = GetItem((object*)locals,2);
	if (z == NULL && x->type == TYPE_INT && y->type == TYPE_INT)
	{	
		object *r = BinaryOp(x,y,OPCODE_BINARY_POWER);
		return(r);
	}
	else
	if (z != NULL && x->type == TYPE_INT && y->type == TYPE_INT && z->type == TYPE_INT)
	{
		object *r = BinaryOp(x,y,OPCODE_BINARY_POWER);
		object *rm = BinaryOp(r,z,OPCODE_BINARY_MODULO);
		gc_IncRefCount(r);
		gc_DecRefCount(r);
		return(rm);
	}
	//return ((object*)iter);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_min(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *min = NULL;
	INDEX i = 0;
	object *t = GetItem((object *)locals,0);	
	if(t->type == TYPE_ITER)
	{
		tuple_object *to = iter_TupleExpand((iter_object*)t,vm);
		gc_IncRefCount((object*)to);
		object *r = if_min(vm,to,NULL);
		gc_DecRefCount((object*)to);
		return(r);
	}

	while(i < locals->list->num)
	{
		object *t = GetItem((object *)locals,i);	

		if (t != NULL)
			switch (t->type)
			{
				case TYPE_INT:
					if(min == NULL)
						min = t;
					else
					if(((int_object*)t)->value < ((int_object*)min)->value)
						min = t;//((int_object*)t)->value;
					break;
			}
		i++;
	}
	if(min == NULL)
	{
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
	}
	return (min);
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
	iter_object *iter = iter_CreateIter(GetItem((object *)locals,0),vm);
	gc_IncRefCount((object*)iter);
	object *r = CreateEmptyObject(TYPE_TRUE);
	object *n = NULL;
	do
	{
		n = iter_NextNow(iter,vm);
		if(n->type != TYPE_TRUE)
		 r->type = TYPE_FALSE;
	}while(n->type != TYPE_NONE);
	gc_DecRefCount((object*)iter);
	return(r);
}

object *if_any(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	iter_object *iter  = iter_CreateIter(GetItem((object *)locals,0),vm);
	gc_IncRefCount((object*)iter);
	object *r = CreateEmptyObject(TYPE_FALSE);
	object *n = NULL;
	do
	{
		n = iter_NextNow(iter,vm);
		if(n->type == TYPE_TRUE)
		 r->type = TYPE_TRUE;
	}while(n->type != TYPE_NONE);
	gc_DecRefCount((object*)iter);
	return(r);
}

object *if_chr(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *a = GetItem((object*)locals,0);
	if(a->type == TYPE_INT)
	{
		char *chr = str_FromChar(((int_object*)a)->value);
		unicode_object *r = CreateUnicodeObject(chr);
		#ifdef USE_DEBUGGING
		assert(mem_free(chr));
		#else
		free(chr);
		#endif
		return((object*)r);
	}
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}

object *if_ord(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	object *a = GetItem((object*)locals,0);
	if(a->type == TYPE_UNICODE && strlen(((unicode_object*)a)->value) > 0)
	{	
		int_object *r = CreateIntObject(((unicode_object*)a)->value[0]);
		return((object*)r);
	}
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
	object *a = GetItem((object*)locals,0);
	if(a->type == TYPE_INT)
	{
		NUM chars = 0;
		for(INDEX i = 0;i<4;i++) // calculate space requirements
		{
			if((((int_object*)a)->value >> (i*8) & 255) > 0 )
				chars = i;
		}
		#ifdef USE_DEBUGGING
		char *tmp = (char *)mem_malloc(chars*2 + 1, "if_hex(); tmp");
		#else
		char *tmp = (char *)malloc(chars*2 + 1);
		#endif
		memset(tmp,0,chars*2 + 1);
		sprintf(tmp,"%X",((int_object*)a)->value);
		
		unicode_object *r = CreateUnicodeObject(tmp);
		#ifdef USE_DEBUGGING
		assert(mem_free(tmp));
		#else
		free(tmp);
		#endif
		return((object*)r);
	}
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
*/
object *if_iter(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
	if(locals->list->num == 2)
	{//uses a sentinel
		//TODO will not work
		iter_object *iter  = iter_CreateIter(GetItem((object *)locals,0),vm);
		return((object*)iter);
	}
	else
	if(locals->list->num == 2)
	{
		iter_object *iter  = iter_CreateIter(GetItem((object *)locals,0),vm);
		return((object*)iter);
	}

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}
/*
object *if_map(struct _vm *vm,tuple_object *locals,tuple_object *kw_locals)
{

	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);
}
*/

#ifndef USE_ARDUINO_FUNCTIONS


object *a_pinMode(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin,mode
	object *pin = GetItem((object*)locals,0);
	object *mode = GetItem((object*)locals,1);
	//printf("pinMode\n");
	//TODO only a temp solution
	if(pin->type == TYPE_KV)
		pin = (object*)((kv_object*)pin)->value;
	if(mode->type == TYPE_KV)
		mode = (object*)((kv_object*)mode)->value;
	if(pin->type == TYPE_INT && mode->type == TYPE_INT)
		printf("pinMode: %d to %d\n",((int_object*)pin)->value,((int_object*)mode)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *a_digitalRead(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin
	//printf("digitalRead\n");
	object *pin = GetItem((object*)locals,0);
	int r = 0;
	if(pin->type == TYPE_INT)
		printf("digitalRead: %d\n",((int_object*)pin)->value);
	int_object *tmp = CreateIntObject(r);
	return ((object*)tmp);	
	
}

object *a_digitalWrite(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin,value
	if(locals->list->num < 2)
	{
		//printf("not enough args for digitalWrite();\r\n");
		object *tmp =CreateEmptyObject(TYPE_NONE);
		return (tmp);	
	}
	object *pin = GetItem((object*)locals,0);
	object *val = GetItem((object*)locals,1);
    //PrintObject(pin);
	//printf("\r\nval:");
	//PrintObject(val);
	//printf("\r\n");
	//printf("pt:%c,vt:%c\r\n",pin->type,val->type);
	//printf("digitalWrite\n");
	//TODO only a temp solution
	if(pin->type == TYPE_KV)
		pin =(object*) ((kv_object*)pin)->value;
	if(val->type == TYPE_KV)
	{
		val = (object*) ((kv_object*)val)->value;
		//printf("pt:%c,vt:%c\r\n",pin->type,val->type);
		//printf("val:%d\r\n",((int_object*)val)->value);
	}
	if((pin->type == TYPE_INT) && (val->type == TYPE_INT))
	{
		printf("digitalWrite pin: %d val: %d\n",((int_object*)pin)->value,((int_object*)val)->value);
	}
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *a_analogRead(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin
 	//printf("analogRead\n");
 	object *pin = GetItem((object*)locals,0);
	int r = 0;
	if(pin->type == TYPE_INT)
		printf("analogRead: %d\n",((int_object*)pin)->value);
	int_object *tmp =CreateIntObject(r);
	return ((object*)tmp);	
}

object *a_analogWrite(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //pin,value
 	//printf("analogWrite\n");
	object *pin = GetItem((object*)locals,0);
	object *val = GetItem((object*)locals,1);
	if(pin->type == TYPE_KV)
		pin = (object*)((kv_object*)pin)->value;
	if(val->type == TYPE_KV)
		val = (object*)((kv_object*)val)->value;
	//DumpObject(pin,0);
	//DumpObject(val,0);
	if(pin->type == TYPE_INT && val->type == TYPE_INT)
		printf("analogWrite pin: %d val: %d\n",((int_object*)pin)->value,((int_object*)val)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *a_delay(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //ms
 	//printf("delay\n");
 	object *ms = GetItem((object*)locals,0);
	//if(ms->type == TYPE_INT)
	printf("delay: %d\n",((int_object*)ms)->value);
	//delay(((int_object*)ms)->value);
	//delay(8);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}
/*
object *a_serialprint(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //message
 	//printf("serial.Println\n");
 	object *message = GetItem((object*)locals,0);
	//if(message->type == TYPE_UNICODE)
	//	Serial.print(((unicode_object*)message)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

object *a_serialBegin(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //baudrate
 	//printf("serial.Begin\n");
 	object *baudrate = GetItem((object*)locals,0);
	//if(baudrate->type == TYPE_INT)
	//	Serial.begin(((int_object*)baudrate)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}
*/
void AddArduinoGlobal(vm *vm)
{
	code_object *a_globals = CreateCodeObject(str_Copy("arduino"));
	AddCodeCFunction((object*)a_globals,"pinMode",&a_pinMode);
	AddCodeCFunction((object*)a_globals,"digitalRead",&a_digitalRead);
	AddCodeCFunction((object*)a_globals,"digitalWrite",&a_digitalWrite);
	AddCodeCFunction((object*)a_globals,"analogRead",&a_analogRead);
	AddCodeCFunction((object*)a_globals,"analogWrite",&a_analogWrite);
	AddCodeCFunction((object*)a_globals,"delay",&a_delay);
	//AddCodeCFunction((object*)a_globals,"Serial.print",&a_serialprint);
	//AddCodeCFunction((object*)a_globals,"Serial.Begin",&a_serialBegin);
	AddCodeName((object*)a_globals,(object*)CreateUnicodeObject(str_Copy("INPUT")),(object*)CreateIntObject(0));
	AddCodeName((object*)a_globals,(object*)CreateUnicodeObject(str_Copy("OUTPUT")),(object*)CreateIntObject(1));
	AddCodeName((object*)a_globals,(object*)CreateUnicodeObject(str_Copy("LOW")),(object*)CreateIntObject(0));
	AddCodeName((object*)a_globals,(object*)CreateUnicodeObject(str_Copy("HIGH")),(object*)CreateIntObject(1));
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("arduino")),(object*)a_globals);
}

#endif



void AddInternalFunctions(struct _vm *vm)
{
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("list")),(object*)CreateCFunctionObject(&if_list));
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("range")),(object*)CreateCFunctionObject(&if_range));
	#ifndef USE_ARDUINO_FUNCTIONS
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("print")),(object*)CreateCFunctionObject(&if_print));
	#endif
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("sum")),(object*)CreateCFunctionObject(&if_sum));
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("next")),(object*)CreateCFunctionObject(&if_next));
	#ifdef USE_INTERNAL_CLASSES
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("open")),(object*)CreateCFunctionObject(&if_open));
	#endif
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("iter")),(object*)CreateCFunctionObject(&if_iter));
	vm_AddGlobal(vm,(object*)CreateUnicodeObject(str_Copy("custom_code")),(object*)CreateCFunctionObject(&custom_code));
}

