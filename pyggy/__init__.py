from .core import lib


def startup(__initialized=[False]):
    if not __initialized[0]:
        __initialized[0] = True
        return lib.git_threads_init()
