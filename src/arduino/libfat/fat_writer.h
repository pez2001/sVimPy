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