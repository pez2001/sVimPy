#include "stack.h"

stack* stack_Init(long items_num)
{
if(!items_num)
 return(NULL);
stack *tmp = (stack*)mem_malloc(sizeof(stack));
tmp->items = (object**)mem_malloc(items_num*sizeof(object*));
printf("init stack @%x\n",tmp);
printf("init stack items @%x\n",tmp->items);
tmp->top = 0;

//callstack_items = (object**)mem_malloc(CALLSTACK_MIN_ITEMS*sizeof(object*));
//callstack_top = 0;

//recycle_items = (object**)mem_malloc(STACK_MIN_ITEMS*sizeof(object*));
//recycle_top = 0;
return(tmp);
}

void stack_Close(stack *stack,int free_objects)
{
if(stack != NULL)
{
if(free_objects)
{
printf("freeing %d stack items\n",stack->top);
for(int i=0;i<stack->top;i++)
 if(stack->items[i]->flags & OFLAG_ON_STACK) 
  FreeObject(stack->items[i]);
 }
 else
  if(stack->top>0)
   printf("%d items left untouched on stack\n",stack->top);
 
 mem_free(stack->items);
 mem_free(stack);
 printf("freed stack\n");
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
stack->items[stack->top] = x;
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

void stack_SetTop(object *x,stack *stack)
{
if(stack->top < 1)
{
 printf("stack underflow - no top\n");
 return(NULL);
 }
 stack->items[stack->top-1] = x;

}

object *stack_Pop(stack *stack)
{
if(stack->top < 1)
{
 printf("stack underflow\n");
 return(NULL);
 }
object *r = stack->items[stack->top-1];
if(r->flags & OFLAG_ON_STACK)
 {
 //printf("removed stack only item\n");
 if(!stack_Contains(r,recycle))
  stack_Push(r,recycle);
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

/*
void recycle_Remove(object *x)
{
for(int i =0;i<recycle_top;i++)
{
if(x == recycle_items[i])
 printf("found object @%x\n",x);

}
}
*/
/*
object *recycle_Pop()
{
if(recycle_top < 1)
{
 printf("recycle stack underflow\n");
 return(NULL);
 }
object *r = recycle_items[recycle_top-1];
recycle_top--;
return(r);
}

void recycle_Push(object *x)
{
//printf("added recycle item @%x\n",x);
recycle_items[recycle_top] = x;
recycle_top++;
}
*/
/*
void callstack_Push(object *x)
{
callstack_items[callstack_top] = x;
callstack_top++;
}

object *callstack_Pop()
{
if(callstack_top < 0)
{
 printf("callstack underflow\n");
return(NULL);
 }
object *r = callstack_items[callstack_top-1];
//if(r->flags & OFLAG_ON_STACK)
// {
// printf("removed stack only item from callstack\n");
// recycle_Push(r);
// }
 
callstack_top--;
//if(stack_top < 0)
// stack_top = 0;
return(r);
}
*/
/*

void stack_IncreaseSize()
{

}
void stack_DecreaseSize()
{

}
*/
