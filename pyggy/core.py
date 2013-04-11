from cffi import FFI

import headers

__all__ = [
    'ffi',
    'lib',
]

ffi = FFI()
defs = '\n'.join([
    headers.version,
    headers.common,
    headers.threads,
    headers.errors,
    headers.types,
    headers.oid,
    headers.signature,
    headers.index,
    headers.odb,
    headers.repository,
    headers.revwalk,
    headers.merge,
    headers.graph,
    headers.strarray,
    headers.refs,
    headers.reflog,
    headers.revparse,
    headers.object,
    headers.blob,
    headers.commit,
    headers.tag,
    headers.tree,
    headers.diff,
    headers.config,
    headers.net,
    headers.transport,
    headers.checkout,
    headers.remote,
    headers.clone,
    headers.push,
    headers.attr,
    headers.ignore,
    headers.branch,
    headers.refspec,
    headers.status,
    headers.submodule,
    headers.notes,
    headers.reset,
    headers.message,
    headers.pack,
    headers.stash,
])
ffi.cdef(defs)
lib = ffi.verify("#include <git2.h>",
                 libraries=['git2'],
                 library_dirs=['/usr/local/opt/libgit2/lib'],
                 include_dirs=['/usr/local/opt/libgit2/include'])
