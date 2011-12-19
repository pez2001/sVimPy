#include "internal_functions.h"


object *if_range(stack *stack)
{
//printf("range called\n");
if(stack->top <2)
{
		object *tmp = AllocEmptyObject();
		tmp->type = TYPE_NONE;
		tmp->flags = OFLAG_ON_STACK;
        return(tmp);
}
object *s = stack_Pop(stack);
object *e = stack_Pop(stack);

if(s->type == TYPE_INT && e->type == TYPE_INT)
{
object *r = AllocObject();
r->type = TYPE_TUPLE;
r->flags = OFLAG_ON_STACK;
r->ptr = AllocTupleObject();
int n = (long)e->ptr - (long)s->ptr;
((tuple_object*)r->ptr)->num = n;
((tuple_object*)r->ptr)->items = (object**)mem_malloc(n*sizeof(object*),"if_range() items");
for(int i = 0;i<n;i++)
{
((tuple_object*)r->ptr)->items[i] = AllocObject();
((tuple_object*)r->ptr)->items[i]->type = TYPE_INT;
((tuple_object*)r->ptr)->items[i]->ptr = (long)s->ptr + i;
}
//DumpObject(r,0);
return(r);
}

object *tmp = AllocEmptyObject();
tmp->type = TYPE_NONE;
tmp->flags = OFLAG_ON_STACK;
return(tmp);
}

object *if_print(stack *stack)
{
//printf("print called\n");
int num =stack->top;
			for(int i = 0;i<num;i++)
			{
				object *tos = stack_Pop(stack);
				//if(tos != NULL && tos->type == TYPE_UNICODE)
				//printf("arg[%d]:%s\n",i,((unicode_object*)tos->ptr)->content );
				//else 
				if(i)
				 printf(" ");
				if(tos != NULL)
				//printf("tos type:%c\n",tos->type);
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
					}
				//else 
				//printf("tos = NULL\n");
			}
		object *tmp = AllocEmptyObject();
		tmp->type = TYPE_NONE;
		tmp->flags = OFLAG_ON_STACK;
        return(tmp);
}
