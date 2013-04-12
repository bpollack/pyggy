from .core import lib, ffi
from . import error


def oid(sha):
    oid = ffi.new('git_oid *')
    if lib.git_oid_fromstrn(oid, sha, len(sha)):
        raise error.GitException
    return oid


def sha(oid):
    sha = ffi.new('char[40]')
    lib.git_oid_fmt(sha, oid)
    return ffi.string(sha)

propertycache = property
