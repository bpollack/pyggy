from .core import lib, ffi


class GitException(Exception):
    def __init__(self):
        error = lib.giterr_last()
        message = ffi.string(error.message) if error != ffi.NULL else 'unknown error'
        super(GitException, self).__init__(message)
