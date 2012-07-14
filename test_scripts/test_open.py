def o():
	with open('test.txt') as fp:
		for line in iter(fp.readline, ''):
			print(line)
o()