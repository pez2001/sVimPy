def func(i):
	def sub_func(f):
		for b in range(0,f):
			print(a*b)
	x = i
	x &= 5
	x ^= 1
	x |= 4
	x += 11
	x *= 3
	x -= 21
	x %= 7
	x = rek(x)
	print("x:",x,"\n")
	for a in range(1,10):
		sub_func(a)

def rek(e):
	if(e < 1000):
		e = rek(e*e)
	return(e)
for i in range(1,30):
	func(i)

