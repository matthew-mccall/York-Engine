#!/usr/bin/env bash

./libs/vcpkg/vcpkg install "sdl2[vulkan]" --recurse
./libs/vcpkg/vcpkg install shaderc
./libs/vcpkg/vcpkg install xerces-c
./libs/vcpkg/vcpkg install curlpp
./libs/vcpkg/vcpkg install fmt