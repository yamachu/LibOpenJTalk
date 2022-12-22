#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $(basename $0) [some-android-arch]" >&2
    exit 1
fi

# get submodules
git submodule update --init

# initialize hts_engine_API
pushd hts_engine_API
chmod +x setup.sh
./setup.sh

export TOOLCHAIN=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64
export TARGET=$1
export API=21

export AR=$TOOLCHAIN/bin/llvm-ar
export CC=$TOOLCHAIN/bin/$TARGET$API-clang
export AS=$CC
export CXX=$TOOLCHAIN/bin/$TARGET$API-clang++
export LD=$TOOLCHAIN/bin/ld
export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
export STRIP=$TOOLCHAIN/bin/llvm-strip

./configure --host $TARGET
make -j4
popd

# build OpenJTalk
./configure --with-hts-engine-header-path=`pwd`/hts_engine_API/include --with-hts-engine-library-path=`pwd`/hts_engine_API/lib --with-charset=UTF-8 --host $TARGET
make -j || : # ここはCrossBuildの弊害で、実行できないバイナリを吐き出してしまって落ちるので、失敗を無視している

# build library
make -C library/full linux_shared CXX=$CXX LINK=$CXX AR=$AR
make -C library/lang linux_shared CXX=$CXX LINK=$CXX AR=$AR

mv library/full/libopenjtalk.so library/full/${TARGET}-libopenjtalk.so
mv library/lang/libopenjtalk-lang.so library/lang/${TARGET}-libopenjtalk-lang.so
