#!/bin/bash

if [[ $1 == "test" ]]; then
    echo "building for testing"
    cmake \
        -S . \
        -B build-test \
        -DBUILD_TESTS=ON \
        -DBUILD_APP=OFF \
        && cmake \
            --build build-test \
            --config Release \
            -j4
else
    echo "building normally"
    mkdir instdir
    cmake \
        -S . \
        -B build \
        -DCMAKE_INSTALL_PREFIX=/usr \
        && cmake \
            --build build \
            --config Release \
            -j4
fi

if [[ -f build/compile_commands.json ]]; then
    rm compile_commands.json
    mv build/compile_commands.json ./
fi
