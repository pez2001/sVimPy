def func():
	x = 11
	x = -x
	print("x:", x , "\n")
	x = -x
	x = x % 2
	x = x * 5
	x = x * 5 - x * 15
	print("x:", x , "\n")
	x = +x
	print("x:", x , "\n")
	x = ~x
	print("x:", x , "\n")


	x = (x*10) ** 2
	print("x:", x , "\n")
	y = 12
	y = 12 | 15
	print("y:",y,"\n")
	return x

o = func()
print ("func ret:",o,"\n")
