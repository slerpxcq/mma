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
    include "3rdparty/glad"
    include "3rdparty/imgui"
    include "3rdparty/ImGuizmo"
    include "3rdparty/spdlog"
    include "3rdparty/nativefiledialog"
    include "3rdparty/EventBus"
    include "3rdparty/yaml-cpp"

    externalproject "glfw"
        location "3rdparty/glfw/build/src"
        kind "StaticLib"
        language "C"
        configmap {
            ["Debug"] = "Debug",
            ["Release"] = "MinSizeRel"
        }
		
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

	externalproject "EventBus"
		location "3rdparty/EventBus/build/lib"
		kind "StaticLib"
		language "C++"
		configmap {
			["Debug"] = "Debug",
			["Release"] = "MinSizeRel"
		}
	
	group "Test"
	group ""

    include "src/Core"
    include "src/Editor"
    include "src/Common"
