#include "stack.h"

object **stack_items;
long stack_top = 0;
//long stack_
object **callstack_items;
long callstack_top = 0;

object **recycle_items;
long recycle_top = 0;

void stack_Init()
{
stack_items = (object**)mem_malloc(STACK_MIN_ITEMS*sizeof(object*));
stack_top = 0;

callstack_items = (object**)mem_malloc(CALLSTACK_MIN_ITEMS*sizeof(object*));
callstack_top = 0;

recycle_items = (object**)mem_malloc(STACK_MIN_ITEMS*sizeof(object*));
recycle_top = 0;

}

void stack_Close()
{
printf("freeing %d stack items, %d callstack items, %d recycle items\n",stack_top,callstack_top,recycle_top);
for(int i=0;i<stack_top;i++)
 FreeObject(stack_items[i]);
 mem_free(stack_items);
for(int i=0;i<callstack_top;i++)
 FreeObject(callstack_items[i]);
 mem_free(callstack_items);
for(int i=0;i<recycle_top;i++)
 FreeObject(recycle_items[i]);
 mem_free(recycle_items);


}

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
recycle_items[recycle_top] = x;
recycle_top++;
}


void stack_Push(object *x)
{
stack_items[stack_top] = x;
stack_top++;
}

object *stack_Pop()
{
if(stack_top < 1)
{
 printf("stack underflow\n");
 return(NULL);
 }
object *r = stack_items[stack_top-1];
if(r->flags & OFLAG_ON_STACK)
 {
 printf("removed stack only item\n");
 recycle_Push(r);
 }
stack_top--;
//if(stack_top < 0)
// stack_top = 0;
return(r);
}

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
if(r->flags & OFLAG_ON_STACK)
 {
 printf("removed stack only item from callstack\n");
 recycle_Push(r);
 }
callstack_top--;
//if(stack_top < 0)
// stack_top = 0;
return(r);
}

void stack_IncreaseSize()
{

}
void stack_DecreaseSize()
{

}