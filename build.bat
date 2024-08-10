rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\dev\vcpkg\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build .

.\Debug\McDoner.exe