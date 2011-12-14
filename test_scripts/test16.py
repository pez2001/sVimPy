def f():
	for a in (1,2,3):
		for b in (1,2,3,4,5):
			if(b == 1):
				continue
			print("a:",a,"b:",b,"\n")
			if(b == 3):
				break
f()
print("thru\n")
o = 10
o **= 2
print(o)
