from .core import lib, ffi
from .objects import Commit, Raw
from . import error


class RepoNotFoundException(error.GitException):
    pass


class Repo(object):
    def __init__(self, path):
        self._path = path
        self._repo = None

    def __del__(self):
        self.close()

    def commit(self, oid):
        return Commit(self, oid)

    def create(self, bare=False):
        repo = ffi.new('git_repository **')
        err = lib.git_repository_init(repo, self.path, bare)
        if err:
            self._repo = None
            raise error.GitException
        self._repo = repo[0]

    def close(self):
        if self._repo:
            lib.git_repository_free(self._repo)
            self._repo = None

    def open(self):
        if self._repo:
            return
        repo = ffi.new('git_repository **')
        err = lib.git_repository_open_ext(repo, self.path, 0, ffi.NULL)
        if err:
            if err == lib.GIT_ENOTFOUND:
                raise RepoNotFoundException(self.path)
            raise error.GitException
        self._repo = repo[0]

    @property
    def path(self):
        return self._path

    def raw(self, oid):
        return Raw(self, oid)

    @property
    def pointer(self):
        return self._repo
