git submodule update --init --recursive

.\libs\vcpkg\bootstrap-vcpkg.bat

cd libs\shaderc
python .\utils\git-sync-deps
cd ..\..\

.\libs\vcpkg\vcpkg install sdl2[vulkan]:x64-windows
.\libs\vcpkg\vcpkg install boost-circular-buffer:x64-windows
.\libs\vcpkg\vcpkg install boost-uuid:x64-windows
.\libs\vcpkg\vcpkg install xerces-c:x64-windows
.\libs\vcpkg\vcpkg install curlpp:x64-windows
.\libs\vcpkg\vcpkg install fmt:x64-windows
.\libs\vcpkg\vcpkg install taskflow:x64-windows