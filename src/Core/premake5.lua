project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin/%{prj.name}/%{cfg.buildcfg}"
    objdir "%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}"

    pchheader "CorePch.hpp"
    pchsource "CorePch.cpp"

    links {
        "glad",
        "glfw",
        "imgui",
        "ImGuizmo",
        "spdlog",
        "nativefiledialog",
        "EventBus",
		"BulletCollision",
		"BulletDynamics",
		"LinearMath",
        "../%{LibDir.icu4c}/*.lib",
        "yaml-cpp",
        "opengl32.lib",
        "Common"
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
		"%{IncDir.tinygltf}",
        "../",
		"."
    }


    files { 
        "**.cpp",
        "**.hpp"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
	    defines { "MM_CORE_DEBUG "}
		runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        optimize "on"
		runtime "Release"
