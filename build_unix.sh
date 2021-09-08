#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) [mac,linux]" >&2
    exit 1
fi

# get submodules
git submodule update --init

# initialize hts_engine_API
pushd hts_engine_API
chmod +x setup.sh
./setup.sh
./configure
make -j4
popd

# build OpenJTalk
./configure --with-hts-engine-header-path=`pwd`/hts_engine_API/include --with-hts-engine-library-path=`pwd`/hts_engine_API/lib --with-charset=UTF-8
make -j

# build library
if [ $1 = "mac" ]; then
    make -C library/full mac_shared
    make -C library/lang mac_shared
elif [ $1 = "linux" ]; then
    make -C library/full linux_shared
    make -C library/lang linux_shared
fi
