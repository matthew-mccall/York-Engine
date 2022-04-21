#!/usr/bin/env bash

pattern="1*"
dirs=( $pattern )

if [ ! -d "${dirs[0]}" ]; then
  curl -fsSLO https://sdk.lunarg.com/sdk/download/latest/linux/vulkan-sdk.tar.gz
  tar xvf vulkan-sdk*
fi