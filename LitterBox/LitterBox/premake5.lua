project "LitterBox"
    kind "StaticLib" -- Engine Library
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

    }

    -- Includes for any additional directories/dependencies for this project
    includedirs
    {
        "%{wks.location}/LitterBox/src",
        "%{wks.location}/dependencies/glm"
    }

    -- Links to libraries by providing their project's name
    links
    {

    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug" -- uses the debug Runtime Library
        symbols "On"
        architecture "x86_64"

    filter "configurations:Release"
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"