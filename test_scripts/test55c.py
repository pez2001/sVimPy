def f(x=1, y=2, *args, z=3, t=4, **kwargs):
	print(x)
	print(y)
	print(args)
	print(z)
	print(t)
	print(kwargs)
f("a","b",z="c",t="d")