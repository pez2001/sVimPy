def func():
	x = 11
	x &= 5
	x ^= 1
	x |= 4
	x += 11
	x *= 3
	x -= 21
	x %= 7
	return x

o = func()
print ("func ret:",o,"\n")
o /= 3
o += 20
o //= 3
o = o // 2
for a in (1,2,3):
	print("a:",a,"\n")
print ("div:",o,"\n")
