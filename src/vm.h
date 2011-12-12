#ifndef VM_H
#define VM_H

#define DEBUG
#include "object.h"
#include "opcodes.h"
#include "stack.h"
#include "numops.h"
extern const opcode opcodes[];

extern stack *blocks;

object *ExecuteObject(object *obj,object *caller,object *global,stack *locals,int argc);


#endif