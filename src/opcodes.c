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


#include "opcodes.h"

#if defined(USE_DEBUGGING) || defined(USE_ARDUINO_OPCODE_DEBUGGING)

const NUM cmp_opscount = 12;

const cmp_op cmp_ops[] = {
{0, "<"},
{1, "<="},
{2, "=="},
{3, "!="},
{4, ">"},
{5, ">="},
{6, "in"},
{7, "not in"},
{8, "is"},
{9, "is not"},
{10, "exception match"}, //NOT SUPPORTED
{11, "BAD"} //NOT SUPPORTED
};

const NUM opcodecount = 102;

#ifndef USE_ARDUINO_OPCODE_DEBUGGING


const opcode opcodes[] = {
{OPCODE_STOP_CODE, "STOP_CODE", "Indicates end-of-code to the compiler, not used by the interpreter.", 0, 1},	// SUPPORTED
{OPCODE_POP_TOP, "POP_TOP", "Removes the top-of-stack (TOS) item.", 0, 1},	// SUPPORTED
{OPCODE_ROT_TWO, "ROT_TWO", "Swaps the two top-most stack items.", 0, 1},	// SUPPORTED
{OPCODE_ROT_THREE, "ROT_THREE", "Lifts second and third stack item one position up, moves top down to position three.", 0, 1},	// SUPPORTED
{OPCODE_DUP_TOP, "DUP_TOP", "Duplicates the reference on top of the stack.", 0, 1},	// SUPPORTED
{OPCODE_DUP_TOP_TWO, "DUP_TOP_TWO", "Duplicates the reference on top of the stack.", 0, 1},	// SUPPORTED
{OPCODE_NOP, "NOP", "Do nothing code. Used as a placeholder by the bytecode optimizer.", 0, 1},	// SUPPORTED
{OPCODE_UNARY_POSITIVE, "UNARY_POSITIVE", "Implements TOS = +TOS.", 0, 1},	// SUPPORTED
{OPCODE_UNARY_NEGATIVE, "UNARY_NEGATIVE", "Implements TOS = -TOS.", 0, 1},	// SUPPORTED
{OPCODE_UNARY_NOT, "UNARY_NOT", "Implements TOS = not TOS.", 0, 1},	// SUPPORTED
{OPCODE_UNARY_INVERT, "UNARY_INVERT", "Implements TOS = ~TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_POWER, "BINARY_POWER", "Implements TOS = TOS1 ** TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_MULTIPLY, "BINARY_MULTIPLY", "Implements TOS = TOS1 * TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_DIVIDE, "BINARY_DIVIDE", "Implements TOS = TOS1 / TOS when from __future__ import division is not in effect.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_MODULO, "BINARY_MODULO", "Implements TOS = TOS1 % TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_ADD, "BINARY_ADD", "Implements TOS = TOS1 + TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_SUBTRACT, "BINARY_SUBTRACT", "Implements TOS = TOS1 - TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_SUBSCR, "BINARY_SUBSCR", "Implements TOS = TOS1[TOS].", 0, 1},	// SUPPORTED
{OPCODE_BINARY_FLOOR_DIVIDE, "BINARY_FLOOR_DIVIDE", "Implements TOS = TOS1 // TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_TRUE_DIVIDE, "BINARY_TRUE_DIVIDE", "Implements TOS = TOS1 / TOS when from __future__ import division is in effect.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_FLOOR_DIVIDE, "INPLACE_FLOOR_DIVIDE", "Implements in-place TOS = TOS1 // TOS.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_TRUE_DIVIDE, "INPLACE_TRUE_DIVIDE", "Implements in-place TOS = TOS1 / TOS when from __future__ import division is in effect.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_ADD, "INPLACE_ADD", "Implements in-place TOS = TOS1 + TOS.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_SUBTRACT, "INPLACE_SUBTRACT", "Implements in-place TOS = TOS1 - TOS.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_MULTIPLY, "INPLACE_MULTIPLY", "Implements in-place TOS = TOS1 * TOS.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_DIVIDE, "INPLACE_DIVIDE", "Implements in-place TOS = TOS1 / TOS when from __future__ import division is not in effect.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_MODULO, "INPLACE_MODULO", "Implements in-place TOS = TOS1 % TOS.", 0, 1},	// SUPPORTED
{OPCODE_STORE_SUBSCR, "STORE_SUBSCR", "Implements TOS1[TOS] = TOS2.", 0, 1},	// SUPPORTED
{OPCODE_DELETE_SUBSCR, "DELETE_SUBSCR", "Implements del TOS1[TOS].", 0, 1},	// SUPPORTED
{OPCODE_BINARY_AND, "BINARY_AND", "Implements TOS = TOS1 & TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_XOR, "BINARY_XOR", "Implements TOS = TOS1 ^ TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_OR, "BINARY_OR", "Implements TOS = TOS1 | TOS.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_POWER, "INPLACE_POWER", "Implements in-place TOS = TOS1 ** TOS.", 0, 1},	// SUPPORTED
{OPCODE_GET_ITER, "GET_ITER", "Implements TOS = iter(TOS).", 0, 1},	// SUPPORTED
{OPCODE_PRINT_EXPR, "PRINT_EXPR", "Implements the expression statement for the interactive mode.\n\
TOS is removed from the stack and printed. In non-interactive mode, an expression statement is terminated with POP_STACK.", 0, 1},	// SUPPORTED 
{OPCODE_INPLACE_AND, "INPLACE_AND", "Implements in-place TOS = TOS1 & TOS.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_XOR, "INPLACE_XOR", "Implements in-place TOS = TOS1 ^ TOS.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_OR, "INPLACE_OR", "Implements in-place TOS = TOS1 | TOS.", 0, 1},	// SUPPORTED
{OPCODE_BREAK_LOOP, "BREAK_LOOP", "Terminates a loop due to a break statement.", 0, 1},	// SUPPORTED
{OPCODE_RETURN_VALUE, "RETURN_VALUE", "Returns with TOS to the caller of the function.", 0, 1},	// SUPPORTED
{OPCODE_POP_BLOCK, "POP_BLOCK", "Removes 1 block from the block stack.Per frame, there is a stack of blocks, denoting nested loops,try blocks,and such.", 0, 1},	// SUPPORTED
{OPCODE_STORE_NAME, "STORE_NAME", "Implements name = TOS. /namei/ is the index of name in the attribute co_names of the code object.\n\
The compiler tries to use STORE_LOCAL or STORE_GLOBAL if possible.", 1, 1},	// SUPPORTED
{OPCODE_DELETE_NAME, "DELETE_NAME", "Implements del name, where /namei/ is the index into co_names attribute of the code object.", 1, 1},	// SUPPORTED
{OPCODE_UNPACK_SEQUENCE, "UNPACK_SEQUENCE", "Unpacks TOS into /count/ individual values, which are put onto the stack right-to-left.", 1, 1},	// SUPPORTED
{OPCODE_FOR_ITER, "FOR_ITER", "TOS is an iterator. Call its next() method. If this yields a new value, push it on the stack\n\
(leaving the iterator below it). If the iterator indicates it is exhausted TOS is popped,\n\
and the byte code counter is incremented by /delta/.", 1, 1},	// SUPPORTED
{OPCODE_STORE_GLOBAL, "STORE_GLOBAL", "Works as STORE_NAME(/namei/), but stores the name as a global.", 1, 1},	// SUPPORTED
{OPCODE_DELETE_GLOBAL, "DELETE_GLOBAL", "Works as DELETE_NAME(/namei/), but deletes a global name.", 1, 1},	// SUPPORTED 																											// 
{OPCODE_LOAD_CONST, "LOAD_CONST", "Pushes \"co_consts[/consti/]\" onto the stack.", 1, 1},	// SUPPORTED
{OPCODE_LOAD_NAME, "LOAD_NAME", "Pushes the value associated with \"co_names[/namei/]\" onto the stack.", 1, 1},	// SUPPORTED
{OPCODE_BUILD_TUPLE, "BUILD_TUPLE", "Creates a tuple consuming /count/ items from the stack, and pushes the resulting tuple onto the stack.", 1, 1},	// SUPPORTED
{OPCODE_BUILD_LIST, "BUILD_LIST", "Works as BUILD_TUPLE(/count/), but creates a list.", 1, 1},	// SUPPORTED
{OPCODE_COMPARE_OP, "COMPARE_OP", "Performs a Boolean operation. The operation name can be found in cmp_op[/opname/].", 1, 1},	// SUPPORTED 
{OPCODE_JUMP_FORWARD, "JUMP_FORWARD", "Increments byte code counter by /delta/.", 1, 1},	// SUPPORTED
{OPCODE_JUMP_IF_FALSE, "JUMP_IF_FALSE", "If TOS is false, increment the byte code counter by /delta/. TOS is not changed.", 1, 1},	// SUPPORTED
{OPCODE_JUMP_IF_TRUE, "JUMP_IF_TRUE", "If TOS is true, increment the byte code counter by /delta/. TOS is left on the stack.", 1, 1},	// SUPPORTED
{OPCODE_JUMP_ABSOLUTE, "JUMP_ABSOLUTE", "Set byte code counter to /target/.", 1, 1},	// SUPPORTED
{OPCODE_POP_JUMP_IF_FALSE, "POP_JUMP_IF_FALSE", "If TOS is false, sets the bytecode counter to target. TOS is popped.", 1, 1},	// SUPPORTED
{OPCODE_POP_JUMP_IF_TRUE, "POP_JUMP_IF_TRUE", "If TOS is true, sets the bytecode counter to target. TOS is popped.", 1, 1},	// SUPPORTED
{OPCODE_LOAD_GLOBAL, "LOAD_GLOBAL", "Loads the global named co_names[/namei/] onto the stack.", 1, 1},	// SUPPORTED
{OPCODE_SETUP_LOOP, "SETUP_LOOP", "Pushes a block for a loop onto the block stack.\n\
The block spans from the current instruction with a size of /delta/ bytes.", 1, 1},	// SUPPORTED
{OPCODE_LOAD_FAST, "LOAD_FAST", "Pushes a reference to the local co_varnames[/var_num/] onto the stack.", 1, 1},	// SUPPORTED
{OPCODE_STORE_FAST, "STORE_FAST", "Stores TOS into the local co_varnames[/var_num/].", 1, 1},	// SUPPORTED
{OPCODE_DELETE_FAST, "DELETE_FAST", "Deletes local co_varnames[/var_num/].", 1, 1},	// SUPPORTED
{OPCODE_INPLACE_LSHIFT, "INPLACE_LSHIFT", "Implements in-place TOS = TOS1 << TOS.", 0, 1},	// SUPPORTED
{OPCODE_INPLACE_RSHIFT, "INPLACE_RSHIFT", "Implements in-place TOS = TOS1 >> TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_LSHIFT, "BINARY_LSHIFT", "Implements TOS = TOS1 << TOS.", 0, 1},	// SUPPORTED
{OPCODE_BINARY_RSHIFT, "BINARY_RSHIFT", "Implements TOS = TOS1 >> TOS.", 0, 1},	// SUPPORTED
{OPCODE_EXTENDED_ARG, "EXTENDED_ARG", "Support for opargs more than 16 bits long.", 1, 1},	// SUPPORTED
{OPCODE_CALL_FUNCTION, "CALL_FUNCTION", "Calls a function. The low byte of /argc/ indicates the number of positional parameters,\n\
the high byte the number of keyword parameters. On the stack, the opcode finds the keyword parameters first.\n\
For each keyword argument, the value is on top of the key. Below the keyword parameters, the positional parameters are on the stack,\n\
with the right-most parameter on top. Below the parameters, the function object to call is on the stack.", 1, 1},	// SUPPORTED
{OPCODE_MAKE_FUNCTION, "MAKE_FUNCTION", "Pushes a new function object on the stack. TOS is the code associated with the function.\n\
The function object is defined to have /argc/ default parameters, which are found below TOS.", 1, 1},	// SUPPORTED
{OPCODE_BUILD_SLICE, "BUILD_SLICE", "Pushes a slice object on the stack. /argc/ must be 2 or 3. If it is 2, slice(TOS1, TOS) is pushed;\n\
if it is 3, slice(TOS2, TOS1, TOS) is pushed. See the slice() built-in function for more information.", 1, 0},
{OPCODE_LOAD_DEREF, "LOAD_DEREF", "Loads the cell contained in slot /i/ of the cell and free variable storage.\n\
Pushes a reference to the object the cell contains on the stack.", 1, 1},
{OPCODE_STORE_DEREF, "STORE_DEREF", "Stores TOS into the cell contained in slot /i/ of the cell and free variable storage.", 1, 1},
{OPCODE_DELETE_DEREF, "DELETE_DEREF", "Empties the cell contained in slot i of the cell and free variable storage. Used by the del statement.", 1, 1},
{OPCODE_CALL_FUNCTION_VAR, "CALL_FUNCTION_VAR", "Calls a function. /argc/ is interpreted as in CALL_FUNCTION.\n\
The top element on the stack contains the variable argument list, followed by keyword and positional arguments.", 1, 1}, // SUPPORTED
{OPCODE_CALL_FUNCTION_KW, "CALL_FUNCTION_KW", "Calls a function. /argc/ is interpreted as in CALL_FUNCTION.\n\
The top element on the stack contains the keyword arguments dictionary, followed by explicit keyword and positional arguments.", 1, 1}, // SUPPORTED
{OPCODE_CALL_FUNCTION_VAR_KW, "CALL_FUNCTION_VAR_KW", "Calls a function. /argc/ is interpreted as in CALL_FUNCTION.\n\
The top element on the stack contains the keyword arguments dictionary, followed by the variable-arguments tuple,\n\
followed by explicit keyword and positional arguments.", 1, 1}, // SUPPORTED
{OPCODE_END_FINALLY, "END_FINALLY", "Terminates a finally clause. The interpreter recalls whether the exception has to be re-raised,\n\
or whether the function returns, and continues with the outer-next block.", 0, 1},
{OPCODE_LOAD_BUILD_CLASS, "LOAD_BUILD_CLASS", "Pushes builtins.__build_class__() onto the stack. It is later called by CALL_FUNCTION to construct a class.", 0, 1},
{OPCODE_POP_EXCEPT, "POP_EXCEPT", "No Description.", 0, 0},
{OPCODE_RAISE_VARARGS, "RAISE_VARARGS", "Raises an exception. /argc/ indicates the number of parameters to the raise statement,\n\
ranging from 0 to 3. The handler will find the traceback as TOS2, the parameter as TOS1, and the exception as TOS.", 1, 1},
{OPCODE_IMPORT_FROM, "IMPORT_FROM", "Loads the attribute co_names[/namei/] from the module found in TOS.\n\
The resulting object is pushed onto the stack, to be subsequently stored by a STORE_FAST instruction.", 1, 1},
{OPCODE_IMPORT_STAR, "IMPORT_STAR", "Loads all symbols not starting with \"_\" directly from the module TOS to the local namespace.\n\
The module is popped after loading all names. This opcode implements from module import *.", 0, 1},
{OPCODE_YIELD_VALUE, "YIELD_VALUE", "Pops TOS and yields it from a generator.", 0, 1},
{OPCODE_STORE_LOCALS, "STORE_LOCALS", "Pops TOS from the stack and stores it as the current frame’s f_locals. This is used in class construction.", 0, 1},
{OPCODE_STORE_MAP, "STORE_MAP", "Store a key and value pair in a dictionary. Pops the key and value while leaving the dictionary on the stack.", 0, 1},
{OPCODE_STORE_ATTR, "STORE_ATTR", "Implements TOS.name = TOS1, where /namei/ is the index of name in co_names.", 1, 1},
{OPCODE_DELETE_ATTR, "DELETE_ATTR", "Implements del TOS.name, using /namei/ as index into co_names.", 1, 1},
{OPCODE_SETUP_EXCEPT, "SETUP_EXCEPT", "Pushes a try block from a try-except clause onto the block stack. /delta/ points to the first except block.", 1, 0},
{OPCODE_SETUP_FINALLY, "SETUP_FINALLY", "Pushes a try block from a try-except clause onto the block stack. /delta/ points to the finally block.", 1, 0},
{OPCODE_WITH_CLEANUP, "WITH_CLEANUP", "Cleans up the stack when a with statement block exits.\n\
TOS is the context manager’s __exit__() bound method. Below TOS are 1–3 values\n\
indicating how/why the finally clause was entered:\n\
SECOND = None\n\
(SECOND, THIRD) = (WHY_{RETURN,CONTINUE}), retval\n\
SECOND = WHY_*; no retval below it\n\
(SECOND, THIRD, FOURTH) = exc_info()\n\
In the last case, TOS(SECOND, THIRD, FOURTH) is called, otherwise TOS(None, None, None). In addition, TOS is removed from the stack.\n\
If the stack represents an exception, and the function call returns a ‘true’ value, this information is “zapped”\n\
and replaced with a single WHY_SILENCED to prevent END_FINALLY from re-raising the exception. (But non-local gotos will still be resumed.)", 0, 1},
{OPCODE_BUILD_MAP, "BUILD_MAP", "Pushes a new empty dictionary object onto the stack.\n\
The argument is ignored and set to /zero/ by the compiler.", 1, 1}, // SUPPORTED
{OPCODE_BUILD_SET, "BUILD_SET", "Pushes a new empty set object onto the stack.\n\
The argument is ignored and set to /zero/ by the compiler.", 1, 1},
{OPCODE_MAKE_CLOSURE, "MAKE_CLOSURE", "Creates a new function object, sets its func_closure slot, and pushes it on the stack.\n\
TOS is the code associated with the function. If the code object has N free variables, the next N items on the stack are the cells\n\
for these variables. The function also has /argc/ default parameters, where are found before the cells.", 1, 1},
{OPCODE_LOAD_CLOSURE, "LOAD_CLOSURE", "Pushes a reference to the cell contained in slot /i/ of the cell and free variable storage.\n\
The name of the variable is co_cellvars[i] if i is less than the length of co_cellvars. Otherwise it is co_freevars[i - len(co_cellvars)].", 1, 1},
{OPCODE_LIST_APPEND,"LIST_APPEND", "Calls list.append(TOS[-i], TOS). Used to implement list comprehensions.", 1, 1},
{OPCODE_SET_ADD, "SET_ADD","Calls set.add(TOS1[-i], TOS). Used to implement set comprehensions.", 1, 1},
{OPCODE_MAP_ADD, "MAP_ADD","Calls dict.setitem(TOS1[-i], TOS, TOS1). Used to implement dict comprehensions.", 1, 1},
{OPCODE_IMPORT_NAME, "IMPORT_NAME","Imports the module co_names[namei]. TOS and TOS1 are popped\n\
and provide the fromlist and level arguments of __import__(). The module object is pushed onto the stack.\n\
The current namespace is not affected: for a proper import statement,\n\
a subsequent STORE_FAST instruction modifies the namespace.", 1, 1},
{OPCODE_UNPACK_EX, "UNPACK_EX","Implements assignment with a starred target: Unpacks an iterable in TOS into individual values,\n\
where the total number of values can be smaller than the number of items in the iterable:\n\
one the new values will be a list of all leftover items.The low byte of counts is the number of values\n\
before the list value, the high byte of counts the number of values after it. The resulting values are put onto the stack right-to-left.", 1, 0},
{OPCODE_LOAD_ATTR, "LOAD_ATTR", "Replaces TOS with getattr(TOS, co_names[/namei/]).", 1, 1},
{OPCODE_SETUP_WITH, "SETUP_WITH", "This opcode performs several operations before a with block starts.\n\
 First, it loads __exit__() from the context manager and pushes it onto the stack for later use by WITH_CLEANUP.\n\
 Then, __enter__() is called, and a finally block pointing to delta is pushed. Finally, the result of calling the enter method is pushed onto the stack.\n\
 The next opcode will either ignore it (POP_TOP), or store it in (a) variable(s) (STORE_FAST, STORE_NAME, or UNPACK_SEQUENCE).", 1, 1}
};

void DumpUnsupportedOpCodes(void)
{
	printf("unsupported opcodes(%d of %d):\n",
		   opcodecount - GetSupportedOpcodesNum(), opcodecount);
	for (NUM i = 0; i < opcodecount; i++)
		if (!opcodes[i].supported)
		{
	if((debug_level & DEBUG_FULL_DUMP) > 0)
			debug_printf(DEBUG_FULL_DUMP,"[%3d,%xh] opcode: [ %s ]\n%s\n\n", i, opcodes[i].opcode,opcodes[i].name,opcodes[i].description);
		else
			debug_printf(DEBUG_ALL,"[%3d,%xh] opcode: [ %s ]\n", i, opcodes[i].opcode,opcodes[i].name);

		
		}
}

NUM GetSupportedOpcodesNum(void)
{
	unsigned int r = 0;

	for (NUM i = 0; i < opcodecount; i++)
		if (opcodes[i].supported)
			r++;
	return (r);
}

INDEX GetOpcodeIndex(unsigned char opcode)
{
	int r = -1;

	// int i = 0;
	for (NUM i = 0; i < opcodecount; i++)
	{
		if (opcode == opcodes[i].opcode)
		{
			r = i;
			break;
		}
	}
	return (r);
}

#else

#include "avr/pgmspace.h"

const opcode opcodes[] PROGMEM = {
{OPCODE_STOP_CODE, "STOP_CODE", 0},	// SUPPORTED
{OPCODE_POP_TOP, "POP_TOP", 0},	// SUPPORTED
{OPCODE_ROT_TWO, "ROT_TWO",  0},	// SUPPORTED
{OPCODE_ROT_THREE, "ROT_THREE",  0},	// SUPPORTED
{OPCODE_DUP_TOP, "DUP_TOP", 1},	// SUPPORTED
{OPCODE_DUP_TOP_TWO,  0},	// SUPPORTED
{OPCODE_NOP, "NOP",  0},	// SUPPORTED
{OPCODE_UNARY_POSITIVE, "UNARY_POSITIVE",  0},	// SUPPORTED
{OPCODE_UNARY_NEGATIVE, "UNARY_NEGATIVE", 0},	// SUPPORTED
{OPCODE_UNARY_NOT, "UNARY_NOT",  0},	// SUPPORTED
{OPCODE_UNARY_INVERT, "UNARY_INVERT", 0},	// SUPPORTED
{OPCODE_BINARY_POWER, "BINARY_POWER",  0},	// SUPPORTED
{OPCODE_BINARY_MULTIPLY, "BINARY_MULTIPLY",0},	// SUPPORTED
{OPCODE_BINARY_DIVIDE, "BINARY_DIVIDE",  0},	// SUPPORTED
{OPCODE_BINARY_MODULO, "BINARY_MODULO",  0},	// SUPPORTED
{OPCODE_BINARY_ADD, "BINARY_ADD",  0},	// SUPPORTED
{OPCODE_BINARY_SUBTRACT, "BINARY_SUBTRACT",  0},	// SUPPORTED
{OPCODE_BINARY_SUBSCR, "BINARY_SUBSCR",  0},	// SUPPORTED
{OPCODE_BINARY_FLOOR_DIVIDE, "BINARY_FLOOR_DIVIDE",  0},	// SUPPORTED
{OPCODE_BINARY_TRUE_DIVIDE, "BINARY_TRUE_DIVIDE", 0},	// SUPPORTED
{OPCODE_INPLACE_FLOOR_DIVIDE, "INPLACE_FLOOR_DIVIDE", 0},	// SUPPORTED
{OPCODE_INPLACE_TRUE_DIVIDE, "INPLACE_TRUE_DIVIDE", 0},	// SUPPORTED
{OPCODE_INPLACE_ADD, "INPLACE_ADD",  0},	// SUPPORTED
{OPCODE_INPLACE_SUBTRACT, "INPLACE_SUBTRACT", 0},	// SUPPORTED
{OPCODE_INPLACE_MULTIPLY, "INPLACE_MULTIPLY", 0},	// SUPPORTED
{OPCODE_INPLACE_DIVIDE, "INPLACE_DIVIDE",  0},	// SUPPORTED
{OPCODE_INPLACE_MODULO, "INPLACE_MODULO",  0},	// SUPPORTED
{OPCODE_STORE_SUBSCR, "STORE_SUBSCR", 0},	// SUPPORTED
{OPCODE_DELETE_SUBSCR, "DELETE_SUBSCR", 0},	// SUPPORTED
{OPCODE_BINARY_AND, "BINARY_AND",  0},	// SUPPORTED
{OPCODE_BINARY_XOR, "BINARY_XOR",  0},	// SUPPORTED
{OPCODE_BINARY_OR, "BINARY_OR", 0},	// SUPPORTED
{OPCODE_INPLACE_POWER, "INPLACE_POWER",  0},	// SUPPORTED
{OPCODE_GET_ITER, "GET_ITER", 0},	// SUPPORTED
{OPCODE_PRINT_EXPR, "PRINT_EXPR", 0},	// SUPPORTED 
{OPCODE_INPLACE_AND, "INPLACE_AND",  0},	// SUPPORTED
{OPCODE_INPLACE_XOR, "INPLACE_XOR", 0},	// SUPPORTED
{OPCODE_INPLACE_OR, "INPLACE_OR",0},	// SUPPORTED
{OPCODE_BREAK_LOOP, "BREAK_LOOP", 0},	// SUPPORTED
{OPCODE_RETURN_VALUE, "RETURN_VALUE",0},	// SUPPORTED
{OPCODE_POP_BLOCK, "POP_BLOCK",  0},	// SUPPORTED
{OPCODE_STORE_NAME, "STORE_NAME", 1},	// SUPPORTED
{OPCODE_DELETE_NAME, "DELETE_NAME",  1},	// SUPPORTED
{OPCODE_UNPACK_SEQUENCE, "UNPACK_SEQUENCE", 1},	// SUPPORTED
{OPCODE_STORE_GLOBAL, "STORE_GLOBAL", 1},	// SUPPORTED
{OPCODE_DELETE_GLOBAL, "DELETE_GLOBAL", 1},	// SUPPORTED 																											// 
{OPCODE_LOAD_CONST, "LOAD_CONST", 1},	// SUPPORTED
{OPCODE_LOAD_NAME, "LOAD_NAME",  1},	// SUPPORTED
{OPCODE_BUILD_TUPLE, "BUILD_TUPLE", 1},	// SUPPORTED
{OPCODE_BUILD_LIST, "BUILD_LIST", 1},	// SUPPORTED
{OPCODE_COMPARE_OP, "COMPARE_OP",  1},	// SUPPORTED 
{OPCODE_JUMP_FORWARD, "JUMP_FORWARD",  1},	// SUPPORTED
{OPCODE_JUMP_IF_FALSE, "JUMP_IF_FALSE",  1},	// SUPPORTED
{OPCODE_JUMP_IF_TRUE, "JUMP_IF_TRUE",  1},	// SUPPORTED
{OPCODE_JUMP_ABSOLUTE, "JUMP_ABSOLUTE", 1},	// SUPPORTED
{OPCODE_POP_JUMP_IF_FALSE, "POP_JUMP_IF_FALSE", 1},	// SUPPORTED
{OPCODE_POP_JUMP_IF_TRUE, "POP_JUMP_IF_TRUE", 1},	// SUPPORTED
{OPCODE_LOAD_GLOBAL, "LOAD_GLOBAL",  1},	// SUPPORTED
{OPCODE_SETUP_LOOP, "SETUP_LOOP",  1},	// SUPPORTED
{OPCODE_LOAD_FAST, "LOAD_FAST",  1},	// SUPPORTED
{OPCODE_STORE_FAST, "STORE_FAST", 1},	// SUPPORTED
{OPCODE_DELETE_FAST, "DELETE_FAST",  1},	// SUPPORTED
{OPCODE_INPLACE_LSHIFT, "INPLACE_LSHIFT", 0},	// SUPPORTED
{OPCODE_INPLACE_RSHIFT, "INPLACE_RSHIFT",  0},	// SUPPORTED
{OPCODE_BINARY_LSHIFT, "BINARY_LSHIFT",  0},	// SUPPORTED
{OPCODE_BINARY_RSHIFT, "BINARY_RSHIFT", 0},	// SUPPORTED
{OPCODE_EXTENDED_ARG, "EXTENDED_ARG",  1},	// SUPPORTED
{OPCODE_CALL_FUNCTION, "CALL_FUNCTION",1},	// SUPPORTED
{OPCODE_MAKE_FUNCTION, "MAKE_FUNCTION",  1},	// SUPPORTED
{OPCODE_BUILD_SLICE, "BUILD_SLICE",  1},
{OPCODE_LOAD_DEREF, "LOAD_DEREF",  1},
{OPCODE_STORE_DEREF, "STORE_DEREF",  1},
{OPCODE_DELETE_DEREF, "DELETE_DEREF",  1},
{OPCODE_CALL_FUNCTION_VAR, "CALL_FUNCTION_VAR",  1}, // SUPPORTED
{OPCODE_CALL_FUNCTION_KW, "CALL_FUNCTION_KW",  1}, // SUPPORTED
{OPCODE_CALL_FUNCTION_VAR_KW, "CALL_FUNCTION_VAR_KW", 1}, // SUPPORTED
{OPCODE_END_FINALLY, "END_FINALLY", 0},
{OPCODE_LOAD_BUILD_CLASS, "LOAD_BUILD_CLASS",  0},
{OPCODE_BUILD_CLASS, "BUILD_CLASS",  0},
{OPCODE_RAISE_VARARGS, "RAISE_VARARGS",  1},
{OPCODE_IMPORT_FROM, "IMPORT_FROM", 1},
{OPCODE_IMPORT_STAR, "IMPORT_STAR",  0},
{OPCODE_YIELD_VALUE, "YIELD_VALUE", 0},
{OPCODE_STORE_LOCALS, "STORE_LOCALS", 0},
{OPCODE_STORE_MAP, "STORE_MAP", 0},
{OPCODE_STORE_ATTR, "STORE_ATTR", 1},
{OPCODE_DELETE_ATTR, "DELETE_ATTR",1},
{OPCODE_SETUP_EXCEPT, "SETUP_EXCEPT",  1},
{OPCODE_SETUP_FINALLY, "SETUP_FINALLY",  1},
{OPCODE_WITH_CLEANUP, "WITH_CLEANUP", 0},
{OPCODE_BUILD_MAP, "BUILD_MAP", 1}, // SUPPORTED
{OPCODE_BUILD_SET, "BUILD_SET", 1},
{OPCODE_MAKE_CLOSURE, "MAKE_CLOSURE",1},
{OPCODE_LOAD_CLOSURE, "LOAD_CLOSURE", 1},
{OPCODE_LIST_APPEND,"LIST_APPEND",1},
{OPCODE_SET_ADD, "SET_ADD", 1},
{OPCODE_MAP_ADD, "MAP_ADD", 1},
{OPCODE_IMPORT_NAME, "IMPORT_NAME",1},
{OPCODE_UNPACK_EX, "UNPACK_EX",1}
};

INDEX GetOpcodeIndex(unsigned char opcode)
{
	int r = -1;

	// int i = 0;
	for (NUM i = 0; i < opcodecount; i++)
	{
		unsigned char op = pgm_read_byte(&opcodes[i]);
		if (opcode == op)
		{
			r = i;
			break;
		}
	}
	return (r);
}

#endif


#endif
