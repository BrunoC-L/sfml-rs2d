import json
from constants import MAP_SIZE_CHUNKS, TPC

'''
This file is meant to be ran once after modifying data, either adding new
objects or using the editor to add some objects in game

So far there is just 1 operation but I could imagine adding some with later
Patches, that operation is the editor script which applies changes made through
The UI from the Editor Program in src/map editor, there is a file that stores
the name of each file that has been modified since this script was last ran.
The editor script uses data from /resource/editor/data/ that data is organised
by chunk but separated in 1 file per tile, it is better for the editor but
I couldn't see my server loading 29 * 25 * 64 * 64 files (TPC=64)

editor takes data by tile and reorganise it into type then chunk file
ex: assets/walls/*.txt files, same for monsters, etc. those are faster to read for the
server when booting up, otherwise it would completely kill the program.
'''

def main(args):
    args = [a.lower() for a in args]
    verbose = not 'silence' in args
    all = 'all' in args
    from os import listdir
    changesDir = '../resource/editor/changes'
    changes = listdir(changesDir)

    if not all:
        changesDone = listdir(changesDir + ' done')
        changes = [c for c in changes if c not in changesDone]
        
    if verbose:
        print(f'Applying {len(changes)} changes')

    keys = [k for k in ['wall', 'monster', 'npc', 'object', 'item'] if 'no' + k not in args]
    if all:
        for key in keys:
            for cx in range(MAP_SIZE_CHUNKS["x"]):
                for cy in range(MAP_SIZE_CHUNKS["y"]):
                    for cz in range(1):
                        try:
                            open(f'../resource/chunks-{TPC}/{key}s/{cx}-{cy}-{cz}.txt', 'w+')
                        except:
                            pass

    for change in changes:
        cx, cy, cz, x, y = change.split()
        with open(f'../resource/editor/data/chunks-{TPC}/{cx}-{cy}-{cz}/{x}-{y}.json') as f:
            content  = json.load(f)
            # get walls, objects, monsters, npcs, put them in their file
            for key in keys:
                # for walls value is between 0 and 15
                # for other types it's a list of folder names in the key folder
                # ex value = ['Tree']
                value = content[key]
                # We just append, ideally we would read the entire file but there is 
                # not much to be gained from not reading duplicate entries in the file,
                # where there is alot to be gained from not having to read the entire file now
                # (in program complexity and execution time)
                for m in ['a', 'w+']: # first try to append, otherwise create file
                    try :
                        with open(f'../resource/chunks-{TPC}/{key}s/{cx}-{cy}-{cz}.txt', m) as chunk:
                            chunk.write(f'{x}-{y}:{value}\n')
                            break
                    except:
                        pass
        open(f'{changesDir} done/{cx} {cy} {cz} {x} {y}', 'w+')

if __name__ == '__main__':
    import sys
    main(sys.argv[1:])
