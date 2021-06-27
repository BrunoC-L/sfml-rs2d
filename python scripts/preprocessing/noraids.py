from PIL import Image

# This is not meant to be ran anymore, just once to create mapnoraids.jpg

image = Image.open('../assets/map.jpg')
image.crop((3584, 0, 11008, 6400)).save('mapnoraids.jpg')
