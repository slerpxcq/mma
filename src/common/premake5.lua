project "Common"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin/%{prj.name}/%{cfg.buildcfg}"
    objdir "%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}"

    includedirs { 
        "%{wks.location}/%{IncDir.glfw}",
        "%{wks.location}/%{IncDir.glad}",
        "%{wks.location}/%{IncDir.imgui}",
        "%{wks.location}/%{IncDir.ImGuizmo}",
        "%{wks.location}/%{IncDir.bullet3}",
        "%{wks.location}/%{IncDir.spdlog}",
        "%{wks.location}/%{IncDir.glm}",
        "%{wks.location}/%{IncDir.stb}",
        "%{wks.location}/%{IncDir.nativefiledialog}",
        "%{wks.location}/%{IncDir.EventBus}",
        "%{wks.location}/%{IncDir.icu4c}",
        "%{wks.location}/%{IncDir.yaml}",
    }

    files { 
        "**.cpp",
        "**.hpp"
    }

    links {
        "%{wks.location}/%{LibDir.icu4c}/*.lib"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
	    defines { "MM_DEBUG "}
		runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        optimize "on"
		runtime "Release"