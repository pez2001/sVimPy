def f():
	print("y1\n")
	yield 1
	print("y2\n")
	yield 2

g = f()
next(g)
next(g)
