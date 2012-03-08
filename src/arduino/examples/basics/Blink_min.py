import arduino

def setup():
	pinMode(13, OUTPUT);     


def loop():
	digitalWrite(13, HIGH)
	delay(1000)
	digitalWrite(13, LOW)
	delay(1000)              

