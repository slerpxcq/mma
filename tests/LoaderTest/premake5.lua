project "LoaderTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin/%{prj.name}/%{cfg.buildcfg}"
    objdir "%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}"

    includedirs { 
        "%{wks.location}/%{IncDir.googletest}"
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

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "on"
		runtime "Debug"

    filter "configurations:Release"
        optimize "on"
		runtime "Release"







