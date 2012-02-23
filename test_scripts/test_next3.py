def f():
	for r in range(1,3):
		yield r
r = f()
print(next(r))
print(next(r))
