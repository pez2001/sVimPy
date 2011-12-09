def func():
	def sub_func():
		print("hi from subfunc\n")
	x = 1
	x = x + 1
	x = (x + x + 2 )  / 2
	x = (x + x - 1) 
	x = rek(x)
	print("x:",x,"\n")
	x = "Hallo Welt"
	print("x:",x,"\n")
#	y = "Gute Nacht"
#	print("y:",y,"\n")
#	v = "Und bis morgen"
#	print("y:"+ y+ " " + v +"\n")
	sub_func()
	if("hi" == "hi"):
		print("== string compare added\n")

	if(1 == 1):
		print("== num compare added\n")
	x = 11
	x = -x
	x = -x
	x = x % 2
	print("x:", x , "\n")
	x = (x*10) ** 2
	print("x:", x , "\n")
	y = 12
	y = 12 | 15
	print("y:",y,"\n")
	if((x == 100) or (y == 15)):
		print("or\n")
	return x

def rek(e):
	if(e < 1000):
		e = rek(e*e)
	return(e)

o = func()
print ("o:",o,"\n")
