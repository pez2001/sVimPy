def f5(p = None):
	if p is None:
		p = ["f5"]
	return p
r = f5()
print(r)
r = f5("hi")
print(r)
r = f5()
print(r)
