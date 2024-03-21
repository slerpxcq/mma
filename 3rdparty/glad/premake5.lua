project "glad"
	kind "StaticLib"
	language "C"

    targetdir "%{wks.location}/bin/%{prj.name}/%{cfg.buildcfg}"
    objdir "%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}"

    files {
        "src/*.c",
    }

	includedirs {
		"include"
	}

    filter "system:windows"
        systemversion "latest"
        defines {
			"_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        symbols "on"

    filter "configurations:Release"
        optimize "on"

