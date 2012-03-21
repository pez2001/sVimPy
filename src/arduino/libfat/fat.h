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

#ifndef fat_h
#define fat_h

#include <inttypes.h>

typedef struct
{
  byte bootable;
  byte start_sector_chs[3];
  byte type;
  byte last_sector_chs[3];
  uint32_t start_sector_lba;
  uint32_t number_of_sectors;
}partition_entry;

typedef struct
{
 //byte jump_code[3];
 //char oem_name[8];
 uint16_t bytes_per_sector;
 byte sectors_per_cluster;
 uint16_t reserved_sectors;
 byte number_fats;
 uint16_t max_root_entries;
 uint16_t total_medium_sectors;
 byte media_descriptor;
 uint16_t number_fat_sectors;
 uint16_t sectors_per_track;
 uint16_t number_heads;
 uint32_t hidden_sectors;
 uint32_t total_sectors; 
}boot_sector_start;

/*
typedef struct
{
 byte bios_medium_id;
 byte reserved;
 byte extra_boot_signature;
 uint32_t serial_number;
 char name[11];
 char fat_variant[8];
}boot_sector_old_end;
*/
/*
typedef struct
{
  uint32_t number_fat_sectors;
  uint16_t fat_flags;
  uint16_t fat32_version;
  uint32_t root_directory_start_sector;
  uint16_t fs_information_sector;
  uint16_t boot_sector_copy;
  byte reserved[12];
  byte bios_medium_id;
  byte reserved_too;
  byte extra_boot_signature;
  uint32_t serial_number;
  char name[11];
  char fat_variant[8];
}boot_sector_new_end;
*/

#define FILE_ATTRIBUTE_READ_ONLY 1
#define FILE_ATTRIBUTE_HIDDEN 2
#define FILE_ATTRIBUTE_SYSTEM 4
#define FILE_ATTRIBUTE_VOLUME_LABEL 8
#define FILE_ATTRIBUTE_DIRECTORY 16
#define FILE_ATTRIBUTE_ARCHIVE 32
#define FILE_ATTRIBUTE_LFN 15


typedef struct
{
  uint8_t filename[8];
  uint8_t extension[3];
  uint8_t file_attributes;
  byte reserved;
  uint8_t creation_time_ms;
  uint16_t creation_time;
  uint16_t creation_date;
  uint16_t last_access_date;
  uint16_t start_cluster_high;
  uint16_t last_modified_time;
  uint16_t last_modified_date;
  uint16_t start_cluster_low;
  uint32_t file_size;
}directory_entry;

typedef struct
{
  uint8_t ordinal_field;
  uint16_t uc1;
  uint16_t uc2;
  uint16_t uc3;
  uint16_t uc4;
  uint16_t uc5;
  uint8_t file_attributes;
  byte type_reserved;
  uint8_t checksum;
  uint16_t uc6;
  uint16_t uc7;
  uint16_t uc8;
  uint16_t uc9;
  uint16_t uc10;
  uint16_t uc11;
  uint16_t cluster_unused;
  uint16_t uc12;
  uint16_t uc13;
}directory_entry_lfn_overlay;
//}lfn_overlay;

typedef struct 
{
//char *filename;
byte file_attributes;
uint32_t file_size;  
uint32_t start_cluster;
}entry_info;



#endif
