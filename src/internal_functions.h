#ifndef INTERNAL_FUNCTIONS_H
#define INTERNAL_FUNCTIONS_H

#include "object.h"
#include "stack.h"
#include "vm.h"

object *pow(object *base,object *exp);

object *if_range(stack *stack);
object *if_print(stack *stack);


#endif