#include "stack.h"

object **stack_items;
long stack_top = 0;
//long stack_
object **callstack_items;
long callstack_top = 0;

void stack_Init()
{
stack_items = (object**)malloc(STACK_MIN_ITEMS*sizeof(object*));
stack_top = 0;

callstack_items = (object**)malloc(STACK_MIN_ITEMS*sizeof(object*));
callstack_top = 0;

}

void stack_Push(object *x)
{
stack_items[stack_top] = x;
stack_top++;
}

object *stack_Pop()
{
if(stack_top < 0)
 return(NULL);
object *r = stack_items[stack_top-1];
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
 return(NULL);
object *r = callstack_items[callstack_top-1];
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