def f():
	for r in range(1,3):
		yield r
print(list(f()))