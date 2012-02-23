def f():
	for r in range(1,3):
		yield r
print(next(f()))
print(next(f()))
print(next(f()))
