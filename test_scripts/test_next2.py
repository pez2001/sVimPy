def f():
	for r in range(1,3):
		yield r
for r in f():
	print(r)
