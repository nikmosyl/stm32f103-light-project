#!/bin/bash
set -e

rm -rf build
mkdir build
cd build

echo "Dir created"

cmake ..
echo "Configured"

make -j$(sysctl -n hw.ncpu)
echo "Compiled"

st-flash write stm32f103-light-project.bin 0x08000000
echo "Flash Done"
