基于python c/api的c实现的linklist

使用方法1:

```
gcc -shared -fPIC link_list.c -I/usr/include/python2.7/ -L/usr/lib -lpython2.7 -o linklist.so

```

使用方法2:

```
python setup.py install
```