project "Mono"
    staticruntime "On"

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
        "%{wks.location}/Mono/src",
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
        "%{IncludeDir.FFmpeg}"
    }

    postbuildcommands
    {
        -- "xcopy \"%{wks.location}LBEditor\\Assets\" \"$(TargetDir)Assets\" /Y /I /E",
        -- "xcopy \"%{wks.location}LBEditor\\Editor\" \"$(TargetDir)Editor\" /Y /I /E",
        -- "xcopy \"%{wks.location}LBEditor\\imgui.ini\" \"$(TargetDir)\" /Y /I /E",
        -- "xcopy \"%{wks.location}LBEditor\\CSharpAssembly.dll\" \"$(TargetDir)\" /Y /I /E",

        "{COPYFILE} \"%{wks.location}dependencies/FMOD/core/lib/x64/fmod.dll\" \"%{wks.location}bin/" .. outputDir .. "/Mono\"",
        "{COPYFILE} \"%{wks.location}dependencies/FreeType/objs/freetype.dll\" \"%{wks.location}bin/" .. outputDir .. "/Mono\"",
        "xcopy \"%{wks.location}dependencies\\Mono\\bin\\mono-2.0-sgen.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\Mono\" /y",
        "xcopy \"%{wks.location}dependencies\\Mono\\lib\\mono\\4.5\\*\" \"%{wks.location}bin\\" .. outputDir .. "\\Mono\\Library\\mono\\4.5\" /y /i /s",
        "xcopy /s /y /i \"%{wks.location}dependencies\\FFmpeg\\bin\\*.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\Mono\"",
    }

    -- Under references, this checks the project dependencies
    links
    {
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:RTTR"
        kind "ConsoleApp" -- Outputs a console
        runtime "Debug" -- uses the debug Runtime Library
        defines { "_MEMORY" }
        symbols "On"
        architecture "x86_64"

    filter "configurations:Mono"
        kind "ConsoleApp" -- Outputs a console
        runtime "Debug" -- uses the debug Runtime Library
        defines { "_MEMORY" }
        symbols "On"
        architecture "x86_64"
