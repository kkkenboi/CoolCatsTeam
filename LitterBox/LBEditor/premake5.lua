project "LBEditor"
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
        "%{wks.location}/LBEngine/src",
        "%{wks.location}/LBEditor/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.FreeType}",
        "%{IncludeDir.FMOD}",
        "%{IncludeDir.RapidJSON}",
        "%{IncludeDir.Mono}"
    }

    postbuildcommands
    {
        "xcopy \"%{wks.location}LBEditor\\Assets\" \"$(TargetDir)Assets\" /Y /I /E",
        "xcopy \"%{wks.location}LBEditor\\Editor\" \"$(TargetDir)Editor\" /Y /I /E",
        "xcopy \"%{wks.location}LBEditor\\imgui.ini\" \"$(TargetDir)\" /Y /I /E",
        "xcopy \"%{wks.location}LBEditor\\CSharpAssembly.dll\" \"$(TargetDir)\" /Y /I /E",

        "{COPYFILE} \"%{wks.location}dependencies/FMOD/core/lib/x64/fmod.dll\" \"%{wks.location}bin/" .. outputDir .. "/LBEditor\"",
        "{COPYFILE} \"%{wks.location}dependencies/FreeType/objs/freetype.dll\" \"%{wks.location}bin/" .. outputDir .. "/LBEditor\"",
        "xcopy \"%{wks.location}dependencies\\Mono\\bin\\mono-2.0-sgen.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\LBEditor\" /y",
        "xcopy \"%{wks.location}dependencies\\Mono\\lib\\mono\\4.5\\*\" \"%{wks.location}bin\\" .. outputDir .. "\\LBEditor\\Library\\mono\\4.5\" /y /i /s",
    }

    -- Link to our engine library
    links
    {
        "LBEngine",
        "LBMono"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Release"
        kind "None" 
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"

    filter "configurations:Editor"
        kind "ConsoleApp" -- Outputs a console
        runtime "Debug" -- uses the debug Runtime Library
        symbols "On"
        architecture "x86_64"

    filter "configurations:Engine"
        kind "None" 
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"

    filter "configurations:Mono"
        kind "None" 
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"
