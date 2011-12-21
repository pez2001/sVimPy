def e():
	o = 150
	n = o*o
	tossed = [False]*(o+1)

	p = 2
	while p*p < n:
		if(tossed[p] == False):
			print(p,", ")
			for j in range(p*p,o,p):
				tossed[j] = True
		p+= 1
e()