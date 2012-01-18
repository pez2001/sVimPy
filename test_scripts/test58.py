a = 1
def fa():
	a = 2
	def fb():
		global a
		print("fb:",a)
		a = 3
	fb()
	print("fa:",a)
fa()
print("m:",a)


b = 1
def fba():
	global b
	print("fba:",b)
	b = 2
	def fbb():
		global b
		print("fbb:",b)
		b = 3
	fbb()
	print("fba(after fbb):",b)
fba()
print("m:",b)
