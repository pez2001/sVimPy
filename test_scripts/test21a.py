a = 25
def f():
	global a
	a = "hi"
	print("f:",a,"\n")
	del a
f()
a = "new"
print("m:"+a+"\n")

