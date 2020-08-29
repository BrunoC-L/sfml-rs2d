def getObjects():
    files = [
        'tree',
        'copper ore',
    ]
    objectMap = {}
    for filename in files:
        f = open(f'./objectPositions/{filename} positions.txt', 'r')
        itemLine = f.readline().replace('\n', '')
        while(line := f.readline()):
            coords = line.split(',')
            x, y = int(coords[0]), int(coords[1])
            coords = (x, y)
            old = objectMap.get(coords)
            old = old + '////' if old else ''
            objectMap.update({
                coords: old + itemLine.replace('posx', str(x)).replace('posy', str(y))
            })
    return objectMap
