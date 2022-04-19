#!/usr/bin/env bash

# Assuming we are invoked from the project's root directory

for arg in "$@"
do
    if [[ $arg == "--install-sdl" ]]; then
      INSTALL_SDL=1
    fi
done

# Update submodules
git submodule update --init --recursive

# Initialize vcpkg
./libs/vcpkg/bootstrap-vcpkg.sh

# Initialize shaderc dependency
cd libs/shaderc
./utils/git-sync-deps

# Return to previous working directory
cd ../../

if [[ -n $INSTALL_SDL ]]; then
    "Installing SDL2"
    rm -rf ./libs/SDL/build
    
    if which ninja; then
      cmake -G Ninja -B ./libs/SDL/build -S ./libs/SDL
    else
      cmake -B ./libs/SDL/build -S ./libs/SDL
    fi
    
    cmake --build ./libs/SDL/build
    echo "You may need administrative priviliges to install SDL2"
    sudo cmake --install ./libs/SDL/build
fi

if [[ "$OSTYPE" == "linux-gnu"* ]]; then

  if ./libs/vcpkg/vcpkg list | grep -q "sdl2:x64-linux[ \t]"; then
    ./libs/vcpkg/vcpkg remove "sdl2:x64-linux"
  fi

  ./libs/vcpkg/vcpkg install "sdl2[vulkan]:x64-linux-dynamic" --recurse --overlay-triplets=custom-triplets

elif [[ "$OSTYPE" == "darwin"* ]]; then

  if ./libs/vcpkg/vcpkg list | grep -q "sdl2:x64-osx[ \t]"; then
    ./libs/vcpkg/vcpkg remove "sdl2:x64-osx"
  fi
    
./libs/vcpkg/vcpkg install "sdl2[vulkan]:x64-osx-dynamic" --recurse --overlay-triplets=custom-triplets
    
fi

./libs/vcpkg/vcpkg install boost-circular-buffer
./libs/vcpkg/vcpkg install xerces-c
./libs/vcpkg/vcpkg install curlpp
./libs/vcpkg/vcpkg install fmt