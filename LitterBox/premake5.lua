-- Solutions
workspace "LitterBox"
    
    architecture "x86_64"
    startproject "LBEditor"        -- Set startup project
    toolset "v143"               -- Toolset v143 = Visual Studio 2022

    configurations
    {
        "Release",
        "Editor",
        "Engine",
        "Mono",
    }

    platforms
    {
        "x64",
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
IncludeDir["glm"]           = "%{wks.location}/dependencies/glm/glm"
IncludeDir["ImGui"]         = "%{wks.location}/dependencies/ImGui"
IncludeDir["spdlog"]        = "%{wks.location}/dependencies/spdlog"
IncludeDir["stb"]           = "%{wks.location}/dependencies/stb"
IncludeDir["FreeType"]      = "%{wks.location}/dependencies/FreeType/**"
IncludeDir["FMOD"]          = "%{wks.location}/dependencies/FMOD/include"
IncludeDir["RapidJSON"]     = "%{wks.location}/dependencies/RapidJSON/include"
IncludeDir["Mono"]          = "%{wks.location}/dependencies/Mono/include/mono-2.0"

-- Projects 
group "Dependencies"
-- Dependencies that are not here are header-only files
    include "dependencies/GLFW"
    include "dependencies/Glad"
    include "dependencies/ImGui"
group ""

group "Script"
    include "LBMono"
group ""

group "Engine"
    include "LBEngine"
group ""

group "Editor" 
    include "LBEditor"
group ""

group "Game"
    include "PurrfectPutt"
group ""