#!/bin/bash

# initialize hts_engine_API
git clone https://github.com/yamachu/hts_engine_API.git
pushd hts_engine_API
./configure
make -j4
popd

# build OpenJTalk
./configure --with-hts-engine-header-path=`pwd`/hts_engine_API/include --with-hts-engine-library-path=`pwd`/hts_engine_API/lib --with-charset=UTF-8
make -j4 || : # osx build failed?
cd library
if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    make mac_shared
else
    make linux_shared
fi
