# -*- coding: utf-8 -*-

import linklist

l = linklist.LinkList()
l.append("张三")
l.append("12")
l.append(666)

for i in l:
    print i

print len(l)
print l[1]
