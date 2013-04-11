===================================
 pyggy - libgit2 wrapper for Python
===================================

pyggy is a libgit2 wrapper for Python.  Unlike the pygit2 project, pyggy uses
cffi to wrap libgit2.

pyggy comes in two pieces: a low-level wrapper around libgit2 that is little
more than a jacket around the underlying C datatypes (pyggy.core), and a
high-level, Pythonic API for working with libgit2 at a higher level of
abstraction.

Prequisites
===========

First, you'll need to build and install libgit2.  libgit2, at an appropriate
version, will already be checked out for you in this working copy, but not yet
built.  Patches to make building that happen automatically when running
`setup.py` would be most gratefully welcome.

You will also need libffi installed and in your `PKG_CONFG_PATH`.  Assuming you
are on Ubuntu, you can simply run `apt-get install libffi-dev` and you're done.
If you're on OS X and using Homebrew, you'll instead want to do something like

::

    brew install libffi
    export PKG_CONFIG_PATH=/usr/local/opt/libffi/lib/pkgconfig

Installation
============

Once you've installed the prerequisites, `python setup.py install` should take
care of the rest.
