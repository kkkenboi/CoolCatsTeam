-- Solutions
workspace "LitterBox"
    
    architecture "x86_64"
    startproject "Editor"     -- Set startup project
    toolset "v143"          -- Toolset v143 = Visual Studio 2022

    configurations
    {
        "Debug",
        "Release",
    }

    platforms
    {
        "Editor",
        "Executable",
    }

    flags
    {
        "MultiProcessorCompile", -- Enable Multicore Compilation
    }

outputDir = "%{cfg.buildcfg}-%{cfg.platform}" -- Only working on Windows

-- Projects 
group "Dependencies"
    -- Add submodules project here
group ""

group "Engine"
    include "LitterBox"
group ""

group "Sandbox"
    -- Add testing environment here
group ""