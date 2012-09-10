def p_item(a):
	return(a*2)

global a
a="hallo"
b=["a","b","c"]

print("b:")
c = map(p_item,b)
for t in c:
	print(t)
print("id:")
print(id(a))
print("globals tuple:")
print(globals())
print("locals tuple:")
print(locals())
