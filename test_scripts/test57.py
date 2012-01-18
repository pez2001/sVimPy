def f(i):
	a = i
	def g():
		return(a+1)
	a += 2
	
	return g()
print(f(1))
