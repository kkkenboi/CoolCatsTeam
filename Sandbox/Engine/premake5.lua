project "Engine"
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

    -- Includes for any additional dependencies for this project
    includedirs
    {
        "%{wks.location}/Engine/src",
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

    postbuildcommands
    {
        -- "xcopy \"%{wks.location}LBEditor\\Assets\" \"$(TargetDir)Assets\" /Y /I /E",
        -- "xcopy \"%{wks.location}LBEditor\\Editor\" \"$(TargetDir)Editor\" /Y /I /E",
        -- "xcopy \"%{wks.location}LBEditor\\imgui.ini\" \"$(TargetDir)\" /Y /I /E",
        -- "xcopy \"%{wks.location}LBEditor\\CSharpAssembly.dll\" \"$(TargetDir)\" /Y /I /E",

        "{COPYFILE} \"%{wks.location}dependencies/FMOD/core/lib/x64/fmod.dll\" \"%{wks.location}bin/" .. outputDir .. "/Engine\"",
        "{COPYFILE} \"%{wks.location}dependencies/FreeType/objs/freetype.dll\" \"%{wks.location}bin/" .. outputDir .. "/Engine\"",
        "xcopy \"%{wks.location}dependencies\\Mono\\bin\\mono-2.0-sgen.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\Engine\" /y",
        "xcopy \"%{wks.location}dependencies\\Mono\\lib\\mono\\4.5\\*\" \"%{wks.location}bin\\" .. outputDir .. "\\Engine\\Library\\mono\\4.5\" /y /i /s",
        "xcopy /s /y /i \"%{wks.location}dependencies\\FFmpeg\\bin\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\Engine\"",
        "xcopy /s /y /i \"%{wks.location}dependencies\\RTTR\\bin\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\Engine\"",
    }

    
    libdirs
    {
        "%{wks.location}/dependencies/RTTR/lib"
    }


    filter "system:windows"
        systemversion "latest"

    filter "configurations:Editor-Release"
        kind "ConsoleApp" -- Outputs a console

        -- Multi-Threaded (/MT)
        staticruntime "on"
        runtime "Release"

        optimize "On"
        architecture "x86_64"

        defines { "_EDITOR_RELEASE" }

        links
        {
            "rttr_core.lib"
        }


    filter "configurations:Editor-Debug"
        kind "ConsoleApp" 

        -- Multi-Threaded Debug DLL (/MDd)
        staticruntime "off"
        runtime "Debug"

        symbols "On"
        architecture "x86_64"

        defines { "_EDITOR_DEBUG" }

        links
        {
            "rttr_core_d.lib"
        }


    filter "configurations:Game-Release"
        kind "ConsoleApp" 
        
        -- Multi-Threaded (/MT)
        staticruntime "on"
        runtime "Release"

        optimize "On"
        architecture "x86_64"

        defines { "_GAME_RELEASE" }

        links
        {
            "rttr_core.lib"
        }
