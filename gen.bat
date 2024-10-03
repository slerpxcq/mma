@echo off
:: CMake
:: -------- Bullet --------
pushd .
cd "3rdparty/bullet3"
cmake -S . -B build
popd
:: -------- glfw --------
pushd .
cd "3rdparty/glfw"
cmake -S . -B build -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON
popd
:: -------- googletest --------
pushd .
cd "3rdparty/googletest"
cmake -S . -B build -Dgtest_force_shared_crt=ON
popd
:: -------- EventBus --------
pushd .
cd "3rdparty/EventBus"
cmake -S . -B build -DENABLE_TEST=OFF
popd
:: -------- spdlog --------
pushd .
cd "3rdparty/spdlog"
cmake -S . -B build 
popd
:: :: -------- yaml-cpp --------
pushd .
cd "3rdparty/yaml-cpp"
cmake -S . -B build -DYAML_USE_SYSTEM_GTEST=OFF
popd

:: premake
"3rdparty/premake5/premake5" vs2022
