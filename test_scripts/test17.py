x = "hi"
def f():
	x = (1,2,3,5)
	v = (1,4)
	y = [1,"hi",3,99]
	y[3]= v[1]
	print("v:",v)
	print("y:",y)
	for a in range(1,4):
		print("y[",a,"]:",y[a])
	for b in y:
		print("b:",b)
	print("f thru\n")
f()
print(x+"\n")
