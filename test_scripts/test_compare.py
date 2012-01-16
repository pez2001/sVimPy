def func():
	i = 0
	if(i < 1):
		print("cmp0 works")
	i = 1
	if(i <= 1):
		print("cmp1 works")
	if(i == 1):
		print("cmp2 works")
	i = 0
	if(i != 1):
		print("cmp3 works")
	i = 2
	if(i > 1):
		print("cmp4 works")
	i = 1
	if(i >= 1):
		print("cmp5 works")
	a = (1,2)
	if(i in a):
		print("cmp6 works")
	i = 3
	if(i not in a):
		print("cmp7 works")
	i = 1
	b = 1
	if(i is b):
		print("cmp8 works")
	b = 1.1
	if(i is not b):
		print("cmp9 works")

func()

