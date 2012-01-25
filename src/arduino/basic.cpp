#include "basic.h"

extern "C" void __cxa_pure_virtual() { while (1); }

inline void setup (void) {
  
 Serial.begin(115200);
  delay(250);
 Serial.print("hi");
  pinMode(13, OUTPUT);     
}
void loop (void) {
  digitalWrite(13, HIGH);   // set the LED on
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(1000);              // wait for a second
 
}
int main(void) {
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

