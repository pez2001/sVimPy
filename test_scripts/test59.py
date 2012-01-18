def f():
	yield 1
	yield 2
a = f()
for i in a:
	print(i)