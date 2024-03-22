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

    group "3rdparty"
    include "3rdparty/glfw"
    include "3rdparty/glad"
    include "3rdparty/imgui"
    include "3rdparty/ImGuizmo"
    include "3rdparty/bullet3"
    include "3rdparty/spdlog"
    group ""

    project "MMX"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        targetdir "bin/%{prj.name}/%{cfg.buildcfg}"
        objdir "obj/%{prj.name}/%{cfg.buildcfg}"

        pchheader "mmpch.hpp"
        pchsource "src/mmpch.cpp"

        links {
            "glad",
            "glfw",
            "imgui",
            "ImGuizmo",
            "bullet3",
            "spdlog",
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
            "src"
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