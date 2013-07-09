from os.path import join as pathjoin

from .core import lib, ffi
from .objects import Commit, Oid, _Oid, Raw, ReferenceDb, Walker
from . import error


class RepoNotFoundException(error.GitException):
    pass


_HEX = frozenset(c for c in 'abcdef0123456789')


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

    def __contains__(self, rev):
        try:
            self[rev]
            return True
        except KeyError:
            return False

    def __getitem__(self, rev):
        return self.commit(self._resolve_rev(rev))

    def add_alternate(self, path):
        with open(pathjoin(self.odb_path, 'info', 'alternates'), 'ab+') as alternates:
            if alternates.tell() > 0:
                alternates.seek(-1)
                last = alternates.read(1)
                if last != '\n':
                    alternates.write('\n')
            alternates.write(path)
            alternates.write('\n')
        odb = ffi.new('git_odb **')
        if lib.git_repository_odb(odb, self._repo):
            raise error.GitException
        odb = odb[0]
        lib.git_odb_add_disk_alternate(odb, path)
        lib.git_odb_free(odb)

    def get_alternates(self):
        try:
            with open(pathjoin(self.odb_path, 'info', 'alternates'), 'rb') as f:
                return [l.strip() for l in f if l and not l.startswith('#')]
        except IOError:
            return []

    def _add_symbolic_reference(self, name, target):
        ref = ffi.new('git_reference **')
        if lib.git_reference_symbolic_create(ref, self._repo, name, target, 0):
            raise error.GitException
        else:
            lib.git_reference_free(ref[0])

    @property
    def branches(self):
        return ReferenceDb(self, 'refs/heads/')

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

    def pull(self, url):
        remote = ffi.new('git_remote **')
        if lib.git_remote_create_inmemory(remote, self._repo, 'refs/*:refs/*', url):
            raise error.GitException
        remote = remote[0]
        try:
            lib.git_remote_set_update_fetchhead(remote, 0)
            if (lib.git_remote_connect(remote, lib.GIT_DIRECTION_FETCH) or
                    lib.git_remote_download(remote, ffi.NULL, ffi.NULL) or
                    lib.git_remote_update_tips(remote)):
                raise error.GitException
        finally:
            if lib.git_remote_connected(remote):
                lib.git_remote_disconnect(remote)
            lib.git_remote_free(remote)

    @property
    def head(self):
        return self['HEAD'].oid.sha

    @property
    def tags(self):
        return ReferenceDb(self, 'refs/tags/')

    @property
    def odb_path(self):
        if lib.git_repository_is_bare(self._repo):
            return pathjoin(self.path, 'objects')
        else:
            return pathjoin(self.path, '.git', 'objects')

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

    def _resolve_rev(self, rev):
        if isinstance(rev, unicode):
            rev = rev.encode('utf8')

        # If what you've got looks like a SHA,
        # we're gonna take it as a SHA
        if isinstance(rev, _Oid) or (
                isinstance(rev, str) and
                len(rev) >= lib.GIT_OID_MINPREFIXLEN and
                all(c in _HEX for c in rev)):
            return rev
        ref = ffi.new('git_reference **')
        err = lib.git_reference_dwim(ref, self._repo, rev)
        if err:
            if err == lib.GIT_ENOTFOUND:
                raise KeyError(rev)
            raise error.GitException

        ref = ref[0]
        resolved_ref = ffi.new('git_reference **')
        if lib.git_reference_resolve(resolved_ref, ref):
            raise error.GitException
        resolved_ref = resolved_ref[0]
        rev = Oid(lib.git_reference_target(resolved_ref)).sha
        lib.git_reference_free(ref)
        lib.git_reference_free(resolved_ref)
        return rev
