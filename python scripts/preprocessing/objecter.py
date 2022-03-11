'''
This script is in charge of taking objects from "resource/objects/"
and creating the texture file in "assets/textures/objects.png"
along with "resource/objects/objectName2texture.txt", we need
a file like that because the relationship between objects
and textures is not 1 to 1, the textures are in a tilemap, and
some objects are larger than 1 tile, for example trees are 2x2
tiles so they will use up 4 textures, but a tree is a single object.
Also multi-state objects can use mutiple textures per tile
'''

def objecter(args):
    verbose = not 'silence' in args
    
    from os import listdir, path
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
            texturesjsonarr = []
            for dir in listdir(f'{objectsDir}/{object}/img/'):
                if path.isdir(f'{objectsDir}/{object}/img/{dir}'):
                    texturesjsonarr += [[]]
                    imageNames = listdir(f'{objectsDir}/{object}/img/{dir}')
                    for imageName in imageNames:
                        image = Image.open(f'{objectsDir}/{object}/img/{dir}/{imageName}')
                        image.thumbnail((PPT, PPT))
                        x_offset = PPT * (textureIndex  % OBJECT_TEXTURE_FILE_SIZE)
                        y_offset = PPT * (textureIndex // OBJECT_TEXTURE_FILE_SIZE)
                        textures.paste(image, (x_offset, y_offset))
                        x_y = imageName.split('.')[0] # 0-0.png -> 0-0
                        map.write(f'{object}-{dir}: {x_y}: {textureIndex}\n')
                        texturesjsonarr[-1] += [textureIndex]
                        textureIndex += 1
            objfilecontentbeforetextures = json.load(open(f'{objectsDir}/{object}/object.json', 'r'))
            objfilecontentbeforetextures['textures'] = texturesjsonarr
            with open(f'{objectsDir}/{object}/object.json', 'w+') as objfile:
                objfile.write(json.dumps(objfilecontentbeforetextures, indent='\t') + "\n")
    textures.save(f'../assets/textures-{PPT}/objects.png', 'png')

if __name__ == '__main__':
    objecter([])
