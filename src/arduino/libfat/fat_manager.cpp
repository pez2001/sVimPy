#include "fat_manager.h"


FatManager::FatManager()
{

//uint32_t offset;
//uint16_t find_id;
//entry_info *file_info; 
file_offset = 0;
find_id = -1;
file_info = NULL; 
}
bool FatManager::OpenFile(char *filename)
{
  uint32_t start_cluster = ROOT_DIRECTORY_START_CLUSTER;
  char *tok;
  entry_info *ei = NULL;
  fat_initialize_buffers();
  //fat_initialize();
  tok = strtok (filename,"/\\");
  //Serial.println(tok);
  while (tok != NULL)
  {
    if(ei != NULL)
     free(ei);
	ei = NULL;
    ei = GetEntryInfo(tok,start_cluster);
	if(ei != NULL)
	{
    //Serial.print("dir:"); 
    //Serial.println(ei->start_cluster,DEC);
    start_cluster = ei->start_cluster;
	}
	else 
	{
	 //Serial.println("not found");
	 return(false);
	}
    tok = strtok (NULL, "/\\");
    //if(tok != NULL)
	// Serial.println(tok);
  }
  
  //TODO check if tok needs free call
  
  
  
  //ei = GetEntryInfo("plot.tcl",start_cluster);
  //ei = GetEntryInfo("noname.drl",start_cluster);
  //ei = GetEntryInfo("hola.txt",ROOT_DIRECTORY_START_CLUSTER);
  //Serial.print("file:"); 
  //Serial.println(ei->start_cluster,DEC);
  file_info = ei;
  fat_close_buffers();
}
bool FatManager::IsOpen()
{
return(file_info != NULL);
}
void FatManager::CloseFile()
{
if(file_info != NULL)
 free(file_info);
 file_info = NULL;
 file_offset = 0;
}
int FatManager::ReadFile(char **buf,uint16_t length)
{
 int buf_size = length;
 //Serial.println("fib");
 fat_initialize_buffers();
 
 if(file_offset + buf_size > file_info->file_size)
  buf_size = file_info->file_size - file_offset;
 if(buf_size == 0)
 {
  fat_close_buffers();
  //Serial.println("ee");
  return(0);
 }
 
 //Serial.println("rfc");
 int r = ReadFileContent(file_info->start_cluster,buf,file_offset,buf_size);
 //Serial.println("rfcd");
 //if(r==0)
 // Serial.println("r==0");
 file_offset += r;
 fat_close_buffers();
 //Serial.println("fcb");
 return(r);
}
bool FatManager::ReadLine(char **buf)
{
//read a line of the file
//Serial.println("rd");
uint32_t oo = file_offset;
uint8_t buf_size = 20;
char *content = NULL;
char *line = NULL;
bool found_line = false;
short line_len = 0;
//Serial.println("bw");
uint8_t s = 0;
//Serial.print("fo:");
//Serial.println(file_offset,DEC);
while(!found_line)
{
int r = ReadFile(&content,buf_size);
//Serial.print("r:");
//Serial.println(r,DEC);
if(r == 0)
{
 //Serial.println("ll");
 free(content);
 return(false);
}
short le = 0;
if(content[0] == 13 || content[0] == 10)
  s = 1;
 else 
  s = 0;

for(short i = s;i<r;i++)
{
 le = i-s;
 if(content[i] == 13 || content[i] == 10)
 {
  found_line = true;
  //Serial.println("fl");
  break;
 }
}
le++;
//Serial.println("fc");
char *tline = (char*)malloc(line_len + le + 1);//accounting the termination char
memset(tline,0,line_len+le+1);
if(line != NULL)
{
memcpy(tline,line,line_len);
free(line);
}
memcpy(tline+line_len,content + s,le);
free(content);
//Serial.print("tl:");
//Serial.println(tline);
line = tline;
line_len += le + s;
}
*buf = line;
file_offset = oo + line_len + 1;
//Serial.print("fo:");
//Serial.println(file_offset,DEC);
//Serial.print("fs:");
//Serial.println(file_info->file_size,DEC);
return(true);
}
bool FatManager::Seek(uint32_t offset)
{
 file_offset = offset;
}
char *FatManager::FindFirst(char *path)
{


}
char *FatManager::FindPrev()
{


}
char *FatManager::FindNext()
{


}
uint32_t FatManager::GetSize()
{
if(IsOpen())
 return(file_info->file_size);
return(0);
}
/* 
void FatManager::TestRead()
{


  int free_mem = memory.GetFree();
  Serial.print("fr:");
  Serial.println(free_mem,DEC);
  long gap = memory.GetGapSize();
  Serial.print("gp:");
  Serial.println(gap,DEC);
  fat_initialize_buffers();
  //fat_initialize();
  free_mem = memory.GetFree();
  Serial.print("fr:");
  Serial.println(free_mem,DEC);
  gap = memory.GetGapSize();
  Serial.print("gp:");
  Serial.println(gap,DEC);
  uint32_t start_cluster = 15;
  uint32_t file_size = 0;
  //entry_info *ei = GetEntryInfo("Abs.html",ROOT_DIRECTORY_START_CLUSTER);
  entry_info *ei = GetEntryInfo("drill_files",ROOT_DIRECTORY_START_CLUSTER);
  Serial.print("dir:"); 
  Serial.println(ei->start_cluster,DEC);
  start_cluster = ei->start_cluster;
  free(ei);
  //ei = GetEntryInfo("plot.tcl",start_cluster);
  ei = GetEntryInfo("noname.drl",start_cluster);
  //ei = GetEntryInfo("hola.txt",ROOT_DIRECTORY_START_CLUSTER);
  Serial.print("file:"); 
  Serial.println(ei->start_cluster,DEC);
  start_cluster = ei->start_cluster;
  file_size = ei->file_size;
  free(ei);
  //free_mem = memory.GetFree();
  //Serial.print("free ram:");
  //Serial.println(free_mem,DEC);

 gap = memory.GetGapSize();
 Serial.print("gp:");
 Serial.println(gap,DEC);
 char *contents = NULL;
 int offset = 0;
 int r = 0;
 int r2 = 0;
 int buf_size = CONTENT_SIZE;
 gap = memory.GetGapSize();
 Serial.print("gp:");
 Serial.println(gap,DEC);
// r = -1;
 while (r != -1 && r2 < file_size)
 {
 r2+= r;  
 if(buf_size > file_size - r2)
  buf_size = file_size - r2;
 led.On(PWR_LED);
 r = ReadFileContent(start_cluster,&contents,offset,buf_size);
 led.Off(PWR_LED);
 //Serial.println(r,DEC);
 //free_mem = memory.GetFree();
 //Serial.print("free ram:");
 //Serial.println(free_mem,DEC);

 for(int o=0;o<r;o++){
  Serial.print(contents[o]);
//lcd.DisplayText((char*)&contents);
}
 int lp = 0;
 int c = 255;
 //lcd_driver.clear();
 //for(int o=0;o<r;o++)
 //{
  if(lp == 8)
  {
   lcd_driver.setCursor(0,1);
  }
  if(lp==16)
  {
  //delay(500);
  lcd_driver.clear();
  lp = 0;
  }
  lcd_driver.print(contents[o]);

  //Serial.write(contents[o]);
  //delay(20);
  //lp++;
  //analogWrite(14,255-lp*15);
   //led.Update();
 //}
 free(contents);      
 offset += buf_size;
 //r = -1;
 led.Update();
}
//analogWrite(14,0);

 //Serial.println("");
 fat_close_buffers();
 Serial.println("thru");
 gap = memory.GetGapSize();
 Serial.print("gp:");
 Serial.println(gap,DEC);
 free_mem = memory.GetFree();
 Serial.print("fr:");
 Serial.println(free_mem,DEC);
 
 //digitalWrite(10,LOW);

}
*/
