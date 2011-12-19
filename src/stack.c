#include "stack.h"

stack* stack_Init(long items_num)
{
if(!items_num)
 return(NULL);
stack *tmp = (stack*)mem_malloc(sizeof(stack),"stack_Init() return");
tmp->items = (object**)mem_malloc(items_num*sizeof(object*),"stack_Init() items");
//printf("init stack @%x\n",tmp);
//printf("init stack items @%x\n",tmp->items);
tmp->top = 0;
tmp->num = items_num;
//callstack_items = (object**)mem_malloc(CALLSTACK_MIN_ITEMS*sizeof(object*));
//callstack_top = 0;

//recycle_items = (object**)mem_malloc(STACK_MIN_ITEMS*sizeof(object*));
//recycle_top = 0;
return(tmp);
}

void stack_IncreaseSize(int items_num,stack *stack)
{
if(stack == NULL)
 return;
stack->items = (object**)mem_realloc(stack->items,stack->num + items_num*sizeof(object*));
stack->num += items_num;
printf("increasing stack size by:%d\n",items_num);
}

void stack_Close(stack *stack,int free_objects)
{
if(stack != NULL)
{
if(free_objects)
{
printf("freeing %d stack items\n",stack->top);
for(int i=0;i<stack->top;i++)
 if((stack->items[i]->flags & OFLAG_ON_STACK) > 0) 
 {
  printf("free:%d\n",i);
  FreeObject(stack->items[i]);
  }
 }
 else
  if(stack->top>0)
   printf("%d items left untouched on stack\n",stack->top);
 
 assert(mem_free(stack->items));
 //printf("freed stack items @%x\n",stack->items);
 assert(mem_free(stack));
 //printf("freed stack @%x\n",stack);
//for(int i=0;i<callstack_top;i++)
// if(callstack_items[i]->flags & OFLAG_ON_STACK) 
// FreeObject(callstack_items[i]);
// mem_free(callstack_items);
//for(int i=0;i<recycle_top;i++)
// FreeObject(recycle_items[i]);
// mem_free(recycle_items);
}
}



void stack_Push(object *x,stack *stack)
{
if(x == NULL)
 return;
if(stack->top == stack->num)
 stack_IncreaseSize(1,stack);
stack->items[stack->top] = x;
//printf("put object on stack @:%d\n",stack->top);
//DumpObject(x,0);
stack->top++;
}

object *stack_Top(stack *stack)
{
if(stack->top < 1)
{
 printf("stack underflow - no top\n");
 return(NULL);
 }
object *r = stack->items[stack->top-1];
return(r);
}

object *stack_Second(stack *stack)
{
if(stack->top < 2)
{
 printf("stack underflow - no top\n");
 return(NULL);
 }
object *r = stack->items[stack->top-2];
return(r);
}

object *stack_Third(stack *stack)
{
if(stack->top < 3)
{
 printf("stack underflow - no top\n");
 return(NULL);
 }
object *r = stack->items[stack->top-3];
return(r);
}



void stack_SetTop(object *x,stack *stack)
{
if(stack->top < 1)
{
 printf("stack underflow - no top\n");
 return(NULL);
 }
 stack->items[stack->top-1] = x;

}

void stack_SetSecond(object *x,stack *stack)
{
if(stack->top < 2)
{
 printf("stack underflow - no top\n");
 return(NULL);
 }
 stack->items[stack->top-2] = x;

}

void stack_SetThird(object *x,stack *stack)
{
if(stack->top < 3)
{
 printf("stack underflow - no top\n");
 return(NULL);
 }
 stack->items[stack->top-3] = x;

}

void stack_Adjust(int by,stack *stack)
{
if((stack->top + by) > stack->num)
 stack_IncreaseSize(((stack->top +by) - stack->num),stack);

stack->top += by;
}

void stack_Dump(stack *stack)
{
printf("Dumping %d stack items\n",stack->top);
for(int i=0;i<stack->top;i++)
{
	printf("item: %d\n",i);
	DumpObject(stack->items[i],1);

}
}


object *stack_Pop(stack *stack)
{
if(stack->top < 1)
{
 printf("stack underflow\n");
 return(NULL);
 }
object *r = stack->items[stack->top-1];
if((r->flags & OFLAG_ON_STACK))
 {
 //printf("removed stack only item\n");
 if(!stack_Contains(r,recycle))
 {
  printf("put object on recycle stack @:%d\n",recycle->top);
  stack_Push(r,recycle);
  }
 }
stack->top--;
//if(stack_top < 0)
// stack_top = 0;
return(r);
}

int stack_Contains(object *x,stack *stack)
{
for(int i=0;i<stack->top;i++)
 if(stack->items[i] == x)
  return(1);
return(0);
}


void recycle_Remove(object *x)
{
for(int i =0;i<recycle->top;i++)
{
if(x == recycle->items[i])
 printf("found object @%x\n",x);

}
}

