project "LBMono"
    kind "SharedLib" -- Outputs a console
    staticruntime "On"
    targetextension (".dll")

    language "C#"
    warnings "Extra" -- Set warnings level to 4 for this project

    -- Engine output directory
    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "src/**.cs",
    }

    -- Include the header file to be exposed
    includedirs
    {
        "%{wks.location}/LBEngine/src",
        "%{wks.location}/LBMonoDLL/src/**",
        -- "%{IncludeDir.GLFW}",
        -- "%{IncludeDir.Glad}",
        -- "%{IncludeDir.glm}",
        -- "%{IncludeDir.ImGui}",
        -- "%{IncludeDir.spdlog}",
        -- "%{IncludeDir.stb}",
        -- "%{IncludeDir.FreeType}",
        -- "%{IncludeDir.FMOD}",
        -- "%{IncludeDir.RapidJSON}",
        -- "%{IncludeDir.Mono}"

    }

    postbuildcommands
    {
        -- "{COPYFILE} \"%{wks.location}LBMonoDLL/CSharpAssembly.dll\" \"%{wks.location}bin/" .. outputDir .. "/LBMonoDLL\"",
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Release"
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"

    filter "configurations:Editor"
        runtime "Debug" -- uses the debug Runtime Library
        symbols "On"
        architecture "x86_64"

    filter "configurations:Sandbox"
        runtime "Debug" -- uses the debug Runtime Library
        symbols "On"
        architecture "x86_64"

    filter "configurations:Engine"
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"

    filter "configurations:Mono"
        runtime "Release" -- uses the release Runtime Library
        optimize "On"
        architecture "x86_64"
