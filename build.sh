#!/bin/bash

set -e

cd $(hg root)/libgit2
test -d build && rm -r build
mkdir build && cd build
cmake .. -DBUILD_CLAR=OFF -DTHREADSAFE=ON
cmake --build .

cd $(hg root)
if [ $(uname) = "Darwin" ]
then
    export PKG_CONFIG_PATH=/usr/local/opt/libffi/lib/pkgconfig
fi
test -d venv && rm -r venv
virtualenv venv
venv/bin/pip install cffi
