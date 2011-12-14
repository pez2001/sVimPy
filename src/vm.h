#ifndef VM_H
#define VM_H

#define DEBUG
#include "object.h"
#include "opcodes.h"
#include "stack.h"
#include "numops.h"
extern const opcode opcodes[];

extern stack *blocks;

#define FUNC_PYTHON 1
#define FUNC_C 2
#define FUNC_C_OBJ 4

typedef struct {
unsigned int type;
union func_def
{
object *(*func)(stack *stack);
object *(*func_obj)(object *object);
object *code;
}func;
char *name;
}function_definition;

function_definition *CreateCFunction(object *(*func)(stack *stack),char *name);
function_definition *CreateCObjFunction(object *(*func)(object *obj),char *name);
function_definition *CreatePythonFunction(object *code,char *name);

int AddFunctionDefinition(function_definition *fd);
void RemoveFunction(char *name);
void RemoveFunctionDefinition(function_definition *fd);
object *ExecuteCFunction(char *name,stack *stack);
object *ExecuteCObjFunction(char *name,object *obj);
function_definition *FindFunction(char *name);
void vm_Init();
void vm_Close();

object *BuildList(stack *stack,int argc);


object *ExecuteObject(object *obj,object *caller,object *global,stack *locals,int argc);


#endif