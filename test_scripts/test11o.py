def func():
	def sub_func():
		print("hi from subfunc\n")
	x = 1
	x = x + 1
	x = x + x + 2
	x = x + x - 1
	x = rek(x)
	print("x:",x,"\n")
#	x = "Hallo Welt"
#	print("x:",x,"\n")
#	y = "Gute Nacht"
#	print("y:",y,"\n")
#	v = "Und bis morgen"
#	print("y:"+ y+ " " + v +"\n")
	sub_func()

def rek(e):
	if(e < 100000):
		e = rek(e*e-1)
	return(e)

func()

