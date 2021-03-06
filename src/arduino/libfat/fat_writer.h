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

#ifndef fat_writer_h
#define fat_writer_h

#include "fat_reader.h"

int CreateFile(char *filename,uint32_t directory_start_cluster);
int WriteFile(uint32_t file_start_cluster,char* buffer,uint32_t offset,uint16_t length);
int DeleteFile(char *filename,uint32_t directory_start_cluster);
int RenameFile(char *filename,char*new_filename,uint32_t directory_start_cluster);
int SetCluster(uint32_t cluster,uint32_t points_to);
int AppendClusters(uint32_t cluster,uint32_t number_clusters);

uint32_t GetFreeCluster();




#endif