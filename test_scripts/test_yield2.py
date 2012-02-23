def f():
	for r in range(1,3):
		yield r
	return 5
print(list(f()))