import os
import sys


class Headers(object):
    def __init__(self, mod):
        self.mod = mod
        self.headers = {}
        for name in os.listdir(os.path.dirname(__file__)):
            if name.endswith('.h'):
                with open(os.path.join(os.path.dirname(__file__), name), 'rb') as f:
                    self.headers[name[:-2]] = f.read()

    def __getattr__(self, attr):
        try:
            return getattr(self.mod, attr)
        except AttributeError:
            return self.headers[attr]

sys.modules[__name__] = Headers(sys.modules[__name__])
