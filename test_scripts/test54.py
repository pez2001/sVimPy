def f(x, y, *args, z=3, t=4, **kwargs):
	print(x)
	print(y)
	print(args)
	print(z)
	print(t)
	print(kwargs)
 
f(1,2)
f("a","b","c","d")
f("a","b",z="c",t="d")
