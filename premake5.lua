workspace "MMX"
    architecture "x64"
    configurations { 
        "Debug", 
        "Release" 
    }

	ObjDir = "%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}"
	TargetDir = "%{wks.location}/bin/%{prj.name}/%{cfg.buildcfg}"

    IncDir = {}
    IncDir["glfw"] = "%{wks.location}/3rdparty/glfw/include"
    IncDir["glad"] = "%{wks.location}/3rdparty/glad/include"
    IncDir["imgui"] = "%{wks.location}/3rdparty/imgui"
    IncDir["ImGuizmo"] = "%{wks.location}/3rdparty/ImGuizmo"
    IncDir["bullet3"] = "%{wks.location}/3rdparty/bullet3/src"
    IncDir["spdlog"] = "%{wks.location}/3rdparty/spdlog/include"
    IncDir["glm"] = "%{wks.location}/3rdparty/glm"
    IncDir["stb"] = "%{wks.location}/3rdparty/stb"
    IncDir["nativefiledialog"] = "%{wks.location}/3rdparty/nativefiledialog/src/include"
    IncDir["EventBus"] = "%{wks.location}/3rdparty/EventBus/lib/src"
    IncDir["icu4c"] = "%{wks.location}/3rdparty/icu4c/include"
    IncDir["yaml"] = "%{wks.location}/3rdparty/yaml-cpp/include"
    IncDir["googletest"] = "%{wks.location}/3rdparty/googletest/googletest/include"
	IncDir["tinygltf"] = "%{wks.location}/3rdparty/tinygltf"

    LibDir = {}
    LibDir["icu4c"] = "%{wks.location}/3rdparty/icu4c/lib64"

    group "3rdparty"
    include "3rdparty/glad"
    include "3rdparty/imgui"
    include "3rdparty/ImGuizmo"
    include "3rdparty/nativefiledialog"

    externalproject "glfw"
        location "3rdparty/glfw/build/src"
        kind "StaticLib"
        language "C"
        configmap {
            ["Debug"] = "Debug",
            ["Release"] = "MinSizeRel"
        }

	externalproject "EventBus"
		location "3rdparty/EventBus/build/lib"
		kind "StaticLib"
		language "C++"
		configmap {
			["Debug"] = "Debug",
			["Release"] = "MinSizeRel"
		}

	externalproject "spdlog"
		location "3rdparty/spdlog/build"
		kind "StaticLib"
		language "C++"
		configmap {
			["Debug"] = "Debug",
			["Release"] = "MinSizeRel"
		}

	externalproject "yaml-cpp"
		location "3rdparty/yaml-cpp/build"
		kind "StaticLib"
		language "C++"
		configmap {
			["Debug"] = "Debug",
			["Release"] = "MinSizeRel"
		}

    externalproject "gtest"
		location "3rdparty/googletest/build/googletest"
		kind "StaticLib"
		language "C++"
		configmap {
			["Debug"] = "Debug",
			["Release"] = "MinSizeRel"
		}
        filter "configurations:Debug"
            symbols "on"
            runtime "Debug"

        filter "configurations:Release"
            optimize "on"
            runtime "Release"
		
	group "3rdparty/Bullet"
	externalproject "BulletCollision"
		location "3rdparty/bullet3/build/src/BulletCollision"
		kind "StaticLib"
		language "C++"
		configmap {
			["Debug"] = "Debug",
			["Release"] = "MinSizeRel"
		}
		
	externalproject "BulletDynamics"
		location "3rdparty/bullet3/build/src/BulletDynamics"
		kind "StaticLib"
		language "C++"
		configmap {
			["Debug"] = "Debug",
			["Release"] = "MinSizeRel"
		}
		
	externalproject "LinearMath"
		location "3rdparty/bullet3/build/src/LinearMath"
		kind "StaticLib"
		language "C++"
		configmap {
			["Debug"] = "Debug",
			["Release"] = "MinSizeRel"
		}
    group ""

	group "Tests"
    include "tests"
	group ""

    include "src/Core"
    include "src/Editor"
    include "src/Common"

