from constants import CHUNKS

'''
This script  is the basis for the game data
It uses the RS Map to split it into chunks
So that the client can load only some chunks
And it uses map coloring to try to place walls
This process takes a while So I recommend
not trying it yourself, the data it
generates is already on git!
'''

def main(args):
    args = [a.lower() for a in args]
    verbose = not 'silence' in args
    esttime = not 'notime' in args

    texture = not 'notext' in args
    walls   = not 'nowall' in args

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
    if walls:
        from chunks.waller import waller
        ops += [waller]

    def operate():
        for cx, cy in CHUNKS:
            for op in ops:
                op(cx, cy)
        if verbose: print(f'\r{l} / {l}')

    if esttime:
        from timeLogger import TimeLogger
        with TimeLogger(l, 29 * 25, 'chunk'): operate()
    else:
        operate()

if __name__ == '__main__':
    import sys
    main(sys.argv[1:])
