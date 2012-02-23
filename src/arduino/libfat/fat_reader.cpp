#include "fat_reader.h"

/*class fat
{
  //union end headers
  //#define KEEP_FILES_OPEN //sd must be removed in code or else strange things could happen
  fat()
  {
    
  }
//byte buffer[512];
//partition_entry entry;
//boot_sector_start start;
//boot_sector_new_end new_end;
//boot_sector_old_end old_end;
//boot_sector_new_end *new_end = NULL;
//boot_sector_old_end *old_end = NULL;
*/  

mmc mmc_card;
byte *buffer = NULL;
//partition_entry *entry = NULL;
boot_sector_start *start = NULL;

uint8_t bits_per_cluster = 16; //TODO read this from fat variant
uint32_t root_directory_start_sector = -1;
uint32_t fat_start_sector = -1;
uint32_t data_start_sector = -1;
uint32_t cluster_size = -1;
//bool found_fat = false;
//bool is_old_fat = true;
//char *volume_label = NULL;


void fat_initialize_buffers()
{
 buffer = (byte*)malloc(512);
 //Serial.println("buffer allocated");
// entry = (partition_entry*)malloc(sizeof(partition_entry));
 //Serial.println("partition entry buffer allocated");
 start = (boot_sector_start*)malloc(sizeof(boot_sector_start));
 //new_end = (boot_sector_new_end*)malloc(sizeof(boot_sector_new_end));
 //old_end = (boot_sector_old_end*)malloc(sizeof(boot_sector_old_end));
}

void fat_close_buffers()
{
 //if(entry != NULL)
 // free(entry);
 if(buffer != NULL)
  free(buffer); 
 //if(new_end != NULL)
 // free(new_end);
 //if(old_end != NULL)
 // free(old_end);
 if(start != NULL)
  free(start);
 buffer = NULL;
 start = NULL;
 //old_end = NULL;
 //entry = NULL;
 //new_end = NULL;
}  

void fat_initialize()
{
// Serial.println("fat initializing");
  //mmc_card = mmc();
  
  if(mmc_card.initialize() != RES_OK)
   Serial.println("error initializing card");
 // else 
 //  Serial.println("card initialized"); 

// Serial.println("mmc finished");
   
 
 //mmc_card.initialize();
 uint32_t start_sector = FindFirstFat();
// Serial.println("fat found");
 ReadBootSector(start_sector);
/*
 if(FindFirstFat())
  Serial.println("found first fat partition");
 if(ReadBootSector())
  Serial.println("found boot sector"); 
 */
// Serial.println("fat initialized");
}

void fat_close()
{
 //free(mmc_card);
 //mmc_card = NULL;
//found_fat = false;
cluster_size = -1;
root_directory_start_sector = -1;
fat_start_sector = -1;
data_start_sector = -1;
//Serial.println("fat closed");
}  

uint32_t FindFirstFat()
{
  if(mmc_card.readSectors(buffer,0,1)==RES_OK)
  {
   //test_partition_entry *entry = (test_partition_entry*)&buffer[446];
   //entry = (partition_entry*)malloc(sizeof(partition_entry));
   //Serial.println("premalloc entry");
   //partition_entry *loop_entry = (partition_entry*)malloc(sizeof(partition_entry));
   //Serial.println("malloced entry");
   partition_entry *entry = (partition_entry*)malloc(sizeof(partition_entry));
   int i = 0;
   while(i < 4)
   {
   //memcpy(loop_entry,&buffer[446 + (16*i)],sizeof(partition_entry));
   memcpy(entry,&buffer[446 + (16*i)],sizeof(partition_entry));
   if(entry->bootable == 0x80)
    Serial.println("partition is bootable");
   //if(loop_entry->type == 1 || loop_entry->type == 4 || loop_entry->type == 6 || loop_entry->type == 11 || loop_entry->type == 12 || loop_entry->type == 14)
   if(entry->type == 1 || entry->type == 4 || entry->type == 6 || entry->type == 11 || entry->type == 12 || entry->type == 14)
   {
    //free(loop_entry);
    free(entry);
    return(entry->start_sector_lba);
   }
   i++;
   } 
   if(buffer[510]==0x55 && buffer[511] == 0xaa)
    Serial.println("buffer contains a bootsector,but no fat partition");
   else
    Serial.println("no bootsector found");  
  //free(loop_entry); 
  free(entry);
  }
  else Serial.println("error reading sector");
  return(0);
}  


int ReadBootSector(uint32_t start_sector)
{
  if(mmc_card.readSectors(buffer,start_sector,1)==RES_OK)
  {
   //start = (boot_sector_start*)malloc(sizeof(boot_sector_start));
   memcpy(start,buffer+11,sizeof(boot_sector_start));

    root_directory_start_sector = start_sector + (start->number_fats * start->number_fat_sectors)+start->reserved_sectors;
    fat_start_sector = start_sector + start->reserved_sectors;
    data_start_sector = root_directory_start_sector + ((start->max_root_entries * 32)/512);
   cluster_size = start->bytes_per_sector * start->sectors_per_cluster;
   if(buffer[57] != 49) //tricky check for fat type before copying the rest of the important boot_sector contents
   {
    //new_end = (boot_sector_new_end*)malloc(sizeof(boot_sector_new_end));
    //memcpy(new_end,&buffer+ sizeof(boot_sector_start),sizeof(boot_sector_new_end));
    //is_old_fat = false;
    //found_fat = true;
    return(1);
   }
   else
   {
    //old_end = (boot_sector_old_end*)malloc(sizeof(boot_sector_old_end));
    //memcpy(old_end,&buffer[sizeof(boot_sector_start)],sizeof(boot_sector_old_end));
    //found_fat = true;
    return(1);
    }
 }
 return(0);
}  

uint64_t ClusterToSector(uint32_t cluster)
{
  return((start->sectors_per_cluster * (cluster-2)) + data_start_sector);
}  

char *GetLfnPart(directory_entry_lfn_overlay *lfn)
{
 char *part = (char*)malloc(14);
 part[0] = lfn->uc1 & 255;
 part[1] = lfn->uc2 & 255;
 part[2] = lfn->uc3 & 255;
 part[3] = lfn->uc4 & 255;
 part[4] = lfn->uc5 & 255;
 part[5] = lfn->uc6 & 255;
 part[6] = lfn->uc7 & 255;
 part[7] = lfn->uc8 & 255;
 part[8] = lfn->uc9 & 255;
 part[9] = lfn->uc10 & 255;
 part[10] = lfn->uc11 & 255;
 part[11] = lfn->uc12 & 255;
 part[12] = lfn->uc13 & 255;
 part[13] = 0;
return(part);  
}  

entry_info *GetFirstEntryInfo(uint32_t directory_start_cluster)
{
  
  
}

entry_info *GetNextEntryInfo(uint32_t directory_start_cluster)
{
  
}  


entry_info *GetEntryInfo(char *filename,uint32_t directory_start_cluster)
{
entry_info *ei = NULL;
//Serial.println("gei");
fat_initialize();
boolean found_last_entry = false;
//Serial.println("gei");

directory_entry *dir_entry = (directory_entry*)malloc(sizeof(directory_entry));
//int sector_i = 0;
char *dir_filename = (char*)malloc(255);//lfn filename can be a bit longer than 8+3
boolean found_lfn = false;
char *lfn_parts[20];
int lfn_parts_num = 0;
//int lfn_start = 254;
uint32_t offset = 0;
uint32_t read_cluster = directory_start_cluster;
while(!found_last_entry)
{
 //Serial.print("offset:"); 
 //Serial.println(offset,DEC);
 //uint32_t dir_start_cluster = offset / cluster_size;
 uint64_t read_sector = 0;
 uint32_t dir_start_sector = 0;
 uint16_t dir_start_byte = 0;

if(directory_start_cluster == ROOT_DIRECTORY_START_CLUSTER)
{
 dir_start_sector = root_directory_start_sector; 
 dir_start_sector += (offset / start->bytes_per_sector);
 read_sector = dir_start_sector;
}
else
{
 /* 
 int i = 0;
 while(i< dir_start_cluster)
 { 
  read_cluster = GetNextCluster(read_cluster);
  if(read_cluster >= 0xFFF8)
   return(ei);
  i++;
 } 
 
 read_sector = ClusterToSector(read_cluster) + dir_start_sector;
 */
 uint32_t dir_start_sector = (offset / start->bytes_per_sector) % start->sectors_per_cluster;
 uint16_t dir_start_byte = offset % start->bytes_per_sector;

 read_sector = ClusterToSector(read_cluster) + dir_start_sector;
 }
 int ic = 0;
 while(ic<start->sectors_per_cluster)
 {
 if(mmc_card.readSectors(buffer,read_sector, 1)==RES_OK)
 {
  //Serial.print("read directory sector: ");
  //Serial.println(dir_start_sector,DEC);
  memset(dir_filename,0,255);
  int ri = 0;
  while(ri < 16)
  {
   //test_directory_entry *dir_entry = (test_directory_entry*)malloc(sizeof(test_directory_entry));
   memcpy(dir_entry,&buffer[(offset % start->bytes_per_sector)],sizeof(directory_entry));
   if(dir_entry->file_attributes == FILE_ATTRIBUTE_LFN)
   {  
    directory_entry_lfn_overlay *lfn = (directory_entry_lfn_overlay*)dir_entry;
    char *lfn_part = GetLfnPart(lfn);
    lfn_parts[lfn_parts_num++] = lfn_part;
    found_lfn = true;
   }  
   else
   {
   //memset(dir_filename,0,255);
   if(dir_entry->filename[0] != 0)
   {
     
    if(!found_lfn)
    {
    //Serial.println("no lfn found");
    memset(dir_filename,0,255);
    for(int i=0;i<8;i++)
    {
     if(dir_entry->filename[i] == 32) 
     {
      int e = i;
      boolean break_loop = true;
      while(e<8)
      {
       if(dir_entry->filename[e] != 32)
        break_loop = false;
       e++;
      }
      if(break_loop)
       break;
     }
     dir_filename[i] = dir_entry->filename[i];
    }  
   //TODO check if ext exists and only add it if 
   boolean has_extension = false;
   for(int i=0;i<3;i++)
    {
     if(dir_entry->extension[i] != ' ')
      has_extension = true; 
    }
   if(has_extension)
   {
   dir_filename[strlen(dir_filename)] = '.';
   for(int i = 0;i<3;i++)
   {
    if(dir_entry->extension[i] == 0) 
    {
     if(i==0)
      dir_filename[strlen(dir_filename)-1] = 0;
     break;
    }
    dir_filename[strlen(dir_filename)] = dir_entry->extension[i];
   }
   }  
   }
  else
  {
   //move lfn to front of dir_name  
   for(int i=lfn_parts_num-1;i>=0;i--)
   {
    memcpy(&dir_filename[strlen(dir_filename)],(char*)lfn_parts[i],strlen((char*)lfn_parts[i]));
   }
  }
  //Serial.print("f:");
  //Serial.println(dir_filename);
  //Serial.print("c:");
  //Serial.println(filename);
  
  if(!strcmp(strlwr(dir_filename),strlwr(filename)) && (dir_entry->filename[0] != 0xE5))
  {
   //start_cluster = dir_entry->start_cluster_low;
   ei = (entry_info*)malloc(sizeof(entry_info));
   ei->file_attributes = dir_entry->file_attributes;
   ei->file_size = dir_entry->file_size;
   ei->start_cluster = dir_entry->start_cluster_low;
   found_last_entry = true;
   //Serial.print("ei:");
   //Serial.println(ei->start_cluster,DEC);
   //Serial.println("ff");
   //break;
  }
  //Serial.println("resetting dir_filename");
  memset(dir_filename,0,255);
  found_lfn = false;
  //lfn_start = 254;
  for(int i=0;i<lfn_parts_num;i++)
   free(lfn_parts[i]);
  lfn_parts_num =0;
  if(found_last_entry)
   break;
  }
  else
  {
   found_last_entry = true;
   break;
  }
 } 
  offset+=32;
  ri++;  
 }
 //if(found_last_entry)
 // break;
}
ic++;
read_sector++;

}
if(directory_start_cluster != ROOT_DIRECTORY_START_CLUSTER)
{
  read_cluster = GetNextCluster(read_cluster);
  if(read_cluster >= 0xFFF8)
   found_last_entry = true;
}


//found_last_entry = true;  
//directory_start_sector++;
}
free(dir_entry);
free(dir_filename);
fat_close();
return(ei);
}

uint32_t GetNextCluster(uint32_t cluster)
{
  int si = ((cluster) * (bits_per_cluster/8) )/512;
  int so = ((cluster) * (bits_per_cluster/8) )%512;
  uint32_t next_cluster = -1;
 if(mmc_card.readSectors(buffer,fat_start_sector + si, 1)==RES_OK)
 {
  next_cluster=(uint16_t)buffer[so]+((uint16_t)buffer[so+1]<<8);
 }  
return(next_cluster);
}



int ReadFileContent(uint32_t file_start_cluster,char **out_buffer,uint32_t offset,uint16_t length)
{
 fat_initialize();
 //Serial.println("fic");
 char *content = (char*)malloc(length);
 //Serial.println("mac");
 memset(content,0,length);
 uint32_t data_number_sectors = length / start->bytes_per_sector;
 int is = 0;//actual sector to read in loop
 uint16_t content_offset = 0;
 uint16_t content_read = 0; 
 //until all data for a full buffer is read increase offset  + decrease length + decrease is
 while(length > 0)
 {
 uint32_t data_start_cluster = offset / cluster_size;
 uint32_t data_start_sector = (offset / start->bytes_per_sector) % start->sectors_per_cluster;
 uint16_t data_start_byte = offset % start->bytes_per_sector;
 uint16_t data_length = start->bytes_per_sector - data_start_byte;
 if(data_length > length)
  data_length = length;
  if(data_length < length)
   {
   //Serial.println("need to read more sectors");
   //data_number_sectors++;
   }
  //continue reading sectors till buffer is filled up
 //now get the first cluster position
 
 uint32_t read_cluster = file_start_cluster;
 //uint32_t next_cluster=file_start_cluster;
 int i = 0;
 while(i< data_start_cluster)
 { 
  read_cluster = GetNextCluster(read_cluster);
  if(read_cluster >= 0xFFF8)
   return(-1);
  i++;
 } 
 uint64_t read_sector = ClusterToSector(read_cluster) + data_start_sector;
 if(mmc_card.readSectors(buffer,read_sector, 1)==RES_OK)
 {
    memcpy(&content[content_offset],&buffer[data_start_byte],data_length);
 } 
 content_offset += data_length; 
 length -= data_length;
 offset += data_length;
 content_read += data_length;
} 
 fat_close();
 *out_buffer=content;
 return(content_read); 
}  

int GetFilesize(char *filename,uint32_t directory_start_cluster)
{
return(-1);  
}

int GetDirectoryFiles(int c,uint32_t directory_start_cluster)
{
return(-1);  
}
  



  
  
