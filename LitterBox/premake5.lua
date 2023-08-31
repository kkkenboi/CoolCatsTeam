-- Solutions
workspace "LitterBox"
    
    architecture "x86_64"
    startproject "Editor"        -- Set startup project
    toolset "v143"               -- Toolset v143 = Visual Studio 2022

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

-- Standard output directory
outputDir = "%{cfg.buildcfg}-%{cfg.platform}"

-- Include directories 
IncludeDir = {}
IncludeDir["GLFW"]          = "%{wks.location}/dependencies/GLFW/include"
IncludeDir["Glad"]          = "%{wks.location}/dependencies/Glad/include"
IncludeDir["ImGui"]         = "%{wks.location}/dependencies/ImGui"

-- Projects 
group "Dependencies"
    include "dependencies/GLFW"
    include "dependencies/Glad"
    include "dependencies/ImGui"
group ""

group "Engine"
    include "LitterBox"
group ""

group "Editor"
    include "Editor"
group ""

group "Sandbox"
    include "Sandbox"
group ""