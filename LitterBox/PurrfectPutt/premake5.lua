project "PurrfectPutt"
    staticruntime "On"

    language "C++"
    cppdialect "C++20"
    warnings "Extra" -- Set warnings level to 4 for this project

    -- Engine output directory
    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "./**.json",
        "**.cpp"
    }

    -- Includes for any additional dependencies for this project
    includedirs
    {
        "%{wks.location}/LBEngine/src",
        "%{wks.location}/PurrfectPutt",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        -- "%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.FreeType}",
        "%{IncludeDir.FMOD}",
        "%{IncludeDir.RapidJSON}",
        "%{IncludeDir.Mono}"
    }

    postbuildcommands
    {

        "xcopy \"%{wks.location}LBEditor\\Assets\" \"%{wks.location}PurrfectPutt\\Assets\" /Y /I /E",
        "xcopy \"%{wks.location}LBEditor\\Editor\" \"%{wks.location}PurrfectPutt\\Library\" /Y /I /E",
        "xcopy \"%{wks.location}PurrfectPutt\\Assets\" \"$(TargetDir)Assets\" /Y /I /E",
        "xcopy \"%{wks.location}PurrfectPutt\\Library\" \"$(TargetDir)Library\" /Y /I /E",
        "xcopy \"%{wks.location}PurrfectPutt\\CSharpAssembly.dll\" \"$(TargetDir)\" /Y /I /E",

        "{COPYFILE} \"%{wks.location}dependencies/FMOD/core/lib/x64/fmod.dll\" \"%{wks.location}bin/" .. outputDir .. "/PurrfectPutt\"",
        "{COPYFILE} \"%{wks.location}dependencies/FreeType/objs/freetype.dll\" \"%{wks.location}bin/" .. outputDir .. "/PurrfectPutt\"",
        "xcopy \"%{wks.location}dependencies\\Mono\\bin\\mono-2.0-sgen.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\PurrfectPutt\" /y",
        "xcopy \"%{wks.location}dependencies\\Mono\\lib\\mono\\4.5\\*\" \"%{wks.location}bin\\" .. outputDir .. "\\PurrfectPutt\\Library\\mono\\4.5\" /y /i /s",
    }

    -- Link to our engine library
    links
    {
        "LBEngine",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Release"
        kind "WindowedApp" 
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"

    filter "configurations:Editor"
        kind "None"
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
