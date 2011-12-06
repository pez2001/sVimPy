#ifndef OPCODES_H
#define OPCODES_H



typedef struct {
unsigned char opcode;  //opcode in bytecode representation
#ifdef DEBUG
char *name;    //string version
char *description;    //a short description
//unsigned int num_parameters; //number of parameters
unsigned int argcount;
#endif
} opcode;


int GetOpcodeIndex(unsigned char opcode);
/*
#ifdef OPCODES_C
#define EXTERN 
#else
#define EXTERN extern
#endif
*/


#endif