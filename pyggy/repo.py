from .core import lib, ffi
from .objects import Commit, Oid, Raw, Walker
from . import error


class RepoNotFoundException(error.GitException):
    pass


class Repo(object):
    def __init__(self, path):
        self._path = path.encode('utf8') if isinstance(path, unicode) else path
        self._repo = None
        self._walker = None
        self._tree_cache = {}

    def __del__(self):
        self.close()

    def __enter__(self):
        self.open()
        return self

    def __exit__(self, type, value, traceback):
        self.close()

    def __getitem__(self, rev):
        ref = ffi.new('git_reference **')
        if isinstance(rev, unicode):
            rev = rev.encode('utf8')
        if not isinstance(rev, str):
            rev = rev.sha
        if not lib.git_reference_dwim(ref, self._repo, rev):
            ref = ref[0]
            resolved_ref = ffi.new('git_reference **')
            assert not lib.git_reference_resolve(resolved_ref, ref)
            resolved_ref = resolved_ref[0]
            rev = Oid(lib.git_reference_target(resolved_ref)).sha
            lib.git_reference_free(ref)
            lib.git_reference_free(resolved_ref)
        return self.commit(rev)

    def branches(self):
        heads = {}

        @ffi.callback('int(char *, git_branch_t, void *)')
        def add_branch(name, type, payload):
            oid = ffi.new('git_oid *')
            name = ffi.string(name)
            if not lib.git_reference_name_to_id(oid, self._repo, 'refs/heads/' + name):
                heads[name] = Oid(oid).sha
            return 0

        lib.git_branch_foreach(self._repo, lib.GIT_BRANCH_LOCAL, add_branch, ffi.NULL)
        return heads

    def commit(self, oid):
        return Commit(self, oid)

    def create(self, bare=False):
        repo = ffi.new('git_repository **')
        if lib.git_repository_init(repo, self.path, bare):
            raise error.GitException
        self._repo = repo[0]

    def close(self):
        if self._repo:
            if self._walker:
                self._walker.close()
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
    def walker(self):
        if self._repo and not self._walker:
            self._walker = Walker(self)
        return self._walker

    @property
    def path(self):
        return self._path

    def raw(self, oid):
        return Raw(self, oid)

    @property
    def pointer(self):
        return self._repo
