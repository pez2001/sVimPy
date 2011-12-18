a = 25
def f():
	global a
	a = "hi"
	print("f:",a,"\n")
	del a
	b = 0
	g = [0]
	g[b] += 1
	del b
	c = 1
	del c
	j = [1,2]
	del j[1]
	(ji,ki) = "hi"
	print("ji:",ji,"\n")
	print("ki:"+ki+"\n")
f()
a = "new"
print("m:"+a+"\n")
d = 1
del d

