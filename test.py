import linklist

l = linklist.LinkList()
l.append("zhao")
l.append(3)
for i in l:
    print i
print l
del l
print type(linklist)
