local global = "lswap"

workspace (global)
    configurations { "Debug", "Release" }
    platforms { "x86" }

project (global)
    kind "ConsoleApp"
	language "C++"
	location "src"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"

	files { "src/**" }

	externalincludedirs { "curl/curl/include" }
	syslibdirs { "curl/lib" }

	defines { "_CRT_SECURE_NO_WARNINGS", "CURL_STATICLIB" }
	links { "libcurl.lib" }

	buildoptions { "/std:c++latest" }

	characterset "MBCS"
	warnings "Off"
	optimize "Size"