class test():
	def main(self,x,l):
		if(l==2):
			return
		self.main("2nd",l+1)
		print("main:",x,",l:",l)
t = test()
t.main("1st",0)
