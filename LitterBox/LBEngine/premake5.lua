project "LBEngine"
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
        "src/LBEngine/Renderer/**.shader", 
    }

    -- Includes for any additional directories/dependencies for this project
    includedirs
    {
        "%{wks.location}/LBEngine/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        --"%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.FreeType}",
        "%{IncludeDir.FMOD}",
        "%{IncludeDir.RapidJSON}",
        "%{IncludeDir.Mono}",
        "%{IncludeDir.FFmpeg}"
    }

    libdirs
    {
        "%{wks.location}/dependencies/FMOD/core/lib/x64",
        "%{wks.location}/dependencies/FreeType/lib",
        "%{wks.location}/dependencies/Mono/lib",
        "%{wks.location}/dependencies/Mono/lib/mono/4.5/*", 
        "%{wks.location}/dependencies/FFmpeg/lib", 
    }

    -- Links to libraries by providing their project's name
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "fmod_vc",
        "mono-2.0-sgen",
        "freetype",
        "avutil.lib",
        "avcodec.lib",
        "avformat.lib",
        "swscale.lib"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Release"
        kind "StaticLib" 
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"

    filter "configurations:Editor"
        kind "StaticLib"
        runtime "Debug" -- uses the debug Runtime Library
        defines { "_MEMORY" }
        symbols "On"
        architecture "x86_64"

    filter "configurations:Sandbox"
        kind "StaticLib"
        runtime "Debug" -- uses the debug Runtime Library
        defines { "_MEMORY" }
        symbols "On"
        architecture "x86_64"

    filter "configurations:Engine"
        kind "StaticLib" -- Engine Library
        runtime "Release" -- uses the release Runtime Library
        defines { "_MEMORY" }
        optimize "On"
        architecture "x86_64"

    filter "configurations:Mono"
        kind "None" -- Outputs a console
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"
