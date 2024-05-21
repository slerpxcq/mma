add_executable("MMX-OGRE"
  "src/main.cpp"
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_dependencies("MMX-OGRE"
    "imgui"
    "ImGuizmo"
    "bullet3"
    "spdlog"
    "nativefiledialog"
    "ogre-next"
    "yaml-cpp"
  )
  set_target_properties("MMX-OGRE" PROPERTIES
    OUTPUT_NAME "MMX-OGRE"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Users/Admin/Desktop/mmx-ogre/bin/MMX-OGRE/Debug"
    LIBRARY_OUTPUT_DIRECTORY "C:/Users/Admin/Desktop/mmx-ogre/bin/MMX-OGRE/Debug"
    RUNTIME_OUTPUT_DIRECTORY "C:/Users/Admin/Desktop/mmx-ogre/bin/MMX-OGRE/Debug"
  )
endif()
target_include_directories("MMX-OGRE" PRIVATE
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/imgui>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/ImGuizmo>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/bullet3/src>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/spdlog/include>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/glm>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/stb>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/nativefiledialog/src/include>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/icu4c/include>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/yaml-cpp/include>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/ogre-next/OgreMain/include>
  $<$<CONFIG:Debug>:C:/Users/Admin/Desktop/mmx-ogre/src>
)
target_compile_definitions("MMX-OGRE" PRIVATE
  $<$<CONFIG:Debug>:MM_DEBUG>
)
target_link_directories("MMX-OGRE" PRIVATE
)
target_link_libraries("MMX-OGRE"
  $<$<CONFIG:Debug>:imgui>
  $<$<CONFIG:Debug>:ImGuizmo>
  $<$<CONFIG:Debug>:bullet3>
  $<$<CONFIG:Debug>:spdlog>
  $<$<CONFIG:Debug>:nativefiledialog>
  $<$<CONFIG:Debug>:ogre-next>
  $<$<CONFIG:Debug>:yaml-cpp>
  $<$<CONFIG:Debug>:3rdparty/icu4c/lib64/*.lib>
)
target_compile_options("MMX-OGRE" PRIVATE
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-Wno-26812>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
  $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-Wno-26812>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("MMX-OGRE" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Debug)
target_precompile_headers("MMX-OGRE" PUBLIC mmpch.hpp)
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
  add_dependencies("MMX-OGRE"
    "imgui"
    "ImGuizmo"
    "bullet3"
    "spdlog"
    "nativefiledialog"
    "ogre-next"
    "yaml-cpp"
  )
  set_target_properties("MMX-OGRE" PROPERTIES
    OUTPUT_NAME "MMX-OGRE"
    ARCHIVE_OUTPUT_DIRECTORY "C:/Users/Admin/Desktop/mmx-ogre/bin/MMX-OGRE/Release"
    LIBRARY_OUTPUT_DIRECTORY "C:/Users/Admin/Desktop/mmx-ogre/bin/MMX-OGRE/Release"
    RUNTIME_OUTPUT_DIRECTORY "C:/Users/Admin/Desktop/mmx-ogre/bin/MMX-OGRE/Release"
  )
endif()
target_include_directories("MMX-OGRE" PRIVATE
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/imgui>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/ImGuizmo>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/bullet3/src>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/spdlog/include>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/glm>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/stb>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/nativefiledialog/src/include>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/icu4c/include>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/yaml-cpp/include>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/3rdparty/ogre-next/OgreMain/include>
  $<$<CONFIG:Release>:C:/Users/Admin/Desktop/mmx-ogre/src>
)
target_compile_definitions("MMX-OGRE" PRIVATE
  $<$<CONFIG:Release>:MM_RELEASE>
)
target_link_directories("MMX-OGRE" PRIVATE
)
target_link_libraries("MMX-OGRE"
  $<$<CONFIG:Release>:imgui>
  $<$<CONFIG:Release>:ImGuizmo>
  $<$<CONFIG:Release>:bullet3>
  $<$<CONFIG:Release>:spdlog>
  $<$<CONFIG:Release>:nativefiledialog>
  $<$<CONFIG:Release>:ogre-next>
  $<$<CONFIG:Release>:yaml-cpp>
  $<$<CONFIG:Release>:3rdparty/icu4c/lib64/*.lib>
)
target_compile_options("MMX-OGRE" PRIVATE
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-Wno-26812>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
  $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-Wno-26812>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("MMX-OGRE" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
target_precompile_headers("MMX-OGRE" PUBLIC mmpch.hpp)
endif()