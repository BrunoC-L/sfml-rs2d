import json

'''
This file is meant to be ran once after modifying data, either adding new
objects or using the editor to add some objects in game

So far there is just 1 operation but I could imagine adding some with later
Patches, that operation is the editor script which applies changes made through
The UI from the Editor Program in src/map editor, there is a file that stores
the name of each file that has been modified since this script was last ran.
The editor script uses data from /assets/editor/data/ that data is organised
by chunk but separated in 1 file per tile, it is better for the editor but
I didn't see my server loading 29 * 25 * 64 * 64 files and readingthrough the JSON!

editor takes data by tile and reorganise it into type then chunk file
ex: assets/walls/*.txt files, same for monsters, etc. those are faster to read for the
server when booting up, otherwise it would completely kill the program... Though I should
probably use a thread pool to load chunks... TODO
'''

def main(args):
    args = [a.lower() for a in args]
    verbose = not 'silence' in args
    
    from os import listdir, remove
    changesDir = '../assets/editor/changes'
    changes = listdir(changesDir)
    changesDone = listdir(changesDir + ' done')
    if verbose:
        print(f'Applying {len(changes)} changes')

    keys = ['walls', 'monsters', 'npcs', 'objects', 'items']

    for change in changes: # yes, the changes are filenames
        if change in changesDone:
            continue
        cx, cy, x, y, z = change.split()
        with open(f'../assets/editor/data/chunks/{cx}-{cy}-{z}/{x}-{y}.json') as f:
            content  = json.load(f)
            # get walls, objects, monsters, npcs, put them in their file
            for key in keys:
                value = content[key]
                # We just append, ideally we would read the entire file but there is 
                # not much to be gained from not reading duplicate entries in the file,
                # where there is alot to be gained from not having to read the entire file now
                # (in program complexity and execution time)
                with open(f'../assets/{key}/{cx}-{cy}-{z}.txt', 'a') as chunk:
                    # for walls value is between 0 and 15
                    # for other types it's a list of folder names in the key folder
                    # ex [tree] would indicate a list of 1 tree, very complicated I KNOW
                    chunk.write(f'{x}-{y}: {value}\n')
        open(f'{changesDir} done/{cx} {cy} {z} {x} {y}', 'w+')

if __name__ == '__main__':
    import sys
    main(sys.argv[1:])
