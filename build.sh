#!/bin/bash

set -e

cd $(hg root)/libgit2
test -d build && rm -r build
mkdir build && cd build
cmake ..
cmake --build .

cd $(hg root)
export PKG_CONFIG_PATH=/usr/local/Cellar/libffi/3.0.13/lib/pkgconfig
test -d venv && rm -r venv
virtualenv venv
venv/bin/pip install cffi
