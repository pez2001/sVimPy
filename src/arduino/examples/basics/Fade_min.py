import arduino

b = 0    # how bright the LED is
f = 5    # how many points to fade the LED by

def setup():
	# declare pin 9 to be an output:
	pinMode(9, OUTPUT)


def loop():
	global b
	global f
	# set the brightness of pin 9:
	analogWrite(9, b)    

	# change the brightness for next time through the loop:
	b = b + f

	# reverse the direction of the fading at the ends of the fade: 
	if (b == 0 or b == 255):
		f = -f 
     
	# wait for 30 milliseconds to see the dimming effect    
	delay(30)                            
