def f():
	def g():
		print("g executed\n")
	g()	
	print("f executed\n")
f()
x = 1
x <<= 4
print("x:",x,"\n")
x >>= 2
print("x:",x,"\n")
x = x >> 2
print("x:",x,"\n")
x = x << 4
print("x:",x,"\n")
