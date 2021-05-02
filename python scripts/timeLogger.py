from datetime import datetime

class TimeLogger:
    def __init__(self, count, maxCount, what):
        self.startTime = datetime.now()
        self.count = count
        self.maxCount = maxCount
        self.what = what

    def __enter__(self):
        return # could return self to give an API to log other times but right now the context has no use other than being destroyed
    
    def __exit__(self, *args, **kwargs):
        endTime = datetime.now()
        delta  = endTime - self.startTime
        cdelta = delta / self.count
        tdelta = cdelta * self.maxCount # the map is 29 by 25 chunks
        print(f'operation took {self.formatTimeDelta(delta)}')
        print(f'estimating {self.formatTimeDelta(cdelta)} per {self.what}')
        print(f'estimating {self.formatTimeDelta(tdelta)} for {self.maxCount} {self.plural(self.what)}')

    def formatTimeDelta(self, timeDelta):
        h, m, s = str(timeDelta).split(':')
        if h != '0':
            return f'{int(h)} hours and {int(m)} minutes'
        elif m != '00':
            return f'{int(m)} minutes and {float(s):.0f} seconds'
        else:
            return f'{float(s):.2f} seconds'

    def plural(self, s):
        if s[-3:] == 'oot':
            return s[:-3] + 'eet'
        elif s[-4:] == 'ooth':
            return s[:-4] + 'eeth'
        elif s[-1] == 'y':
            return s[:-1] + 'ies'
        return s + 's'
