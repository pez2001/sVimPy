def func():
	x = 11
	x = x & 5
	x = x ^1
	x = x | 4
	x = x + 11
	x = x * 3
	x = x - 21
	x = x % 7
	return x

o = func()
print ("func ret:",o,"\n")
o = o / 3
o = o + 20
o = o // 3
o = o // 2
for a in (1,2,3):
	print("a:",a,"\n")
print ("div:",o,"\n")
