import os

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
    headers.strarray,
    headers.threads,
    headers.errors,
    headers.types,
    headers.oid,
    headers.signature,
    headers.index,
    headers.odb,
    headers.repository,
    headers.revwalk,
    headers.diff,
    headers.merge,
    headers.graph,
    headers.refs,
    headers.reflog,
    headers.revparse,
    headers.object,
    headers.blob,
    headers.commit,
    headers.tag,
    headers.tree,
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

__args = {}
__includes = os.getenv('PYGGY_INCLUDES')
__libraries = os.getenv('PYGGY_LIBRARIES')
__module = os.getenv('PYGGY_MODULE_NAME')
if __includes:
    __args['include_dirs'] = __includes.split(':')
if __libraries:
    __args['library_dirs'] = __libraries.split(':')
if __module:
    __args['modulename'] = __module

lib = ffi.verify("#include <git2.h>",
                 libraries=['git2'],
                 ext_package='pyggy',
                 **__args)
