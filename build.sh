#!/bin/bash
set -e

rm -rf build

cd build
echo "Dir created"

make -j$(nproc)
echo "Compiled"

st-flash write panaled.bin 0x08000000
echo "Flash Done"
