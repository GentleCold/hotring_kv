#!/bin/bash

if [ ! -d build ]; then
  mkdir build
  mkdir build/release
  mkdir build/debug
fi

echo "=====CMAKE INFO====="

if [ "$1" = "r" ] || [ "$1" = "release" ]; then
  cd "build/release"
  cmake -DCMAKE_BUILD_TYPE=Release ../..
else
  cd "build/debug"
  cmake -DCMAKE_BUILD_TYPE=Debug ../..
fi

mv compile_commands.json ../../

echo ""
echo "=====MAKE INFO====="

make -j4

echo ""
echo "======RUN INFO======"

cd ../../bin

if [ "$1" = "r" ] || [ "$1" = "release" ]; then
  ./test_all_release
else
  ./test_all_debug
fi
