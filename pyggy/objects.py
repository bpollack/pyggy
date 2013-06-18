import weakref

from .core import lib, ffi
from . import error, util


class Oid(object):
    def __init__(self, sha):
        if isinstance(sha, str) or isinstance(sha, unicode):
            self._sha = sha
            self._oid = util.oid(sha)
        else:
            self._sha = util.sha(sha)
            self._oid = sha

    @property
    def sha(self):
        return self._sha

    @property
    def oid(self):
        return self._oid

    def __len__(self):
        return len(self.sha)

    def __str__(self):
        return self.sha


class User(object):
    def __init__(self, git_signature):
        self.name = ffi.string(git_signature.name)
        self.email = ffi.string(git_signature.email)
        self.when = git_signature.when

    def __str__(self):
        return '%s <%s>' % (self.name, self.email)


class Raw(object):
    def __init__(self, repo, oid=None):
        self._repo = weakref.proxy(repo)
        self.oid = Oid(oid)
        self.data = None

    def read(self):
        odb = ffi.new('git_odb **')
        err = lib.git_repository_odb(odb, self._repo.pointer)
        if err:
            raise error.GitException
        odb = odb[0]
        odb_object = ffi.new('git_odb_object **')
        err = lib.git_odb_read(odb_object, odb, self.oid.oid)
        if err:
            raise error.GitException
        odb_object = odb_object[0]
        self.data = ffi.buffer(lib.git_odb_object_data(odb_object), lib.git_odb_object_size(odb_object))[:]
        lib.git_odb_object_free(odb_object)
        lib.git_odb_free(odb)


class Commit(object):
    def __init__(self, repo, oid=None):
        self._repo = weakref.proxy(repo)
        self.oid = Oid(oid)
        self._dirty = True

    def read(self):
        if not self._dirty:
            return
        commit = ffi.new('git_commit **')
        err = lib.git_commit_lookup_prefix(commit, self._repo.pointer,
                                           self.oid.oid, len(self.oid))
        if err:
            if err == lib.GIT_ENOTFOUND:
                raise KeyError
            raise error.GitException
        commit = commit[0]

        self._author = User(lib.git_commit_author(commit))
        self._committer = User(lib.git_commit_committer(commit))
        self._message = ffi.string(lib.git_commit_message(commit))
        encoding = lib.git_commit_message_encoding(commit)
        self._message_encoding = ffi.string(encoding) if encoding != ffi.NULL else None
        self._commit_time = lib.git_commit_time(commit)

        self._parent_ids = []
        for idx in xrange(lib.git_commit_parentcount(commit)):
            self._parent_ids.append(lib.git_commit_parent_id(commit, idx))
        self._parents = None

        self._tree_id = lib.git_commit_tree_id(commit)
        self._tree = None

        lib.git_commit_free(commit)

        self._dirty = False

    @property
    def author(self):
        self.read()
        return self._author

    @property
    def committer(self):
        self.read()
        return self._committer

    @property
    def message(self):
        self.read()
        return self._message

    def __repr__(self):
        return '<Commit(%s)>' % self.oid
