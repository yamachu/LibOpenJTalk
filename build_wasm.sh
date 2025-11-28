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
emconfigure ./configure --host=wasm32-unknown-none --with-hts-engine-header-path=`pwd`/hts_engine_API/include --with-hts-engine-library-path=`pwd`/hts_engine_API/lib --with-charset=UTF-8 CXXFLAGS="-g -O2 -Wno-register"
emmake make -j

emmake make -C library/lang wasm_static CXX=em++ AR=emar LINK=em++ CXXFLAGS="-O2 -Wall -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wno-register"
emmake make -C library/full wasm_static CXX=em++ AR=emar LINK=em++ CXXFLAGS="-O2 -Wall -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wno-register"