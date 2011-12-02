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
                            {0x,"","",0}
                            };
#elseif
instruction instructions[]={{0x00,0},
                            {0x01,0},
                            {0x,0}
                            };

#endif



#endif