#!/usr/bin/env bash

# Assuming we are invoked from the project's root directory

# Update submodules
git submodule update --init --recursive

# Initialize vcpkg
./libs/vcpkg/bootstrap-vcpkg.sh

# Initialize shaderc dependency
cd libs/shaderc
./utils/git-sync-deps

# Return to previous working directory
cd ../../

