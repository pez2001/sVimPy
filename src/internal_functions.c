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

OBJECT_ID StringAdd(OBJECT_ID a_id,OBJECT_ID b_id)
{
	unicode_object *a = (unicode_object*)mem_lock(a_id);
	unicode_object *b = (unicode_object*)mem_lock(b_id);
	BYTES_ID tmp = str_Cat(a->value,b->value);
	mem_unlock(b_id,0);
	mem_unlock(a_id,0);
	UNICODE_ID r = obj_CreateUnicode(tmp);
	return(r);
}

OBJECT_ID StringMultiply(OBJECT_ID a_id,OBJECT_ID b_id)//TODO check string alloc and leaks
{
	NUM bi;
	int_object *b = (int_object*)mem_lock(b_id);
	unicode_object *a = (unicode_object*)mem_lock(a_id);
	BYTES_ID as_id = a->value;
	bi = b->value;
	mem_unlock(a_id,0);
	mem_unlock(b_id,0);

	char *as = (char*)mem_lock(as_id);
	
	#ifdef USE_MEMORY_DEBUGGING
	BYTES_ID tmp_id = mem_malloc_debug((strlen(as)*bi) + 1,MEM_POOL_CLASS_DYNAMIC, "str_Multiply() return");
	#else
	BYTES_ID tmp_id = mem_malloc((strlen(as)*bi) + 1,MEM_POOL_CLASS_DYNAMIC);
	#endif
	char *tmp = (char*)mem_lock(tmp_id);
	
	memset(tmp,0,(strlen(as)*bi) + 1);

	for(INDEX i = 0;i< bi;i++)
	{
		memcpy(tmp+(i*strlen(as)), as, strlen(as));
	}
	OBJECT_ID r = obj_CreateUnicode(tmp_id);//TODO mem_create_string_copy
	mem_unlock(tmp_id,1);
	mem_unlock(as_id,0);
	return(r);
}

OBJECT_ID StringCompare(OBJECT_ID a_id,OBJECT_ID b_id)
{
	unicode_object *a = (unicode_object*)mem_lock(a_id);
	unicode_object *b = (unicode_object*)mem_lock(b_id);
	
	OBJECT_ID as = a->value;
	OBJECT_ID bs = b->value;
	OBJECT_ID r = obj_CreateEmpty(mem_compare(as,bs) ? TYPE_TRUE : TYPE_FALSE);
	mem_unlock(b_id,0);
	mem_unlock(a_id,0);
	//#ifdef USE_DEBUGGING
	//debug_printf (DEBUG_VERBOSE_STEP,"%s == %s == %c\n", as, bs,obj_GetType(r));
	//#endif
	return(r);
}

OBJECT_ID BinaryOp(OBJECT_ID tos_id,OBJECT_ID tos1_id,unsigned char op)
{
	object *tos = (object*)mem_lock(tos_id);
	object *tos1 = (object*)mem_lock(tos1_id);
	OBJECT_ID new_tos_id = 0;
	if(tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE && op == OPCODE_BINARY_ADD) //string add -> returns string
	{
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		return(StringAdd(tos1_id,tos_id));
	}
	
	if(tos->type == TYPE_INT && tos1->type == TYPE_UNICODE && op == OPCODE_BINARY_MULTIPLY) //unicode multiply -> returns unicode
	{
		//printf("string multiply\n");
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		return(StringMultiply(tos1_id,tos_id));
	}
	if(tos->type == TYPE_INT && tos1->type == TYPE_TUPLE && op == OPCODE_BINARY_MULTIPLY) //tuple multiply -> returns tuple
	{
		NUM a =(NUM) ((int_object*)tos)->value;

		NUM mnum = tuple_GetLen(tos1_id);
		if(mnum == 1)
		{	
			OBJECT_ID old = tos1_id;
			tos1_id = tuple_GetItem(tos1_id,0);
			mem_unlock(old,0);//TODO what if tos1_id == old .... ???????
			tos1 = (object*)mem_lock(tos1_id);
		}
		TUPLE_ID mtr = obj_CreateTuple(a);
		for(NUM i = 0; i < a; i++)
		{
			tuple_SetItem(mtr,i,tos1_id);
		}
		#ifdef USE_DEBUGGING
		if((debug_level & DEBUG_VERBOSE_STEP) > 0)
			obj_Dump(mtr,0,1);
		#endif
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		return(mtr);
	}
	if(tos->type == TYPE_BINARY_FLOAT || tos1->type == TYPE_BINARY_FLOAT) //mixed op -> returns float
	{
		FLOAT af = 0.0f;
		FLOAT bf = 0.0f;
		//printf("float ret\n");
		
		if(tos->type == TYPE_INT)
		//if (tos1->type == TYPE_INT)
		{
			af = (FLOAT) ((int_object*)tos)->value;
			//af = (FLOAT) ((int_object*)tos1)->value;
		}	
		if(tos1->type == TYPE_INT)
		//if (tos->type == TYPE_INT)
		{
			bf = (FLOAT) ((int_object*)tos1)->value;
			//bf = (FLOAT) ((int_object*)tos)->value;
		}	
		if(tos->type == TYPE_BINARY_FLOAT)
		//if (tos1->type == TYPE_BINARY_FLOAT)
		{
			af = ((float_object*)tos)->value;
			//af = ((float_object*)tos1)->value;
		}	
		if(tos1->type == TYPE_BINARY_FLOAT)
		//if (tos->type == TYPE_BINARY_FLOAT)
		{
			bf = ((float_object*)tos1)->value;
			//bf = ((float_object*)tos)->value;
		}	
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		new_tos_id = obj_CreateFloat(0);
		float_object *new_tos = (float_object*)mem_lock(new_tos_id);
		switch(op)
		{
			case OPCODE_INPLACE_MULTIPLY:
			case OPCODE_BINARY_MULTIPLY:
				{
					new_tos->value = bf  * af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g * %7g = %7g\n", bf, af, bf  * af);
					#endif
				}
				break;
			case OPCODE_INPLACE_OR:
			case OPCODE_BINARY_OR:
				{
					new_tos->value = (long)bf  | (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g | %7g = %7g\n", bf, af, (long)bf  | (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_XOR:
			case OPCODE_BINARY_XOR:
				{
					new_tos->value = (long)bf  ^ (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g ^ %7g = %7g\n", bf, af, (long)bf  ^ (long)af);
					#endif
				}
				break;			
			case OPCODE_INPLACE_AND:
			case OPCODE_BINARY_AND:
				{
					new_tos->value = (long)bf  & (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g & %7g = %7g\n", bf, af, (long)bf  & (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_LSHIFT:
			case OPCODE_BINARY_LSHIFT:
				{
					new_tos->value = (long)bf << (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g << %7g = %7g\n", bf, af, (long)bf << (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_RSHIFT:
			case OPCODE_BINARY_RSHIFT:
				{
					new_tos->value =  (long)bf >>  (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g>> %7g = %7g\n", bf, af,  (long)bf >>  (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_MODULO:
			case OPCODE_BINARY_MODULO:
				{
					new_tos->value =  (long)bf %  (long)af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g %% %7g = %7g\n", bf, af,  (long)bf %  (long)af);
					#endif
				}
				break;
			case OPCODE_INPLACE_FLOOR_DIVIDE:
			case OPCODE_BINARY_FLOOR_DIVIDE:
				{
					new_tos->value = floor(bf / af);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g // %7g = %7g\n", bf, af, floor(bf / af));
					#endif
				}
				break;
			case OPCODE_INPLACE_TRUE_DIVIDE:
			case OPCODE_BINARY_TRUE_DIVIDE:
				{
					new_tos->value = bf / af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g / %7g = %7g\n", bf, af, bf / af);
					#endif
				}
				break;
			case OPCODE_INPLACE_SUBTRACT:
			case OPCODE_BINARY_SUBTRACT:
				{
					new_tos->value = bf-af;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g - %7g = %7g\n", bf, af, bf - af);
					#endif
				}
				break;
			case OPCODE_INPLACE_POWER:
			case OPCODE_BINARY_POWER:
				{
					new_tos->value = num_pow(bf, af); //TODO this will most likely not work correctly
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g ** %7g = %7g\n", bf, af, num_pow(bf, af));
					#endif
				}
				break;
			case OPCODE_INPLACE_ADD:
			case OPCODE_BINARY_ADD:
				{
					new_tos->value = bf + af;
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
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		//a = ((int_object*)tos1)->value;
		//b = ((int_object*)tos)->value;
		new_tos_id = obj_CreateInt(0);
		int_object *new_tos = (int_object*)mem_lock(new_tos_id);
		switch(op)
		{
			case OPCODE_INPLACE_MULTIPLY:
			case OPCODE_BINARY_MULTIPLY:
				{
					new_tos->value = b * a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d * %d = %d\n", b, a, b * a);
					#endif
				}
				break;	
			case OPCODE_INPLACE_OR:
			case OPCODE_BINARY_OR:
				{
					new_tos->value = b | a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d | %d = %d\n", b, a, b | a);
					#endif
				}
				break;
			case OPCODE_INPLACE_XOR:
			case OPCODE_BINARY_XOR:
				{
					new_tos->value = b ^ a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d ^ %d = %d\n", b, a, b ^ a);
					#endif
				}
				break;
			case OPCODE_INPLACE_AND:
			case OPCODE_BINARY_AND:
				{
					new_tos->value = b & a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d & %d = %d\n", b, a, b & a);
					#endif
				}
				break;
			case OPCODE_INPLACE_LSHIFT:
			case OPCODE_BINARY_LSHIFT:
				{
					new_tos->value = b << a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d << %d = %d\n", b, a, b << a);
					#endif
				}
				break;
			case OPCODE_INPLACE_RSHIFT:
			case OPCODE_BINARY_RSHIFT:
				{
					new_tos->value = b >> a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d >> %d = %d\n", b, a, b >> a);
					#endif
				}
				break;
			case OPCODE_INPLACE_MODULO:
			case OPCODE_BINARY_MODULO:
				{
					if(a == 0)
						new_tos->value = 0;
					else
						new_tos->value = b % a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d %% %d = %d\n", b, a,((int_object*)new_tos)->value);
					#endif
				}
				break;
			case OPCODE_INPLACE_FLOOR_DIVIDE:
			case OPCODE_BINARY_FLOOR_DIVIDE:
				{
					new_tos->value = b / a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d / %d = %d\n", b, a, b / a);
					#endif
				}
				break;
			case OPCODE_INPLACE_TRUE_DIVIDE:
			case OPCODE_BINARY_TRUE_DIVIDE:
				{
					new_tos->value = b / a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d / %d = %d\n", b, a, b / a);
					#endif
				}
				break;
			case OPCODE_INPLACE_SUBTRACT:
			case OPCODE_BINARY_SUBTRACT:
				{
					new_tos->value = b-a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d - %d = %d\n", b, a, b - a);
					#endif
				}
				break;
			case OPCODE_INPLACE_POWER:
			case OPCODE_BINARY_POWER:
				{
					new_tos->value = num_pow(b, a);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d ** %d = %d\n", b, a, ((int_object*)new_tos)->value);
					#endif
				}
				break;
			case OPCODE_INPLACE_ADD:
			case OPCODE_BINARY_ADD:
				{
					new_tos->value = b + a;
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d + %d = %d\n", b, a, b + a);
					#endif
				}
				break;
		}
	}
	mem_unlock(new_tos_id,1);
	return(new_tos_id);
}

OBJECT_ID CompareOp(OBJECT_ID tos_id,OBJECT_ID tos1_id,unsigned char cmp_op)
{
	OBJECT_ID new_tos_id = 0;
	object *tos = (object*)mem_lock(tos_id);
	object *tos1 = (object*)mem_lock(tos1_id);
	if(tos->type == TYPE_UNICODE && tos1->type == TYPE_UNICODE && cmp_op == 2) //string compare
	{
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		return(StringCompare(tos_id,tos1_id));
	}
	if((tos->type == TYPE_FALSE || tos->type == TYPE_TRUE) && (tos1->type == TYPE_FALSE || tos1->type == TYPE_TRUE)) //bool compare
	{
		new_tos_id = obj_CreateEmpty(tos1->type == tos->type ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf (DEBUG_VERBOSE_STEP,"%c == %c == %c\n", tos1->type, tos->type, obj_GetType(new_tos_id));
		#endif
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		return(new_tos_id);
	}
	if(cmp_op == 8) // is (compare instances)
	{
		//new_tos = CreateEmptyObject(tos == tos1 ? TYPE_TRUE : TYPE_FALSE);
		new_tos_id = obj_CreateEmpty(!obj_Compare(tos_id ,tos1_id) ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"%d is %d == %c\n",tos_id, tos1_id, obj_GetType(new_tos_id));
		#endif
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		return(new_tos_id);
	}
	if(cmp_op == 9) // is not (compare instances)
	{
		//new_tos = CreateEmptyObject(tos != tos1 ? TYPE_TRUE : TYPE_FALSE);
		new_tos_id = obj_CreateEmpty(obj_Compare(tos_id ,tos1_id) ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"%d is not %d == %c\n",tos_id, tos1_id, obj_GetType(new_tos_id));
		#endif
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		return(new_tos_id);
		}
	if(cmp_op == 6) // in (contained in tuple)
	{
		new_tos_id = obj_CreateEmpty( tuple_GetItemIndex(tos_id, tos1_id) != -1 ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"%d in %d == %c\n",tos_id, tos1_id, obj_GetType(new_tos_id));
		#endif
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		return(new_tos_id);
		}
	if(cmp_op == 7) // not int (not contained in tuple)
	{
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"index:%d\n",tuple_GetItemIndex(tos_id, tos1_id));
		#endif
		new_tos_id = obj_CreateEmpty( tuple_GetItemIndex(tos_id, tos1_id) == -1 ? TYPE_TRUE : TYPE_FALSE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_VERBOSE_STEP,"%d not in %d == %c\n",tos_id, tos1_id, obj_GetType(new_tos_id));
		#endif
		mem_unlock(tos1_id,0);
		mem_unlock(tos_id,0);
		return(new_tos_id);
		}
	if(tos->type == TYPE_BINARY_FLOAT || tos1->type == TYPE_BINARY_FLOAT) //mixed objects
	{
		FLOAT af = 0.0f;
		FLOAT bf = 0.0f;
		
		if(tos->type == TYPE_INT)
		{
			af = (FLOAT) ((int_object*)tos)->value;
		}	
		if(tos1->type == TYPE_INT)
		{
			bf = (FLOAT) ((int_object*)tos1)->value;
		}	
		if(tos->type == TYPE_BINARY_FLOAT)
		{
			af = ((float_object*)tos)->value;
		}	
		if(tos1->type == TYPE_BINARY_FLOAT)
		{
			bf = ((float_object*)tos1)->value;
		}	
		switch (cmp_op)
		{
			case 0:	// <
				{
					new_tos_id = obj_CreateEmpty(bf < af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g < %7g == %c\n", bf, af, obj_GetType(new_tos_id));
					#endif
				}
				break;
			case 1:	// <=
				{
					new_tos_id = obj_CreateEmpty(bf <= af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g <= %7g == %c\n", bf, af, obj_GetType(new_tos_id));
					#endif
				}
				break;
			case 2:	// ==
				{
					new_tos_id = obj_CreateEmpty(bf == af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g == %7g == %c\n", bf, af, obj_GetType(new_tos_id));
					#endif
				}
				break;
			case 3:	// !=
				{
					new_tos_id = obj_CreateEmpty(bf != af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g != %7g == %c\n", bf, af, obj_GetType(new_tos_id));
					#endif
				}
				break;
			case 4:	// >
				{
					new_tos_id = obj_CreateEmpty( bf > af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g < %7g == %c\n", bf, af, obj_GetType(new_tos_id));
					#endif
				}
				break;
			case 5:	// >=
				{
					new_tos_id = obj_CreateEmpty(bf >= af ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%7g >= %7g == %c\n", bf, af, obj_GetType(new_tos_id));
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
					new_tos_id = obj_CreateEmpty(b < a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d < %d == %c\n", b, a, obj_GetType(new_tos_id));
					#endif
			}
			break;
			case 1:	// <=
				{
					new_tos_id =  obj_CreateEmpty(b <= a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d <= %d == %c\n", b, a, obj_GetType(new_tos_id));
					#endif
			}
			break;
			case 2:	// ==
				{
					new_tos_id =  obj_CreateEmpty(b == a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d == %d == %c\n", b, a, obj_GetType(new_tos_id));
					#endif
				}
				break;
			case 3:	// !=
				{
					new_tos_id =  obj_CreateEmpty(b != a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d != %d == %c\n", b, a, obj_GetType(new_tos_id));
					#endif
			}
			break;
			case 4:	// >
				{
					new_tos_id =  obj_CreateEmpty( b > a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d < %d == %c\n", b, a, obj_GetType(new_tos_id));
					#endif
				}
				break;
			case 5:	// >=
				{
					new_tos_id =  obj_CreateEmpty(b >= a ? TYPE_TRUE : TYPE_FALSE);
					#ifdef USE_DEBUGGING
					debug_printf(DEBUG_VERBOSE_STEP,"%d >= %d == %c\n", b, a, obj_GetType(new_tos_id));
					#endif
				}
				break;
		}
	}
	mem_unlock(tos1_id,0);
	mem_unlock(tos_id,0);
	return(new_tos_id);
}

OBJECT_ID custom_code(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID a = tuple_GetItem(locals,0);
	OBJECT_ID b = tuple_GetItem(locals,1);
	return(BinaryOp(a,b,OPCODE_BINARY_ADD));
}

/*
//OBJECT_ID if_assert(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
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
OBJECT_ID if_build_class(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	//printf("build_class called\n");
	FUNCTION_ID class_function_object_id = tuple_GetItem(locals,0);
	UNICODE_ID class_name_id = tuple_GetItem(locals,1);
	function_object *fo = (function_object*)mem_lock(class_function_object_id);
	CLASS_ID class_id = obj_CreateClass(fo->func,0);
	mem_unlock(class_function_object_id,0);

	class_object *class = (class_object*)mem_lock(class_id);
	if(tuple_GetLen(locals)> 2)
	{
		class->base_classes = obj_CreateTuple(tuple_GetLen(locals) - 2);
		obj_IncRefCount(class->base_classes);
		for(INDEX i = 0;i<tuple_GetLen(locals)-2;i++)
		{
			OBJECT_ID bc = tuple_GetItem(locals,2+i);
			tuple_SetItem(class->base_classes,i, bc);
		}
	}
	else
		class->base_classes = 0;
	mem_unlock(class_id,1);
	unicode_object *class_name = (unicode_object*)mem_lock(class_name_id);
	vm_AddGlobal(vm,obj_CreateUnicode(mem_copy(class_name->value)),class_id);
	mem_unlock(class_name_id,0);
	return(class_id);
}

#ifdef USE_INTERNAL_CLASSES
OBJECT_ID if_open(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	//if(x->type == TYPE_UNICODE)
	//	printf("opening file:%s\n",((unicode_object*)x)->value);
	
	UNICODE_ID file_class_key_id = obj_CreateUnicode(mem_create_string("file_class"));
	CLASS_ID file_class_id = vm_GetGlobal(vm,file_class_key_id);
	obj_IncRefCount(file_class_key_id);
	obj_DecRefCount(file_class_key_id);
	CLASS_INSTANCE_ID file_instance_id = obj_CreateClassInstance(file_class_id);	
	//#ifndef USE_ARDUINO_FUNCTIONS
	OBJECT_ID x_id = tuple_GetItem(locals,0);
	unicode_object *x = (unicode_object*)mem_lock(x_id);
	STREAM_ID fs = stream_CreateFromFile(x->value,mem_create_string("rb")); //TODO check for leaks
	mem_unlock(x_id,0);
	stream_Open(fs);
	TAG_ID file_tag = obj_CreateTag(fs);
	UNICODE_ID file_name = obj_CreateUnicode(mem_create_string("__file__"));
	obj_SetAttribute(file_instance_id,file_name,file_tag);	
	//#endif
	return(file_instance_id);
		
	//object *tmp =CreateEmptyObject(TYPE_NONE);
	//return (tmp);
}
#endif

OBJECT_ID if_list(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	TUPLE_ID r = obj_CreateTuple(0);
	INDEX i = 0;
	while(i < tuple_GetLen(locals))
	{
		OBJECT_ID t = tuple_GetItem(locals,i);
		if(obj_GetType(t) == TYPE_ITER)
			iter_ExpandTuple(t,vm,r);
		else
			tuple_AppendItem(r,t);
		i++;
	}
	return(r);
}

OBJECT_ID if_next(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	ITER_ID iter  = iter_CreateIter(tuple_GetItem(locals,0),vm);
	//object *iter = GetItem((object*)locals,0);
	obj_IncRefCount(iter);
	OBJECT_ID next = iter_NextNow(iter,vm);
	obj_DecRefCount(iter);
	return(next);
}

OBJECT_ID if_range(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	if(tuple_GetLen(locals) < 1)
	{
		OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"not enough args for range\n");
		#endif
		return(tmp);
	}
	ITER_ID iter = obj_CreateIter();
	OBJECT_ID s = tuple_GetItem(locals,0);
	OBJECT_ID e =  0;
	if(tuple_GetLen(locals) > 0)
		e = tuple_GetItem(locals,1);
	OBJECT_ID st = 0;
	if(tuple_GetLen(locals) > 1)
		st = tuple_GetItem(locals,2);
		
	if(st == 0 && e == 0 && obj_GetType(s) == TYPE_INT)
	{
		int_object *ts = (int_object*)mem_lock(s);
		iter_InitSequence(iter,vm,0,ts->value,1);//zero to s by 1
		mem_unlock(s,0);
	}
	else if(st == 0 && obj_GetType(s) == TYPE_INT && obj_GetType(e) == TYPE_INT)
	{
		int_object *ts = (int_object*)mem_lock(s);
		int_object *te = (int_object*)mem_lock(e);
		iter_InitSequence(iter,vm,ts->value,te->value,1);
		mem_unlock(e,0);
		mem_unlock(s,0);
	}
	else if(obj_GetType(s) == TYPE_INT && obj_GetType(e) == TYPE_INT && obj_GetType(st) == TYPE_INT)
	{
		int_object *ts = (int_object*)mem_lock(s);
		int_object *te = (int_object*)mem_lock(e);
		int_object *tst = (int_object*)mem_lock(st);
		iter_InitSequence(iter,vm,ts->value,te->value,tst->value);
		mem_unlock(st,0);
		mem_unlock(e,0);
		mem_unlock(s,0);
	}
	return(iter);
}
#ifndef USE_ARDUINO_FUNCTIONS
OBJECT_ID if_print(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	BOOL printed_something = 0;
	INDEX i = 0; 
	UNICODE_ID end = obj_CreateUnicode(mem_create_string("end"));
	obj_IncRefCount(end);
	OBJECT_ID endval = tuple_GetDictItem(kw_locals,end);
	obj_DecRefCount(end);
	while(i < tuple_GetLen(locals))
	{
		OBJECT_ID t = tuple_GetItem(locals,i);//TODO maybe move this into PrintObject() as case ITER:	
		if(obj_GetType(t) == TYPE_ITER)
		{
			TUPLE_ID to = iter_TupleExpand(t,vm);
			obj_IncRefCount(to);
			obj_Print(to);
			obj_DecRefCount(to);
			i++;
			continue;
		}
		if(obj_GetType(t) == TYPE_NONE)
		{
			i++;
			continue;
		}
		if (printed_something)
			printf(" ");
		if(t != 0)
			obj_Print(t);
		printed_something = 1;
		i++;
	}
	if(printed_something && endval != 0 && obj_GetType(endval) == TYPE_UNICODE)
	{	
		unicode_object *ue = (unicode_object*)mem_lock(endval);
		char *send = (char*)mem_lock(ue->value); 
		printf("%s",send);
		mem_unlock(ue->value,0);
		mem_unlock(endval,0);
	}
	else
	if(printed_something)
		printf("\n");
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
#endif

OBJECT_ID if_sum(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	INT sum = 0;
	INDEX i = 0;
	while(i < tuple_GetLen(locals))
	{
		OBJECT_ID t_id = tuple_GetItem(locals,i);//TODO maybe move this into PrintObject() as case ITER:	
		if(obj_GetType(t_id) == TYPE_ITER)
		{
			TUPLE_ID to = iter_TupleExpand(t_id,vm);
			obj_IncRefCount(to);
			OBJECT_ID r_id = if_sum(vm,to,0);
			switch (obj_GetType(r_id))
			{
				case TYPE_INT:
				{
					int_object *r = (int_object*)mem_lock(r_id);
					sum += r->value;
					mem_unlock(r_id,0);
				}
				break;
			}
			obj_IncRefCount(r_id);
			obj_DecRefCount(r_id);
			obj_DecRefCount(to);
			i++;
			continue;
		}

		if(t_id != 0)
			switch(obj_GetType(t_id))
			{
				case TYPE_INT:
				{
					int_object *t = (int_object*)mem_lock(t_id);
					sum += t->value;
					mem_unlock(t_id,0);
				}
				break;
			}
		i++;
	}
	INT_ID tmp = obj_CreateInt(sum);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"returning sum:%d\n",sum);
	#endif
	return(tmp);
}

OBJECT_ID if_pow(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	if(tuple_GetLen(locals) < 2)
	{
		OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
		#ifdef USE_DEBUGGING
		debug_printf(DEBUG_ALL,"not enough args for pow\n");
		#endif
		return(tmp);
	}
	OBJECT_ID x = tuple_GetItem(locals,0);
	OBJECT_ID y = tuple_GetItem(locals,1);
	OBJECT_ID z =  0;
	if(tuple_GetLen(locals) > 2)
		z = tuple_GetItem(locals,2);
	if(z == 0 && obj_GetType(x) == TYPE_INT && obj_GetType(y) == TYPE_INT)
	{	
		//object *r = BinaryOp(y,x,OPCODE_BINARY_POWER);
		int_object *ix = (int_object*)mem_lock(x);
		int_object *iy = (int_object*)mem_lock(y);
		OBJECT_ID r = obj_CreateInt(num_pow(ix->value,iy->value));
		mem_unlock(y,0);
		mem_unlock(x,0);
		//PrintObject(r);
		return(r);
	}
	else
	if (z != 0 && obj_GetType(x) == TYPE_INT && obj_GetType(y) == TYPE_INT && obj_GetType(z) == TYPE_INT)
	{
		OBJECT_ID r = BinaryOp(y,x,OPCODE_BINARY_POWER);
		OBJECT_ID rm = BinaryOp(r,z,OPCODE_BINARY_MODULO);
		obj_IncRefCount(r);
		obj_DecRefCount(r);
		return(rm);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID if_max(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID max_id = 0;
	INDEX i = 0;
	OBJECT_ID t_id = tuple_GetItem(locals,0);	
	if(obj_GetType(t_id) == TYPE_ITER)
	{
		TUPLE_ID to = iter_TupleExpand(t_id,vm);
		obj_IncRefCount(to);
		OBJECT_ID r = if_max(vm,to,0);
		obj_DecRefCount(to);
		//return(r);
		max_id = r;
	}

	while(i < tuple_GetLen(locals))
	{
		OBJECT_ID t_id = tuple_GetItem(locals,i);	

		if(t_id != 0)
		{
			switch(obj_GetType(t_id))
			{
				case TYPE_INT:
					{
						int_object *t = (int_object*)mem_lock(t_id);
						if(max_id == 0) //TODO bad construction , may lead into crashes if  min_id == t_id
							max_id = t_id;
						else
						{
							int_object *max = (int_object*)mem_lock(max_id);	
							if(t->value > max->value)
							{
								mem_unlock(max_id,0);
								max_id = t_id;//((int_object*)t)->value;
							}
						}
						mem_unlock(t_id,0);
					}
					break;
			}
		}
		i++;
	}
	if(max_id == 0)
	{
		OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
		return(tmp);
	}
	return(max_id);
}

OBJECT_ID if_abs(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID a_id = tuple_GetItem(locals,0);
	switch(obj_GetType(a_id))
	{
		case TYPE_INT:
			{
				int_object *a = (int_object*)mem_lock(a_id);
				INT_ID tmp = obj_CreateInt(abs(a->value));
				mem_unlock(a_id,0);
				return(tmp);
			}
		case TYPE_BINARY_FLOAT:
			{
				int_object *a = (int_object*)mem_lock(a_id);
				FLOAT_ID tmp = obj_CreateFloat(abs(a->value));
				mem_unlock(a_id,0);
				return(tmp);
			}
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID if_len(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	if(tuple_GetLen(locals) != 1)
	{
		OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
		return(tmp);
	}
	NUM ilen = tuple_GetLen(tuple_GetItem(locals,0));
	INT_ID len = obj_CreateInt(ilen);
	#ifdef USE_DEBUGGING
	debug_printf(DEBUG_VERBOSE_STEP,"returning len:%d\n",ilen);
	#endif
	return(len);
}

OBJECT_ID if_min(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID min_id = 0;
	INDEX i = 0;
	OBJECT_ID t_id = tuple_GetItem(locals,0);	
	if(obj_GetType(t_id) == TYPE_ITER)
	{
		TUPLE_ID to = iter_TupleExpand(t_id,vm);
		obj_IncRefCount(to);
		OBJECT_ID r = if_min(vm,to,0);
		obj_DecRefCount(to);
		//return(r);
		min_id = r;
	}

	while(i < tuple_GetLen(locals))
	{
		OBJECT_ID t_id = tuple_GetItem(locals,i);	

		if(t_id != 0)
		{
			switch(obj_GetType(t_id))
			{
				case TYPE_INT:
					{
						int_object *t = (int_object*)mem_lock(t_id);
						if(min_id == 0) //TODO bad construction , may lead into crashes if  min_id == t_id
							min_id = t_id;
						else
						{
							int_object *min = (int_object*)mem_lock(min_id);	
							if(t->value < min->value)
							{
								mem_unlock(min_id,0);
								min_id = t_id;//((int_object*)t)->value;
							}
						}
						mem_unlock(t_id,0);
					}
					break;
			}
		}
		i++;
	}
	if(min_id == 0)
	{
		OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
		return(tmp);
	}
	return(min_id);
}

OBJECT_ID if_sorted(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID if_reversed(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID if_all(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	ITER_ID iter = iter_CreateIter(tuple_GetItem(locals,0),vm);
	obj_IncRefCount(iter);
	OBJECT_ID r_id = obj_CreateEmpty(TYPE_TRUE);
	object *r = (object*)mem_lock(r_id);
	OBJECT_ID n = 0;
	do
	{
		n = iter_NextNow(iter,vm);
		if(obj_GetType(n) != TYPE_TRUE)
		 r->type = TYPE_FALSE;
	}while(obj_GetType(n) != TYPE_NONE);
	obj_DecRefCount(iter);
	mem_unlock(r_id,1);
	return(r_id);
}

OBJECT_ID if_any(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	ITER_ID iter  = iter_CreateIter(tuple_GetItem(locals,0),vm);
	obj_IncRefCount(iter);
	OBJECT_ID r_id = obj_CreateEmpty(TYPE_TRUE);
	object *r = (object*)mem_lock(r_id);
	OBJECT_ID n = 0;
	do
	{
		n = iter_NextNow(iter,vm);
		if(obj_GetType(n) == TYPE_TRUE)
		 r->type = TYPE_TRUE;
	}while(obj_GetType(n) != TYPE_NONE);
	obj_DecRefCount(iter);
	mem_unlock(r_id,1);
	return(r_id);
}

OBJECT_ID if_chr(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID a_id = tuple_GetItem(locals,0);
	if(obj_GetType(a_id) == TYPE_INT)
	{
		int_object *a = (int_object*)mem_lock(a_id);
		BYTES_ID chr = str_FromChar(a->value);
		UNICODE_ID r = obj_CreateUnicode(chr);
		mem_unlock(a_id,0);
		//free(chr);TODO check if it leaks here
		return(r);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID if_ord(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID a_id = tuple_GetItem(locals,0);
	unicode_object *a = (unicode_object*)mem_lock(a_id);
	if(a->type == TYPE_UNICODE)
	{
		char *av = (char*)mem_lock(a->value);
		if(strlen(av) > 0)
		{	
			INT_ID r = obj_CreateInt(av[0]);
			mem_unlock(a->value,0);
			mem_unlock(a_id,0);
			return(r);
		}
		mem_unlock(a->value,0);
	}
	mem_unlock(a_id,0);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID if_cmp(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID if_hex(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID a_id = tuple_GetItem(locals,0);
	int_object *a = (int_object*)mem_lock(a_id);
	if(a->type == TYPE_INT)
	{
		NUM chars = 0;
		for(INDEX i = 0;i<4;i++) // calculate space requirements
		{
			if((a->value >> (i*8) & 255) > 0 )
				chars = i;
		}
		#ifdef USE_MEMORY_DEBUGGING
		BYTES_ID tmp_id = mem_malloc_debug(chars*2 + 1,MEM_POOL_CLASS_DYNAMIC, "if_hex(); tmp");
		#else
		BYTES_ID tmp_id = mem_malloc(chars*2 + 1,MEM_POOL_CLASS_DYNAMIC);
		#endif
		char *tmp = (char*)mem_lock(tmp_id);
		memset(tmp,0,chars*2 + 1);
		sprintf(tmp,"%X",a->value);
		
		UNICODE_ID r = obj_CreateUnicode(mem_create_string(tmp));
		mem_unlock(tmp_id,1);
	
		mem_free(tmp_id);
		return(r);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);

}

OBJECT_ID if_int(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID if_float(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}

OBJECT_ID if_iter(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	if(tuple_GetLen(locals) == 2)
	{//uses a sentinel
		//TODO will not work
		ITER_ID iter  = iter_CreateIter(tuple_GetItem(locals,0),vm);
		return(iter);
	}
	else
	if(tuple_GetLen(locals) == 2)
	{
		ITER_ID iter  = iter_CreateIter(tuple_GetItem(locals,0),vm);
		return(iter);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
/*
OBJECT_ID if_map(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);
}
*/

#ifndef USE_ARDUINO_FUNCTIONS

OBJECT_ID a_pinMode(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	if(tuple_GetLen(locals) < 2)
	{
		OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
		return(tmp);	
	}
	OBJECT_ID pin = tuple_GetItem(locals,0);
	OBJECT_ID mode = tuple_GetItem(locals,1);
	if(obj_GetType(pin) == TYPE_KV)
	{
		kv_object *kpin = (kv_object*)mem_lock(pin);
		OBJECT_ID old = pin;
		pin = kpin->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(mode) == TYPE_KV)
	{
		kv_object *kmode = (kv_object*)mem_lock(mode);
		OBJECT_ID old = mode;
		mode = kmode->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(pin) == TYPE_INT && obj_GetType(mode) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		int_object *imode = (int_object*)mem_lock(mode);
		printf("pinMode: %d to %d\n",ipin->value,imode->value);
		mem_unlock(mode,0);
		mem_unlock(pin,0);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);	
}

OBJECT_ID a_digitalRead(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	OBJECT_ID pin = tuple_GetItem(locals,0);
	int r = 0;
	if(obj_GetType(pin) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		printf("digitalRead: %d\n",ipin->value);
		mem_unlock(pin,0);
	}
	INT_ID tmp = obj_CreateInt(r);
	return(tmp);	
}

OBJECT_ID a_digitalWrite(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
 	if(tuple_GetLen(locals) < 2)
	{
		OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
		return(tmp);	
	}
	OBJECT_ID pin = tuple_GetItem(locals,0);
	OBJECT_ID val = tuple_GetItem(locals,1);
	if(obj_GetType(pin) == TYPE_KV)
	{
		kv_object *kpin = (kv_object*)mem_lock(pin);
		OBJECT_ID old = pin;
		pin = kpin->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(val) == TYPE_KV)
	{
		kv_object *kval = (kv_object*)mem_lock(val);
		OBJECT_ID old = val;
		val = kval->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(pin) == TYPE_INT && obj_GetType(val) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		int_object *ival = (int_object*)mem_lock(val);
		printf("digitalWrite pin: %d val: %d\n",ipin->value,ival->value);
		mem_unlock(val,0);
		mem_unlock(pin,0);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);	
}

OBJECT_ID a_analogRead(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
 	OBJECT_ID pin = tuple_GetItem(locals,0);
	int r = 0;
	if(obj_GetType(pin) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		printf("analogRead: %d\n",ipin->value);
		mem_unlock(pin,0);
	}
	INT_ID tmp = obj_CreateInt(r);
	return(tmp);	
}

OBJECT_ID a_analogWrite(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
	//obj_Dump(locals,1,1);
	OBJECT_ID pin = tuple_GetItem(locals,0);
	OBJECT_ID val = tuple_GetItem(locals,1);
	if(obj_GetType(pin) == TYPE_KV)
	{
		kv_object *kpin = (kv_object*)mem_lock(pin);
		OBJECT_ID old = pin;
		pin = kpin->value;
		mem_unlock(old,0);
	}
	if(obj_GetType(val) == TYPE_KV)
	{
		kv_object *kval = (kv_object*)mem_lock(val);
		OBJECT_ID old = val;
		val = kval->value;
		mem_unlock(old,0);
		printf("kv'd value\n");
	}
	if(obj_GetType(pin) == TYPE_INT && obj_GetType(val) == TYPE_INT)
	{
		int_object *ipin = (int_object*)mem_lock(pin);
		int_object *ival = (int_object*)mem_lock(val);
		//obj_Dump(val,1,1);
		//obj_Print(val);
		printf("analogWrite pin: %d\n",ipin->value);
		printf("value: %d\n",ival->value);
		mem_unlock(val,0);
		mem_unlock(pin,0);
	}
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);	
}

OBJECT_ID a_delay(VM_ID vm,TUPLE_ID locals,TUPLE_ID kw_locals)
{
 	OBJECT_ID ms_id = tuple_GetItem(locals,0);
	int_object *ms = (int_object*)mem_lock(ms_id);
	printf("delay: %d\n",ms->value);
	//TODO add delay here
	mem_unlock(ms_id,0);
	OBJECT_ID tmp = obj_CreateEmpty(TYPE_NONE);
	return(tmp);	
}
/*
OBJECT_ID a_serialprint(vm *vm,tuple_object *locals,tuple_object *kw_locals)
{
 //message
 	//printf("serial.Println\n");
 	object *message = GetItem((object*)locals,0);
	//if(message->type == TYPE_UNICODE)
	//	Serial.print(((unicode_object*)message)->value);
	object *tmp =CreateEmptyObject(TYPE_NONE);
	return (tmp);	
}

OBJECT_ID a_serialBegin(vm *vm,tuple_object *locals,tuple_object *kw_locals)
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
void AddArduinoGlobal(VM_ID vm)
{
	CODE_ID a_globals = obj_CreateCode(mem_create_string("arduino"));
	obj_AddCodeCFunction(a_globals,mem_create_string("pinMode"),&a_pinMode);
	obj_AddCodeCFunction(a_globals,mem_create_string("digitalRead"),&a_digitalRead);
	obj_AddCodeCFunction(a_globals,mem_create_string("digitalWrite"),&a_digitalWrite);
	obj_AddCodeCFunction(a_globals,mem_create_string("analogRead"),&a_analogRead);
	obj_AddCodeCFunction(a_globals,mem_create_string("analogWrite"),&a_analogWrite);
	obj_AddCodeCFunction(a_globals,mem_create_string("delay"),&a_delay);
	//obj_AddCodeCFunction(a_globals,mem_create_string("Serial.print"),&a_serialprint);
	//obj_AddCodeCFunction(a_globals,mem_create_string("Serial.Begin"),&a_serialBegin);
	obj_AddCodeName(a_globals,obj_CreateUnicode(mem_create_string("INPUT")),obj_CreateInt(0));
	obj_AddCodeName(a_globals,obj_CreateUnicode(mem_create_string("OUTPUT")),obj_CreateInt(1));
	obj_AddCodeName(a_globals,obj_CreateUnicode(mem_create_string("LOW")),obj_CreateInt(0));
	obj_AddCodeName(a_globals,obj_CreateUnicode(mem_create_string("HIGH")),obj_CreateInt(1));
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("arduino")),a_globals);
}

#endif

void AddInternalFunctions(VM_ID vm)
{
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("list")),obj_CreateCFunction(&if_list));
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("range")),obj_CreateCFunction(&if_range));
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("pow")),obj_CreateCFunction(&if_pow));
	#ifndef USE_ARDUINO_FUNCTIONS
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("print")),obj_CreateCFunction(&if_print));
	#endif
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("sum")),obj_CreateCFunction(&if_sum));
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("next")),obj_CreateCFunction(&if_next));
	#ifdef USE_INTERNAL_CLASSES
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("open")),obj_CreateCFunction(&if_open));
	#endif
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("iter")),obj_CreateCFunction(&if_iter));
	vm_AddGlobal(vm,obj_CreateUnicode(mem_create_string("custom_code")),obj_CreateCFunction(&custom_code));
}

