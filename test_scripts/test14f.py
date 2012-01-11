def func():
	x = 11.7
	x += 11.6
	x *= 3.7
	x -= 21.9
	return x

o = func()
print ("func ret:",o,"\n")
o /= 3.1
o += 20.5
o //= 3.8
o = o // 2.1
for a in (1,2,3):
	print("a:",a,"\n")
print ("div:",o,"\n")
