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

#ifndef OPCODES_H
#define OPCODES_H


#define OPCODE_STOP_CODE	 						0x00
#define OPCODE_POP_TOP								0x01
#define OPCODE_ROT_TWO							 	0x02
#define OPCODE_ROT_THREE							0x03
#define OPCODE_DUP_TOP 		 						0x04
#define OPCODE_DUP_TOP_TWO 					0x05
#define OPCODE_NOP 										0x09
#define OPCODE_UNARY_POSITIVE 				0x0a
#define OPCODE_UNARY_NEGATIVE				0x0b
#define OPCODE_UNARY_NOT 						0x0c
#define OPCODE_UNARY_INVERT 					0x0f
#define OPCODE_BINARY_POWER 					0x13
#define OPCODE_BINARY_MULTIPLY 				0x14
#define OPCODE_BINARY_DIVIDE 					0x15
#define OPCODE_BINARY_MODULO 				0x16
#define OPCODE_BINARY_ADD 						0x17
#define OPCODE_BINARY_SUBTRACT 				0x18
#define OPCODE_BINARY_SUBSCR 					0x19
#define OPCODE_BINARY_FLOOR_DIVIDE		0x1a
#define OPCODE_BINARY_TRUE_DIVIDE 			0x1b
#define OPCODE_INPLACE_FLOOR_DIVIDE	 	0x1c
#define OPCODE_INPLACE_TRUE_DIVIDE 		0x1d
#define OPCODE_STORE_MAP 						0x36
#define OPCODE_INPLACE_ADD 						0x37
#define OPCODE_INPLACE_SUBTRACT 			0x38
#define OPCODE_INPLACE_MULTIPLY 				0x39
#define OPCODE_INPLACE_DIVIDE 					0x3a
#define OPCODE_INPLACE_MODULO 				0x3b
#define OPCODE_STORE_SUBSCR 					0x3c
#define OPCODE_DELETE_SUBSCR 				0x3d
#define OPCODE_BINARY_LSHIFT 					0x3e
#define OPCODE_BINARY_RSHIFT 					0x3f
#define OPCODE_BINARY_AND 						0x40
#define OPCODE_BINARY_XOR							0x41
#define OPCODE_BINARY_OR 							0x42
#define OPCODE_INPLACE_POWER 				0x43
#define OPCODE_GET_ITER 								0x44
#define OPCODE_STORE_LOCALS 					0x45
#define OPCODE_PRINT_EXPR 						0x47
#define OPCODE_LOAD_BUILD_CLASS 			0x48
#define OPCODE_INPLACE_LSHIFT					0x4b
#define OPCODE_INPLACE_RSHIFT 					0x4c
#define OPCODE_INPLACE_AND						0x4d
#define OPCODE_INPLACE_XOR 						0x4e
#define OPCODE_INPLACE_OR 						0x4f
#define OPCODE_BREAK_LOOP 						0x50
#define OPCODE_WITH_CLEANUP 					0x52
#define OPCODE_RETURN_VALUE 					0x53
#define OPCODE_IMPORT_STAR						0x54
#define OPCODE_YIELD_VALUE 						0x56
#define OPCODE_POP_BLOCK 						0x57
#define OPCODE_END_FINALLY 						0x58
#define OPCODE_BUILD_CLASS						0x59
#define OPCODE_STORE_NAME 						0x5a
#define OPCODE_DELETE_NAME 					0x5b
#define OPCODE_UNPACK_SEQUENCE 			0x5c
#define OPCODE_FOR_ITER 							0x5d
#define OPCODE_STORE_ATTR 						0x5f
#define OPCODE_DELETE_ATTR 						0x60
#define OPCODE_STORE_GLOBAL 					0x61
#define OPCODE_DELETE_GLOBAL 					0x62
#define OPCODE_LOAD_CONST 						0x64
#define OPCODE_LOAD_NAME 						0x65
#define OPCODE_BUILD_TUPLE 						0x66
#define OPCODE_BUILD_LIST 							0x67
#define OPCODE_BUILD_MAP 							0x69
#define OPCODE_LOAD_ATTR 							0x99 //WRONG ID
#define OPCODE_COMPARE_OP						0x6b
#define OPCODE_IMPORT_FROM 					0x6c
#define OPCODE_JUMP_FORWARD 					0x6e
#define OPCODE_JUMP_IF_FALSE 					0x6f
#define OPCODE_JUMP_IF_TRUE 					0x70
#define OPCODE_JUMP_ABSOLUTE 				0x71
#define OPCODE_POP_JUMP_IF_FALSE 			0x72
#define OPCODE_POP_JUMP_IF_TRUE 			0x73
#define OPCODE_LOAD_GLOBAL 					0x74
#define OPCODE_SETUP_LOOP 						0x78
#define OPCODE_SETUP_EXCEPT 					0x79
#define OPCODE_SETUP_FINALLY 					0x7a
#define OPCODE_LOAD_FAST 							0x7c
#define OPCODE_STORE_FAST 						0x7d
#define OPCODE_DELETE_FAST 						0x7e
#define OPCODE_RAISE_VARARGS 				0x82
#define OPCODE_CALL_FUNCTION 					0x83
#define OPCODE_MAKE_FUNCTION 					0x84
#define OPCODE_BUILD_SLICE							0x85
//#define OPCODE_MAKE_CLOSURE 					0x86
//#define OPCODE_LOAD_CLOSURE 					0x87
#define OPCODE_LOAD_DEREF 						0x88
#define OPCODE_STORE_DEREF 					0x89
#define OPCODE_CALL_FUNCTION_VAR 			0x8c
#define OPCODE_CALL_FUNCTION_KW 			0x8d
#define OPCODE_CALL_FUNCTION_VAR_KW 	0x8e
#define OPCODE_EXTENDED_ARG 					0x8f






typedef struct
{
	unsigned char opcode;		// opcode in bytecode representation
#ifdef DEBUG
	char *name;					// string version
	char *description;			// a short description
	// unsigned int num_parameters; //number of parameters
	unsigned int argcount;
	unsigned int supported;
#endif
} opcode;

void DumpUnsupportedOpCodes();

unsigned int GetSupportedOpcodesNum();

int GetOpcodeIndex(unsigned char opcode);

/* 
   #ifdef OPCODES_C #define EXTERN #else #define EXTERN extern #endif */


#endif
