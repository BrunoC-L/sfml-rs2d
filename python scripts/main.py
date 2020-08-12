import compresser2
import texturer3
import waller
import chunker5

n = 10
m = 26
chunksX = range(29)
#chunksX = [17, 18, 19]
chunksY = range(25)
#chunksY = [13, 14, 15]

# for i,cx in enumerate(chunksX):
#     for cy in chunksY:
#         print(len(chunksY) * i + 1, ' out of ', len(chunksX) * len(chunksY))
#         compresser2.compress(cx, cy, n, m)
#         texturer3.texturer(cx, cy, n, m)
#         waller.wall(cx, cy)
#         chunker5.chunker(cx, cy)


a = 1
b = 1
c = 1
def x():
    return a if b else c

def y():
    return b and a or c

def log(f):
    def wrapper():
        print('begin')
        f()
        print('end')
    return wrapper

@log
def g():
    print('middle')

g = log(g)

g()

x = lambda y : y * y

print()

myobject = ("Bruno", "Ici", "Dog")
name, addr, pet = myobject
x = 2

