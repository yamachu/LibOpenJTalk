#!/bin/bash

# get submodules
git submodule update --init

# initialize hts_engine_API
pushd hts_engine_API
chmod +x setup.sh
emconfigure ./configure --host=wasm32-unknown-none
emmake make -j4
popd

# build OpenJTalk
emconfigure ./configure --host=wasm32-unknown-none --with-hts-engine-header-path=`pwd`/hts_engine_API/include --with-hts-engine-library-path=`pwd`/hts_engine_API/lib --with-charset=UTF-8
emmake make -j

emmake make -C library/lang wasm_static CXX=em++ AR=emar LINK=em++
emmake make -C library/full wasm_static CXX=em++ AR=emar LINK=em++