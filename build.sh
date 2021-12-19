#!/bin/bash

mkdir -p build
pushd build
c++ ../main.cpp -o handmadesokoban -g `sdl2-config --cflags --libs`
popd