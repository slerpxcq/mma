cmake_minimum_required(VERSION 3.16)

set(PREMAKE_BUILD_TYPES "Debug" "Release")
get_property(multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(multi_config)
  set(CMAKE_CONFIGURATION_TYPES "${PREMAKE_BUILD_TYPES}" CACHE STRING "list of supported configuration types" FORCE)
else()
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type of the project.")
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${PREMAKE_BUILD_TYPES}")
  if(NOT CMAKE_BUILD_TYPE IN_LIST PREMAKE_BUILD_TYPES)
    message(FATAL_ERROR
      "Invalid build type '${CMAKE_BUILD_TYPE}'.
      CMAKE_BUILD_TYPE must be any one of the possible values:
      ${PREMAKE_BUILD_TYPES}"
    )
  endif()
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "")
set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELEASE "")

project("MMX")
include(3rdparty/ImGuizmo/ImGuizmo.cmake)
include(3rdparty/bullet3/bullet3.cmake)
include(3rdparty/imgui/imgui.cmake)
include(3rdparty/nativefiledialog/nativefiledialog.cmake)
include(3rdparty/ogre-next/ogre-next.cmake)
include(3rdparty/spdlog/spdlog.cmake)
include(3rdparty/yaml-cpp/yaml-cpp.cmake)
include(MMX-OGRE.cmake)