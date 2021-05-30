'''
This script is in charge of taking objects from "resource/objects/"
and creating the texture file in "assets/textures/objects.png"
along with "resource/objects/objectName2texture.txt", we need
a file like that because the relationship between objects
and textures is not 1 to 1, the textures are in a tilemap, and
some objects are larger than 1 tile, for example trees are 2x2
tiles so they will use up 4 textures, but a tree is a single object.
'''

def objecter(args):
    verbose = not 'silence' in args
    
    from os import listdir
    import json
    from PIL import Image
    from constants import PPT, OBJECT_TEXTURE_FILE_SIZE
    
    objectsDir = '../resource/objects/list'
    objects = listdir(objectsDir)
    objects.sort()
    if verbose:
        print(f'found {len(objects)} objects')
    textures = Image.new('RGB', (PPT * OBJECT_TEXTURE_FILE_SIZE, PPT * OBJECT_TEXTURE_FILE_SIZE))
    textures.putalpha(0)
    textureIndex = 1 # leave 0 transparent
    with open('../resource/objects/objectName2texture.txt', 'w+') as map:
        for object in objects:
            with open(f'{objectsDir}/{object}/object.json', 'r') as f:
                if verbose:
                    print(object)
                content = json.load(f)
                for i, c in enumerate(content):
                    imageNames = listdir(f'{objectsDir}/{object}/img/{i}')
                    for imageName in imageNames:
                        image = Image.open(f'{objectsDir}/{object}/img/{i}/{imageName}')
                        x_offset = PPT * (textureIndex  % OBJECT_TEXTURE_FILE_SIZE)
                        y_offset = PPT * (textureIndex // OBJECT_TEXTURE_FILE_SIZE)
                        textures.paste(image, (x_offset, y_offset))
                        x_y = imageName.split('.')[0] # 0-0.png -> 0-0
                        map.write(f'{object}-{i}: {x_y}: {textureIndex}\n')
                        textureIndex += 1
    textures.save('../assets/textures/objects.png', 'png')