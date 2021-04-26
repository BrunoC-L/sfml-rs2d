from constants import CHUNKS

'''
This file is meant to be ran once with all chunks enabled in constants.py
And all options enabled (no disabling scripts through sys.argv)

The result from doing this is the generation of the textures for each chunk,
The objects in each chunk, the monsters, the NPC, the items, the walls, etc
'''

def main(args):
    args = [a.lower() for a in args]
    verbose = not 'silence' in args
    texture = not 'notext' in args
    objects = not 'noobj' in args
    walls   = not 'nowall' in args
    esttime = not 'notime' in args

    ops = []
    l = len(CHUNKS)

    if verbose:
        global count
        count = 0
        def printCurrent(*_):
            global count
            count += 1
            print(f'\r{count} / {l}', end='')
        ops += [printCurrent]
    if texture:
        from chunks.texturer import texturer
        ops += [texturer]
    if objects:
        from chunks.objecter import objecter
        ops += [objecter]
    if walls:
        from chunks.waller import waller
        ops += [waller]

    def operate():
        for cx, cy in CHUNKS:
            for op in ops:
                op(cx, cy)
        if verbose: print(f'\r{l} / {l}')

    if esttime:
        from timer import TimeLogger
        with TimeLogger(l, 29 * 25, 'chunk'): operate()
    else:
        operate()

if __name__ == '__main__':
    import sys
    main(sys.argv[1:])
