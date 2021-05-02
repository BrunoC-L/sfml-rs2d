'''
This script is in charge of taking objects from "assets/data/objects/"
and creating the texture file in "assets/textures/objects/texture.png"

along with "assets/data/objectName2texture.txt" *

*   we need a file like that because the relationship between objects
    and textures is not 1 to 1, the textures are in a tilemap, which
    is a structure for quickly drawing alot of objects based on a
    texture index in a texture file, those files can be like 256 * 256
    textures of 32x32 px for example. The relationship is not 1 to 1
    because some objects are larger than 1 tile, for example trees are 2x2
    tiles so they will use up 4 textures, but a tree is a single object.

we will map each of the tile to a new name like [tree-x-y] for x, y in [00,01,10,11]
this step is also assumed to be done by the other users of the tree object file and
the objectName2texture.txt file will containg entries like "tree-0-0: 26" to represent
that the tile at 0,0 of object named tree is at the 26th index in the texture file

of course, since for images, positive y goes down, the ids
of a 3x3 object would be like this on the map facing north:
obj0-0  obj1-0  obj2-0
obj0-1  obj1-1  obj2-1
obj0-2  obj1-2  obj2-2
'''

def objecter(args):
    verbose = not 'silence' in args
    
    from os import listdir
    import json
    from PIL import Image
    from constants import PPT, OBJECT_TEXTURE_FILE_SIZE
    objectsDir = '../assets/data/objects'
    objects = listdir(objectsDir)
    objects.sort()
    if verbose:
        print(f'found {len(objects)} objects')
    textures = Image.new('RGB', (PPT * OBJECT_TEXTURE_FILE_SIZE, PPT * OBJECT_TEXTURE_FILE_SIZE))
    textures.putalpha(0)
    textureIndex = 1 # leave 0 transparent
    with open('../assets/data/objectName2texture.txt', 'w+') as map:
        with open('../assets/data/objectNames.txt', 'w+') as set:
            for object in objects:
                with open(f'{objectsDir}/{object}/object.json', 'r') as f:
                    content = json.load(f)
                    objectName = object
                    if verbose:
                        print(objectName)
                    imageNames = listdir(f'{objectsDir}/{object}/img')
                    for imageName in imageNames:
                        image = Image.open(f'{objectsDir}/{object}/img/{imageName}')
                        x_offset = PPT * (textureIndex  % OBJECT_TEXTURE_FILE_SIZE)
                        y_offset = PPT * (textureIndex // OBJECT_TEXTURE_FILE_SIZE)
                        textures.paste(image, (x_offset, y_offset))
                        x_y = imageName.split('.')[0] # 0-0.png -> 0-0
                        map.write(f'{objectName}: {x_y}: {textureIndex}\n')
                        textureIndex += 1
                    set.write(f'{objectName}\n')
    textures.save('../assets/textures/objects.png', 'png')
