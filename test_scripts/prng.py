rnd_seed = 121294348
def rnd():
	global rnd_seed
	a = 214013
	b = 2531011
	m = pow(2,32)
	x = (rnd_seed * a + b) % m
	rnd_seed = x
	return x

def rnd_r(range):
	return rnd() % (range + 1)

def rnd_mr(min,max):
	return min + rnd() % (max + 1 - min)

def list():
	rm = 50
	ra = 100
	num = 100
	for i in range(num):
		r = rnd_mr(rm,ra)
		print("r[",i,"]:",r)

list()