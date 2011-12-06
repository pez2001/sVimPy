#ifndef STACK_H
#define STACK_H
#include "object.h"

#define STACK_ITEM_SIZE 4
//#define STACK_TYPE *void
#define STACK_TYPE *object;

#define STACK_MIN_ITEMS 200
#define STACK_SIZE_INCREASE 200

void stack_Init();
object *stack_Pop();
void stack_Push(object *x);

object *callstack_Pop();
void callstack_Push(object *x);


#endif
