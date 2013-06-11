#!/usr/bin/env python

from distutils.core import setup

# for CFFI, so that .verify() will be called properly
from pyggy import core

setup(name='pyggy',
      version='0.1',
      description='cffi-based Python wrapper for libgit2',
      author='Benjamin Pollack',
      author_email='benjamin@bitquabit.com',
      ext_package='pyggy',
      ext_modules=[core.ffi.verifier.get_extension()],
      requires=['cffi (==0.6)'],
      packages=['pyggy'])
