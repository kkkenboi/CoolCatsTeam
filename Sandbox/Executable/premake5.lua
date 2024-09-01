project "Executable"
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
        "%{wks.location}/Executable/src",
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

    links
    {
        "EngineDLL"
    }

    postbuildcommands
    {
        -- "xcopy \"%{wks.location}LBEditor\\Assets\" \"$(TargetDir)Assets\" /Y /I /E",
        -- "xcopy \"%{wks.location}LBEditor\\Editor\" \"$(TargetDir)Editor\" /Y /I /E",
        -- "xcopy \"%{wks.location}LBEditor\\imgui.ini\" \"$(TargetDir)\" /Y /I /E",
        -- "xcopy \"%{wks.location}LBEditor\\CSharpAssembly.dll\" \"$(TargetDir)\" /Y /I /E",

        "{COPYFILE} \"%{wks.location}dependencies/FMOD/core/lib/x64/fmod.dll\" \"%{wks.location}bin/" .. outputDir .. "/%{prj.name}\"",
        "{COPYFILE} \"%{wks.location}dependencies/FreeType/objs/freetype.dll\" \"%{wks.location}bin/" .. outputDir .. "/%{prj.name}\"",
        "xcopy \"%{wks.location}dependencies\\Mono\\bin\\mono-2.0-sgen.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\" /y",
        "xcopy \"%{wks.location}dependencies\\Mono\\lib\\mono\\4.5\\*\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\\Library\\mono\\4.5\" /y /i /s",
        "xcopy /s /y /i \"%{wks.location}dependencies\\FFmpeg\\bin\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
        "xcopy /s /y /i \"%{wks.location}dependencies\\RTTR\\bin\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
        "xcopy /s /y /i \"%{wks.location}bin\\" .. outputDir .. "\\EngineDLL\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
    }

    
    libdirs
    {
        "%{wks.location}/dependencies/RTTR/lib",
        "%{wks.location}/bin/EngineDLL"
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

        defines { "EDITOR_MODE" }

    filter "configurations:Editor-Debug"
        kind "ConsoleApp" 

        -- Multi-Threaded Debug DLL (/MDd)
        staticruntime "off"
        runtime "Debug"

        symbols "On"
        architecture "x86_64"

        defines { "EDITOR_MODE" }

    filter "configurations:Game-Release"
        kind "ConsoleApp" 
        
        -- Multi-Threaded (/MT)
        staticruntime "on"
        runtime "Release"

        optimize "On"
        architecture "x86_64"

        defines { "GAME_RELEASE" }
