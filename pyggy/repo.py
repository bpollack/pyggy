from os.path import join as pathjoin

from .core import lib, ffi
from .objects import Commit, Oid, _Oid, Raw, ReferenceDb, Walker
from . import error


class RepoNotFoundException(error.GitException):
    """represents that a repository could not be foudn on disk"""
    pass


_HEX = frozenset(c for c in 'abcdef0123456789')


class Repo(object):
    """represents a bare or full Git repository

    Repo objects should ideally be handled inside a Python with
    construct for proper resource management, but will also handle
    themselve correctly on CPython via ref counting if they
    are not.  Do note that you at must call .open() if you are
    not using the object in a with block, however.
    """

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
        """checks whether a given SHA, ref, or tag exists

        Note that this performs exactly the same amount of work as
        simply loading the commit in the first place, so you will be
        best off trying to grab a commit in a try/catch block, rather
        than testing for it ahead of time, in the common context.
        """
        try:
            self[rev]
            return True
        except KeyError:
            return False

    def __getitem__(self, rev):
        """returns a reified commit object by SHA, ref, or tag

        Throws a KeyError if the ref or tag cannot be resolved, or
        if the provided SHA does not exist.
        """
        return self.commit(self._resolve_rev(rev))

    def add_alternate(self, path, permanent=True):
        """adds an alternate to the object lookup store

        Note that this method defaults to permanently storing the alterate
        on disk.  If you do not want that behavior, be sure to specify
        permanent=False when calling.
        """
        if permanent:
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
        """gets the full list of alternates

        Note that get_alternates does not currently return the
        computed list of alternates, but rather the *permanent*
        alternates.  This behavior will likely change in a future
        release.
        """
        try:
            with open(pathjoin(self.odb_path, 'info', 'alternates'), 'rb') as f:
                return [l.strip() for l in f if l and not l.startswith('#')]
        except IOError:
            return []

    @property
    def branches(self):
        """an editable dict-like object representing all branches"""
        return ReferenceDb(self, 'refs/heads/')

    def commit(self, oid):
        """return a commit from a given SHA or OID

        Note that ref and tag lookup is not performed here; use the
        generic __getattr__ behavior on this object for that.
        """
        return Commit(self, oid)

    def create(self, bare=False):
        """create the repository on disk

        Throws a GitException if the repository already exists, or
        could not be created.
        """
        repo = ffi.new('git_repository **')
        if lib.git_repository_init(repo, self.path, bare):
            raise error.GitException
        self._repo = repo[0]

    def close(self):
        """close this repository and all associated handles

        Note that any objects that belong to this repository
        once you close it will be an indefinite state, and working
        with them will likely hard-crash your program.

        This method may safely be called multiple times."""
        if getattr(self, '_repo', None):
            if getattr(self, '_walker', None):
                self._walker.close()
            lib.git_repository_free(self._repo)
            self._repo = None

    def open(self):
        """open the repository for work

        This method may safely be called multiple times.
        """
        if self._repo:
            return
        repo = ffi.new('git_repository **')
        err = lib.git_repository_open_ext(repo, self.path, 0, ffi.NULL)
        if err:
            if err == lib.GIT_ENOTFOUND:
                raise RepoNotFoundException(self.path)
            raise error.GitException
        self._repo = repo[0]

    def mirror(self, url):
        """make this repository mirror another

        This will ensure that all extraneous refs have been pruned,
        that all foreign refs and tags exist locally, and that you will
        go to space today--provided you had no local changes.

        You probably want to use the pull() or push() methods for the
        common case instead."""
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

            # prune no-longer-existent stuff
            existing_branches = set(self.branches)
            remote_branches = set()

            @ffi.callback('int(git_remote_head *, void *)')
            def add_remote_branch(remote_head, payload):
                remote_branches.add(ffi.string(remote_head.name))
                return 0

            lib.git_remote_ls(remote, add_remote_branch, ffi.NULL)
            for branch in existing_branches - remote_branches:
                del self.branches[branch]
        finally:
            if lib.git_remote_connected(remote):
                lib.git_remote_disconnect(remote)
            lib.git_remote_free(remote)

    @property
    def head(self):
        return self['HEAD'].oid.sha

    @head.setter
    def head(self, branch):
        ref = ffi.new('git_reference **')
        if lib.git_reference_symbolic_create(ref, self._repo, 'HEAD', branch, 1):
            raise error.GitException
        lib.git_reference_free(ref[0])

    @property
    def tags(self):
        return ReferenceDb(self, 'refs/tags/')

    @property
    def odb_path(self):
        """the path on disk to this repository's object store

        You probably want Repo.path in most cases instead."""
        if lib.git_repository_is_bare(self._repo):
            return pathjoin(self.path, 'objects')
        else:
            return pathjoin(self.path, '.git', 'objects')

    @property
    def walker(self):
        """the repository's default walker

        Each repository only has one default walker.  If you want
        to perform multiple walks at once, you will need to create
        your own Walker objects manually.
        """
        if self._repo and not self._walker:
            self._walker = Walker(self)
        return self._walker

    @property
    def path(self):
        """return the path to the main repository on disk"""
        return self._path

    def raw(self, oid):
        """return the raw object for a given OID, without parsing

        This method will likely change or disappear in a future release.
        """
        return Raw(self, oid)

    @property
    def pointer(self):
        """the underlying C pointer for this repository

        If you are not writing part of pyggy itself, you should never,
        ever call this method.
        """
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
