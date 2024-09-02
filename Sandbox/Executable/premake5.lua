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
        "%{wks.location}/Editor/src",
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
    
    libdirs
    {
        "%{wks.location}/dependencies/RTTR/lib",
        "%{wks.location}/bin/Engine",
        "%{wks.location}/bin/Editor"
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
        
        links
        {
            "Editor",
            "Engine"
        }    

    filter "configurations:Editor-Debug"
        kind "ConsoleApp" 

        -- Multi-Threaded Debug DLL (/MDd)
        staticruntime "off"
        runtime "Debug"

        symbols "On"
        architecture "x86_64"

        defines { "EDITOR_MODE" }
        
        links
        {
            "Editor",
            "Engine"
        }    
    
    filter "configurations:Editor-Release or configurations:Editor-Debug"
        -- This post build should have both Engine and Editor DLL
        postbuildcommands
        {    
            "{COPYFILE} \"%{wks.location}dependencies/FMOD/core/lib/x64/fmod.dll\" \"%{wks.location}bin/" .. outputDir .. "/%{prj.name}\"",
            "{COPYFILE} \"%{wks.location}dependencies/FreeType/objs/freetype.dll\" \"%{wks.location}bin/" .. outputDir .. "/%{prj.name}\"",
            "xcopy \"%{wks.location}dependencies\\Mono\\bin\\mono-2.0-sgen.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\" /y",
            "xcopy \"%{wks.location}dependencies\\Mono\\lib\\mono\\4.5\\*\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\\Library\\mono\\4.5\" /y /i /s",
            "xcopy /s /y /i \"%{wks.location}dependencies\\FFmpeg\\bin\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
            "xcopy /s /y /i \"%{wks.location}dependencies\\RTTR\\bin\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
            "xcopy /s /y /i \"%{wks.location}bin\\" .. outputDir .. "\\Engine\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
            "xcopy /s /y /i \"%{wks.location}bin\\" .. outputDir .. "\\Editor\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
        }


    filter "configurations:Game-Release"
        kind "ConsoleApp" 
        
        -- Multi-Threaded (/MT)
        staticruntime "on"
        runtime "Release"

        optimize "On"
        architecture "x86_64"

        defines { "GAME_RELEASE" }

        links
        {
            "Engine"
        }    

        -- This post build should have only the Engine DLL
        postbuildcommands
        {    
            "{COPYFILE} \"%{wks.location}dependencies/FMOD/core/lib/x64/fmod.dll\" \"%{wks.location}bin/" .. outputDir .. "/%{prj.name}\"",
            "{COPYFILE} \"%{wks.location}dependencies/FreeType/objs/freetype.dll\" \"%{wks.location}bin/" .. outputDir .. "/%{prj.name}\"",
            "xcopy \"%{wks.location}dependencies\\Mono\\bin\\mono-2.0-sgen.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\" /y",
            "xcopy \"%{wks.location}dependencies\\Mono\\lib\\mono\\4.5\\*\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\\Library\\mono\\4.5\" /y /i /s",
            "xcopy /s /y /i \"%{wks.location}dependencies\\FFmpeg\\bin\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
            "xcopy /s /y /i \"%{wks.location}dependencies\\RTTR\\bin\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
            "xcopy /s /y /i \"%{wks.location}bin\\" .. outputDir .. "\\Engine\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\%{prj.name}\"",
        }
    

