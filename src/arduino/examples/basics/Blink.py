"""
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
  
  based on blink.ino from the arduino examples
"""
import arduino

def setup():
#initialize the digital pin as an output.
#Pin 13 has an LED connected on most Arduino boards:
	pinMode(13, OUTPUT);     


def loop():
#set the LED on
	digitalWrite(13, HIGH)
#wait for a second
	delay(1000)
#set the LED off              
	digitalWrite(13, LOW)
#wait for a second
	delay(1000)              

