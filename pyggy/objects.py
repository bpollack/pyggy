from collections import defaultdict, namedtuple
import weakref

from .core import lib, ffi
from . import error, util


def _infinitedict():
    return defaultdict(_infinitedict)


def Oid(sha):
    if isinstance(sha, _Oid):
        return sha
    return _Oid(sha)


class _Oid(object):
    def __init__(self, sha):
        if isinstance(sha, basestring):
            self._sha = str(sha)
            self._oid = util.oid(self._sha)
        else:
            self._sha = util.sha(sha)
            self._oid = sha

    @property
    def sha(self):
        return self._sha

    @property
    def pointer(self):
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
        self._repo = weakref.ref(repo)
        self.oid = Oid(oid)
        self.data = None

    def read(self):
        odb = ffi.new('git_odb **')
        if lib.git_repository_odb(odb, self._repo().pointer):
            raise error.GitException
        odb = odb[0]
        try:
            odb_object = ffi.new('git_odb_object **')
            if lib.git_odb_read(odb_object, odb, self.oid.pointer):
                raise error.GitException
            odb_object = odb_object[0]
            self.data = ffi.buffer(lib.git_odb_object_data(odb_object), lib.git_odb_object_size(odb_object))[:]
            lib.git_odb_object_free(odb_object)
        finally:
            lib.git_odb_free(odb)


class Commit(object):
    def __init__(self, repo, oid=None):
        self._repo = weakref.ref(repo)
        self.oid = Oid(oid)
        self._dirty = True

    def read(self):
        if not self._dirty:
            return
        commit = ffi.new('git_commit **')
        err = lib.git_commit_lookup_prefix(commit, self._repo().pointer,
                                           self.oid.pointer, len(self.oid))
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
            self._parent_ids.append(Oid(lib.git_commit_parent_id(commit, idx)).sha)
        self._parents = None

        self._tree = Tree(self._repo(), lib.git_commit_tree_id(commit))

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

    @property
    def parent_ids(self):
        self.read()
        return self._parent_ids

    @property
    def sha(self):
        return self.oid.sha

    @property
    def tree(self):
        self.read()
        return self._tree

    def __repr__(self):
        return '<Commit(%s)>' % self.oid


class TreeEntry(namedtuple('TreeEntry', ('name', 'sha', 'mode'))):
    pass


class Tree(object):
    def __init__(self, repo, oid):
        self._repo = weakref.ref(repo)
        self.oid = Oid(oid)
        self._cache = repo._tree_cache
        self._dirty = True
        self._manifest = None

    def read(self):
        if not self._dirty:
            return
        tree = ffi.new('git_tree **')
        if lib.git_tree_lookup(tree, self._repo().pointer, self.oid.pointer):
            raise error.GitException
        tree = tree[0]

        sha_map = {}
        trees = _infinitedict()

        @ffi.callback('int(const char *, const git_tree_entry *, void *)')
        def add_tree(root, entry, payload):
            root = ffi.string(root).strip('/')
            base = trees
            if root:
                for directory in root.split('/'):
                    base = base[directory]
            name = ffi.string(lib.git_tree_entry_name(entry))
            mode = lib.git_tree_entry_filemode(entry)
            sha = Oid(lib.git_tree_entry_id(entry)).sha
            if mode & lib.GIT_FILEMODE_TREE:
                if sha in self._cache:
                    base[name] = self._cache[sha]
                    return 1
                else:
                    sha_map[sha] = base[name]
                    return 0
            else:
                base[name] = (sha, mode)
                return 0

        lib.git_tree_walk(tree, lib.GIT_TREEWALK_PRE, add_tree, ffi.NULL)
        lib.git_tree_free(tree)

        self._cache.update(sha_map)
        self._manifest = {}
        self._flatten('', trees, self._manifest)

    @property
    def manifest(self):
        self.read()
        return self._manifest

    def _flatten(self, base, tree, manifest):
        for k, v in tree.viewitems():
            full = k if not base else base + '/' + k
            if isinstance(v, tuple):
                manifest[full] = v
            else:
                self._flatten(full, v, manifest)


class Walker(object):
    def __init__(self, repo):
        self._repo = weakref.ref(repo)
        self._walker = None
        self._walking = False
        self._limit = None

    def close(self):
        if self._walker:
            lib.git_revwalk_free(self._walker)
            self._walker = None

    def open(self, include=[], exclude=[], limit=None):
        self._ensure_walker_allocated()
        lib.git_revwalk_reset(self._walker)
        if not include:
            include = self._repo().branches().viewvalues()
        for sha in include:
            lib.git_revwalk_push(self._walker, Oid(str(sha)).pointer)
        for sha in exclude:
            lib.git_revwalk_hide(self._walker, Oid(str(sha)).pointer)
        lib.git_revwalk_sorting(self._walker, lib.GIT_SORT_TOPOLOGICAL)
        self._limit = limit
        self._walking = True

    def __iter__(self):
        if not self._walking:
            self.open()
            self._remaining = self._limit
        return self

    def next(self):
        oid = ffi.new('git_oid *')
        if self._remaining == 0 or lib.git_revwalk_next(oid, self._walker) == lib.GIT_ITEROVER:
            self._walking = False
            raise StopIteration
        if self._remaining is not None:
            self._remaining -= 1
        return Oid(oid)

    def _ensure_walker_allocated(self):
        if self._walker:
            return
        walker = ffi.new('git_revwalk **')
        if lib.git_revwalk_new(walker, self._repo().pointer):
            raise error.GitException
        self._walker = walker[0]
