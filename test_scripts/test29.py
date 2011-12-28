def f(i):
	a = i
	def g():
		return(a+1)
	return g()
print(f(1))
