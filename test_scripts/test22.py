def f():
	def g():
		print("g executed\n")
	g()	
	print("f executed\n")
f()
