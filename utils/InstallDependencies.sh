#!/usr/bin/env bash

if [[ "$OSTYPE" == "linux-gnu"* ]]; then

  if ./libs/vcpkg/vcpkg list | grep -q "sdl2:x64-linux[ \t]"; then
    echo "Reinstalling SDL2"
    ./libs/vcpkg/vcpkg remove "sdl2:x64-osx"
  fi

  ./libs/vcpkg/vcpkg install "sdl2[vulkan]:x64-linux-dynamic" --recurse --overlay-triplets=custom-triplets

elif [[ "$OSTYPE" == "darwin"* ]]; then

  if ./libs/vcpkg/vcpkg list | grep -q "sdl2:x64-osx[ \t]"; then
    echo "Reinstalling SDL2"
    ./libs/vcpkg/vcpkg remove "sdl2:x64-osx"

fi
    
./libs/vcpkg/vcpkg install "sdl2[vulkan]:x64-osx-dynamic" --recurse --overlay-triplets=custom-triplets
    
fi

./libs/vcpkg/vcpkg install xerces-c
./libs/vcpkg/vcpkg install curlpp
./libs/vcpkg/vcpkg install fmt