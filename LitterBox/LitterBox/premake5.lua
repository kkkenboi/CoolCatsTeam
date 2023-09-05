project "LitterBox"
    kind "StaticLib" -- Engine Library
    staticruntime "On"

    language "C++"
    cppdialect "C++20"
    warnings "Extra" -- Set warnings level to 4 for this project

    -- Engine output directory
    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    -- Precompiled headers for engine
    pchheader "pch.h"
    pchsource "src/pch.cpp"
    
    forceincludes { "pch.h" } -- Inserts this include for every src file

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "UNREFERENCED_PARAMETER"
    }

    -- Includes for any additional directories/dependencies for this project
    includedirs
    {
        "%{wks.location}/LitterBox/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.FreeType}"
    }

    -- Links to libraries by providing their project's name
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        -- "FreeType"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug" -- uses the debug Runtime Library
        symbols "On"
        defines
        {
            "_DEBUG"
        }    
        architecture "x86_64"

    filter "configurations:Release"
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"