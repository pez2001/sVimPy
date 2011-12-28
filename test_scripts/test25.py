def f(n):
	for i in range(n):
		yield i

a = list(f(5))
print("a:",a)
