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

    project "MMX"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        targetdir "bin/%{prj.name}/%{cfg.buildcfg}"
        objdir "obj/%{prj.name}/%{cfg.buildcfg}"
        vectorextensions "AVX2"

        pchheader "mmpch.hpp"
        pchsource "src/mmpch.cpp"

        -- Copy icu4c dll's
        postbuildcommands {
            "xcopy /y /f \"3rdparty/icu4c/bin64/icudt75.dll\" \"%{cfg.targetdir}\"",
            "xcopy /y /f \"3rdparty/icu4c/bin64/icuin75.dll\" \"%{cfg.targetdir}\"",
            "xcopy /y /f \"3rdparty/icu4c/bin64/icutu75.dll\" \"%{cfg.targetdir}\"",
            "xcopy /y /f \"3rdparty/icu4c/bin64/icuuc75.dll\" \"%{cfg.targetdir}\""
        }

        links {
            "glad",
            "glfw",
            "imgui",
            "ImGuizmo",
            "bullet3",
            "spdlog",
            "nativefiledialog",
            "EventBus",
            "%{LibDir.icu4c}/*.lib",
            "yaml-cpp",
            "opengl32.lib"
        }

        includedirs {
            "%{IncDir.glfw}",
            "%{IncDir.glad}",
            "%{IncDir.imgui}",
            "%{IncDir.ImGuizmo}",
            "%{IncDir.bullet3}",
            "%{IncDir.spdlog}",
            "%{IncDir.glm}",
            "%{IncDir.stb}",
            "%{IncDir.nativefiledialog}",
            "%{IncDir.EventBus}",
            "%{IncDir.icu4c}",
            "%{IncDir.yaml}",
            "src"
        }

        defines {

        }

        files { 
            "src/**.cpp",
            "src/**.hpp"
        }

        filter "system:windows"
            staticruntime "on"
            systemversion "latest"
            disablewarnings {
                "26812"
            }

        filter "configurations:Debug"
            defines { 
                "MM_DEBUG" 
            }
            symbols "on"

        filter "configurations:Release"
            defines {
                "MM_RELEASE"
            }
            optimize "on"