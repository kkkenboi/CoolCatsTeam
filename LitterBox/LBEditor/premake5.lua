project "LBEditor"
    kind "ConsoleApp" -- Outputs a console
    staticruntime "On"

    language "C++"
    cppdialect "C++20"
    warnings "Extra" -- Set warnings level to 4 for this project

    -- Engine output directory
    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    -- Includes for any additional dependencies for this project
    includedirs
    {
        "%{wks.location}/LBEngine/src",
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
        "mkdir \"$(TargetDir)Assets\"", 
        "mkdir \"$(TargetDir)Assets/Font\"", 
        "mkdir \"$(TargetDir)Assets/Audio\"",
        "mkdir \"$(TargetDir)Assets/Art\"",
        "mkdir \"$(TargetDir)Assets/Scenes\"",
        "mkdir \"$(TargetDir)Assets/Scripts\"",
        "mkdir \"$(TargetDir)Assets/Prefabs\"",
        "mkdir \"$(TargetDir)Builds\"",
        "mkdir \"$(TargetDir)Editor\"",
        "mkdir \"$(TargetDir)Editor/Lib\"",
        "mkdir \"$(TargetDir)Logs\"",
        "{COPYFILE} \"%{wks.location}dependencies/FMOD/core/lib/x64/fmod.dll\" \"%{wks.location}bin/" .. outputDir .. "/LBEditor\"",
        "xcopy \"%{wks.location}dependencies\\Mono\\bin\\mono-2.0-sgen.dll\" \"%{wks.location}bin\\" .. outputDir .. "\\LBEditor\" /y",
        "xcopy \"%{wks.location}dependencies\\Mono\\lib\\mono\\4.5\\*\" \"%{wks.location}bin\\" .. outputDir .. "\\lib\\mono\\4.5\" /y /i /s",
    }

    -- Link to our engine library
    links
    {
        "LBEngine"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug" -- uses the debug Runtime Library
        symbols "On"
        architecture "x86_64"

    filter "configurations:Release"
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"