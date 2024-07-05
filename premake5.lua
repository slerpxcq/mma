workspace "MMX"
    architecture "x64"
    configurations { 
        "Debug", 
        "Release" 
    }

    IncDir = {}
    IncDir["glfw"] = "3rdparty/glfw/include"
    IncDir["glad"] = "3rdparty/glad/include"
    IncDir["imgui"] = "3rdparty/imgui"
    IncDir["ImGuizmo"] = "3rdparty/ImGuizmo"
    IncDir["bullet3"] = "3rdparty/bullet3/src"
    IncDir["spdlog"] = "3rdparty/spdlog/include"
    IncDir["glm"] = "3rdparty/glm"
    IncDir["stb"] = "3rdparty/stb"
    IncDir["nativefiledialog"] = "3rdparty/nativefiledialog/src/include"
    IncDir["EventBus"] = "3rdparty/EventBus/lib/src"
    IncDir["icu4c"] = "3rdparty/icu4c/include"
    IncDir["yaml"] = "3rdparty/yaml-cpp/include"

    LibDir = {}
    LibDir["icu4c"] = "3rdparty/icu4c/lib64"

    group "3rdparty"
    include "3rdparty/glfw"
    include "3rdparty/glad"
    include "3rdparty/imgui"
    include "3rdparty/ImGuizmo"
    include "3rdparty/bullet3"
    include "3rdparty/spdlog"
    include "3rdparty/nativefiledialog"
    include "3rdparty/EventBus"
    include "3rdparty/yaml-cpp"
    group ""

    include "src/core"
    include "src/renderer"
    include "src/editor"
    include "src/common"
