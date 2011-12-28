def f():
	a = sum(i*i for i in range(10))
	print("a:",a)
	return a
r = f()
print("r:",r)