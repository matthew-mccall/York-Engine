#!/usr/bin/env bash

wget -qO - http://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-focal.list http://packages.lunarg.com/vulkan/lunarg-vulkan-focal.list
sudo apt-get update  -y
sudo apt-get install -y vulkan-sdk ninja-build llvm clang libpthread-stubs0-dev xorg-dev