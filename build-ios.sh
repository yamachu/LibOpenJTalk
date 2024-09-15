#!/bin/bash

# get submodules
git submodule update --init

# initialize hts_engine_API
pushd hts_engine_API
chmod +x setup.sh
./setup.sh

for TARGET in x86_64-apple-darwin arm-apple-darwin; do
    export AR=`xcrun -f ar`
    export CC=`xcrun -f clang`
    export AS=$CC
    export CXX=`xcrun -f clang++`
    export LD=`xcrun -f ld`
    export RANLIB=`xcrun -f ranlib`
    export STRIP=`xcrun -f strip`

    ./configure --host $TARGET
    make clean && make -j4
    popd

    # build OpenJTalk
    ./configure --with-hts-engine-header-path=`pwd`/hts_engine_API/include --with-hts-engine-library-path=`pwd`/hts_engine_API/lib --with-charset=UTF-8 --host $TARGET
    make -j || : # ここはCrossBuildの弊害で、実行できないバイナリを吐き出してしまって落ちるので、失敗を無視している

    # build library
    make -C library/full linux_shared CXX=$CXX LINK=$CXX AR=$AR
    make -C library/lang linux_shared CXX=$CXX LINK=$CXX AR=$AR

mv library/full/libopenjtalk.so library/full/${TARGET}-libopenjtalk.so
mv library/lang/libopenjtalk-lang.so library/lang/${TARGET}-libopenjtalk-lang.so
