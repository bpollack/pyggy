from collections import defaultdict, namedtuple, MutableMapping
import weakref

from .core import lib, ffi
from . import error, util


def _infinitedict():
    return defaultdict(_infinitedict)


def _valid_ref_name(name):
    if name[0] == '/' or name[-1] == '/' or name[-1] == '.':
        return False
    if name.count('/') != 2:
        return False
    if not all(40 < ord(c) < 176 for c in name):
        return False
    if any(reject in name
           for reject
           in ['/.', '//', '?', '*', '[', '..', '\\', '^', '*', '@{', '~', '.lock']):
        return False
    return True


def Oid(sha):
    """convenience wrapper to allow calling Oid(an_oid)"""
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
    """represents a parsed Git user"""
    def __init__(self, git_signature):
        self.name = ffi.string(git_signature.name)
        self.email = ffi.string(git_signature.email)
        self.when = git_signature.when

    def __str__(self):
        return '%s <%s>' % (self.name, self.email)


class Blob(object):
    """represents a blob from the ODB"""
    def __init__(self, repo, oid=None):
        self._repo = weakref.ref(repo)
        self.oid = Oid(oid)
        self._data = None

    def read(self):
        blob = ffi.new('git_blob **')
        if lib.git_blob_lookup(blob, self._repo().pointer, self.oid.pointer):
            raise error.GitException
        blob = blob[0]
        size = lib.git_blob_rawsize(blob)
        raw = lib.git_blob_rawcontent(blob)
        self._data = ffi.buffer(raw, size)[:]
        lib.git_blob_free(blob)

    @property
    def data(self):
        if self._data is None:
            self.read()
        return self._data


class Raw(object):
    """represents a completely unparsed raw object from the ODB

    This class is extremely likely to radically change in a near-future
    release."""
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
    """represents a Git commit

    This object loads lazily, so it is (relatively) safe to hold onto
    several thousand of them at once, as long as you haven't fully
    reified them.  On the flip side, this means that, like all Pyggy
    objects, you cannot keep Commit objects around after you have
    called Repo.close.
    """
    def __init__(self, repo, oid=None, load=True):
        self._repo = weakref.ref(repo)
        self.oid = Oid(oid)
        self._dirty = True
        if load:
            self.read()

    def __getitem__(self, name):
        self.read()
        return self.tree[name]

    def read(self):
        """loads the actual commit data into this object

        You almost never need to call this method; the commit
        will load its data if it needs to when you access its
        properties.
        """

        # Abort if we're not dirty or not a real commit
        if not self._dirty or self.oid is None:
            return

        commit = ffi.new('git_commit **')
        err = lib.git_commit_lookup_prefix(commit, self._repo().pointer,
                                           self.oid.pointer, len(self.oid))
        if err:
            if err == lib.GIT_ENOTFOUND:
                raise KeyError(self.oid.sha)
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
        """return a User object representing this commit's author"""
        self.read()
        return self._author

    @property
    def committer(self):
        """return a User object representing this commit's committer"""
        self.read()
        return self._committer

    @property
    def manifest(self):
        """return the full file manifest"""
        self.read()
        return self.tree.manifest

    @property
    def message(self):
        """return the lossy unicode representing this commit message"""
        self.read()
        return self._message

    @property
    def parents(self):
        """return the full parent objects for this commit

        If you do not need full parent commit objects, consider using
        parent_ids instead.
        """
        self.read()
        if self._parents is None:
            self._parents = [self._repo().commit(sha) for sha in self._parent_ids]
        return self._parents

    @property
    def parent_ids(self):
        """return the list of parent IDs

        These are not full-fledge parent objects; you'll need to reify them
        using the appropriate repository if you want the full data, or you'll
        need to use the parents property instead."""
        self.read()
        return self._parent_ids

    @property
    def sha(self):
        """return the string SHA for this commit

        This is a convenience function that simply wraps calling .oid.sha.
        """
        return self.oid.sha

    @property
    def tree(self):
        """return the tree that corresponds to this commit

        The tree (and its subtrees) will not actually be fully realized
        until you begin working ith its methods.  If you want it to
        be loaded immediately, call .read() on the Tree you receive
        from this method immediately afterwards.
        """
        self.read()
        return self._tree

    def __repr__(self):
        return '<Commit(%s)>' % self.oid


class ReferenceDb(MutableMapping):
    """manages a Git refstore

    Each ReferenceDb only manages a prefix of the Git reference
    namespace.  While it'd be possible to create a ReferenceDb for
    the empty string, and therefore to manage all refs through it,
    pyggy encourages you (and does internally) use separate
    ReferenceDbs for different name spaces to avoid inadvertantly
    creating tags when you wanted to create a branch and vice-versa.
    """
    def __init__(self, repo, prefix):
        self._repo = weakref.ref(repo)
        self._prefix = prefix
        if self._prefix[-1] != '/':
            self._prefix += '/'

    # Mapping
    def __iter__(self):
        for k, v in self.iteritems():
            yield k

    def __getitem__(self, name):
        assert name.startswith(self._prefix)
        ref = ffi.new('git_reference **')
        if lib.git_reference_lookup(ref, self._repo().pointer, name):
            raise error.GitException
        raw = ref[0]
        try:
            ref = ffi.new('git_reference **')
            if lib.git_reference_resolve(ref, raw):
                raise error.GitException
            resolved = ref[0]
            target = Oid(lib.git_reference_target(resolved)).sha
            lib.git_reference_free(resolved)
            return target
        finally:
            lib.git_reference_free(raw)

    def __len__(self):
        # Seriously, don't call this method; it's stupid, but required for collections.Mapping
        return len(list(iter(self)))

    # MutableMapping
    def __setitem__(self, name, sha):
        assert name.startswith(self._prefix)
        if not _valid_ref_name(name):
            raise ValueError("invalid ref name: {0}".format(name))
        ref = ffi.new('git_reference **')
        if lib.git_reference_create(ref, self._repo().pointer, name, Oid(sha).pointer, 1):
            raise error.GitException
        else:
            lib.git_reference_free(ref[0])

    def __delitem__(self, name):
        assert name.startswith(self._prefix)
        ref = ffi.new('git_reference **')
        err = lib.git_reference_lookup(ref, self._repo().pointer, name)
        if err:
            if err == lib.ENOTFOUND:
                raise KeyError(name)
            raise error.GitException
        ref = ref[0]
        try:
            if lib.git_reference_delete(ref):
                raise error.GitException
        finally:
            lib.git_reference_free(ref)

    # Auxiliary
    def iteritems(self):
        iterator = ffi.new('git_reference_iterator **')
        glob = self._prefix + '*'
        if lib.git_reference_iterator_glob_new(iterator, self._repo().pointer, glob):
            raise error.GitException
        iterator = iterator[0]
        try:
            while True:
                ref = ffi.new('git_reference **')
                if lib.git_reference_next(ref, iterator) == lib.GIT_ITEROVER:
                    raise StopIteration
                original = ref[0]
                name = ffi.string(lib.git_reference_name(original))

                ref = ffi.new('git_reference **')
                if lib.git_reference_resolve(ref, original):
                    raise error.GitException
                resolved = ref[0]
                target = Oid(lib.git_reference_target(resolved)).sha

                yield name, target
                lib.git_reference_free(resolved)
        finally:
            lib.git_reference_iterator_free(iterator)


class TreeEntry(namedtuple('TreeEntry', ('name', 'sha', 'mode'))):
    def __init__(self, *args, **kwargs):
        super(TreeEntry, self).__init__(*args, **kwargs)
        self.children = kwargs.get('children', defaultdict(TreeEntry))

    @property
    def is_directory(self):
        return self.mode & lib.GIT_FILEMODE_TREE


class Tree(object):
    def __init__(self, repo, oid):
        self._repo = weakref.ref(repo)
        self.oid = Oid(oid)
        self._dirty = True
        self._entries = None
        self._manifest = None

    def __getitem__(self, name):
        self.read()
        entry = self._manifest[name]
        if entry.is_directory:
            return entry
        else:
            return Blob(self._repo(), entry.sha)

    def read(self):
        """reads this tree, *plus* fully realizes the manifest cache"""
        if not self._dirty:
            return
        tree = ffi.new('git_tree **')
        if lib.git_tree_lookup(tree, self._repo().pointer, self.oid.pointer):
            raise error.GitException
        tree = tree[0]

        sha_map = {}
        trees = TreeEntry('', self.sha, lib.GIT_FILEMODE_TREE)
        cache = self._repo()._tree_cache

        @ffi.callback('int(const char *, const git_tree_entry *, void *)')
        def add_tree(root, entry, payload):
            root = ffi.string(root).strip('/')
            base = trees
            if root:
                for directory in root.split('/'):
                    base = base.children[directory]
            name = ffi.string(lib.git_tree_entry_name(entry))
            mode = lib.git_tree_entry_filemode(entry)
            sha = Oid(lib.git_tree_entry_id(entry)).sha
            if mode & lib.GIT_FILEMODE_TREE:
                if sha in cache:
                    base.children[name] = cache[sha]
                    return 1
                else:
                    sha_map[sha] = base.children[name] = TreeEntry(name, sha, mode)
                    return 0
            else:
                base.children[name] = TreeEntry(name, sha, mode)
                return 0

        lib.git_tree_walk(tree, lib.GIT_TREEWALK_PRE, add_tree, ffi.NULL)
        lib.git_tree_free(tree)

        cache.update(sha_map)
        self._entries = trees.children.items()
        self._manifest = {}
        self._flatten('', trees, self._manifest)
        self._dirty = False

    @property
    def entries(self):
        """return a single layer of TreeEntries

        The higher-level way to accomplish what you're probably trying to do
        is to use Commit.maanifest.
        """
        self.read()
        return self._entries

    @property
    def manifest(self):
        """return the fully realized tree under this point with full paths"""
        self.read()
        return self._manifest

    @property
    def sha(self):
        return self.oid.sha

    def _flatten(self, base, tree, manifest):
        for k, v in tree.children.viewitems():
            full = k if not base else base + '/' + k
            if v.is_directory:
                self._flatten(full, v, manifest)
            else:
                manifest[full] = v


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
        self._remaining = self._limit

    def __iter__(self):
        if not self._walking:
            self.open()
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
