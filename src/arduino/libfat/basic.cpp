#include "basic.h"

extern "C" void __cxa_pure_virtual() { while (1); }
ShiftRegister ShiftReg;//(CLK,STR,DOUT,LSBFIRST,3);//shield
LCD lcd_driver;
LedManager led;
LcdManager lcd;
Joypad joypad;
//Memory memory;
FatManager fat;
MenuManager menu;
Positioning pos;
NCDrill drill;

//LCD lcd();
//ShiftRegister2 reg;
bool show = true;
int dir = 1;

int sm = 0;
void joypad_button_release()
{
 lcd.DisplayMessage("b");
 led.Flash(GREEN_LED,20,1000,1000);	
 led.Flash(RED_LED,10,500,500);	
 led.StopFlashing(YELLOW_LED);
 //lcd.SetCursor(3+8,4);
 //lcd.SetCursor(3,4);
 //lcd.SetCursor(0,15);
 //lcd.SetCursor(7,4);
 //lcd.SetCursor(6,6);
 pos.SetStepMode(sm);
 sm++;
 if(sm == 5)
  sm = 0;
  
  
 if(dir == 1)
  dir = -1;
 else 
  dir = 1;
 //pos.SetMotorsToSleep();
 //if( show == true)
 // lcd.SwitchCursor(ON);
 // else 
 // lcd.SwitchCursor(OFF);
 // show = !show;
}
inline void setup (void) {
  
  led.Flash(RST_LED,INFINITE_TIMES,5,20);
  Serial.begin(9600);
  delay(250);
  //int free_mem = memory.GetFree();
  //Serial.print("fr:");
  //Serial.println(free_mem,DEC);
  //long gap = memory.GetGapSize();
  //Serial.print("gp:");
  //Serial.println(gap,DEC);

  //Serial.println("Beginning initializations");
  ShiftReg.set(0,0);
  ShiftReg.set(0,1);
  ShiftReg.set(0,2);
  lcd.Init();
  pos.CalibrateX(25000);
  pos.CalibrateY(25000);
  pos.CalibrateZ(25000);
  pos.SetHome();
  //Serial.println("starting fat test");
  led.FlashOnce(GREEN_LED);
  //fat.OpenFile("drill_files\\noname.drl");
  //fat.OpenFile("mill_files\\test.gcode");
  //char *content = NULL;
  /*
  for(int i=0;i<10;i++)
  {
  fat.Seek(0);
  Serial.print("s:");
  Serial.println(i,DEC);
  int r = 20;
  while(r=fat.ReadFile(&content,20))
  {
  for(int o=0;o<r;o++){
   Serial.print(content[o]);
  }
  free(content);      
  content = NULL;
  }
  }
  //free(content);      
  //content = NULL;
  */
  /*
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
  led.FlashOnce(RED_LED);
  }
  //free(content);      
  //content = NULL;
  fat.CloseFile();
  */
  led.FlashOnce(GREEN_LED);
  //drill.ParseFile("drill_files\\noname.drl");
  joypad.Grab(55,NULL,joypad_button_release);
  //Serial.println("flashing leds");
  int i = 0;
  led.Flash(YELLOW_LED,INFINITE_TIMES,5,20);
  //lcd.DisplayScroller(0,0,3,"scrolling text");
  //lcd.DisplayScroller(0,0,9,"long scrolling text");
  //lcd.DisplayScroller(0,0,10,"too short");
  //lcd.DisplayScroller(1,8,3,"shorter");
  //lcd.DisplayScroller(1,7,4,"shorter exp");
  //gap = memory.GetGapSize();
  //Serial.print("gp:");
  //Serial.println(gap,DEC);
  if(drill.IsReady())
   drill.Start();
  pos.SetMotorRPM(300,300,300);
  led.StopFlashing(RST_LED);
  pos.SetMotorsToSleep();
}
void loop (void) {

  if(!drill.IsExecuting())
  {
   led.FlashOnce(YELLOW_LED);
   //lcd.DisplayText("executed drill file");
  }
  else
  {
   //char msg[16];
   //sprintf((char*)&msg,"p%d,s%d,o%d,t%d",drill.GetPercentage(),pos.GetStepsLeft(),pos.GetX(),pos.GetTargetX());
   //sprintf((char*)&msg,"%d,%d,%d,%d,%d",pos.GetStepsLeft(),pos.GetX(),pos.GetTargetX(),pos.GetY(),pos.GetTargetY());
   //sprintf((char*)&msg,"sl:%d,X:%d,TX:%d,Y:%d,TY:%d,Z:%d,TZ:%d",pos.GetStepsLeft(),pos.GetX(),pos.GetTargetX(),pos.GetY(),pos.GetTargetY(),pos.GetZ(),pos.GetTargetZ());
   //lcd.DisplayText((char*)&msg);
   //Serial.println(msg);
  }
  //Serial.println("loop");

   pos.SetMotorRPM((abs(joypad.GetY())+1)*8,(abs(joypad.GetX())+1)*8,300);

   if(!pos.IsWorking())
   {
   led.FlashOnce(YELLOW_LED);
   if(joypad.GetY()>0)
    pos.StepY(1);
   if(joypad.GetY()<0)
	pos.StepY(-1);
	
   if(joypad.GetX()>0)
    pos.StepX(1);
   if(joypad.GetX()<0)
	pos.StepX(-1);
   //pos.StepY(dir);
   led.FlashOnce(RED_LED);
   }

   if(joypad.IsMoving())
   {
   led.FlashOnce(GREEN_LED);
   Serial.print("x:");
   Serial.println(joypad.GetX(),DEC);
   Serial.print("y:");
   Serial.println(joypad.GetY(),DEC);

   //pos.StepZ(dir);
   //char msg[16];
   //sprintf((char*)&msg,"x: :%04d y::%04d",joypad.GetRelX(),joypad.GetRelY());
   //sprintf("hi
   //Serial.println((char*)&msg);
   //lcd.DisplayText((char*)&msg);
   }
   //itoa(joypad.GetX(),&
   //lcd_driver.print("x:");
   //lcd_driver.print(joypad.GetX(),DEC);
   //lcd_driver.print("  ");
   //lcd_driver.setCursor(0,1);
   //lcd_driver.print("y:");
   //lcd_driver.print(joypad.GetY(),DEC);
   //lcd_driver.print("  ");

   if(joypad.GetButton())
   {
  led.FlashOnce(GREEN_LED);
  //pos.StepX(dir);
  led.FlashOnce(YELLOW_LED);
  //pos.StepY(dir);
  led.FlashOnce(RED_LED);
  //pos.StepZ(dir);
    
	//lcd_driver.print("#");
	//lcd.DisplayMessage("!button pressed!");
    //joypad.Calibrate();
   }
   joypad.Update();
   lcd.Update();
   led.Update();
   pos.Update();
   drill.Update();
   /*
  if(i%300 == 0)
  led.On(GREEN_LED);
  //Serial.print("Green is:");
  //if(led.IsOn(GREEN_LED))
  // Serial.println(" ON");
  //else
  // Serial.println(" OFF");
  //delay(10);
  if(i%2  == 0)
  led.Off(GREEN_LED);
  //delay(10);
  if(i%160 == 0)
  led.On(YELLOW_LED);
  //delay(10);
  led.Off(YELLOW_LED);
  //delay(10);
  if(i%400  == 0)
  led.On(RED_LED);
  //delay(10);
  led.Off(RED_LED);
  //delay(10);
  if(i%30  == 0)
  led.On(PWR_LED);
  //delay(10);
  //if(i%120 == 0)
  led.Off(PWR_LED);
  //delay(10);
  if(i%600 == 0)
  led.On(RST_LED);
  //delay(10);
  led.Off(RST_LED);
  //delay(10);
  */
//  i++;
 
}
int main(void) {
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

