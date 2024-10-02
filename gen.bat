@echo off
:: CMake
pushd .
cd "3rdparty/bullet3"
cmake -S . -B build
cd "3rdparty/glfw"
cmake -S . -B build -D USE_MSVC_RUNTIME_LIBRARY_DLL=ON
popd
:: premake
"3rdparty/premake5/premake5" vs2022