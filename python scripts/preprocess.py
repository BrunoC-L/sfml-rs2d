def main(args):
    arg = args[0].lower()
    args = [arg.lower() for arg in args[1:]]

    if arg == 'objecter':
        from preprocessing.objecter import objecter
        objecter(args)

if __name__ == '__main__':
    import sys
    args = [arg.lower() for arg in sys.argv[1:]]
    main(args)
