def f1():
	print("f1")
f1()
def f2():
	def f3():
		print("f3")
	f3()
f2()
a = [1,2]
def f4(a,b):
	print(a)
	print(b)
f4(*a)
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
def f6(a,b="Hi"):
	print(a)
	print(b)
f6("a","b")
f6("c")
def f7(a,b):
	print(a)
	print(b)

a = {"a":1,"b":2}
f7(**a)
def f8(a,b,c):
	print(a)
	print(b)
	print(c)
a = {"b":1,"c":2}
b = (3,)
f8(*b, **a)
b = (3,4,)
a = {"c":1,"d":2}
def f9(a,b,c,d):
	print(a)
	print(b)
	print(c)
	print(d)
f9(*b,**a)
def f10(a,b,c):
	print(a)
	print(b)
	print(c)

a = {"b":1,"c":2}
f10(3,**a)

