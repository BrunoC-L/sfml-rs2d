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

for i,cx in enumerate(chunksX):
    for cy in chunksY:
        print(len(chunksY) * i + 1, ' out of ', len(chunksX) * len(chunksY))
        compresser2.compress(cx, cy, n, m)
        texturer3.texturer(cx, cy, n, m)
        waller.wall(cx, cy)
        chunker5.chunker(cx, cy)
