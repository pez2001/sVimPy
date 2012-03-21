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