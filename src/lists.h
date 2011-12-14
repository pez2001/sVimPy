#ifndef LISTS_H
#define LISTS_H

#include "object.h"



void **ptr_CreateList(unsigned int num);
void ptr_CloseList(void **list);
void ptr_Push(void **list,void *ptr);
void* ptr_Pop(void **list);
int ptr_Insert(void **list,int index,void *ptr);
int ptr_Remove(void **list,int index);
int ptr_Clear(void **list);



#endif
