//#include "basic.h"
#include "../libarduino/Arduino.h"
#include "shift_register.h"
#include "fat_manager.h"
#include "mmc_driver.h"

extern "C" void __cxa_pure_virtual() { while (1); }
//ShiftRegister ShiftReg;//(CLK,STR,DOUT,LSBFIRST,3);//shield
FatManager fat;
mmc mmc_card2;

inline void setup (void) {
  
  Serial.begin(115200);
  delay(2500);
  //pinMode(12,INPUT);
  //int cd = digitalRead(12);
  //Serial.println("Beginning initializations:");
  //Serial.println(cd,DEC);
  //fat.OpenFile("*");
  byte *buffer = (byte*)malloc(1024);
  if(mmc_card2.initialize() != RES_OK)
   Serial.println("error initializing card");
  long s = 0;
  while(mmc_card2.readSectors(buffer,s,2)==RES_OK)
  {
    for(int i=0;i<1024;i++)
    {
    Serial.print((char)buffer[i]);
    }
  s+=2;
  }
  /*
  fat.OpenFile("MMC PICTURES\\Photo0018.jpg");
  Serial.println("opened file");

  char *content = NULL;
  int r = 0;
  while((r=fat.ReadFile(&content,512)) != 0)
  {
   //Serial.print("r:");
   //Serial.println(r,DEC);
  for(int o=0;o<r;o++)
  {

   Serial.print(content[o]);
  }
  //free(content);      
  //content = NULL;
  }*/
  /*
  //free(content);      
  //content = NULL;
  for(int i=0;i<10;i++)
  {
  fat.Seek(0);
  Serial.print("s:");
  Serial.println(i,DEC);
  while(fat.ReadLine(&content))
  {
   Serial.print("rl:");  
   Serial.println(content);
   free(content);      
  }
  }
  //free(content);      
  //content = NULL;
  */
  //fat.CloseFile();
  //Serial.println("Finished initializations");
}
void loop (void) {

}

int main(void) {
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

