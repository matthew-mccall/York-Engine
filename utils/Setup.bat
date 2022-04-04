git submodule update --init --recursive

.\libs\vcpkg\bootstrap-vcpkg.bat

cd libs\shaderc
python .\utils\git-sync-deps
cd ..\..\
