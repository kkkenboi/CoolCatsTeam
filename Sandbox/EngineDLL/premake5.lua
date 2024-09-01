project "EngineDLL"
    language "C++"
    cppdialect "C++20"
    warnings "Extra" -- Set warnings level to 4 for this project

    -- Engine output directory
    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "./**.cs",
        "./**.json",
        "src/**.h",
        "src/**.cpp",
    }

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

    -- Includes for any additional dependencies for this project
    includedirs
    {
        "%{wks.location}/EngineDLL/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.FreeType}",
        "%{IncludeDir.FMOD}",
        "%{IncludeDir.RapidJSON}",
        "%{IncludeDir.Mono}",
        "%{IncludeDir.FFmpeg}",
        "%{IncludeDir.RTTRLib}",
        "%{IncludeDir.RTTRLibD}"
    }
    
    libdirs
    {
        "%{wks.location}/dependencies/FMOD/core/lib/x64",
        "%{wks.location}/dependencies/FreeType/lib",
        "%{wks.location}/dependencies/Mono/lib",
        "%{wks.location}/dependencies/Mono/lib/mono/4.5/*", 
        "%{wks.location}/dependencies/FFmpeg/lib", 
        "%{wks.location}/dependencies/RTTR/lib"
    }


    filter "system:windows"
        systemversion "latest"

    filter "configurations:Editor-Release"
        kind "SharedLib" 

        -- Multi-Threaded (/MT)
        staticruntime "on"
        runtime "Release"

        optimize "On"
        architecture "x86_64"

        defines { "EDITOR_MODE" }

        links
        {
            "rttr_core.lib"
        }


    filter "configurations:Editor-Debug"
        kind "SharedLib" 

        -- Multi-Threaded Debug DLL (/MDd)
        staticruntime "off"
        runtime "Debug"

        symbols "On"
        architecture "x86_64"

        defines { "EDITOR_MODE" }

        links
        {
            "rttr_core_d.lib"
        }


    filter "configurations:Game-Release"
        kind "SharedLib" 

        -- Multi-Threaded (/MT)
        staticruntime "on"
        runtime "Release"

        optimize "On"
        architecture "x86_64"

        defines { "GAME_RELEASE" }

        links
        {
            "rttr_core.lib"
        }
