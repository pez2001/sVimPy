def e():
	o = 35000
	n = o*o
	tossed = [False]*(o+1)

	p = 2
	while p*p < n:
		if(tossed[p] == False):
			print(p,", ")
			for j in range(p,o,p):
				tossed[j] = True
		p+= 1
e()