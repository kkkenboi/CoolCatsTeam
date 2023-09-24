project "Editor"
    kind "ConsoleApp" -- Outputs a console
    staticruntime "on"

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

    defines
    {

    }

    -- Includes for any additional dependencies for this project
    includedirs
    {
        "%{wks.location}/LitterBox/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.FreeType}",
        "%{IncludeDir.FMOD}"

    }

    postbuildcommands
    {
        "{COPYFILE} %{wks.location}dependencies/FMOD/core/lib/x64/fmod.dll %{wks.location}bin/" .. outputDir .. "/Editor"
    }


    -- Link to our engine library
    links
    {
        "LitterBox"
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