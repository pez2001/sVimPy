#define DEBUG
//#define OPCODES_C
#include "opcodes.h"

const unsigned int opcodecount = 95;
#ifdef DEBUG

const opcode opcodes[]={{0x00,"STOP_CODE","Indicates end-of-code to the compiler, not used by the interpreter.",0,1}, 													//SUPPORTED
                            {0x01,"POP_TOP","Removes the top-of-stack (TOS) item.",0,1},																										//SUPPORTED
                            {0x02,"ROT_TWO","Swaps the two top-most stack items.",0,1},																										//SUPPORTED
                            {0x03,"ROT_THREE","Lifts second and third stack item one position up, moves top down to position three.",0,1},									//SUPPORTED
                            {0x04,"DUP_TOP","Duplicates the reference on top of the stack.",0,1},																								//SUPPORTED
                            {0x05,"DUP_TOP_TWO","Duplicates the reference on top of the stack.",0,1},																						//SUPPORTED
//                            {0x05,"ROT_FOUR","Lifts second, third and forth stack item one position up, moves top down to position four.",0}, 								//DEPRECATED
                            {0x09,"NOP","Do nothing code. Used as a placeholder by the bytecode optimizer.",0,1},																		//SUPPORTED
                            {0x0a,"UNARY_POSITIVE","Implements TOS = +TOS.",0,1},																											//SUPPORTED
                            {0x0b,"UNARY_NEGATIVE","Implements TOS = -TOS.",0,1},																											//SUPPORTED
                            {0x0c,"UNARY_NOT","Implements TOS = not TOS.",0,1},																													//SUPPORTED
//                            {0x0d,"UNARY_CONVERT","Implements TOS = `TOS`.",0},																											//DEPRECATED
                            {0x0f,"UNARY_INVERT","Implements TOS = ~TOS.",0,1},																												//SUPPORTED
//                            {0x12,"LIST_APPEND","Calls list.append(TOS1, TOS). Used to implement list comprehensions.",0},													//DEPRECATED
                            {0x13,"BINARY_POWER","Implements TOS = TOS1 ** TOS.",0,1},																									//SUPPORTED
                            {0x14,"BINARY_MULTIPLY","Implements TOS = TOS1 * TOS.",0,1},																									//SUPPORTED
                            {0x15,"BINARY_DIVIDE","Implements TOS = TOS1 / TOS when from __future__ import division is not in effect.",0,1},							//SUPPORTED
                            {0x16,"BINARY_MODULO","Implements TOS = TOS1 % TOS.",0,1},																									//SUPPORTED
                            {0x17,"BINARY_ADD","Implements TOS = TOS1 + TOS.",0,1},																											//SUPPORTED
                            {0x18,"BINARY_SUBTRACT","Implements TOS = TOS1 - TOS.",0,1},																								//SUPPORTED
                            {0x19,"BINARY_SUBSCR","Implements TOS = TOS1[TOS].",0,1},																									//SUPPORTED
                            {0x1a,"BINARY_FLOOR_DIVIDE","Implements TOS = TOS1 // TOS.",0,1},																						//SUPPORTED
                            {0x1b,"BINARY_TRUE_DIVIDE","Implements TOS = TOS1 / TOS when from __future__ import division is in effect.",0,1},						//SUPPORTED
                            {0x1c,"INPLACE_FLOOR_DIVIDE","Implements in-place TOS = TOS1 // TOS.",0,1},																			//SUPPORTED
                            {0x1d,"INPLACE_TRUE_DIVIDE","Implements in-place TOS = TOS1 / TOS when from __future__ import division is in effect.",0,1},			//SUPPORTED
//                            {0x1e,"SLICE","Implements TOS = TOS[:].",0},																																//DEPRECATED
//                            {0x1f,"SLICE+1","Implements TOS = TOS1[TOS:].",0},																													//DEPRECATED
//                            {0x20,"SLICE+2","Implements TOS = TOS1[:TOS].",0},																													//DEPRECATED
//                            {0x21,"SLICE+3","Implements TOS = TOS2[TOS1:TOS].",0},																											//DEPRECATED
//                            {0x28,"STORE_SLICE","Implements TOS[:] = TOS1.",0},																												//DEPRECATED
//                            {0x29,"STORE_SLICE+1","Implements TOS1[TOS:] = TOS2.",0},																									//DEPRECATED
//                            {0x2a,"STORE_SLICE+2","Implements TOS1[:TOS] = TOS2.",0},																									//DEPRECATED
//                            {0x2b,"STORE_SLICE+3","Implements TOS2[TOS1:TOS] = TOS3.",0},																							//DEPRECATED
//                            {0x32,"DELETE_SLICE","Implements del TOS[:].",0},																														//DEPRECATED
//                            {0x33,"DELETE_SLICE+1","Implements del TOS1[TOS:].",0},																											//DEPRECATED
//                            {0x34,"DELETE_SLICE+2","Implements del TOS1[:TOS].",0},																											//DEPRECATED
//                            {0x35,"DELETE_SLICE+3","Implements del TOS2[TOS1:TOS].",0},																									//DEPRECATED
                            {0x36,"STORE_MAP","No description.",0,0},
                            {0x37,"INPLACE_ADD","Implements in-place TOS = TOS1 + TOS.",0,1},																							//SUPPORTED
                            {0x38,"INPLACE_SUBTRACT","Implements in-place TOS = TOS1 - TOS.",0,1},																					//SUPPORTED
                            {0x39,"INPLACE_MULTIPLY","Implements in-place TOS = TOS1 * TOS.",0,1},																					//SUPPORTED
                            {0x3a,"INPLACE_DIVIDE","Implements in-place TOS = TOS1 / TOS when from __future__ import division is not in effect.",0,1},				//SUPPORTED
                            {0x3b,"INPLACE_MODULO","Implements in-place TOS = TOS1 % TOS.",0,1},																					//SUPPORTED
                            {0x3c,"STORE_SUBSCR","Implements TOS1[TOS] = TOS2.",0,1},																									//SUPPORTED
                            {0x3d,"DELETE_SUBSCR","Implements del TOS1[TOS].",0,0},
                            {0x3e,"BINARY_LSHIFT","Implements TOS = TOS1 << TOS.",0,0},
                            {0x3f,"BINARY_RSHIFT","Implements TOS = TOS1 >> TOS.",0,0},
                            {0x40,"BINARY_AND","Implements TOS = TOS1 & TOS.",0,1},																											//SUPPORTED
                            {0x41,"BINARY_XOR","Implements TOS = TOS1 ^ TOS.",0,1},																											//SUPPORTED
                            {0x42,"BINARY_OR","Implements TOS = TOS1 | TOS.",0,1},																											//SUPPORTED
                            {0x43,"INPLACE_POWER","Implements in-place TOS = TOS1 ** TOS.",0,1},																						//SUPPORTED
                            {0x44,"GET_ITER","Implements TOS = iter(TOS).",0,1},																													//SUPPORTED
                            {0x45,"STORE_LOCALS","No description.",0,0},
                            {0x47,"PRINT_EXPR","Implements the expression statement for the interactive mode.\
							TOS is removed from the stack and printed. In non-interactive mode, an expression statement is terminated with POP_STACK.",0,1}, 	//SUPPORTED (interactive part not supported yet)
//                            {0x47,"PRINT_ITEM","Prints TOS to the file-like object bound to sys.stdout.\
// There is one such instruction for each item in the print statement.",0}, 																														//DEPRECATED
                            {0x48,"LOAD_BUILD_CLASS","No description.",0,0},
//                            {0x48,"PRINT_NEWLINE","Prints a new line on sys.stdout.\
// This is generated as the last operation of a print statement, unless the statement ends with a comma.",0},																	//DEPRECATED
//                            {0x49,"PRINT_ITEM_TO","Like PRINT_ITEM, but prints the item second from TOS to the file-like object at TOS. This is used by the extended print statement.",0},
//                            {0x4a,"PRINT_NEWLINE_TO","Like PRINT_NEWLINE, but prints the new line on the file-like object on the TOS. This is used by the extended print statement.",0},
                            {0x4b,"INPLACE_LSHIFT","Implements in-place TOS = TOS1 << TOS.",0,0},
                            {0x4c,"INPLACE_RSHIFT","Implements in-place TOS = TOS1 >> TOS.",0,0},
                            {0x4d,"INPLACE_AND","Implements in-place TOS = TOS1 & TOS.",0,1},																							//SUPPORTED
                            {0x4e,"INPLACE_XOR","Implements in-place TOS = TOS1 ^ TOS.",0,1},																							//SUPPORTED
                            {0x4f,"INPLACE_OR","Implements in-place TOS = TOS1 | TOS.",0,1},																								//SUPPORTED
                            {0x50,"BREAK_LOOP","Terminates a loop due to a break statement.",0,1},																						//SUPPORTED
                            {0x52,"WITH_CLEANUP","No description.",0,0},
//                            {0x52,"LOAD_LOCALS","Pushes a reference to the locals of the current scope on the stack.\
// This is used in the code for a class definition: After the class body is evaluated, the locals are passed to the class definition.",0},									//DEPRECATED
                            {0x53,"RETURN_VALUE","Returns with TOS to the caller of the function.",0,1},																					//SUPPORTED
                            {0x54,"IMPORT_STAR","Loads all symbols not starting with \"_\" directly from the module TOS to the local namespace.\
							The module is popped after loading all names. This opcode implements from module import *.",0,0},
//                            {0x55,"EXEC_STMT","Implements exec TOS2,TOS1,TOS. The compiler fills missing optional parameters with None.",0},					//DEPRECATED AND WILL NEVER BE SUPPORTED (BLOATING)
                            {0x56,"YIELD_VALUE","Pops TOS and yields it from a generator.",0,0},
                            {0x57,"POP_BLOCK","Removes one block from the block stack.\
							Per frame, there is a stack of blocks, denoting nested loops, try statements, and such.",0,1},																//SUPPORTED
                            {0x58,"END_FINALLY","Terminates a finally clause. The interpreter recalls whether the exception has to be re-raised,\
							or whether the function returns, and continues with the outer-next block.",0,0},
                            {0x59,"BUILD_CLASS","Creates a new class object. TOS is the methods dictionary,\
							TOS1 the tuple of the names of the base classes, and TOS2 the class name.",0,0},
                            {0x5a,"STORE_NAME","Implements name = TOS. /namei/ is the index of name in the attribute co_names of the code object.\
							The compiler tries to use STORE_LOCAL or STORE_GLOBAL if possible.",2,1},																				//SUPPORTED
                            {0x5b,"DELETE_NAME","Implements del name, where /namei/ is the index into co_names attribute of the code object.",2,0},
                            {0x5c,"UNPACK_SEQUENCE","Unpacks TOS into /count/ individual values, which are put onto the stack right-to-left.",2,0},
                            {0x5d,"FOR_ITER","TOS is an iterator. Call its next() method. If this yields a new value, push it on the stack (leaving the iterator below it).\
							If the iterator indicates it is exhausted TOS is popped, and the byte code counter is incremented by /delta/.",2,1},									//SUPPORTED
                            {0x5f,"STORE_ATTR","Implements TOS.name = TOS1, where /namei/ is the index of name in co_names.",2,0},
                            {0x60,"DELETE_ATTR","Implements del TOS.name, using /namei/ as index into co_names.",2,0},
                            {0x61,"STORE_GLOBAL","Works as STORE_NAME(/namei/), but stores the name as a global.",2,1},													//SUPPORTED
                            {0x62,"DELETE_GLOBAL","Works as DELETE_NAME(/namei/), but deletes a global name.",2,0},												
                            {0x63,"DUP_TOPX","Duplicate /count/ items, keeping them in the same order.\
							Due to implementation limits, count should be between 1 and 5 inclusive.",0,0},
                            {0x64,"LOAD_CONST","Pushes \"co_consts[/consti/]\" onto the stack.",2,1},																						//SUPPORTED
                            {0x65,"LOAD_NAME","Pushes the value associated with \"co_names[/namei/]\" onto the stack.",2,1},													//SUPPORTED
                            {0x66,"BUILD_TUPLE","Creates a tuple consuming /count/ items from the stack, and pushes the resulting tuple onto the stack.",2,0},
                            {0x67,"BUILD_LIST","Works as BUILD_TUPLE(/count/), but creates a list.",2,1},																				//SUPPORTED
                            {0x68,"BUILD_MAP","Pushes a new empty dictionary object onto the stack.\
							The argument is ignored and set to /zero/ by the compiler.",2,0},
                            {0x69,"LOAD_ATTR","Replaces TOS with getattr(TOS, co_names[/namei/]).",2,0},
                            {0x6b,"COMPARE_OP","Performs a Boolean operation. The operation name can be found in cmp_op[/opname/].",2,1},							//SUPPORTED	some ops still missing
//							{0x6b,"IMPORT_NAME","Imports the module co_names[/namei/]. The module object is pushed onto the stack.\
// The current namespace is not affected: for a proper import statement, a subsequent STORE_FAST instruction modifies the namespace.",2},	
							{0x6c,"IMPORT_FROM","Loads the attribute co_names[/namei/] from the module found in TOS.\
							The resulting object is pushed onto the stack, to be subsequently stored by a STORE_FAST instruction.",2,0},
							{0x6e,"JUMP_FORWARD","Increments byte code counter by /delta/.",2,1},																						//SUPPORTED
							{0x6f,"JUMP_IF_FALSE","If TOS is false, increment the byte code counter by /delta/. TOS is not changed.",2,0},
							{0x70,"JUMP_IF_TRUE","If TOS is true, increment the byte code counter by /delta/. TOS is left on the stack.",2,0},
							{0x71,"JUMP_ABSOLUTE","Set byte code counter to /target/.",2,1},																									//SUPPORTED
							{0x72,"POP_JUMP_IF_FALSE","no description.",2,1},																														//SUPPORTED
							{0x73,"POP_JUMP_IF_TRUE","no description.",2,1},																														//SUPPORTED
							{0x74,"LOAD_GLOBAL","Loads the global named co_names[/namei/] onto the stack.",2,1},																//SUPPORTED
//							{0x77,"CONTINUE_LOOP","Continues a loop due to a continue statement. /target/ is the address\
//							to jump to (which should be a FOR_ITER instruction).",2,1},																												//DEPRECATED
							{0x78,"SETUP_LOOP","Pushes a block for a loop onto the block stack.\
							The block spans from the current instruction with a size of /delta/ bytes.",2,1},																					//SUPPORTED
							{0x79,"SETUP_EXCEPT","Pushes a try block from a try-except clause onto the block stack. /delta/ points to the first except block.",2,0},
							{0x7a,"SETUP_FINALLY","Pushes a try block from a try-except clause onto the block stack. /delta/ points to the finally block.",2,0},
							{0x7c,"LOAD_FAST","Pushes a reference to the local co_varnames[/var_num/] onto the stack.",2,1},													//SUPPORTED
							{0x7d,"STORE_FAST","Stores TOS into the local co_varnames[/var_num/].",2,1},																				//SUPPORTED
							{0x7e,"DELETE_FAST","Deletes local co_varnames[/var_num/].",2,0},
							{0x82,"RAISE_VARARGS","Raises an exception. /argc/ indicates the number of parameters to the raise statement,\
							ranging from 0 to 3. The handler will find the traceback as TOS2, the parameter as TOS1, and the exception as TOS.",2,0},
							{0x83,"CALL_FUNCTION","Calls a function. The low byte of /argc/ indicates the number of positional parameters,\
							the high byte the number of keyword parameters. On the stack, the opcode finds the keyword parameters first.\
							For each keyword argument, the value is on top of the key. Below the keyword parameters, the positional parameters are on the stack,\
							with the right-most parameter on top. Below the parameters, the function object to call is on the stack.",2,1},										//SUPPORTED
							{0x84,"MAKE_FUNCTION","Pushes a new function object on the stack. TOS is the code associated with the function.\
							The function object is defined to have /argc/ default parameters, which are found below TOS.",2,1},															//SKIPPED ATM
							{0x85,"BUILD_SLICE","Pushes a slice object on the stack. /argc/ must be 2 or 3. If it is 2, slice(TOS1, TOS) is pushed;\
							if it is 3, slice(TOS2, TOS1, TOS) is pushed. See the slice() built-in function for more information.",2,0},
							{0x86,"MAKE_CLOSURE","Creates a new function object, sets its func_closure slot, and pushes it on the stack.\
							TOS is the code associated with the function. If the code object has N free variables, the next N items on the stack are the cells\
							for these variables. The function also has /argc/ default parameters, where are found before the cells.",2,0},
							{0x87,"LOAD_CLOSURE","Pushes a reference to the cell contained in slot /i/ of the cell and free variable storage.\
							The name of the variable is co_cellvars[i] if i is less than the length of co_cellvars. Otherwise it is co_freevars[i - len(co_cellvars)].",2,0},
							{0x88,"LOAD_DEREF","Loads the cell contained in slot /i/ of the cell and free variable storage.\
							Pushes a reference to the object the cell contains on the stack.",2,0},
							{0x89,"STORE_DEREF","Stores TOS into the cell contained in slot /i/ of the cell and free variable storage.",2,0},
							{0x8c,"CALL_FUNCTION_VAR","Calls a function. /argc/ is interpreted as in CALL_FUNCTION.\
							The top element on the stack contains the variable argument list, followed by keyword and positional arguments.",2,0},
							{0x8d,"CALL_FUNCTION_KW","Calls a function. /argc/ is interpreted as in CALL_FUNCTION.\
							The top element on the stack contains the keyword arguments dictionary, followed by explicit keyword and positional arguments.",2},
							{0x8e,"CALL_FUNCTION_VAR_KW","Calls a function. /argc/ is interpreted as in CALL_FUNCTION.\
							The top element on the stack contains the keyword arguments dictionary, followed by the variable-arguments tuple,\
							followed by explicit keyword and positional arguments.",2,0},
							{0x8f,"EXTENDED_ARG","Support for opargs more than 16 bits long.",2,0}
                            };

void DumpUnsupportedOpCodes()
{
printf("unsupported opcodes(%d of %d):\n",opcodecount - GetSupportedOpcodesNum(),opcodecount);
for(int i=0;i<opcodecount;i++)
 if(!opcodes[i].supported)
	 printf("[%d,%xh] opcode: [ %s ]\n",i,opcodes[i].opcode,opcodes[i].name);
}

unsigned int GetSupportedOpcodesNum()
{
unsigned int r= 0;
for(int i=0;i<opcodecount;i++)
 if(opcodes[i].supported)
  r++;
return(r);
}

							

#else
const opcode opcodes[]={{0x5a},
                            {0x5b},
                            {0x5c},
                            {0x5d},
                            {0x5f},
                            {0x60},
                            {0x61},
                            {0x62},
                            {0x64},
                            {0x65},
                            {0x66},
                            {0x67},
                            {0x68},
                            {0x69},
							{0x6b},
							{0x6c},
							{0x6e},
							{0x6f},
							{0x70},
							{0x71},
							{0x74},
							{0x77},
							{0x78},
							{0x79},
							{0x7a},
							{0x7c},
							{0x7d},
							{0x7e},
							{0x82},
							{0x83},
							{0x84},
							{0x85},
							{0x86},
							{0x87},
							{0x88},
							{0x89},
							{0x8c},
							{0x8d},
							{0x8e},
							{0x8f}
                            };

#endif


int GetOpcodeIndex(unsigned char opcode)
{
int r = -1;
//int i = 0;
for(int i=0;i<opcodecount;i++)
{
 if(opcode == opcodes[i].opcode)
 {
  r= i;
  break;
 }
}
return(r);
}

