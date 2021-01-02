from objecter import getObjects
import texturer3
import waller
import chunker5

chunksX = range(29)
# chunksX = [18]
chunksY = range(25)
# chunksY = [13, 14]
objects = getObjects()

for i,cx in enumerate(chunksX):
    for j,cy in enumerate(chunksY):
        print(f'{len(chunksY) * i + j} out of {len(chunksX) * len(chunksY)}')
        texturer3.texturer(cx, cy)
        waller.wall(cx, cy)
        chunker5.chunker(cx, cy, objects)

print(f'{len(chunksX) * len(chunksY)} out of {len(chunksX) * len(chunksY)}')
