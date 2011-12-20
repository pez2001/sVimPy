/*
 * sVimPy - small Virtual interpreting machine for Python
 * (c) 2012 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */

#ifndef VM_H
#define VM_H

#define DEBUG
#include "object.h"
#include "opcodes.h"
#include "stack.h"
#include "numops.h"

#include "assert.h"


typedef struct {



}vm;

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
vm *vm_Init(stream *s);
void vm_Close();


object *ExecuteObject(object *obj,object *caller,object *global,stack *locals,int argc);


#endif