project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin/%{prj.name}/%{cfg.buildcfg}"
    objdir "%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}"

    pchheader "EditorPch.hpp"
    pchsource "EditorPch.cpp"

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
        "../",
		"."
    }

    postbuildcommands {
        "xcopy /y /f \"../../3rdparty/icu4c/bin64/icudt75.dll\" \"%{cfg.targetdir}\"",
        "xcopy /y /f \"../../3rdparty/icu4c/bin64/icuin75.dll\" \"%{cfg.targetdir}\"",
        "xcopy /y /f \"../../3rdparty/icu4c/bin64/icutu75.dll\" \"%{cfg.targetdir}\"",
        "xcopy /y /f \"../../3rdparty/icu4c/bin64/icuuc75.dll\" \"%{cfg.targetdir}\""
    }

    files { 
        "**.cpp",
        "**.hpp"
    }

    links {
        "Core",
        "Common",
		"ImGuizmo"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
		defines { "MM_APP_DEBUG" }
		runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        optimize "on"
		runtime "Release"