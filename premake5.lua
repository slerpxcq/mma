require "cmake"
workspace "MMX"
    architecture "x64"
    configurations { 
        "Debug", 
        "Release" 
    }

    IncDir = {}
    IncDir["imgui"] = "3rdparty/imgui"
    IncDir["ogre"] = "3rdparty/ogre/OgreMain/include"
    IncDir["ImGuizmo"] = "3rdparty/ImGuizmo"
    IncDir["bullet3"] = "3rdparty/bullet3/src"
    IncDir["spdlog"] = "3rdparty/spdlog/include"
    IncDir["glm"] = "3rdparty/glm"
    IncDir["stb"] = "3rdparty/stb"
    IncDir["nativefiledialog"] = "3rdparty/nativefiledialog/src/include"
    IncDir["icu4c"] = "3rdparty/icu4c/include"
    IncDir["yaml"] = "3rdparty/yaml-cpp/include"

    LibDir = {}
    LibDir["icu4c"] = "3rdparty/icu4c/lib64"

    group "3rdparty"
    include "3rdparty/imgui"
    include "3rdparty/ImGuizmo"
    include "3rdparty/bullet3"
    include "3rdparty/spdlog"
    include "3rdparty/nativefiledialog"
    include "3rdparty/yaml-cpp"
    -- include "3rdparty/ogre-next"
    group ""

    project "MMX-OGRE"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        targetdir "bin/%{prj.name}/%{cfg.buildcfg}"
        objdir "obj/%{prj.name}/%{cfg.buildcfg}"
        -- vectorextensions "AVX2"

        --pchheader "mmpch.hpp"
        --pchsource "src/mmpch.cpp"

        -- Copy icu4c dll's

        links {
            "imgui",
            "ImGuizmo",
            "bullet3",
            "spdlog",
            "nativefiledialog",
            "%{LibDir.icu4c}/*.lib",
            -- "ogre-next",
            "yaml-cpp",
        }

        includedirs {
            "%{IncDir.imgui}",
            "%{IncDir.ImGuizmo}",
            "%{IncDir.bullet3}",
            "%{IncDir.spdlog}",
            "%{IncDir.glm}",
            "%{IncDir.stb}",
            "%{IncDir.nativefiledialog}",
            "%{IncDir.icu4c}",
            "%{IncDir.yaml}",
            "%{IncDir.ogre}",
            "3rdparty/ogre/build/include",
            "3rdparty/ogre/Components/*/include",
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
            postbuildcommands {
                "xcopy /y /f \"3rdparty/icu4c/bin64/icudt75.dll\" \"%{cfg.targetdir}\"",
                "xcopy /y /f \"3rdparty/icu4c/bin64/icuin75.dll\" \"%{cfg.targetdir}\"",
                "xcopy /y /f \"3rdparty/icu4c/bin64/icutu75.dll\" \"%{cfg.targetdir}\"",
                "xcopy /y /f \"3rdparty/icu4c/bin64/icuuc75.dll\" \"%{cfg.targetdir}\""
            }

        filter "configurations:Debug"
            defines { "MM_DEBUG" }
            postbuildcommands {
                "xcopy /y /f \"3rdparty/ogre/build/bin/debug/*.dll\" \"%{cfg.targetdir}\"",
            }
            links { "3rdparty/ogre/build/lib/Debug/*.lib" }
            symbols "on"

        filter "configurations:Release"
            defines { "MM_RELEASE" }
            postbuildcommands {
                "xcopy /y /f \"3rdparty/ogre/build/bin/minsizerel/*.dll\" \"%{cfg.targetdir}\"",
            }
            links { "3rdparty/ogre/build/lib/MinSizeRel/*.lib" }
            optimize "on"