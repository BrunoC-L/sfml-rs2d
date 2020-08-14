from PIL import Image
image = Image.open('../assets/map.jpg')
image.crop((3584, 0, 11008, 6400)).save('other.jpg')
