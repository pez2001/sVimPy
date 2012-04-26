def main(x,l):
	if(l==2):
		return
	main("2nd",l+1)
	print("main:",x,",l:",l)
main("1st",0)
