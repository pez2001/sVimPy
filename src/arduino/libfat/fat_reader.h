#ifndef fat_reader_h
#define fat_reader_h

#include "mmc_driver.h"
#include "fat.h"


#define ROOT_DIRECTORY_START_CLUSTER -1


void fat_initialize_buffers();
void fat_close_buffers();
void fat_initialize();
void fat_close();
uint32_t FindFirstFat();
int ReadBootSector(uint32_t start_sector);
uint64_t ClusterToSector(uint32_t cluster);
char *GetLfnPart(directory_entry_lfn_overlay *lfn);
//entry_info *GetFirstEntryInfo(uint32_t directory_start_cluster);
//entry_info *GetNextEntryInfo(uint32_t directory_start_cluster);
entry_info *GetEntryInfoByIndex(uint16_t id,uint32_t directory_start_cluster);
entry_info *GetEntryInfo(char *filename,uint32_t directory_start_cluster);
uint32_t GetNextCluster(uint32_t cluster);
int ReadFileContent(uint32_t file_start_cluster,char **out_buffer,uint32_t offset,uint16_t length);
int GetFilesize(char *filename,uint32_t directory_start_cluster);
int GetDirectoryFiles(int c,uint32_t directory_start_cluster);




#endif