project "PMXTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "%{TargetDir}"
    objdir "%{ObjDir}"

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
        "%{IncDir.googletest}",
		"%{wks.location}/src"
    }
	
	links {
		"%{LibDir.icu4c}/*.lib"
	}
        
    files { 
        "**.cpp",
        "**.hpp"
    }

    dependson {
        "Common",
        "Core"
    }

    links {
        "gtest",
        "Common",
        "Core"
    }

    postbuildcommands {
        "xcopy /y /f \"%{wks.location}/3rdparty/icu4c/bin64/icudt75.dll\" \"%{cfg.targetdir}\"",
        "xcopy /y /f \"%{wks.location}/3rdparty/icu4c/bin64/icuin75.dll\" \"%{cfg.targetdir}\"",
        "xcopy /y /f \"%{wks.location}/3rdparty/icu4c/bin64/icutu75.dll\" \"%{cfg.targetdir}\"",
        "xcopy /y /f \"%{wks.location}/3rdparty/icu4c/bin64/icuuc75.dll\" \"%{cfg.targetdir}\""
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "on"
		runtime "Debug"

    filter "configurations:Release"
        optimize "on"
		runtime "Release"







