#include "fat_writer.h"

int WriteFile(uint32_t file_start_cluster,char* buffer,uint32_t offset,uint16_t length)
{
return(-1);  
}

int DeleteFile(char *filename,uint32_t directory_start_cluster)
{
  //set shortname first character to 0xE5
  //clear cluster chain
return(-1);  
}  

int RenameFile(char *filename,char*new_filename,uint32_t directory_start_cluster)
{
return(-1);  
}  

int SetCluster(uint32_t cluster,uint32_t points_to)
{
 //remember to update fat copies too
  
}  

int AppendClusters(uint32_t cluster,uint32_t number_clusters)
{
  
}


  

int CreateFile(char *filename,uint32_t directory_start_cluster)
{
  
  
}




uint32_t GetFreeCluster()
{
return(-1);  
}
