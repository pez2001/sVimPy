class test:
	def show(self, x):
		print("x:"+x+",i:"+self.i)

class test2(test): pass


def main():
	c = test2()
	c.i = "i set"
	c.show("inter")
	print("main")
main()
