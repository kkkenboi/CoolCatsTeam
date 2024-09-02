project "Editor"
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
        "./**.h",
        "./**.cpp",
    }

    -- Includes for any additional dependencies for this project
    includedirs
    {
        "%{wks.location}/Engine/src",
        "%{wks.location}/Editor/src",
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
        "%{IncludeDir.RTTRLibD}",
        "%{IncludeDir.EnTT}"
    }

    links
    {
        "Engine"
    }
    
    libdirs
    {
        "%{wks.location}/dependencies/RTTR/lib",
        "%{wks.location}/bin/Engine"
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
        kind "None" 

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
