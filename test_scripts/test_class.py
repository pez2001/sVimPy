class test_class:
	def __init__(self,i):
		self.reset(i)
	def reset(self,i):
		self.i = i
	def show(self, x):
		print("x:"+x+",i:"+self.i)

def main():
	c = test_class("init")
	c.show("inter")
	print("main")
main()
