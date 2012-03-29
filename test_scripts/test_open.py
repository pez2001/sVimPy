def o():
	with open('arduino.cpp') as fp:
		for line in iter(fp.readline, ''):
			print(line)
o()