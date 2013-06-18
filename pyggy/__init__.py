from .core import lib


def startup():
    return lib.git_threads_init()
