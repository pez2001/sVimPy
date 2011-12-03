#ifndef OPCODES_H
#define OPCODES_H

#define DEBUG


typedef struct {
unsigned char opcode;  //opcode in bytecode representation
#ifdef DEBUG
char *name;    //string version
char *description;    //a short description
#endif
unsigned int num_parameters; //number of parameters
} opcode;

#ifdef DEBUG
instruction instructions[]={{0x00,"STOP_CODE","Indicates end-of-code to the compiler, not used by the interpreter.",0},
                            {0x01,"POP_TOP","Removes the top-of-stack (TOS) item.",0},
                            {0x02,"ROT_TWO","Swaps the two top-most stack items.",0},
                            {0x03,"ROT_THREE","Lifts second and third stack item one position up, moves top down to position three.",0},
                            {0x04,"DUP_TOP","Duplicates the reference on top of the stack.",0},
                            {0x05,"ROT_FOUR","Lifts second, third and forth stack item one position up, moves top down to position four.",0},
                            {0x09,"NOP","Do nothing code. Used as a placeholder by the bytecode optimizer.",0},
                            {0x0a,"UNARY_POSITIVE","Implements TOS = +TOS.",0},
                            {0x0b,"UNARY_NEGATIVE","Implements TOS = -TOS.",0},
                            {0x0c,"UNARY_NOT","Implements TOS = not TOS.",0},
                            {0x0d,"UNARY_CONVERT","Implements TOS = `TOS`.",0},
                            {0x0f,"UNARY_INVERT","Implements TOS = ~TOS.",0},
                            {0x12,"LIST_APPEND","Calls list.append(TOS1, TOS). Used to implement list comprehensions.",0},
                            {0x13,"BINARY_POWER","Implements TOS = TOS1 ** TOS.",0},
                            {0x14,"BINARY_MULTIPLY","Implements TOS = TOS1 * TOS.",0},
                            {0x15,"BINARY_DIVIDE","Implements TOS = TOS1 / TOS when from __future__ import division is not in effect.",0},
                            {0x16,"BINARY_MODULO","Implements TOS = TOS1 % TOS.",0},
                            {0x17,"BINARY_ADD","Implements TOS = TOS1 + TOS.",0},
                            {0x18,"BINARY_SUBTRACT","Implements TOS = TOS1 - TOS.",0},
                            {0x19,"BINARY_SUBSCR","Implements TOS = TOS1[TOS].",0},
                            {0x1a,"BINARY_FLOOR_DIVIDE","Implements TOS = TOS1 // TOS.",0},
                            {0x1b,"BINARY_TRUE_DIVIDE","Implements TOS = TOS1 / TOS when from __future__ import division is in effect.",0},
                            {0x1c,"INPLACE_FLOOR_DIVIDE","Implements in-place TOS = TOS1 // TOS.",0},
                            {0x1d,"INPLACE_TRUE_DIVIDE","Implements in-place TOS = TOS1 / TOS when from __future__ import division is in effect.",0},
                            {0x1e,"SLICE","Implements TOS = TOS[:].",0},
                            {0x1f,"SLICE+1","Implements TOS = TOS1[TOS:].",0},
                            {0x20,"SLICE+2","Implements TOS = TOS1[:TOS].",0},
                            {0x21,"SLICE+3","Implements TOS = TOS2[TOS1:TOS].",0},
                            {0x28,"STORE_SLICE","Implements TOS[:] = TOS1.",0},
                            {0x29,"STORE_SLICE+1","Implements TOS1[TOS:] = TOS2.",0},
                            {0x2a,"STORE_SLICE+2","Implements TOS1[:TOS] = TOS2.",0},
                            {0x2b,"STORE_SLICE+3","Implements TOS2[TOS1:TOS] = TOS3.",0},
                            {0x32,"DELETE_SLICE","Implements del TOS[:].",0},
                            {0x33,"DELETE_SLICE+1","Implements del TOS1[TOS:].",0},
                            {0x34,"DELETE_SLICE+2","Implements del TOS1[:TOS].",0},
                            {0x35,"DELETE_SLICE+3","Implements del TOS2[TOS1:TOS].",0},
                            {0x37,"INPLACE_ADD","Implements in-place TOS = TOS1 + TOS.",0},
                            {0x38,"INPLACE_SUBTRACT","Implements in-place TOS = TOS1 - TOS.",0},
                            {0x39,"INPLACE_MULTIPLY","Implements in-place TOS = TOS1 * TOS.",0},
                            {0x3a,"INPLACE_DIVIDE","Implements in-place TOS = TOS1 / TOS when from __future__ import division is not in effect.",0},
                            {0x3b,"INPLACE_MODULO","Implements in-place TOS = TOS1 % TOS.",0},
                            {0x3c,"STORE_SUBSCR","Implements TOS1[TOS] = TOS2.",0},
                            {0x3d,"DELETE_SUBSCR","Implements del TOS1[TOS].",0},
                            {0x3e,"BINARY_LSHIFT","Implements TOS = TOS1 << TOS.",0},
                            {0x3f,"BINARY_RSHIFT","Implements TOS = TOS1 >> TOS.",0},
                            {0x40,"BINARY_AND","Implements TOS = TOS1 & TOS.",0},
                            {0x41,"BINARY_XOR","Implements TOS = TOS1 ^ TOS.",0},
                            {0x42,"BINARY_OR","Implements TOS = TOS1 | TOS.",0},
                            {0x43,"INPLACE_POWER","Implements in-place TOS = TOS1 ** TOS.",0},
                            {0x44,"GET_ITER","Implements TOS = iter(TOS).",0},
                            {0x46,"PRINT_EXPR","Implements the expression statement for the interactive mode. TOS is removed from the stack and printed. In non-interactive mode, an expression statement is terminated with POP_STACK.",0},
                            {0x47,"PRINT_ITEM","Prints TOS to the file-like object bound to sys.stdout. There is one such instruction for each item in the print statement.",0},
                            {0x48,"PRINT_NEWLINE","Prints a new line on sys.stdout. This is generated as the last operation of a print statement, unless the statement ends with a comma.",0},
                            {0x49,"PRINT_ITEM_TO","Like PRINT_ITEM, but prints the item second from TOS to the file-like object at TOS. This is used by the extended print statement.",0},
                            {0x4a,"PRINT_NEWLINE_TO","Like PRINT_NEWLINE, but prints the new line on the file-like object on the TOS. This is used by the extended print statement.",0},
                            {0x4b,"INPLACE_LSHIFT","Implements in-place TOS = TOS1 << TOS.",0},
                            {0x4c,"INPLACE_RSHIFT","Implements in-place TOS = TOS1 >> TOS.",0},
                            {0x4d,"INPLACE_AND","Implements in-place TOS = TOS1 & TOS.",0},
                            {0x4e,"INPLACE_XOR","Implements in-place TOS = TOS1 ^ TOS.",0},
                            {0x4f,"INPLACE_OR","Implements in-place TOS = TOS1 | TOS.",0},
                            {0x50,"BREAK_LOOP","Terminates a loop due to a break statement.",0},
                            {0x51,"WITH_CLEANUP","???",0},
                            {0x52,"LOAD_LOCALS","Pushes a reference to the locals of the current scope on the stack. This is used in the code for a class definition: After the class body is evaluated, the locals are passed to the class definition.",0},
                            {0x53,"RETURN_VALUE","Returns with TOS to the caller of the function.",0},
                            {0x54,"IMPORT_STAR","Loads all symbols not starting with "_" directly from the module TOS to the local namespace. The module is popped after loading all names. This opcode implements from module import *.",0},
                            {0x55,"EXEC_STMT","Implements exec TOS2,TOS1,TOS. The compiler fills missing optional parameters with None.",0},
                            {0x56,"YIELD_VALUE","Pops TOS and yields it from a generator.",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0},
                            {0x,"","",0}
                            };
#elseif
instruction instructions[]={{0x00,0},
                            {0x01,0},
                            {0x,0}
                            };

#endif



#endif