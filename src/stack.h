#ifndef STACK_H
#define STACK_H
#include "object.h"

#define STACK_ITEM_SIZE 4
//#define STACK_TYPE *void
#define STACK_TYPE *object;

#define STACK_MIN_ITEMS 100
#define CALLSTACK_MIN_ITEMS 20
#define STACK_SIZE_INCREASE 200


typedef struct 
{
object **items;
long top;
}stack;

extern stack *recycle;

stack* stack_Init(long items_num);
object *stack_Pop(stack *stack);
void stack_Push(object *x,stack *stack);
void stack_Close(stack *stack,int free_objects);
int stack_Contains(object *x,stack *stack);
object *stack_Top(stack *stack);
void stack_SetTop(object *x,stack *stack);

object *stack_Second(stack *stack);
void stack_SetSecond(object *x,stack *stack);
object *stack_Third(stack *stack);
void stack_SetThird(object *x,stack *stack);

void stack_Adjust(int by,stack *stack);


//object *callstack_Pop();
//void callstack_Push(object *x);

//object *recycle_Pop();
//void recycle_Push(object *x);


#endif
