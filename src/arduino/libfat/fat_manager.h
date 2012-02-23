#ifndef fat_manager_h
#define fat_manager_h

#include "fat_reader.h"
//#include "../utils/memory.h"

#include <stdio.h>
#include <string.h>



#define CONTENT_SIZE 25

class FatManager
{
 public:
 FatManager();
 bool OpenFile(char *filename);
 bool IsOpen();
 void CloseFile();
 int ReadFile(char **buf,uint16_t length);
 bool ReadLine(char **buf);
 bool Seek(uint32_t offset);
char *FindFirst(char *path);
char *FindPrev();
char *FindNext();
 uint32_t GetSize();
 
 
 
 
 //void TestRead();

 private:
uint32_t file_offset;
uint16_t find_id;
entry_info *file_info; 
};

extern FatManager fat;
#endif